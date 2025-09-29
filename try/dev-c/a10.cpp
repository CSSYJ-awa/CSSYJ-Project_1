#include <iostream>
#include <string>
using namespace std;
int main()
{
    string s, temp;
    cin >> s;
    while (1)
    {
        cin >> temp;
        s += ' ' + temp;
        if (temp[temp.size()-1] == '.')
            break;
    }
    cout << s << endl;
    return 0;
}