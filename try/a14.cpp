#include <iostream>
#include <string>
using namespace std;
int main()
{
    string s;
    // cin >> s;
    s = "asdfgasdg";
    for (int k = 0; k < 10; k++)
    {
        s += s;
    }
    s = s + "q" + s;
    int j;
    for (int i = 0; i < s.size(); i++)
    {
        for (j = i + 1; j < s.size(); j++)
            if (s[i] == s[j])
                break;
        if (j == s.size())
        {
            cout << s[i];
            return 0;
        }
    }
    cout << "no" << endl;
    return 0;
}