#include <iostream>
using namespace std;

double cabs(double x)
{
    if (x > 0)
        return x;
    else
        return -x;
}
int main()
{
    double x1, x2, y1, y2;
    double dx, dy, mht;
    cin >> x1 >> y1 >> x2 >> y2;
    mht = cabs(x1 - x2) + cabs(y1 - y2);
    cout << mht << endl;
    return 0;
}