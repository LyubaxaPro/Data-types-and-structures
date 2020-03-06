#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define CORRECT_ERROR 1
#define SUCCESS 0
#define EMPTY_STACK -2
#define FULL_STACK -1
#define MAX_STR_SIZE 30
#define MALLOC_ERROR 3

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

#define MIN_SIZE 0
#define MAX_SIZE 4010
//Структура стека для реализации с использованием масива
typedef struct
{
    char stack[MAX_SIZE];  // Массив данных
    int size;  // Текущий размер стека
}stack_by_arr;

struct Node
{
    char val;  // Поле данных
    struct Node *ptr;  // Указатель на следующий элемент
};

//Структура стека для реализации с использованием односвязного списка
typedef struct
{
    struct Node *Head;  // Указатель на начало стека
    int size;  //  Размер стека
}stack_by_list;

typedef struct Node tNode;

//Структура списка свободных областей
typedef struct
{
    tNode* buffer[MAX_SIZE];  // Массив данных
    int size;  // Текущий размер списка
}address_list;

int check_stack_by_arr_size(stack_by_arr *stack)
{
    if (stack->size == 0)
    {
        return EMPTY_STACK;
    }
    if (stack->size == MAX_SIZE)
    {
        return FULL_STACK;
    }
    return SUCCESS;
}

int push_stack_by_arr(stack_by_arr *stack, const char val)
{
    if (check_stack_by_arr_size(stack) != FULL_STACK)
    {
        stack->stack[stack->size] = val;
        stack->size ++;
        return SUCCESS;
    }
    else
    {
        return FULL_STACK;
    }
}

void push_to_stack_by_arr_processing(stack_by_arr *stack)
{
    char string[MAX_STR_SIZE];
    printf("\nInput char in any register to push: ");
    scanf("%s", string);
    if (!((string[1] == '\n' || string[1] == '\0') && ((string[0] >= 'a' && string[0] <= 'z')
          || (string[0] >= 'A' && string[0] <= 'Z'))))
    {
        printf("\nNot correct val\n");
        return;
    }
    if (push_stack_by_arr(stack, string[0]) == FULL_STACK)
    {
        printf("\nStack is full");
        return;
    }
    else
    {
        printf("\nVariable recorded successfully\n");
    }
}

int check_str(char *string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z')))
        {
            printf("Wrong symbol in string");
            return CORRECT_ERROR;
        }
        i++;
    }
    return SUCCESS;
}

char pop_from_stack_by_arr(stack_by_arr *stack, const int flag)  // flag == 0 - печатать адрес переменной
                                                           // flag == 1 - не печатать адрес переменной
{
    if (stack->size == 0)
    {
        return EMPTY_STACK;
    }

    char val = stack->stack[stack->size - 1];
    if (flag == 0)
    {
        printf("\naddress of val: %p    ", &stack->stack[stack->size - 1]);
    }
    stack->size -= 1;
    return val;
}

void show_stack_by_arr(stack_by_arr *stack)
{
    printf("\nStack size = %d", stack->size);
    printf("\nStack by arr:\n");
    char temp;
    while(stack->size != 0)
    {
        temp = pop_from_stack_by_arr(stack, 0);
        printf("%c\n", temp);
    }
    printf("\n");
}

void check_if_str_is_p8m_using_stack_by_arr(char *string, int flag) // flag = 0 -print warnings, else do not print
{
    stack_by_arr stack_2;
    stack_by_arr stack_copy;
    stack_2.size = 0;
    stack_copy.size = 0;
    char val;
    if (strlen(string) == 0)
    {
        if (flag == 0)
            printf("\nEmpty stack!\n");
        return;
    }

    if (strlen(string) == 1)
    {
        if (flag == 0)
            printf("\nString is palindrome\n");
        return;
    }
    for(int i = 0; i < strlen(string); i++)
        push_stack_by_arr(&stack_copy, string[i]);

    for (int i = 0; i < strlen(string) / 2; i++)
    {
        val = pop_from_stack_by_arr(&stack_copy, 1);
        push_stack_by_arr(&stack_2, val) ;
    }
   if (stack_copy.size == stack_2.size + 1)
   {
       pop_from_stack_by_arr(&stack_copy, 1);
   }
   int size = stack_copy.size;
   for (int i = 0; i < size; i++)
   {
       if (pop_from_stack_by_arr(&stack_copy, 1) != pop_from_stack_by_arr(&stack_2, 1))
       {
           if (flag == 0)
               printf("\nString is not palindrome\n");
           return;
       }
   }
   if (flag == 0)
       printf("\nString is palindrome\n");
}

