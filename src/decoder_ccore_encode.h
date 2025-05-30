//
// Prioriy encoder module for GRANDAB3
//

#ifndef DECODER_CCORE_ENCODE_H
#define DECODER_CCORE_ENCODE_H

#include "defs.h"

//Prioriy encoder module
class decoder_ccore_encode
{

private:

public:

    decoder_ccore_encode() {};

    #pragma hls_design interface ccore
    void CCS_BLOCK(run)(decoder_n_t reduced, decoder_idx_t &idx, bool &found)
    {
        found = 0;
        idx = 0;

        //Codebook membership check
        if(reduced[0] == 1){
            idx = 0;
            found = 1;
        }else if(reduced[1] == 1){
            idx = 1;
            found = 1;
        }else if(reduced[2] == 1){
            idx = 2;
            found = 1;
        }else if(reduced[3] == 1){
            idx = 3;
            found = 1;
        }else if(reduced[4] == 1){
            idx = 4;
            found = 1;
        }else if(reduced[5] == 1){
            idx = 5;
            found = 1;
        }else if(reduced[6] == 1){
            idx = 6;
            found = 1;
        }else if(reduced[7] == 1){
            idx = 7;
            found = 1;
        }else if(reduced[8] == 1){
            idx = 8;
            found = 1;
        }else if(reduced[9] == 1){
            idx = 9;
            found = 1;
        }else if(reduced[10] == 1){
            idx = 10;
            found = 1;
        }else if(reduced[11] == 1){
            idx = 11;
            found = 1;
        }else if(reduced[12] == 1){
            idx = 12;
            found = 1;
        }else if(reduced[13] == 1){
            idx = 13;
            found = 1;
        }else if(reduced[14] == 1){
            idx = 14;
            found = 1;
        }else if(reduced[15] == 1){
            idx = 15;
            found = 1;
        }else if(reduced[16] == 1){
            idx = 16;
            found = 1;
        }else if(reduced[17] == 1){
            idx = 17;
            found = 1;
        }else if(reduced[18] == 1){
            idx = 18;
            found = 1;
        }else if(reduced[19] == 1){
            idx = 19;
            found = 1;
        }else if(reduced[20] == 1){
            idx = 20;
            found = 1;
        }else if(reduced[21] == 1){
            idx = 21;
            found = 1;
        }else if(reduced[22] == 1){
            idx = 22;
            found = 1;
        }else if(reduced[23] == 1){
            idx = 23;
            found = 1;
        }else if(reduced[24] == 1){
            idx = 24;
            found = 1;
        }else if(reduced[25] == 1){
            idx = 25;
            found = 1;
        }else if(reduced[26] == 1){
            idx = 26;
            found = 1;
        }else if(reduced[27] == 1){
            idx = 27;
            found = 1;
        }else if(reduced[28] == 1){
            idx = 28;
            found = 1;
        }else if(reduced[29] == 1){
            idx = 29;
            found = 1;
        }else if(reduced[30] == 1){
            idx = 30;
            found = 1;
        }else if(reduced[31] == 1){
            idx = 31;
            found = 1;
        }else if(reduced[32] == 1){
            idx = 32;
            found = 1;
        }else if(reduced[33] == 1){
            idx = 33;
            found = 1;
        }else if(reduced[34] == 1){
            idx = 34;
            found = 1;
        }else if(reduced[35] == 1){
            idx = 35;
            found = 1;
        }else if(reduced[36] == 1){
            idx = 36;
            found = 1;
        }else if(reduced[37] == 1){
            idx = 37;
            found = 1;
        }else if(reduced[38] == 1){
            idx = 38;
            found = 1;
        }else if(reduced[39] == 1){
            idx = 39;
            found = 1;
        }else if(reduced[40] == 1){
            idx = 40;
            found = 1;
        }else if(reduced[41] == 1){
            idx = 41;
            found = 1;
        }else if(reduced[42] == 1){
            idx = 42;
            found = 1;
        }else if(reduced[43] == 1){
            idx = 43;
            found = 1;
        }else if(reduced[44] == 1){
            idx = 44;
            found = 1;
        }else if(reduced[45] == 1){
            idx = 45;
            found = 1;
        }else if(reduced[46] == 1){
            idx = 46;
            found = 1;
        }else if(reduced[47] == 1){
            idx = 47;
            found = 1;
        }else if(reduced[48] == 1){
            idx = 48;
            found = 1;
        }else if(reduced[49] == 1){
            idx = 49;
            found = 1;
        }else if(reduced[50] == 1){
            idx = 50;
            found = 1;
        }else if(reduced[51] == 1){
            idx = 51;
            found = 1;
        }else if(reduced[52] == 1){
            idx = 52;
            found = 1;
        }else if(reduced[53] == 1){
            idx = 53;
            found = 1;
        }else if(reduced[54] == 1){
            idx = 54;
            found = 1;
        }else if(reduced[55] == 1){
            idx = 55;
            found = 1;
        }else if(reduced[56] == 1){
            idx = 56;
            found = 1;
        }else if(reduced[57] == 1){
            idx = 57;
            found = 1;
        }else if(reduced[58] == 1){
            idx = 58;
            found = 1;
        }else if(reduced[59] == 1){
            idx = 59;
            found = 1;
        }else if(reduced[60] == 1){
            idx = 60;
            found = 1;
        }else if(reduced[61] == 1){
            idx = 61;
            found = 1;
        }else if(reduced[62] == 1){
            idx = 62;
            found = 1;
        }else if(reduced[63] == 1){
            idx = 63;
            found = 1;
        }else if(reduced[64] == 1){
            idx = 64;
            found = 1;
        }else if(reduced[65] == 1){
            idx = 65;
            found = 1;
        }else if(reduced[66] == 1){
            idx = 66;
            found = 1;
        }else if(reduced[67] == 1){
            idx = 67;
            found = 1;
        }else if(reduced[68] == 1){
            idx = 68;
            found = 1;
        }else if(reduced[69] == 1){
            idx = 69;
            found = 1;
        }else if(reduced[70] == 1){
            idx = 70;
            found = 1;
        }else if(reduced[71] == 1){
            idx = 71;
            found = 1;
        }else if(reduced[72] == 1){
            idx = 72;
            found = 1;
        }else if(reduced[73] == 1){
            idx = 73;
            found = 1;
        }else if(reduced[74] == 1){
            idx = 74;
            found = 1;
        }else if(reduced[75] == 1){
            idx = 75;
            found = 1;
        }else if(reduced[76] == 1){
            idx = 76;
            found = 1;
        }else if(reduced[77] == 1){
            idx = 77;
            found = 1;
        }else if(reduced[78] == 1){
            idx = 78;
            found = 1;
        }else if(reduced[79] == 1){
            idx = 79;
            found = 1;
        }else if(reduced[80] == 1){
            idx = 80;
            found = 1;
        }else if(reduced[81] == 1){
            idx = 81;
            found = 1;
        }else if(reduced[82] == 1){
            idx = 82;
            found = 1;
        }else if(reduced[83] == 1){
            idx = 83;
            found = 1;
        }else if(reduced[84] == 1){
            idx = 84;
            found = 1;
        }else if(reduced[85] == 1){
            idx = 85;
            found = 1;
        }else if(reduced[86] == 1){
            idx = 86;
            found = 1;
        }else if(reduced[87] == 1){
            idx = 87;
            found = 1;
        }else if(reduced[88] == 1){
            idx = 88;
            found = 1;
        }else if(reduced[89] == 1){
            idx = 89;
            found = 1;
        }else if(reduced[90] == 1){
            idx = 90;
            found = 1;
        }else if(reduced[91] == 1){
            idx = 91;
            found = 1;
        }else if(reduced[92] == 1){
            idx = 92;
            found = 1;
        }else if(reduced[93] == 1){
            idx = 93;
            found = 1;
        }else if(reduced[94] == 1){
            idx = 94;
            found = 1;
        }else if(reduced[95] == 1){
            idx = 95;
            found = 1;
        }else if(reduced[96] == 1){
            idx = 96;
            found = 1;
        }else if(reduced[97] == 1){
            idx = 97;
            found = 1;
        }else if(reduced[98] == 1){
            idx = 98;
            found = 1;
        }else if(reduced[99] == 1){
            idx = 99;
            found = 1;
        }else if(reduced[100] == 1){
            idx = 100;
            found = 1;
        }else if(reduced[101] == 1){
            idx = 101;
            found = 1;
        }else if(reduced[102] == 1){
            idx = 102;
            found = 1;
        }else if(reduced[103] == 1){
            idx = 103;
            found = 1;
        }else if(reduced[104] == 1){
            idx = 104;
            found = 1;
        }else if(reduced[105] == 1){
            idx = 105;
            found = 1;
        }else if(reduced[106] == 1){
            idx = 106;
            found = 1;
        }else if(reduced[107] == 1){
            idx = 107;
            found = 1;
        }else if(reduced[108] == 1){
            idx = 108;
            found = 1;
        }else if(reduced[109] == 1){
            idx = 109;
            found = 1;
        }else if(reduced[110] == 1){
            idx = 110;
            found = 1;
        }else if(reduced[111] == 1){
            idx = 111;
            found = 1;
        }else if(reduced[112] == 1){
            idx = 112;
            found = 1;
        }else if(reduced[113] == 1){
            idx = 113;
            found = 1;
        }else if(reduced[114] == 1){
            idx = 114;
            found = 1;
        }else if(reduced[115] == 1){
            idx = 115;
            found = 1;
        }else if(reduced[116] == 1){
            idx = 116;
            found = 1;
        }else if(reduced[117] == 1){
            idx = 117;
            found = 1;
        }else if(reduced[118] == 1){
            idx = 118;
            found = 1;
        }else if(reduced[119] == 1){
            idx = 119;
            found = 1;
        }else if(reduced[120] == 1){
            idx = 120;
            found = 1;
        }else if(reduced[121] == 1){
            idx = 121;
            found = 1;
        }else if(reduced[122] == 1){
            idx = 122;
            found = 1;
        }else if(reduced[123] == 1){
            idx = 123;
            found = 1;
        }else if(reduced[124] == 1){
            idx = 124;
            found = 1;
        }else if(reduced[125] == 1){
            idx = 125;
            found = 1;
        }else if(reduced[126] == 1){
            idx = 126;
            found = 1;
        }
    }
};

/*
//Priority encoder
class decoder_ccore_encode
{

private:

public:

    decoder_ccore_encode() {};

    #pragma hls_design interface ccore
    void CCS_BLOCK(run)(decoder_n_t reduced, decoder_idx_t &idx, bool &found)
    {
        found = 0;
        idx = 0;

        //Codebook membership check
        #pragma hls_unroll yes
        ENCODE: for(int i=0; i<N; i++)
        {
            if(reduced[i] == 1)
            {
                idx = i;
                found = true;
                break;
            }
        }
    }
};
*/

#endif 
