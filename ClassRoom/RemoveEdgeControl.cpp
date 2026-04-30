// RemoveEdgeControl.cpp
// 运行后会：
//  - 从 HKCU Run 中删除 EdgeControl 启动项
//  - 终止正在运行的安装位置的 EdgeControl 进程

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cwctype>

std::wstring get_install_path() {
    wchar_t* appdata = nullptr;
    size_t len = 0;
    _wdupenv_s(&appdata, &len, L"APPDATA");
    std::wstring path;
    if (appdata) {
        path = appdata;
        free(appdata);
    } else {
        wchar_t userprofile[MAX_PATH];
        if (GetEnvironmentVariableW(L"USERPROFILE", userprofile, MAX_PATH)) path = userprofile;
    }
    if (!path.empty() && path.back() != L'\\') path.push_back(L'\\');
    path += L"EdgeControl\\EdgeControl.exe";
    return path;
}

bool delete_run_key() {
    LSTATUS st = RegDeleteKeyValueW(HKEY_CURRENT_USER,
                                     L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                                     L"EdgeControl");
    return (st == ERROR_SUCCESS || st == ERROR_FILE_NOT_FOUND);
}

std::vector<DWORD> find_pids_by_image_path(const std::wstring& targetPath) {
    std::vector<DWORD> pids;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return pids;
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            DWORD pid = pe.th32ProcessID;
            // 首先按进程名匹配（EdgeControl.exe），避免无法获取路径时漏掉目标进程
            std::wstring pname = pe.szExeFile;
            std::transform(pname.begin(), pname.end(), pname.begin(), towlower);
            std::wstring targetName = L"edgecontrol.exe";
            if (pname == targetName) {
                pids.push_back(pid);
                continue;
            }
            // 否则尝试按完整路径匹配
            HANDLE ph = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, FALSE, pid);
            if (ph) {
                wchar_t exePath[MAX_PATH] = {0};
                DWORD size = MAX_PATH;
                if (QueryFullProcessImageNameW(ph, 0, exePath, &size)) {
                    std::wstring p(exePath);
                    // 忽略大小写比较
                    for (auto &c : p) c = towlower(c);
                    std::wstring t = targetPath;
                    for (auto &c : t) c = towlower(c);
                    if (p == t) pids.push_back(pid);
                }
                CloseHandle(ph);
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return pids;
}

void terminate_pids(const std::vector<DWORD>& pids) {
    for (DWORD pid : pids) {
        HANDLE h = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!h) {
            // try with broader access
            h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        }
        if (h) {
            TerminateProcess(h, 0);
            CloseHandle(h);
        }
    }
}

static bool enable_debug_privilege() {
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) return false;
    LUID luid;
    if (!LookupPrivilegeValueW(NULL, L"SeDebugPrivilege", &luid)) { CloseHandle(hToken); return false; }
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    bool ok = (GetLastError() == ERROR_SUCCESS);
    CloseHandle(hToken);
    return ok;
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    // 隐藏控制台窗口（如果以控制台方式启动）
    HWND cw = GetConsoleWindow();
    if (cw) ShowWindow(cw, SW_HIDE);
    // 尝试提升权限以便能终止其他进程
    (void)enable_debug_privilege();

    std::wstring install = get_install_path();

    bool regOk = delete_run_key();
    (void)regOk;

    auto pids = find_pids_by_image_path(install);
    if (!pids.empty()) {
        terminate_pids(pids);
    }

    // 不删除可执行文件：只移除启动项并终止运行实例
    return 0;
}
