#include "enums.h"
#include "Reading file.cpp"


struct label
{
   char name[MAX_LABEL_NAME] = "";
   int address = -1;
};

typedef label elem_t;
#include "DynamicArray.cpp"

void MakeLabel           (char* str, DynamicArray* arr, int address);
int  AddressOfLabel      (char* str, DynamicArray* arr);
bool IsRegister          (char* str);
MODE GetMode             (char* str);
void CompileRegister     (char* str, char* buffer, int* ofs, char* old_str);
void CompileArguments    (char* str, int len, char* buffer, int* ofs);
void CompileJumpArguments(char* str, int len, char* buffer, int* ofs, char* command, DynamicArray* labels);
int  CompileText         (str* lines, size_t num, char* buffer, DynamicArray* labels);
void CompileFile         (FILE* user_file, FILE* work_file);

int main(const int argc, const char* argv[])
{   
    FILE* user_input = nullptr;
    
    if (argc - 1 == 0)
    {
     	user_input = fopen("input.txt", "r");
    }
        
    if (argc - 1 > 0)
    {
        user_input = fopen(argv[1], "r");
    }

    if (user_input == nullptr)
    {
        assert(! "can't open user's input file\n");
    }


    FILE* work_file = fopen("bin\\work_file.bin", "wb");
    if (work_file == nullptr)
    {
        assert(! "can't open work file\n");
    }

    CompileFile(user_input, work_file);

    fclose(work_file);
           work_file = nullptr;

    fclose(user_input);
           user_input = nullptr;

    return 0;
}

void MakeLabel(char* str, DynamicArray* arr, int address)
{
    label new_label = {};
    int len = 0;                                  
    sscanf(str, "%s%n", new_label.name, &len);
    new_label.name[len] = '\0';

    new_label.address = address; 
    PushArr(arr, new_label);
}

int AddressOfLabel(char* str, DynamicArray* arr)
{
    for (size_t i = 0; i < arr->size; ++i)
    {
        if (strcmp(arr->buffer[i].name, str) == 0)
        {
            return arr->buffer[i].address + VERS_SIZE;
        }
    }

    return -1;
}

bool IsRegister(char* str)
{
    return(str[0] ==  'r'                     &&
           str[1] >=  'a'                     &&
           str[1] <= ('a' + NUM_OF_REGISTERS) &&
           str[2] ==  'x');
}

MODE GetMode(char** str)
{
    double tmp = 0;
    bool IsPlus = false;
    if (strchr(*str, '+')) IsPlus = true;

    bool IsBrace = false;
    if ((*str)[0] == '[') 
    {
        (*str)++;
        IsBrace = true;
    }

    if (!IsPlus)
    {
        if (sscanf(*str, "%lf", &tmp)) 
        {
            if (IsBrace) return RAM_DOUBLE;
            return STACK_DOUBLE;
        }
        if (IsBrace) return RAM_REGISTER;
        return STACK_REGISTER;
    }
    if (IsBrace) return RAM_BOTH;
    return STACK_BOTH;
}

void CompileRegister(char* str, char* buffer, int* ofs, char* old_str)
{
    if (IsRegister(str))                                            
    {                                                                   
        buffer[(*ofs)++] = str[1] - 'a' + 1;                      
    }                                                                   
    else /*if register is ok*/                                          
    {                              
        printf("syntax error : unknown register\nCheck line : ");
        printf("%s\n", old_str);                                            
    }   
}

void CompileArguments(char* str, int len, char* buffer, int* ofs)
{
    double elem = 0;                       

    char* old_str = str;
    while (str[len + 1] == ' ')  len++;                                 
    str += len + 1;
    
    int mode = GetMode(&str);
    buffer[(*ofs)++] = mode;
    mode = mode % 4; //if mode == ram func GetMode increases str and now it 
                     //is the same that mode == stack 
    switch (mode)
    {
        case STACK_DOUBLE : 
        {
            sscanf(str, "%lf", &elem);
            *(double*)(buffer + *ofs) = elem;                                   
            *ofs += sizeof(double);   
            break; 
        }
        case STACK_REGISTER : 
        {
            CompileRegister(str, buffer, ofs, old_str);
            break;
        }
        case STACK_BOTH : 
        {   
            CompileRegister(str, buffer, ofs, old_str);
            str = strchr(str, '+') + 1;

            sscanf(str, "%lf", &elem);
            *(double*)(buffer + *ofs) = elem;
            *ofs += sizeof(double);   
            break;       
        }
    }
}

void CompileJumpArguments(char* str, int len, char* buffer, int* ofs, char* command, DynamicArray* labels)
{
    sscanf(str + len + 1, "%s", command);
    *(double*)(buffer + *ofs) = AddressOfLabel(command, labels);
    *ofs += sizeof(double);
}

int CompileText(str* lines, size_t num, char* buffer, DynamicArray* labels)
{
    assert(lines  != nullptr);
    assert(buffer != nullptr);

    int ofs = 0;

    for (size_t j = 0; j < num; j++)
    {	
        int len = -1;
        char* command = (char*)calloc(MAX_COMMAND_SIZE, sizeof(char));

        char* comment = strchr(lines[j].str, ';');
        if (comment) comment[0] = '\0'; 

        sscanf(lines[j].str, "%s%n", command, &len);

        char* label = strchr(lines[j].str, ':'); 
        if (label) 
        {
            MakeLabel(lines[j].str, labels, ofs);
            continue;
        }

        #define DEF_CMD(name, num, arg, code)                                                   \
                if (strcmp(command, #name) == 0)                                                \
                {                                                                               \
                    buffer[ofs++] = num;	                                                \
                    if (arg == 1)                                                               \
                    {                                                                           \
                        CompileArguments(lines[j].str, len, buffer, &ofs);                      \
                    }                                                                           \
                    else if (arg == -1)/*jmp arg*/                                              \
                    {                                                                           \
                        CompileJumpArguments(lines[j].str, len, buffer, &ofs, command, labels); \
                    }                                                                           \
                } else
        #include "commands.h"   
         /*else*/
            if (lines[j].len) 
            {
                printf("Syntax error\nCheck line : %llu\n", j);
                printf("%s\n", lines[j].str);
            }
        
	free(command);
    }

    #undef DEF_CMD
    return ofs;
}

void CompileFile(FILE* user_file, FILE* work_file)
{
    assert(user_file);
    assert(work_file);

    text user_text = {};
    ReadTextAndMakeLines(&user_text, user_file);

    char* buffer = (char*)calloc(MAX_SIZE, sizeof(char));
    DynamicArray* labels = ConstructArray(DYNAMIC_ARRAY_STANDART_SIZE);

    int ofs = CompileText(user_text.lines, user_text.num_str, buffer, labels);
    CompileText(user_text.lines, user_text.num_str, buffer, labels);

    fwrite("ZS1.0", VERS_SIZE, sizeof(char), work_file);
    fwrite(buffer, ofs, sizeof(char), work_file);

    free(buffer);
}

