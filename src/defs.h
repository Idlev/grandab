//
// Author: Tuomas Aaltonen - tuomas.aaltonen@tuni.fi 
//
// Bit-accurate datatypes
// System parameters
//

#ifndef DEFS_H
#define DEFS_H

#include "ac_int.h"
#include "ac_fixed.h"
#include "ac_channel.h"

//Code parameters
const int K = 106; //Message length
const int N = 127; //Codeword length
const int M = N-K; //Syndrome length
const float R = (float)K/(float)N; //Code rate

//Parallelization factor
//const int P = 1;               
//const int P = 2;
const int P = 4;
//const int P = 6;
//const int P = 8;

//Adjust according to factor P
//const int STEPS = 1+4032; //P=1
//const int STEPS = 1+2016; //P=2
const int STEPS = 1+1024; //P=4
//const int STEPS = 1+693;  //P=6
//const int STEPS = 1+528;  //P=8

//AC-datatypes
typedef ac_int<2,false> decoder_stage_t;    //Stage encoding
typedef ac_int<1,true> decoder_in_t;        //Input quantization
typedef ac_int<N,false> decoder_n_t;        //Bit-accurate with redundancy
typedef ac_int<M,false> decoder_m_t;        //Bit-accurate syndrome type

typedef ac_int<ac::log2_ceil<N>::val, false> decoder_idx_t;  //N-indexing

const int WIDTH = (P==1 ? 1 : ac::log2_ceil<P>::val); //P-encoder id
typedef ac_int<WIDTH, false> pid_type_t; 

//Input channel array type
struct inStruct_t
{
    decoder_in_t data[N];
};

//Type for encoder search tree
struct searchStruct_t
{
    pid_type_t p_id;
    bool f;
    decoder_idx_t idx;
};

#endif
