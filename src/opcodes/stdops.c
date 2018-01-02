/*
	stdops.c
	
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

//All non-prefixed opcodes and their respective implementation functions
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
	rr_reg8(A);
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
	ld_nn_n(L);
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
	dec_reg8(A);
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
	and_hl();
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
	prefix_cb();
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
	ret_cc(NOTCARRY);
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
	ld_a_nnmem();
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

