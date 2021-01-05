#define TOP top(&cpu->stack); POP;
#define POP pop(&cpu->stack)

#define RETURN cpu->ofs = cpu->max_ofs; return

#define CHECKSIZE if (GetStkSize(&cpu->stack) == 0) { printf("Top from empty stack\n");\
													  printf("ofs : %d\n", cpu->ofs);  \
													  RETURN; }

#define ARG *(double*)(cpu->work_code + cpu->ofs + 1)

#define BYTE_ARG cpu->work_code[cpu->ofs++] 

#define CHECK_MODE_ON_RGS_AND_DOUBLE if (mode & IS_REGISTER)                            \
									{ 													\
										arg += cpu->RGS[BYTE_ARG - 1];					\
									}													\
																						\
									if (mode & IS_DOUBLE)								\
									{													\
										arg += *(double*)(cpu->work_code + cpu->ofs);	\
										cpu->ofs += sizeof(double);						\
									}

#define TAKE_ARGS 	CHECKSIZE;			\
					double elem1 = TOP;	\
					CHECKSIZE;			\
					double elem2 = TOP
	


DEF_CMD(push, 77, 1, 
{
	cpu->ofs++;
	double arg = 0;
	int mode = BYTE_ARG;
	
	CHECK_MODE_ON_RGS_AND_DOUBLE;

	if (mode & IS_RAM) arg = cpu->RAM[(int)arg];

	push(&cpu->stack, arg);
	cpu->ofs--;
})

DEF_CMD(pop, 99, 1,
{
	cpu->ofs++;
	double arg = 0;
	int mode = BYTE_ARG;
	
	if (mode & IS_RAM) 
	{
		CHECK_MODE_ON_RGS_AND_DOUBLE;

		CHECKSIZE;
		cpu->RAM[int(round(arg))] = TOP;
	}
	else
	{
		CHECKSIZE;
		cpu->RGS[BYTE_ARG - 1] = TOP;
	}
	cpu->ofs--;
})

DEF_CMD(in, 0, 0,
{
	double elem = 0;
	scanf("%lf", &elem);
    push(&cpu->stack, elem);
})

DEF_CMD(sum, 1, 0, 
{
	TAKE_ARGS;
	push(&cpu->stack, elem1 + elem2);
})

DEF_CMD(dec, 2, 0, 
{
	TAKE_ARGS;
	push(&cpu->stack, elem2 - elem1);
})

DEF_CMD(mul, 3, 0, 
{
	TAKE_ARGS;
	push(&cpu->stack, elem1 * elem2);
})

DEF_CMD(div, 4, 0, 
{
	TAKE_ARGS;
	if (elem1 == 0) 
    {
    	printf("Division by zero");
    	RETURN;
    }
    push(&cpu->stack, elem2 / elem1);
})

DEF_CMD(sin, 5, 0, 
{
	CHECKSIZE;
	double elem = TOP;
	push(&cpu->stack, sin(elem));
})

DEF_CMD(cos, 6, 0, 
{
	CHECKSIZE;
	double elem = TOP;
	push(&cpu->stack, cos(elem));})

DEF_CMD(sqrt, 7, 0, 
{
	double elem = TOP;
    if (elem < 0)
    {
    	printf("trying to sqrt from elem < 0\nofs = %d\n", cpu->ofs);
    	RETURN;
	}
    push(&cpu->stack, sqrt(elem));
})

DEF_CMD(stop, 8, 0, 
{
	DestroyCPU(cpu);
	//printf("Successfull ending\n");
	cpu->ofs = cpu->max_ofs;
})

DEF_CMD(out, 9, 0, 
{
	CHECKSIZE;
	printf("<<%lf>>\n", top(&cpu->stack));
	POP;
})

DEF_CMD(jmp, 10, -1, 
{
	cpu->ofs = ARG;
	cpu->ofs--;
})

DEF_CMD(ja, 11, -1, 
{
	TAKE_ARGS;
	if (elem2 > elem1)
	{
		cpu->ofs = ARG;
		cpu->ofs--;
	}
	else
	{
		cpu->ofs += sizeof(double);
	}
})

DEF_CMD(jae, 12, -1, 
{
	TAKE_ARGS;
	if (elem2 >= elem1)
	{
		cpu->ofs = ARG;
		cpu->ofs--;
	}
	else
	{
		cpu->ofs += sizeof(double);
	}
})

DEF_CMD(jb, 13, -1, 
{
	TAKE_ARGS;
	if (elem2 < elem1)
	{
		cpu->ofs = ARG;
		cpu->ofs--;
	}
	else
	{
		cpu->ofs += sizeof(double);
	}
})

DEF_CMD(jbe, 14, -1, 
{
	TAKE_ARGS;
	if (elem2 <= elem1)
	{
		cpu->ofs = ARG;
		cpu->ofs--;
	}
	else
	{
		cpu->ofs += sizeof(double);
	}
})

DEF_CMD(je, 15, -1, 
{
	TAKE_ARGS;
	if (elem2 == elem1)
	{
		cpu->ofs = ARG;
		cpu->ofs--;
	}
	else
	{
		cpu->ofs += sizeof(double);
	}})

DEF_CMD(jne, 16, -1, 
{
	TAKE_ARGS;
	if (elem2 != elem1)
	{
		cpu->ofs = ARG;
		cpu->ofs--;
	}
	else
	{
		cpu->ofs += sizeof(double);
	}
})

DEF_CMD(call, 17, -1,
{
	push(&cpu->call, cpu->ofs + sizeof(double));
	cpu->ofs = ARG;
	cpu->ofs--;
})

DEF_CMD(ret, 18, 0,
{
	cpu->ofs = top(&cpu->call);
	pop(&cpu->call);
})

DEF_CMD(txwindow, 19, 0, 
{
	txCreateWindow(500, 500, true);
})

DEF_CMD(refresh, 20, 0,
{
	for (int y = 0; y < 500; ++y)
		for (int x = 0; x < 500; ++x)
		{
			if (cpu->RAM[y * 500 + x])
			{
				txRectangle(x, y, (x + 2), (y + 2));
			}
		}
})

DEF_CMD(round, 21, 0, 
{
	double elem = TOP;
	push(&cpu->stack, round(elem));
})

DEF_CMD(random, 22, 0,
{
	push(&cpu->stack, rand() % 300 - 150);
})

DEF_CMD(changecolor, 23, 0,
{
	int elem = rand() % 5;
	switch (elem)
	{
	case 0 :
		txSetColor(TX_YELLOW);
		break;
	case 1 : 
		txSetColor(TX_GREEN);
		break;
	case 2 : 
		txSetColor(TX_CYAN);
		break;
	case 3 : 
		txSetColor(TX_MAGENTA);
		break;
	case 4 : 
		txSetColor(TX_ORANGE);
		break;
	}
})

DEF_CMD(clear, 24, 0, 
{
	
	for (int y = 0; y < 500; ++y)
	{
		for (int x = 0; x < 500; ++x)
		{
			cpu->RAM[y * 500 + x] = 0;
		}
	}	
})

DEF_CMD(sleep, 25, 0, 
{
	txSleep(10);
})

DEF_CMD(blue, 26, 0, 
{
	txSetColor(TX_CYAN);
})

DEF_CMD(black, 27, 0, 
{
	txSetColor(TX_BLACK);
})
