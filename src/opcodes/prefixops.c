/*
	prefixops.c

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

//All CB-prefixed opcodes
void opcb_00()
{
	rlc_reg8(B);
}

void opcb_01()
{
	rlc_reg8(C);
}

void opcb_02()
{
	rlc_reg8(D);
}

void opcb_03()
{
	rlc_reg8(E);
}

void opcb_04()
{
	rlc_reg8(H);
}

void opcb_05()
{
	rlc_reg8(L);
}

void opcb_06()
{
	rlc_hl();
}

void opcb_07()
{
	rlc_reg8(A);
}

void opcb_08()
{
	rrc_reg8(B);
}

void opcb_09()
{
	rrc_reg8(C);
}

void opcb_0a()
{
	rrc_reg8(D);
}

void opcb_0b()
{
	rrc_reg8(E);
}

void opcb_0c()
{
	rrc_reg8(H);
}

void opcb_0d()
{
	rrc_reg8(L);
}

void opcb_0e()
{
	rrc_hl();
}

void opcb_0f()
{
	rrc_reg8(A);
}

void opcb_10()
{
	rl_reg8(B);
}

void opcb_11()
{
	rl_reg8(C);
}

void opcb_12()
{
	rl_reg8(D);
}

void opcb_13()
{
	rl_reg8(E);
}

void opcb_14()
{
	rl_reg8(H);
}

void opcb_15()
{
	rl_reg8(L);
}

void opcb_16()
{
	rl_hl();
}

void opcb_17()
{
	rl_reg8(A);
}

void opcb_18()
{
	rr_reg8(B);
}

void opcb_19()
{
	rr_reg8(C);
}

void opcb_1a()
{
	rr_reg8(D);
}

void opcb_1b()
{
	rr_reg8(E);
}

void opcb_1c()
{
	rr_reg8(H);
}

void opcb_1d()
{
	rr_reg8(L);
}

void opcb_1e()
{
	rr_hl();
}

void opcb_1f()
{
	rr_reg8(A);
}

void opcb_20()
{
	sla_reg8(B);
}

void opcb_21()
{
	sla_reg8(C);
}

void opcb_22()
{
	sla_reg8(D);
}

void opcb_23()
{
	sla_reg8(E);
}

void opcb_24()
{
	sla_reg8(H);
}

void opcb_25()
{
	sla_reg8(L);
}

void opcb_26()
{
	sla_hl();
}

void opcb_27()
{
	sla_reg8(A);
}

void opcb_28()
{
	sra_reg8(B);
}

void opcb_29()
{
	sra_reg8(C);
}

void opcb_2a()
{
	sra_reg8(D);
}

void opcb_2b()
{
	sra_reg8(E);
}

void opcb_2c()
{
	sra_reg8(H);
}

void opcb_2d()
{
	sra_reg8(L);
}

void opcb_2e()
{
	sra_hl();
}

void opcb_2f()
{
	sra_reg8(A);
}

void opcb_30()
{
	swap_reg8(B);
}

void opcb_31()
{
	swap_reg8(C);
}

void opcb_32()
{
	swap_reg8(D);
}

void opcb_33()
{
	swap_reg8(E);
}

void opcb_34()
{
	swap_reg8(H);
}

void opcb_35()
{
	swap_reg8(L);
}

void opcb_36()
{
	swap_hl();
}

void opcb_37()
{
	swap_reg8(A);
}

void opcb_38()
{
	srl_reg8(B);
}

void opcb_39()
{
	srl_reg8(C);
}

void opcb_3a()
{
	srl_reg8(D);
}

void opcb_3b()
{
	srl_reg8(E);
}

void opcb_3c()
{
	srl_reg8(H);
}

void opcb_3d()
{
	srl_reg8(L);
}

void opcb_3e()
{
	srl_hl();
}

void opcb_3f()
{
	srl_reg8(A);
}

void opcb_40()
{
	bit_reg8(0, B);
}

void opcb_41()
{
	bit_reg8(0, C);
}

void opcb_42()
{
	bit_reg8(0, D);
}

void opcb_43()
{
	bit_reg8(0, E);
}

void opcb_44()
{
	bit_reg8(0, H);
}

void opcb_45()
{
	bit_reg8(0, L);
}

void opcb_46()
{
	bit_hl(0);
}

void opcb_47()
{
	bit_reg8(0, A);
}

void opcb_48()
{
	bit_reg8(1, B);
}

void opcb_49()
{
	bit_reg8(1, C);
}

void opcb_4a()
{
	bit_reg8(1, D);
}

void opcb_4b()
{
	bit_reg8(1, E);
}

void opcb_4c()
{
	bit_reg8(1, H);
}

void opcb_4d()
{
	bit_reg8(1, L);
}

void opcb_4e()
{
	bit_hl(1);
}

void opcb_4f()
{
	bit_reg8(1, A);
}

void opcb_50()
{
	bit_reg8(2, B);
}

void opcb_51()
{
	bit_reg8(2, C);
}

void opcb_52()
{
	bit_reg8(2, D);
}

void opcb_53()
{
	bit_reg8(2, E);
}

void opcb_54()
{
	bit_reg8(2, H);
}

void opcb_55()
{
	bit_reg8(2, L);
}

void opcb_56()
{
	bit_hl(2);
}

void opcb_57()
{
	bit_reg8(2, A);
}

void opcb_58()
{
	bit_reg8(3, B);
}

void opcb_59()
{
	bit_reg8(3, C);
}

void opcb_5a()
{
	bit_reg8(3, D);
}

void opcb_5b()
{
	bit_reg8(3, E);
}

void opcb_5c()
{
	bit_reg8(3, H);
}

void opcb_5d()
{
	bit_reg8(3, L);
}

void opcb_5e()
{
	bit_hl(3);
}

void opcb_5f()
{
	bit_reg8(3, A);
}

void opcb_60()
{
	bit_reg8(4, B);
}

void opcb_61()
{
	bit_reg8(4, C);
}

void opcb_62()
{
	bit_reg8(4, D);
}

void opcb_63()
{
	bit_reg8(4, E);
}

void opcb_64()
{
	bit_reg8(4, H);
}

void opcb_65()
{
	bit_reg8(4, L);
}

void opcb_66()
{
	bit_hl(4);
}

void opcb_67()
{
	bit_reg8(4, A);
}

void opcb_68()
{
	bit_reg8(5, B);
}

void opcb_69()
{
	bit_reg8(5, C);
}

void opcb_6a()
{
	bit_reg8(5, D);
}

void opcb_6b()
{
	bit_reg8(5, E);
}

void opcb_6c()
{
	bit_reg8(5, H);
}

void opcb_6d()
{
	bit_reg8(5, L);
}

void opcb_6e()
{
	bit_hl(5);
}

void opcb_6f()
{
	bit_reg8(5, A);
}

void opcb_70()
{
	bit_reg8(6, B);
}

void opcb_71()
{
	bit_reg8(6, C);
}

void opcb_72()
{
	bit_reg8(6, D);
}

void opcb_73()
{
	bit_reg8(6, E);
}

void opcb_74()
{
	bit_reg8(6, H);
}

void opcb_75()
{
	bit_reg8(6, L);
}

void opcb_76()
{
	bit_hl(6);
}

void opcb_77()
{
	bit_reg8(6, A);
}

void opcb_78()
{
	bit_reg8(7, B);
}

void opcb_79()
{
	bit_reg8(7, C);
}

void opcb_7a()
{
	bit_reg8(7, D);
}

void opcb_7b()
{
	bit_reg8(7, E);
}

void opcb_7c()
{
	bit_reg8(7, H);
}

void opcb_7d()
{
	bit_reg8(7, L);
}

void opcb_7e()
{
	bit_hl(7);
}

void opcb_7f()
{
	bit_reg8(7, A);
}

void opcb_80()
{
	res_reg8(0, B);
}

void opcb_81()
{
	res_reg8(0, C);
}

void opcb_82()
{
	res_reg8(0, D);
}

void opcb_83()
{
	res_reg8(0, E);
}

void opcb_84()
{
	res_reg8(0, H);
}

void opcb_85()
{
	res_reg8(0, L);
}

void opcb_86()
{
	res_hl(0);
}

void opcb_87()
{
	res_reg8(0, A);
}

void opcb_88()
{
	res_reg8(1, B);
}

void opcb_89()
{
	res_reg8(1, C);
}

void opcb_8a()
{
	res_reg8(1, D);
}

void opcb_8b()
{
	res_reg8(1, E);
}

void opcb_8c()
{
	res_reg8(1, H);
}

void opcb_8d()
{
	res_reg8(1, L);
}

void opcb_8e()
{
	res_hl(1);
}

void opcb_8f()
{
	res_reg8(1, A);
}

void opcb_90()
{
	res_reg8(2, B);
}

void opcb_91()
{
	res_reg8(2, C);
}

void opcb_92()
{
	res_reg8(2, D);
}

void opcb_93()
{
	res_reg8(2, E);
}

void opcb_94()
{
	res_reg8(2, H);
}

void opcb_95()
{
	res_reg8(2, L);
}

void opcb_96()
{
	res_hl(2);
}

void opcb_97()
{
	res_reg8(2, A);
}

void opcb_98()
{
	res_reg8(3, B);
}

void opcb_99()
{
	res_reg8(3, C);
}

void opcb_9a()
{
	res_reg8(3, D);
}

void opcb_9b()
{
	res_reg8(3, E);
}

void opcb_9c()
{
	res_reg8(3, H);
}

void opcb_9d()
{
	res_reg8(3, L);
}

void opcb_9e()
{
	res_hl(3);
}

void opcb_9f()
{
	res_reg8(3, A);
}

void opcb_a0()
{
	res_reg8(4, B);
}

void opcb_a1()
{
	res_reg8(4, C);
}

void opcb_a2()
{
	res_reg8(4, D);
}

void opcb_a3()
{
	res_reg8(4, E);
}

void opcb_a4()
{
	res_reg8(4, H);
}

void opcb_a5()
{
	res_reg8(4, L);
}

void opcb_a6()
{
	res_hl(4);
}

void opcb_a7()
{
	res_reg8(4, A);
}

void opcb_a8()
{
	res_reg8(5, B);
}

void opcb_a9()
{
	res_reg8(5, C);
}

void opcb_aa()
{
	res_reg8(5, D);
}

void opcb_ab()
{
	res_reg8(5, E);
}

void opcb_ac()
{
	res_reg8(5, H);
}

void opcb_ad()
{
	res_reg8(5, L);
}

void opcb_ae()
{
	res_hl(5);
}

void opcb_af()
{
	res_reg8(5, A);
}

void opcb_b0()
{
	res_reg8(6, B);
}

void opcb_b1()
{
	res_reg8(6, C);
}

void opcb_b2()
{
	res_reg8(6, D);
}

void opcb_b3()
{
	res_reg8(6, E);
}

void opcb_b4()
{
	res_reg8(6, H);
}

void opcb_b5()
{
	res_reg8(6, L);
}

void opcb_b6()
{
	res_hl(6);
}

void opcb_b7()
{
	res_reg8(6, A);
}

void opcb_b8()
{
	res_reg8(7, B);
}

void opcb_b9()
{
	res_reg8(7, C);
}

void opcb_ba()
{
	res_reg8(7, D);
}

void opcb_bb()
{
	res_reg8(7, E);
}

void opcb_bc()
{
	res_reg8(7, H);
}

void opcb_bd()
{
	res_reg8(7, L);
}

void opcb_be()
{
	res_hl(7);
}

void opcb_bf()
{
	res_reg8(7, A);
}

void opcb_c0()
{
	set_reg8(0, B);
}

void opcb_c1()
{
	set_reg8(0, C);
}

void opcb_c2()
{
	set_reg8(0, D);
}

void opcb_c3()
{
	set_reg8(0, E);
}

void opcb_c4()
{
	set_reg8(0, H);
}

void opcb_c5()
{
	set_reg8(0, L);
}

void opcb_c6()
{
	set_hl(0);
}

void opcb_c7()
{
	set_reg8(0, A);
}

void opcb_c8()
{
	set_reg8(1, B);
}

void opcb_c9()
{
	set_reg8(1, C);
}

void opcb_ca()
{
	set_reg8(1, D);
}

void opcb_cb()
{
	set_reg8(1, E);
}

void opcb_cc()
{
	set_reg8(1, H);
}

void opcb_cd()
{
	set_reg8(1, L);
}

void opcb_ce()
{
	set_hl(1);
}

void opcb_cf()
{
	set_reg8(1, A);
}

void opcb_d0()
{
	set_reg8(2, B);
}

void opcb_d1()
{
	set_reg8(2, C);
}

void opcb_d2()
{
	set_reg8(2, D);
}

void opcb_d3()
{
	set_reg8(2, E);
}

void opcb_d4()
{
	set_reg8(2, H);
}

void opcb_d5()
{
	set_reg8(2, L);
}

void opcb_d6()
{
	set_hl(2);
}

void opcb_d7()
{
	set_reg8(2, A);
}

void opcb_d8()
{
	set_reg8(3, B);
}

void opcb_d9()
{
	set_reg8(3, C);
}

void opcb_da()
{
	set_reg8(3, D);
}

void opcb_db()
{
	set_reg8(3, E);
}

void opcb_dc()
{
	set_reg8(3, H);
}

void opcb_dd()
{
	set_reg8(3, L);
}

void opcb_de()
{
	set_hl(3);
}

void opcb_df()
{
	set_reg8(3, A);
}

void opcb_e0()
{
	set_reg8(4, B);
}

void opcb_e1()
{
	set_reg8(4, C);
}

void opcb_e2()
{
	set_reg8(4, D);
}

void opcb_e3()
{
	set_reg8(4, E);
}

void opcb_e4()
{
	set_reg8(4, H);
}

void opcb_e5()
{
	set_reg8(4, L);
}

void opcb_e6()
{
	set_hl(4);
}

void opcb_e7()
{
	set_reg8(4, A);
}

void opcb_e8()
{
	set_reg8(5, B);
}

void opcb_e9()
{
	set_reg8(5, C);
}

void opcb_ea()
{
	set_reg8(5, D);
}

void opcb_eb()
{
	set_reg8(5, E);
}

void opcb_ec()
{
	set_reg8(5, H);
}

void opcb_ed()
{
	set_reg8(5, L);
}

void opcb_ee()
{
	set_hl(5);
}

void opcb_ef()
{
	set_reg8(5, A);
}

void opcb_f0()
{
	set_reg8(6, B);
}

void opcb_f1()
{
	set_reg8(6, C);
}

void opcb_f2()
{
	set_reg8(6, D);
}

void opcb_f3()
{
	set_reg8(6, E);
}

void opcb_f4()
{
	set_reg8(6, H);
}

void opcb_f5()
{
	set_reg8(6, L);
}

void opcb_f6()
{
	set_hl(6);
}

void opcb_f7()
{
	set_reg8(6, A);
}

void opcb_f8()
{
	set_reg8(7, B);
}

void opcb_f9()
{
	set_reg8(7, C);
}

void opcb_fa()
{
	set_reg8(7, D);
}

void opcb_fb()
{
	set_reg8(7, E);
}

void opcb_fc()
{
	set_reg8(7, H);
}

void opcb_fd()
{
	set_reg8(7, L);
}

void opcb_fe()
{
	set_hl(7);
}

void opcb_ff()
{
	set_reg8(7, A);
}

