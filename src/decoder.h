//
// Author: Tuomas Aaltonen – tuomas.aaltonen@tuni.fi
//
// High-Level model of GRAND with ABandonment 
// (GRANDAB, AB=3) decoder
//
// Synthesizable with Catapult HLS
//

#ifndef DECODER_H
#define DECODER_H

#include <mc_scverify.h>

#include "defs.h"
#include "decoder_ccore_encode.h"
#include "search.h"

//NOR-reduction module
class decoder_ccore_reduce
{

private:

public:

    decoder_ccore_reduce() {};

    #pragma hls_design interface ccore
    void CCS_BLOCK(run)(decoder_m_t tep[N], decoder_n_t &reduced)
    {
        #pragma hls_unroll yes
        REDUCE: for(int i=0; i<N; i++)
        {
            reduced[i] = !tep[i].or_reduce();
        }
    }
};

//Cyclic shift module
class decoder_ccore_cshift
{

private:

public:

    decoder_ccore_cshift(){};

    #pragma hls_design interface ccore
    void CCS_BLOCK(cshift)(decoder_m_t regs[N], decoder_idx_t idxs[N], decoder_idx_t count)
    {
        decoder_m_t tmp_reg = regs[0];
        decoder_idx_t tmp_idx = idxs[0];

        #pragma hls_unroll yes
        for(int i=0; i<N; i++)
        {
            if(i == N-1-count){
                regs[i] = tmp_reg;
                idxs[i] = tmp_idx;
            }else if(i < N-1){
                regs[i] = regs[i+1];
                idxs[i] = idxs[i+1];
            }
        }
    }
};

//Up-shift module
class decoder_ccore_ushift
{

private:

public:

    decoder_ccore_ushift(){};

    #pragma hls_design interface ccore
    void CCS_BLOCK(ushift)(decoder_m_t regs[N], decoder_idx_t idxs[N])
    {

        #pragma hls_unroll yes
        for(int i=0; i<N; i++)
        {
            if(i < N-P)
            {
                regs[i] = regs[i+P];
                idxs[i] = idxs[i+P];
            }
        }
    }
};

//Peripheral circuitry module
class decoder_ccore_peripherals
{

private:

    //CCOREs
    decoder_ccore_reduce ccore_red;
    decoder_ccore_encode ccore_enc;

public:

    decoder_ccore_peripherals() {};

    #pragma hls_design interface ccore
    void CCS_BLOCK(run)(decoder_m_t tep_sr[N], decoder_m_t ctrl_tep_arr[P], decoder_idx_t idx_arr[P], bool found_arr[P])
    {
        //Peripheral logic replication
        #pragma hls_unroll yes
        REP: for(int j=0; j<P; j++)
        {
            decoder_m_t tep_par[N];
            #pragma hls_unroll yes
            for(int k=0; k<N; k++){
                tep_par[k] = ctrl_tep_arr[j]^tep_sr[k];
            }

            bool tmp_found;
            decoder_n_t reduced;
            decoder_idx_t tmp_idx;
            ccore_red.run(tep_par,reduced);
            ccore_enc.run(reduced,tmp_idx,tmp_found);                

            idx_arr[j] = tmp_idx;
            found_arr[j] = tmp_found;
        }
    }
};


//Decoder design block
#pragma design
class decoder_class
{

private:

    //CCOREs
    decoder_ccore_peripherals ccore_per;
    decoder_ccore_cshift ccore_cshift;
    decoder_ccore_ushift ccore_ushift1;
    decoder_ccore_ushift ccore_ushift2;

    //Parity-check matrix
    decoder_m_t H_mtx[N];

    //Stage
    decoder_stage_t curr_stage;

    //Compute syndrome of the received codeword
    void get_syndrome(decoder_n_t rec_cw, decoder_m_t &sc)
    {
        #pragma hls_unroll yes
        SYND: for(int i=0; i<M; i++)
        {
            decoder_n_t H_tmp;
            #pragma hls_unroll yes
            READ: for(int j=0; j<N; j++){
               H_tmp[j] = H_mtx[j][i];
            }

            decoder_n_t tmp = H_tmp & rec_cw;
            sc[i] = tmp.xor_reduce();
        }    
    }

