/*******************************************************************************
HorridgeTemplate.cpp

Created by Geoffrey Barrows.

History:
March 6, 2017: Created.
===============================================================================
Copyright (c) 2017, Centeye, Inc.
All rights reserved.

* This software is licensed for use only on products by Centeye.
* All modifications to this software shall be the property of Centeye.
* Use of this software is solely at your own risk and Centeye shall not be 
responsible for any damages, direct or indirect, sustained or caused while 
using this software.
* Centeye makes no warranty against patent infringement.
* Use of this software implies consent to the above terms.
* Any deviation from the above terms shall be allowable only through written 
authorization.
*******************************************************************************/


#include "HorridgeTemplate.h"
#define myprintf pComm->TxPrintF	// macro to simplify 


/*******************************************************************************
Initialize
Object initialization

Inputs:	none
*******************************************************************************/
void HorridgeTemplate::Initialize(void)
{
	short i;
	
	BCActive = 0x00;
	for (i=0; i<HT_MAX_BINCHANNELS; ++i) {
		BCType[i] = -1;
		BCLineStart[i] = 1;
		BCLineSkip[i] = 0;
		BCLineNum[i] = 0;
		BCRadius[i] = 0;
		BCPixelStart[i] = BCPixelStop[i] = 1;
		BCThreshold[i] = 0;
	}
	CapNum = 0;
	for (i=0; i<HT_MAX_CAP; ++i) {
		CapType[i] = -1;
		CapR1[i] = CapR2[i] = CapC1[i] = CapC2[i] = 0;
		CapInvArea[i] = 1.0;
		CapQX[i] = CapQY[i] = 0.0;
	}
	CTNum = 0;
	for (i=0; i<HT_MAX_CAPTYPE; ++i) {
		CTAlpha[i] = 0.1;
		CTAlphaOM[i] = 0.9;
	}
	FFScalar = 2.5;
	
	// Make stimulation array T4
	T4[0] = 0; // 0 is 0000 -> 0000 stim 0
	T4[1] = -1; // 1 is 0000 -> 0001 stim -1
	T4[2] = 0; // 2 is 0000 -> 0010 stim 0
	T4[3] = -1; // 3 is 0000 -> 0011 stim -1
	T4[4] = 0; // 4 is 0000 -> 0100 stim 0
	T4[5] = -1; // 5 is 0000 -> 0101 stim -1
	T4[6] = 0; // 6 is 0000 -> 0110 stim 0
	T4[7] = -1; // 7 is 0000 -> 0111 stim -1
	T4[8] = 1; // 8 is 0000 -> 1000 stim 1
	T4[9] = 0; // 9 is 0000 -> 1001 stim 0
	T4[10] = 1; // 10 is 0000 -> 1010 stim 1
	T4[11] = 0; // 11 is 0000 -> 1011 stim 0
	T4[12] = 1; // 12 is 0000 -> 1100 stim 1
	T4[13] = 0; // 13 is 0000 -> 1101 stim 0
	T4[14] = 1; // 14 is 0000 -> 1110 stim 1
	T4[15] = 0; // 15 is 0000 -> 1111 stim 0
	T4[16] = 1; // 16 is 0001 -> 0000 stim 1
	T4[17] = 0; // 17 is 0001 -> 0001 stim 0
	T4[18] = -1; // 18 is 0001 -> 0010 stim -1
	T4[19] = -2; // 19 is 0001 -> 0011 stim -2
	T4[20] = 1; // 20 is 0001 -> 0100 stim 1
	T4[21] = 0; // 21 is 0001 -> 0101 stim 0
	T4[22] = -1; // 22 is 0001 -> 0110 stim -1
	T4[23] = -2; // 23 is 0001 -> 0111 stim -2
	T4[24] = 2; // 24 is 0001 -> 1000 stim 2
	T4[25] = 1; // 25 is 0001 -> 1001 stim 1
	T4[26] = 0; // 26 is 0001 -> 1010 stim 0
	T4[27] = -1; // 27 is 0001 -> 1011 stim -1
	T4[28] = 2; // 28 is 0001 -> 1100 stim 2
	T4[29] = 1; // 29 is 0001 -> 1101 stim 1
	T4[30] = 0; // 30 is 0001 -> 1110 stim 0
	T4[31] = -1; // 31 is 0001 -> 1111 stim -1
	T4[32] = 0; // 32 is 0010 -> 0000 stim 0
	T4[33] = 1; // 33 is 0010 -> 0001 stim 1
	T4[34] = 0; // 34 is 0010 -> 0010 stim 0
	T4[35] = 1; // 35 is 0010 -> 0011 stim 1
	T4[36] = -2; // 36 is 0010 -> 0100 stim -2
	T4[37] = -1; // 37 is 0010 -> 0101 stim -1
	T4[38] = -2; // 38 is 0010 -> 0110 stim -2
	T4[39] = -1; // 39 is 0010 -> 0111 stim -1
	T4[40] = 1; // 40 is 0010 -> 1000 stim 1
	T4[41] = 2; // 41 is 0010 -> 1001 stim 2
	T4[42] = 1; // 42 is 0010 -> 1010 stim 1
	T4[43] = 2; // 43 is 0010 -> 1011 stim 2
	T4[44] = -1; // 44 is 0010 -> 1100 stim -1
	T4[45] = 0; // 45 is 0010 -> 1101 stim 0
	T4[46] = -1; // 46 is 0010 -> 1110 stim -1
	T4[47] = 0; // 47 is 0010 -> 1111 stim 0
	T4[48] = 1; // 48 is 0011 -> 0000 stim 1
	T4[49] = 2; // 49 is 0011 -> 0001 stim 2
	T4[50] = -1; // 50 is 0011 -> 0010 stim -1
	T4[51] = 0; // 51 is 0011 -> 0011 stim 0
	T4[52] = -1; // 52 is 0011 -> 0100 stim -1
	T4[53] = 0; // 53 is 0011 -> 0101 stim 0
	T4[54] = -3; // 54 is 0011 -> 0110 stim -3
	T4[55] = -2; // 55 is 0011 -> 0111 stim -2
	T4[56] = 2; // 56 is 0011 -> 1000 stim 2
	T4[57] = 3; // 57 is 0011 -> 1001 stim 3
	T4[58] = 0; // 58 is 0011 -> 1010 stim 0
	T4[59] = 1; // 59 is 0011 -> 1011 stim 1
	T4[60] = 0; // 60 is 0011 -> 1100 stim 0
	T4[61] = 1; // 61 is 0011 -> 1101 stim 1
	T4[62] = -2; // 62 is 0011 -> 1110 stim -2
	T4[63] = -1; // 63 is 0011 -> 1111 stim -1
	T4[64] = 0; // 64 is 0100 -> 0000 stim 0
	T4[65] = -1; // 65 is 0100 -> 0001 stim -1
	T4[66] = 2; // 66 is 0100 -> 0010 stim 2
	T4[67] = 1; // 67 is 0100 -> 0011 stim 1
	T4[68] = 0; // 68 is 0100 -> 0100 stim 0
	T4[69] = -1; // 69 is 0100 -> 0101 stim -1
	T4[70] = 2; // 70 is 0100 -> 0110 stim 2
	T4[71] = 1; // 71 is 0100 -> 0111 stim 1
	T4[72] = -1; // 72 is 0100 -> 1000 stim -1
	T4[73] = -2; // 73 is 0100 -> 1001 stim -2
	T4[74] = 1; // 74 is 0100 -> 1010 stim 1
	T4[75] = 0; // 75 is 0100 -> 1011 stim 0
	T4[76] = -1; // 76 is 0100 -> 1100 stim -1
	T4[77] = -2; // 77 is 0100 -> 1101 stim -2
	T4[78] = 1; // 78 is 0100 -> 1110 stim 1
	T4[79] = 0; // 79 is 0100 -> 1111 stim 0
	T4[80] = 1; // 80 is 0101 -> 0000 stim 1
	T4[81] = 0; // 81 is 0101 -> 0001 stim 0
	T4[82] = 1; // 82 is 0101 -> 0010 stim 1
	T4[83] = 0; // 83 is 0101 -> 0011 stim 0
	T4[84] = 1; // 84 is 0101 -> 0100 stim 1
	T4[85] = 0; // 85 is 0101 -> 0101 stim 0
	T4[86] = 1; // 86 is 0101 -> 0110 stim 1
	T4[87] = 0; // 87 is 0101 -> 0111 stim 0
	T4[88] = 0; // 88 is 0101 -> 1000 stim 0
	T4[89] = -1; // 89 is 0101 -> 1001 stim -1
	T4[90] = 0; // 90 is 0101 -> 1010 stim 0
	T4[91] = -1; // 91 is 0101 -> 1011 stim -1
	T4[92] = 0; // 92 is 0101 -> 1100 stim 0
	T4[93] = -1; // 93 is 0101 -> 1101 stim -1
	T4[94] = 0; // 94 is 0101 -> 1110 stim 0
	T4[95] = -1; // 95 is 0101 -> 1111 stim -1
	T4[96] = 0; // 96 is 0110 -> 0000 stim 0
	T4[97] = 1; // 97 is 0110 -> 0001 stim 1
	T4[98] = 2; // 98 is 0110 -> 0010 stim 2
	T4[99] = 3; // 99 is 0110 -> 0011 stim 3
	T4[100] = -2; // 100 is 0110 -> 0100 stim -2
	T4[101] = -1; // 101 is 0110 -> 0101 stim -1
	T4[102] = 0; // 102 is 0110 -> 0110 stim 0
	T4[103] = 1; // 103 is 0110 -> 0111 stim 1
	T4[104] = -1; // 104 is 0110 -> 1000 stim -1
	T4[105] = 0; // 105 is 0110 -> 1001 stim 0
	T4[106] = 1; // 106 is 0110 -> 1010 stim 1
	T4[107] = 2; // 107 is 0110 -> 1011 stim 2
	T4[108] = -3; // 108 is 0110 -> 1100 stim -3
	T4[109] = -2; // 109 is 0110 -> 1101 stim -2
	T4[110] = -1; // 110 is 0110 -> 1110 stim -1
	T4[111] = 0; // 111 is 0110 -> 1111 stim 0
	T4[112] = 1; // 112 is 0111 -> 0000 stim 1
	T4[113] = 2; // 113 is 0111 -> 0001 stim 2
	T4[114] = 1; // 114 is 0111 -> 0010 stim 1
	T4[115] = 2; // 115 is 0111 -> 0011 stim 2
	T4[116] = -1; // 116 is 0111 -> 0100 stim -1
	T4[117] = 0; // 117 is 0111 -> 0101 stim 0
	T4[118] = -1; // 118 is 0111 -> 0110 stim -1
	T4[119] = 0; // 119 is 0111 -> 0111 stim 0
	T4[120] = 0; // 120 is 0111 -> 1000 stim 0
	T4[121] = 1; // 121 is 0111 -> 1001 stim 1
	T4[122] = 0; // 122 is 0111 -> 1010 stim 0
	T4[123] = 1; // 123 is 0111 -> 1011 stim 1
	T4[124] = -2; // 124 is 0111 -> 1100 stim -2
	T4[125] = -1; // 125 is 0111 -> 1101 stim -1
	T4[126] = -2; // 126 is 0111 -> 1110 stim -2
	T4[127] = -1; // 127 is 0111 -> 1111 stim -1
	T4[128] = -1; // 128 is 1000 -> 0000 stim -1
	T4[129] = -2; // 129 is 1000 -> 0001 stim -2
	T4[130] = -1; // 130 is 1000 -> 0010 stim -1
	T4[131] = -2; // 131 is 1000 -> 0011 stim -2
	T4[132] = 1; // 132 is 1000 -> 0100 stim 1
	T4[133] = 0; // 133 is 1000 -> 0101 stim 0
	T4[134] = 1; // 134 is 1000 -> 0110 stim 1
	T4[135] = 0; // 135 is 1000 -> 0111 stim 0
	T4[136] = 0; // 136 is 1000 -> 1000 stim 0
	T4[137] = -1; // 137 is 1000 -> 1001 stim -1
	T4[138] = 0; // 138 is 1000 -> 1010 stim 0
	T4[139] = -1; // 139 is 1000 -> 1011 stim -1
	T4[140] = 2; // 140 is 1000 -> 1100 stim 2
	T4[141] = 1; // 141 is 1000 -> 1101 stim 1
	T4[142] = 2; // 142 is 1000 -> 1110 stim 2
	T4[143] = 1; // 143 is 1000 -> 1111 stim 1
	T4[144] = 0; // 144 is 1001 -> 0000 stim 0
	T4[145] = -1; // 145 is 1001 -> 0001 stim -1
	T4[146] = -2; // 146 is 1001 -> 0010 stim -2
	T4[147] = -3; // 147 is 1001 -> 0011 stim -3
	T4[148] = 2; // 148 is 1001 -> 0100 stim 2
	T4[149] = 1; // 149 is 1001 -> 0101 stim 1
	T4[150] = 0; // 150 is 1001 -> 0110 stim 0
	T4[151] = -1; // 151 is 1001 -> 0111 stim -1
	T4[152] = 1; // 152 is 1001 -> 1000 stim 1
	T4[153] = 0; // 153 is 1001 -> 1001 stim 0
	T4[154] = -1; // 154 is 1001 -> 1010 stim -1
	T4[155] = -2; // 155 is 1001 -> 1011 stim -2
	T4[156] = 3; // 156 is 1001 -> 1100 stim 3
	T4[157] = 2; // 157 is 1001 -> 1101 stim 2
	T4[158] = 1; // 158 is 1001 -> 1110 stim 1
	T4[159] = 0; // 159 is 1001 -> 1111 stim 0
	T4[160] = -1; // 160 is 1010 -> 0000 stim -1
	T4[161] = 0; // 161 is 1010 -> 0001 stim 0
	T4[162] = -1; // 162 is 1010 -> 0010 stim -1
	T4[163] = 0; // 163 is 1010 -> 0011 stim 0
	T4[164] = -1; // 164 is 1010 -> 0100 stim -1
	T4[165] = 0; // 165 is 1010 -> 0101 stim 0
	T4[166] = -1; // 166 is 1010 -> 0110 stim -1
	T4[167] = 0; // 167 is 1010 -> 0111 stim 0
	T4[168] = 0; // 168 is 1010 -> 1000 stim 0
	T4[169] = 1; // 169 is 1010 -> 1001 stim 1
	T4[170] = 0; // 170 is 1010 -> 1010 stim 0
	T4[171] = 1; // 171 is 1010 -> 1011 stim 1
	T4[172] = 0; // 172 is 1010 -> 1100 stim 0
	T4[173] = 1; // 173 is 1010 -> 1101 stim 1
	T4[174] = 0; // 174 is 1010 -> 1110 stim 0
	T4[175] = 1; // 175 is 1010 -> 1111 stim 1
	T4[176] = 0; // 176 is 1011 -> 0000 stim 0
	T4[177] = 1; // 177 is 1011 -> 0001 stim 1
	T4[178] = -2; // 178 is 1011 -> 0010 stim -2
	T4[179] = -1; // 179 is 1011 -> 0011 stim -1
	T4[180] = 0; // 180 is 1011 -> 0100 stim 0
	T4[181] = 1; // 181 is 1011 -> 0101 stim 1
	T4[182] = -2; // 182 is 1011 -> 0110 stim -2
	T4[183] = -1; // 183 is 1011 -> 0111 stim -1
	T4[184] = 1; // 184 is 1011 -> 1000 stim 1
	T4[185] = 2; // 185 is 1011 -> 1001 stim 2
	T4[186] = -1; // 186 is 1011 -> 1010 stim -1
	T4[187] = 0; // 187 is 1011 -> 1011 stim 0
	T4[188] = 1; // 188 is 1011 -> 1100 stim 1
	T4[189] = 2; // 189 is 1011 -> 1101 stim 2
	T4[190] = -1; // 190 is 1011 -> 1110 stim -1
	T4[191] = 0; // 191 is 1011 -> 1111 stim 0
	T4[192] = -1; // 192 is 1100 -> 0000 stim -1
	T4[193] = -2; // 193 is 1100 -> 0001 stim -2
	T4[194] = 1; // 194 is 1100 -> 0010 stim 1
	T4[195] = 0; // 195 is 1100 -> 0011 stim 0
	T4[196] = 1; // 196 is 1100 -> 0100 stim 1
	T4[197] = 0; // 197 is 1100 -> 0101 stim 0
	T4[198] = 3; // 198 is 1100 -> 0110 stim 3
	T4[199] = 2; // 199 is 1100 -> 0111 stim 2
	T4[200] = -2; // 200 is 1100 -> 1000 stim -2
	T4[201] = -3; // 201 is 1100 -> 1001 stim -3
	T4[202] = 0; // 202 is 1100 -> 1010 stim 0
	T4[203] = -1; // 203 is 1100 -> 1011 stim -1
	T4[204] = 0; // 204 is 1100 -> 1100 stim 0
	T4[205] = -1; // 205 is 1100 -> 1101 stim -1
	T4[206] = 2; // 206 is 1100 -> 1110 stim 2
	T4[207] = 1; // 207 is 1100 -> 1111 stim 1
	T4[208] = 0; // 208 is 1101 -> 0000 stim 0
	T4[209] = -1; // 209 is 1101 -> 0001 stim -1
	T4[210] = 0; // 210 is 1101 -> 0010 stim 0
	T4[211] = -1; // 211 is 1101 -> 0011 stim -1
	T4[212] = 2; // 212 is 1101 -> 0100 stim 2
	T4[213] = 1; // 213 is 1101 -> 0101 stim 1
	T4[214] = 2; // 214 is 1101 -> 0110 stim 2
	T4[215] = 1; // 215 is 1101 -> 0111 stim 1
	T4[216] = -1; // 216 is 1101 -> 1000 stim -1
	T4[217] = -2; // 217 is 1101 -> 1001 stim -2
	T4[218] = -1; // 218 is 1101 -> 1010 stim -1
	T4[219] = -2; // 219 is 1101 -> 1011 stim -2
	T4[220] = 1; // 220 is 1101 -> 1100 stim 1
	T4[221] = 0; // 221 is 1101 -> 1101 stim 0
	T4[222] = 1; // 222 is 1101 -> 1110 stim 1
	T4[223] = 0; // 223 is 1101 -> 1111 stim 0
	T4[224] = -1; // 224 is 1110 -> 0000 stim -1
	T4[225] = 0; // 225 is 1110 -> 0001 stim 0
	T4[226] = 1; // 226 is 1110 -> 0010 stim 1
	T4[227] = 2; // 227 is 1110 -> 0011 stim 2
	T4[228] = -1; // 228 is 1110 -> 0100 stim -1
	T4[229] = 0; // 229 is 1110 -> 0101 stim 0
	T4[230] = 1; // 230 is 1110 -> 0110 stim 1
	T4[231] = 2; // 231 is 1110 -> 0111 stim 2
	T4[232] = -2; // 232 is 1110 -> 1000 stim -2
	T4[233] = -1; // 233 is 1110 -> 1001 stim -1
	T4[234] = 0; // 234 is 1110 -> 1010 stim 0
	T4[235] = 1; // 235 is 1110 -> 1011 stim 1
	T4[236] = -2; // 236 is 1110 -> 1100 stim -2
	T4[237] = -1; // 237 is 1110 -> 1101 stim -1
	T4[238] = 0; // 238 is 1110 -> 1110 stim 0
	T4[239] = 1; // 239 is 1110 -> 1111 stim 1
	T4[240] = 0; // 240 is 1111 -> 0000 stim 0
	T4[241] = 1; // 241 is 1111 -> 0001 stim 1
	T4[242] = 0; // 242 is 1111 -> 0010 stim 0
	T4[243] = 1; // 243 is 1111 -> 0011 stim 1
	T4[244] = 0; // 244 is 1111 -> 0100 stim 0
	T4[245] = 1; // 245 is 1111 -> 0101 stim 1
	T4[246] = 0; // 246 is 1111 -> 0110 stim 0
	T4[247] = 1; // 247 is 1111 -> 0111 stim 1
	T4[248] = -1; // 248 is 1111 -> 1000 stim -1
	T4[249] = 0; // 249 is 1111 -> 1001 stim 0
	T4[250] = -1; // 250 is 1111 -> 1010 stim -1
	T4[251] = 0; // 251 is 1111 -> 1011 stim 0
	T4[252] = -1; // 252 is 1111 -> 1100 stim -1
	T4[253] = 0; // 253 is 1111 -> 1101 stim 0
	T4[254] = -1; // 254 is 1111 -> 1110 stim -1
	T4[255] = 0; // 255 is 1111 -> 1111 stim 0
	// Make stimulation array T2
	T2[0] = 0; // 0 is 00 -> 00 stim 0	
	T2[1] = -1; // 0 is 00 -> 01 stim 0
	T2[2] = 1; // 0 is 00 -> 10 stim 0
	T2[3] = 0; // 0 is 00 -> 11 stim 0
	T2[4] = 1; // 0 is 01 -> 00 stim 0	
	T2[5] = 0; // 0 is 01 -> 01 stim 0
	T2[6] = 0; // 0 is 01 -> 10 stim 0
	T2[7] = -1; // 0 is 01 -> 11 stim 0
	T2[8] = -1; // 0 is 10 -> 00 stim 0	
	T2[9] = 0; // 0 is 10 -> 01 stim 0
	T2[10] = 0; // 0 is 10 -> 10 stim 0
	T2[11] = 1; // 0 is 10 -> 11 stim 0
	T2[12] = 0; // 0 is 11 -> 00 stim 0	
	T2[13] = 1; // 0 is 11 -> 01 stim 0
	T2[14] = -1; // 0 is 11 -> 10 stim 0
	T2[15] = 0; // 0 is 11 -> 11 stim 0	
}

