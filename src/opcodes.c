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
void opcode_00()
{
	nop();
}

void opcode_01()
{
	ld_n_nn(BC);
}

void opcode_02()
{
	ld_mem_a(BC);
}

void opcode_03()
{
	inc_reg16(BC);
}

void opcode_04()
{
	inc_reg8(B);
}

void opcode_05()
{
	dec_reg8(B);
}

void opcode_06()
{
	ld_nn_n(B);
}

void opcode_07()
{
	rlc_reg8(A);
}

void opcode_08()
{
	ld_nn_sp();
}

void opcode_09()
{
	add_hl_reg16(BC);
}

void opcode_0a()
{
	ld_a_mem(BC);
}

void opcode_0b()
{
	dec_reg16(BC);
}

void opcode_0c()
{
	inc_reg8(C);
}

void opcode_0d()
{
	dec_reg8(C);
}

void opcode_0e()
{
	ld_nn_n(C);
}

void opcode_0f()
{
	rrc_reg8(A);
}

void opcode_10()
{
	stop();
}

void opcode_11()
{
	ld_n_nn(DE);
}

void opcode_12()
{
	ld_mem_a(DE);
}

void opcode_13()
{
	inc_reg16(DE);
}

void opcode_14()
{
	inc_reg8(D);
}

void opcode_15()
{
	dec_reg8(D);
}

void opcode_16()
{
	ld_nn_n(D);
}

void opcode_17()
{
	rl_reg8(A);
}

void opcode_18()
{
	jr();
}

void opcode_19()
{
	add_hl_reg16(DE);
}

void opcode_1a()
{
	ld_a_mem(DE);
}

void opcode_1b()
{
	dec_reg16(DE);
}

void opcode_1c()
{
	inc_reg8(E);
}

void opcode_1d()
{
	dec_reg8(E);
}

void opcode_1e()
{
	ld_nn_n(E);
}

void opcode_1f()
{
	rrc_reg8(A);
}

void opcode_20()
{
	jr_cc(NOTZERO);
}

void opcode_21()
{
	ld_n_nn(HL);
}

void opcode_22()
{
	ld_hli_a();
}

void opcode_23()
{
	inc_reg16(HL);
}

void opcode_24()
{
	inc_reg8(H);
}

void opcode_25()
{
	dec_reg8(H);
}

void opcode_26()
{
	ld_nn_n(H);
}

void opcode_27()
{
	daa();
}

void opcode_28()
{
	jr_cc(ZERO);
}

void opcode_29()
{
	add_hl_reg16(HL);
}

void opcode_2a()
{
	ld_a_hli();
}

void opcode_2b()
{
	dec_reg16(HL);
}

void opcode_2c()
{
	inc_reg8(L);
}

void opcode_2d()
{
	dec_reg8(L);
}

void opcode_2e()
{
	ld_nn_n(E);
}

void opcode_2f()
{
	cpl();
}

void opcode_30()
{
	jr_cc(NOTCARRY);
}

void opcode_31()
{
	ld_n_nn(SP);
}

void opcode_32()
{
	ld_hld_a();
}

void opcode_33()
{
	inc_reg16(SP);
}

void opcode_34()
{
	inc_hl();
}

void opcode_35()
{
	dec_hl();
}

void opcode_36()
{
	ld_mem_n();
}

void opcode_37()
{
	scf();
}

void opcode_38()
{
	jr_cc(CARRY);
}

void opcode_39()
{
	add_hl_reg16(SP);
}

void opcode_3a()
{
	ld_a_hld();
}

void opcode_3b()
{
	dec_reg16(SP);
}

void opcode_3c()
{
	inc_reg8(A);
}

void opcode_3d()
{
	dec_reg8(D);
}

void opcode_3e()
{
	ld_nn_n(A);
}

void opcode_3f()
{
	ccf();
}

void opcode_40()
{
	ld_reg8_reg8(B, B);
}

void opcode_41()
{
	ld_reg8_reg8(B, C);
}