void clean_stack_by_arr(stack_by_arr *stack)
{
    stack->size = 0;
}

void delete_stack_by_list(stack_by_list *stack)
{
    struct Node *temp_ptr = NULL;
    struct Node *next_ptr = stack->Head;
    while(next_ptr != NULL)
    {
        temp_ptr = next_ptr;
        next_ptr = temp_ptr->ptr;
        free(temp_ptr);
    }
    stack->size = 0;
    stack->Head = NULL;
}

int push_stack_by_list(stack_by_list *stack, const char val)
{
    if (stack->size == MAX_SIZE)
    {
        return FULL_STACK;
    }
    struct Node *temp_ptr = NULL;
    temp_ptr = malloc(sizeof(struct Node));
    if (temp_ptr == NULL)
    {
        printf("\nMalloc error!\n");
        return MALLOC_ERROR;
    }
    temp_ptr->ptr = stack->Head;
    temp_ptr->val = val;
    stack->Head = temp_ptr;
    stack->size++;
    return SUCCESS;
}

void push_stack_by_list_processing(stack_by_list *stack)
{
    char string[MAX_STR_SIZE];
    printf("\nInput char in any register to push: ");
    scanf("%s", string);
    if (!((string[1] == '\n' || string[1] == '\0') && ((string[0] >= 'a' && string[0] <= 'z')
          || (string[0] >= 'A' && string[0] <= 'Z'))))
    {
        printf("\nNot correct val\n");
        return;
    }
    int rc = push_stack_by_list(stack, string[0]);
    if (rc == FULL_STACK)
    {
        printf("\nStack is full!\n");
        return;
    }
    if (rc == MALLOC_ERROR)
    {
        printf("\nMemmory allocation error\n");
        return;
    }
    printf("\nVariable recorded successfully\n");
}

char pop_from_stack_by_list(stack_by_list *stack, const int flag, address_list *adress_struct)
{
    if (stack->size == 0)
    {
        return EMPTY_STACK;
    }

    struct Node *temp_ptr;
    char val = stack->Head->val;
    temp_ptr = stack->Head->ptr;
    if (flag == 0)
    {
        printf("\nAddress of val: %p\n", stack->Head);

        if (adress_struct->size == MAX_SIZE)
        {
            printf("\nAdress buffer is full!\n");
        }
        else
        {
            adress_struct->buffer[adress_struct->size] = stack->Head;
            adress_struct->size += 1;
        }
    }

    free(stack->Head);
    stack->Head = temp_ptr;
    stack->size--;
    return val;
}

void show_stack_by_list(stack_by_list *stack, address_list *address_struct)
{
    printf("Stack size : %d\n", stack->size);
   // struct Node *temp_ptr = stack->Head;

    printf("Stack\n");
    char temp;
    while(stack->size != 0)
    {
        temp = pop_from_stack_by_list(stack, 0, address_struct);
        printf("val:  %c\n", temp);
//        printf("Val: %c       Address of val: %p\n", temp_ptr->val, temp_ptr)
//        temp_ptr = temp_ptr->ptr;
    }
}

void show_address_list(address_list *adress_struct)
{
    printf("\nShow adresses of free areas: \n");
    for(int i = adress_struct->size - 1; i >= 0; i--)
    {
        printf("%p\n", adress_struct->buffer[i]);
    }
}