/*******************************************************************************
SetupBinChannel
Sets up a binarization channel

Inputs:	see below
*******************************************************************************/
void HorridgeTemplate::SetupBinChannel(unsigned char channel, 
				       unsigned char channeltype, // Note that this byte is defined by direction nibble and feature nibble
				       unsigned char linestart,
				       unsigned char lineskip,
				       unsigned char linenum,
				       unsigned char radius,
				       unsigned char pixelstart,
				       unsigned char pixelstop,
				       int threshold)
{
	// check to see if this a valid channel
	if (channel >= HT_MAX_BINCHANNELS)
		return; // return without action if not
	// set binarization channel active
	BCActive |= (0x01 << channel); 
	// set other parameters
	BCType[channel] = channeltype;
	BCLineStart[channel] = linestart;
	BCLineSkip[channel] = lineskip;
	BCLineNum[channel] = linenum;
	BCRadius[channel] = radius;
	BCPixelStart[channel] = pixelstart;
	BCPixelStop[channel] = pixelstop;
	BCThreshold[channel] = threshold;
}

/*******************************************************************************
SetupCap
Sets up one capacitor

Inputs:	see below
*******************************************************************************/
void HorridgeTemplate::SetupCap(short capnum,
				unsigned char capr1,
				unsigned char capr2,
				unsigned char capc1,
				unsigned char capc2,
				char captype)
{
	if (capnum + 1 >= HT_MAX_CAP)
		return;
	// increase CapNum if needed
	if (capnum + 1 > CapNum)
		CapNum = capnum + 1;
	// set other parameters
	CapR1[capnum] = capr1;
	CapR2[capnum] = capr2;
	CapC1[capnum] = capc1;
	CapC2[capnum] = capc2;
	CapType[capnum] = captype;
	CapInvArea[capnum] = 1.0 / ((1 + capr2 - capr1) * (1 + capc2 - capc1));
}

