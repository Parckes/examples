#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef int elem;

void HP (int n, int hp[])
{
    for (int i = 0; i < n; ++i)
        scanf ("%d", &hp[i]);
    return;
}
void attack (int hp[], int n, int diff)
{
    int d;
    hp[n-1] -= diff;
    while (hp[n-1] < hp[n-2])
    {
        d = hp[n-1];
        hp[n-1] = hp[n-2];
        hp[n-2] = d;
        --n;
    }

    return;
}

void merge_sort_impl (elem* a, int size, elem* buff)
    {
    if (size <= 1)
    {
        return;
    }
    int lsize = size / 2;
    int rsize = size - lsize;

    merge_sort_impl(a, lsize, buff);
    merge_sort_impl(a + lsize, rsize, buff);

    int L = 0, R = lsize;
    int k = 0;
    while (L < lsize && R < size)
    {
        if (a[L] < a[R]) {
            buff[k++] = a[L++];
        } else {
            buff[k++] = a[R++];
        }
    }
    while (L < lsize)
    {
        buff[k++] = a[L++];
    }
    while (R < size)
    {
        buff[k++] = a[R++];
    }
    memcpy(a, buff, size * sizeof(elem));
}


void merge_sort(elem* a, int size)
    {
     if (size < 10000 / sizeof(elem))
        {
        elem buff[size];
        merge_sort_impl(a, size, buff);
        }
     else
        {
        elem* buff = calloc (size, sizeof(elem));
        merge_sort_impl (a, size, buff);
        free (buff);
        }
}


int main ()
{
    int n, q, p;
    scanf ("%d %d %d ",&n, &p, &q);


    int* hp;
    hp = (int*) calloc(n, sizeof (int));


    HP (n, hp);

    int diff = p - q;
    int s = 0;                            // кол-во ударов
    int gm = 0;                           // global minus, сколько очков здоровья вычтено у всех монстров

    int hp_size = n;
    merge_sort(hp, hp_size);

    while ((hp[n-1] - gm) > 0)
    {
        gm += q;
        attack (hp, n, diff);
        ++s;
    }
    printf ("%d",s);

    free (hp);
    return 0;

}
