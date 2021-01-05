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

    return 0;
}

bool IsVersOk(CPU* cpu)
{
    return (cpu->work_code[0] == cpu->vers[0] and
            cpu->work_code[1] == cpu->vers[1] and
            cpu->work_code[2] == cpu->vers[2] and
            cpu->work_code[4] == cpu->vers[4]);
}

void CheckVersion(CPU* cpu)
{
    if (!IsVersOk(cpu))
    {
        assert(! "compiler and assembler versions do not match");
    }
}

void ConstructCPU(CPU* cpu, FILE* work_file)
{
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
    cpu->vers = nullptr;
    Destroy(&cpu->stack);

    free(cpu->work_code);
}

void WorkWithCommand(CPU* cpu)
{
    int command = cpu->work_code[cpu->ofs];
   
    #define DEF_CMD(name, num, arg, code) \
        case num :                        \
        {                                 \
                                          \
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

    // StackDump(&cpu->stack);
    // printf("rax : %lf\n", cpu->RGS[0]);
    // printf("rbx : %lf\n", cpu->RGS[1]);
    // printf("rcx : %lf\n", cpu->RGS[2]);
    // printf("%lf  %lf %lf %lf\n", cpu->RAM[0], cpu->RAM[1], cpu->RAM[2], cpu->RAM[2]);
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