void opcode_42()
{
	ld_reg8_reg8(B, D);
}

void opcode_43()
{
	ld_reg8_reg8(B, E);
}

void opcode_44()
{
	ld_reg8_reg8(B, H);
}

void opcode_45()
{
	ld_reg8_reg8(B, L);
}

void opcode_46()
{
	ld_reg8_mem(B);
}

void opcode_47()
{
	ld_reg8_reg8(B, A);
}

void opcode_48()
{
	ld_reg8_reg8(C, B);
}

void opcode_49()
{
	ld_reg8_reg8(C, C);
}

void opcode_4a()
{
	ld_reg8_reg8(C, D);
}

void opcode_4b()
{
	ld_reg8_reg8(C, E);
}

void opcode_4c()
{
	ld_reg8_reg8(C, H);
}

void opcode_4d()
{
	ld_reg8_reg8(C, L);
}

void opcode_4e()
{
	ld_reg8_mem(C);
}

void opcode_4f()
{
	ld_reg8_reg8(C, A);
}

void opcode_50()
{
	ld_reg8_reg8(D, B);
}

void opcode_51()
{
	ld_reg8_reg8(D, C);
}

void opcode_52()
{
	ld_reg8_reg8(D, D);
}

void opcode_53()
{
	ld_reg8_reg8(D, E);
}

void opcode_54()
{
	ld_reg8_reg8(D, H);
}

void opcode_55()
{
	ld_reg8_reg8(D, L);
}

void opcode_56()
{
	ld_reg8_mem(D);
}

void opcode_57()
{
	ld_reg8_reg8(D, A);
}

void opcode_58()
{
	ld_reg8_reg8(E, B);
}

void opcode_59()
{
	ld_reg8_reg8(E, C);
}

void opcode_5a()
{
	ld_reg8_reg8(E, D);
}

void opcode_5b()
{
	ld_reg8_reg8(E, E);
}

void opcode_5c()
{
	ld_reg8_reg8(E, H);
}

void opcode_5d()
{
	ld_reg8_reg8(E, L);
}

void opcode_5e()
{
	ld_reg8_mem(E);
}

void opcode_5f()
{
	ld_reg8_reg8(E, A);
}

void opcode_60()
{
	ld_reg8_reg8(H, B);
}

void opcode_61()
{
	ld_reg8_reg8(H, C);
}

void opcode_62()
{
	ld_reg8_reg8(H, D);
}

void opcode_63()
{
	ld_reg8_reg8(H, E);
}

void opcode_64()
{
	ld_reg8_reg8(H, H);
}

void opcode_65()
{
	ld_reg8_reg8(H, L);
}

void opcode_66()
{
	ld_reg8_mem(H);
}

void opcode_67()
{
	ld_reg8_reg8(H, A);
}

void opcode_68()
{
	ld_reg8_reg8(L, B);
}

void opcode_69()
{
	ld_reg8_reg8(L, C);
}

void opcode_6a()
{
	ld_reg8_reg8(L, D);
}

void opcode_6b()
{
	ld_reg8_reg8(L, E);
}

void opcode_6c()
{
	ld_reg8_reg8(L, H);
}

void opcode_6d()
{
	ld_reg8_reg8(L, L);
}

void opcode_6e()
{
	ld_reg8_mem(L);
}

void opcode_6f()
{
	ld_reg8_reg8(L, A);
}

void opcode_70()
{
	ld_mem_reg8(B);
}

void opcode_71()
{
	ld_mem_reg8(C);
}

void opcode_72()
{
	ld_mem_reg8(D);
}

void opcode_73()
{
	ld_mem_reg8(E);
}

void opcode_74()
{
	ld_mem_reg8(H);
}

void opcode_75()
{
	ld_mem_reg8(L);
}

void opcode_76()
{
	halt();
}

void opcode_77()
{
	ld_mem_reg8(A);
}

void opcode_78()
{
	ld_reg8_reg8(A, B);
}

void opcode_79()
{
	ld_reg8_reg8(A, C);
}

