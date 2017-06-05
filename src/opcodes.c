#include "opcodes.h"
#include "globaldefs.h"
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
}

void opcode_01()
{
}

void opcode_02()
{
}

void opcode_03()
{
}

void opcode_04()
{
}

void opcode_05()
{
}

void opcode_06()
{
}

void opcode_07()
{
}

void opcode_08()
{
}

void opcode_09()
{
}

void opcode_0a()
{
}

void opcode_0b()
{
}

void opcode_0c()
{
}

void opcode_0d()
{
}

void opcode_0e()
{
}

void opcode_0f()
{
}

void opcode_10()
{
}

void opcode_11()
{
}

void opcode_12()
{
}

void opcode_13()
{
}

void opcode_14()
{
}

void opcode_15()
{
}

void opcode_16()
{
}

void opcode_17()
{
}

void opcode_18()
{
}

void opcode_19()
{
}

void opcode_1a()
{
}

void opcode_1b()
{
}

void opcode_1c()
{
}

void opcode_1d()
{
}

void opcode_1e()
{
}

void opcode_1f()
{
}

void opcode_20()
{
}

void opcode_21()
{
}

void opcode_22()
{
}

void opcode_23()
{
}

void opcode_24()
{
}

void opcode_25()
{
}

void opcode_26()
{
}

void opcode_27()
{
}

void opcode_28()
{
}

void opcode_29()
{
}

void opcode_2a()
{
}

void opcode_2b()
{
}

void opcode_2c()
{
}

void opcode_2d()
{
}

void opcode_2e()
{
}

void opcode_2f()
{
}

void opcode_30()
{
}

void opcode_31()
{
}

void opcode_32()
{
}

void opcode_33()
{
}

void opcode_34()
{
}

void opcode_35()
{
}

void opcode_36()
{
}

void opcode_37()
{
}

void opcode_38()
{
}

void opcode_39()
{
}

void opcode_3a()
{
}

void opcode_3b()
{
}

void opcode_3c()
{
}

void opcode_3d()
{
}

void opcode_3e()
{
}

void opcode_3f()
{
}

void opcode_40()
{
}

void opcode_41()
{
}

void opcode_42()
{
}

void opcode_43()
{
}

void opcode_44()
{
}

void opcode_45()
{
}

void opcode_46()
{
}

void opcode_47()
{
}

void opcode_48()
{
}

void opcode_49()
{
}

void opcode_4a()
{
}

void opcode_4b()
{
}

void opcode_4c()
{
}

void opcode_4d()
{
}

void opcode_4e()
{
}

void opcode_4f()
{
}

void opcode_50()
{
}

void opcode_51()
{
}

void opcode_52()
{
}

void opcode_53()
{
}

void opcode_54()
{
}

void opcode_55()
{
}

void opcode_56()
{
}

void opcode_57()
{
}

void opcode_58()
{
}

void opcode_59()
{
}

void opcode_5a()
{
}

void opcode_5b()
{
}

void opcode_5c()
{
}

void opcode_5d()
{
}

void opcode_5e()
{
}

void opcode_5f()
{
}

void opcode_60()
{
}

void opcode_61()
{
}

void opcode_62()
{
}

void opcode_63()
{
}

void opcode_64()
{
}

void opcode_65()
{
}

void opcode_66()
{
}

void opcode_67()
{
}

void opcode_68()
{
}

void opcode_69()
{
}

void opcode_6a()
{
}

void opcode_6b()
{
}

void opcode_6c()
{
}

void opcode_6d()
{
}

void opcode_6e()
{
}

void opcode_6f()
{
}

void opcode_70()
{
}

void opcode_71()
{
}

void opcode_72()
{
}

void opcode_73()
{
}

void opcode_74()
{
}

void opcode_75()
{
}

void opcode_76()
{
}

void opcode_77()
{
}

void opcode_78()
{
}

void opcode_79()
{
}

void opcode_7a()
{
}

void opcode_7b()
{
}

void opcode_7c()
{
}

void opcode_7d()
{
}

void opcode_7e()
{
}

void opcode_7f()
{
}

void opcode_80()
{
}

