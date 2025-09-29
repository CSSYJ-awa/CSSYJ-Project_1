#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int main()
{
    int a;
    cout << "输入题数:";
    cin >> a;
    cout << "共" << a << "题\n";
    for (int i = 1; i <= a; i++)
    {
        int x, y, n, num = 0;
        srand(time(NULL));
        x = 100 + rand() % 900;
        y = 100 + rand() % 900;
        cout << "第" << i << "题：";
        do
        {
            cout << x << "+" << y << "=?\n";
            cin >> n;
            num++;
        } while (n != x + y);
        cout << "用了" << num << "次" << endl;
    }
    return 0;
}