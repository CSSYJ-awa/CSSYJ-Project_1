#include <iostream>
#include <string>
using namespace std;

int main()
{
    int line;
    cin>>line;
    // 初始化计数数组
    int count[26] = {0};
    for (int a = 0; a < 2; a++)
    {
        string s;
        getline(cin, s);
        // 使用传统 for 循环统计字符频率
        for (size_t i = 0; i < s.length(); i++)
        {
            char c = s[i];
            count[c - 'a']++;
        }
    }
    int count_max = 0;
    for (int a = 0; a < 26; a++)
        if (count[a] > count_max)
            count_max = count[a];

    // 使用传统 for 循环查找第一个唯一字符
    for (int a = 0; a < count_max; a++)
    // count[a]
    // count[b]
    {
        for (int b = 0; b < 26; b++)
        {
            if (count[b] >= count_max - a)
                cout << "*";
            else
                cout << " ";
            cout << " ";
        }
        cout << endl;
    }
    for(int a=0;a<26;a++)
    {
        cout<<char(97+a);
        cout<<" ";
    }
    return 0;
}
