
cmake_minimum_required(VERSION 3.15)
project(bsp)
        
set(DRIVER_TARGETS gpio;intc;tmrctr;uartlite)
set(DRIVER_LOCATIONS C:/Xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/gpio_v4_11;C:/Xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/intc_v3_18;C:/Xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/tmrctr_v4_12;C:/Xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/uartlite_v3_10)

list(LENGTH DRIVER_TARGETS no_of_drivers)
set(index 0)

while(${index} LESS ${no_of_drivers})
    list(GET DRIVER_TARGETS ${index} drv)
    list(GET DRIVER_LOCATIONS ${index} drv_dir)
    set(exp_dir "${drv_dir}/examples")
    add_custom_target(
        ${drv}_example ALL
        COMMAND lopper -f -O C:/Users/Alejandro/Documents/git/vitis_Urbana_2024_1/genesys2_microblaze_standard/microblaze_0/standalone_microblaze_0/bsp/libsrc/${drv} C:/Users/Alejandro/Documents/git/vitis_Urbana_2024_1/genesys2_microblaze_standard/microblaze_0/standalone_microblaze_0/bsp/hw_artifacts/microblaze_0_baremetal.dts -- bmcmake_metadata_xlnx microblaze_0 ${exp_dir} drvcmake_metadata
        BYPRODUCTS x${drv}_exlist.yaml)
    MATH(EXPR index "${index}+1")
endwhile()
