#include <iostream>
using namespace std;
int main()
{
    //ASCII[1:49 a:97 A:65]
    int i = 97;
    for (int a = 0; a < 26; a++)
    {
        cout << char(i + a);
    }
    return 0;
}