#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "hash.h"
#include "math.h"
extern int errno;
#define LOAD_FACTOR 0.5

int HASHING_MODE = 0;

int Hash_UInt_M3(long long key, int size)
{
  key ^= (key << 13);
  key ^= (key >> 17);
  key ^= (key << 5);
  return abs(key % size);
}

int hash6432shift(long long key, int size)
{
  key = (~key) + (key << 18); // key = (key << 18) - key - 1;
  key = key ^ (key >> 31);
  key = key * 21; // key = (key + (key << 2)) + (key << 4);
  key = key ^ (key >> 11);
  key = key + (key << 6);
  key = key ^ (key >> 22);
  return abs(key % size);
}

int hash32shiftmult(int key, int size)
{
  int c2 = 0x27d4eb2d; // a prime or an odd constant
  key = (key ^ 61) ^ (key >> 16);
  key = key + (key << 3);
  key = key ^ (key >> 4);
  key = key * c2;
  key = key ^ (key >> 15);
  return abs(key % size);
}

int hash(int key, int table_size) {
    switch (HASHING_MODE) {
    case 0:
        return Hash_UInt_M3(key, table_size);
    case 1:
        return hash6432shift(key, table_size);
    case 2:
        return hash32shiftmult(key, table_size);
    }
    return TRUE;
}

void clean_hash_table(data_record *arr, int arr_size)
{
    for (int i = 0; i < arr_size; i++)
    {
        arr[i].is_free = TRUE;
        arr[i].is_deleted = FALSE;
    }
}

void hash_table_create(hash_table *hash_t, int size)
{
    data_record *temp = (data_record*)malloc(size * sizeof(data_record));
    if (temp == NULL)
    {
        errno = MALLOC_ERROR;
        printf("Malloc error!\n");
        return;
    }
    hash_t->arr = temp;
    hash_t->table_size = size;
    hash_t->cur_size = 0;
    clean_hash_table(hash_t->arr, hash_t->table_size);
}

void resize_hash_table(hash_table *hash_t) {
    hash_table temp_hash_t;
    hash_table_create(&temp_hash_t, hash_t->table_size * 2);
    int num = 0;
    int temp_cmp = 0;

    // перезаписываем сатрые значения в новую табличу с новой хэшфункцией
    for (int i = 0; i < hash_t->table_size; ++i) {
        if (!hash_t->arr[i].is_free && hash_t->arr[i].is_deleted != TRUE) {
            insert_in_hash_table(&temp_hash_t, hash_t->arr[i].data, &num, &temp_cmp);
        }
    }
    // обновляем старую хэштаблицу
    free(hash_t->arr);
    *hash_t = temp_hash_t;
}

void insert_in_hash_table(hash_table *hash_t, int data, int *num_of_collision, int *temp_cmp)
{
    if (hash_t->cur_size * 1.0 / hash_t->table_size > LOAD_FACTOR)
    {
        resize_hash_table(hash_t);
    }
    int in_table = find_in_hash_table(hash_t, data, temp_cmp);
    if(in_table) {
        return;
    }
    int index = hash(data, hash_t->table_size);
    if (hash_t->arr[index].is_free == FALSE) *num_of_collision += 1;
    while (hash_t->arr[index].is_free == FALSE)
    {
        *temp_cmp += 1;
        index = (index + 1) % hash_t->table_size;
    }
    hash_t->arr[index].data = data;
    hash_t->arr[index].is_free = FALSE;
    hash_t->arr[index].is_deleted = FALSE;
    hash_t->cur_size++;

}

int find_in_hash_table(hash_table *hash_t, int data, int *temp_cmp)
{
    int index = hash(data, hash_t->table_size);
    while (hash_t->arr[index].is_free == FALSE || hash_t->arr[index].is_deleted == TRUE)
    {
        *temp_cmp += 1;
        if(hash_t->arr[index].is_deleted && hash_t->arr[index].data
                == data)
            return FALSE;
        if (hash_t->arr[index].data == data)
            return TRUE;
        index = (index + 1) % hash_t->table_size;
    }
    return FALSE;
}


int delete_in_hash_table(hash_table *hash_t, int data, int *num_cmp)
{
    int index = hash(data, hash_t->table_size);
    while (hash_t->arr[index].is_free == FALSE || hash_t->arr[index].is_deleted)
    {
        *num_cmp += 1;
        if (hash_t->arr[index].is_deleted)
        {
            if (hash_t->arr[index].data == data)
                return FALSE;
        }
        if (hash_t->arr[index].data == data)
        {
            hash_t->arr[index].is_free = TRUE;
            hash_t->arr[index].is_deleted = TRUE;
            --hash_t->cur_size;
            return TRUE; // вернули 1 - удачно удалили элемент
        }
        index = (index + 1) % hash_t->table_size;
    }
    return FALSE;
}

int change_hash_function(hash_table *hash_t)
{
    HASHING_MODE++;
    int num_of_collision = 0;
    if (HASHING_MODE >= 2)
    {
        printf("\nDont have another hash function\n");
        return FALSE;
    }

    hash_table temp_hash_t;
    int temp_cmp = 0;
    hash_table_create(&temp_hash_t, hash_t->table_size);

    // перезаписываем старые значения в новую табличу с новой хэшфункцией
    for (int i = 0; i < hash_t->table_size; ++i) {
        if (!hash_t->arr[i].is_free && hash_t->arr[i].is_deleted != TRUE) {
            insert_in_hash_table(&temp_hash_t, hash_t->arr[i].data, &num_of_collision, &temp_cmp);
        }
    }
    // обновляем старую хэштаблицу
    free(hash_t->arr);
    *hash_t = temp_hash_t;
    return num_of_collision;
}

void print_hash_table(hash_table *hash_t)
{
    printf("\nHash table\n");
    printf("Hash table alloceted size = %d, Hash table size = %d\n", hash_t->table_size, hash_t->cur_size);

    printf("\nHash table: ");
    for (int i = 0; i < hash_t->table_size; i++)
    {
        if (hash_t->arr[i].is_free == FALSE)
            printf("  %d (%d)    ", hash_t->arr[i].data, hash(hash_t->arr[i].data, hash_t->table_size));
        else
            printf("_ ");
    }
    printf("\n");
}
