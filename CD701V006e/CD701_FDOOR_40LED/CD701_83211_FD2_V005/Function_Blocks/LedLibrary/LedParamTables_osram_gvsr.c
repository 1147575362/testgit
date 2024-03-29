#include <ledParamTables.h>
/* Efficiency is 1024 @25C , the value is 990*1024/1000 when efficiency is 99.0% */ 
//#if LEDPARAMTABLES == OSRAM_GVSR
static const TempParams_t TEMP_CURVE_TABLE_RED[TEMP_CMP_NUM_MAX] = 
{
    /*T(1C) efficiency deltaV(mV) x(%), y(%) */
/*0*/{1182U,34,1017U,1032U},
/*2*/{1168U,31,1018U,1032U},
/*4*/{1155U,28,1018U,1031U},
/*6*/{1142U,25,1019U,1030U},
/*8*/{1129U,21,1019U,1030U},
/*10*/{1116U,19,1020U,1029U},
/*12*/{1103U,17,1020U,1028U},
/*14*/{1089U,15,1021U,1028U},
/*16*/{1076U,12,1021U,1027U},
/*18*/{1063U,9,1021U,1026U},
/*20*/{1050U,6,1022U,1026U},
/*22*/{1037U,3,1023U,1024U},
/*24*/{1024U,0,1024U,1024U},
/*26*/{1010U,-3,1024U,1023U},
/*28*/{997U,-6,1025U,1022U},
/*30*/{984U,-9,1026U,1021U},
/*32*/{971U,-12,1026U,1021U},
/*34*/{958U,-15,1026U,1020U},
/*36*/{944U,-18,1026U,1019U},
/*38*/{931U,-21,1027U,1018U},
/*40*/{918U,-24,1027U,1018U},
/*42*/{905U,-27,1027U,1017U},
/*44*/{892U,-30,1027U,1016U},
/*46*/{873U,-33,1028U,1015U},
/*48*/{866U,-36,1028U,1015U},
/*50*/{855U,-39,1028U,1014U},
/*52*/{835U,-42,1029U,1013U},
/*54*/{821U,-45,1029U,1012U},
/*56*/{801U,-49,1029U,1012U},
/*58*/{785U,-53,1030U,1010U},
/*60*/{776U,-55,1030U,1010U},
/*62*/{766U,-57,1030U,1010U},
/*64*/{750U,-60,1030U,1009U},
/*66*/{741U,-62,1030U,1009U},
/*68*/{734U,-64,1030U,1008U},
/*70*/{731U,-68,1031U,1008U},
/*72*/{713U,-70,1031U,1008U},
/*74*/{703U,-73,1031U,1007U},
/*76*/{694U,-77,1031U,1007U},
/*78*/{676U,-82,1031U,1006U},
/*80*/{657U,-87,1032U,1006U},
/*82*/{648U,-89,1032U,1005U},
/*84*/{636U,-92,1032U,1005U},
/*86*/{616U,-97,1032U,1004U},
/*88*/{597U,-100,1032U,1004U},
/*90*/{585U,-102,1033U,1003U},
/*92*/{576U,-103,1033U,1003U},
/*94*/{557U,-107,1033U,1003U},
/*96*/{547U,-110,1033U,1002U},
/*98*/{537U,-113,1033U,1002U},
/*100*/{526U,-116,1033U,1002U},
/*102*/{516U,-120,1033U,1001U},
/*104*/{505U,-123,1033U,1001U},
/*106*/{495U,-126,1033U,1001U},
/*108*/{485U,-128,1034U,1000U},
/*110*/{474U,-130,1034U,1000U},
/*112*/{464U,-133,1034U,1000U},
/*114*/{453U,-135,1034U,999U},
/*116*/{443U,-140,1034U,999U},
/*118*/{433U,-143,1034U,998U},
/*120*/{422U,-146,1034U,998U},

};

static const TempParams_t TEMP_CURVE_TABLE_GREEN[TEMP_CMP_NUM_MAX] = 
{
    /*T(1C) efficiency deltaV(mV) x(%), y(%) */
/*0*/{1028U,28,978U,1027U},
/*2*/{1027U,23,982U,1027U},
/*4*/{1027U,22,986U,1026U},
/*6*/{1027U,18,990U,1026U},
/*8*/{1026U,16,993U,1026U},
/*10*/{1026U,13,997U,1025U},
/*12*/{1026U,12,1001U,1025U},
/*14*/{1025U,10,1005U,1025U},
/*16*/{1025U,8,1008U,1025U},
/*18*/{1025U,6,1012U,1024U},
/*20*/{1024U,4,1016U,1024U},
/*22*/{1024U,2,1020U,1024U},
/*24*/{1024U,0,1024U,1024U},
/*26*/{1023U,-2,1027U,1023U},
/*28*/{1023U,-4,1031U,1023U},
/*30*/{1022U,-6,1035U,1023U},
/*32*/{1022U,-8,1039U,1022U},
/*34*/{1022U,-10,1042U,1022U},
/*36*/{1021U,-12,1046U,1022U},
/*38*/{1021U,-14,1050U,1021U},
/*40*/{1021U,-16,1054U,1021U},
/*42*/{1020U,-18,1056U,1021U},
/*44*/{1020U,-20,1067U,1020U},
/*46*/{1020U,-22,1074U,1019U},
/*48*/{1019U,-24,1080U,1018U},
/*50*/{1019U,-26,1086U,1017U},
/*52*/{1019U,-28,1093U,1016U},
/*54*/{1018U,-30,1096U,1015U},
/*56*/{1018U,-36,1101U,1014U},
/*58*/{1018U,-38,1106U,1013U},
/*60*/{1017U,-41,1112U,1013U},
/*62*/{1016U,-45,1117U,1012U},
/*64*/{1016U,-47,1121U,1011U},
/*66*/{1015U,-51,1118U,1011U},
/*68*/{1015U,-52,1128U,1010U},
/*70*/{1015U,-56,1133U,1009U},
/*72*/{1013U,-56,1140U,1008U},
/*74*/{1012U,-58,1142U,1007U},
/*76*/{1010U,-61,1145U,1007U},
/*78*/{1010U,-62,1148U,1006U},
/*80*/{1009U,-64,1152U,1006U},
/*82*/{1006U,-66,1158U,1005U},
/*84*/{1004U,-67,1164U,1004U},
/*86*/{1001U,-69,1169U,1003U},
/*88*/{998U,-71,1177U,1002U},
/*90*/{995U,-73,1182U,1001U},
/*92*/{994U,-75,1185U,1000U},
/*94*/{993U,-77,1187U,1000U},
/*96*/{991U,-79,1190U,999U},
/*98*/{990U,-80,1194U,999U},
/*100*/{987U,-82,1200U,997U},
/*102*/{985U,-84,1203U,997U},
/*104*/{983U,-86,1207U,996U},
/*106*/{982U,-86,1211U,996U},
/*108*/{980U,-87,1214U,996U},
/*110*/{978U,-89,1218U,995U},
/*112*/{976U,-91,1222U,995U},
/*114*/{975U,-92,1226U,994U},
/*116*/{973U,-96,1229U,994U},
/*118*/{971U,-98,1233U,993U},
/*120*/{969U,-100,1237U,993U},
};


