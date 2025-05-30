#Catapult Build GRANDAB3
project new -name GRANDAB3
solution new -state initial
solution options defaults

flow package require /SCVerify
solution options set /Cache/UserCacheHome ./ccore_cache
solution options set /Output/GenerateCycleNetlist false

#Vivado
options set Flows/Vivado/RUN_PNR true

#Set working directory to 'HLS_Build'
set script_path [ file dirname [ file normalize [ info script ] ] ]
set path [file dirname $script_path]
file mkdir $path/HLS_Build
set_working_dir $path/HLS_Build

#Add source files
options set Input/SearchPath $path/inc
solution file add ../src/tb_grandab.cpp -type C++ -exclude true
solution file add ../src/decoder.h -type CHEADER

#Hierarchy, Target technology, Clocks
go new
go analyze
solution design set decoder_class -top
go compile
solution design set decoder_ccore_peripherals::run -combinational
solution design set decoder_ccore_ushift::ushift -combinational
solution design set decoder_ccore_cshift::cshift -combinational
solution library add mgc_Xilinx-VIRTEX-uplus-2L_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family VIRTEX-uplus -speed -2L -part xcvu9p-flga2104-2L-e
go libraries
solution design set decoder_ccore_peripherals::run -combinational
solution design set decoder_ccore_ushift::ushift -combinational
solution design set decoder_ccore_cshift::cshift -combinational
directive set -CLOCKS {clk {-CLOCK_PERIOD 10.0 -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME 5.0 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND sync -RESET_SYNC_ACTIVE high -RESET_ASYNC_ACTIVE low -ENABLE_ACTIVE high}}

#Architectural constraints
go assembly
directive set /decoder_class -EFFORT_LEVEL high

#Generate RTL
go architect
go extract

#Sed delay values
proc adjustSdcIoDelaysVhdl {} {
set solutionPath [solution get /SOLUTION_DIR]
set sdcFile $solutionPath/vivado_concat_vhdl/concat_rtl.vhdl.xv.sdc
exec sed "s/set_input_delay -clock \\\[get_clocks {clk}\\\] 0.0/set_input_delay -clock \\\[get_clocks {clk}\\\] 1.0/" -i $sdcFile
exec sed "s/set_output_delay -clock \\\[get_clocks {clk}\\\] 0.0/set_output_delay -clock \\\[get_clocks {clk}\\\] 9.0/" -i $sdcFile
exec sed "s/set_input_delay -clock \\\[get_clocks {clk}\\\] 1.0 \\\[get_ports {rst}\\\]/set_input_delay -clock \\\[get_clocks {clk}\\\] 0.1 \\\[get_ports {rst}\\\]/" -i $sdcFile
}

adjustSdcIoDelaysVhdl

#Sed delay values
proc adjustSdcIoDelaysVerilog {} {
set solutionPath [solution get /SOLUTION_DIR]
set sdcFile $solutionPath/vivado_concat_v/concat_rtl.v.xv.sdc
exec sed "s/set_input_delay -clock \\\[get_clocks {clk}\\\] 0.0/set_input_delay -clock \\\[get_clocks {clk}\\\] 1.0/" -i $sdcFile
exec sed "s/set_output_delay -clock \\\[get_clocks {clk}\\\] 0.0/set_output_delay -clock \\\[get_clocks {clk}\\\] 9.0/" -i $sdcFile
exec sed "s/set_input_delay -clock \\\[get_clocks {clk}\\\] 1.0 \\\[get_ports {rst}\\\]/set_input_delay -clock \\\[get_clocks {clk}\\\] 0.1 \\\[get_ports {rst}\\\]/" -i $sdcFile
}

adjustSdcIoDelaysVerilog
