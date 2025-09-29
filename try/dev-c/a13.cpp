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

    for (int i = 0; i < s.size(); i++)
    {
        int c = 0;
        for (int j = i + 1; j < s.size(); j++)
            if (s[i] == s[j])
            {
                c++;
                break;
            }
        if (c == 0)
        {
            cout << s[i];
            return 0;
        }
    }
    cout << "no" << endl;
    return 0;
}