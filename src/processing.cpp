#include "super_puper_stack_struct.h"
#include "enums.h"

#include "TxLib.h"

struct CPU
{
    char* vers = {};
    Stack stack = {};

    Stack call = {};

    char* work_code = nullptr;

    double RGS[NUM_OF_REGISTERS] = {};
    double* RAM = nullptr;

    size_t max_ofs = 0;
    size_t ofs = 0;
};


bool IsVersOk       (CPU* cpu);
void CheckVersion   (CPU* cpu);
void ConstructCPU   (CPU* cpu, FILE* work_file);
void DestroyCPU     (CPU* cpu);
void WorkWithCommand(CPU* cpu);
void processing     (CPU* cpu);


int main(const int argc, const char* argv[])
{
    FILE* work_file = fopen("bin\\work_file.bin", "rb");
    if (work_file == nullptr)
    {
        assert(! "can't open work file\n");
    }

    CPU cpu = {};
    ConstructCPU(&cpu, work_file);

    fclose(work_file);
           work_file = nullptr;

    processing(&cpu);

    DestroyCPU(&cpu);

    return 0;
}

bool IsVersOk(CPU* cpu)
{
    assert(cpu);

    return (cpu->work_code[0] == cpu->vers[0] and
            cpu->work_code[1] == cpu->vers[1] and
            cpu->work_code[2] == cpu->vers[2] and
            cpu->work_code[4] == cpu->vers[4]);
}

void CheckVersion(CPU* cpu)
{
    assert(cpu);

    if (!IsVersOk(cpu))
    {
        assert(! "compiler and assembler versions do not match");
    }
}

void ConstructCPU(CPU* cpu, FILE* work_file)
{
    assert(cpu);

    char vers[6] = "ZS1.0";
    cpu->vers = vers;
    CONSTRUCT(&cpu->stack);
    CONSTRUCT(&cpu->call);

    cpu->work_code = (char*)calloc(MAX_SIZE, sizeof(char)); 
    cpu->max_ofs = fread(cpu->work_code, sizeof(char), MAX_SIZE, work_file);
    
    cpu->RAM = (double*)calloc(MILLION_OF_TERABYTES, sizeof(double));

    CheckVersion(cpu);
    cpu->ofs = VERS_SIZE;
}

void DestroyCPU(CPU* cpu)
{
    assert(cpu);
    assert(cpu->work_code);
    assert(cpu->RAM);

    cpu->vers = nullptr;
    Destroy(&cpu->stack);

    free(cpu->work_code);
    free(cpu->RAM);
}

void WorkWithCommand(CPU* cpu)
{
    assert(cpu);

    int command = cpu->work_code[cpu->ofs];
   
    #define DEF_CMD(name, num, arg, code) \
        case num :                        \
        {                                 \
            code                          \
            break;                        \
        }

    switch (command)
    {
        #include "commands.h"

        default : 
        {
            printf("unknown command in bytecode(ofs = %d, code : %d)\n", cpu->ofs, cpu->work_code[cpu->ofs]);
        }
    }

    cpu->ofs++;
}

void processing(CPU* cpu)
{
    assert(cpu);

    while (cpu->ofs < cpu->max_ofs)
    {
        WorkWithCommand(cpu);
    }
}
