#include<stdio.h>
#include<string.h>
#include <stdlib.h>
//#include <windows.h>

#define SUCCESS 0
#define SIZE_OF_MANT 32 // 30 чисел + точка(если есть) и "\0"
#define LEN_OF_MANT 30 // Максимальная длина мантиссы
#define  MAX_VAL_OF_EXP 99999 // Максимальное значение порядка
#define  MIN_VAL_OF_EXP -99999 // Максимальное значение порядка
#define MAX_LEN 40
#define TRUE 1
#define FALSE 0
#define MAX_EXP_LEN 5

// Набор кодов ошибок
#define EMPTY_INPUT 1 // Пустой ввод
#define NOT_CORRECT_NUM 2 // Вместо целого числа введено вещественное
#define NOT_CORRECT_FORM_OF_NUM 3 // Некорректная запись числа
#define LEN_LIMIT 4 // Пользователь ввёл слишком длинное число
#define NULL_DEL 5 // Деление на 0
#define NOT_TEMPLATE 6 // Число не соответствует заданному шаблону
#define LEN_MANT_LIMIT 7 // Длина мантиссы больше максимально возможной
#define LEN_EXP_LIMIT 8 // Длина порядка больше максимально возможной
#define LIMIT_ERROR 9

typedef struct // структура для хранения чисел в проавильном формате
{
    int sign_m; // знак мантиссы
    char mant[SIZE_OF_MANT]; // значение мантиссы
    int exp_num; // порядок числа (значение после Е)
} num;

//Считывает строку, отсеивает случаи ошибок ввода с кодами 1-5
int my_input(char* input_data, const int is_integer){
    char temp_char = getchar();
    int temp_pos = 0;
    int count_null_befor_point = 0;
    int data_is_only_null = TRUE; //  перед тем как считать 0 проверяет, что этот 0 не будет значением старшего разряда целой части числа

    // while ((temp_char = getchar()) == ' '); //игнорируем возможное считывание пробелов;

    if (temp_char == '\n')
        return EMPTY_INPUT;

    while (temp_pos < MAX_LEN && temp_char != '\n' && temp_char != EOF){

        if (is_integer && (temp_char == '.' || temp_char == 'E'))
            return NOT_CORRECT_NUM;

        if (temp_char == '0' && data_is_only_null && count_null_befor_point >= 1){
            return NOT_CORRECT_FORM_OF_NUM;
        }

        if (data_is_only_null && temp_char == '0'){
            if (is_integer)
                return NULL_DEL;
            else{
                count_null_befor_point++;
                temp_char = getchar();
                continue;
            }
        }

        if (data_is_only_null && temp_char != '0'){
            data_is_only_null = FALSE;
        }

        input_data[temp_pos++] = temp_char;
        temp_char = getchar();
    }
    input_data[temp_pos] = '\0';
    if (temp_pos == MAX_LEN)
        return LEN_LIMIT;

    return SUCCESS;

}

// Печатает сообщения об ошибках
void print_error_message(const int code){
    if (code == 1)
        printf ("Число не было введено.\n");
    if (code == 2)
        printf("Целое число было введено некорректно.\n");
    if (code == 3)
        printf("Число записано некорректно\n");
    if (code == 4)
        printf("Превышена максимально возможная длина числа.\n");
    if (code == 5)
        printf("Деление на 0 не может быть обработано.");
    if (code == 6)
        printf("Число не соответствует заданному шаблону");
    if (code == 7)
        printf("Длина мантиссы больше максимально возможной");
    if (code == 8)
        printf("Длина порядка больше макисмально возможного");
}

int is_digit(const char temp){
    if (temp >= '0' && temp <= '9')
        return TRUE;
    return FALSE;
}

/* Проверяет на корректное наличие в строке следующих частей числа
 * cease 1 - знак мантиссы
 * case 2 - число/точка мантиссы
 * case 3 - часть мантиссы после точки
 * case 4 - проверка наличия порядка числа
 * case 5 - знак порядка числа
 * case 6 - порядок числа
 */