void opcode_81()
{
}

void opcode_82()
{
}

void opcode_83()
{
}

void opcode_84()
{
}

void opcode_85()
{
}

void opcode_86()
{
}

void opcode_87()
{
}

void opcode_88()
{
}

void opcode_89()
{
}

void opcode_8a()
{
}

void opcode_8b()
{
}

void opcode_8c()
{
}

void opcode_8d()
{
}

void opcode_8e()
{
}

void opcode_8f()
{
}

void opcode_90()
{
}

void opcode_91()
{
}

void opcode_92()
{
}

void opcode_93()
{
}

void opcode_94()
{
}

void opcode_95()
{
}

void opcode_96()
{
}

void opcode_97()
{
}

void opcode_98()
{
}

void opcode_99()
{
}

void opcode_9a()
{
}

void opcode_9b()
{
}

void opcode_9c()
{
}

void opcode_9d()
{
}

void opcode_9e()
{
}

void opcode_9f()
{
}

void opcode_a0()
{
}

void opcode_a1()
{
}

void opcode_a2()
{
}

void opcode_a3()
{
}

void opcode_a4()
{
}

void opcode_a5()
{
}

void opcode_a6()
{
}

void opcode_a7()
{
}

void opcode_a8()
{
}

void opcode_a9()
{
}

void opcode_aa()
{
}

void opcode_ab()
{
}

void opcode_ac()
{
}

void opcode_ad()
{
}

void opcode_ae()
{
}

void opcode_af()
{
}

void opcode_b0()
{
}

void opcode_b1()
{
}

void opcode_b2()
{
}

void opcode_b3()
{
}

void opcode_b4()
{
}

void opcode_b5()
{
}

void opcode_b6()
{
}

void opcode_b7()
{
}

void opcode_b8()
{
}

void opcode_b9()
{
}

void opcode_ba()
{
}

void opcode_bb()
{
}

void opcode_bc()
{
}

void opcode_bd()
{
}

void opcode_be()
{
}

void opcode_bf()
{
}

void opcode_c0()
{
}

void opcode_c1()
{
}

void opcode_c2()
{
}

void opcode_c3()
{
}

void opcode_c4()
{
}

void opcode_c5()
{
}

void opcode_c6()
{
}

void opcode_c7()
{
}

void opcode_c8()
{
}

void opcode_c9()
{
}

void opcode_ca()
{
}

void opcode_cb()
{
}

void opcode_cc()
{
}

void opcode_cd()
{
}

void opcode_ce()
{
}

void opcode_cf()
{
}

void opcode_d0()
{
}

void opcode_d1()
{
}

void opcode_d2()
{
}

void opcode_d3()
{
}

void opcode_d4()
{
}

void opcode_d5()
{
}

void opcode_d6()
{
}

void opcode_d7()
{
}

void opcode_d8()
{
}

void opcode_d9()
{
}

void opcode_da()
{
}

void opcode_db()
{
}

void opcode_dc()
{
}

void opcode_dd()
{
}

void opcode_de()
{
}

void opcode_df()
{
}

void opcode_e0()
{
}

void opcode_e1()
{
}

void opcode_e2()
{
}

void opcode_e3()
{
}

void opcode_e4()
{
}

void opcode_e5()
{
}

void opcode_e6()
{
}

void opcode_e7()
{
}

void opcode_e8()
{
}

void opcode_e9()
{
}

void opcode_ea()
{
}

void opcode_eb()
{
}

void opcode_ec()
{
}

void opcode_ed()
{
}

void opcode_ee()
{
}

void opcode_ef()
{
}

void opcode_f0()
{
}

void opcode_f1()
{
}

void opcode_f2()
{
}

void opcode_f3()
{
}

void opcode_f4()
{
}

void opcode_f5()
{
}

void opcode_f6()
{
}

void opcode_f7()
{
}

void opcode_f8()
{
}

void opcode_f9()
{
}

void opcode_fa()
{
}

void opcode_fb()
{
}

void opcode_fc()
{
}

void opcode_fd()
{
}

void opcode_fe()
{
}

void opcode_ff()
{
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


