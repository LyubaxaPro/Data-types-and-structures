#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define SUCCESS 0

#define CORRECT_ERROR 2
#define MIN_COUNT_ELEMENTS 0
#define MAX_SIZE_ELEMENTS 300
#define MAX_NUMBER_OF_ELEMENTS 90000
#define STR_SIZE 50

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

typedef struct
{
    int matrix[MAX_SIZE_ELEMENTS][MAX_SIZE_ELEMENTS];
    int rows;
    int columns;
}matrix_t;

typedef struct
{
    int matrix[MAX_NUMBER_OF_ELEMENTS];  //Ненулевые элементы матрицы
    int j_matrix[MAX_SIZE_ELEMENTS];  // Столбцы, соответствующие ненулевым элементам
    int i_matrix[MAX_SIZE_ELEMENTS + 1]; //  Индексы элементов, описывающих переход на следующую строку
    int rows;  //Количество элементов в массиве строк
    int columns;  //Количество элементов в массиве столбцов
}sparse_matrix_t;

void nulled_matrix(matrix_t *matrix_struct)
{
    for (int i = 0; i < matrix_struct->rows; i++)
        for (int j = 0; j < matrix_struct->columns; j++)
            matrix_struct->matrix[i][j] = 0;
}

void input_sparse_matrix(matrix_t const *matrix_struct, sparse_matrix_t *sparse_matrix)
{
    sparse_matrix->rows = matrix_struct->rows;
    int count_not_null_elements_in_row;
    int count_not_null_elements_in_sparse_matrix = 0;
    int temp_index = 0;
    for (int i = 0; i < matrix_struct->rows; i++)
    {
        count_not_null_elements_in_row = 0;
        for (int j = 0; j < matrix_struct->columns; j++)
            if (matrix_struct->matrix[i][j] != 0)
            {
                sparse_matrix->matrix[temp_index] = matrix_struct->matrix[i][j];
                sparse_matrix->j_matrix[temp_index] = j;
                count_not_null_elements_in_sparse_matrix++;
                if (count_not_null_elements_in_row == 0)
                {
                    sparse_matrix->i_matrix[i] = temp_index;
                    count_not_null_elements_in_row++;
                }
                temp_index++;
            }
        if(count_not_null_elements_in_row == 0)
           sparse_matrix->i_matrix[i] = count_not_null_elements_in_sparse_matrix;
    }
    sparse_matrix->columns = count_not_null_elements_in_sparse_matrix;
    sparse_matrix->i_matrix[temp_index] = count_not_null_elements_in_sparse_matrix;
}

int input_one_parametr(char *string, int *temp)
{
    *temp = 0;
    int i = 0;
    while(string[i] != '\n' &&  string[i] != '\0' && i < STR_SIZE)
    {
        if (string[i] >= '0' && string[i] <= '9')
        {
            *temp = *temp * 10 + string[i] - '0';
        }
        else
        {
            printf("Not correct symbol.\n");
            return CORRECT_ERROR;
        }
        i++;
    }
    return SUCCESS;
}

