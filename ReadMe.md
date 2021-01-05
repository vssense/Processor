This is my processor and assembler
----------------------------------

									(c) Sergey Zelenkin

You can use registers (number of register you can change in enums.h, max = 26)

Theirs names will be : rax, rbx, rcx, rdx ...


You can use RAM

>	to push element from stack to RAM use command "pop [x]" where x can be:
>
>	- double 		: "pop [2]"
>	- rax    		: "pop [rax]"
>	- rax + double  : "pop [rax + 2]"
	
>	note : only with space between pop and '[' !!! \
>	note : only [rax + double], not [double + rax]

You can use labels
>	For this print label name on the other string, put space and ':' \
>	Then you can jump to this string with commands : jmp, ja, jae, jb, jbe, je, jne

You can use functions
>	- write function as a label
>	- in main part write call "func name" 
>	- at the end of the finc write ret!


## Commands 

- in :\
	scanning double and pushing it to the stack

- out :\
	printing top element of the stack and popping it

- stop :\
	you have to finish your programm with this commands

- push : 
    -	push with **[ ]**   - pushing element from RAM to stack
    -	push **register** - pushing register to stack
    -	push **double**   - pushing double to stack

- pop :
	- pop with **[ ]**     - pushing element from stack to RAM 
	- pop **regiser**   - pushing element from stack to register 

- sum, dec, mul, div \
	double x = top(stack)\
	pop\
	double y = top(stack)\
	pop\
	push(y "opeation" x)

- sin, cos, sqrt\
	double x = top(stack)\
	pop\
	push("opeation" x)

- jmp "name of label" \
	jumps to label

- ja, jae, jb, jbe, je, jne "**name of label**" \
	 **before this you HAVE TO push two elements to the stack** \
     (they will be popped and compared)
	
	jump if :
    - ja    : elem2 >  elem1
    - jae   : elem2 >= elem1
    - jb    : elem2 <  elem1
	- jbe   : elem2 <= elem1
	- je    : elem2 == elem1
	- jne   : elem2 != elem1 \
	P.S. :\
    elem1 = top\
	pop\
    elem2 = top\
    pop
- call, ret\
	calls function and returns from it


##### EASY EXAMPLE :
Program askes you to put a double and prints (double + 1) until you won't put 7
```
START : 
	in 
	pop rax
	push rax
	push 1
	sum
	out

	push rax
	push 7

	jne START :
	stop
```

##### EXAMPLE WITH SENSE  : 
program calcucaling roots of square equation\
(if D < 0 returns 0, if a = b = c = 0 returns infinity (if not rotate your screen on 90 degrees)
```
in
pop rax
in
pop rbx
in
pop rcx

push rax
push 0
jne CONTINUE
push rbx
push 0
jne CONTINUE
push rcx
push 0
jne CONTINUE
push 8
out
jmp STOP

CONTINUE :
push rax
push 0
je YES1
jmp DISCR

NO_ROOTS :
push 0
out
jmp STOP

YES1 :
	call LINEAR
	jmp STOP

LINEAR : 
	push rcx
	push -1
	mul
	push rbx
	div
	out
	ret

DISCR :
	push rbx
	push rbx
	mul
	push -4
	push rax
	push rcx
	mul
	mul
	sum
	pop rdx

	push rdx
	push 0
	jae YES2
	jmp NO_ROOTS

	YES2 :
	call ROOTS
	jmp STOP


	ROOTS :
		push rdx
		sqrt
		pop rdx
		push -1
		push rbx
		mul
		push rdx
		sum
		push 2
		push rax
		mul
		div
		out
		push -1
		push rbx
		mul
		push rdx
		dec
		push 2
		push rax
		mul
		div
		out
		ret
STOP :
	stop
```