/*******************************************************************************
SetupCapType
Sets up one capacitor type

Inputs:	see below
*******************************************************************************/
void HorridgeTemplate::SetupCapType(short captypenum,
				    float alpha)
{
	if (captypenum >= HT_MAX_CAPTYPE || captypenum < 0)
		return;
	CTAlpha[captypenum] = alpha;
	CTAlphaOM[captypenum] = 1.0 - alpha;
}


/*******************************************************************************
QuickUpdateParams
Generic command-driven input, to be used from test command or single integer
command input

Inputs:	command as single integer, read as decimal-type integer. Formating is
as follows:

10aaa -> BCActive = aaa. For example, to make only binarization channels 0 and 1
active we would want to send aaa=3 and thus command = decimal 10003 = 0x2713.

2ccaa -> CTAlpha[cc] = aa * 0.01. This sets the "alpha" and "1-alpha" values for
a capacitor type. For example, to set alpha = 0.15 for capacitor type 1, we
would use command = decimal 20115.

3bttt -> BCType[b] = ttt. This sets the binarization type of a channel. Note
that the binarization type must include both the direction and the kernel.
For example, if we wish to set binarization channel 2 so that it is X-direction
and uses kernal 5 (+1 -1 -1 +1) we would send:

	command = 30000 + 2000 + HT_CHANNEL_IS_ROW + HT_CHANNEL_FEAT_5_PNNP
	        = 32000 + 0x10 + 0x05
	        = 32000 + 16 + 5
	        = 32021

4bttt -> BCThreshold[b] = (ttt-500). This sets the threshold for a
binarization channel, and allows for both negative and positive thresholds.
For example, to zero the threshold for binariztion channel 3, send
command = 43500. To set the threshold for binarization channel 3 to 70, send
command = 43070. To set it to -40 send command = 43460

5brrr -> BCRadius[b] = rrr. This sets the radius of the feature of a
binarization channel. IMPORTANT: The code does NOT check to see if the new 
radius will result in accessing out-of-bounds values of the array. So if
used improperly, you could get funny results or crash the sensor (needing
a power cycle restart). Example- to set the radius of binarization channel 0 
to 3, send command = 50003.

6ffff -> FFScalar = ffff * 0.01. This changes the value of FFScalar. For
example, to set FFScalar to 7.5, send command = 60750.
*******************************************************************************/
void HorridgeTemplate::QuickUpdateParams(unsigned int command)
{
	int i1,i2,i3;
	switch (command / 10000) {
	case 1: // 10aaa: set HT.BCActive to aaa
		i1 = command % 10000;
		BCActive = i1 & 0x000000FF;
		break;
	case 2: // 2ccaa: set HT.CTAlpha[cc] = aa * 0.01, also CTAlphaOM
		i1 = command % 10000;
		i2 = i1 / 100; // i2=cc
		i3 = i1 % 100; // i3=aa
		if (i2 < HT_MAX_CAPTYPE) {
			CTAlpha[i2] = i3 * 0.01;
			CTAlphaOM[i2] = 1.0 - i3 * 0.01;
		}
		break;
	case 3: // 3bttt: set binarization channel b to type ttt
		i1 = command % 10000;
		i2 = i1 / 1000; // i2=b
		i3 = i1 % 1000; // i3=ttt
		if (i2 < 8)
			BCType[i2] = i3;
		break;
	case 4: // 4bttt: set binarization channel b threshold to ttt-500
		i1 = command % 10000;
		i2 = i1 / 1000; // i2=b
		i3 = i1 % 1000; // i3=ttt
		if (i2 < 8)
			BCThreshold[i2] = (i3 - 500);
		break;		
	case 5: // 5brrr: set binarization channel bbb radius to rrr
		i1 = command % 10000;
		i2 = i1 / 1000; // i2=b
		i3 = i1 % 1000; // i3=rrr
		if (i2 < 8)
			BCRadius[i2] = i3;
		break;	
	case 6: // 6ffff: set FFScalar
		FFScalar = (command % 10000) * 0.01;
		break;
	}	
}


