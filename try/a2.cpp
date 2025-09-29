#include <iostream>
#include <cmath>
#define MAXN 1000010
bool a[MAXN];
int prime[MAXN];
using namespace std;
int main()
{
    int n, m;
    cin >> n;
    for (int i = 2; i <= sqrt(n) + 1; i++)
        if (a[i] == 0)
            for (int j = 2; i * j <= n; j++)
                a[i * j] = 1;
    m = 0;
    for (int i = 2; i <= n; i++)
        if (a[i] == 0)
            prime[m++] = i;
    for (int i = 0; i < m; i++)
        cout << prime[i] << " ";
    return 0;
}
