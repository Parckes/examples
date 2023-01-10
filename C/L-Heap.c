#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(a, b) { __typeof__(a) c = (a); (a) = (b); (b) = (c); }

typedef struct node
{
    int i;
    int value;
    int pos;
} node_t;

typedef struct
{
    node_t* arr;
    int* insert;
    int veloc;
    int size;
    int amount;
    int dupl;
} heap_t;


void siftUp (heap_t *heap, int i) {
    while (i > 1 && heap->arr[i].value < heap->arr[i / 2].value) {
        SWAP (heap->arr[i], heap->arr[i / 2]);
        SWAP (heap->arr[i].pos, heap->arr[i / 2].pos);
        SWAP (heap->insert[heap->arr[i].i], heap->insert[heap->arr[i / 2].i]);
        i /= 2;
    }
    printf ("ok\n");
}

void siftDown (heap_t *heap, int i) {
    while (2 * i <= heap->size) {
        int min = -1;
        if (2 * i + 1 <= heap->size &&
            heap->arr[2 * i + 1].value < heap->arr[i].value &&
            heap->arr[2 * i + 1].value < heap->arr[2 * i].value) {
            min = 2 * i + 1;
        }
        else if (2 * i <= heap->size &&
                heap->arr[2 * i].value < heap->arr[i].value) {
            min = 2 * i;
        }
        if (min == -1)
            return;
        else {
            SWAP (heap->arr[i], heap->arr[min]);
            SWAP (heap->arr[i].pos, heap->arr[min].pos);
            SWAP (heap->insert[heap->arr[i].i], heap->insert[heap->arr[min].i]);
            i = min;
        }
    }
}

void insert (heap_t* heap, int x) {
    if (heap->size == heap->veloc) {
        heap->veloc += (heap->veloc == 0);
        heap->veloc *= 200001;
        (*heap).arr = (node_t* ) realloc (heap->arr, heap->veloc *
                                                                sizeof(node_t));
        (*heap).insert = (int* ) realloc (heap->insert, heap->veloc *
                                                                sizeof(int));
    }
    if (heap->dupl == 1) {
        for (int l = 1; l <= heap->amount; l++)
            heap->insert[l] = 0;
        heap->dupl = 0;
    }
    heap->arr[++(heap->size)].value = x;
    heap->arr[heap->size].i = ++(heap->amount);
    heap->arr[heap->size].pos = heap->size;
    int k = heap->size;
    heap->insert[heap->amount] = k;
    siftUp (heap, heap->size);
}

void extract_min (heap_t* heap) {
    if (heap->size == 0) {
        printf ("error\n");
        return;
    }
    printf ("%d\n", heap->arr[1].value);
    int j = heap->insert[heap->arr[1].i];
    SWAP (heap->arr[1], heap->arr[heap->size]);
    heap->arr[1].pos = 1;
    heap->insert[heap->arr[1].i] = j;
    heap->insert[heap->arr[heap->size].i] = 0;
    --(heap->size);
    siftDown (heap, 1);
}


void get_min (heap_t* heap) {
    if (heap->size == 0) {
        printf ("error\n");
        return;
    }
    printf ("%d\n", heap->arr[1].value);
}

void size (heap_t* heap) {
    printf ("%d\n", heap->size);
}

void clear (heap_t* heap) {
    heap->size = 0;
    heap->dupl = 1;
    printf ("ok\n");
}

void delete (heap_t* heap, int i) {
    if (heap->amount < i) {
        printf ("error\n");
        return;
    }
    if (heap->dupl == 1) {
        for (int l = 1; l <= heap->amount; l++)
            heap->insert[l] = 0;
        heap->dupl = 0;
    }
    int value = -1;
    node_t t;
    if (heap->insert[i] != 0)
    {
        value = heap->arr[heap->insert[i]].value;
        t = heap->arr[heap->insert[i]];
    }
    if (value == -1) {
        printf ("error\n");
        return;
    }
    int k = heap->insert[i];
    heap->arr[heap->insert[i]] = heap->arr[heap->size];
    heap->arr[heap->size] = t;
    heap->arr[heap->insert[i]].pos = t.pos;
    heap->insert[heap->arr[heap->insert[i]].i] = k;
    heap->insert[i] = 0;
    --(heap->size);
    siftDown (heap, k);
    siftUp (heap, k);
}

void change (heap_t* heap, int i, int delta) {
    if (heap->amount < i) {
        printf ("error\n");
        return;
    }
    if (heap->dupl == 1) {
        for (int l = 1; l <= heap->amount; l++)
            heap->insert[l] = 0;
        heap->dupl = 0;
    }
    int value = -1;
    if (heap->insert[i] != 0) {
        value = heap->arr[heap->insert[i]].value;
    }
    if (value == -1) {
        printf ("error\n");
    return;
    }
    heap->arr[heap->insert[i]].value = delta;
    siftDown (heap, heap->insert[i]);
    siftUp (heap, heap->insert[i]);
}

void init_heap (heap_t* heap) {
    heap->arr = 0;
    heap->insert = 0;
    heap->size = 0;
    heap->amount = 0;
    heap->veloc = 0;
    heap->dupl = 0;
}

int main () {
    heap_t heap;
    init_heap (&heap);
    char req[20];
    int  req_num;
    scanf ("%d", &req_num);

    for (int i = 0; i < req_num; ++i) {
        scanf ("%s", req);
        if (strcmp (req, "insert") == 0) {
            int x = 0;
            scanf("%d", &x);
            insert(&heap, x);
        }

        if (strcmp (req, "extract_min") == 0)
            extract_min (&heap);

        if (strcmp (req, "get_min")   == 0)
            get_min (&heap);

        if (strcmp (req, "delete")  == 0) {
            int x = 0;
            scanf ("%d", &x);
            delete (&heap, x);
        }

        if (strcmp (req, "size")  == 0)
            size(&heap);

        if (strcmp (req, "change")  == 0) {
            int x = 0;
            int n = 0;
            scanf("%d %d", &x, &n);
            change (&heap, x, n);
        }

        if (strcmp (req, "clear") == 0)
            clear (&heap);
    }
    return 0;
}