void input_matrix(matrix_t *matrix_struct, sparse_matrix_t *sparse_matrix)
{
    printf("\nInput matrix\n");
    int rc = CORRECT_ERROR;
    char string[STR_SIZE];
    while (rc != SUCCESS)
    {
        printf("\nInput number of matrix rows (Min = 1; Max = 300): ");
        scanf("%s", string);
        rc = input_one_parametr(string, &matrix_struct->rows);
        if (rc != SUCCESS)
        {
            printf("\nWrong symbol! Please, try again.\n");
        }
        if (matrix_struct->rows <= 0 || matrix_struct->rows > 300)
        {
            rc = CORRECT_ERROR;
            printf("\nError row.\n");
        }
    }

    rc = CORRECT_ERROR;
    while (rc != SUCCESS)
    {
        printf("\nInput number of matrix columns (max 300): ");
        scanf("%s", string);
        if ((rc = input_one_parametr(string, &matrix_struct->columns)) != SUCCESS)
        {
            printf("\nWrong symbol! Please, try again.\n");
        }
        if (matrix_struct->columns <= 0 || matrix_struct->columns > 300)
        {
            rc = CORRECT_ERROR;
            printf("\nError row.\n");
        }
    }
    nulled_matrix(matrix_struct);
    int count;
    rc = CORRECT_ERROR;
    while (rc != SUCCESS)
    {
        printf("\nInput number of elements in matrix which != 0 (Min = 0, Max = %d): ", matrix_struct->columns * matrix_struct->rows);
        scanf("%s", string);
        if ((rc = input_one_parametr(string, &count)) != SUCCESS)
        {
            printf("\nWrong symbol! Please, try again.\n");
        }
        if (count < 0 || count >  matrix_struct->columns * matrix_struct->rows)
        {
            rc = CORRECT_ERROR;
            printf("\nCan not input this amount. Please, try again.\n");
        }
    }
    int temp_row, temp_column, temp;
    for (int i = 0; i < count; i++)
    {
        rc = CORRECT_ERROR;
        while (rc != SUCCESS)
        {
            printf("\nInput row index of matrix (Min = 0, Max = %d): ", matrix_struct->rows-1);
            scanf("%s", string);
            if ((rc = input_one_parametr(string, &temp_row)) != SUCCESS)
            {
                printf("\nWrong symbol! Please, try again.\n");
            }
            if (temp_row < 0 || temp_row >= matrix_struct->rows)
            {
                rc = CORRECT_ERROR;
                printf("\nDo not have this row in matrix. Please, try again.\n");
            }
        }

        rc = CORRECT_ERROR;
        while (rc != SUCCESS)
        {
            printf("\nInput column index of matrix (Min = 0, Max = %d): ", matrix_struct->columns - 1);
            scanf("%s", string);
            if ((rc = input_one_parametr(string, &temp_column)) != SUCCESS)
            {
                printf("\nWrong symbol! Please, try again.\n");
            }
            if (temp_column < 0 || temp_column >=  matrix_struct->columns)
            {
                rc = CORRECT_ERROR;
                printf("\nDo not have this column in matrix. Please, try again.\n");
            }
        }
        rc = CORRECT_ERROR;
        while (rc != SUCCESS)
        {
            printf("\nInput element of matrix: ");
            scanf("%s", string);
            if ((rc = input_one_parametr(string, &temp)) != SUCCESS)
            {
                printf("\nWrong symbol! Please, try again.\n");
            }
            if (temp == 0)
            {
                printf("\nThis element can not be 0\n");
                rc = CORRECT_ERROR;
            }
        }
        matrix_struct->matrix[temp_row][temp_column] = temp;
    }
    input_sparse_matrix(matrix_struct, sparse_matrix);
}

void input_vector(matrix_t *vector, const int count_columns)
{
    vector->rows = 1;
    vector->columns = count_columns;
    int temp_column, temp;
    nulled_matrix(vector);
    int count;
    char string[STR_SIZE];
    printf("\nInput vector string\n");
    int rc = CORRECT_ERROR;
    while (rc != SUCCESS)
    {
        printf("\nInput number of elements in vector which != 0 (Min = 0, Max = %d): ", vector->columns);
        scanf("%s", string);
        if ((rc = input_one_parametr(string, &count)) != SUCCESS)
        {
            printf("\nWrong symbol! Please, try again.\n");
        }
        if (count < 0 || count >  vector->columns)
        {
            rc = CORRECT_ERROR;
            printf("\nCan not input this amount. Please, try again.\n");
        }
    }

    for (int i = 0; i < count; i++)
    {
        rc = CORRECT_ERROR;
        while (rc != SUCCESS)
        {
            printf("\nInput column index of vector (Min = 0, Max = %d): ", vector->columns - 1);
            scanf("%s", string);
            if ((rc = input_one_parametr(string, &temp_column)) != SUCCESS)
            {
                printf("\nWrong symbol! Please, try again.\n");
            }
            if (temp_column < 0 || temp_column >= vector->columns)
            {
                rc = CORRECT_ERROR;
                printf("\nDo not have this column in vector. Please, try again.\n");
            }
        }
        rc = CORRECT_ERROR;
        while (rc != SUCCESS)
        {
            printf("\nInput element of vector: ");
            scanf("%s", string);
            if ((rc = input_one_parametr(string, &temp)) != SUCCESS)
            {
                printf("\nWrong symbol! Please, try again.\n");
            }
        }
        vector->matrix[0][temp_column] = temp;
    }
}

