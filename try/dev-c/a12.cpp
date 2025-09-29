#include <iostream>
#include <string>
using namespace std;
int main()
{
    string s, temp, n;
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
        if (n.find(s[i]) == -1)
        {
            for (j = i + 1; j < s.size(); j++)
                if (s[i] == s[j])
                    n+=s[i];
            break;
            if (j == s.size())
            {
                cout << s[i];
                return 0;
            }
        }
    }
    cout << "no" << endl;
    return 0;
}