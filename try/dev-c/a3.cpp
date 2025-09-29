#include <iostream>
#include <cstring>
using namespace std;
#define MAXN 1000010
int prime[MAXN], a[MAXN];
int main()
{
    int n, i, j;
    cin >> n;
    int num = 0;
    memset(a, 0, sizeof(a));
    for (i = 2; i < n; i++)
    {
        if (!a[i])
            prime[num++] = i;
        for (j = 0; j < num && i * prime[j] < n; j++)
        {
            a[prime[j] * i] = 1;
            if (!i % prime[j])
                break;
        }
    }
    for (i = 0; i < num; i++)
        cout << prime[i] << " ";
    return 0;
}