/*******************************************************************************
MakeIntegralImages
Makes integral images from acquires source image

Inputs:	none
*******************************************************************************/
void HorridgeTemplate::MakeIntegralImages(void)
{
	short r,c,ndx;
	int ival;
	
	// compute row integral image
	for (r = 0; r < HT_ROWS; ++r) {
		ndx = r * HT_COLS; 
		ival = pImgIntRow[ndx] = pImgSource[ndx];
		for (c = 1; c < HT_COLS; ++c) {
			ndx++;
			ival += pImgSource[ndx];
			pImgIntRow[ndx] = ival;
		}
	}

	// compute col integral image
	for (c = 0; c < HT_COLS; ++c) {
		ndx = c; 
		ival = pImgIntCol[ndx] = pImgSource[ndx];
		for (r = 1; r < HT_ROWS; ++r) {
			ndx += HT_COLS;
			ival += pImgSource[ndx];
			pImgIntCol[ndx] = ival;
		}
	}
}

/*******************************************************************************
ComputeBinaryImages
Computes binary images

Inputs:	none
*******************************************************************************/
void HorridgeTemplate::ComputeBinaryImages(void)
{
	int featval, featval2; // value of a feature
	short channel; // channel number
	short line; // line number
	short featrow; // feature row- this indexes individual lines and changes orientation based on whether these are row or column features
	short featcol; // feature column
	short ndx; // pixel index
	short radiusoffsetminus, radiusoffsetplus; // offsets used to account for rect pixel radius
	unsigned char channelmask; // mask used to identify channel
	
	// loop through binary pixels to copy new to old and clear new
	for (ndx=0; ndx<HT_NUMPIXELS; ++ndx) {
		pImgBinOld[ndx] = pImgBinNew[ndx];
		pImgBinNew[ndx] = 0;
	}
		
	// loop through channels and compute features and binary values
	for (channel = 0; channel < HT_MAX_BINCHANNELS; ++channel) {
		channelmask = 0x01 << channel;
		if (BCActive & channelmask) { // check to see if this channel is active
			// extract feature information
			bool isrowtype = (BCType[channel] & HT_CHANNEL_IS_ROW);
			bool iscoltype = (BCType[channel] & HT_CHANNEL_IS_COL);
			unsigned char feattype = (BCType[channel] & HT_CHANNEL_FEAT_EXTRACTMASK);
			// if a row-type feature, do here
			if (isrowtype) {
				radiusoffsetminus = (BCRadius[channel] + 1) * HT_COLS;
				radiusoffsetplus = (BCRadius[channel]) * HT_COLS;
				for (line = 0; line < BCLineNum[channel]; ++line) { // loop through lines
					featrow = BCLineStart[channel] + line * BCLineSkip[channel]; // compute feature row
					// compute scratch space
					for (featcol = 0; featcol < HT_COLS; ++featcol) {
						ndx = featrow * HT_COLS + featcol;
						Scratch[featcol] = pImgIntCol[ndx + radiusoffsetplus] - pImgIntCol[ndx - radiusoffsetminus];
					}
					for (featcol = BCPixelStart[channel]; featcol <= BCPixelStop[channel]; ++featcol) { // loop through pixels of a feature row
						// compute pixel index
						ndx = featrow * HT_COLS + featcol;
						// the case statement below selects a method of computing a feature
						switch (feattype) {
						case HT_CHANNEL_FEAT_1_PN:
							featval = Scratch[featcol]; // P
							featval -= Scratch[featcol + 1]; // N
							featval2 = featval;
							break;
						case HT_CHANNEL_FEAT_2_PPNN:
							featval = Scratch[featcol - 1]; // P
							featval += Scratch[featcol]; // P
							featval -= Scratch[featcol + 1]; // N
							featval -= Scratch[featcol + 2]; // N
							break;
						case HT_CHANNEL_FEAT_3_PPPNNN:
							featval = Scratch[featcol - 2]; // P
							featval += Scratch[featcol - 1]; // P
							featval += Scratch[featcol]; // P
							featval -= Scratch[featcol + 1]; // N
							featval -= Scratch[featcol + 2]; // N
							featval -= Scratch[featcol + 3]; // N
							break;	
						case HT_CHANNEL_FEAT_4_P2NP:
							featval = Scratch[featcol - 1]; // P
							featval -= 2 * Scratch[featcol]; // 2N
							featval += Scratch[featcol + 1]; // P
							break;	
						case HT_CHANNEL_FEAT_5_PNNP:
							featval = Scratch[featcol - 1]; // P
							featval -= Scratch[featcol]; // N
							featval -= Scratch[featcol + 1]; // N
							featval += Scratch[featcol + 2]; // P
							break;		
						case HT_CHANNEL_FEAT_6_PP2N2NPP:
							featval = Scratch[featcol - 2]; // P
							featval = Scratch[featcol - 1]; // P
							featval -= 2 * Scratch[featcol]; // 2N
							featval -= 2 * Scratch[featcol + 1]; // 2N
							featval += Scratch[featcol + 2]; // P
							featval += Scratch[featcol + 3]; // P
							break;		
						case HT_CHANNEL_FEAT_7_P2N2PN:
							featval = Scratch[featcol - 1]; // P
							featval -= 2 * Scratch[featcol]; // 2N
							featval += 2 * Scratch[featcol + 1]; // 2P
							featval -= Scratch[featcol + 2]; // N
							break;		
						case HT_CHANNEL_FEAT_8_PNNPPN:
							featval = Scratch[featcol - 2]; // P
							featval -= Scratch[featcol - 1]; // N
							featval -= Scratch[featcol]; // N
							featval += Scratch[featcol + 1]; // P
							featval += Scratch[featcol + 2]; // P
							featval -= Scratch[featcol + 3]; // N
							break;	
						case HT_CHANNEL_FEAT_9_PP2N2N2P2PNN:
							featval = Scratch[featcol - 3]; // P
							featval += Scratch[featcol - 2]; // P
							featval -= 2 * Scratch[featcol - 1]; // 2N
							featval -= 2 * Scratch[featcol]; // 2N
							featval += 2 * Scratch[featcol + 1]; // 2P
							featval += 2 * Scratch[featcol + 2]; // 2P
							featval -= Scratch[featcol + 3]; // N
							featval -= Scratch[featcol + 4]; // N
							break;								
						// we will add the other features later
						default:
							featval = 0;
						} // switch
						// binarize the feature
						if (featval >= BCThreshold[channel]) // if feature value is greater than threshold...
						    pImgBinNew[ndx] |= channelmask; // set corresponding bit high
					} // featcol
				} // line
			} // if isrowtype
			// if column type then do here
			if (iscoltype) {
				radiusoffsetminus = BCRadius[channel] + 1;
				radiusoffsetplus = BCRadius[channel];
				for (line = 0; line < BCLineNum[channel]; ++line) { // loop through lines
					featrow = BCLineStart[channel] + line * BCLineSkip[channel]; // compute feature row
					// compute scratch space
					for (featcol = 0; featcol < HT_ROWS; ++featcol) {
						ndx = featcol * HT_COLS + featrow;
						Scratch[featcol] = pImgIntRow[ndx + radiusoffsetplus] - pImgIntRow[ndx - radiusoffsetminus];
					}
					for (featcol = BCPixelStart[channel]; featcol <= BCPixelStop[channel]; ++featcol) { // loop through pixels of a feature row
						// compute pixel index
						ndx = featcol * HT_COLS + featrow;
						// the case statement below selects a method of computing a feature
						switch (feattype) {
						case HT_CHANNEL_FEAT_1_PN:
							featval = Scratch[featcol]; // P
							featval -= Scratch[featcol + 1]; // N
							featval2 = featval;
							break;
						case HT_CHANNEL_FEAT_2_PPNN:
							featval = Scratch[featcol - 1]; // P
							featval += Scratch[featcol]; // P
							featval -= Scratch[featcol + 1]; // N
							featval -= Scratch[featcol + 2]; // N
							break;
						case HT_CHANNEL_FEAT_3_PPPNNN:
							featval = Scratch[featcol - 2]; // P
							featval += Scratch[featcol - 1]; // P
							featval += Scratch[featcol]; // P
							featval -= Scratch[featcol + 1]; // N
							featval -= Scratch[featcol + 2]; // N
							featval -= Scratch[featcol + 3]; // N
							break;	
						case HT_CHANNEL_FEAT_4_P2NP:
							featval = Scratch[featcol - 1]; // P
							featval -= 2 * Scratch[featcol]; // 2N
							featval += Scratch[featcol + 1]; // P
							break;	
						case HT_CHANNEL_FEAT_5_PNNP:
							featval = Scratch[featcol - 1]; // P
							featval -= Scratch[featcol]; // N
							featval -= Scratch[featcol + 1]; // N
							featval += Scratch[featcol + 2]; // P
							break;		
						case HT_CHANNEL_FEAT_6_PP2N2NPP:
							featval = Scratch[featcol - 2]; // P
							featval = Scratch[featcol - 1]; // P
							featval -= 2 * Scratch[featcol]; // 2N
							featval -= 2 * Scratch[featcol + 1]; // 2N
							featval += Scratch[featcol + 2]; // P
							featval += Scratch[featcol + 3]; // P
							break;		
						case HT_CHANNEL_FEAT_7_P2N2PN:
							featval = Scratch[featcol - 1]; // P
							featval -= 2 * Scratch[featcol]; // 2N
							featval += 2 * Scratch[featcol + 1]; // 2P
							featval -= Scratch[featcol + 2]; // N
							break;		
						case HT_CHANNEL_FEAT_8_PNNPPN:
							featval = Scratch[featcol - 2]; // P
							featval -= Scratch[featcol - 1]; // N
							featval -= Scratch[featcol]; // N
							featval += Scratch[featcol + 1]; // P
							featval += Scratch[featcol + 2]; // P
							featval -= Scratch[featcol + 3]; // N
							break;	
						case HT_CHANNEL_FEAT_9_PP2N2N2P2PNN:
							featval = Scratch[featcol - 3]; // P
							featval += Scratch[featcol - 2]; // P
							featval -= 2 * Scratch[featcol - 1]; // 2N
							featval -= 2 * Scratch[featcol]; // 2N
							featval += 2 * Scratch[featcol + 1]; // 2P
							featval += 2 * Scratch[featcol + 2]; // 2P
							featval -= Scratch[featcol + 3]; // N
							featval -= Scratch[featcol + 4]; // N
							break;								
						// we will add the other features later
						default:
							featval = 0;
						} // switch
						// binarize the feature
						if (featval >= BCThreshold[channel]) // if feature value is greater than threshold...
						    pImgBinNew[ndx] |= channelmask; // set corresponding bit high
					} // featcol
				} // line
			} // if iscoltype			
		} // if channel active
	} // channel
}

