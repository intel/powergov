all: rapl_lib_shared rapl_lib_static power_gov_static

rapl_lib_shared: 
	gcc -fpic -c -g msr.c cpuid.c rapl.c 
	gcc -shared -o librapl.so msr.o cpuid.o rapl.o

rapl_lib_static: 
	gcc -c -g msr.c cpuid.c rapl.c 
	ar rcs librapl.a msr.o cpuid.o rapl.o

power_gov_static: 
	gcc power_gov.c -I. -L. -lm -o power_gov ./librapl.a

power_gov: 
	gcc power_gov.c -I. -L. -lm -lrapl -o power_gov 

clean: 
	rm -f power_gov librapl.so librapl.a msr.o cpuid.o rapl.o 