int check_data(char* data){
    int part_of_num = 1; //Часть числа, предполагаемая к чтению
    int point = FALSE; // Переменная флаг, показывающая была ли точка в мантиссе
    int temp_mant_len = 0;
    int temp_exp_len = 0;
    int sign = 0;

    for (int i = 0; data[i]; i++)
    {
        switch (part_of_num)
        {
            case 1:
                if (data[i] == '+' || data[i] == '-') // прочитан знак, далее следует цифра/точка
                {
                    part_of_num = 2;
                    sign = 1;
                    break;
                }

                if (is_digit(data[i]))
                { // Прочитана цифра, далее следует цифра
                    temp_mant_len++;
                    part_of_num = 2;
                }

                else if (data[i] == '.') // прочитана точка, далее следует цифра/отсутсвие цифры
                    part_of_num = 3;

                else if (data[i] == 'E') // мантиссы не ожидается, переход к стадии ввода знака и значения порядка
                    part_of_num = 5;

                else
                    return NOT_TEMPLATE; // Сценарии, подходящие по шаблону, не были обнаружены
                break;

            case 2:
                //проверка длины мантиссы
                if (i >= sign + LEN_OF_MANT && data[i] != 'E' && data[i] != '.')
                    return LEN_MANT_LIMIT;
                if (data[i] == 'E') // Прочитан знак "E", далее следует знак порядка
                {
                    part_of_num = 5;
                }

                else if (data[i] == '.')
                    part_of_num = 3;

                else if (!is_digit(data[i])) {
                    return NOT_TEMPLATE;
                }

                break;

            case 3:
                point = TRUE;
                if (is_digit(data[i])) {
                    temp_mant_len++;
                    part_of_num = 4;
                }
                else
                {
                    if (data[i] == '\0')
                        return SUCCESS;
                    else if (data[i] == 'E')
                        part_of_num = 4;
                    else
                        return NOT_TEMPLATE;
                }

            case 4:
                if (data[i] == 'E'){
                    if (i >= sign + point + LEN_OF_MANT && data[i] != 'E' && data[i] != '.')
                        return LEN_MANT_LIMIT;
                    part_of_num = 5;
                }
                else if (!is_digit(data[i]))
                    return NOT_TEMPLATE;
                break;

            case 5:
                if (data[i] == '+' || data[i] == '-')
                    part_of_num = 6;
                if (is_digit(data[i])){
                    temp_exp_len++;
                    part_of_num = 6;
                }
                else if(data[i] != '-' && data[i] != '+')
                    return NOT_TEMPLATE;
                break;

            case 6:
                if(!is_digit(data[i]))
                    return NOT_TEMPLATE;
                else
                    temp_exp_len++;
                if (temp_exp_len > MAX_EXP_LEN)
                    return LEN_EXP_LIMIT;
                break;
            default:
                return LEN_EXP_LIMIT;

        }
    }
    return SUCCESS;
}

//Приводим строку к нормализованному виду в структуре num
void parse_input_data(char* data, num* data_struct){ // Заполнение полей структуры
    int point = FALSE; // логический флаг, поднимается если в мантиссе есть точка
    int temp_data_pos = 0;
    int temp_mant_pos = 0;
    int mant_flag = FALSE; //Логический флаг, поднимается если мантисса не была введена
    if (data[temp_data_pos] == '-')
    {
        data_struct->sign_m = FALSE; // Записываем знак минуса в отельное поле структуры
    }
    if (data[temp_data_pos] == '-' || data[temp_data_pos] == '+')
        temp_data_pos++;
    else if (data[temp_data_pos] == 'E') // если знака и значения не было, а была "Е", то наше значение - 1
    {
        strcpy(data_struct->mant, ".1000000000000000000000000000000\0");
        mant_flag = TRUE;
    }
    while ((is_digit(data[temp_data_pos]) || data[temp_data_pos] == '.')
           && temp_mant_pos < SIZE_OF_MANT - 1) // запись значения в мантиссу
    {
        if (data[temp_data_pos] == '.') // Отмечаем точку в мантиссе, если она есть
            point = TRUE;
        data_struct->mant[temp_mant_pos++] = data[temp_data_pos++];
    }
    if (!point){ //Если точка не была введена всёравно вставляем её в конец мантиссы(нормализация)
        data_struct->mant[temp_mant_pos++] = '.';
    }
    while (temp_mant_pos < SIZE_OF_MANT - 1) // дополняем мантиссу нулями до размера в 30 значащих чисел (может повлиять на точность деления)
        data_struct->mant[temp_mant_pos++] = '0';
    data_struct->mant[temp_mant_pos] = '\0';
    int sign_exp = 1; // знак порядка по умолчанию
    if (data[temp_data_pos] == 'E') // переходим к считыванию значения порядка
        temp_data_pos++;
    if (data[temp_data_pos] == '-') // проверяем знак порядка
    {
        sign_exp = -1;
        temp_data_pos++;
    }
    else if (data[temp_data_pos] == '+') {
        temp_data_pos++; // Пропускаем потому что уже учли этот случай
    }
    data_struct->exp_num = 0;
    while (data[temp_data_pos]) // Считываем порядок
    {
        data_struct->exp_num = data_struct->exp_num * 10 + (int)(data[temp_data_pos++] - '0');
    }
    data_struct->exp_num += mant_flag;
    data_struct->exp_num *= sign_exp;

    //Преобразуем мантиссу к виду .(0)xxxxx.. Сдвигаем точку и увеличиваем порядок
    int temp_point_pos = 0;
    while (data_struct->mant[temp_point_pos] != '.') // Ищем позицию точки
    {
        temp_point_pos++;
    }
    data_struct->exp_num += temp_point_pos; // Увеличиваем значение порядка на величину сдвига
    while (temp_point_pos > 0)
    {
        data_struct->mant[temp_point_pos] = data_struct->mant[temp_point_pos - 1]; //Сдвигаем точку
        temp_point_pos--;
    }
    data_struct->mant[temp_point_pos] = '0'; // Заменяем точку нулём чтобы сделить за переполнением
}

