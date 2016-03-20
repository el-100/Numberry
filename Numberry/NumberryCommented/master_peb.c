// need correct get proc addresses
// need long arithmetics

__4 = 0;	 // writed letters
*_4 = &__4;	 // 

// Text "Fibonacci "
_2_1[] = { 1868720454, 1667457390, 8297 }; // part 1
// msvcr120.dll
_2_2[] = { 0x775c3a63, 0x6f646e69, 0x735c7377, 0x65747379, 0x5c32336d, 0x6376736d, 0x30323172, 0x6c6c642e, 0 };
// printf mask "%s%i->%i%c"
_2_3[] = { 0x69257325, 0x69253e2d, 0x00006325 };
// scanf mask "%i"
_2_4[] = { 0x00006925 };

// _asm push [esp]
_2_5_2[] = { 0xc32434ff };

// calculate count
_5 = 22;  // default

// iterator
_6 = 0;

_01 = 0; // fib tmp
_02 = 0; // fib last
_03 = 1; // fib curr

// handle to kernel32.dll
_7 = -1;
*_8 = 0;

// handle to ntdll.dll
_9 = -1;

// handle to PEB struct
_10 = -1;

// _stdcall
(*_2001)($1, $2, $3, $4); // VirtualProtect
(*_2002)($1); // LoadLibrary
(*_2003)($1); // ExitProcess

// msvcr120.dll handle
_7000;

// _cdecl
(*_3000)(); // printf
(*_3002)(); // scanf

// asm push [esp];
(*_00)();

main($0)
{
	// kernel32.dll
	//_7 = *(&$1 + 15) - 102815; // some hardcode ( only win 8)

	// ntdll.dll
	//_9 = *(&$1 + 18) - 303659; // it
	//_9 = *(&$1 + 35) - 303617; // or it

	// PEB struct
	_10 = *(&$0 + 7); // from stack

	// get kernel32.dll handle from PEB.ldr
	_8 = (_10 + 12);
	_8 = (*_8 + 20);
	_8 = (*_8);
	_8 = (*_8);
	_8 = (*_8 + 16);
	_7 = *_8;

	// func handles (!hardcode!)
	_2001 = _7 + 101040; // VirtualProtect
	_2002 = _7 + 102272; // LoadLibraryA
	_2003 = _7 + 170064; // ExitProcess

	// VirtualProtect stack normalizer code "_asm push [esp]"
	_2_5_1(0, 0, 0); // Variant 1
	_00 = _2_5_2;
	_2_5_10(1, 2, 3); // Variant 2

	_00(); // push [esp]

	// LoadLibrary("c:\\windows\\system32\\msvcr120.dll");
	_7000 = _2002(_2_2);

	// GetProcAddress
	_3000 = _7000 + 536537; // printf
	_3002 = _7000 + 537388; // scanf

	_3002(_2_4, &_5); // scanf

	_1000(); // recursive calculate fibonacci
}

// fibonacci calculate
_1000()
{
	_01 = _02 + _03;
	_02 = _03;
	_03 = _01;

	_6++;

	// printf
	_3000(_2_3, _2_1, _6, _03, 10);

	// next iteration or exit
	_6 < _5 ? _1000() : 0; // _2003(0);
}


// Virtual protect stack normalizer code "_asm push [esp]"
// Variant 1
_2_5_1($1, $2, $3)
{
	$1 == 0 ? _2_5_1(1, 0, 0) : _2001(_2_5_2, 1, 64, _4);
}

// Virtual protect stack normalizer code "_asm push [esp]"
// Variant 2
_2_5_10($1, $2, $3, $4)
{
	$4 = *(&$1 - 1); // current return point
	_2001(_2_5_2, 1, 64, _4);

	_00(); _00(); _00(); _00();
	*(&$1 - 1) = $4; // return return point
}