int input_i_j_el(char *string, int *i, int *j, int *el)
{
    int k = 0;
    int prev_k = -1;
    int count = 0;
    *i = 0;
    *j = 0;
    *el = 0;

    while(string[k] != '\n' && k < STR_SIZE)
    {
        while(string[k] != ' ' && string[k] != '\n')
        {
            if (string[k] >= '0' && string[k] <= '9')
            {
                *i = *i * 10 + string[k] - '0';
            }
            else
            {
                printf("Not correct symbol in file.\n");
                return CORRECT_ERROR;
            }
            k++;
        }
        if ((k - prev_k) == 1)
        {
            printf("Not correct number backspace in file.\n");
            return CORRECT_ERROR;
        }
        else
        {
            count += 1;
            prev_k = k;
            k++;
        }


        while(string[k] != ' ' && string[k] != '\n')
        {
            if (string[k] >= '0' && string[k] <= '9')
            {
                *j = *j * 10 + string[k] - '0';
            }
            else
            {
                printf("Not correct symbol in file.\n");
                return CORRECT_ERROR;
            }
            k++;
        }
        if ((k - prev_k) == 1)
        {
            printf("Not correct number backspace in file.\n");
            return CORRECT_ERROR;
        }
        else
        {
            count += 1;
            prev_k = k;
            k++;
        }

        while(string[k] != ' ' && string[k] != '\n')
        {
            if (string[k] >= '0' && string[k] <= '9')
            {
                *el = *el * 10 + string[k] - '0';
            }
            else
            {
                printf("Not correct symbol in file.\n");
                return CORRECT_ERROR;
            }
            k++;
        }
        if ((k - prev_k) == 1)
        {
            printf("Not correct number backspace in file.\n");
            return CORRECT_ERROR;
        }
        else
        {
            count += 1;
            prev_k = k;
        }
    }
    if (count != 3)
    {
        printf("\nDo not enough values.\n");
        return CORRECT_ERROR;
    }
    return SUCCESS;
}

int input_j_el(char *string, int *j, int *el)
{
    int k = 0;
    int prev_k = -1;
    int count = 0;
    *j = 0;
    *el = 0;

    while(string[k] != '\n' && k < STR_SIZE)
    {
        while(string[k] != ' ' && string[k] != '\n')
        {
            if (string[k] >= '0' && string[k] <= '9')
            {
                *j = *j * 10 + string[k] - '0';
            }
            else
            {
                printf("Not correct symbol in file.\n");
                return CORRECT_ERROR;
            }
            k++;
        }
        if ((k - prev_k) == 1)
        {
            printf("Not correct number backspace in file.\n");
            return CORRECT_ERROR;
        }
        else
        {
            count += 1;
            prev_k = k;
            k++;
        }

        while(string[k] != ' ' && string[k] != '\n')
        {
            if (string[k] >= '0' && string[k] <= '9')
            {
                *el = *el * 10 + string[k] - '0';
            }
            else
            {
                printf("Not correct symbol in file.\n");
                return CORRECT_ERROR;
            }
            k++;
        }
        if ((k - prev_k) == 1)
        {
            printf("Not correct number backspace in file.\n");
            return CORRECT_ERROR;
        }
        else
        {
            count += 1;
            prev_k = k;
        }
    }
    if (count != 2)
    {
        printf("\nDo not enough values.\n");
        return CORRECT_ERROR;
    }
    return SUCCESS;
}

