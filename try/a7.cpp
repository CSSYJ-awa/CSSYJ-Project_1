#include <iostream>
using namespace std;
int a, b;
int *p;
int main()
{
    cin >> a >> b;
    if (a > b)
        p = &a;
    else
        p = &b;
    *p += 10;
    cout << *p << endl;
    cout << p << endl;
    cout << &p << endl;
    cout << a << endl;
    cout << b << endl;
    cout << &b << endl;
    return 0;
}
