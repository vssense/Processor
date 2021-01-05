#include "super_puper_stack_struct.h"

//-------------------------------------------------------------
//!  Constructing the stack
//!
//!  @param [in] stack       pointer to a stack struct
//!  @param [in] capacity    capacity of a new stack
//!  @param [in] name_stack  name of stack
//!
//!  @note use func by macro CONSTRUCT(you will get stack name)
//-------------------------------------------------------------
void Construct(Stack* stack, size_t capacity, const char* name_stack)
{
    assert(stack);
    if (capacity > MAX_SIZE_STACK)
    {
        printf("ARE YOU REALLY TRYING TO CONSTRUCT STACK WITH THIS SIZE?\n");
        assert(0);
    }
    #ifdef MYSUPERPUPERDEBUG
    stack->buffer_ = (Elem_t*)calloc(capacity + 2, sizeof(Elem_t));
    #else
    stack->buffer_ = (Elem_t*)calloc(capacity, sizeof(Elem_t));
    #endif
    stack->size_ = 0;
    stack->capacity_ = capacity;

    #ifdef MYSUPERPUPERDEBUG
    stack->hash_ = HashFunc(stack);
    stack->name_stack_ = name_stack + 1;
    SetTheCanaries(stack);
    stack->status_ = NO_ERR;
    for (size_t i = 0; i < capacity; ++i)
    {
        stack->buffer_[i] = NAN;
    }
    #endif
    ASSERT_OK(stack);
}

//-------------------------------------------------------------
//!  Getting Stack Size
//!
//!  @param [in] stack    pointer to a stack struct
//!
//!  @return size of stack
//!  @note size is unsigned
//-------------------------------------------------------------

size_t GetStkSize(Stack* stack)
{
    ASSERT_OK(stack);
    return stack->size_;
}

//-------------------------------------------------------------
//!  Pushing an element to stack
//!
//!  @param [in] stack    pointer to a stack struct
//!  @param [in] value    value of element
//!
//!  @note reallocate up buffer if size >= capacity
//!  @note reallocate down buffer if size <= capacity / 4
//-------------------------------------------------------------

void push(Stack* stack, Elem_t value)
{
    ASSERT_OK(stack);
    CheckBuffer(stack);
    stack->buffer_[stack->size_++] = value;
    #ifdef MYSUPERPUPERDEBUG
    stack->hash_ = HashFunc(stack);
    #endif
    ASSERT_OK(stack);
}

//-------------------------------------------------------------
//!  Getting a top element
//!
//!  @param [in] stack    pointer to a stack struct
//!
//!  @return a top element
//!  @note if size == 0 returns NAN
//-------------------------------------------------------------

Elem_t top(Stack* stack)
{
    ASSERT_OK(stack);
    if (stack->size_ > 0)
    {
        return stack->buffer_[stack->size_ - 1];
    }
    #ifdef MYSUPERPUPERDEBUG
    stack->status_ = TOP_EMPTY;
    #endif
    ASSERT_OK(stack);
    return NAN;
}

//-------------------------------------------------------------
//!  Deleting a top element
//!
//!  @param [in] stack    pointer to a stack struct
//!
//!  @note if size == 0 doing nothing(or returns error and assert(0)
//-------------------------------------------------------------

void pop(Stack* stack)
{
    ASSERT_OK(stack);

    if (stack->size_ > 0)
    {
        stack->buffer_[stack->size_ - 1] = NAN;
        stack->size_--;

        #ifdef MYSUPERPUPERDEBUG
        stack->hash_ = HashFunc(stack);
        #endif

        CheckBuffer(stack);
    }
    else
    {
        #ifdef MYSUPERPUPERDEBUG
        stack->status_ = POP_EMPTY;
        #endif
    }

    ASSERT_OK(stack);
}

//-------------------------------------------------------------
//!  Clearing the stack
//!
//!  @param [in] stack    pointer to a stack struct
//!
//!  @note reallocate buffer to capacity = 10
//-------------------------------------------------------------

void clear(Stack* stack)
{
    ASSERT_OK(stack);
    for (size_t i = 0; i < stack->size_; ++i)
    {
        stack->buffer_[i] = NAN;
    }
    stack->size_ = 0;
    #ifdef MYSUPERPUPERDEBUG
    stack->buffer_ = (Elem_t*)realloc(stack->buffer_ - 1, (STANDARD_SIZE_STACK + 2) * sizeof(Elem_t));
    stack->capacity_ = STANDARD_SIZE_STACK;
    SetTheCanaries(stack);
    stack->hash_ = HashFunc(stack);
    #else
    stack->buffer_ = (Elem_t*)realloc(stack->buffer_, STANDARD_SIZE_STACK * sizeof(Elem_t));
    stack->capacity_ = STANDARD_SIZE_STACK;
    #endif
    ASSERT_OK(stack);
}