static const TempParams_t TEMP_CURVE_TABLE_BLUE[TEMP_CMP_NUM_MAX] = 
{
    /*T(1C) efficiency deltaV(mV) x(%), y(%) */
/*0*/{931U,35,1032U,941U},
/*2*/{938U,32,1031U,948U},
/*4*/{946U,30,1031U,955U},
/*6*/{954U,26,1030U,962U},
/*8*/{962U,24,1029U,968U},
/*10*/{969U,21,1029U,975U},
/*12*/{977U,18,1028U,982U},
/*14*/{985U,15,1027U,989U},
/*16*/{993U,12,1026U,996U},
/*18*/{1000U,9,1026U,1003U},
/*20*/{1008U,6,1025U,1010U},
/*22*/{1016U,3,1024U,1017U},
/*24*/{1024U,0,1024U,1024U},
/*26*/{1031U,-3,1023U,1030U},
/*28*/{1039U,-6,1022U,1037U},
/*30*/{1047U,-9,1021U,1044U},
/*32*/{1054U,-12,1021U,1051U},
/*34*/{1062U,-15,1020U,1058U},
/*36*/{1070U,-18,1019U,1065U},
/*38*/{1078U,-21,1018U,1072U},
/*40*/{1085U,-24,1018U,1079U},
/*42*/{1093U,-27,1017U,1085U},
/*44*/{1101U,-30,1016U,1092U},
/*46*/{1109U,-33,1016U,1099U},
/*48*/{1116U,-36,1015U,1106U},
/*50*/{1127U,-39,1013U,1111U},
/*52*/{1133U,-42,1012U,1115U},
/*54*/{1139U,-45,1011U,1120U},
/*56*/{1150U,-50,1009U,1129U},
/*58*/{1168U,-51,1007U,1143U},
/*60*/{1177U,-54,1005U,1150U},
/*62*/{1187U,-59,1003U,1161U},
/*64*/{1196U,-60,1002U,1170U},
/*66*/{1201U,-63,1000U,1187U},
/*68*/{1207U,-66,1000U,1182U},
/*70*/{1216U,-70,998U,1191U},
/*72*/{1222U,-72,997U,1198U},
/*74*/{1227U,-75,996U,1205U},
/*76*/{1237U,-78,994U,1216U},
/*78*/{1248U,-81,993U,1230U},
/*80*/{1256U,-83,991U,1242U},
/*82*/{1263U,-87,990U,1249U},
/*84*/{1268U,-88,989U,1255U},
/*86*/{1275U,-92,988U,1267U},
/*88*/{1287U,-95,986U,1281U},
/*90*/{1295U,-98,984U,1292U},
/*92*/{1302U,-101,983U,1299U},
/*94*/{1307U,-104,982U,1308U},
/*96*/{1315U,-111,980U,1320U},
/*98*/{1316U,-112,978U,1331U},
/*100*/{1317U,-115,976U,1343U},
/*102*/{1317U,-118,973U,1354U},
/*104*/{1318U,-120,971U,1366U},
/*106*/{1319U,-123,969U,1377U},
/*108*/{1320U,-124,967U,1389U},
/*110*/{1320U,-124,965U,1400U},
/*112*/{1321U,-127,963U,1412U},
/*114*/{1322U,-128,960U,1423U},
/*116*/{1323U,-132,958U,1434U},
/*118*/{1323U,-135,956U,1446U},
/*120*/{1324U,-138,954U,1457U},


};



TempParams_t const * LP_GetTempTableRed(void)
{
    return TEMP_CURVE_TABLE_RED;
}

TempParams_t const * LP_GetTempTableGreen(void)
{
    return TEMP_CURVE_TABLE_GREEN;
}

TempParams_t const * LP_GetTempTableBlue(void)
{
    return TEMP_CURVE_TABLE_BLUE;
}



/*
const AccurateColorXYY_t calibratedColortable[] = 
{
  {21845, 21845, 100, 255,255,255},
  {X0, Y0, 100, 254,254,255},
};
*/



