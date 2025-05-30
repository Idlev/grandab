//
// GRANDAB (AB=3) decoder golden reference
//

#ifndef REF_DECODER_H
#define REF_DECODER_H

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

#include "defs.h"

class ref_decoder
{

private:

    //
    int H_data[N][M];           //Matrix from file
    std::bitset<N> H_mtx[M];    //Matrix as bitset
    std::bitset<M> synd;        //Syndrome
    std::bitset<N> hd;          //Hard-decision
    std::bitset<N> temp;        //hd modified with TEPs
    int error;                  //Syndrome error count

    //Read parity-check matrix from file
    void readSyndmtx(const char* filename, int data[N][M])
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

        for(auto i=0; i<N; i++)
        {
            std::getline(inFile,line);

            for(auto j=0; j<M; j++)
            {   
                if(line[j] == '0')
                    data[i][j] = 0;
                else if(line[j] == '1')
                    data[i][j] = 1;
                else
                    std::cout << "Unexpected value!" << std::endl;
            }
        }

        inFile.close();
    }

    //Compute syndrome (s = temp*H) 
    void get_syndrome(std::bitset<N> H_mtx[M], std::bitset<N> temp, std::bitset<M> &synd)
    {
        for(auto i=0; i<M; i++)
            synd[i] = (temp & H_mtx[i]).count()%2;
    }

    //Syndrome checksum, number of set bits
    void check_sum(std::bitset<M> synd, int &error)
    {
        error = synd.count();
    }

public:

    ref_decoder()
    {
        //Read in parity-check matrix
        readSyndmtx("H_mtx.txt", H_data);

        //Matrix as columnwise bitset
        for(auto i=0; i<M; i++){
            for(auto j=0; j<N; j++){
                H_mtx[i][j] = H_data[j][i];
            }
        }
    }

    //Main decode function
    void decode(float awgn[N], std::bitset<N> &dec, unsigned int &n_flips, unsigned int &complexity)
    {
        //Hard-decision values
        for(auto i=0; i<N; i++)
        {
            if(awgn[i] >= 0){
                hd[i] = 0;
            }else{
                hd[i] = 1;
            }
        }

        //Syndrome without bit-flips
        error = 0;
        get_syndrome(H_mtx,hd,synd);
        check_sum(synd,error);   
        complexity++;

        //Match
        if(error == 0){
            dec = hd;
            return;
        }

        //TEPs HW=1
        for(auto i=0; i<N; i++)
        {
            temp = hd;
            error = 0;

            //Bit-flip(s)
            temp.flip(i);

            //Syndrome
            get_syndrome(H_mtx,temp,synd);
            check_sum(synd,error);
            complexity++;

            //Match
            if(error == 0)
            {
                //std::cout << "Match 1! REF idx: " << i << std::endl;
                dec = temp;
                
                return;
            }
        
        }//i

        //TEPs HW=2
        for(auto i=0; i<N-1; i++){
        for(auto j=i+1; j<N; j++)
        {
            temp = hd;
            error = 0;

            //Bit-flips
            temp.flip(i);
            temp.flip(j);

            //Syndrome
            get_syndrome(H_mtx,temp,synd);
            check_sum(synd,error);
            complexity++;

            //Match
            if(error == 0)
            {
                //std::cout << "Match 2! REF idx: " << i << "," << j << std::endl;
                dec = temp;
                
                return;
            }
        
        }//i
        }//j

        //TEPs HW=3
        for(auto i=0; i<N-2; i++){
        for(auto j=i+1; j<N-1; j++){
        for(auto k=j+1; k<N; k++)
        {
            temp = hd;
            error = 0;

            //Bit-flips
            temp.flip(i);
            temp.flip(j);
            temp.flip(k);

            //Syndrome
            get_syndrome(H_mtx,temp,synd);
            check_sum(synd,error);
            complexity++;

            //Match
            if(error == 0)
            {
                //std::cout << "Match 3! REF idx: " << i << "," << j << "," << k << std::endl;
                dec = temp;
                
                return;
            }

        }//i
        }//j
        }//k

        //No match
        dec = hd;

    }//decode

};

#endif