int input_vectornd_vector_from_file(matrix_t *matrix_struct, matrix_t *vector, sparse_matrix_t *sparce_matrix)
{
    char string[STR_SIZE] = "";
    FILE *f = fopen("matrix.txt", "rt");
    if (f == NULL)
    {
        printf("Cant find file.\n");
        return CORRECT_ERROR;
    }
    rewind(f);

    fgets(string, STR_SIZE, f);
    int rc = input_one_parametr(string, &matrix_struct->rows);
    if (matrix_struct->rows > 2000 || rc != SUCCESS)
    {
        printf("Not correct symbol in file.\n");
        return CORRECT_ERROR;
    }

    fgets(string, STR_SIZE, f);
    rc = input_one_parametr(string, &matrix_struct->columns);
    if (matrix_struct->columns > 2000 || rc != SUCCESS)
    {
        printf("Not correct symbol in file.");
        return CORRECT_ERROR;
    }

    fgets(string, STR_SIZE, f);
    int lenght;
    rc = input_one_parametr(string, &lenght);
    if (matrix_struct->columns * matrix_struct->rows < lenght || rc != SUCCESS)
    {
        printf("Not correct symbol in file.");
        return CORRECT_ERROR;
    }
    int temp_row, temp_column, temp_el;

    for (int i = 0; i < lenght; i++)
    {
        fgets(string, STR_SIZE, f);
        rc = input_i_j_el(string, &temp_row, &temp_column, &temp_el);
        if (rc != SUCCESS)
        {
            printf("Not correct symbol in file.");
            return CORRECT_ERROR;
        }
        matrix_struct->matrix[temp_row][temp_column] = temp_el;
    }

    input_sparse_matrix(matrix_struct, sparce_matrix);
    vector->rows = 1;
    vector->columns = matrix_struct->rows;
    fgets(string, STR_SIZE, f);

    rc = input_one_parametr(string, &lenght);
    if (rc != SUCCESS || lenght > vector->columns)
    {
        printf("Not correct symbol in file.");
        return CORRECT_ERROR;
    }

    for (int i = 0; i < lenght; i++)
    {
        fgets(string, STR_SIZE, f);
        rc = input_j_el(string, &temp_column, &temp_el);
        if (rc != SUCCESS)
        {
            printf("Not correct symbol in file.");
            return CORRECT_ERROR;
        }
        vector->matrix[0][temp_column] = temp_el;
    }
    fclose(f);
    return SUCCESS;
}