void opcode_7a()
{
	ld_reg8_reg8(A, D);
}

void opcode_7b()
{
	ld_reg8_reg8(A, E);
}

void opcode_7c()
{
	ld_reg8_reg8(A, H);
}

void opcode_7d()
{
	ld_reg8_reg8(A, L);
}

void opcode_7e()
{
	ld_reg8_mem(A);
}

void opcode_7f()
{
	ld_reg8_reg8(A, A);
}

void opcode_80()
{
	add_a_reg8(B);
}

void opcode_81()
{
	add_a_reg8(C);
}

void opcode_82()
{
	add_a_reg8(D);
}

void opcode_83()
{
	add_a_reg8(E);
}

void opcode_84()
{
	add_a_reg8(H);
}

void opcode_85()
{
	add_a_reg8(L);
}

void opcode_86()
{
	add_a_hl();
}

void opcode_87()
{
	add_a_reg8(A);
}

void opcode_88()
{
	adc_a_reg8(B);
}

void opcode_89()
{
	adc_a_reg8(C);
}

void opcode_8a()
{
	adc_a_reg8(D);
}

void opcode_8b()
{
	adc_a_reg8(E);
}

void opcode_8c()
{
	adc_a_reg8(H);
}

void opcode_8d()
{
	adc_a_reg8(L);
}

void opcode_8e()
{
	adc_a_hl();
}

void opcode_8f()
{
	adc_a_reg8(A);
}

void opcode_90()
{
	sub_a_reg8(B);
}

void opcode_91()
{
	sub_a_reg8(C);
}

void opcode_92()
{
	sub_a_reg8(D);
}

void opcode_93()
{
	sub_a_reg8(E);
}

void opcode_94()
{
	sub_a_reg8(H);
}

void opcode_95()
{
	sub_a_reg8(L);
}

void opcode_96()
{
	sub_a_hl();
}

void opcode_97()
{
	sub_a_reg8(A);
}

void opcode_98()
{
	sbc_a_reg8(B);
}

void opcode_99()
{
	sbc_a_reg8(C);
}

void opcode_9a()
{
	sbc_a_reg8(D);
}

void opcode_9b()
{
	sbc_a_reg8(E);
}

void opcode_9c()
{
	sbc_a_reg8(H);
}

void opcode_9d()
{
	sbc_a_reg8(L);
}

void opcode_9e()
{
	sbc_a_hl();
}

void opcode_9f()
{
	sbc_a_reg8(A);
}

void opcode_a0()
{
	and_reg8(B);
}

void opcode_a1()
{
	and_reg8(C);
}

void opcode_a2()
{
	and_reg8(D);
}

void opcode_a3()
{
	and_reg8(E);
}

void opcode_a4()
{
	and_reg8(H);
}

void opcode_a5()
{
	and_reg8(L);
}

void opcode_a6()
{
	and_hl
}

void opcode_a7()
{
	and_reg8(A);
}

void opcode_a8()
{
	xor_reg8(B);
}

void opcode_a9()
{
	xor_reg8(C);
}

void opcode_aa()
{
	xor_reg8(D);
}

void opcode_ab()
{
	xor_reg8(E);
}

void opcode_ac()
{
	xor_reg8(H);
}

void opcode_ad()
{
	xor_reg8(L);
}

void opcode_ae()
{
	xor_hl();
}

void opcode_af()
{
	xor_reg8(A);
}

void opcode_b0()
{
	or_reg8(B);
}

void opcode_b1()
{
	or_reg8(C);
}

void opcode_b2()
{
	or_reg8(D);
}

void opcode_b3()
{
	or_reg8(E);
}

void opcode_b4()
{
	or_reg8(H);
}

void opcode_b5()
{
	or_reg8(L);
}

void opcode_b6()
{
	or_hl();
}

void opcode_b7()
{
	or_reg8(A);
}

void opcode_b8()
{
	cp_reg8(B);
}

void opcode_b9()
{
	cp_reg8(C);
}

void opcode_ba()
{
	cp_reg8(D);
}

