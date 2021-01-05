#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_SIZE = 100000;
const int MAX_COMMAND_SIZE = 20;
const int VERS_SIZE = 6;
const int NUM_OF_REGISTERS = 26;
const int MAX_LABEL_NAME = 20;
const int DYNAMIC_ARRAY_STANDART_SIZE = 40;
const int MILLION_OF_TERABYTES = int(1e7);

enum MODE 
{
    STACK_DOUBLE   = 1,
    STACK_REGISTER = 2,
    STACK_BOTH     = 3,
    RAM_DOUBLE     = 5,
    RAM_REGISTER   = 6,
    RAM_BOTH       = 7
};

enum ARG_MODE
{
    IS_DOUBLE   = 1,
    IS_REGISTER = 2,
    IS_RAM      = 4
};

#define DEF_CMD(name, num, arg, code) \
        CMD_##name = num,

enum commands
{
    #include "commands.h"
};

#undef DEF_CMD