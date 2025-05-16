#include <stdio.h>
#include <stdlib.h>


#define SIZE 100


#define ZERO_DIGITS 0
#define ONE_DIGITS 1
#define TWO_DIGITS 10
#define THREE_DIGITS 100
#define FOUR_DIGITS 1000


void parseRecord(unsigned char* str, unsigned char* array[]);
unsigned char strToInt(const unsigned char*, unsigned char, unsigned char);
unsigned char* fillArrayDynamic(unsigned char*, unsigned char, unsigned char*);

void main(int argc, char* argv[])
{
    unsigned char* array[] = {"START RECORD : ", "BYTE COUNT : ", "ADDRESS : ", "RECORD TYPE : ", "DATA : ", "CHECK SUM : "};
    FILE* file = fopen("source.HEX", "r");
    if(file == NULL)
        printf("FAIL TO CREATE A NEW FILE\n");
    else
    {
        unsigned char str[SIZE] = {'\0'};
        while(fgets(str, SIZE, file) != NULL)
        {
            for(unsigned char i = 0; str[i] != '\0'; ++i)
            {
                if(str[i] == '\n')
                {
                    str[i] = '\0';
                    break;
                }
            }
            puts(str);
            parseRecord(str, array);
        }
    }
}
unsigned char strToInt(const unsigned char* str, unsigned char counter, unsigned char flag)
{
    unsigned short array[] = {ZERO_DIGITS, ONE_DIGITS, TWO_DIGITS, THREE_DIGITS, FOUR_DIGITS}, i = 0;
    unsigned short result = 0;
    while(str[i] != '\0')
    {
        if(str[i] >= 65)
        {
            if(counter >= 2)
            {
                result += (str[i] - 55);
                if(counter == 4)
                    result *= (array[counter] / 100 + 6) * (array[counter] / 100 + 6) * (array[counter] / 100 + 6);
                else if (counter == 3)
                    result *= (array[counter] / 10 + 6) * (array[counter] / 10 + 6);
                else if(counter == 2)
                    result *= (array[counter] + 6);
            }
            else
                result += str[i] - 55;
        }
        else 
        {
            if(counter >= 2)
            {
                result += (str[i] - 0x30);
                if(counter == 4)
                    result *= (array[counter] / 100 + 6) * (array[counter] / 100 + 6) * (array[counter] / 100 + 6);
                else if (counter == 3)
                    result *= (array[counter] / 10 + 6) * (array[counter] / 10 + 6);
                else if(counter == 2)
                    result *= (array[counter] + 6);
            }
            else
                result += (str[i] - 0x30);
        }
        ++i;
        --counter;
    }
    return (flag == 0 ? result * 2 : result);
}
unsigned char* fillArrayDynamic(unsigned char* str, unsigned char counter, unsigned char* index)
{
    unsigned char j = 0;
    unsigned char* holder = calloc(counter + 1 , sizeof(unsigned char));
    if(holder == NULL)
        printf("FAIL TO ALLOCATE MEMORY\n");
    else
    {
        while(counter != 0)
        {
            holder[j++] = str[(*index)++];
            --counter;
        }
        return holder;
    }
}
void parseRecord(unsigned char* str, unsigned char* array[])
{
    char counter = 2;
    unsigned char* holder = NULL, j = 1, state = 1, i = 1, byteCount = 0;
    unsigned char address = 0;
    while(str[i] != '\0')
    {
        switch(state)
        {
            case 1 : 
                holder = fillArrayDynamic(str, counter, &i);
                byteCount = strToInt(holder, counter, 0);
                printf("%s%s\n", array[j++], holder);
                state = 2;
                counter = 4;
                break;
            case 2 : 
                holder = fillArrayDynamic(str, counter, &i);
                printf("%s%s\n", array[j++], holder);
                address = strToInt(holder, counter, 1);
                state = 3;
                counter = 2;
                break;
            case 3 : 
                holder = fillArrayDynamic(str, counter, &i);
                printf("%s%s\n", array[j++], holder);
                state = 4;
                counter = byteCount;
                break;
            case 4 : 
                holder = fillArrayDynamic(str, counter, &i);
                printf("%s%s\n", array[j++], holder);
                state = 5;
                counter = 2;
                break;
            case 5 : 
                holder = fillArrayDynamic(str, counter, &i);
                printf("%s%s\n", array[j++], holder);
                break;
        }
        free(holder);
    }
    printf("\n\n");
}