//-------------------------------------------------------------
//!  Destroying the stack
//!
//!  @param [in] stack    pointer to a stack struct
//!
//!  @note destroy everything
//-------------------------------------------------------------

void Destroy(Stack* stack)
{
    ASSERT_OK(stack);
    #ifdef MYSUPERPUPERDEBUG
    free(stack->buffer_ - 1);
    stack->hash_ = 0;
    #else
    free(stack->buffer_ - 1);
    #endif
    stack->size_ = 0;
    stack->capacity_ = 0;
    stack->buffer_ = nullptr;
}

//-------------------------------------------------------------
//!  Checks, do buffer need to reallocate
//!
//!  @param [in] stack    pointer to a stack struct
//!
//!  @note calls reallocateUp(size == capacity) or reallocateDown(size <= capacity / 4
//-------------------------------------------------------------

void CheckBuffer(Stack* stack)
{
    ASSERT_OK(stack);
    if (stack->size_ >= stack->capacity_ - 1)
    {
        reallocateUp(stack, stack->capacity_ * sizeof(Elem_t) * 2);
    }
    if ((stack->size_ <= stack->capacity_ / 4) and (stack->size_ > 100))
    {

        reallocateDown(stack, (stack->capacity_ / 2) * sizeof(Elem_t));
    }
    ASSERT_OK(stack);
}

//-------------------------------------------------------------
//!  Reallocates buffer up
//!
//!  @param [in] stack          pointer to a stack struct
//!  @param [in] size_in_bytes  size in bytes how many memory do you need
//!
//!  @note assert if we don't have that much memory
//-------------------------------------------------------------

void reallocateUp(Stack* stack, size_t size_in_bytes)
{
    #ifdef MYSUPERPUPERDEBUG
    Elem_t* tmp = (Elem_t*)realloc(stack->buffer_ - 1, size_in_bytes + (2 * sizeof(Elem_t)));
    assert(tmp);
    stack->capacity_ = size_in_bytes / sizeof(Elem_t);
    stack->buffer_ = tmp;
    SetTheCanaries(stack);
    for (size_t i = stack->capacity_ - 1; i >= stack->capacity_ / 2; --i)
    {
        stack->buffer_[i] = NAN;
    }
    stack->hash_ = HashFunc(stack);
    #else
    Elem_t* tmp = (Elem_t*)realloc(stack->buffer_, size_in_bytes);
    assert(tmp);
    stack->capacity_ = size_in_bytes / sizeof(Elem_t);
    stack->buffer_ = tmp;
    #endif
}

//-------------------------------------------------------------
//!  Reallocates buffer down
//!
//!  @param [in] stack          pointer to a stack struct
//!  @param [in] size_in_bytes  size in bytes how many memory do you need
//!
//-------------------------------------------------------------

void reallocateDown(Stack* stack, size_t size_in_bytes)
{
    #ifdef MYSUPERPUPERDEBUG
    stack->buffer_ = (Elem_t*)realloc(stack->buffer_ - 1, size_in_bytes + (2 * sizeof(Elem_t)));
    stack->capacity_ = size_in_bytes / sizeof(Elem_t);
    SetTheCanaries(stack);
    stack->hash_ = HashFunc(stack);
    #else
    stack->buffer_ = (Elem_t*)realloc(stack->buffer_, size_in_bytes * sizeof(Elem_t));
    stack->capacity_ = size_in_bytes / sizeof(Elem_t);
    #endif
}


#ifdef MYSUPERPUPERDEBUG
//-------------------------------------------------------------
//!  Calculating hash = elems * theirs indexes + size/capacity
//!
//!  @param [in] stack   pointer to a stack struct
//!
//!  @return hash
//-------------------------------------------------------------

int HashFunc(Stack* stack)
{
    double hash = 2.2 * (double)stack->size_ - 1.1 * (double)stack->capacity_;
    for (size_t i = 0; i < stack->size_; i++)
    {
        hash += (Elem_t)(i + 1) * stack->buffer_[i];
    }
    return int(hash);
}