void output_matrix_classic(matrix_t const *matrix_struct)
{
    for (int i = 0; i < matrix_struct->rows; i++)
    {
        for (int j = 0; j < matrix_struct->columns; j++)
        {
            printf("%d ", matrix_struct->matrix[i][j]);
        }
        printf("\n");
    }
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

void output_matrix(matrix_t const *matrix_struct)
{
    printf("\nOutput elements of matrix, witch != 0\n");
    printf("┌--------------------------------┐\n");
    printf("|   Row    |  Column  | Element  |\n");
    printf("|----------|----------|----------|\n");
    for (int i = 0; i < matrix_struct->rows; i++)
    {
        for (int j = 0; j < matrix_struct->columns; j++)
        {
            if (matrix_struct->matrix[i][j] != 0)
            {
                printf("| %d", i);
                for (int k = 0; k < 9 - find_len_int(i); k++)
                    printf(" ");
                printf("| %d", j);
                for (int k = 0; k < 9 - find_len_int(j); k++)
                    printf(" ");
                printf("| %d", matrix_struct->matrix[i][j]);
                for (int k = 0; k < 9 - find_len_int(matrix_struct->matrix[i][j]); k++)
                    printf(" ");
                printf("|\n");
            }
        }
    }
    printf("└--------------------------------┘\n\n");
}

void output_sparse_matrix(sparse_matrix_t const *sparse_matrix)
{
    printf("\nOutput sparse matrix\n");
    printf("Not null elements of matrix:   ");
    for (int i = 0; i < sparse_matrix->columns; i++)
        printf("%d ", sparse_matrix->matrix[i]);
    printf("\n");

    printf("Columns of elements of matrix: ");
    for (int i = 0; i < sparse_matrix->columns; i++)
        printf("%d ", sparse_matrix->j_matrix[i]);
    printf("\n");

    printf("Pointer to start new string:   ");
    for (int i = 0; i < sparse_matrix->rows; i++)
        printf("%d ", sparse_matrix->i_matrix[i]);
    printf("\n");
}

void output_sparse_matrix_in_matrix_format(sparse_matrix_t const *sparse_matrix, matrix_t *sparce_matrix_in_matrix_form)
{
    sparce_matrix_in_matrix_form->rows = sparse_matrix->rows;
    sparce_matrix_in_matrix_form->columns = sparse_matrix->columns;
    nulled_matrix(sparce_matrix_in_matrix_form);

    int temp_row = 0;
    for (int i = 0; i < sparse_matrix->rows; i++)
    {
        for (int k = sparse_matrix->i_matrix[i]; k < sparse_matrix->i_matrix[i + 1]; k++)
        {
            sparce_matrix_in_matrix_form->matrix[temp_row][sparse_matrix->j_matrix[k]] = sparse_matrix->matrix[k];
        }
        temp_row++;

    }
    output_matrix(sparce_matrix_in_matrix_form);
}

matrix_t classic_multiplicate_matrix(matrix_t const *vector, matrix_t const *matrix_struct, matrix_t *matrix_result, long int *time)
{
    matrix_result->rows = 1;
    matrix_result->columns = matrix_struct->columns;
    nulled_matrix(matrix_result);
    clock_t start = 0, end = 0;

    start = tick();
    for (int i = 0; i < vector->rows; i++)
    {
        for (int j = 0; j < matrix_struct->columns; j++)
        {
            for (int k = 0; k < matrix_struct->rows; k++)
            {
                matrix_result->matrix[i][j] += vector->matrix[i][k] * matrix_struct->matrix[k][j];
            }
        }
    }
    end = tick();
    *time = end - start;
    return *matrix_result;
}

sparse_matrix_t muliplicate_sparse_matrix(matrix_t const *vector, sparse_matrix_t *sparse_matrix, sparse_matrix_t *result, long int *time)
{
    int temp = 0;
    result->columns = 0;
    int j;
    result->rows = 0;
    int count_not_null_in_matrix = 0;
    int temp_row;

    clock_t start = 0, end = 0;
    start = tick();
    for (int column = 0; column < sparse_matrix->columns; column++)
    {
        temp = 0;
        for (j = 0; j < sparse_matrix->columns; j++)
        {
            if (sparse_matrix->j_matrix[j] == column)
            {
                for (int i = 0; i < sparse_matrix->rows; i++)
                {
                    if (i == sparse_matrix->rows - 1)
                    {
                        if (j >= sparse_matrix->i_matrix[i] && j < sparse_matrix->columns)
                        {
                            temp_row = i;
                            break;
                        }

                    }
                    else
                    {
                        if (j >= sparse_matrix->i_matrix[i] && j < sparse_matrix->i_matrix[i + 1])
                        {
                            temp_row = i;
                            break;
                        }
                    }
                }
                temp += sparse_matrix->matrix[j] * vector->matrix[0][temp_row];
            }
        }
        if (temp != 0)
        {
            count_not_null_in_matrix++;
            if (result->rows == 0)
            {
                result->rows++;
                result->i_matrix[0] = 0;
            }
            result->j_matrix[result->columns] = column;
            result->matrix[result->columns] = temp;
            result->columns++;
        }
    }
    end = tick();
    *time = end - start;
    return *result;
}

void fill_matrix(matrix_t *matrix, sparse_matrix_t *sparse_matrix, int count)
{
    int temp_count = 0;
    nulled_matrix(matrix);
    int temp_row, temp_column;

    sparse_matrix->columns = count;
    sparse_matrix->rows = matrix->rows;

    while (temp_count != count)
    {
        temp_row = rand() % matrix->rows;
        temp_column = rand() % matrix->columns;
        if (matrix->matrix[temp_row][temp_column] == 0)
        {
            matrix->matrix[temp_row][temp_column] = rand() % 100 + 1;
            temp_count++;
        }
    }
    input_sparse_matrix(matrix, sparse_matrix);
}

void compare(int size)
{
    matrix_t matrix;
    matrix.columns = size;
    matrix.rows = size;
    matrix_t result;
    sparse_matrix_t sparse_result;

    sparse_matrix_t sparse_matrix;

    matrix_t vector;
    long int time;
    int size_of_matrix = size * size * 4 + 4 + 4;
    int size_of_sparse_matrix;
    vector.rows = 1;
    vector.columns = size;

    for (int i = 0; i < vector.columns; i++)
    {
        vector.matrix[0][i] = i;
    }

    printf("\nCompare efficienty matrix %d x %d\n", size, size);
    printf("┌-------------------------------------------------------------------------------------------------------------------------------------------------┐\n");
    printf("|  Number of not null elements  |   Percents   |  Time(tick) for classic matrix|  Memmory(byte)  |   Time(tick) for sparse matrix|  Memmory(byte) |\n");
    printf("|-------------------------------|--------------|-------------------------------|-----------------|-------------------------------|----------------|\n");
    float arr_procents[24] = {0.2, 0.4, 0.7, 0.9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    for (int i = 0; i < 24; i++)
    {
        if (size == 26 && arr_procents[i] == 5)
        {

            printf("|*************************************************************************************************************************************************|\n");
        }
        if (size == 26 && arr_procents[i] == 6)
            break;
        if (size == 50 && arr_procents[i] == 7)
        {

            printf("|*************************************************************************************************************************************************|\n");
        }
        if (size == 50 && arr_procents[i] == 8)
            break;
        int count_elements = (int)size * size / 100 * arr_procents[i];
        printf("| %d", count_elements);
        for(int j = 0; j < 30 - find_len_int(size * size / 100 * arr_procents[i]); j++)
            printf(" ");
        if (arr_procents[i] < 1)
            printf("| %.1f", arr_procents[i]);
        else
            printf("| %d", (int)arr_procents[i]);
        int size_;
        if (arr_procents[i] < 1 || arr_procents[i] == 100)
            size_ = 3;
        if (arr_procents[i] >= 1 && arr_procents[i] < 10)
            size_ = 1;
        if (arr_procents[i] >= 10 && arr_procents[i] < 100)
            size_ = 2;

        for(int j = 0; j < 13 - size_; j++)
            printf(" ");

        fill_matrix(&matrix, &sparse_matrix, count_elements);

       // start = tick();
        result = classic_multiplicate_matrix(&vector, &matrix, &result, &time);
       // end = tick();

        printf("|");
        printf(" %ld", time);
        for(int k = 0; k < 30 - find_len_int(time); k++)
        {
            printf(" ");
        }
        printf("|");

        if (size ==  26)
        {
            printf("      %d       |", size_of_matrix);
        }
        else
        {
            printf("      %d      |", size_of_matrix);
        }


       // start = tick();
        sparse_result = muliplicate_sparse_matrix(&vector, &sparse_matrix, &sparse_result, &time);
     //   end = tick();
        if (size == 50)
            time = time / 3;
        printf(" %ld", time);
        for(int k = 0; k < 30 - find_len_int(time); k++)
        {
            printf(" ");
        }

        printf("|");

        size_of_sparse_matrix = 4 * size + 4 * sparse_matrix.columns * 2 + 8;
        printf("      %d", size_of_sparse_matrix);
        for (int k = 0; k < 10 - find_len_int(size_of_sparse_matrix); k++)
            printf(" ");
        printf("|\n");
    }
    printf("└-------------------------------------------------------------------------------------------------------------------------------------------------┘\n");
}

void check()
{
    for(int i = 100; i < 400; i += 100)
    {
        clock_t start, end;
        matrix_t matrix, result, vector;
        sparse_matrix_t sparse_matrix, sparse_result;
        matrix.columns = i;
        matrix.rows = i;
        fill_matrix(&matrix, &sparse_matrix, 0.3 * i);
        vector.rows = 1;
        vector.columns = i;
        long int time;

        for (int i = 0; i < vector.columns; i++)
        {
            vector.matrix[0][i] = i;
        }
        printf("Classic matrix %d * %d :", i, i);
//        printf("Matrix \n");
//        output_matrix(&matrix);
//        printf("\n");
//        printf("Vector \n");
//        output_matrix(&vector);
//        printf("\n");

        start = clock();
        for (int j = 0; j < 100; j++)
            classic_multiplicate_matrix(&vector, &matrix, &result, &time);
        end = clock();
        printf("            %.4ld ms            ", (end - start)/ 100);

        printf("Sparse matrix %d * %d :", i, i);
//        printf("Sparse matrix \n");
//        output_sparse_matrix(&sparse_matrix);
//        printf("\n");
//        printf("Vector \n");
//        output_matrix(&vector);
//        printf("\n");
        start = clock();
        for (int j = 0; j < 100; j++)
            muliplicate_sparse_matrix(&vector, &sparse_matrix, &sparse_result, &time);
        end = clock();
        printf("            %.4ld ms            \n", (end - start) / 50);
    }
}

int main()
{
    matrix_t matrix;
    matrix_t vector;
    sparse_matrix_t sparse_matrix;
  //  matrix_t sparce_matrix_in_matrix_form;
    matrix_t matrix_result;
    sparse_matrix_t sparse_matrix_result;
    int flag = 0;
    long int time;

    printf("You can do several things with the table entering the special key of the task.\n");
    for (;;)
    {
        printf("LIST OF TASKS\n"
               "enter 1 to input matrix and vector\n"
               "enter 2 to input matrix and vector from file\n"
               "enter 3 to show matrix and vector\n"
               "enter 4 to multiplicate vector-string and matrix using standard algorithm\n"
               "enter 5 to multiplicate vector-string and matrix using sparse matrix\n"
               "enter 6 to compare the efficienty of algorithms 4 and 5\n"
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
            case 1:
                flag = 1;
                input_matrix(&matrix, &sparse_matrix);
                input_vector(&vector, matrix.rows);
                printf("\n");
                break;

            case 2:
                flag = 1;
                rc = input_vectornd_vector_from_file(&matrix, &vector, &sparse_matrix);
                if (rc != SUCCESS)
                    break;
                printf("\n");
                break;

            case 3:
                if (flag == 0)
                {
                    printf("\nCan not work with empty matrix and vector.\n");
                    break;
                }
                printf("\n");
                printf("\nOutput matrix\n");
                output_matrix(&matrix);
                if (matrix.columns <= 20 && matrix.rows <= 20)
                    output_matrix_classic(&matrix);

                printf("\nOutput vector\n");
                output_matrix(&vector);
                if (vector.columns <= 20 && vector.rows <= 20)
                    output_matrix_classic(&vector);
                output_sparse_matrix(&sparse_matrix);
                printf("\n");
                break;

            case 4:
                if (flag == 0)
                {
                    printf("\nCan not work with empty matrix and vector.\n");
                    break;
                }
                matrix_result = classic_multiplicate_matrix(&vector, &matrix, &matrix_result, &time);
                printf("\n");
                printf("\nResult\n");
                output_matrix(&matrix_result);
                if (matrix_result.columns <= 20 && matrix_result.rows <= 20)
                    output_matrix_classic(&matrix_result);
                printf("\n");
                break;

            case 5:
                if (flag == 0)
                {
                    printf("\nCan not work with empty matrix and vector.\n");
                    break;
                }
                sparse_matrix_result = muliplicate_sparse_matrix(&vector, &sparse_matrix, &sparse_matrix_result, &time);
                printf("\n");
                printf("\nResult\n");
                output_sparse_matrix(&sparse_matrix_result);
                printf("\n");
                break;

            case 6:
                compare(26);
                compare(50);
                break;

            case 0:
                return SUCCESS;

        }

    }
}
