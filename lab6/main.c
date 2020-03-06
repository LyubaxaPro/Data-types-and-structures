#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "ddp.h"
#include "avl.h"
#include "hash.h"
#include<unistd.h>
#include<time.h>
#include <sys/stat.h>

#define calc_time_for_func(func, sum_time, sum_cmp, count) \
    temp_cmp = 0; \
    start = tick(); \
    func; \
    stop = tick(); \
    sum_time += stop - start; \
    sum_cmp += temp_cmp; \
    count += 1; \

int fill_file()
{
    int n;
    printf("\nEnter number of elements in file: ");
    scanf("%d", &n);
    int used[1000];
    for (int i = 0; i < n; i++)
    {
        used[i] = 0;
    }

    int temp;
    FILE *f = fopen("input.txt", "w");
    for (int i = 0; i < n; i++)
    {
        temp = rand() % n;
        if (used[temp] == 1)
        {
            i--;
            continue;
        }
        used[temp] = 1;
        fprintf(f, "%d ", temp);
    }
    fclose(f);
    return n;
}

int errno = SUCCESS;
#define START_TABLE_SIZE 10

#define CHANGE_STRUCT 123

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

int find_in_file(int data, int *temp_cmp)
{
    FILE *f = fopen("test.txt", "r");
    int rc = 1;
    int temp_value;
    int position = 0;
    while (!feof(f))
    {
        position++;
        rc = fscanf(f, "%d", &temp_value);
        if (rc != TRUE)
        {
            break;
        }
        *temp_cmp += 1;
        if (temp_value == data)
        {
            fclose(f);
            return position;
        }
    }
    fclose(f);
    return -1;
}

void delete_in_file(int data, int *temp_cmp, int *time)
{
    int position;
    int arr[2000];
    int n = 0;
    int rc;
    int temp_value;

    FILE *f = fopen("test.txt", "r");

    int start = tick();
    position = find_in_file(data, temp_cmp);
    if (position == -1)
    {
        int stop = tick();
        *time = stop - start;
    }
    while (!feof(f))
    {
        rc = fscanf(f, "%d", &temp_value);
        if (rc != TRUE)
        {
            break;
        }
        arr[n] = temp_value;
        n++;
    }

    fclose(f);

    if (position != -1)
        *temp_cmp = position;
    else
    {
        *temp_cmp = n;
        return;
    }
    f = fopen("test.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d ", arr[i]);
    fclose(f);
    int stop = tick();
    *time = stop - start;
}

