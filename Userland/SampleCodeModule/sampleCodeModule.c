/* sampleCodeModule.c */

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;
extern void sysWriteTest(char * pointer);
extern void sysReadTest(char * pointer);
int main() {
	//All the following code may be removed 
	*v = 'X';
	*(v+1) = 0x74;
	char c = 'H';
	while (1)
	{		
		
	}
	


	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;
	return 0xDEADBEEF;
}