/*
	opcodes.c

	Copyright 2017 Nolan Varani

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

#include "opcodes.h"
#include "globaldefs.h"
#include "z80.h"
#include <stdlib.h>
#include <stdio.h>

opcode_func *standard_opcodes;
opcode_func *prefix_opcodes;

/*
	All those opcodes!

	`for i in {0..255}; do hex=`printf '%.2x' $i`; echo -e "void opcode_${hex}()\n{\n}\n"; done`
	`for i in {0..255}; do hex=`printf '%.2x' $i`; echo -e "void opcb_${hex}()\n{\n}\n"; done`
*/
//NORMAL
#include "opcodes/stdops.c"

//PREFIX
#include "opcodes/prefixops.c"

/*
	Other functions
*/
void allocate_bytecode_tables()
{
	standard_opcodes = calloc(TABLE_SIZE, sizeof(opcode_func));
	GET_FN(STD_OP_PREFIX);
	prefix_opcodes = calloc(TABLE_SIZE, sizeof(opcode_func));
	GET_CB(CB_OP_PREFIX);
}
void free_bytecode_tables()
{
	free(standard_opcodes);
	free(prefix_opcodes);
}
