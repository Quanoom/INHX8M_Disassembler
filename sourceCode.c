#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 100


#define ZERO_DIGITS 0
#define ONE_DIGITS 1
#define TWO_DIGITS 10
#define THREE_DIGITS 100
#define FOUR_DIGITS 1000


bool parseRecord(unsigned char* str, unsigned char* array[]);
unsigned char strToInt(const unsigned char*, unsigned char, unsigned char);
unsigned char* fillArrayDynamic(unsigned char*, unsigned char, unsigned char*);
void generateAsmCode(unsigned char*, unsigned char);

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
            if(!parseRecord(str, array))
                break;
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
bool parseRecord(unsigned char* str, unsigned char* array[])
{
    char counter = 2;
    unsigned char* holder = NULL, j = 1, state = 1, i = 1, byteCount = 0, address = 0;
    while(str[i] != '\0')
    {
        switch(state)
        {
            case 1 : 
                holder = fillArrayDynamic(str, counter, &i);
                byteCount = strToInt(holder, counter, 0);
                state = 2;
                counter = 4;
                break;
            case 2 : 
                holder = fillArrayDynamic(str, counter, &i);
                address = strToInt(holder, counter, 1);
                if(address % 2 != 0)
                    return false;
                state = 3;
                counter = 2;
                break;
            case 3 : 
                holder = fillArrayDynamic(str, counter, &i);
                state = 4;
                counter = byteCount;
                break;
            case 4 : 
                holder = fillArrayDynamic(str, counter, &i);
                generateAsmCode(holder, address);
                state = 5;
                counter = 2;
                break;
            case 5 : 
                holder = fillArrayDynamic(str, counter, &i);
                break;
        }
        free(holder);
    }
    printf("\n\n");
    return true;
}
void generateAsmCode(unsigned char* machineCode, unsigned char startAddress)
{
    FILE* filePointer = fopen("assemblyCode.txt", "a");
    if(filePointer == NULL)
        printf("FAIL TO OPEN THE FILE\n");
    else
    {
        unsigned char array[5] = {'\0'}, lastAddress = 0;
        lastAddress = startAddress + (strlen(machineCode) / 2) - 2;
        unsigned char sequence[] = {2, 3, 0, 1}, offset = 4, index, i = 0;
        while(startAddress <= lastAddress)
        {
            index = 0;
            printf("0x%X\t", startAddress);
            for(; index < 4; ++index)
                array[index] = machineCode[sequence[index] + i];
            printf("%s\n", array);
            startAddress += 2;
            i += 4;
        }
    }
}