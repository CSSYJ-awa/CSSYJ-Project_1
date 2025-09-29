#include <iostream>
#include <cstring>

int main()
{
    // 定义一个整型数组
    int arr[10];

    // 使用 memset 将数组的每个字节设置为 0
    // 注意：memset 按字节操作，对于非字符类型需谨慎使用
    std::memset(arr, 0, sizeof(arr));

    // 输出数组元素，验证是否都被初始化为 0
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }

    // 使用 memset 将数组的每个字节设置为 1
    // 对于 int 类型，每个字节设为 1 后，整个 int 值不为 1
    std::memset(arr, 1, sizeof(arr));

    // 输出数组元素，观察设置为 1 后的结果
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }

    return 0;
}