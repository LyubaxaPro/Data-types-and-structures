#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#define SIZE_OF_QUEUE_ARR 3000
#define SIZE_OF_ADDRESS_ARR 4000
#define TEST_ARR_SIZE 4
#define START_POSITION 0

#define SUCCESS 0
#define CORRECT_ERROR 2
#define FULL 1
#define MALLOC_ERROR 3

#define NUM_ELEMNTS_TO_BREAK 1000
#define STEP 100

typedef struct
{
    int type; // 1 или 2
    double time_to_arrive; //время прихода в очередь

}arr_node;

typedef struct
{
    int type;
    arr_node buf[SIZE_OF_QUEUE_ARR];
    int max_size;
    int start;
    int cur_size;
}arr_queue;

struct list_node
{
    int type; // 1 или 2
    double time_to_arrive; //время прихода в очередь
    struct list_node *p_next;
};

typedef struct
{
    int type;
    int max_size;
    int cur_size;
    struct list_node *pin;     //указатель на начало очереди
}list_queue;

typedef struct list_node Node;
//Структура списка свободных областей
typedef struct
{
    Node* buffer[SIZE_OF_ADDRESS_ARR];  // Массив данных
    int size;  // Текущий размер списка
}address_list;

void push_q_arr(arr_queue *q, double time_of_arr, int type)
{
    if (q->cur_size == q->max_size - 1) {
        printf("Queue is full!\n");
        return;
    }
    q->buf[(q->start + q->cur_size) % q->max_size].time_to_arrive = time_of_arr;
    q->buf[(q->start + q->cur_size) % q->max_size].type = type;
    q->cur_size++;
}

arr_node pop_q_arr(arr_queue *q)
{
    arr_node cur_arr_node;
    if (q->cur_size == 0) {
        printf("Queue is empty.\n");
        cur_arr_node.type = -1;
        return cur_arr_node;
    }
    cur_arr_node = q->buf[q->start];
    q->start = (q->start + 1) % q->max_size;
    q->cur_size--;
    return cur_arr_node;
}

int push_q_list(list_queue *q, double time_to_arr, int type)
{
    if (q->cur_size == q->max_size)
    {
        printf("Queue is full!\n");
        return FULL;
    }
    struct list_node *node = malloc(sizeof(struct list_node));
    if (node == NULL)
    {
        printf("Malloc error\n");
        return MALLOC_ERROR;
    }
    node->time_to_arrive = time_to_arr;
    node->type = type;
    node->p_next = NULL;

    if (q->pin == NULL)
    {
        q->pin = node;
        q->cur_size++;
        return SUCCESS;
    }

    struct list_node *curptr = q->pin;
    while (curptr->p_next != NULL)
        curptr = curptr->p_next;

    curptr->p_next = node;
    q->cur_size++;
    return SUCCESS;
}

void pop_q_list(list_queue *q, double *time_arrive, int *type, address_list *address)
{
    struct list_node *node;
    if (q->cur_size == 0)
    {
        printf("Queue is empty!\n");
        *type = -1;
        return;
    }

    if (address->size == SIZE_OF_ADDRESS_ARR)
    {
        printf("Address list is full\n");
        address->size = 0;
    }
    else
    {
        address->buffer[address->size] = q->pin;
        address->size++;
    }

    *time_arrive = q->pin->time_to_arrive;
    *type = q->pin->type;
    node = q->pin;
    q->pin = q->pin->p_next;
    free(node);
    q->cur_size--;
    return;
}

void delete_queue(list_queue *q)
{
    struct list_node *curptr = q->pin;
    struct list_node *temp;
    while (curptr != NULL)
    {
        temp = curptr;
        curptr = curptr->p_next;
        free(temp);
    }
}

double generate_rand_time(int min, int max)
{
    return (max - min) * (rand() % 100) * 0.01 + min;
}

void usleep(double time) {
    // printf("\nusleep in %lf : ", time);
    //sleep(time);
}

void queue_arr_constructor(arr_queue *q, int type, int max_size)
{
    q->cur_size = 0;
    q->max_size = max_size;
    q->type = type;
    q->start = 0;
}