/*******************************************************************************
ComputeStimulations
Computes stimulations

Inputs:	none
*******************************************************************************/
void HorridgeTemplate::ComputeStimulations(void)
{
	short channel; // channel number
	short line; // line number
	short featrow; // feature row- this indexes individual lines and changes orientation based on whether these are row or column features
	short featcol; // feature column
	short ndx; // pixel index
	unsigned char templatetype; // template type
	unsigned char channelmask; // mask used to identify channel	
			
	// loop through stimultions and clear
	for (ndx=0; ndx<HT_NUMPIXELS; ++ndx) 
		pImgStimIntX[ndx] = pImgStimIntY[ndx] = 0;
		
	// loop through channels and compute template stimulations
	for (channel = 0; channel < HT_MAX_BINCHANNELS; ++channel) {
		channelmask = 0x01 << channel;
		if (BCActive & channelmask) { // check to see if this channel is active
			// extract feature information
			bool isrowtype = (BCType[channel] & HT_CHANNEL_IS_ROW);
			bool iscoltype = (BCType[channel] & HT_CHANNEL_IS_COL);
			unsigned char feattype = (BCType[channel] & HT_CHANNEL_FEAT_EXTRACTMASK);
			// if a row-type feature, do here
			if (isrowtype) {
				for (line = 0; line < BCLineNum[channel]; ++line) { // loop through lines
					featrow = BCLineStart[channel] + line * BCLineSkip[channel]; // compute feature row
					for (featcol = BCPixelStart[channel]; featcol <= BCPixelStop[channel]; ++featcol) { // loop through pixels of a feature row
						// compute pixel index
						ndx = featrow * HT_COLS + featcol;
						// compute template type
						templatetype = 0;
						if (pImgBinOld[ndx] & channelmask)
							templatetype += 8;
						if (pImgBinOld[ndx+1] & channelmask)
							templatetype += 4;
						if (pImgBinNew[ndx] & channelmask)
							templatetype += 2;
						if (pImgBinNew[ndx+1] & channelmask)
							templatetype += 1;
						// compute template stimulation
						pImgStimIntX[ndx] += T2[templatetype];
					} // featcol
				} // line
			} // if isrowtype
			// if a col-type feature, do here
			if (iscoltype) {
				for (line = 0; line < BCLineNum[channel]; ++line) { // loop through lines
					featrow = BCLineStart[channel] + line * BCLineSkip[channel]; // compute feature row
					for (featcol = BCPixelStart[channel]; featcol <= BCPixelStop[channel]; ++featcol) { // loop through pixels of a feature row
						// compute pixel index
						ndx = featcol * HT_COLS + featrow;
						// compute template type
						templatetype = 0;
						if (pImgBinOld[ndx] & channelmask)
							templatetype += 8;
						if (pImgBinOld[ndx+HT_COLS] & channelmask)
							templatetype += 4;
						if (pImgBinNew[ndx] & channelmask)
							templatetype += 2;
						if (pImgBinNew[ndx+HT_COLS] & channelmask)
							templatetype += 1;
						// compute template stimulation
						pImgStimIntY[ndx] += T2[templatetype];
					} // featcol
				} // line
			} // if iscoltype
		} // if channel active
	} // channel
						    
	// Loop through stimulations and turn into integral images
	// TODO we can just loop through pixels themselves in one loop
	for (featcol = 0; featcol < HT_COLS; featcol++) {
		for (featrow = 1; featrow < HT_ROWS; featrow++) {
			ndx = featrow * HT_COLS + featcol; // compute index
			pImgStimIntX[ndx] += pImgStimIntX[ndx-HT_COLS]; // add previous row of integral image
			pImgStimIntY[ndx] += pImgStimIntY[ndx-HT_COLS]; // add previous row of integral image
		}
	}
}

