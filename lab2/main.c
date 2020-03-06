#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define STR_SIZE 30
#define STRING_SIZE 330
#define MIN_COUNT 40
#define MAX_COUNT 2040
#define ERROR_REGISTR 1
#define ERROR_SYMBOL 2
#define SUCCESS 0
#define TABLE_IS_FULL 6
#define SIZE_ERROR 3
#define EMPTY_INPUT 4
#define CORRECT_ERROR 5

typedef struct
{
    int new_table_position;
    int old_table_position;
    int price;
}table_keys;

typedef struct
{
    int garanty;
}new_st;

typedef struct
{
    int year_of_car_manufacture;
    int mileage;
    int number_of_repairs;
    int number_of_car_owners;
}old_st;

typedef union
{
    new_st new_s;
    old_st old;
}state;


typedef struct
{
    int index;
    char brand[STR_SIZE];  // Марка машины
    char country[STR_SIZE];  // Страна-производитель
    char color[STR_SIZE]; // Цвет
    int price; // Цена
    char condition[STR_SIZE]; // Состояние
    state condition_st;
}automobiles;

void rewrite_file(automobiles* table, const int temp_size)
{

    FILE* f = fopen("new_table.txt", "w");
    if (f == NULL)
    {
        printf("Cant find file.\n");
        return;
    }
    rewind(f);
    for (int i = 0; i < temp_size; i++)
    {
        fprintf(f, "%s %s %s %d %s ", table[i].brand, table[i].country, table[i].color, table[i].price, table[i].condition);
        if (strcmp(table[i].condition, "new") == SUCCESS)
            fprintf(f, "%d\n", table[i].condition_st.new_s.garanty);
        else
            fprintf(f, "%d %d %d %d\n", table[i].condition_st.old.year_of_car_manufacture, table[i].condition_st.old.mileage,
                    table[i].condition_st.old.number_of_repairs, table[i].condition_st.old.number_of_car_owners);
    }
    fclose(f);
}

int fill_str(char *p_string, char *string, int first_is_upper)
{
    if (string[0] == '\n')
    {
        printf("\nEmpty input.\n");
        return EMPTY_INPUT;
    }
    int is_not_file = 0;
    int j = 0; int k = 0;
    for(j = 0; string[j] != ',' && string[j] != '\n'; j++)
    {
        if (string[j] == '\0')
        {
            is_not_file = 1;
            break;
        }
        if (j == 0 && first_is_upper)
        {
            if (string[j] >= 'A' && string[j] <= 'Z')
            {
                p_string[j] = string[j];
                continue;
            }
            if (string[j] >= 'a' && string[j] <= 'z')
            {
                printf("Not correct registr of char.\n");
                return ERROR_REGISTR;
            }
            else
            {
                printf("Not correct symbol of char.\n");
                return ERROR_SYMBOL;
            }
        }
        if (!first_is_upper)
        {
            if ((string[j] >= 'a' && string[j] <= 'z') || (string[j] == ' '))
            {
                p_string[j] = string[j];
                continue;
            }
            else
            {
                printf("Not correct symbol of char.\n");
                return ERROR_SYMBOL;
            }

        }
        if ((j != 0 || first_is_upper) && ((string[j] >= 'a' && string[j] <= 'z') || (string[j] >= 'A' && string[j] <= 'Z') || string[j] == ' '))
        {
            p_string[j] = string[j];
        }
        else
        {
            printf("\nNot correct symbol of char.\n");
            return ERROR_SYMBOL;
        }
    }
    p_string[j] = '\0';
    j += 2;
    if (!is_not_file)
    {
        for (k = 0; k < strlen(string) - j; k++)
        {
            string[k] = string[k + j];
        }
    }
    return SUCCESS;
}

int fill_num(int* num, char* string)
{
    int is_not_file = 0;
    int j = 0; int k = 0;
    if (string[0] == '\n')
    {
        printf("\nEmpty input.\n");
        return EMPTY_INPUT;
    }
    *num = 0;
    for(j = 0; string[j] != ',' && string[j] != '\n'; j++)
    {
        if (string[j] == '\0')
        {
            is_not_file = 1;
            break;
        }
        if (string[j] >= '0' && string[j] <= '9')
        {
            *num = *num * 10 + string[j] - '0';
        }
        else
        {
            printf("\nNot correct symbol.\n");
            return ERROR_SYMBOL;
        }
    }
    j += 2;
    if (!is_not_file)
    {
        for (k = 0; k < strlen(string) - j; k++)
        {
            string[k] = string[k + j];
        }
    }
    return SUCCESS;
}