int is_zero(char* mant)
{
    for(int i = 0; i < SIZE_OF_MANT; i++){
        if (mant[i] != '0')
            return FALSE;
    }
    return TRUE;
}

int mant_cmp(num divident, num divisor) // Сравнивает мантиссы, возвращает TRUE если делимое >= делителю, FALSE иначе
{
    for(int i = 0; divident.mant[i]; i++)
    {
        if ((int)(divident.mant[i] - divisor.mant[i]) < 0)
            return FALSE;
        if ((int)(divident.mant[i] - divisor.mant[i]) > 0 )
            return TRUE;
    }
    return TRUE;
}



void mant_shift(num* data_struct)  //Сдвигаем мантиссу влево
{
    int shift = 1;
    for (int i = 0; i < SIZE_OF_MANT - 1 - shift; i++)
        data_struct->mant[i] = data_struct->mant[i + shift];
    data_struct->mant[SIZE_OF_MANT - 1 - shift] = '0';
}

int div_by_int(num* divident, num divisor)
{
    if (!mant_cmp(*divident, divisor)) // невозможно сделать вычитание, так как второе число больше
        return FALSE;
    for (int i = strlen(divisor.mant) - 1; i >= 0; i--) // классическая процедура вычитания в столбик
    {
        if (divident->mant[i] - divisor.mant[i] >= 0) // если меньшему разряду не нужно занимать из более высокого разряда десятку
            divident->mant[i] -= divisor.mant[i] - '0';
        else
        {
            int j = i - 1; // ищем, у кого занять
            while (divident->mant[j] - '0' == 0)
                j--;
            divident->mant[j]--;
            j++;
            for (; j < i; j++) //обновляем разряды
                divident->mant[j] += 9;
            divident->mant[i] += 10 - (divisor.mant[i] - '0'); // делаем вычитание в разряде-заемщике
        }
    }
    return TRUE;
}

int div_iter(num* divident, const num divisor)
{
    int iter = 0;
    while (div_by_int(divident, divisor)) // Деление нацело
        iter++;
    mant_shift(divident); // сдвигаем мантиссу (аналогично тому, как происходит деление в столбик)
    return iter;
}

