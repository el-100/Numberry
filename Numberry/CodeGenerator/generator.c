#include <Windows.h>
#include <stdio.h>


long readfile(char *filename, char *buffer);
long writefile(char *filename, char *buffer);


int main($0)
{
	void *PEB = NULL,
		*Ldr = NULL,
		*Flink = NULL,
		*p = NULL,
		*BaseAddress = NULL,
		*BaseKernel32Address = NULL,
		*FullDllName = NULL;

	// pointer to PEB struct
	__asm
	{
		mov eax, fs:[0x30]
		mov PEB, eax
	}

	// getting PEB address in stack
	printf("[!] PEB: %i\n", PEB);
	int peb_offset;
	for (peb_offset = 0; peb_offset < 50; peb_offset++)
	{
		void *q = *(&$0 + peb_offset); // from stack
		if (q == PEB)
		{
			printf("[!] PEB = *(&$0 + %i)\n", peb_offset);
			break;
		}
	}

	// getting base address of kernel32.dll
	Ldr = *((void **)((unsigned char *)PEB + 0x0c));
	Flink = *((void **)((unsigned char *)Ldr + 0x1c));
	p = Flink;
	do
	{
		BaseAddress = *((void **)((unsigned char *)p + 0x08));
		FullDllName = *((void **)((unsigned char *)p + 0x18));
		//wprintf(L"FullDllName is %s\n", FullDllName);
		if (wcscmp(L"C:\\Windows\\SYSTEM32\\KERNEL32.DLL", FullDllName) == 0)
		{
			printf("[!] BaseAddress of kernel32.dll is %x\n", BaseAddress);
			BaseKernel32Address = BaseAddress;
			break;
		}
		p = *((void **)p);
	} while (Flink != p);

	// get addresses some functions
	int pVirtualProtect = (int)VirtualProtect - (int)BaseKernel32Address;
	int pLoadLibraryA = (int)LoadLibraryA - (int)BaseKernel32Address;
	int pExitProcess = (int)ExitProcess - (int)BaseKernel32Address;

	printf("[!] VirtualProtect: %i\n", pVirtualProtect);
	printf("[!] LoadLibraryA: %i\n", pLoadLibraryA);
	printf("[!] ExitProcess: %i\n", pExitProcess);

	printf("\n");

	// load library with printf and scanf. Get them addresses
	int lib = LoadLibraryA("c:\\windows\\system32\\msvcr120.dll");
	int pprintf = (int)GetProcAddress(lib, "printf") - lib;
	int pscanf = (int)GetProcAddress(lib, "scanf") - lib;


	printf("[!] printf: %i\n", pprintf);
	printf("[!] scanf: %i\n", pscanf);

	// read codemasks
	char* codemask_commented;
	char* codemask_obfuscated;
	readfile("codemask_commented.txt", &codemask_commented);
	readfile("codemask_obfuscated.txt", &codemask_obfuscated);

	// compose obfuscated code
	char *code_obfuscated;
	int len;
	len = snprintf(NULL, 0, codemask_obfuscated, peb_offset, pVirtualProtect, pLoadLibraryA, pExitProcess, pprintf, pscanf);
	printf("[!] Obfuscated code has length %d\n", len);
	if (!(code_obfuscated = malloc((len + 1) * sizeof(char))))
		return EXIT_FAILURE;
	len = snprintf(code_obfuscated, len + 1, codemask_obfuscated, peb_offset, pVirtualProtect, pLoadLibraryA, pExitProcess, pprintf, pscanf);

	// compose commented code
	char *code_commented;
	len = snprintf(NULL, 0, codemask_commented, peb_offset, pVirtualProtect, pLoadLibraryA, pExitProcess, pprintf, pscanf);
	if (!(code_commented = malloc((len + 1) * sizeof(char))))
		return EXIT_FAILURE;
	len = snprintf(code_commented, len + 1, codemask_commented, peb_offset, pVirtualProtect, pLoadLibraryA, pExitProcess, pprintf, pscanf);

	// printing obfuscated code
	printf("\n\n[!] Obfuscated code:\n%s\n", code_obfuscated);

	// save codes to projects
	writefile("..\\NumberryCommented\\master_peb.c", code_commented);
	writefile("..\\Numberry\\obfuscate.c", code_obfuscated);

	printf("\n\n[!] Commented and obfuscated codes saved to suitable projects\n");
	printf("Wait key..\n");
	getchar();
	return 0;
}




long readfile(char *filename, char **buffer)
{
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	*buffer = malloc(fsize + 1);
	fread(*buffer, fsize, 1, f);
	fclose(f);

	(*buffer)[fsize] = 0;
	return fsize;
}

long writefile(char *filename, char *buffer)
{
	FILE *f = fopen(filename, "wb");
	int len = fprintf(f, buffer);
	fclose(f);
	return len;
}