/*******************************************************************************
StimCaps
Stimulate and leak capacitors

Inputs:	none
*******************************************************************************/
void HorridgeTemplate::StimCaps(void)
{
	short capacitor; // capacitor index
	short col; // column
	short ndx1, ndx2; // indices
	short nax,nay; // number of active binarization channels in each direction
	short i;
	float newchargex, newchargey; // new charge into capacitor this cycle
	float inax,inay; // inverse of the number of active binarization channels

	
	// compute number of active binarization channels
	nax = nay = 0;
	for (i=0; i<HT_MAX_BINCHANNELS; ++i) {
		if (BCActive & (0x00000001 << i)) {
			if (BCType[i] & HT_CHANNEL_IS_ROW)
				nax++;
			if (BCType[i] & HT_CHANNEL_IS_COL)
				nay++;
		}
	}
	inax = 1.0 / (nax + 0.000001);
	inay = 1.0 / (nay + 0.000001);
	
	for (capacitor = 0; capacitor < CapNum; ++capacitor) {	
		// if inactive, continue
		if (CapType[capacitor]<0)
			continue;
		
		// compute charge to add to capacitor
		newchargex = newchargey = 0.0;
		for (col = CapC1[capacitor]; col<= CapC2[capacitor]; ++col) {
			// compute indices
			ndx1 = (CapR1[capacitor] - 1) * HT_COLS + col;
			ndx2 = CapR2[capacitor] * HT_COLS + col;
			// add to charge
			newchargex += pImgStimIntX[ndx2] - pImgStimIntX[ndx1];
			newchargey += pImgStimIntY[ndx2] - pImgStimIntY[ndx1];
		}
		
		// rescale charge
		newchargex *= CapInvArea[capacitor] * inax;
		newchargey *= CapInvArea[capacitor] * inay;	
		
		// update capacitor based on rescaled charge
		CapQX[capacitor] = CTAlphaOM[CapType[capacitor]] * CapQX[capacitor] + CTAlpha[CapType[capacitor]] * newchargex;	
		CapQY[capacitor] = CTAlphaOM[CapType[capacitor]] * CapQY[capacitor] + CTAlpha[CapType[capacitor]] * newchargey;	
	}
}
	

