#include <stdio.h>
#include <stdlib.h>
#define SIZE 100


void parseRecord(unsigned char* str, unsigned char* array[]);

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
            puts(str);
            parseRecord(str, array);
        }
    }
}
void parseRecord(unsigned char* str, unsigned char* array[])
{
    char counter = 2;
    unsigned char* holder = NULL, j = 0, state = 0, k = 0, i = 0, byteCount = 0;
    while(str[i] != '\0')
    {
        holder = calloc(counter + 1 , sizeof(unsigned char));
        if(state == 0)
        {
            printf("%s%c\n", array[j++], str[i++]);
            state = 1;
        }
        else if(state == 1)
        {
            while(counter != 0)
            {
                holder[k++] = str[i++];
                --counter;
            }
            if(holder[1] == 'A')
                byteCount = 10; 
            else if(holder[1] == 'B')
                byteCount = 11; 
            else if(holder[1] == 'C')
                byteCount = 12; 
            else if(holder[1] == 'D')
                byteCount = 13;
            else if(holder[1] == 'E')
                byteCount = 14;
            else if(holder[1] == 'F')
                byteCount = 15;  
            else
                byteCount = holder[1] - 0x30;
            if(holder[0] - 0x30 >= 1)
                byteCount += (holder[0] - 0x30) * 10 + 6; 
            byteCount *= 2;
            printf("bytecount = %u\n", byteCount);
            printf("%s%s\n", array[j++], holder);
            state = 2;
            counter = 4;
        }
        else if(state == 2)
        {
            while(counter != 0)
            {
                holder[k++] = str[i++];
                --counter;
            }
            printf("%s%s\n", array[j++], holder);
            state = 3;
            counter = 2;
        }
        else if(state == 3)
        {
            while(counter != 0)
            {
                holder[k++] = str[i++];
                --counter;
            }
            printf("%s%s\n", array[j++], holder);
            state = 4;
            counter = byteCount;
        }
        else if(state == 4)
        {
            while(counter != 0)
            {
                holder[k++] = str[i++];
                --counter;
            }
            printf("%s%s\n", array[j++], holder);
            state = 4;
            counter = byteCount;
        }
        k = 0;
    }
}