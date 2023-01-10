#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct node {
    int elem;
    struct node* next;
} node_t;

typedef struct queue {
    node_t* first;
    node_t* last;
    int size;
} queue_t;


void init_queue(queue_t* q) {
    q->first = NULL;
    q->size = 0;
    return;
}

void clear (queue_t* q) {
    while (q->first)
        pop_front(q);
    return;
}

void push(queue_t* q, int elem) {
    node_t* new_node = calloc(1, sizeof(node_t));
    new_node->elem = elem;

    if (q->first)
        q->last->next = new_node;
    else
        q->first = new_node;

    q->last = new_node;
    return;
}

int front(queue_t* q) {
    return (q->first)->elem;
}

void pop_front(queue_t* q) {
    node_t* x = q->first;
    q->first = q->first->next;
    fprintf(stderr, "POP %d\n", x->elem);
    free(x);
    return;
}

int main () {
    queue_t first;
    queue_t second;
    init_queue (&first);
    init_queue (&second);
    int moves = 0;

    for (int i = 0; i < CARDS; ++i) {
        int d;
        scanf ("%d", &d);
        push_front (&first, d);
    }
    for (int i = 0; i < CARDS; ++i) {
        int d;
        scanf ("%d", &d);
        push_front (&second, d);
    }

    while (first.size && second.size) {
        int f = front (&first);
        int s = front (&second);
        if (!f || !s) {
            if (!f) {
              pop_front (&second);
              pop_front (&first);
              push( &first, f);
              push (&first, s);
            }
            if (!s) {
              pop_front (&second);
              pop_front (&first);
              push (&second, f);
              push (&second, s);
            }
        }
        else {
            if (f > s) {
                pop_front (&second);
                pop_front (&first);
                push (&first, f);
                push (&first, s);
            }
            else {
                pop_front (&second);
                pop_front (&first);
                push (&second, f);
                push (&second, s);
            }
        }
        moves++;
    }
    if (first.size) {
        printf ("first %d", moves);
    }
    else {
        printf ("second %d", moves);
    }
    clear (&first);
    clear (&second);
    return 0;
}
