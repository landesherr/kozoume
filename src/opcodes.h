/*
	opcodes.h

	Copyright 2018 Nolan Varani

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	        http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#pragma once

typedef void (*const opcode_func)(void);

#define TABLE_SIZE 0x100
#define STD_OP_PREFIX opcode
#define CB_OP_PREFIX opcb

extern opcode_func standard_opcodes[TABLE_SIZE];
extern opcode_func prefix_opcodes[TABLE_SIZE];

#define H0 0
#define H1 1
#define H2 2
#define H3 3
#define H4 4
#define H5 5
#define H6 6
#define H7 7
#define H8 8
#define H9 9
#define HA a
#define HB b
#define HC c
#define HD d
#define HE e
#define HF f

/*
	Godawful Macros
	Don't do this. I can get away with it because 
*/
//DECLARE THE FUNCTIONS
#define M5(F) void F (void);
//3. You can't explain that!
#define M4(P, I) M5(P ## _ ## I)
#define M3(P, I, J) M4(P, I ## J)
#define M2(P, I, J) M3(P, I, J)
//2. And again
#define M1(P, I) M2(P, I, H0) M2(P, I, H1) M2(P, I, H2) M2(P, I, H3) M2(P, I, H4) M2(P, I, H5) M2(P, I, H6) M2(P, I, H7) M2(P, I, H8) M2(P, I, H9) M2(P, I, HA) M2(P, I, HB) M2(P, I, HC) M2(P, I, HD) M2(P, I, HE) M2(P, I, HF)
//1. Divide and conquer. Split it all into the consituent hex digits
#define DEC_FN(P) M1(P, H0) M1(P, H1) M1(P, H2) M1(P, H3) M1(P, H4) M1(P, H5) M1(P, H6) M1(P, H7) M1(P, H8) M1(P, H9) M1(P, HA) M1(P, HB) M1(P, HC) M1(P, HD) M1(P, HE) M1(P, HF)


//ADD OPCODES TO TABLE
#define F5(I, F) [I] = &F
//3. You can't explain that!
#define F6(B) 0x ## B
#define F4(P, I) F5(F6(I), P ## _ ## I)
#define F3(P, I, J) F4(P, I ## J)
#define F2(P, I, J) F3(P, I, J)
//2. And again
#define F1(P, I) F2(P, I, H0), F2(P, I, H1), F2(P, I, H2), F2(P, I, H3), F2(P, I, H4), F2(P, I, H5), F2(P, I, H6), F2(P, I, H7), F2(P, I, H8), F2(P, I, H9), F2(P, I, HA), F2(P, I, HB), F2(P, I, HC), F2(P, I, HD), F2(P, I, HE), F2(P, I, HF)
//1. Divide and conquer. Split it all into the consituent hex digits
#define GET_FN(P) { F1(P, H0), F1(P, H1), F1(P, H2), F1(P, H3), F1(P, H4), F1(P, H5), F1(P, H6), F1(P, H7), F1(P, H8), F1(P, H9), F1(P, HA), F1(P, HB), F1(P, HC), F1(P, HD), F1(P, HE), F1(P, HF) }

//All function prototypes for opcodes
DEC_FN(STD_OP_PREFIX)
DEC_FN(CB_OP_PREFIX)
