#include <iostream>
using namespace std;
int main()
{
    float num1, num2;          // 存储两个操作数
    char op;                   // 存储运算符
    cin >> num1 >> num2 >> op; // 连续输入两个数字和一个运算符
    switch (op)
    {
    case '+':
        cout << num1 + num2;
        break; // 加法
    case '-':
        cout << num1 - num2; // 减法
        break;
    case '*':
        cout << num1 * num2; // 乘法
        break;
    case '/':
        if (num2)
            cout << num1 / num2;
        else
            cout << "Divided by zero!"; // 除数为0处理
        break;                          // 除法
    default:
        cout << "Invalid operator!";
        break; // 无效运算符处理
    }
}