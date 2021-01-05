#pragma once


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define MYSUPERPUPERDEBUG

#ifdef MYSUPERPUPERDEBUG
#define CONSTRUCT(stack) Construct(stack, STANDARD_SIZE_STACK, #stack)
#define ASSERT_OK(stack); if(!StackOK(stack)) {StackDump(stack); assert(!"OK");}
#else
#define CONSTRUCT(stack) Construct(stack, STANDARD_SIZE_STACK, #stack)
#define ASSERT_OK; ;
#endif

const long long int CANARY = 0xBADC0DEDEAD;
const unsigned  int MAX_SIZE_STACK = 2000000;
const unsigned  int STANDARD_SIZE_STACK = 10;
typedef double Elem_t;

#ifdef MYSUPERPUPERDEBUG
enum STACK_STATUS {STACK_IS_NOT_OK = 0, STACK_IS_OK = 1};
enum STACK_ERROR
{
    NO_ERR,
    SIZE_ERR,
    CAPACITY_ERR,
    POP_EMPTY,
    TOP_EMPTY,
    STACK_POINTER_ERR,
    BUFFER_POINTER_ERR,
    BUFFER_ERR,
    BUF_CANARIES_ERR,
    STR_CANARIES_ERR,
    HASH_ERR,
};
#endif

struct Stack
{
    #ifdef MYSUPERPUPERDEBUG
    double left_canary_ = CANARY;
    #endif
    size_t capacity_ = 0;
    size_t size_ = 0;
    Elem_t* buffer_ = nullptr;

    #ifdef MYSUPERPUPERDEBUG
    const char* name_stack_ = nullptr;
    int hash_ = 0;
    STACK_ERROR status_ = NO_ERR;
    double right_canary_ = CANARY;
    #endif
};

void         Construct     (Stack* stack, size_t size, const char* name_stack);
size_t       GetStkSize    (Stack* stack);
void         push          (Stack* stack, Elem_t value);
Elem_t       top           (Stack* stack);
void         pop           (Stack* stack);
void         clear         (Stack* stack);
void         Destroy       (Stack* stack);
void         CheckBuffer   (Stack* stack);
void         reallocateUp  (Stack* stack, size_t size_in_bytes);
void         reallocateDown(Stack* stack, size_t size_in_bytes);

#ifdef MYSUPERPUPERDEBUG
void         SetTheCanaries(Stack* stack);
int          HashFunc      (Stack* stack);
STACK_STATUS StackOK       (Stack* stack);
void         StackDump     (Stack* stack);
#endif
