/*
	opcodes.c

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

#include "opcodes.h"
#include "globaldefs.h"
#include "z80.h"

/*
	All those opcodes!

	`for i in {0..255}; do hex=`printf '%.2x' $i`; echo -e "void opcode_${hex}()\n{\n}\n"; done`
	`for i in {0..255}; do hex=`printf '%.2x' $i`; echo -e "void opcb_${hex}()\n{\n}\n"; done`
*/
//NORMAL
#include "opcodes/stdops.c"

//PREFIX
#include "opcodes/prefixops.c"

opcode_func standard_opcodes[TABLE_SIZE] = GET_FN(STD_OP_PREFIX);
opcode_func prefix_opcodes[TABLE_SIZE] = GET_FN(CB_OP_PREFIX);

