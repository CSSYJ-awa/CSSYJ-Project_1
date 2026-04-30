// EdgeControl.cpp
// 监控是否插入标签为 LZHISAGAY 的移动存储设备，
// 若存在则每秒检测并关闭 Edge 浏览器（先优雅关闭，再强制结束）。

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

static const std::wstring TARGET_LABEL = L"LZHISAGAY";
static const int POLL_MS = 1000;

// 安装路径：%APPDATA%\EdgeControl\EdgeControl.exe
std::wstring get_install_path() {
    wchar_t* appdata = nullptr;
    size_t len = 0;
    _wdupenv_s(&appdata, &len, L"APPDATA");
    std::wstring path;
    if (appdata) {
        path = appdata;
        free(appdata);
    } else {
        // 回退到用户配置目录
        wchar_t userprofile[MAX_PATH];
        if (GetEnvironmentVariableW(L"USERPROFILE", userprofile, MAX_PATH)) path = userprofile;
    }
    if (!path.empty() && path.back() != L'\\') path.push_back(L'\\');
    path += L"EdgeControl\\EdgeControl.exe";
    return path;
}

bool ensure_installed_and_restarted_hidden() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring cur(exePath);
    std::wstring dest = get_install_path();
    // If already running from dest, nothing to do
    if (_wcsicmp(cur.c_str(), dest.c_str()) == 0) return false;

    // Create target dir
    size_t pos = dest.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        std::wstring dir = dest.substr(0, pos);
        CreateDirectoryW(dir.c_str(), NULL);
    }

    // Copy executable (overwrite)
    CopyFileW(cur.c_str(), dest.c_str(), FALSE);

    // Write HKCU Run key
    RegSetKeyValueW(HKEY_CURRENT_USER,
                    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                    L"EdgeControl",
                    REG_SZ,
                    (const BYTE*)dest.c_str(),
                    (DWORD)((dest.size()+1) * sizeof(wchar_t)));

    // Launch the copied exe hidden
    std::wstring cmd = L"\"" + dest + L"\"";
    std::vector<wchar_t> cmdbuf(cmd.begin(), cmd.end());
    cmdbuf.push_back(0);

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcessW(NULL, cmdbuf.data(), NULL, NULL, FALSE,
                             CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (ok) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    // Exit current process so the installed copy runs alone
    ExitProcess(0);
    return true;
}

bool has_target_removable() {
    DWORD drives = GetLogicalDrives();
    for (wchar_t d = L'A'; d <= L'Z'; ++d) {
        if (!(drives & (1u << (d - L'A')))) continue;
        wchar_t root[4] = {d, L':', L'\\', L'\0'};
        UINT type = GetDriveTypeW(root);
        if (type != DRIVE_REMOVABLE && type != DRIVE_FIXED) continue; // 一般 USB 是 REMOVABLE
        wchar_t label[261] = {0};
        if (GetVolumeInformationW(root, label, _countof(label), nullptr, nullptr, nullptr, nullptr, 0)) {
            std::wstring vol(label);
            // 忽略大小写比较
            std::transform(vol.begin(), vol.end(), vol.begin(), ::towlower);
            std::wstring target = TARGET_LABEL;
            std::transform(target.begin(), target.end(), target.begin(), ::towlower);
            if (vol == target) return true;
        }
    }
    return false;
}

std::vector<DWORD> find_edge_pids() {
    std::vector<DWORD> pids;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return pids;
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            std::wstring name = pe.szExeFile;
            std::wstring lname = name;
            std::transform(lname.begin(), lname.end(), lname.begin(), ::towlower);
            if (lname == L"msedge.exe" || lname == L"microsoftedge.exe") {
                pids.push_back(pe.th32ProcessID);
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return pids;
}

// EnumWindows callback to close windows that belong to a given PID
struct CloseContext { DWORD pid; bool closedAny; };

BOOL CALLBACK EnumForPid(HWND hwnd, LPARAM lParam) {
    CloseContext* ctx = reinterpret_cast<CloseContext*>(lParam);
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid != ctx->pid) return TRUE;
    // Only top-level visible windows
    if (!IsWindowVisible(hwnd)) return TRUE;
    // Post WM_CLOSE for graceful shutdown
    PostMessageW(hwnd, WM_CLOSE, 0, 0);
    ctx->closedAny = true;
    return TRUE;
}

void force_kill_pid(DWORD pid) {
    HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (h) {
        TerminateProcess(h, 1);
        CloseHandle(h);
    }
}

void attempt_close_edges() {
    auto pids = find_edge_pids();
    if (pids.empty()) return;
    // First try graceful close by sending WM_CLOSE to windows of those pids
    for (DWORD pid : pids) {
        CloseContext ctx{pid, false};
        EnumWindows(EnumForPid, reinterpret_cast<LPARAM>(&ctx));
    }
    // wait briefly to let processes exit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // Re-check remaining edge pids and force kill if still present
    auto remaining = find_edge_pids();
    for (DWORD pid : remaining) {
        force_kill_pid(pid);
    }
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    // 隐藏控制台窗口（如果以控制台方式启动）
    HWND cw = GetConsoleWindow();
    if (cw) ShowWindow(cw, SW_HIDE);

    while (true) {
        bool present = has_target_removable();
        if (present) {
            attempt_close_edges();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(POLL_MS));
    }

    return 0;
}
