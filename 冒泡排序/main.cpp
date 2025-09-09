#include <iostream>

/*时间复杂度；最好情况（原数组有序）：O(n)
最坏情况（原数组逆序）：O(n²)
平均情况：O(n²)
*/
void BubbleSort(int* arr, int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - 1 - i; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main()
{
    int arr[] = {5, 2, 9, 3, 7, 1, 8, 4, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    BubbleSort(arr, n);

    std::cout << "排序结果: ";
    for (int i = 0; i < n; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    return 0;
}