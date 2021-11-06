#include "stdio.h"

void quick_sort(int q[], int l, int r)
{
    if (l >= r)
        return;
    int i = l - 1, j = r + 1, x = q[l];
    while (i < j)
    {
        do
            i++;
        while (q[i] < x);
        do
            j--;
        while (q[j] > x);
        if (i < j)
        {
            q[i] = q[i] ^ q[j];
            q[j] = q[i] ^ q[j];
            q[i] = q[i] ^ q[j];
        }
        else
            break;
    }
    quick_sort(q, l, j), quick_sort(q, j + 1, r);
}

void main()
{
    int a[] = {1, 2, 4 ,3, 34, 4535, 5, 6, 4435, 7, 32};
    int l = 0, r = 10;//就是长度
    quick_sort(a, l, r);
    for(int i = 0; i < 11; i++)
        printf("%d ", a[i]);
}