/*******************************************************************************
MakeSimulatedImage
generate synthetic images - this was used for debugging during development

Inputs:	none
*******************************************************************************/
void HorridgeTemplate::MakeSimulatedImage(short method, float dispx, float dispy)
{
	short row,col;
	short ndx;
	short imval;
	short stimcol, stimrow;
	int idispx = int(dispx);
	int idispy = int(dispy);
	
	switch (method) {
	case 1: // whole image motion
		for (row = 0; row < HT_ROWS; ++row)
			for (col = 0; col < HT_COLS; ++col) {
				ndx = row * HT_COLS + col;
				bool rowhappy = (((row - idispy) & 0x00000003) <= 1);
				bool colhappy = (((col - idispx) & 0x00000003) <= 1);
				imval = (rowhappy && colhappy) ? 10 : 0;
//				if (imval)
//					imval += (col % 2);
				pImgSource[ndx] = imval;
			}
		break;
	case 2: // partial image motion
		for (row = 10; row < 20; ++row)
			for (col = 40; col < 50; ++col) {
				ndx = row * HT_COLS + col;
				bool rowhappy = (((row - idispy) & 0x00000003) <= 1);
				bool colhappy = (((col - idispx) & 0x00000003) <= 1);
				imval = (rowhappy && colhappy) ? 10 : 0;
//				if (imval)
//					imval += (col % 2);				
				pImgSource[ndx] = imval;
			}
		break;		
	case 3: // swooping sprite, mostly X
		stimcol = idispx % HT_COLS;
		stimrow = (idispx / 15) % HT_ROWS;
		for (row = 0; row < HT_ROWS; ++row)
			for (col = 0; col < HT_COLS; ++col) {
				ndx = row * HT_COLS + col;
				if (row==stimrow && col==stimcol)
					pImgSource[ndx] = 10;
				else
					pImgSource[ndx] = 0;
			}
		break;
	case 4: // swooping sprite, mostly Y
		stimcol = (idispx / 20) % HT_COLS;
		stimrow = idispx % HT_ROWS;
		for (row = 0; row < HT_ROWS; ++row)
			for (col = 0; col < HT_COLS; ++col) {
				ndx = row * HT_COLS + col;
				if (row==stimrow && col==stimcol)
					pImgSource[ndx] = 10;
				else
					pImgSource[ndx] = 0;
			}
		break;		
	}
}
	
