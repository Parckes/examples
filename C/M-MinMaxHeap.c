#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int* heap;
    int* buff;
    int* insert;
    int size;
} heap_t;

void swap_heap(heap_t* heap, int i, int r)
{
    int node = heap->buff[i];
    heap->buff[i] = heap->buff[r];
    heap->buff[r] = node;
    int ind_i = heap->insert[i];
    int ind_r = heap->insert[r];

    node = heap->insert[i];
    heap->heap[ind_i] = r;
    heap->heap[ind_r] = i;
    heap->insert[i] = heap->insert[r];
    heap->insert[r] = node;
    return;
}

void siftDown_min(heap_t* heap, int i)
{
    while (2 * i < heap->size)
    {
        int l = 2 * i;
        int r = 2 * i + 1;
        int y = l;
        if (r < heap->size && heap->buff[r] < heap->buff[l])
        {
            y = r;
        }
        if (heap->buff[i] <= heap->buff[y])
        {
            break;
        }
        swap_heap(heap, i, y);
        i = y;
    }
    return;
}

void siftUp_min(heap_t* heap, int i)
{
    while (heap->buff[i] < heap->buff[(i / 2)] && i > 1)
    {
        swap_heap(heap, i, i / 2);
        i /= 2;
    }
    return;
}

void siftDown_max(heap_t* heap, int i)
{
while (2 * i < heap->size)
    {
        int l = 2 * i;
        int r = 2 * i + 1;
        int y = l;
        if (r < heap->size && heap->buff[r] > heap->buff[l])
        {
            y = r;
        }
        if (heap->buff[i] >= heap->buff[y])
        {
            break;
        }
        swap_heap(heap, i, y);
        i = y;
    }
    return;
}

void siftUp_max(heap_t* heap, int i)
{
    while (heap->buff[i] > heap->buff[(i / 2)] && i > 1)
    {
        swap_heap(heap, i, i / 2);
        i /= 2;
    }
    return;
}

void insert_min(heap_t* heap, int ins, int x)
{
    heap->buff[heap->size] = x;
    heap->heap[ins] = heap->size;
    heap->insert[heap->size] = ins;
    siftUp_min(heap, heap->size);
    heap->size += 1;
    return;
}

void insert_max(heap_t* heap, int ins, int x)
{
    heap->buff[heap->size] = x;
    heap->heap[ins] = heap->size;
    heap->insert[heap->size] = ins;
    siftUp_max(heap, heap->size);
    ++(heap->size);
    return;
}

int extract_min(heap_t* heap)
{
    int *t = &(heap->heap[heap->insert[1]]);
    int node = heap->buff[1];
    --(heap->size);
    swap_heap(heap, 1, heap->size);
    siftDown_min(heap, 1);
    *t = 0;
    return node;
}

int extract_max(heap_t* heap)
{
    int *t = &(heap->heap[heap->insert[1]]);
    int node = heap->buff[1];
    heap->size -= 1;
    swap_heap(heap, 1, heap->size);
    siftDown_max(heap, 1);
    *t = 0;
    return node;
}

void delete_min(heap_t* heap, int x)
{
    int i = heap->heap[x];
    heap->size -= 1;
    swap_heap(heap, heap->heap[x], heap->size);
    siftDown_min(heap, i);
    siftUp_min(heap, i);
    heap->heap[x] = 0;
}

void delete_max(heap_t* heap, int x)
{
    int i = heap->heap[x];
    heap->size -= 1;
    swap_heap(heap, heap->heap[x], heap->size);
    siftDown_max(heap, i);
    siftUp_max(heap, i);
    heap->heap[x] = 0;
}

heap_t* init_heap(int m)
{
    heap_t *p = calloc(1, sizeof(heap_t));
    p->buff = calloc(m + 1, sizeof(int));
    p->heap = calloc(m + 1, sizeof(int));
    p->insert = calloc(m + 1, sizeof(int));
    p->size = 1;
    return p;
}

void clear(heap_t *heap)
{
    free(heap->buff);
    free(heap->heap);
    free(heap->insert);
    heap->buff = 0;
    heap->heap = 0;
    heap->insert = 0;
    free(heap);
}

int main()
{
    int m;
    scanf("%d", &m);
    heap_t* heap_min = init_heap(m);
    heap_t* heap_max = init_heap(m);
    int ins = 1;

    for (int i = 0; i < m; i++)
    {
        char req[20];
        scanf("%s", req);

        if (strcmp(req, "insert") == 0)
        {
            int node = 0;
            scanf("%d", &node);
            insert_min(heap_min, ins, node);
            insert_max(heap_max, ins, node);
            ins++;
            printf("ok\n");
            continue;
        }

        if (strcmp(req, "extract_min") == 0)
        {
            if (heap_min->size <= 1)
            {
                printf("error\n");
                continue;
            }
            int x = heap_min->insert[1];
            int node = extract_min(heap_min);
            delete_max(heap_max, x);
            printf("%d\n", node);
            continue;
        }

        if (strcmp(req, "extract_max") == 0)
        {
            if (heap_max->size <= 1)
            {
                printf("error\n");
                continue;
            }
            int x = heap_max->insert[1];
            int node = extract_max(heap_max);
            delete_min(heap_min, x);
            printf("%d\n", node);
            continue;
        }

        if (strcmp(req, "get_min") == 0)
        {
            if (heap_min->size <= 1)
            {
                printf("error\n");
                continue;
            }
            printf("%d\n", heap_min->buff[1]);
            continue;
        }

        if (strcmp(req, "get_max") == 0)
        {
            if (heap_max->size <= 1)
            {
                printf("error\n");
                continue;
            }
                printf("%d\n",
                heap_max->buff[1]);
                continue;
        }

        if (strcmp(req, "size") == 0)
        {
            printf("%d\n", heap_min->size - 1);
            continue;
        }

        if (strcmp(req, "clear") == 0)
        {
            clear(heap_min);
            clear(heap_max);

            heap_min = init_heap(m);
            heap_max = init_heap(m);

            printf("ok\n");
            continue;
        }
    }

    clear(heap_min);
    clear(heap_max);
    return 0;
}
