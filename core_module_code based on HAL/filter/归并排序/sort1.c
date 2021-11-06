#include "stdio.h"
int tmp[100] = {0};

void merge_sort(int q[], int l, int r)
{
    if (l >= r)
        return;
    int mid = l + r >> 1;
    merge_sort(q, l, mid);
    merge_sort(q, mid + 1, r);
    int k = 0, i = l, j = mid + 1;
    while (i <= mid && j <= r)
        if (q[i] < q[j])
            tmp[k++] = q[i++];
        else
            tmp[k++] = q[j++];
    while (i <= mid)
        tmp[k++] = q[i++];
    while (j <= r)
        tmp[k++] = q[j++];
    for (i = l, j = 0; i <= r; i++, j++)
        q[i] = tmp[j];
}

void main()
{
    int q[] = {12,32, 43,5564, 654,1, 54, 5, 2334, 232, 55, 3424, 4, 56, 7};
    merge_sort(q, 0, 14);
    for(int i = 0; i < 14; i++)
        printf("%d ", q[i]);
}