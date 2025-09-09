#include<iostream>
void Swap(int* tmp1, int* tmp2)
{
	int tmp = *tmp1;
	*tmp1 = *tmp2;
	*tmp2 = tmp;
}

void PartSort(int* a, int left, int right)
{
	//递归终止条件
	if (left >= right)
	{
		return;
	}
	//单次快排
	int begin = left, end = right;
    //取基准值
	int keyi = begin;
	while (begin < end)
	{
		//从右指针开始向左找比基准值小的数
		while (begin < end && a[end] >= a[keyi])
		{
			end--;
		}
		//从左指针开始向右找比基准值大的数
		while (begin < end && a[begin] <= a[keyi])
		{
			begin++;
		}
		//交换两个指针找到的数
		Swap(&a[begin], &a[end]);
	}
	//最后交换，将基准值换到两个指针的位置
	Swap(&a[begin], &a[keyi]);

	//递归放好位置的基准值左右边即可
	PartSort(a, left, begin);
	PartSort(a, begin + 1, right);
}