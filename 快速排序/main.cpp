#include<iostream>
//挖坑法
void QuickSort(int* a, int left, int right)
{
// 假设按照升序对array数组中[left, right)区间中的元素进行排序
	if (right <= left)
		return;
// 按照基准值对array数组的 [left, right)区间中的元素进行划分
	int keyi = PartSort(a, left, right);
// 划分成功后以div为边界形成了左右两部分 [left, keyi-1) 和 [keyi+1, right)
// 递归排[left, keyi-1)
	QuickSort(a, left, keyi - 1);
// 递归排[keyi+1, right)
	QuickSort(a, keyi + 1, right);
}

int PartSort(int* a, int left, int right)
{
    int p = a[left]; // 选第一个元素为基准
    int i = left, j = right;
    while (i < j)
    {
        // 从右往左找比基准小的
        while (i < j && a[j] >= p) j--;
        //找到后，把 a[j] 放到 a[i] 位置，并且 i++，相当于“挖坑填数”
        if (i < j)
         a[i++] = a[j];
        // 从左往右找比基准大的
        while (i < j && a[i] <= p) i++;
        if (i < j)
         a[j--] = a[i];
    }
    a[i] = p; // 基准归位
    return i;     // 返回基准位置
}

int main()
{
    int arr[]= {5, 2, 9, 3, 7, 1, 8, 4, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    QuickSort(arr, 0, n - 1);

    std::cout << "排序结果: ";
    for (int i = 0; i < n; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    return 0;
}