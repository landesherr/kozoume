#pragma once

#ifdef DEBUG_MODE
	#define dbgwrite(...) printf(__VA_ARGS__)
#else
	#define dbgwrite(...) while(false){}
#endif

typedef unsigned short word;
typedef unsigned char byte;
