#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


typedef struct elem {
    int64_t value;
    struct elem* next;
} elem_t;

typedef struct array {
    int64_t** a;
    int64_t size;
} array_t;


void push (array_t* arr) {
    int64_t n;
    scanf ("%ld", &n);
    arr->a[arr->size] = malloc (sizeof(int64_t));
    *(arr->a[arr->size]) = n;
    arr->size++;
    printf ("ok\n");
    return;
}

void pop (array_t* arr) {
    if (!arr->size)
    {
        printf ("error\n");
        return;
    }
    arr->size--;
    printf ("%ld\n", *arr->a[arr->size]);
    free(arr->a[arr->size]);
    return;
}

void get (array_t* arr) {
    int64_t k;
    scanf ("%ld", &k);
    if (k > arr->size - 1)
    {
        printf ("Incorrect index\n");
        return;
    }
    printf ("%ld\n", *arr->a[k]);
    return;
}

void size (array_t* arr) {
    printf ("%ld\n", arr->size);
    return;
}

void clear_ok (array_t* arr) {
    for (int64_t i = arr->size; i > 0; --i)
    {
        free (arr->a[i - 1]);
    }
    arr->size = 0;
    printf ("ok\n");
    return;
}

void clear (array_t* arr) {
    for (int64_t i = arr->size; i > 0; --i) {
        free (arr->a[i - 1]);
    }
    arr->size = 0;
    return;
}


int main() {
    int64_t N;
    scanf ("%ld", &N);

    array_t arr;
    arr.a = calloc (N, sizeof (int64_t*));
    arr.size = 0;
    char req[20];

    for (int64_t i = 0; i < N; ++i) {
        scanf ("%s", req);
        if (strcmp (req, "push")  == 0)
            push (&arr);

        if (strcmp (req, "get")   == 0)
            get (&arr);

        if (strcmp (req, "pop")   == 0)
            pop (&arr);

        if (strcmp (req, "size")  == 0)
            size (&arr);

        if (strcmp (req, "clear") == 0)
            clear_ok (&arr);
    }
    clear (&arr);
    free (arr.a);
    return 0;
}
