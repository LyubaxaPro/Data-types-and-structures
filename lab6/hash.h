#ifndef HASH_H
#define HASH_H

#include "structs.h"
// hash functions
int hash6432shift(long long key, int size);
int Hash_UInt_M3(long long key, int size);
int hash32shiftmult(int key, int size);
// hash table functions
int hash(int key, int table_size);
void clean_hash_table(data_record *arr, int arr_size);
void insert_in_hash_table(hash_table *hash_t, int data, int *num_of_collision, int *temp_cmp);
int find_in_hash_table(hash_table *hash_t, int data, int *temp_cmp);
int delete_in_hash_table(hash_table *hash_t, int data, int *num_cmp);
int change_hash_function(hash_table *hash_t);
void print_hash_table(hash_table *hash_t);
void hash_table_create(hash_table *hash_t, int size);


#endif // HASH_H