int string_parse(char* string, automobiles* table)
{
    int temp;
    temp = fill_str(table->brand, string, 1);
    if (temp != SUCCESS)
    {
        printf("\nInput error\n");
        return temp;
    }
    //temp = fill_str(table->country, string, 1);
    if ((temp = fill_str(table->country, string, 1)) != SUCCESS)
    {
        printf("\nInput error\n");
        return temp;
    }
    temp = fill_str(table->color, string, 0);
    if (temp != SUCCESS)
    {
        printf("\nInput error\n");
        return temp;
    }
    temp = fill_num(&table->price, string);
    if (temp != SUCCESS)
    {
        printf("\nInput error\n");
        return temp;
    }
    temp = fill_str(table->condition, string, 0);
    if (temp != SUCCESS)
    {
        printf("\nInput error\n");
        return temp;
    }
    if (strcmp(table->condition, "new") == SUCCESS)
    {
        fill_num(&table->condition_st.new_s.garanty, string);
        return SUCCESS;
    }
    if (strcmp(table->condition, "old") == SUCCESS)
    {
        fill_num(&table->condition_st.old.year_of_car_manufacture, string);
        if (table->condition_st.old.year_of_car_manufacture > 2019)
        {
            printf("\nNot correct year of car manufacture\n");
            return CORRECT_ERROR;
        }
        temp = fill_num(&table->condition_st.old.mileage, string);
        if (temp != SUCCESS)
        {
            printf("\nInput error\n");
            return temp;
        }
        temp = fill_num(&table->condition_st.old.number_of_repairs, string);
        if (temp != SUCCESS)
        {
            printf("\nInput error\n");
            return temp;
        }
        temp = fill_num(&table->condition_st.old.number_of_car_owners, string);
        if (temp != SUCCESS)
        {
            printf("\nInput error\n");
            return temp;
        }
        return SUCCESS;

    }
    printf("%s\n", table->condition);
    printf("\nNot correct condition of car.\n");
    return CORRECT_ERROR;
}


