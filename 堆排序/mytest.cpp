#include<iostream>
void Swap(int* tmp1, int* tmp2)
{
	int tmp = *tmp1;
	*tmp1 = *tmp2;
	*tmp2 = tmp;
}

int AdjustDown(int*a,int n,int parent)
{
    int child=parent*2+1;
    while(child<n)
    {
        if(child+1<n&&a[child]<a[child+1])
        child=child+1;
        
        if(a[child]>a[parent])
        {
            Swap(&a[child],&a[parent]);
            parent=child;
            child = parent * 2 + 1; // 更新孩子位置
        }
        else{
            break;
        }
    }
}

void HeapSort(int* a, int n)
{
    for(int i=(n-2)/2;i>=0;i--)
    AdjustDown(a,n,i);

    int end=n-1;
    while(end>0)
    {
        Swap(&a[0],&a[end]);
        AdjustDown(a,end,0);
        end--;
    }
}