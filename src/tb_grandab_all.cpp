//
// Author: Tuomas Aaltonen - tuomas.aaltonen@tuni.fi
//
// GRANDAB (AB=3) decoder testbench
// All test vector error injections up to 3bits
//
// BCH(127,106)
//

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <cmath>
#include <bitset>

#include "defs.h"
#include "ref_grandab.h"
#include "decoder.h"

//Read generator matrix from file
void readGenmtx(const char *filename, int mtx[K][N]);

CCS_MAIN(int argc, char *argv[])
{
    //Pesudorandom number generator
    std::random_device rd_seed;                         //Random number seed
    std::mt19937 gen(rd_seed());                        //Seed generator (Mersenne Twister)
    std::uniform_int_distribution<> unif_distr(0,1);    //Uniform distribution int [0,1]
    std::normal_distribution<float> norm_distr{0,1};    //Normal distribution (0 mean, 1 variance)
    
    //DUT
    decoder_class DUT;                  //DUT instance
    ac_channel<inStruct_t> dataIn_ch;   //Channels
    ac_channel<decoder_n_t> dataOut_ch;
    ac_channel<decoder_m_t> scDebug_ch;
    inStruct_t data_in;                 //Channel data
    decoder_n_t data_out;
    decoder_m_t sc_data_out;

    //Golden ref
    ref_decoder decoder_inst;

    //Decoded message
    std::bitset<N> dec;

    //Generator matrix data
    int G_data[K][N];
    readGenmtx("G_mtx.txt", G_data);

    //G as bitset
    std::bitset<K> G_mtx[N]; 
    for(auto i=0; i<N; i++){
        for(auto j=0; j<K; j++){
            G_mtx[i][j] = G_data[j][i];
        }
    }

    //Output file for results
    FILE *outFile;
    outFile = fopen("output_all.txt","w");
    if(!outFile)
    {
        std::cout << "output file did not open" << std::endl;
    }

    //===================//
    //One bit flip errors
    //===================//

    //Design runs
    for(int i1=0; i1<N; i1++)
    {

    unsigned int n_flips = 0;
    unsigned int complexity = 0;

    //Random message of length K
    std::bitset<K> msg;
    for(int i=0; i<K; i++)
    {
        msg[i] = unif_distr(gen); // [0,1] uniform distribution
    }

    //Message encode
    std::bitset<N> cw;
    for(auto i=0; i<N; i++)
        cw[i] = (msg & G_mtx[i]).count()%2;

    //Modulation
    int data_BPSK[N]; //Binary Phase Shift Keying (BPSK)
    for(int i=0; i<N; i++)
    {
        if(cw[i] == 0)
            data_BPSK[i] = 1;
        else
            data_BPSK[i] = -1;
    }

    //Error injection
    if(data_BPSK[i1] == -1)
        data_BPSK[i1] = 1;
    else if(data_BPSK[i1] == 1)
        data_BPSK[i1] = -1;

    //No Additive Noise
    float data_AWGN[N];
    for(int i=0; i<N; i++)
    {
        data_AWGN[i] = data_BPSK[i];
        data_in.data[i] = std::signbit(data_AWGN[i]);
        //data_in.data[i] = data_AWGN[i];
    }

    //Reset between decoder calls
    dec.reset();

    //Write DUT in
    dataIn_ch.write(data_in);

    //Run decoders
    decoder_inst.decode(data_AWGN,dec,n_flips,complexity);      //Golden ref
    DUT.run(dataIn_ch,dataOut_ch);                              //DUT
    data_out = dataOut_ch.read();                               //Read DUT output

    //Compare outputs
    for(int i=0; i<N; i++)
    {
        if(dec[i] != data_out[i]){
            std::cout << "incorrect bit!: " << i << std::endl;
			fprintf(outFile, "Incorrect bit: %d \n",i);
        }
    }

    }//i1

    //===================//
    //Two bit-flip errors
    //===================//

    //Design runs
    for(int i1=0; i1<N-1; i1++){
    for(int j2=i1+1; j2<N; j2++)
    {

    unsigned int n_flips = 0;
    unsigned int complexity = 0;

    //Random binary message of length K
    std::bitset<K> msg;
    for(int i=0; i<K; i++)
    {
        msg[i] = unif_distr(gen); // [0,1] uniform distribution
    }

    //Message encode
    std::bitset<N> cw;
    for(auto i=0; i<N; i++)
        cw[i] = (msg & G_mtx[i]).count()%2;

    //Modulation
    int data_BPSK[N]; //Binary Phase Shift Keying (BPSK)
    for(int i=0; i<N; i++)
    {
        if(cw[i] == 0)
            data_BPSK[i] = 1;
        else
            data_BPSK[i] = -1;
    }

    //Error injection
    if(data_BPSK[i1] == -1)
        data_BPSK[i1] = 1;
    else if(data_BPSK[i1] == 1)
        data_BPSK[i1] = -1;

    if(data_BPSK[j2] == -1)
        data_BPSK[j2] = 1;
    else if(data_BPSK[j2] == 1)
        data_BPSK[j2] = -1;

    //No Additive Noise
    float data_AWGN[N];
    for(int i=0; i<N; i++)
    {
        data_AWGN[i] = data_BPSK[i];
        data_in.data[i] = std::signbit(data_AWGN[i]);
        //data_in.data[i] = data_AWGN[i];
    }

    //Reset between decoder calls
    dec.reset();

    //Write DUT in
    dataIn_ch.write(data_in);

    //Run decoders
    decoder_inst.decode(data_AWGN,dec,n_flips,complexity);      //Golden ref
    DUT.run(dataIn_ch,dataOut_ch);                              //DUT
    data_out = dataOut_ch.read();                               //Read DUT output

    //Compare outputs
    for(int i=0; i<N; i++)
    {
        if(dec[i] != data_out[i]){
            std::cout << "incorrect bit!: " << i << std::endl;
			fprintf(outFile, "Incorrect bit: %d \n",i);
        }
    }

    }//i1
    }//j2

    //===================//
    //Three bit-flip errors
    //===================//

    //Design runs
    for(int i1=0; i1<N-2; i1++){
    for(int j2=i1+1; j2<N-1; j2++){
    for(int k3=j2+1; k3<N; k3++)
    {

    unsigned int n_flips = 0;
    unsigned int complexity = 0;

    //Random message of length K
    std::bitset<K> msg;
    for(int i=0; i<K; i++)
    {
        msg[i] = unif_distr(gen); // [0,1] uniform distribution
    }

    //Message encode
    std::bitset<N> cw;
    for(auto i=0; i<N; i++)
        cw[i] = (msg & G_mtx[i]).count()%2;

    //Modulation
    int data_BPSK[N]; //Binary Phase Shift Keying (BPSK)
    for(int i=0; i<N; i++)
    {
        if(cw[i] == 0)
            data_BPSK[i] = 1;
        else
            data_BPSK[i] = -1;
    }

    //Error injection
    if(data_BPSK[i1] == -1)
        data_BPSK[i1] = 1;
    else if(data_BPSK[i1] == 1)
        data_BPSK[i1] = -1;

    if(data_BPSK[j2] == -1)
        data_BPSK[j2] = 1;
    else if(data_BPSK[j2] == 1)
        data_BPSK[j2] = -1;

    if(data_BPSK[k3] == -1)
        data_BPSK[k3] = 1;
    else if(data_BPSK[k3] == 1)
        data_BPSK[k3] = -1;
    
    //No Additive Noise
    float data_AWGN[N];
    for(int i=0; i<N; i++)
    {
        data_AWGN[i] = data_BPSK[i];
        data_in.data[i] = std::signbit(data_AWGN[i]);
        //data_in.data[i] = data_AWGN[i];
    }

    //Reset between decoder calls
    dec.reset();

    //Write DUT in
    dataIn_ch.write(data_in);

    //Run decoders
    decoder_inst.decode(data_AWGN,dec,n_flips,complexity);      //Golden ref
    DUT.run(dataIn_ch,dataOut_ch);                              //DUT
    data_out = dataOut_ch.read();                               //Read DUT output

    //Compare outputs
    for(int i=0; i<N; i++)
    {
        if(dec[i] != data_out[i]){
            std::cout << "incorrect bit!: " << i << std::endl;
			fprintf(outFile, "Incorrect bit: %d \n",i);
        }
    }

    }//i1
    }//j2
    }//k3

    //std::cout << "done" << std::endl;

    fclose(outFile);
    CCS_RETURN(0); 
}


//Read generator matrix from file
void readGenmtx(const char *filename, int mtx[K][N])
{
    std::ifstream inFile;
    std::string line;

    const char* path = "../data/";
    std::string full_path = "";
    full_path += path;
    full_path += filename;
    const char* final = full_path.c_str();

    inFile.open(final);
    if(!inFile)
    {
        std::cout << "File " << final << " did not open!" << std::endl;
    }

    for(int i=0; i<K; i++)
    {
        std::getline(inFile,line);

        for(int j=0; j<N; j++)
        {   
            if(line[j] == '0')
                mtx[i][j] = 0;
            else if(line[j] == '1')
                mtx[i][j] = 1;
            else
                std::cout << "value error!" << std::endl;
        }
    }

    inFile.close();
}