int filling_table_from_file(automobiles* table, int size)  // Считывание в таблицу данных из файла
{
    int count = 0;
    FILE *f = fopen("table.txt", "rt");
    if (f == NULL)
    {
        printf("Cant find file.\n");
        return CORRECT_ERROR;
    }
    rewind(f);
    for(int i = 0; i < size; i++)
    {
        if (feof(f))
        {
            break;
        }
        char string[STRING_SIZE] = "";
        fgets(string, 250, f);
        int rc = string_parse(string, &table[count]);
        if (rc != SUCCESS)
        {
            fclose(f);
            return rc;
        }
        table[count].index = count;
        count++;
    }
    fclose(f);
    return SUCCESS;
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

void print_tab(automobiles* table, const int temp_table_size)
{
    printf("\nTOTAL: %d\n\n", temp_table_size);
    printf("┌------------------------------------------------------------------------------------------------------------------------------------------------------┐\n");
    printf("|  №       |      Brand         |    Country    |      Color     |  Price   |  Status  | Garanty | Manufacture year|     Mileage   | Repairs |  Owners |\n");
    printf("|----------|--------------------|---------------|----------------|----------|----------|---------|-----------------|---------------|---------|---------|\n");
    for (int i = 0; i < temp_table_size; i++)
    {
        printf("| %d", table[i].index + 1);
        for (int j = 0; j < 9 - find_len_int(table[i].index + 1); j++)
            printf(" ");
        printf("|");
        printf(" %s", table[i].brand);
        for(int j = 0; j < 19 - strlen(table[i].brand); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %s", table[i].country);
        for(int j = 0; j < 14 - strlen(table[i].country); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %s", table[i].color);
        for(int j = 0; j < 15 - strlen(table[i].color); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %d", table[i].price);
        for(int j = 0; j < 9 - find_len_int(table[i].price); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %s", table[i].condition);
        for(int j = 0; j < 9 - strlen(table[i].condition); j++)
        {
            printf(" ");
        }
        if (strcmp(table[i].condition, "new") == SUCCESS)
        {
            printf("|");
            printf(" %d", table[i].condition_st.new_s.garanty);
            for(int j = 0; j < 8 - find_len_int(table[i].condition_st.new_s.garanty); j++)
            {
                printf(" ");
            }
            printf("|                 |               |         |         |\n");
        }
        else
        {
            printf("|");
            for(int j = 0; j < 9; j++)
                printf(" ");
            printf("|");
            printf(" %d", table[i].condition_st.old.year_of_car_manufacture);
            for(int j = 0; j < 16 - find_len_int(table[i].condition_st.old.year_of_car_manufacture); j++)
            {
                printf(" ");
            }
            printf("|");
            printf(" %d", table[i].condition_st.old.mileage);
            for(int j = 0; j < 14 - find_len_int(table[i].condition_st.old.mileage); j++)
            {
                printf(" ");
            }
            printf("|");
            printf(" %d", table[i].condition_st.old.number_of_repairs);
            for(int j = 0; j < 8 - find_len_int(table[i].condition_st.old.number_of_repairs); j++)
            {
                printf(" ");
            }
            printf("|");
            printf(" %d", table[i].condition_st.old.number_of_car_owners);
            for(int j = 0; j < 8 - find_len_int(table[i].condition_st.old.number_of_car_owners); j++)
            {
                printf(" ");
            }
            printf("|\n");
        }
    }
    printf("└------------------------------------------------------------------------------------------------------------------------------------------------------┘\n");
}

int add_car(automobiles* table, int* temp_size)
{
    FILE *f = fopen("new_table.txt", "r+");
    if (f == NULL)
    {
        printf("Cant find file.\n");
        return CORRECT_ERROR;
    }
    fseek(f, 0, SEEK_END);
    if (*temp_size >= MAX_COUNT)
    {
        printf("Table is already full.");
        return TABLE_IS_FULL;
    }
    char s[STR_SIZE];
    int flag = 1;
    int rc;
    while(flag)
    {
        rc = ERROR_SYMBOL;
        while(rc)
        {
            printf("\nInput brand of automobile :|-------------------| Format(Xxxx, XXXX, XxXx)\n");
            printf("                            ");
            scanf("%s", s);
            if (strlen(s) > 19)
            {
                printf("\nNot correct data.\n");
                continue;
            }
            rc = fill_str(table[*temp_size].brand, s, 1);
        }
        fprintf(f, "%s, ", s);

        rc = ERROR_SYMBOL;
        while(rc)
        {
            printf("\nInput country :|--------------| Format(Xxxx, XXXX, XxXx)\n");
            printf("                ");
            scanf("%s", s);
            if (strlen(s) > 14)
            {
                printf("\nNot correct data.\n");
                continue;
            }
            rc = fill_str(table[*temp_size].country, s, 1);
        }
        fprintf(f, "%s, ", s);

        rc = ERROR_SYMBOL;
        while(rc)
        {
            printf("\nInput color :|---------------| Format(xxxxxx)\n");
            printf("              ");
            scanf("%s", s);
            if (strlen(s) > 15)
            {
                printf("\nNot correct data.\n");
                continue;
            }
            rc = fill_str(table[*temp_size].color, s, 0);
        }
        fprintf(f, "%s, ", s);

        rc = ERROR_SYMBOL;
        while(rc)
        {
            printf("\nInput price :|---------|\n");
            printf("              ");
            scanf("%s", s);
            if (strlen(s) > 9)
            {
                printf("\nNot correct data.\n");
                continue;
            }
            rc = fill_num(&table[*temp_size].price, s);
        }
        fprintf(f, "%s, ", s);

        rc = ERROR_SYMBOL;
        while(rc)
        {
            printf("\nInput condition(old/new) :|---------|\n");
            printf("                           ");
            scanf("%s", s);
            if (strcmp("old", s) == SUCCESS || strcmp("new", s) == SUCCESS)
                rc = fill_str(table[*temp_size].condition, s, 0);
            else
            {
                printf("\nNot correct data.\n");
                continue;
            }
        }
        fprintf(f, "%s, ", s);

        rc = ERROR_SYMBOL;
        while (rc)
        {
            if (strcmp("old", table[*temp_size].condition) == SUCCESS)
            {
                int t = ERROR_SYMBOL;
                while(t)

                {
                    printf("\nInput year of car manufacture :|----|\n");
                    printf("                                ");
                    scanf("%s", s);
                    if (strlen(s) > 4)
                    {
                        printf("\nNot correct data.\n");
                        t = 1;
                        continue;
                    }
                    t = fill_num(&table[*temp_size].condition_st.old.year_of_car_manufacture, s);
                    if (table[*temp_size].condition_st.old.year_of_car_manufacture > 2019)
                    {
                        printf("\nThe year cant'be more than 2019.\n");
                        t = 1;
                        continue;
                    }
                    if (!t)
                    {
                        fprintf(f, "%s, ", s);
                        break;
                    }
                    else
                    {
                        t = 1;
                        printf("\nTry again.\n");
                        continue;
                    }
                }
                t = ERROR_SYMBOL;
                while(t)

                {
                    printf("\nInput car mileage :|--------------|\n");
                    printf("                    ");
                    scanf("%s", s);
                    if (strlen(s) > 14)
                    {
                        printf("\nNot correct data.\n");
                        continue;
                    }
                    t = fill_num(&table[*temp_size].condition_st.old.mileage, s);
                    if (!t)
                    {
                        fprintf(f, "%s, ", s);
                        break;
                    }
                    else
                    {
                        t = 1;
                        printf("\nTry again.\n");
                        continue;
                    }
                }

                t = ERROR_SYMBOL;
                while(t)

                {
                    printf("\nInput number of car's repairs :|--------|\n");
                    printf("                                ");
                    scanf("%s", s);
                    if (strlen(s) > 8)
                    {
                        printf("\nNot correct data.\n");
                        continue;
                    }
                    t = fill_num(&table[*temp_size].condition_st.old.number_of_repairs, s);
                    if (!t)
                    {
                        fprintf(f, "%s, ", s);
                        break;
                    }
                    else
                    {
                        t = 1;
                        printf("\nTry again.\n");
                        continue;
                    }
                }
                t = ERROR_SYMBOL;
                while(t)

                {
                    printf("\nInput number of car's owners :|--------|\n");
                    printf("                               ");
                    scanf("%s", s);
                    if (strlen(s) > 8)
                    {
                        printf("\nNot correct data.\n");
                        continue;
                    }
                    t = fill_num(&table[*temp_size].condition_st.old.number_of_car_owners, s);
                    if (!t)
                    {
                        fprintf(f, "%s, ", s);
                        break;
                    }
                    else
                    {
                        t = 1;
                        printf("\nTry again.\n");
                        continue;
                    }
                }
                rc = SUCCESS;
            }

            if (strcmp("new", table[*temp_size].condition) == SUCCESS)
            {
                int t = ERROR_SYMBOL;
                while(t)

                {
                    printf("\nInput garanty :|--------|\n");
                    printf("                ");
                    scanf("%s", s);
                    if (strlen(s) > 8)
                    {
                        printf("\nNot correct data.\n");
                        continue;
                    }
                    t = fill_num(&table[*temp_size].condition_st.new_s.garanty, s);
                    if (!t)
                    {
                        fprintf(f, "%s, ", s);
                        break;
                    }
                    else
                    {
                        t = 1;
                        printf("\nTry again.\n");
                        continue;
                    }
                }
                rc = SUCCESS;
            }

        }
        flag = SUCCESS;
    }
    table[*temp_size].index = *temp_size;
    *temp_size += 1;
    fclose(f);
    return SUCCESS;
}

int delete(automobiles* table, int* temp_size)
{
    int rc = 0;
    printf("\n Please, the number of the string you want to delete.\n");
    int num;
    rc = scanf("%d", &num);
    if (rc != 1)
    {
        printf("\nNot correct value.\n");
        return CORRECT_ERROR;
    }
    if (num <= 0)
    {
        printf("\n Too small value.\n");
        return CORRECT_ERROR;
    }
    if (num > MAX_COUNT)
    {
        printf("\n Too mach number.\n");
        return CORRECT_ERROR;
    }
    for (int i = num - 1; i < *temp_size; i++)
    {
        table[i] = table[i + 1];
        table[i].index--;
    }
    *temp_size -= 1;
    rewrite_file(table, *temp_size);

    return SUCCESS;
}

void fill_table_keys(automobiles* table, table_keys* tab_k, const int table_size, int* tab_k_size)
{
    *tab_k_size = table_size;
    for(int i = 0; i < table_size; i++)
    {
        tab_k[i].new_table_position = i;
        tab_k[i].old_table_position = i;
        tab_k[i].price = table[i].price;
    }
}

// сортировка таблицы   ключей пузырьком
void bubble_sort(table_keys* tab_k, const int tab_k_size)
{
    table_keys temp;
    for (int i = 0; i < tab_k_size - 1; i++)
    {
        for (int j = 0; j < tab_k_size - 1 - i; j++)
        {
            if(tab_k[j].price > tab_k[j + 1].price)
            {
                temp = tab_k[j + 1];
                tab_k[j + 1] = tab_k[j];
                tab_k[j] = temp;
            }
        }
    }
}

// сортировка таблицы ключей вставками
void insertion_sort(table_keys* tab_k, const int tab_k_size)
{
   int position;
   table_keys temp;
    for (int i = 1; i < tab_k_size; i++)
    {
        temp = tab_k[i];
        position = i - 1;
        while (position >= 0 && tab_k[position].price > temp.price)
        {
            tab_k[position + 1] = tab_k[position];
            position = position - 1;
        }
        tab_k[position + 1] = temp;
    }
    for (int i = 0; i < tab_k_size; i++)
        tab_k[i].new_table_position = i;
}

void bubble_sort_table(automobiles* table, const int table_size)
{
    automobiles temp;
    for (int i = 0; i < table_size - 1; i++)
    {
        for (int j = 0; j < table_size - 1 - i; j++)
        {
            if(table[j].price > table[j + 1].price)
            {
                temp = table[j + 1];
                table[j + 1] = table[j];
                table[j] = temp;
            }
        }
    }
}

void insertion_sort_table(automobiles* table, const int table_size)
{
   int position;
   automobiles temp;
    for (int i = 1; i < table_size; i++)
    {
        temp = table[i];
        position = i - 1;
        while (position >= 0 && table[position].price > temp.price)
        {
            table[position + 1] = table[position];
            position = position - 1;
        }
        table[position + 1] = temp;
    }
}

void print_table_for_sort_tab_k(automobiles* table, table_keys* tab_k, const int table_size, const int tab_k_size)
{
    automobiles temp;
    printf("\nTOTAL: %d\n\n", table_size);
    printf("┌------------------------------------------------------------------------------------------------------------------------------------------------------┐\n");
    printf("|  №       |      Brand         |    Country    |      Color     |  Price   |  Status  | Garanty | Manufacture year|     Mileage   | Repairs |  Owners |\n");
    printf("|----------|--------------------|---------------|----------------|----------|----------|---------|-----------------|---------------|---------|---------|\n");
    for (int t = 0; t < table_size; t++)
    {
        int i = tab_k[t].old_table_position;
            
        printf("| %d", t + 1);
        for (int j = 0; j < 9 - find_len_int(t + 1); j++)
            printf(" ");
        printf("|");
        printf(" %s", table[i].brand);
        for(int j = 0; j < 19 - strlen(table[i].brand); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %s", table[i].country);
        for(int j = 0; j < 14 - strlen(table[i].country); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %s", table[i].color);
        for(int j = 0; j < 15 - strlen(table[i].color); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %d", table[i].price);
        for(int j = 0; j < 9 - find_len_int(table[i].price); j++)
        {
            printf(" ");
        }
        printf("|");
        printf(" %s", table[i].condition);
        for(int j = 0; j < 9 - strlen(table[i].condition); j++)
        {
            printf(" ");
        }
        if (strcmp(table[i].condition, "new") == SUCCESS)
        {
            printf("|");
            printf(" %d", table[i].condition_st.new_s.garanty);
            for(int j = 0; j < 8 - find_len_int(table[i].condition_st.new_s.garanty); j++)
            {
                printf(" ");
            }
            printf("|                 |               |         |         |\n");
        }
        else
        {
            printf("|");
            for(int j = 0; j < 9; j++)
                printf(" ");
            printf("|");
            printf(" %d", table[i].condition_st.old.year_of_car_manufacture);
            for(int j = 0; j < 16 - find_len_int(table[i].condition_st.old.year_of_car_manufacture); j++)
            {
                printf(" ");
            }
            printf("|");
            printf(" %d", table[i].condition_st.old.mileage);
            for(int j = 0; j < 14 - find_len_int(table[i].condition_st.old.mileage); j++)
            {
                printf(" ");
            }
            printf("|");
            printf(" %d", table[i].condition_st.old.number_of_repairs);
            for(int j = 0; j < 8 - find_len_int(table[i].condition_st.old.number_of_repairs); j++)
            {
                printf(" ");
            }
            printf("|");
            printf(" %d", table[i].condition_st.old.number_of_car_owners);
            for(int j = 0; j < 8 - find_len_int(table[i].condition_st.old.number_of_car_owners); j++)
            {
                printf(" ");
            }
            printf("|\n");
        }
    }
    for (int i = 0; i < table_size; i++)
        table[i].index = i;
    printf("└------------------------------------------------------------------------------------------------------------------------------------------------------┘\n");
}

void print_table_keys(table_keys* tab_k, const int tab_k_size)
{
    printf("┌------------------------------------┐\n");
    printf("| New number | Old number |   Price  |\n");
    printf("|------------|------------|----------|\n");

    for (int i = 0; i < tab_k_size; i++)
    {
        printf("| %d", tab_k[i].new_table_position + 1);
        for (int j = 0; j < 11 - find_len_int(tab_k[i].new_table_position + 1); j++)
            printf(" ");
        printf("|");
        printf(" %d", tab_k[i].old_table_position + 1);
        for (int j = 0; j < 11 - find_len_int(tab_k[i].old_table_position + 1); j++)
            printf(" ");
        printf("|");
        printf(" %d", tab_k[i].price);
        for (int j = 0; j < 9 - find_len_int(tab_k[i].price); j++)
            printf(" ");
        printf("|\n");
    }
    printf("└------------------------------------┘\n");
}

int show_special_prices(automobiles* table, const int table_size)
{
    char brand[STR_SIZE];
    char s[STR_SIZE];
    int low_range;
    int hight_range;
    int rc = ERROR_SYMBOL;
    while(rc)
    {
        printf("\nInput brand of automobile :|-------------------| Format(Xxxx, XXXX, XxXx)\n");
        printf("                            ");
        scanf("%s", s);
        if (strlen(s) > 19)
        {
            printf("\nNot correct data.\n");
            continue;
        }
        rc = fill_str(brand, s, 1);
    }

    rc = ERROR_SYMBOL;
    while(rc)
    {
        printf("\nInput min price :|---------|\n");
        printf("                  ");
        scanf("%s", s);
        if (strlen(s) > 9)
        {
            printf("\nNot correct data.\n");
            continue;
        }
        rc = fill_num(&low_range, s);
    }

    rc = ERROR_SYMBOL;
    while(rc)
    {
        printf("\nInput max price :|---------|\n");
        printf("                  ");
        scanf("%s", s);
        if (strlen(s) > 9)
        {
            printf("\nNot correct data.\n");
            continue;
        }
        rc = fill_num(&hight_range, s);
    }

    int count = 0;

    for(int i = 0; i < table_size; i++)
    {
        if ((strcmp(brand, table[i].brand) == SUCCESS) && (table[i].price >= low_range && table[i].price <= hight_range) && (strcmp("old", table[i].condition) == SUCCESS)
         && table[i].condition_st.old.number_of_repairs == 0 && table[i].condition_st.old.number_of_car_owners == 1)
        {
            if (count == 0)
            {
                printf("Prices old cars of the specified brand with one previous owner,lack of repair in the indicated price range.\n");
                printf("┌-----------------------┐\n");
                printf("| №          |   Price  |\n");
                printf("|------------|----------|\n");
            }
            printf("| %d", table[i].index + 1);
            for (int j = 0; j < 11 - find_len_int(table[i].index + 1); j++)
                printf(" ");
            printf("|");
            printf(" %d", table[i].price);
            for (int j = 0; j < 9 - find_len_int(table[i].price); j++)
                printf(" ");
            printf("|\n");
            count++;
        }
    }

    if (count == 0)
    {
        printf("\nDon't have this kind of automobile.\n");
        return CORRECT_ERROR;
    }
    printf("└-----------------------┘\n");
    return SUCCESS;
}

void compare_func(automobiles* table, int temp_table_size, table_keys* tab_k, int* temp_tab_k_size)
{
    //flag = 0;
    int i1;
    printf( "Comparing the efficiency of sorting tables and keys\n\n" );
    printf("Bubble sort\n\n");
    {
        clock_t start2, end2, start1, end1;
        for (int q = 40; q <= 2040; q += 500)
        {
            int rc = filling_table_from_file(table, q);
            if (rc != SUCCESS)
            {
                printf("Error in file.\n");
                return;
            }
            fill_table_keys(table, tab_k, temp_table_size, temp_tab_k_size);
            start1= clock();
            bubble_sort(tab_k, q);
            end1= clock();

            start2= clock();
            bubble_sort_table(table,  q);
            end2= clock();

            printf("~~~Time of sorting %d notes:~~~\n", q);
            printf("Table of keys: %.4ld ms\n", (end1 - start1));
            printf("Initial table: %.4ld ms\n\n", (end2 - start2));
        }
    }
    printf("\n\nInsertion sort\n\n");
    {
        clock_t start2, end2, start1, end1;
        for (int q = 40; q <= 2040; q += 500)
        {
            int rc = filling_table_from_file(table, q);
            if (rc != SUCCESS)
            {
                printf("Error in file.\n");
                return;
            }
            fill_table_keys(table, tab_k, temp_table_size, temp_tab_k_size);
            start1= clock();
            insertion_sort(tab_k, q);
            end1= clock();

            start2= clock();
            insertion_sort_table(table,  q);
            end2= clock();
            printf("~~~Time of sorting %d notes:~~~\n", q);
            printf("Table of keys: %.4ld ms\n", (end1 - start1));
            printf("Initial table: %.4ld ms\n\n", (end2 - start2));
        }
    }
}

int main(void)
{
    automobiles table[MAX_COUNT];
    automobiles table_to_cmp[MAX_COUNT];
    //table_keys keys_tab[MAX_COUNT];


    printf("This program works with table, containing information about from 40 to 2000 automobiles in shop. "
           "It has information about its: brand, country, color, price, status"
           "\nif the car is new, here is information about garanty."
           "\nif the car if old, here is information about year of car manufacture, mileage, number of car's repairs, number of car's owners.\n"
           "You can do several things with the table entering the special key of the task.\n");
    int temp_table_size = MIN_COUNT;
    int check;
    table_keys tab_k[MAX_COUNT];
    int temp_tab_k_size = 0;
    int rc = filling_table_from_file(table, MIN_COUNT);
    if (rc != SUCCESS)
        return rc;
    rewrite_file(table, temp_table_size);
    for (; ;)
    {
        printf("LIST OF TASKS\n"
               "enter 1 to show the table\n"
               "enter 2 to add new car\n"
               "enter 3 to delete the special car\n"
               "enter 4 to show sorted key table(insertion sort) without changing the original table\n"
               "enter 5 to show sorted table(insertion sort) without using keys table\n"
               "enter 6 to show sorted table with using keys table(insertion sort)\n"
               "enter 7 to compare the efficiency of sorting tables without using keys table and keys table (bubble sort/insertion sort)\n"
               "enter 8 to show prices prices oldcars of the specified brand with one previous owner,lack of repair in the indicated price range.\n"
               "enter 9 to close the program\n");
        int s;
        int rc = scanf("%d", &s);
        if (rc != 1)
        {
            printf("Not correct input\n");
            return CORRECT_ERROR;
        }
        switch (s)
        {
             case 1:
                 print_tab(table, temp_table_size);
                break;
             case 2:
                add_car(table, &temp_table_size);
                break;
             case 3:
                check = CORRECT_ERROR;
                while (check != SUCCESS)
                {
                    check = delete(table, &temp_table_size);
                    continue;
                }
                break;
            case 4:
                fill_table_keys(table, tab_k, temp_table_size, &temp_tab_k_size);
                insertion_sort(tab_k, temp_tab_k_size);
                printf("\nSorted keys table\n");
                print_table_keys(tab_k, temp_tab_k_size);
                break;
           case 5:
                insertion_sort_table(table, temp_table_size);
                printf("\nSorted table\n");
                print_tab(table, temp_table_size);
                rewrite_file(table, temp_table_size);
                break;
           case 6:
                fill_table_keys(table, tab_k, temp_table_size, &temp_tab_k_size);
                insertion_sort(tab_k, temp_tab_k_size);
                printf("\nSorted keys table\n");
                print_table_keys(tab_k, temp_tab_k_size);
                printf("Sorted table\n");
                print_table_for_sort_tab_k(table, tab_k, temp_table_size, temp_tab_k_size);
                break;
            case 7:
                compare_func(table_to_cmp, temp_table_size, tab_k, &temp_tab_k_size);
                break;
            case 8:
                check = CORRECT_ERROR;
                while (check != SUCCESS)
                {
                    check = show_special_prices(table, temp_table_size);
                    continue;
                }
                break;
           case 9:
                return SUCCESS;
        }
    }
}