void check_if_str_is_p8m_using_stack_by_list(char *string, address_list *adress_struct, int flag)
{
    char val;
    if (strlen(string) == 0)
    {
        if (flag == 0)
            printf("\nEmpty stack!\n");
        return;
    }

    if (strlen(string) == 1)
    {
        if (flag == 0)
            printf("\nString is palindrome\n");
        return;
    }

    stack_by_list stack_2;
    stack_2.Head = NULL;
    stack_2.size = 0;

    stack_by_list stack_copy;
    stack_copy.size = 0;
    stack_copy.Head = NULL;
    int i = 0;

    while(string[i] != '\0')
    {
        push_stack_by_list(&stack_copy, string[i]);
        i++;
    }
    int size = stack_copy.size / 2;

    for (int i = 0; i < size; i++)
    {
        val = pop_from_stack_by_list(&stack_copy, 1, adress_struct);
        push_stack_by_list(&stack_2, val) ;
    }
   if (stack_copy.size  == stack_2.size + 1)
   {
       pop_from_stack_by_list(&stack_copy, 1, adress_struct);
   }
   if (stack_2.size != stack_copy.size)
   {
       val = pop_from_stack_by_list(&stack_copy, 1, adress_struct);
       push_stack_by_list(&stack_2, val) ;
   }

   size = stack_copy.size;

   for (int i = 0; i < size; i++)
   {
       if (pop_from_stack_by_list(&stack_copy, 1, adress_struct) != pop_from_stack_by_list(&stack_2, 1, adress_struct))
       {
           if (flag == 0)
               printf("\nString is not palindrome\n");
           delete_stack_by_list(&stack_2);
           delete_stack_by_list(&stack_copy);
           return;
       }
   }
   if (flag == 0)
       printf("\nString is palindrome\n");
   delete_stack_by_list(&stack_2);
   delete_stack_by_list(&stack_copy);
}

int find_len_int(int num)
{
    if (num == 0)
        return 1;
    int result = 0;
    int copy = num;
    while (copy)
    {
        copy /= 10;
        result++;
    }
    return result;
}

void compare()
{
    clock_t start = 0, end = 0;
    long int time_arr, time_list;
    printf("\nCompare efficient\n");
    printf("┌-------------------------------------------------------------------------------------------------------------------------------┐\n");
    printf("| Numer of elements |          Stack by array      |         Stack by list       |By list/by array memmory|By list/by array time|\n");
    printf("|                   |------------------------------|-----------------------------|     (percents)         |       (percents)    |\n");
    printf("|                   | Time(tick)    | Memmory(byte)| Time(tick)   | Memmory(byte)|                        |                     |\n");
    printf("|-------------------|---------------|--------------|--------------|--------------|------------------------|---------------------|\n");
    stack_by_arr arr_stack;
    stack_by_list list_stack;
    int memmory_arr, memmory_list;
    memmory_arr = (sizeof(char) * MAX_SIZE + sizeof(int)) * 2;
    address_list adr;
    adr.size = 0;
    int j;
    for(int i = 10; i < 2010; i += 30)
    {
        delete_stack_by_list(&list_stack);
        clean_stack_by_arr(&arr_stack);
        printf("| %d", i);
        for (int j = 0; j < 18 - find_len_int(i); j++)
        {
            printf(" ");
        }
        printf("|");

        char string[MAX_SIZE];
        for (j = 0; j < i; j++)
        {
            string[j] = 'a';
        }
        string[j] = '\0';

        start = tick();
        check_if_str_is_p8m_using_stack_by_arr(string, 1);
        end = tick();
        time_arr = end - start;
        printf(" %ld", time_arr);
        for(int j = 0; j < 14 - find_len_int(end - start); j++)
        {
            printf(" ");
        }
        printf("|");
       // memmory_arr = (arr_stack.size * sizeof(char) + sizeof(int)) * 2;
        printf(" %d", memmory_arr);
        for (int j = 0; j < 13 - find_len_int(memmory_arr); j++)
        {
            printf(" ");
        }
        printf("|");
        start = tick();
        check_if_str_is_p8m_using_stack_by_list(string, &adr, 1);
        end = tick();
        time_list = end - start;
        printf(" %ld", end - start);
        for(int j = 0; j < 13 - find_len_int(end - start); j++)
        {
            printf(" ");
        }
        printf("|");
        memmory_list = (sizeof(struct Node) * strlen(string) + sizeof(int)) * 1.5;
        printf(" %d", memmory_list);
        for (int j = 0; j < 13 - find_len_int(memmory_list); j++)
        {
            printf(" ");
        }
        int temp = (memmory_list * 1. / memmory_arr) * 100;
        printf("| %d", temp);
        for (int k = 0; k < 23 - find_len_int(temp); k++)
            printf(" ");
        temp = (time_list * 1. / time_arr) * 100;
        printf("| %d", temp);
        for (int k = 0; k < 20 - find_len_int(temp); k++)
            printf(" ");
        printf("|\n");
    }
    printf("└------------------------------------------------------------------------------------------------------------------------------┘\n\n");
    delete_stack_by_list(&list_stack);
}

