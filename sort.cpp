#include <cstdio>
#include <cinttypes>
#include <cstring>
using namespace std;

#define MAX_STRING 200

template <class T>
void swap (T a, T b)
{
    T t = a;
    a = b;
    b = t;
}

template <class T>
T max (T a, T b)
{
    return a > b ? a : b;
}

template <class T>
int64_t bin_search (T* a, int64_t beg, int64_t end, T x)
{
    if (a[end] < x)
        return -1;
    int64_t l = 0;
    int64_t r = (end - beg) + 1;
    while ((r - l) > 1)
    {
        if (a[beg + (r + l) / 2 - 1] >= x)
            r = (r + l) / 2;
        else
            l = (r + l) / 2;
    }
    return beg + r - 1;
}

struct Student
{
    char name[MAX_STRING];
    char surname[MAX_STRING];
    int64_t inf;
    int64_t math;
    int64_t rus;
    int64_t ind;

    bool operator< (const Student &r)
    {
        if (inf + math + rus < r.inf + r.math + r.rus)
            return true;
        else if (inf + math + rus == r.inf + r.math + r.rus && ind > r.ind)
            return true;
        else
            return false;
    }

    bool operator> (const Student &r)
    {
        if (inf + math + rus > r.inf + r.math + r.rus)
            return true;
        else if (inf + math + rus == r.inf + r.math + r.rus && ind < r.ind)
            return true;
        else
            return false;
    }
};

//template <class T>
/*void merge(T* a, int64_t beg, int64_t mid, int64_t end)
{
    int64_t l_size = mid - beg;
    int64_t r_size = end - mid + 1;
    if (l_size == 0 || r_size == 0)
        return;
    if (l_size == 1 && r_size == 1)
        if (a[beg] > a[mid])
        {
            swap (a[beg], a[mid]);
            return;
        }
    int64_t x = beg / 2;
    int64_t r_x = bin_search (a, mid, end, a[x]);
}*/

template <class T>
void merge (T* a, int64_t beg, int64_t mid, int64_t end)
{
    T t[end + 1];
    int64_t i = beg;
    int64_t j = mid + 1;
    int64_t k = 0;
    while (i <= mid && j <= end)
    {
        if (a[i] < a[j])
            t[k++] = a[i++];
        else
            t[k++] = a[j++];
    }
    while (i <= mid)
        t[k++] = a[i++];
    while (j <= end)
        t[k++] = a[j++];
    --k;
    while (k >= 0)
    {
        a[beg + k] = t[k];
        --k;
    }
}


template <class T>
void merge_sort (T *a, int64_t beg, int64_t end)
{
    if (end - beg > 0)
    {
        int64_t mid = (beg + end) / 2;
        merge_sort (a, beg, mid);
        merge_sort (a, mid + 1, end);
        merge (a, beg, mid, end);
    }
}

int main ()
{
    int64_t N;
    scanf ("%ld", &N);
    Student* a = new Student[N];
    for (int64_t i = 0; i < N; ++i)
    {
        scanf ("%s %s %ld %ld %ld", (a + i)->surname,
        (a + i)->name, &(a + i)->inf,
        &(a + i)->math, &(a + i)->rus);
        a[i].ind = i;
    }
    merge_sort (a, 0, N - 1);
    for (int64_t i = N - 1; i >= 0; i--)
    {
        printf ("%s %s \n", a[i].surname, a[i].name);
    }
    delete[] a;
    return 0;
}