void resolve(int n)
{
    struct stat st;
    ddp_tree_node *ddp_root = NULL;
    avl_tree_node *avl_root = NULL;
    int max_val = -10000;
    int min_val = -10000;
    int sum_find_cmp_ddp = 0;
    int sum_find_cmp_avl = 0;
    int sum_find_cmp_hash = 0;
    int count_find_ddp = 0;
    int count_find_avl = 0;
    int count_find_hash = 0;
    int num_of_collision = 0;
    int time_sum_find_ddp = 0;
    int time_sum_find_avl = 0;
    int time_sum_find_hash = 0;
    clock_t start = 0, stop = 0;
    int time_ddp_delete = 0;
    int time_avl_delete = 0;
    int time_hash_delete = 0;
    int num_cmp_ddp_delete = 0;
    int num_cmp_avl_delete = 0;
    int num_cmp_hash_delete = 0;

    float expected_numb_of_cmp;
    float expected_num_cmp_for_del;
    int val_to_del = 0;

    int sum_del_ddp = 0;
    int sum_del_avl = 0;
    int sum_del_hash = 0;
    int count_del_ddp = 0;
    int count_del_avl = 0;
    int count_del_hash = 0;

    int sum_time_del_ddp = 0;
    int sum_time_del_avl = 0;

    int sum_add_ddp = 0;
    int count_add_ddp = 0;
    int sum_add_avl = 0;
    int count_add_avl = 0;
    int sum_add_hash = 0;
    int count_add_hash = 0;

    int sum_time_add_ddp = 0;
    int sum_time_add_avl = 0;
    int sum_time_add_hash = 0;
    int temp_value;

    hash_table hash_t;

    printf("File data: ");

    int rc;
    FILE *f = fopen("input.txt", "r");
    if (f == NULL)
    {
        errno = FILE_ERROR;
        printf("Error in file");
        return;
    }
    rc = 1;
    while (!feof(f))
    {
        rc = fscanf(f, "%d", &temp_value);
        if (rc != TRUE)
        {
            break;
        }
        printf("%d ", temp_value);
    }

    printf("\nEnter the expected numer of comparisons for find value in hash table: ");
    scanf("%f", &expected_numb_of_cmp);

    if (expected_numb_of_cmp <= 0)
    {
        while (expected_numb_of_cmp <= 0)
        {
            printf("\nEnter the expected numer of compares: ");
            scanf("%f", &expected_numb_of_cmp);
        }
    }

    printf("\nEnter the expected numer of comparisons for delete value from hash table: ");
    scanf("%f", &expected_num_cmp_for_del);

    if (expected_num_cmp_for_del <= 0)
    {
        while (expected_num_cmp_for_del <= 0)
        {
            printf("\nEnter the expected numer of compares: ");
            scanf("%f", &expected_num_cmp_for_del);
        }
    }

    printf("\nEnter numer for delete: ");
    scanf("%d", &val_to_del);

    hash_table_create(&hash_t, START_TABLE_SIZE);
    if (errno != SUCCESS)
        return;
    int temp_cmp;

    rewind(f);
    rc = 1;
    temp_cmp = 0;
    while (!feof(f))
    {
        rc = fscanf(f, "%d", &temp_value);
        if (temp_value < min_val || min_val == -10000)
        {
            min_val = temp_value;
        }

        if (temp_value > max_val || max_val == -10000)
        {
            max_val = temp_value;
        }
        if (rc != TRUE)
        {
            break;
        }
        ddp_add(&ddp_root, temp_value, &temp_cmp);
        insert_in_hash_table(&hash_t, temp_value, &num_of_collision, &temp_cmp);
        count_add_ddp++;
    }

    int c_add_ddp = 0;
    for (int i = n; i < n + 10; i++)
    {
        calc_time_for_func(ddp_add(&ddp_root, i, &temp_cmp), sum_time_add_ddp, sum_add_ddp, c_add_ddp);
        ddp_remove(&ddp_root, i, &temp_cmp);
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// BEFORE DELETE  ////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\nBefore delete\n");
    printf("\nBinary search tree\n");
    if (n < 20)
        print_t_ddp(ddp_root);
    else
        ddp_in_order(ddp_root, 0);

    make_avl_from_ddp(ddp_root, &avl_root, &sum_add_avl);

    sum_add_avl = 0;
    int c_add_avl = 0;
    for (int i = n; i < n + 10; i++)
    {
        calc_time_for_func(avl_insert(&avl_root, i, &temp_cmp), sum_time_add_avl, sum_add_avl, c_add_avl);
        avl_remove(&avl_root, i, &temp_cmp);
    }
    count_add_avl = count_add_ddp;


    printf("Avl tree\n");
    if (n < 20)
        print_t_avl(avl_root);

    else
        avl_in_order(avl_root, 0);

    print_hash_table(&hash_t);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// FIRST DELETE  ////////////////////////////////////////////////////////////////////////////////////////////////////

    int c_add_hash = 0;
    int temp_collision = 0;

    for (int i = n; i < n + 10; i++)
    {
        calc_time_for_func(insert_in_hash_table(&hash_t, i, &temp_collision, &temp_cmp),
                           sum_time_add_hash, sum_add_hash, c_add_hash);
        delete_in_hash_table(&hash_t, i, &temp_cmp);
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// FIND ELEMENT //////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = min_val; i <= max_val; i++)
    {
        calc_time_for_func(ddp_find(ddp_root, i, &temp_cmp), time_sum_find_ddp, sum_find_cmp_ddp, count_find_ddp);
        calc_time_for_func(avl_find(avl_root, i, &temp_cmp), time_sum_find_avl, sum_find_cmp_avl, count_find_avl);
        calc_time_for_func(find_in_hash_table(&hash_t, i, &temp_cmp), time_sum_find_hash, sum_find_cmp_hash, count_find_hash);
    }

    int num_of_del = 5;

    int count = 0;
    num_cmp_ddp_delete = 0;
    time_ddp_delete = 0;
    for (int i = 0; i < num_of_del; i++)
    {
        calc_time_for_func(ddp_remove(&ddp_root, val_to_del, &temp_cmp), time_ddp_delete, num_cmp_ddp_delete, count);
        if (i != num_of_del - 1)
            ddp_add(&ddp_root, val_to_del, &temp_cmp);
    }

    time_avl_delete = 0;
    num_cmp_avl_delete = 0;
    for (int i = 0; i < num_of_del; i++)
    {
        calc_time_for_func(avl_remove(&avl_root, val_to_del, &temp_cmp), time_avl_delete, num_cmp_avl_delete, count);
        if (i != num_of_del - 1)
            avl_insert(&avl_root, val_to_del, &temp_cmp);
    }

    num_cmp_hash_delete = 0;
    time_hash_delete = 0;
    if (find_in_hash_table(&hash_t, val_to_del, &temp_cmp) == -1)
        num_cmp_hash_delete = n * num_of_del;
    for (int i = 0; i < num_of_del; i++)
    {
        calc_time_for_func(delete_in_hash_table(&hash_t, val_to_del, &temp_cmp), time_hash_delete, num_cmp_hash_delete, count);
        if (i != num_of_del - 1)
            insert_in_hash_table(&hash_t, val_to_del, &temp_collision, &temp_cmp);
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// AFTER DELETE //////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n\n\n\nAfter delete %d\n", val_to_del);
    printf("Binary search tree\n");
    if (n < 20)
        print_t_ddp(ddp_root);
    else
        ddp_in_order(ddp_root, 0);
    printf("Avl tree\n");
    if (n < 20)
        print_t_avl(avl_root);
    else
        avl_in_order(avl_root, 0);
   // print_t_avl(avl_root);
    printf("Hash_table\n");
    print_hash_table(&hash_t);

    float mid_time_find_ddp ,  mid_time_find_avl,  mid_time_find_hash;
    float mid_num_find_cmp_ddp, mid_num_find_cmp_avl, mid_num_find_cmp_hash;
    if (count_find_ddp != 0)
    {
        mid_num_find_cmp_ddp = sum_find_cmp_ddp * 1.0 / count_find_ddp;
        mid_time_find_ddp = time_sum_find_ddp * 1.0 / count_find_ddp;
    }
    else
    {
        mid_num_find_cmp_ddp = 0;
        mid_time_find_ddp = 0;
     }

    if (count_find_avl != 0)
    {
        mid_num_find_cmp_avl = sum_find_cmp_avl * 1.0 / count_find_avl;
        mid_time_find_avl = time_sum_find_avl * 1.0 / count_find_avl;
    }
    else
    {
        mid_num_find_cmp_avl = 0;
        mid_time_find_avl = 0;
    }

    if (count_find_hash != 0)
    {
        mid_num_find_cmp_hash = sum_find_cmp_hash * 1.0 / count_find_hash;
        mid_time_find_hash = time_sum_find_hash * 1.0 / count_find_hash;
    }
    else
    {
        mid_num_find_cmp_hash = 0;
        mid_time_find_hash = 0;
    }

    for(int i = min_val; i < max_val; i++)
    {
        temp_cmp = 0;
        if (ddp_find(ddp_root, i, &temp_cmp))
        {
            calc_time_for_func(ddp_remove(&ddp_root, i, &temp_cmp), sum_time_del_ddp, sum_del_ddp, count_del_ddp);
            ddp_add(&ddp_root, i, &temp_cmp);
//            temp_cmp = 0;
//            start = tick();
//            ddp_remove(&ddp_root, i, &temp_cmp);
//            stop = tick();
//            sum_time_del_ddp += stop - start;
//            sum_del_ddp += temp_cmp;
//            count_del_ddp++;
//            ddp_add(&ddp_root, i, &temp_cmp);
        }
    }

    for(int i = min_val; i < max_val; i++)
    {
        temp_cmp = 0;
        if (avl_find(avl_root, i, &temp_cmp))
        {
            calc_time_for_func(avl_remove(&avl_root, i, &temp_cmp), sum_time_del_avl, sum_del_avl, count_del_avl);
//            temp_cmp = 0;
//            start = tick();
//            avl_remove(&avl_root, i, &temp_cmp);
//            stop = tick();
//            sum_time_del_avl += stop - start;
//            sum_del_avl += temp_cmp;
//            count_del_avl++;
            avl_insert(&avl_root, i, &temp_cmp);
        }
    }

    printf("\n\n\n\nComparison of structures\n");

    printf("\n======Binary search tree======\n\n");
    printf("For delete %d\n", val_to_del);
    printf("Num of comparisons = %d     memory %ld byte     time %d tick\n", num_cmp_ddp_delete / num_of_del, sizeof(ddp_tree_node) * count_add_ddp,  time_ddp_delete / num_of_del);

    if (count_add_ddp == 0)
        count_add_ddp = 1;
    if (count_del_ddp == 0)
        count_del_ddp = 1;

    printf("\nFind\n");
    printf("Midle num of comparison %f     memory %ld byte     middle time %f tick\n", mid_num_find_cmp_ddp, sizeof(ddp_tree_node) * count_add_ddp,  mid_time_find_ddp);

    printf("\nAdd\n");
    printf("Midle num of comparison %f     memory %ld byte     middle time %f tick\n", sum_add_ddp * 1.0 / c_add_ddp , sizeof(ddp_tree_node) * count_add_ddp,  sum_time_add_ddp * 1.0 / c_add_ddp);

    printf("\nDelete\n");
    printf("Midle num of comparison %f     memory %ld byte     middle time %f tick\n", sum_del_ddp * 1.0 / count_del_ddp , sizeof(ddp_tree_node) * count_add_ddp,  sum_time_del_ddp * 1.0 / count_del_ddp);


    printf("\n\n======Avl tree======\n\n");
    printf("For delete %d\n", val_to_del);
    printf("Num of comparisons %d     memory %ld byte     time %d tick\n", num_cmp_avl_delete / num_of_del, sizeof(avl_tree_node) * count_add_avl,  time_avl_delete / num_of_del);

    printf("\nFind\n");
    printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick\n", mid_num_find_cmp_avl, sizeof(avl_tree_node) * count_add_avl,  mid_time_find_avl);

    if (count_add_avl == 0)
        count_add_avl = 1;

    if (count_del_avl == 0)
        count_del_avl = 1;

    printf("\nAdd\n");
    printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick\n", sum_add_avl * 1.0 / c_add_avl, sizeof(avl_tree_node) * count_add_avl,  sum_time_add_avl * 1.0 / c_add_avl);


    printf("\nDelete\n");
    printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick\n", sum_del_avl * 1.0 / count_del_avl, sizeof(avl_tree_node) * count_add_avl,  sum_time_del_avl * 1.0 / count_del_avl);

    printf("\n\n======Hash table======\n");
    printf("\nNumber of collisions %d\n\n", num_of_collision);
    printf("For delete %d\n", val_to_del);
    printf("Num of comparisons %d     memory %ld byte     time %d tick\n", num_cmp_hash_delete / num_of_del, sizeof(int) * hash_t.table_size + sizeof(hash_t),  time_hash_delete / num_of_del);

    printf("\nFind\n");
    printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick\n", mid_num_find_cmp_hash,  sizeof(int) * hash_t.table_size + sizeof(hash_t),  mid_time_find_hash);

    if (mid_num_find_cmp_hash > expected_numb_of_cmp || num_cmp_hash_delete > expected_num_cmp_for_del)
    {
        printf("\n***Rebuild hash table with new hash function***\n");
        temp_cmp = 0;
        insert_in_hash_table(&hash_t, val_to_del, &num_of_collision, &temp_cmp);

        num_of_collision = change_hash_function(&hash_t);
        print_hash_table(&hash_t);
        sum_find_cmp_hash = 0;
        count_find_hash = 0;
        time_sum_find_hash = 0;
        for (int i = min_val; i <= max_val; i++)
        {
//            temp_cmp = 0;
//            start = tick();
//            find_in_hash_table(&hash_t, i, &temp_cmp);
//            stop = tick();
//            time_sum_find_hash += stop - start;
//            sum_find_cmp_hash += temp_cmp;
//            count_find_hash++;
            calc_time_for_func(find_in_hash_table(&hash_t, i, &temp_cmp), time_sum_find_hash, sum_find_cmp_hash, count_find_hash);
        }

        num_cmp_hash_delete = 0;
        time_hash_delete = 0;
        for (int i = 0; i < num_of_del; i++)
        {
            calc_time_for_func(delete_in_hash_table(&hash_t, val_to_del, &temp_cmp), time_hash_delete, num_cmp_hash_delete, count);
            if (i != num_of_del - 1)
                insert_in_hash_table(&hash_t, val_to_del, &count, &temp_cmp);
        }

//        start = tick();
//        delete_in_hash_table(&hash_t, val_to_del, &num_cmp_hash_delete);
//        stop = tick();
//        time_hash_delete = stop - start;

        if (count_find_hash != 0)
        {
            mid_num_find_cmp_hash = sum_find_cmp_hash * 1.0 / count_find_hash;
            mid_time_find_hash = time_sum_find_hash * 1.0 / count_find_hash;
        }
        else
        {
            mid_num_find_cmp_hash = 0;
            mid_time_find_hash = 0;
        }
        printf("\n\n======Hash table======\n\n");
        printf("\nNumber of collisions %d\n\n", num_of_collision);
        printf("For delete %d\n", val_to_del);
        printf("Num of comparisons %d     memory %ld byte     time %d tick\n", num_cmp_hash_delete / num_of_del,  sizeof(int) * hash_t.table_size + sizeof(hash_t),  time_hash_delete / num_of_del);

        printf("\nFind\n");
        printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick\n", mid_num_find_cmp_hash,  sizeof(int) * hash_t.table_size + sizeof(hash_t),  mid_time_find_hash);
    }

    if (count_add_hash == 0)
        count_add_hash = 1;

    printf("\nAdd\n");
    printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick", sum_add_hash * 1.0 / c_add_hash,  sizeof(int) * hash_t.table_size + sizeof(hash_t),  sum_time_add_hash * 1.0 / c_add_hash);

    time_hash_delete = 0;
    count_del_hash = 0;
    sum_del_hash = 0;
    for (int i = min_val; i < max_val; i++)
    {
        calc_time_for_func(delete_in_hash_table(&hash_t, i, &temp_cmp), time_hash_delete, sum_del_hash, count_del_hash);
        insert_in_hash_table(&hash_t, i, &num_of_collision, &temp_cmp);
//        temp_cmp = 0;
//        start = tick();
//        delete_in_hash_table(&hash_t, i, &temp_cmp);
//        stop = tick();
//        insert_in_hash_table(&hash_t, i, &num_of_collision, &temp_cmp);
//        time_hash_delete += stop - start;
//        sum_del_hash += temp_cmp;
//        count_del_hash++;
    }

    if (count_del_hash == 0)
        count_del_hash = 1;

    printf("\nDelete\n");
    printf("Middle num of comparisons %f     memory %ld byte     middle time %f tick\n", sum_del_hash * 1.0 / count_del_hash,  sizeof(int) * hash_t.table_size + sizeof(hash_t), time_hash_delete * 1.0 / count_del_hash);

    delete_tree_avl(&avl_root);
    delete_tree_ddp(&ddp_root);
    free(hash_t.arr);

    rewind(f);
    FILE *f_test = fopen("test.txt", "w");

    int count_add_file = 0;
    int sum_add_time_file = 0;

    rc = 1;
    while (!feof(f))
    {
        rc = fscanf(f, "%d", &temp_value);
        if (rc != TRUE)
        {
            break;
        }
        calc_time_for_func(fprintf(f_test, "%d ", temp_value), sum_add_time_file, temp_cmp, count_add_file);
//        temp_cmp = 0;
//        start = tick();
//        fprintf(f_test, "%d ", temp_value);
//        stop = tick();
//        sum_add_time_file += stop - start;
//        count_add_file++;
    }

    fclose(f_test);

    int sum_find_file = 0;
    int count_find_file = 0;
    int sum_find_time_file = 0;

    for (int i = min_val; i < max_val; i++)
    {
        calc_time_for_func(find_in_file(i, &temp_cmp), sum_find_time_file, sum_find_file, count_find_file);
//        temp_cmp = 0;
//        start = tick();
//        find_in_file(i, &temp_cmp);
//        stop = tick();
//        count_find_file++;
//        sum_find_file += temp_cmp;
//        sum_find_time_file += stop - start;
    }

    int file_del_val_to_del = 0;
    int time_del_val_to_del = 0;
    delete_in_file(val_to_del, &file_del_val_to_del, &time_del_val_to_del);

    int temp_time = 0;
    int time_file_del = 0;
    int c = 0;
    long long int sum_count_file_del = 0;
    int count_file_del = 0;
    for(int i = min_val; i < max_val; i++)
    {
        temp_cmp = 0;
        delete_in_file(i, &temp_cmp, &temp_time);
        time_file_del += temp_time;
        count_file_del++;
        sum_count_file_del += temp_cmp;
        c++;
        if (c == 10)
            break;
    }

    stat("input.txt", &st);
    int size = st.st_size;

    printf("\n\n======File======\n\n");
    printf("For delete %d\n", val_to_del);
    printf("Num of comparisons %d     memory %d byte     time %d tick\n", file_del_val_to_del / num_of_del, size,  time_del_val_to_del / (10 * num_of_del));

    if (count_find_file == 0)
        count_find_file = 1;

    printf("Find\n");
    printf("Middle num of comparisons %f     memory %d byte     middle time %f tick\n", sum_find_file * 1.0 / count_find_file, size,  (sum_find_time_file * 1.0) / (count_find_file));

    if (count_add_file == 0)
        count_add_file = 1;

    printf("\nAdd\n");
    printf("memory %d byte     middle time %f tick\n", size,  sum_add_time_file * 1.0 / (count_add_file));

    if (count_file_del == 0)
        count_file_del = 1;

    printf("\nDelete\n");
    printf("Middle num of comparisons %f     memory %d byte     middle time %d tick\n", sum_count_file_del * 1.0 / count_file_del, size, time_file_del / (count_file_del));


    fclose(f);
    fclose(f_test);
}

int work_with_ddp(ddp_tree_node *test_ddp_root, avl_tree_node *test_avl_root)
{
    int temp_value;
    int temp_cmp = 0;
    int rc, k;
    ddp_tree_node *temp_ddp_node;
    for(;;)
    {
        printf("You can do several things with the bst tree entering the special key of the task.\n");
        printf("\nLIST OF TASKS\n"
               "enter 0 to close the program\n\n"
               "enter 1 to add value in bst tree\n"
               "enter 2 to find value in bst tree\n"
               "enter 3 to delete node from bst tree\n"
               "enter 4 to make avl tree from bst tree\n"
               "enter 5 to paint bst tree\n"
               "enter 6 to delete bst tree\n\n"

               "enter 11 to print bst in pre_order\n"
               "enter 12 to print bst in in_order\n"
               "enter 13 to print bst in post_order\n"
               "enter 22 to chose another structure\n");

        rc = scanf("%d", &k);
        if (rc != 1)
        {
            printf("Not correct input\n");
            return CORRECT_ERROR;
        }
        switch(k)
        {
            case 0:
                return 0;

            case 1:
                printf("\nEnter value you want to add: ");
                scanf("%d", &temp_value);
                ddp_add(&test_ddp_root, temp_value, &temp_cmp);
                break;

            case 2:
                printf("\nEnter value you want to find: ");
                scanf("%d", &temp_value);
                temp_ddp_node = ddp_find(test_ddp_root, temp_value, &temp_cmp);
                if (temp_ddp_node == NULL)
                {
                    printf("\nDont have this value in  bst tree!\n");
                    break;
                }
                printf("value = %d\n\nleft_ptr = %p\nright_ptr = %p\n", temp_ddp_node->data, temp_ddp_node->left, temp_ddp_node->right);
                break;

            case 3:
                printf("\nEnter value you want to delete: ");
                scanf("%d", &temp_value);
                ddp_remove(&test_ddp_root, temp_value, &temp_cmp);
                break;

            case 4:
                delete_tree_avl(&test_avl_root);
                test_avl_root = NULL;
                make_avl_from_ddp(test_ddp_root, &test_avl_root, &temp_cmp);
                if (test_avl_root != NULL)
                    print_t_avl(test_avl_root);
                else
                    printf("\nTree is empty!\n");
                break;

            case 5:
                if (test_ddp_root != NULL)
                    print_t_ddp(test_ddp_root);
                else
                    printf("\nTree is empty\n");
                break;

            case 6:
                delete_tree_ddp(&test_ddp_root);
                break;

            case 12:
                printf("\nBinary search tree in in-order\n");
                if (test_ddp_root == NULL)
                {
                    printf("Binary search tree is empty!");
                }
                else
                    ddp_in_order(test_ddp_root, 0);
                break;
            case 22:
                return CHANGE_STRUCT;
        }
    }
}

int work_with_avl(avl_tree_node *test_avl_root) {
    int temp_value;
    int temp_cmp = 0;
    int rc, k;
    avl_tree_node *temp_avl_node;
    for (;;)
    {
        printf("You can do several things with the avl tree entering the special key of the task.\n");
        printf("\nLIST OF TASKS\n"
               "enter 0 to close the program\n\n"
               "enter 1 to add value in avl tree\n"
               "enter 2 to find value in avl tree\n"
               "enter 3 to delete node from avl tree\n"
               "enter 4 to paint avl tree\n"
               "enter 5 to delete avl tree\n\n"

               "enter 11 to print avl in pre_order\n"
               "enter 12 to print avl in in_order\n"
               "enter 13 to print avl in post_order\n"
               "enter 22 to chose another structure\n");

        rc = scanf("%d", &k);
        if (rc != 1)
        {
            printf("Not correct input\n");
            return CORRECT_ERROR;
        }
        switch(k)
        {
            case 0:
                return 0;

            case 1:
                printf("\nEnter value you want to add: ");
                scanf("%d", &temp_value);
                avl_insert(&test_avl_root, temp_value, &temp_cmp);
                break;

            case 2:
                printf("\nEnter value you want to find: ");
                scanf("%d", &temp_value);
                temp_avl_node= avl_find(test_avl_root, temp_value, &temp_cmp);
                if (temp_avl_node == NULL)
                {
                    printf("\nDont have this value in  ddp tree!\n");
                    break;
                }
                printf("value = %d\nleft_ptr = %p\nright_ptr = %p\nbalance = %d\n", temp_avl_node->data,
                       temp_avl_node->left, temp_avl_node->right, temp_avl_node->balance);
                break;

            case 3:
                printf("\nEnter value you want to delete: ");
                scanf("%d", &temp_value);
                avl_remove(&test_avl_root, temp_value, &temp_cmp);
                break;

            case 4:
                if (test_avl_root != NULL)
                    print_t_avl(test_avl_root);
                else
                    printf("\nTree is empty!\n");
                break;

            case 5:
                delete_tree_avl(&test_avl_root);
                test_avl_root = NULL;
                break;

            case 12:
                printf("\nAvl in in-order\n");
                if (test_avl_root == NULL)
                {
                    printf("Avl tree is empty!");
                }
                else
                    avl_in_order(test_avl_root, 0);
                break;
            case 22:
                return CHANGE_STRUCT;
        }
    }
}

int work_with_hash_table() {
    hash_table hash_t;
    int temp_value;
    int rc;
    int k;
    int num_of_colision = 0;
    int temp_cmp = 0;
    hash_table_create(&hash_t, START_TABLE_SIZE);
    if (errno != SUCCESS)
        return MALLOC_ERROR;
    for(;;)
    {
        printf("You can do several things with the hash table entering the special key of the task.\n");
        printf("\nLIST OF TASKS\n"
               "enter 0 to close the program\n\n"
               "enter 1 to add value in hash table\n"
               "enter 2 to find value in hash table\n"
               "enter 3 to delete node from hash table\n"
               "enter 4 to rebuild hash table\n"
               "enter 5 to clean hash table\n\n"

               "enter 11 to print hash table\n\n");

        rc = scanf("%d", &k);
        if (rc != 1)
        {
            printf("Not correct input\n");
            return CORRECT_ERROR;
        }
        switch(k)
        {
            case 0:
                return 0;

            case 1:
                printf("\nEnter value you want to add: ");
                scanf("%d", &temp_value);
                insert_in_hash_table(&hash_t, temp_value, &num_of_colision, &temp_cmp);
            break;

            case 2:
                printf("\nEnter value you want to find: ");
                scanf("%d", &temp_value);
                if (find_in_hash_table(&hash_t, temp_value, &temp_cmp) == TRUE)
                {
                    printf("\nThis value is in table\n");
                }
                else
                {
                    printf("\nDont have this value in table\n");
                }
            break;

            case 3:
                printf("\nEnter value you want to delete: ");
                scanf("%d", &temp_value);
                if (delete_in_hash_table(&hash_t, temp_value, &temp_cmp) ==  TRUE)
                    printf("\nValue is deleted from hash table\n");
                else
                    printf("\nDont have this value in hash table\n");
            break;

            case 4:
                printf("\nOld hash table\n");
                print_hash_table(&hash_t);
                if (change_hash_function(&hash_t) == FALSE)
                    break;
                printf("\nNew hash table\n");
                print_hash_table(&hash_t);\
            break;

            case 5:
                free(hash_t.arr);
                hash_table_create(&hash_t, START_TABLE_SIZE);
            break;

            case 11:
                printf("\nHash table\n");
                print_hash_table(&hash_t);
            break;

            case 22:
                return CHANGE_STRUCT;
        }
    }
}

int main()
{
    srand(time(NULL));
    ddp_tree_node *test_ddp_root = NULL;
    avl_tree_node *test_avl_root = NULL;
    int n = 10;
    printf("You can do several things with the table entering the special key of the task.\n");
    for (;;)
    {
        printf("\nLIST OF TASKS\n"
               "enter 0 to close the program\n\n"
               "enter 1 to work with binary search tree tree\n"
               "enter 2 to work with avl tree\n"
               "enter 3 to work with hash-table\n"
               "enter 4 to compare work with structs\n"
               "enter 5 to fill file by random\n"
               );

        int s, rc, return_code;
        rc = scanf("%d", &s);
        if (rc != 1)
        {
            printf("Not correct input\n");
            return CORRECT_ERROR;
        }
        switch(s)
        {
            case 0:
                delete_tree_avl(&test_avl_root);
                delete_tree_ddp(&test_ddp_root);
                return 0;

            case 1:
                return_code = work_with_ddp(test_ddp_root, test_avl_root);
                if (return_code != CHANGE_STRUCT)
                    return return_code; // добавить удаление деревьев при ретурне
                break;
            case 2:
                return_code = work_with_avl(test_avl_root);
                if (return_code != CHANGE_STRUCT)
                    return return_code;
                break;
            case 3:
                return_code = work_with_hash_table();
                if (return_code != CHANGE_STRUCT)
                    return return_code;
                break;

            case 4:
                resolve(n);
            break;

            case 5:
                n = fill_file();
            break;

        }

    }
}