int main()
{
    stack_by_arr arr_stack;
    arr_stack.size = 0;
    char val;
    stack_by_list list_stack;
    list_stack.size = 0;
    list_stack.Head = NULL;
    address_list addres_struct;
    char string[MAX_SIZE];
    int rc = CORRECT_ERROR;
    rc += 1;
    addres_struct.size = 0;

    printf("You can do several things with the table entering the special key of the task.\n");
    printf("Max size of stack = 4010");
    for (;;)
    {
        printf("\nLIST OF TASKS\n"
               "enter 1 to push to stack by arr\n"
               "enter 2 to pop from stack by arr\n"
               "enter 3 to show stack by arr and it's size\n"
               "enter 4 to push to stack by list\n"
               "enter 5 to pop from stack by list\n"
               "enter 6 to show stack by list and it's size\n"
               "enter 7 to check if the string is a palindrome using stack by arr\n"
               "enter 8 to check if the string is a palindrome using stack by list\n"
               "enter 9 to clean stack by arr\n"
               "enter 10 to clean stack by list\n"
               "enter 11 to show adress list\n"
               "enter 12 to clean adress list\n"
               "enter 13 to compare efficient\n"
               "enter 0 to close the program\n");
        int s;
        int rc = scanf("%d", &s);
        if (rc != 1)
        {
            printf("Not correct input\n");
            return CORRECT_ERROR;
        }
        switch(s)
        {
            case 0:
                delete_stack_by_list(&list_stack);
                return SUCCESS;

            case 1:
                push_to_stack_by_arr_processing(&arr_stack);
                break;

            case 2:
                val = pop_from_stack_by_arr(&arr_stack, 0);
                if (arr_stack.size == 0 && val == EMPTY_STACK)
                {
                    printf("\nEmpty stack!\n");
                }
                else
                {
                    printf("\nElement of stack: %c\n", val);
                }
                break;

            case 3:
                show_stack_by_arr(&arr_stack);
                break;

            case 4:
                push_stack_by_list_processing(&list_stack);
                break;

            case 5:
                val = pop_from_stack_by_list(&list_stack, 0, &addres_struct);
                if (list_stack.size == 0 && val == EMPTY_STACK)
                {
                    printf("\nEmpty stack!\n");
                }
                else
                {
                    printf("Element of stack: %c\n", val);
                }
                break;

            case 6:
                show_stack_by_list(&list_stack, &addres_struct);
                break;

            case 7:
                rc = CORRECT_ERROR;
                while (rc != SUCCESS)
                {
                    printf("\nInput string without backspace. Max len 4010 symbols. Min len = 1.\n");
                    scanf("%s", string);
                    if (strlen(string) <= 0 || strlen(string) >MAX_SIZE)
                    {
                        printf("Wrong len of string.\n");
                        rc = CORRECT_ERROR;
                        break;
                    }
                    rc = check_str(string);
                    if (rc != SUCCESS)
                        break;
                    check_if_str_is_p8m_using_stack_by_arr(string, 0);
                    rc = SUCCESS;
                }
                break;

            case 8:
                rc = CORRECT_ERROR;
                while (rc != SUCCESS)
                {
                    printf("\nInput string without backspace. Max len 4010 symbols. Min len = 1.\n");
                    scanf("%s", string);
                    if (strlen(string) <= 0 || strlen(string) >MAX_SIZE)
                    {
                        printf("Wrong len of string.\n");
                        rc = CORRECT_ERROR;
                        break;
                    }
                    rc = check_str(string);
                    if (rc != SUCCESS)
                        break;
                    check_if_str_is_p8m_using_stack_by_list(string, &addres_struct, 0);
                    rc = SUCCESS;
                }
                break;

            case 9:
                clean_stack_by_arr(&arr_stack);
                break;

            case 10:
                delete_stack_by_list(&list_stack);
                break;

            case 11:
                show_address_list(&addres_struct);
                break;

            case 12:
                addres_struct.size = 0;
                break;

            case 13:
                compare();
                break;
        }
    }
}