/*******************************************************************************
DumpArrays
Dumps arrays - this was used for debugging and development

Inputs:	flags to tell the function what to do
*******************************************************************************/
void HorridgeTemplate::DumpArrays(unsigned char flags)
{
	short cap;
	short ndx;
	
	if (flags & HT_DUMP_SOURCE)
		pIMG->DumpShortImage(pImgSource,HT_ROWS,HT_COLS,"HTSource");
	if (flags & HT_DUMP_INT) {
		pIMG->DumpIntImage(pImgIntRow,HT_ROWS,HT_COLS,"HTIntRow");
		pIMG->DumpIntImage(pImgIntCol,HT_ROWS,HT_COLS,"HTIntCol");
	}
	if (flags & HT_DUMP_BINNEW)
		pIMG->DumpUCImage(pImgBinNew,HT_ROWS,HT_COLS,"HTBinNew");
	if (flags & HT_DUMP_BINOLD)
		pIMG->DumpUCImage(pImgBinOld,HT_ROWS,HT_COLS,"HTBinOld");
	if (flags & HT_DUMP_STIMX)
		pIMG->DumpShortImage(pImgStimIntX,HT_ROWS,HT_COLS,"HTStimX");
	if (flags & HT_DUMP_STIMY)
		pIMG->DumpShortImage(pImgStimIntY,HT_ROWS,HT_COLS,"HTStimY");
	if (flags & HT_DUMP_CAP) {
		myprintf("CapInfo = [\n");
		for (cap=0; cap<CapNum; ++cap) 
			myprintf("%d %d %d %d %d %f\n",CapType[cap],CapR1[cap],CapR2[cap],CapC1[cap],CapC2[cap]);
		myprintf("];\n");	
		myprintf("CapQ = [");
		for (cap=0; cap<CapNum; ++cap) {
			myprintf(" %f %f",CapQX[cap],CapQY[cap]);
		}
		myprintf("];\n");	
	}
	if (flags & HT_DUMP_BC) {
		myprintf("BCInfo = [\n");
		for (cap=0; cap<HT_MAX_BINCHANNELS; ++cap) 
			myprintf("%d %d %d %d %d %d %d %d\n",
				 (short)BCType[cap],
				 (short)BCLineStart[cap],
				 (short)BCLineSkip[cap],
				 (short)BCLineNum[cap],
				 (short)BCRadius[cap],
				 (short)BCPixelStart[cap],
				 (short)BCPixelStop[cap],
				 BCThreshold[cap]);
		myprintf("];\n");
		myprintf("BCActive = %d;\n",(short)BCActive);
	}
}