    //Process TEPs with HW 1/2/3
    void process(decoder_m_t sc, decoder_idx_t &idxc, decoder_idx_t &idx1, decoder_idx_t &idx2, bool &found)
    {
        //Vars
        decoder_idx_t cyclic_count = 0;     //Cyclic shift count
        decoder_idx_t max_cyclic = 1;       //Maximum iteration count of given cycle (start with 1 for HW=1)
        decoder_idx_t up_count = 0;         //Up-shift count
        decoder_m_t ctrl_tep_arr[P];        //controller TEP syndrome array 
        decoder_idx_t idx_arr[P];           //Priority encoder index output(s)
        bool found_arr[P];                  //Priority encoder instance id(s)

        //Init control arrays
        #pragma hls_unroll yes
        INIT_ARR: for(int i=0; i<P; i++){
            ctrl_tep_arr[i] = 0;
            idx_arr[i] = 0;
            found_arr[i] = 0;
        }

        //Register arrays
        decoder_m_t sr1_data[N];
        decoder_m_t sr2_data[N];
        decoder_idx_t sr1_idxs[N];
        decoder_idx_t sr2_idxs[N];

        //Parallel encoder search output
        searchStruct_t final;

        //Load shift register1
        LOAD: for(int i=0; i<N; i++)
        {
            sr1_data[i] = H_mtx[i];
            sr1_idxs[i] = i;
        }

        //Reset shift register2
        RESET: for(int i=0; i<N; i++)
        {
            sr2_data[i] = 0;
            sr2_idxs[i] = 0;
        }

        //Main processing loop
        #pragma hls_pipeline_init_interval 1
        PRC: for(int i=0; i<STEPS; i++){
            
            //Cyclic shift
            ccore_cshift.cshift(sr2_data,sr2_idxs,up_count);
            cyclic_count++;

            //XOR-gated (cw+sr1+sr2) combined syndromes
            decoder_m_t tep_sr[N];
            #pragma hls_unroll yes
            TEP: for(int j=0; j<N; j++)
            {
                tep_sr[j] = sc^sr1_data[j]^sr2_data[j];
            }
        
            //Peripheral logic replication
            ccore_per.run(tep_sr,ctrl_tep_arr,idx_arr,found_arr);

            //Codebook membership criteria check
            searchStruct_t S_ARR[P];
            #pragma hls_unroll yes
            CBM: for(int j=0; j<P; j++)
            {
                searchStruct_t tmp_s;
                tmp_s.f = found_arr[j];
                tmp_s.p_id = j;
                tmp_s.idx = idx_arr[j];
                S_ARR[j] = tmp_s;
            }

            //Parallel search
            final = recSearch<P>(S_ARR);
            if(final.f == 1){
                
                //HW=2
                if(final.p_id == 0 && curr_stage == 2 && up_count == 0)
                    curr_stage = 1;

                //Get indices corresponding to encoder index
                idxc = up_count+final.p_id-1;
                decoder_idx_t tmp_idx = final.idx;

                #pragma hls_unroll yes
                IDX: for(int k=0; k<N; k++)
                {
                    if(k == tmp_idx){
                        idx1 = sr1_idxs[k];
                        idx2 = sr2_idxs[k];
                    }
                }

                //Early break on codebook membership match
                found = true;
                break;

            }else if(cyclic_count == max_cyclic){

                cyclic_count = 0;   

                //Copy shift register contents
                #pragma hls_unroll yes
                CP: for(int j=0; j<N; j++)
                {
                    sr2_data[j] = sr1_data[j];
                    sr2_idxs[j] = sr1_idxs[j];
                }

                if(curr_stage == 0){

                    curr_stage = 2;
                    
                    //Initialize control TEP syndromes
                    #pragma hls_unroll yes
                    I_CTRL: for(int j=1; j<P; j++){
                        ctrl_tep_arr[j] = H_mtx[j-1];
                    }       

                    //Compute new cyclic shift max
                    max_cyclic = N >> 1;

                }else if(curr_stage == 2){

                    up_count += P;

                    //Update control TEP syndromes
                    #pragma hls_unroll yes
                    U_CTRL: for(int j=0; j<P; j++)
                    {
                        decoder_idx_t tmp_idx = up_count+j-1;

                        #pragma hls_unroll yes
                        U_SEL: for(int k=0; k<N; k++)
                        {
                            if(k == tmp_idx)
                                ctrl_tep_arr[j] = H_mtx[k];
                        }
                    }

                    //Up-shifts
                    ccore_ushift1.ushift(sr1_data,sr1_idxs);
                    ccore_ushift2.ushift(sr2_data,sr2_idxs);

                    //Compute new cyclic shift max
                    max_cyclic = (N-up_count) >> 1;
                }
            }
        }    
    }

public:

    #include "H_mtx.h"

    //Reset
    decoder_class()
    {
        //Init parity check matrix
        INIT_H_ROWS: for(int i=0; i<N; i++)
        {
            INIT_H_COLS: for(int j=0; j<M; j++)
            {
                H_mtx[i][j] = PARITY_CHECK_MTX[j][i];
            }
        }
    }

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<inStruct_t> &dataIn,
                        ac_channel<decoder_n_t> &dataOut)
    {
        //Noise-corrupted input data
        inStruct_t AWGN = dataIn.read();

        //Hard-decision
        decoder_n_t hd;

        #pragma hls_unroll yes
        DATA_READ: for(int i=0; i<N; i++)
        {
            if(AWGN.data[i] >= 0)
                hd[i] = 0;
            else
                hd[i] = 1;
        }

        //Stage (HW1/2/3)
        curr_stage = 0;

        //Variables
        decoder_n_t dec_out = hd;   //Output
        decoder_m_t sc;             //Syndrome of codeword
        decoder_idx_t idxc = 0;     //Control index
        decoder_idx_t idx1 = 0;     //SR1 index1
        decoder_idx_t idx2 = 0;     //SR2 index2
        bool found = 0;             //Codebook membership match flag
        
        //Syndrome of codeword
        get_syndrome(hd,sc);
        if(sc != 0)
        {
            //If non-zero codeword syndrome, generate TEPs up to HW=3
            process(sc,idxc,idx1,idx2,found);
            if(found)
            {
                #pragma hls_unroll yes
                ID: for(int i=0; i<N; i++){
                    if(curr_stage == 0){
                        if(i == idx1)
                            dec_out[i] = !dec_out[i]; //SR1
                    }else if(curr_stage == 1){
                        if(i == idx1)
                            dec_out[i] = !dec_out[i]; //SR1
                        else if(i == idx2)
                            dec_out[i] = !dec_out[i]; //SR2
                    }else if(curr_stage == 2){
                        if(i == idx1)
                            dec_out[i] = !dec_out[i]; //SR1
                        else if(i == idx2)
                            dec_out[i] = !dec_out[i]; //SR2
                        else if(i == idxc)
                            dec_out[i] = !dec_out[i]; //Ctrl
                    }
                }
            }
        }

        //Write to output
        dataOut.write(dec_out);

    }
};

#endif
