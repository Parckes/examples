#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int* a;
    int len;
    int max_len;
    int ind;
} array_t;

typedef struct
{
    int elem;
    int ind;
} part_t;

void SiftDown(part_t* begin, part_t* end, int i) {
    int size = (int) (end - begin);
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int y = i;
    if (l < size && begin[l].elem < begin[y].elem) {
        y = l;
    }
    if (r < size && begin[r].elem < begin[y].elem) {
        y = r;
    }
    if (y != i) {
        part_t t = begin[i];
        begin[i] = begin[y];
        begin[y] = t;
        SiftDown(begin, end, y);
    }
}

void SiftUp(part_t* begin, part_t* end, int i) {
    while (begin[i].elem < begin[(i - 1) / 2].elem) {
        part_t t = begin[i];
        begin[i] = begin[(i - 1) / 2];
        begin[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

void Insert(part_t* begin, part_t* end, part_t elem) {
    int size = (int) (end - begin);

    begin[size] = elem;

    SiftUp(begin, end + 1, size);
}

part_t ExtractMax(part_t* begin, part_t* end) {
    int size = (int) (end - begin);

    int ind = begin[0].ind;
    int elem = begin[0].elem;
    begin[0] = begin[size - 1];

    SiftDown(begin, end - 1, 0);

    part_t t;
    t.elem = elem;
    t.ind = ind;
    return t;
}

int main() {
    int k = 0;
    scanf("%d", &k);

    array_t* d = (array_t*) calloc(k, sizeof(array_t));

    int res_len = 0;

    for (int i = 0; i < k; i++) {
        array_t p;

        int n = 0;
        scanf("%d", &n);
        res_len += n;

        int* a = calloc(n, sizeof(int));
        for (int r = 0; r < n; r++) {
            scanf("%d", &a[r]);
        }

        p.a = a;
        p.len = n;
        p.max_len = n;
        p.ind = i;
        d[i] = p;
    }

    part_t* pyramid = calloc(k, sizeof(part_t));
    int size_pyr = 0;

    int* res = calloc(res_len , sizeof(int));
    int size_res = 0;

    for (int i = 0; i < k; i++) {
        part_t t;
        t.elem = d[i].a[0];
        t.ind = d[i].ind;

        Insert(pyramid, pyramid + size_pyr, t);
        size_pyr++;

        d[i].len--;

    }

    while (size_pyr > 0) {
        part_t t = ExtractMax(pyramid, pyramid + size_pyr);
        size_pyr--;

        int ind = t.ind;
        res[size_res] = t.elem;
        size_res++;

        int max_len = d[ind].max_len;
        int len = d[ind].len;
        if (len > 0) {
            part_t g;
            g.elem = d[ind].a[max_len - len];
            g.ind = d[ind].ind;

            Insert(pyramid, pyramid + size_pyr, g);
            size_pyr++;

            d[ind].len--;
        }
    }

    for (int i = 0; i < size_res; i++) {
        printf("%d ", res[i]);
    }

    for (int i = 0; i < k; i++) {
        free(d[i].a);
    }
    free(d);
    free(pyramid);
    free(res);
    return 0;
}