void opcode_bb()
{
	cp_reg8(E);
}

void opcode_bc()
{
	cp_reg8(H);
}

void opcode_bd()
{
	cp_reg8(L);
}

void opcode_be()
{
	cp_hl();
}

void opcode_bf()
{
	cp_reg8(A);
}

void opcode_c0()
{
	ret_cc(NOTZERO);
}

void opcode_c1()
{
	pop_nn(BC);
}

void opcode_c2()
{
	jp_cc(NOTZERO);
}

void opcode_c3()
{
	jp();
}

void opcode_c4()
{
	call_cc(NOTZERO);
}

void opcode_c5()
{
	push_nn(BC);
}

void opcode_c6()
{
	add_a_n();
}

void opcode_c7()
{
	rst(0x00);
}

void opcode_c8()
{
	ret_cc(ZERO);
}

void opcode_c9()
{
	ret();
}

void opcode_ca()
{
	jp_cc(ZERO);
}

void opcode_cb()
{
	//TODO Handle CB
}

void opcode_cc()
{
	call_cc(ZERO);
}

void opcode_cd()
{
	call();
}

void opcode_ce()
{
	adc_a_n();
}

void opcode_cf()
{
	rst(0x08);
}

void opcode_d0()
{
	ret(NOTCARRY);
}

void opcode_d1()
{
	pop_nn(DE);
}

void opcode_d2()
{
	jp_cc(NOTCARRY);
}

void opcode_d3()
{
	//undefined
}

void opcode_d4()
{
	call_cc(NOTCARRY);
}

void opcode_d5()
{
	push_nn(DE);
}

void opcode_d6()
{
	sub_a_n();
}

void opcode_d7()
{
	rst(0x10);
}

void opcode_d8()
{
	ret_cc(CARRY);
}

void opcode_d9()
{
	reti();
}

void opcode_da()
{
	jp_cc(CARRY);
}

void opcode_db()
{
	//undefined
}

void opcode_dc()
{
	call_cc(CARRY);
}

void opcode_dd()
{
	//undefined
}

void opcode_de()
{
	sbc_a_n();
}

void opcode_df()
{
	rst(0x18);
}

void opcode_e0()
{
	ldh_n_a();
}

void opcode_e1()
{
	pop_nn(HL);
}

void opcode_e2()
{
	ld_c_relative_a();
}

void opcode_e3()
{
	//undefined
}

void opcode_e4()
{
	//undefined
}

void opcode_e5()
{
	push_nn(HL);
}

void opcode_e6()
{
	and_n();
}

void opcode_e7()
{
	rst(0x20);
}

void opcode_e8()
{
	add_sp_n();
}

void opcode_e9()
{
	jp_hl();
}

void opcode_ea()
{
	ld_nnmem_a();
}

void opcode_eb()
{
	//undefined
}

void opcode_ec()
{
	//undefined
}

void opcode_ed()
{
	//undefined
}

void opcode_ee()
{
	xor_n();
}

void opcode_ef()
{
	rst(0x28);
}

void opcode_f0()
{
	ldh_a_n();
}

void opcode_f1()
{
	pop_nn(AF);
}

void opcode_f2()
{
	ld_a_c_relative();
}

void opcode_f3()
{
	di();
}

void opcode_f4()
{
	//undefined
}

void opcode_f5()
{
	push_nn(AF);
}

void opcode_f6()
{
	or_n();
}

void opcode_f7()
{
	rst(0x30);
}

void opcode_f8()
{
	ldhl_sp_n();
}

void opcode_f9()
{
	ld_sp_hl();
}

void opcode_fa()
{
	ld_a_n();
}

void opcode_fb()
{
	ei();
}

void opcode_fc()
{
	//undefined
}

void opcode_fd()
{
	//undefined
}

void opcode_fe()
{
	cp_n();
}

void opcode_ff()
{
	rst(0x38);
}

//PREFIX
void opcb_00()
{
}

void opcb_01()
{
}

void opcb_02()
{
}

void opcb_03()
{
}