void print_arr_q(arr_queue *q)
{
    for (int i = 0; i < q->cur_size; ++i)
    {
        printf("%d %d %lf", i, q->buf[(q->start + i) % q->max_size].type, q->buf[(q->start + i) % q->max_size].time_to_arrive);
    }
}

void queue_list_constructor(list_queue *q, int type)
{
    q->cur_size = 0;
    q->max_size = SIZE_OF_QUEUE_ARR;
    q->pin = NULL;
    q->type = type;
}

void processing_list(address_list *result_addr, address_list *q_address, int min_t1, int max_t1, int min_t2, int max_t2, int min_t3, int max_t3, int min_t4, int max_t4)
{
    double cur_time = 0.;
    list_queue first_q;
    list_queue second_q;
    result_addr->size = 0;
    address_list address;
    address.size = 0;

    queue_list_constructor(&first_q, 1);
    queue_list_constructor(&second_q, 2);

    long long count_q1_processed = 0;
    long long sum_of_size_q1 = 0;
    long long sum_of_size_q2 = 0;
    long long count_q1 = 0;
    long long count_q2 = 0;


    double sum_of_time_q1 = 0;
    int count_time_q1 = 0;
    double sum_of_time_q2 = 0;
    int count_time_q2 = 0;

    double q1_up_time = generate_rand_time(min_t1, max_t1);
    double q2_up_time = generate_rand_time(min_t2, max_t2);
    double oa_up_time = -1;
    double downtime = 0;
    double last_time = 0;
    int count = 0;
    int count_push = 0;
    int count_for_3_q1 = 0;
    int count_for_3_q2 = 0;
    int count_push_q1 = 0;
    int count_push_q2 = 0;
    int count_pop_q1 = 0;
    int count_pop_q2 = 0;

    int node_type;
    double node_time_arr;
    double mid_size_2 = 0;


    int cur_type_in_oa;
    int time_start = clock();
    while(1)
    {
        if (oa_up_time <= q1_up_time && oa_up_time <= q2_up_time)
        {
            if (oa_up_time != -1)
            {
                usleep(oa_up_time - cur_time);
                cur_time = oa_up_time;
                last_time = cur_time;
                if (cur_type_in_oa == 1)
                {
                    count_q1_processed++;
                    if (count_q1_processed % STEP == 0) {
                        printf("---------------------------------------------------------------------------------------------------\n");
                        printf("\nNumber of elements = %lld\n", count_q1_processed);
                        if (count_q1 != 0)
                            printf("Current size of q1 = %d  Mid size of q1 = %lf\n", first_q.cur_size, sum_of_size_q1 * 1.0 / count_q1);
                        else
                            printf("Current size of q1 = %d  Mid size of q1 = %d\n", first_q.cur_size, first_q.cur_size);
                        sum_of_size_q1 = 0;
                        count_for_3_q1 = 0;

                        printf("Number of applications entered in q1 = %d   Number of applications issued q1 = %d\n", count_push_q1, count_pop_q1);
                        count_q1 = 0;
                        if (count_time_q1 != 0)
                            printf("Mid time for processing one element in q1: %lf\n", sum_of_time_q1 / count_time_q1);
                        else
                             printf("Mid time for processing one element in q1: 0\n");

                        mid_size_2 += second_q.cur_size;

                        if (count_q2)
                            printf("Current size of q2 = %d  Mid size of q2 = %lf\n", second_q.cur_size, sum_of_size_q2 * 1.0 / count_q2);
                        else
                            printf("Current size of q2 = %d  Mid size of q2 = %d\n", second_q.cur_size, second_q.cur_size);

                        sum_of_size_q2 = 0;
                        count_for_3_q2 = 0;
                        printf("Number of applications entered in q2 = %d   Number of applications issued q2 = %d\n", count_push_q2, count_pop_q2);
                        count_q2 = 0;
                        if (count_time_q2 != 0)
                            printf("Mid time for processing one element in q2: %lf\n", sum_of_time_q2 / count_time_q2);
                        else
                            printf("Mid time for processing one element in q2: 0\n");

                        printf("---------------------------------------------------------------------------------------------------\n");

                    }
                    if (count_q1_processed == NUM_ELEMNTS_TO_BREAK) {
                        printf("Time of simulation: %lf ms\n", clock() - time_start + cur_time);
                        printf("Time of model: %lf ms\n", cur_time);
                        struct list_node q;
                        printf("Memory of simulation: %ld byte\n", sizeof(q) * (count + first_q.cur_size + second_q.cur_size) + 2 * (sizeof(int) * 4 + sizeof(double)));
                        printf("Downtime: %lf\n", downtime);
                        printf("Number of applications entered %d\n", count + first_q.cur_size + second_q.cur_size);
                        printf("Number of applications issued %d\n", count);
                        printf("The oa worked %d times\n", count);
                        if (count + first_q.cur_size + second_q.cur_size != 0)
                            printf("Measurement error %lf percent\n\n\n", 1.0 * ((second_q.cur_size + first_q.cur_size) * 100) / (count + second_q.cur_size + first_q.cur_size));
                        else
                            printf("Measurement error 0 percent\n\n\n");
                        break;
                    }
                }
            }
            if (first_q.cur_size != 0)

            {
                count_pop_q1++;
                pop_q_list(&first_q, &node_time_arr, &node_type, &address);
                count++;
                downtime += cur_time - last_time;
                sum_of_time_q1 += cur_time - node_time_arr;
                count_time_q1++;
                count_for_3_q1++;
                if (count_for_3_q1 == 3)
                {
                  sum_of_size_q1 += first_q.cur_size;
                  count_q1++;
                  count_for_3_q1 = 0;
                }

                cur_type_in_oa = 1;
                oa_up_time = cur_time + generate_rand_time(min_t3, max_t3);
                continue;
            }
            if (second_q.cur_size != 0)
            {
                count_pop_q2++;
                pop_q_list(&second_q, &node_time_arr, &node_type, &address);
                count++;
                downtime += cur_time - last_time;

                count_for_3_q2++;
                if (count_for_3_q2 == 3)
                {
                  sum_of_size_q2 += second_q.cur_size;
                  count_q2++;
                  count_for_3_q2 = 0;
                }

                sum_of_time_q2 += cur_time - node_time_arr;
                count_time_q2++;
                cur_type_in_oa = 2;
                oa_up_time = cur_time + generate_rand_time(min_t4, max_t4);
                continue;

            }
            oa_up_time = -1;
        }
        if (q1_up_time <= q2_up_time)
        {
            usleep(q1_up_time - cur_time);
            cur_time = q1_up_time;
            if (first_q.cur_size == first_q.max_size)
            {
                printf("First queue is full!\n");
                delete_queue(&first_q);
                delete_queue(&second_q);
                return;
            }
            count_push_q1++;
            push_q_list(&first_q, cur_time, 1);
            count_push++;
            q1_up_time = cur_time + generate_rand_time(min_t1, max_t1);
        }
        else
        {
            usleep(q2_up_time - cur_time);
            cur_time = q2_up_time;
            if (second_q.cur_size == second_q.max_size)
            {
                printf("Second queue is full!\n");
                delete_queue(&first_q);
                delete_queue(&second_q);
                return;
            }
            count_push_q2++;
            push_q_list(&second_q, cur_time, 2);
            count_push++;
            q2_up_time = cur_time + generate_rand_time(min_t2, max_t2);
        }
    }
    q_address->size = 0;

    int flag;
    Node *temp_ptr;
    temp_ptr = second_q.pin;
    while (temp_ptr != NULL)
    {
        q_address->buffer[q_address->size] = temp_ptr;
        q_address->size++;
        temp_ptr = temp_ptr->p_next;
    }

    for (int i = 0; i < address.size; i++)
    {
        flag = 0;
        for (int j = 0; j < result_addr->size; j++)
        {
            if (address.buffer[i] == result_addr->buffer[j])
            {
                flag = 1;
                break;
            }
        }

        for (int j = 0; j < q_address->size; j++)
        {
            if (address.buffer[i] == q_address->buffer[j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            result_addr->buffer[result_addr->size] = address.buffer[i];
            result_addr->size++;
        }
    }

    delete_queue(&first_q);
    delete_queue(&second_q);
}


void processing_arr(int min_t1, int max_t1, int min_t2, int max_t2, int min_t3, int max_t3, int min_t4, int max_t4)
{
    double cur_time = 0.;
    arr_queue first_q;
    arr_queue second_q;
//    arr_queue oa_q;
    queue_arr_constructor(&first_q, 1, SIZE_OF_QUEUE_ARR);
    queue_arr_constructor(&second_q, 2, SIZE_OF_QUEUE_ARR);
//    queue_arr_constructor(&oa_q, 3);
    arr_node temp_node;

    long long count_q1_processed = 0;
    long long sum_of_size_q1 = 0;
    long long sum_of_size_q2 = 0;
    long long count_q1 = 0;
    long long count_q2 = 0;
    int count_push_q1 = 0;
    int count_push_q2 = 0;
    int count_pop_q1 = 0;
    int count_pop_q2 = 0;

    int count_for_3_q1 = 0;
    int count_for_3_q2 = 0;


    double sum_of_time_q1 = 0;
    int count_time_q1 = 0;
    double sum_of_time_q2 = 0;
    int count_time_q2 = 0;

    double q1_up_time = generate_rand_time(min_t1, max_t1);
    double q2_up_time = generate_rand_time(min_t2, max_t2);
    double oa_up_time = -1;
    double downtime = 0;
    double last_time = 0;
    int count = 0;
    int count_push = 0;
    double mid_time;
    double mid_size_1 = 0;
    double mid_size_2 = 0;



    int time_start = clock();
    int cur_type_in_oa;
    while(1)
    {
        if (oa_up_time <= q1_up_time && oa_up_time <= q2_up_time)
        {
            if (oa_up_time != -1)
            {
                usleep(oa_up_time - cur_time);
                cur_time = oa_up_time;
                last_time = cur_time;
                if (cur_type_in_oa == 1)
                {
                    count_q1_processed++;
                    if (count_q1_processed % 100 == 0) {
                        printf("---------------------------------------------------------------------------------------------------------------\n");
                        printf("\nNumber of elements = %lld\n", count_q1_processed);
                        if (count_time_q1 != 0)
                            mid_time = sum_of_time_q1 / count_time_q1;
                        else
                            mid_time = 0;
                        mid_size_1 += first_q.cur_size;
                        if (count_q1 != 0)
                            printf("Current size of q1 = %d  Mid size of q1 = %lf\n", first_q.cur_size, sum_of_size_q1 * 1.0 / count_q1);
                        else
                            printf("Current size of q1 = %d  Mid size of q1 = %d\n", first_q.cur_size, first_q.cur_size);
                        sum_of_size_q1 = 0;
                        count_for_3_q1 = 0;


                        printf("Number of applications entered in q1 = %d   Number of applications issued q1 = %d\n", count_push_q1, count_pop_q1);
                        count_q1 = 0;
                        printf("Mid time for processing one element in q1: %lf\n", mid_time);

                        if (count_time_q2 != 0)
                            mid_time = sum_of_time_q2 / count_time_q2;
                        else
                            mid_time = 0;

                        mid_size_2 += second_q.cur_size;
                        if (count_q2 != 0)
                            printf("Current size of q2 = %d  Mid size of q2 = %lf\n", second_q.cur_size, sum_of_size_q2 * 1.0 / count_q2);
                        else
                            printf("Current size of q2 = %d  Mid size of q2 = %d\n", second_q.cur_size, second_q.cur_size);
                        sum_of_size_q2 = 0;
                        count_for_3_q2 = 0;
                        printf("Number of applications entered in q2 = %d   Number of applications issued q2 = %d\n", count_push_q2, count_pop_q2);
                        count_q2 = 0;
                        printf("Mid time for processing one element in q2: %lf\n", mid_time);
                        printf("---------------------------------------------------------------------------------------------------------------\n");

                    }
                    if (count_q1_processed == NUM_ELEMNTS_TO_BREAK) {
                        printf("Time of simulation: %lf ms\n", clock() - time_start + cur_time);
                        printf("Time of model: %lf ms\n", cur_time);

                        printf("Memory of simulation: %ld byte\n", sizeof(first_q) + sizeof(second_q));
                        printf("Downtime: %lf\n", downtime);
                        printf("Number of applications entered %d\n", count + second_q.cur_size + first_q.cur_size);
                        printf("Number of applications issued %d\n", count);
                        printf("The oa worked %d times\n", count);
                        if ((count + second_q.cur_size + first_q.cur_size) != 0)
                            printf("Measurement error %lf percent\n\n\n", 1.0 * ((second_q.cur_size + first_q.cur_size) * 100) / (count + second_q.cur_size + first_q.cur_size));
                        else
                             printf("Measurement error 0 percent\n\n\n");
                        break;
                    }
                }

            }
            if (first_q.cur_size != 0)
            {
                temp_node = pop_q_arr(&first_q);
                count_pop_q1++;
                count++;
                downtime += cur_time - last_time;
                sum_of_time_q1 += cur_time - temp_node.time_to_arrive;
                count_time_q1++;
                count_for_3_q1++;
                if (count_for_3_q1 == 3)
                {
                  sum_of_size_q1 += first_q.cur_size;
                  count_q1++;
                  count_for_3_q1 = 0;
                }
                //printf("Deleted node: type %d, time %lf\n", temp_node.type, temp_node.time_to_arrive);
                // push_q_arr(&oa_q, temp_node, 1); // handle time
                cur_type_in_oa = 1;
                oa_up_time = cur_time + generate_rand_time(min_t3, max_t3);
                continue;
            }
            if (second_q.cur_size != 0)
            {
                count_pop_q2++;
                temp_node = pop_q_arr(&second_q);
                count++;
                downtime += cur_time - last_time;
                sum_of_time_q2 += cur_time - temp_node.time_to_arrive;
                count_time_q2++;

                count_for_3_q2++;
                if (count_for_3_q2 == 3)
                {
                  sum_of_size_q2 += second_q.cur_size;
                  count_q2++;
                  count_for_3_q2 = 0;
                }
               // printf("Deleted node: type %d, time %lf\n", temp_node.type, temp_node.time_to_arrive);
                // push_q_arr(&oa_q, temp_node, 2); handle time
                cur_type_in_oa = 2;
                oa_up_time = cur_time + generate_rand_time(min_t4, max_t4);
                continue;

            }
            oa_up_time = -1;
        }
        if (q1_up_time <= q2_up_time)
        {
            usleep(q1_up_time - cur_time);
            cur_time = q1_up_time;
            if (first_q.cur_size == first_q.max_size - 1)
            {
                printf("First queue is full!\n");
                return;
            }

            push_q_arr(&first_q, cur_time, 1);
            count_push_q1++;
            count_push++;
            q1_up_time = cur_time + generate_rand_time(min_t1, max_t1);
        }
        else
        {
            usleep(q2_up_time - cur_time);
            cur_time = q2_up_time;
            if (second_q.cur_size == second_q.max_size - 1)
            {
                printf("Second queue is full!\n");
                return;
            }

            push_q_arr(&second_q, cur_time, 2);
            count_push++;
            count_push_q2++;
            q2_up_time = cur_time + generate_rand_time(min_t2, max_t2);
        }
    }
}

void change_time(int *min_t1, int *max_t1, int *min_t2, int *max_t2, int *min_t3, int *max_t3, int *min_t4, int *max_t4)
{
    printf("Chose interval to change: (T1/T2/T3/T4): ");
    char string[4];
    double t;
    int rc;
    scanf("%s", string);
    if (strcmp(string, "T1") == SUCCESS)
    {
        printf("Input new min of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *min_t1 = t;
        printf("Input new max of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *max_t1 = t;
        return;
    }
    if (strcmp(string, "T2") == SUCCESS)
    {
        printf("Input new min of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *min_t2 = t;
        printf("Input new max of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *max_t2 = t;
        return;
    }
    if (strcmp(string, "T3") == SUCCESS)
    {
        printf("Input new min of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *min_t3 = t;
        printf("Input new max of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *max_t3 = t;
        return;
    }
    if (strcmp(string, "T4") == SUCCESS)
    {
        printf("Input new min of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *min_t4 = t;
        printf("Input new max of interval: ");
        rc = scanf("%lf", &t);
        if (rc != 1 || t < 0)
        {
            printf("Error");
            return;
        }
        *max_t4 = t;
        return;
    }
    printf("Error input\n");
}



int main()
{
    srand(time(NULL));
    arr_queue test_q;
    queue_arr_constructor(&test_q, 3, TEST_ARR_SIZE);
    int size;

    list_queue test_list_q;
    test_list_q.cur_size = 0;
    test_list_q.max_size = TEST_ARR_SIZE;
    test_list_q.pin = NULL;
    test_list_q.type = 3;

    int type;
    double time_arr;
    address_list test_list_address;
    test_list_address.size = 0;
    address_list task_list_adress;
    task_list_adress.size = 0;
    double t;
    address_list q_addr;
    arr_node cur;

    int min_t1 = 1;
    int max_t1 = 5;
    int min_t2 = 0;
    int max_t2 = 3;
    int min_t3 = 0;
    int max_t3 = 4;
    int min_t4 = 0;
    int max_t4 = 1;


    printf("You can do several things with the table entering the special key of the task.\n");
    for (;;)
    {
        printf("\nLIST OF TASKS\n"
               "enter 1 to push to queue by arr\n"
               "enter 2 to pop from queue by arr\n"
               "enter 3 to show queue by arr\n"
               "enter 4 to push to queue by list\n"
               "enter 5 to pop from queue by list\n"
               "enter 6 to show queue by list\n"
               "enter 7 to show adress list for test\n"
               "enter 8 to resolve with use queue by arr.\n"
               "enter 9 to resolve with use queue by list.\n"
               "enter 10 to show adress list for queue\n"
               "enter 11 to change time characteristic\n"
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
                delete_queue(&test_list_q);
                return SUCCESS;
            case 1:
                t = generate_rand_time(min_t1, max_t1);
                push_q_arr(&test_q, t, 3);
                break;

            case 2:
                cur = pop_q_arr(&test_q);
                if (cur.type != -1)
                {
                    printf("type %d\ntime to arrive %lf\n\n", cur.type, cur.time_to_arrive);
                }
                break;

            case 3:
                size = test_q.cur_size;
                if (size == 0)
                    printf("Empty queue!\n");
                else
                    printf("Test queue:\n");
                for(int i = 0; i < size; i++)
                {
                    cur = pop_q_arr(&test_q);
                    if (cur.type != -1)
                    {
                        printf("type %d\ntime to arrive %lf\n\n", cur.type, cur.time_to_arrive);
                    }
                }
                break;

            case 4:
                t = generate_rand_time(min_t1, max_t2);
                push_q_list(&test_list_q, t, 3);
                break;

            case 5:
                pop_q_list(&test_list_q, &time_arr, &type, &test_list_address);
                if (type != -1)
                {
                    printf("type %d\ntime to arrive %lf\n\n", type, time_arr);
                }
                break;

            case 6:
                size = test_list_q.cur_size;
                if (size == 0)
                {
                    printf("Empty queue.");
                    break;
                }
                for (int i = 0; i < size; i++)
                {
                    pop_q_list(&test_list_q, &time_arr, &type, &test_list_address);
                    if (type != -1)
                    {
                        printf("type %d\ntime to arrive %lf\n\n", type, time_arr);
                    }
                }
                break;

            case 7:
                printf("Address list for test queue\n");
                for (int i = 0; i < test_list_address.size; i++)
                    printf("%p\n", test_list_address.buffer[i]);
                break;

            case 8:
                printf("Processing with use queue by arr.\n");
                processing_arr(min_t1, max_t1, min_t2, max_t2, min_t3, max_t3, min_t4, max_t4);
                break;

            case 9:
                printf("Processing with use queue by list.\n");
                processing_list(&task_list_adress, &q_addr, min_t1, max_t1, min_t2, max_t2, min_t3, max_t3, min_t4, max_t4);
                break;

            case 10:
                printf("Show list of free address from resolve.\n");
                printf("Address list for queue\n");
                printf("Adress list size: %d", task_list_adress.size);
                printf("\n");
                for (int i = 0; i < task_list_adress.size; i++)
                    printf("%p\n", task_list_adress.buffer[i]);
                break;

            case 11:
                change_time(&min_t1, &max_t1, &min_t2, &max_t2, &min_t3, &max_t3, &min_t4, &max_t4);
                break;
            }
    }
}
