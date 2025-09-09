#include<iostream>
void Swap(int* tmp1,int *tmp2)
{
    int temp=*tmp1;
    *tmp1=*tmp2;
    *tmp2=temp;
}
void PartSort(int *a,int left,int right)
{
    if(left>=right)
    {
        return;
    }

    int begin=left;
    int end=right;
    int keyi=begin;
    while(begin<end)
    {
        while(begin<right&&a[end]>=a[keyi])
        end--;
        while(begin<right&&a[begin]<=a[keyi])
        begin++;
        Swap(&a[begin],&a[end]);
    }
    Swap(&a[begin], &a[keyi]);

    PartSort(a,left,begin);
    PartSort(a, begin + 1, right);
}

//平均时间复杂度：O(n log n)
//最坏时间复杂度：O(n²)
//最好时间复杂度：O(n log n)