//-------------------------------------------------------------
//!  Setting the canaries to the buffer
//!
//!  @param [in] stack   pointer to a stack struct
//!
//!  @note set canaries on first and last place
//!  @return buffer + 1
//-------------------------------------------------------------
void SetTheCanaries(Stack* stack)
{
    stack->buffer_[0] = CANARY;
    stack->buffer_[stack->capacity_ + 1] = CANARY;
    stack->buffer_++;
}

//-------------------------------------------------------------
//!  Checks stack if it was spoiled
//!
//!  @param [in] stack   pointer to a stack struct
//!
//!  @note checks size, capacity, pointers, canaries, hash, and poison
//!  @return STACK_STATUS : ok or not ok
//-------------------------------------------------------------

STACK_STATUS StackOK(Stack* stack)
{
    if (stack == 0)
    {
        return STACK_IS_NOT_OK;
    }
    if (stack->status_ != NO_ERR)
    {
        return STACK_IS_NOT_OK;
    }
    if (stack->size_ > stack->capacity_)
    {
        stack->status_ = SIZE_ERR;
        return STACK_IS_NOT_OK;
    }
    if (stack->buffer_ == 0)
    {
        stack->status_ = BUFFER_POINTER_ERR;
        return STACK_IS_NOT_OK;
    }
    if (stack->size_ > MAX_SIZE_STACK)
    {
        stack->status_ = SIZE_ERR;
        return STACK_IS_NOT_OK;
    }
    if (stack->capacity_ > MAX_SIZE_STACK)
    {
        stack->status_ = CAPACITY_ERR;
        return STACK_IS_NOT_OK;
    }
    if ((long long int)stack->buffer_[-1] != CANARY || (long long int)stack->buffer_[stack->capacity_] != CANARY)
    {
        stack->status_ = BUF_CANARIES_ERR;
        return STACK_IS_NOT_OK;
    }
    if (stack->hash_ != HashFunc(stack))
    {
        stack->status_ = HASH_ERR;
        return STACK_IS_NOT_OK;
    }
    if ((long long int)stack->left_canary_ != CANARY || (long long int)stack->right_canary_ != CANARY)
    {
        stack->status_ = STR_CANARIES_ERR;
        return STACK_IS_NOT_OK;
    }

    for (size_t i = 0; i < stack->capacity_; ++i)
    {
        if (i >= stack->size_)
        {
            if (!isnan(stack->buffer_[i]))
            {
                stack->status_ = BUFFER_ERR;
                return STACK_IS_NOT_OK;
            }
        }
        else
        {
            if (isnan(stack->buffer_[i]))
            {
                stack->status_ = BUFFER_ERR;
                return STACK_IS_NOT_OK;
            }
        }

    }
    return STACK_IS_OK;
}

//-------------------------------------------------------------
//!  Prints all information about stack
//!
//!  @param [in] stack   pointer to a stack struct
//!
//!  @note prints name, size, capacity, pointers, status and elements
//-------------------------------------------------------------

void StackDump(Stack* stack)
{
    printf("Stack [%p] \n", stack);
    printf("Name : <%s>\n"
           "Stack status : ", stack->name_stack_);

    if (stack->status_ == NO_ERR            ) printf(" Stack is OK\n");
    if (stack->status_ == SIZE_ERR          ) printf(" Stack size is not OK\n");
    if (stack->status_ == CAPACITY_ERR      ) printf(" Stack capacity is not OK\n");
    if (stack->status_ == POP_EMPTY         ) printf(" You try to pop from empty stack\n");
    if (stack->status_ == TOP_EMPTY         ) printf(" You try to top from empty stack\n");
    if (stack->status_ == STACK_POINTER_ERR ) printf(" Stack pointer is nullptr\n");
    if (stack->status_ == BUFFER_POINTER_ERR) printf(" Buffer pointer is nullptr\n");
    if (stack->status_ == BUFFER_ERR        ) printf(" Buffer was spoiled\n");
    if (stack->status_ == STR_CANARIES_ERR  ) printf(" Canary from the struct is dead(((\n");
    if (stack->status_ == BUF_CANARIES_ERR  ) printf(" Canary from the buffer is dead(((\n");
    if (stack->status_ == HASH_ERR          ) printf(" Hash broken (check buffer/size/capacity)\n");

    printf("    size     = %u\n"
           "    capacity = %u\n", stack->size_, stack->capacity_);
    printf("     Buffer[%p]:\n", stack->buffer_);
    for (size_t i = 0; i < stack->capacity_; ++i)
    {
        if (i <  stack->size_)
            printf("        *[%2u] = %5lg\n", i, stack->buffer_[i]);
        else
            printf("         [%2u] = %5lg\n", i, stack->buffer_[i]);
    }

}
#endif