void opcb_04()
{
}

void opcb_05()
{
}

void opcb_06()
{
}

void opcb_07()
{
}

void opcb_08()
{
}

void opcb_09()
{
}

void opcb_0a()
{
}

void opcb_0b()
{
}

void opcb_0c()
{
}

void opcb_0d()
{
}

void opcb_0e()
{
}

void opcb_0f()
{
}

void opcb_10()
{
}

void opcb_11()
{
}

void opcb_12()
{
}

void opcb_13()
{
}

void opcb_14()
{
}

void opcb_15()
{
}

void opcb_16()
{
}

void opcb_17()
{
}

void opcb_18()
{
}

void opcb_19()
{
}

void opcb_1a()
{
}

void opcb_1b()
{
}

void opcb_1c()
{
}

void opcb_1d()
{
}

void opcb_1e()
{
}

void opcb_1f()
{
}

void opcb_20()
{
}

void opcb_21()
{
}

void opcb_22()
{
}

void opcb_23()
{
}

void opcb_24()
{
}

void opcb_25()
{
}

void opcb_26()
{
}

void opcb_27()
{
}

void opcb_28()
{
}

void opcb_29()
{
}

void opcb_2a()
{
}

void opcb_2b()
{
}

void opcb_2c()
{
}

void opcb_2d()
{
}

void opcb_2e()
{
}

void opcb_2f()
{
}

void opcb_30()
{
}

void opcb_31()
{
}

void opcb_32()
{
}

void opcb_33()
{
}

void opcb_34()
{
}

void opcb_35()
{
}

void opcb_36()
{
}

void opcb_37()
{
}

void opcb_38()
{
}

void opcb_39()
{
}

void opcb_3a()
{
}

void opcb_3b()
{
}

void opcb_3c()
{
}

void opcb_3d()
{
}

void opcb_3e()
{
}

void opcb_3f()
{
}

void opcb_40()
{
}

void opcb_41()
{
}

void opcb_42()
{
}

void opcb_43()
{
}

void opcb_44()
{
}

void opcb_45()
{
}

void opcb_46()
{
}

void opcb_47()
{
}

void opcb_48()
{
}

void opcb_49()
{
}

void opcb_4a()
{
}

void opcb_4b()
{
}

void opcb_4c()
{
}

void opcb_4d()
{
}

void opcb_4e()
{
}

void opcb_4f()
{
}

void opcb_50()
{
}

void opcb_51()
{
}

void opcb_52()
{
}

void opcb_53()
{
}

void opcb_54()
{
}

void opcb_55()
{
}

void opcb_56()
{
}

void opcb_57()
{
}

void opcb_58()
{
}

void opcb_59()
{
}

void opcb_5a()
{
}

void opcb_5b()
{
}

void opcb_5c()
{
}

void opcb_5d()
{
}

void opcb_5e()
{
}

void opcb_5f()
{
}

void opcb_60()
{
}

void opcb_61()
{
}

void opcb_62()
{
}

void opcb_63()
{
}

void opcb_64()
{
}

void opcb_65()
{
}

void opcb_66()
{
}

void opcb_67()
{
}

void opcb_68()
{
}

void opcb_69()
{
}

void opcb_6a()
{
}

void opcb_6b()
{
}

void opcb_6c()
{
}

void opcb_6d()
{
}

void opcb_6e()
{
}

void opcb_6f()
{
}

void opcb_70()
{
}

void opcb_71()
{
}

void opcb_72()
{
}

void opcb_73()
{
}

void opcb_74()
{
}

void opcb_75()
{
}

void opcb_76()
{
}

void opcb_77()
{
}

void opcb_78()
{
}

void opcb_79()
{
}

void opcb_7a()
{
}

void opcb_7b()
{
}

void opcb_7c()
{
}

void opcb_7d()
{
}

void opcb_7e()
{
}

void opcb_7f()
{
}

void opcb_80()
{
}

void opcb_81()
{
}

void opcb_82()
{
}

void opcb_83()
{
}

