#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALLOCATED 1000

typedef struct stack
{
	int top;
	int  a[ALLOCATED];
} stack_t;

void push (stack_t* stack)
{
	if (stack->top == ALLOCATED)
	{
		printf ("ERROR: overflow\n");
		return;
	}
	scanf ("%d", &stack->a[stack->top]);
	stack->top += 1;

	printf ("ok\n");
	return;
}

void pop (stack_t* stack)
{
	if (!stack->top)
	{
		printf ("error\n");
		return;
	}
	printf ("%d\n", stack->a [stack->top - 1]);
	stack->top -= 1;
	return;
}

void back (stack_t* stack)
{
	if (!stack->top)
        {
                printf ("error\n");
                return;
        }
        printf ("%d\n", stack->a [stack->top - 1]);
	return;
}

void size (stack_t* stack)
{
    printf ("%d\n", stack->top);
    return;
}

void clear (stack_t* stack)
{
   stack->top = 0;
   printf ("ok\n");
   return;
}

int main ()
{
    stack_t stack;
    stack.top = 0;
    char req[10];

    scanf ("%s", req);
	while (strcmp (req, "exit"))
    {
        if (strcmp (req, "push")  == 0)
            push  (&stack);

        if (strcmp (req, "pop")   == 0)
            pop   (&stack);

        if (strcmp (req, "back")  == 0)
            back  (&stack);

        if (strcmp (req, "size")  == 0)
            size  (&stack);

        if (strcmp (req, "clear") == 0)
            clear (&stack);

        scanf ("%s", req);

    }
    	printf ("bye");
	return 0;

}
