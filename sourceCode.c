#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 100
#define LENGTH 6
#define ZERO_DIGITS 0
#define ONE_DIGITS 1
#define TWO_DIGITS 10
#define THREE_DIGITS 100
#define FOUR_DIGITS 1000
#define NUMBER_OF_INSTRUCTIONS 8


typedef enum states     
{
    BYTE_COUNT,
    ADDRESS,
    RECORD_TYPE,
    DATA_SECTION,
    CHECK_SUM
}STATE;


typedef struct list
{
    unsigned char instruction[LENGTH];
    unsigned char opcode[3];
}LIST;

bool parseRecord(unsigned char* str, unsigned char* array[]);
unsigned char strToInt(const unsigned char*, unsigned char, unsigned char);
unsigned char* fillArrayDynamic(unsigned char*, unsigned char, unsigned char*);
void generateAsmCode(unsigned char*, unsigned char);
bool checkOpcode(const unsigned char*, const unsigned char );


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
    unsigned char* holder = NULL, j = 1, i = 1, byteCount = 0, address = 0;
    STATE recordFetch = BYTE_COUNT;
    while(str[i] != '\0')
    {
        switch(recordFetch)
        {
            case BYTE_COUNT : 
                holder = fillArrayDynamic(str, counter, &i);
                byteCount = strToInt(holder, counter, 0);
                recordFetch = ADDRESS;
                counter = 4;
                break;
            case ADDRESS : 
                holder = fillArrayDynamic(str, counter, &i);
                address = strToInt(holder, counter, 1);
                if(address % 2 != 0)
                    return false;
                recordFetch = RECORD_TYPE;
                counter = 2;
                break;
            case RECORD_TYPE : 
                holder = fillArrayDynamic(str, counter, &i);
                recordFetch = DATA_SECTION;
                counter = byteCount;
                break;
            case DATA_SECTION : 
                holder = fillArrayDynamic(str, counter, &i);
                generateAsmCode(holder, address);
                recordFetch = CHECK_SUM;
                counter = 2;
                break;
            case CHECK_SUM : 
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
    unsigned char array[5] = {'\0'}, lastAddress = 0;
    lastAddress = startAddress + (strlen(machineCode) / 2) - 2;
    unsigned char sequence[] = {2, 3, 0, 1}, offset = 4, index, i = 0;
    printf("ADDRESSES\tINSTRUCTIONS : \n");
    while(startAddress <= lastAddress)
    {
        index = 0;
        printf("0x%X\t\t", startAddress);
        for(; index < 4; ++index)
            array[index] = machineCode[sequence[index] + i];
        checkOpcode(array, startAddress);
        startAddress += 2;
        i += 4;
    }
}
bool checkOpcode(const unsigned char* str, const unsigned char address)
{
    LIST array[NUMBER_OF_INSTRUCTIONS] = {{.instruction = "BRA", .opcode = "D0"}, {.instruction = "MOVLW", .opcode = "0E"},
            {.instruction = "ADDLW", .opcode = "0F"}, {.instruction = "ANDLW", .opcode = "0B"},
        {.instruction = "XORLW", .opcode = "0A"}, {.instruction = "IORLW", .opcode = "09"},
    {.instruction = "SUBLW", .opcode = "08"}};

    unsigned char operationCode[3] = {'\0'}, operandWithOpcode[3] = {'\0'};
    for(unsigned char i = 0; i < 2; ++i)
        operationCode[i] = str[i];
    for(unsigned char i = 2; i < 4; ++i)
        operandWithOpcode[i - 2] = str[i];

    for(unsigned char i = 0; i < 7; ++i)
    {
        if(strcmp(operationCode, array[i].opcode) == 0)
            printf("%s\t0x%s\n", array[i].instruction, operandWithOpcode);
    }
}