num division(num divident, num divisor, int *res_is_correct)
{
    num result;
    strcpy(result.mant, "0\n");
    result.exp_num = 0;
    *res_is_correct = TRUE; // Корректность результата

    if (is_zero(divident.mant)) //Если делитель == 0
        return result;

    int temp = 0; // переменная для хранения результата деления текущей стадии
    if (!mant_cmp(divident, divisor)) // если делимое меньше, чем делитель, то берем из степени единицу и
        // домножаем мантиссу на 10
    {
        divident.exp_num -= 1;
        mant_shift(&divident);
    }
    result.mant[0] = '0';

    temp = div_iter(&divident, divisor); // находим первую цифру результата (если мантисса делимого равна нулю, то деление прекращается)
    result.mant[1] = temp + '0';
    int i;

    for (i = 2; i < LEN_OF_MANT + 1; i++) // находим все остальные цифры путем деления в столбик
    {
        if (is_zero(divident.mant)) // если остаток после последнего деления = 0, то числа поделились нацело: конец
        {
            result.mant[i] = '\0';
            temp = 0;
            break;
        }
        if (!mant_cmp(divident, divisor)) // если не вычитается, то берем больше, а в результат идет 0
        {
            mant_shift(&divident);
            result.mant[i] = '0';
        }
        else // если возможно - делим
        {
            temp = div_iter(&divident, divisor);
            result.mant[i] = '0' + temp;
        }
    }
    result.mant[LEN_OF_MANT + 1] = '\0';

    if (i == LEN_OF_MANT + 1 && (temp = div_iter(&divident, divisor)) >= 5) // округление
        result.mant[LEN_OF_MANT] += 1;

    for(; i < LEN_OF_MANT + 1; i++) //приведение матиссы к 30 знакам
        result.mant[i] = '0';
    result.mant[LEN_OF_MANT + 1] = '\0';

    int inc = FALSE;
    for (i = LEN_OF_MANT; i >= 0; i--) // округление высших разрядов (в случае 0.999....)
    {
        result.mant[i] += inc;
        if (result.mant[i] == ':')
        {
            result.mant[i] = '0';
            inc = TRUE;
        }
        else
            break;
    }
    if (result.mant[0] == '1')
    {
        for(i = LEN_OF_MANT; i > 0; i--)
            result.mant[i] = result.mant[i - 1];
        result.mant[0] = '0';
    }
    else
        inc = FALSE;
    result.mant[SIZE_OF_MANT - 1] = '\0';
    if ((divident.sign_m && divisor.sign_m) || (!divident.sign_m && !divisor.sign_m))
        result.sign_m = TRUE;
    else
        result.sign_m = FALSE;
    result.exp_num = divident.exp_num - divisor.exp_num + 1 + inc; // так как уменьшаем порядок во время сдвига результата вправо
    return result;
}

void print_result(num data_struct)
{
    if (abs(data_struct.exp_num) > 99999)
    {
        printf("Выход за пределы разрядной сетки.");
        return;

    }
    printf("                 10        20        30        40\n");
    printf("         ---------|---------|---------|---------|\n");
    printf("Result = ");
    if (data_struct.sign_m == 0)
        printf("-");
    int i, len = strlen(data_struct.mant);
    for (i = len - 1; data_struct.mant[i] == '0' && i > 1; i--);
    data_struct.mant[i + 1] = '\0';
    printf("0.%s", &data_struct.mant[1]);
   // if (data_struct.exp_num != 0)
    printf("E%d", data_struct.exp_num);
}

int main(){
    //system("chcp 65001");

    setbuf(stdout, NULL); // отключение буферизации

    char input_data[MAX_LEN + 1]; // Строка с непроверенными данными, введёнными пользователем.
    num dividend, divisor, result;
    dividend.sign_m = TRUE;
    divisor.sign_m = TRUE;
    int code;
    // Получение данных от пользователя и проверка корректности.
    //Если пользоввтель ввёл данные не правильно, завершаем работу программы, возвращаем соответсвующий ошибке код,
    // выводим сообщение для информирования пользователя.

    printf("Введите делимое - действительное число в форме (+-)m.nЕ(+-)K, где суммарная длина мантиссы (m+n) - до 30 значащих цифр,"
           " а величина порядка K - до 5 цифр.\n");
    printf("В вводимой строке не допускаются пробелы\n");
    printf("         10        20        30       40\n");
    printf("---------|---------|---------|---------|\n");
    code = my_input(input_data, FALSE);
    if (code){
        print_error_message(code);
        return code;
    }
    code = check_data(input_data);
    if (code){
        print_error_message(code);
        return code;
    }
    parse_input_data(input_data, &dividend);

    printf("Введите делитель - целое число, длиной не более 30 десятичных цифр.\n");
    printf("         10        20        30\n");
    printf("---------|---------|---------|\n");
    code = my_input(input_data, TRUE);
    if (code){
        print_error_message(code);
        return code;
    }
    code = check_data(input_data);
    if (code){
        print_error_message(code);
        return code;
    }
    parse_input_data(input_data, &divisor);
    int result_is_correct;
    result = division(dividend, divisor, &result_is_correct);
    if (!result_is_correct)
        printf("В результате деления было полученно некорректное число.\n");
    if (result_is_correct == LIMIT_ERROR)
        printf("Выход за пределы разрядной сетки.");
    else
        print_result(result);
}