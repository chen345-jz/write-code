//堆排序
#include<iostream>

void Swap(int* tmp1, int* tmp2)
{
	int tmp = *tmp1;
	*tmp1 = *tmp2;
	*tmp2 = tmp;
}

// 向下调整堆，使以 parent 为根的子树满足大堆性质
void AdjustDown(int* a, int n, int parent)
{
    int child = parent * 2 + 1; // 找左孩子
    while (child < n)
    {
        // 如果右孩子存在且比左孩子大，选右孩子
        if (child + 1 < n && a[child + 1] > a[child])
        {
            child++;
        }
        // 如果孩子比父节点大，交换，并继续向下调整
        if (a[child] > a[parent])
        {
            Swap(&a[child], &a[parent]);
            parent = child; // 继续向下调整
            child = parent * 2 + 1; // 更新孩子位置
        }
        else
            break; // 已满足堆性质，结束调整
    }
}
void HeapSort(int* a, int n)
{
    // 1. 建堆：从最后一个非叶子节点开始，向前调整每个非叶子节点，形成大根堆
    for (int i = (n - 2) / 2; i >= 0; i--)
    {
        AdjustDown(a, n, i);
    }
    int end = n - 1;
    // 2. 排序：每次把堆顶（最大值）和末尾元素交换，然后调整剩下的部分为大根堆
    while (end > 0)
    {
        Swap(&a[0], &a[end]);      // 堆顶和末尾交换
        AdjustDown(a, end, 0);     // 剩下的部分重新调整为大根堆
        end--;                     // 缩小堆的范围
    }
}

int main()
{
    int arr[] = {5, 2, 9, 3, 7, 1, 8, 4, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    HeapSort(arr, n);

    std::cout << "排序结果: ";
    for (int i = 0; i < n; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    return 0;
}