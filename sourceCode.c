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
#define LITERAL_INSTRUCTION 6
#define BYTE_INSTRUCTION 4
#define JUMP_CALL_INSTRUCTION 1


typedef enum states     
{
    BYTE_COUNT,
    ADDRESS,
    RECORD_TYPE,
    DATA_SECTION,
    CHECK_SUM
}STATE;

typedef enum instructions
{
    LITERAL,
    BIT,
    BYTE,
    JUMP_CALL
}INST;


typedef struct bits
{
    unsigned char a : 1;
    unsigned char d : 1;
}BITS;

typedef struct list
{
    unsigned char instruction[LENGTH];
    unsigned short opcode;
}LIST;

bool parseRecord(unsigned char* str, unsigned char* array[]);
unsigned short strToInt(const unsigned char*, unsigned char, unsigned char);
unsigned char* fillArrayDynamic(unsigned char*, unsigned char, unsigned char*);
void generateAsmCode(unsigned char*, unsigned char);
bool checkOpcode(const unsigned char*, unsigned char);
bool sfunction(LIST*, unsigned short, unsigned char, unsigned char);
bool function(unsigned char , INST, LIST*, unsigned char, unsigned short);


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
unsigned short strToInt(const unsigned char* str, unsigned char counter, unsigned char flag)
{
    unsigned short array[] = {ZERO_DIGITS, ONE_DIGITS, TWO_DIGITS, THREE_DIGITS, FOUR_DIGITS}, i = 0;
    unsigned short result = 0, temp = 0;
    while(str[i] != '\0')
    {
        if(str[i] >= 65)
        {
            if(counter >= 2)
            {
                //result += (str[i] - 55);
                temp = (str[i] - 55);
                if(counter == 4)
                    temp *= (array[counter] / 100 + 6) * (array[counter] / 100 + 6) * (array[counter] / 100 + 6);
                else if (counter == 3)
                    temp *= (array[counter] / 10 + 6) * (array[counter] / 10 + 6);
                else if(counter == 2)
                    temp *= (array[counter] + 6);
            }
            else
                temp = str[i] - 55;
            result += temp;
        }
        else 
        {
            if(counter >= 2)
            {
                temp = (str[i] - 0x30);
                if(counter == 4)
                    temp *= (array[counter] / 100 + 6) * (array[counter] / 100 + 6) * (array[counter] / 100 + 6);
                else if (counter == 3)
                    temp *= (array[counter] / 10 + 6) * (array[counter] / 10 + 6);
                else if(counter == 2)
                    temp *= (array[counter] + 6);
            }
            else
                temp = (str[i] - 0x30);
            result += temp;
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
    unsigned char* holder = NULL, j = 1, i = 1;
    unsigned short byteCount = 0, address = 0;;
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
    printf("ADDRESSES : \tINSTRUCTIONS : \n");
    while(startAddress <= lastAddress)
    {
        index = 0;
        printf("0x%X\t\t", startAddress);
        for(; index < 4; ++index)
            array[index] = machineCode[sequence[index] + i];
        if(! checkOpcode(array, startAddress))
        {
            printf("UNKNOWN OPCODE\n");
            break;
        }
        startAddress += 2;
        i += 4;
    }
}
bool checkOpcode(const unsigned char* str, unsigned char address)
{
    BITS instructionBits;
    unsigned short operationCode = strToInt(str, strlen(str), 1);
    unsigned short result = 0, operand = 0;
    INST which = LITERAL;
    LIST literalInstruction[LITERAL_INSTRUCTION] = {{.instruction = "MOVLW", .opcode = 0x000E}, {.instruction = "ADDLW", .opcode = 0x000F}, 
    {.instruction = "ANDLW", .opcode = 0x000B},{.instruction = "XORLW", .opcode = 0x000A}, 
    {.instruction = "IORLW", .opcode = 0x0009},{.instruction = "SUBLW", .opcode = 0x0008}};

    LIST byteOrientedInstruction[BYTE_INSTRUCTION] = {{.instruction = "MOVWF", .opcode = 0x0037}, 
    {.instruction = "NEGF", .opcode = 0x0036}, {.instruction = "MULWF", .opcode = 0x0001}, 
    {.instruction = "SETF", .opcode = 0x0034}};

    LIST jumpCallIns[JUMP_CALL_INSTRUCTION] = {{.instruction = "BRA", .opcode = 0x000D}};


    result = (operationCode & 0xFF00) >>8;
    operand = operationCode & 0x00FF;
    
    if(function(result, which, literalInstruction, operand, operationCode))
        return true;
    else
    {
        result = (operationCode & 0xFE00) >> 9;
        operand = operationCode & 0x00FF;
        which = BYTE;
        if(function(result, which, byteOrientedInstruction, operand, operationCode))
            return true;
        else
        {
            result = (operationCode & 0xF800) >> 12;
            operand = (operationCode & 0x07FF) * 2 + address + 2 ;
            which = JUMP_CALL;
            if(function(result, which, jumpCallIns, operand, operationCode))
                return true;
            else
                return false;
        }
    }
}
bool function(unsigned char result, INST which, LIST* array, unsigned char operand, unsigned short operationCode)
{
    unsigned char max = 0;
    BITS instructionBits;
    if(which == LITERAL)  
        max = LITERAL_INSTRUCTION;
    else if(which == BYTE)
        max = BYTE_INSTRUCTION;
    else if(which == JUMP_CALL)
        max = JUMP_CALL_INSTRUCTION;
    for(unsigned char i = 0; i < max; ++i)
    {
        if(result == array[i].opcode)
        {
            if(which == LITERAL)
            {
                printf("%s\t0x%X\n", array[i].instruction, operand);
                return true;
            }
            else if(which == BYTE)
            {
                instructionBits.a = (operationCode & 0x0100) >> 8;
                printf("%s\t0x%X, %hu\n", array[i].instruction, operand, instructionBits.a);
                return true;
            }
            else if(which == JUMP_CALL)
            {
                printf("%s\t0x%X\n", array[i].instruction, operand);
                return true;
            }
        }
    } 
    return false;
}