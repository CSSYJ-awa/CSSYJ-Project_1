#include <iostream>
using namespace std;
int num = 0;
int main()
{
    for (int a = 1; a <= 7; a++)
        for (int b = 1; b <= 15; b++)
            for (int c = 1; c <= 15; c++)
                if (a + b + c == 15 && 10 * a + 5 * b + c == 70)
                {
                    num += 1;
                    cout << a << " " << b << " " << c << " " << endl;
                }
    cout << num << endl;
    return 0;
}