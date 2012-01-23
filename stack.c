#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

void Stack_Push(Stack **head, int id, void *data)
{
    Stack *new_item = malloc(sizeof(Stack));
    assert(new_item != NULL);
    
    new_item->id = id;
    new_item->data = data;
    new_item->next = *head;
      
    *head = new_item;
}

Stack *Stack_Pop(Stack **head)
{
    Stack *top;

    if(*head == NULL)
        return NULL;

    top = *head;
    *head = top->next;

    return top;
}

// This is just a reminder...
Stack *Stack_Read(Stack *head)
{
    return head;
}

int Stack_Size(Stack *head)
{
    Stack *sp;
    int size = 0;

    for(sp = head; sp != NULL; sp = sp->next)
        size++;

    return size;
}

/*
int main(int argc, char *argv[])
{
    Stack *zoom_stack = NULL;
    Stack *tmp;

    Stack_Push(&zoom_stack, 1, NULL);
    Stack_Push(&zoom_stack, 2, NULL);
    Stack_Push(&zoom_stack, 3, NULL);
    int stack_size = Stack_Size(zoom_stack);

    while(zoom_stack != NULL)
    {
        tmp = Stack_Pop(&zoom_stack);
        printf("id:%d\n", tmp->id);
    }

    for(tmp = zoom_stack; zoom_stack != NULL; tmp = Stack_Pop(&zoom_stack)) {
        printf("id:%d\n", tmp->id);
    }
    Stack_Pop(&zoom_stack);
}

// Simple testcase

*/
