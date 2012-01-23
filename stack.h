#ifndef _stack_h
#define _stack_h

typedef struct Stack {
    int id;
    void *data;
    struct Stack *next;
} Stack;

void Stack_Push(Stack **head, int id, void *data);
Stack *Stack_Pop(Stack **head);
Stack *Stack_Read(Stack *head);
int Stack_Size(Stack *head);

#endif
