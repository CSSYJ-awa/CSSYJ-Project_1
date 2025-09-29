#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cin >> s;
    
    // 初始化计数数组
    int count[26] = {0};
    
    // 使用传统 for 循环统计字符频率
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        count[c - 'a']++;
    }
    
    // 使用传统 for 循环查找第一个唯一字符
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (count[c - 'a'] == 1) {
            cout << c << endl;
            return 0;
        }
    }
    
    cout << "no" << endl;
    return 0;
}
