options = -Wall -Wpedantic -Wextra

Input = input.txt
WorkFile = work_file.bin

src = src
bin = bin

run : $(bin)\processor.exe $(bin)\$(WorkFile) $(src)\commands.h 
	$(bin)\processor.exe

$(bin)\processor.exe : $(src)\processing.cpp $(bin)\super_puper_stack_struct.o $(src)\enums.h $(src)\super_puper_stack_struct.h $(src)\commands.h
	g++ $(src)\processing.cpp $(bin)\super_puper_stack_struct.o -o $(bin)\processor.exe $(options) -w

$(bin)\super_puper_stack_struct.o : $(src)\super_puper_stack_struct.cpp $(src)\super_puper_stack_struct.h
	g++ -c $(src)\super_puper_stack_struct.cpp -o $(bin)\super_puper_stack_struct.o $(options)

$(bin)\assembler.exe : $(src)\assembler.cpp $(src)\enums.h $(src)\commands.h
	g++ $(src)\assembler.cpp -o $(bin)\assembler.exe $(options)

$(bin)\$(WorkFile) : $(Input) $(bin)\assembler.exe $(src)\commands.h
	$(bin)\assembler.exe $(Input)