void opcb_84()
{
}

void opcb_85()
{
}

void opcb_86()
{
}

void opcb_87()
{
}

void opcb_88()
{
}

void opcb_89()
{
}

void opcb_8a()
{
}

void opcb_8b()
{
}

void opcb_8c()
{
}

void opcb_8d()
{
}

void opcb_8e()
{
}

void opcb_8f()
{
}

void opcb_90()
{
}

void opcb_91()
{
}

void opcb_92()
{
}

void opcb_93()
{
}

void opcb_94()
{
}

void opcb_95()
{
}

void opcb_96()
{
}

void opcb_97()
{
}

void opcb_98()
{
}

void opcb_99()
{
}

void opcb_9a()
{
}

void opcb_9b()
{
}

void opcb_9c()
{
}

void opcb_9d()
{
}

void opcb_9e()
{
}

void opcb_9f()
{
}

void opcb_a0()
{
}

void opcb_a1()
{
}

void opcb_a2()
{
}

void opcb_a3()
{
}

void opcb_a4()
{
}

void opcb_a5()
{
}

void opcb_a6()
{
}

void opcb_a7()
{
}

void opcb_a8()
{
}

void opcb_a9()
{
}

void opcb_aa()
{
}

void opcb_ab()
{
}

void opcb_ac()
{
}

void opcb_ad()
{
}

void opcb_ae()
{
}

void opcb_af()
{
}

void opcb_b0()
{
}

void opcb_b1()
{
}

void opcb_b2()
{
}

void opcb_b3()
{
}

void opcb_b4()
{
}

void opcb_b5()
{
}

void opcb_b6()
{
}

void opcb_b7()
{
}

void opcb_b8()
{
}

void opcb_b9()
{
}

void opcb_ba()
{
}

void opcb_bb()
{
}

void opcb_bc()
{
}

void opcb_bd()
{
}

void opcb_be()
{
}

void opcb_bf()
{
}

void opcb_c0()
{
}

void opcb_c1()
{
}

void opcb_c2()
{
}

void opcb_c3()
{
}

void opcb_c4()
{
}

void opcb_c5()
{
}

void opcb_c6()
{
}

void opcb_c7()
{
}

void opcb_c8()
{
}

void opcb_c9()
{
}

void opcb_ca()
{
}

void opcb_cb()
{
}

void opcb_cc()
{
}

void opcb_cd()
{
}

void opcb_ce()
{
}

void opcb_cf()
{
}

void opcb_d0()
{
}

void opcb_d1()
{
}

void opcb_d2()
{
}

void opcb_d3()
{
}

void opcb_d4()
{
}

void opcb_d5()
{
}

void opcb_d6()
{
}

void opcb_d7()
{
}

void opcb_d8()
{
}

void opcb_d9()
{
}

void opcb_da()
{
}

void opcb_db()
{
}

void opcb_dc()
{
}

void opcb_dd()
{
}

void opcb_de()
{
}

void opcb_df()
{
}

void opcb_e0()
{
}

void opcb_e1()
{
}

void opcb_e2()
{
}

void opcb_e3()
{
}

void opcb_e4()
{
}

void opcb_e5()
{
}

void opcb_e6()
{
}

void opcb_e7()
{
}

void opcb_e8()
{
}

void opcb_e9()
{
}

void opcb_ea()
{
}

void opcb_eb()
{
}

void opcb_ec()
{
}

void opcb_ed()
{
}

void opcb_ee()
{
}

void opcb_ef()
{
}

void opcb_f0()
{
}

void opcb_f1()
{
}

void opcb_f2()
{
}

void opcb_f3()
{
}

void opcb_f4()
{
}

void opcb_f5()
{
}

void opcb_f6()
{
}

void opcb_f7()
{
}

void opcb_f8()
{
}

void opcb_f9()
{
}

void opcb_fa()
{
}

void opcb_fb()
{
}

void opcb_fc()
{
}

void opcb_fd()
{
}

void opcb_fe()
{
}

void opcb_ff()
{
}

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


