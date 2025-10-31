# クロック設定
create_clock -name bclk -period 20833.000 [get_ports bclk]
create_clock -name pblrc -period 325.520 [get_ports pblrc]
create_clock -name reclrc -period 325.520 [get_ports reclrc]


# SSM2603 Audio Codec
set_property -dict {PACKAGE_PIN P18 IOSTANDARD LVCMOS33} [get_ports mute]
set_property -dict {PACKAGE_PIN T19 IOSTANDARD LVCMOS33} [get_ports pblrc]
set_property -dict {PACKAGE_PIN Y18 IOSTANDARD LVCMOS33} [get_ports reclrc]
set_property -dict {PACKAGE_PIN R19 IOSTANDARD LVCMOS33} [get_ports bclk]
set_property -dict {PACKAGE_PIN R18 IOSTANDARD LVCMOS33} [get_ports sdata_out]
set_property -dict {PACKAGE_PIN R16 IOSTANDARD LVCMOS33} [get_ports sdata_in]
set_property -dict {PACKAGE_PIN R17 IOSTANDARD LVCMOS33} [get_ports aud_mclk]

set_property -dict {PACKAGE_PIN N18 IOSTANDARD LVCMOS33} [get_ports IIC_0_scl_io]
set_property -dict {PACKAGE_PIN N17 IOSTANDARD LVCMOS33} [get_ports IIC_0_sda_io]

# Button
set_property -dict {PACKAGE_PIN K18 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[0]}]
set_property -dict {PACKAGE_PIN P16 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[1]}]
set_property -dict {PACKAGE_PIN K19 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[2]}]
set_property -dict {PACKAGE_PIN Y16 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[3]}]

# LED
set_property -dict {PACKAGE_PIN M14 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_o[0]}]
set_property -dict {PACKAGE_PIN M15 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_o[1]}]
set_property -dict {PACKAGE_PIN G14 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_o[2]}]
set_property -dict {PACKAGE_PIN D18 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_o[3]}]

# GPIO(Input)
set_property -dict {PACKAGE_PIN V13 IOSTANDARD LVCMOS33} [get_ports {GPIO_0_tri_i[0]}]
set_property -dict {PACKAGE_PIN U17 IOSTANDARD LVCMOS33} [get_ports {GPIO_0_tri_i[1]}]

# GPIO(Output)
set_property -dict {PACKAGE_PIN T17 IOSTANDARD LVCMOS33} [get_ports {GPIO2_0_tri_o[0]}]
set_property -dict {PACKAGE_PIN Y17 IOSTANDARD LVCMOS33} [get_ports {GPIO2_0_tri_o[1]}]

# (I2C)
set_property -dict {PACKAGE_PIN V12 IOSTANDARD LVCMOS33} [get_ports IIC_1_scl_io]
set_property -dict {PACKAGE_PIN W16 IOSTANDARD LVCMOS33} [get_ports IIC_1_sda_io]
set_property -dict {PACKAGE_PIN J15 IOSTANDARD LVCMOS33} [get_ports tp_rst]

# SDカード
#set_property -dict {PACKAGE_PIN M14 IOSTANDARD LVCMOS33} [get_ports {ld[0]}]


# デバッグ用
# set_false_path -from [get_clocks aud_mclk_design_1_clk_wiz_0_0] -to [get_clocks clk_fpga_0]

# SPI(EMIO Pmod JD)
# set_property -dict {PACKAGE_PIN T14 IOSTANDARD LVCMOS33} [get_ports SPI1_SCLK_O]
# set_property -dict {PACKAGE_PIN T15 IOSTANDARD LVCMOS33} [get_ports SPI1_MOSI_O]
# set_property -dict {PACKAGE_PIN P14 IOSTANDARD LVCMOS33} [get_ports SPI1_SS_O]
# set_property -dict {PACKAGE_PIN R14 IOSTANDARD LVCMOS33} [get_ports SPI1_MISO_I]
# set_property -dict {PACKAGE_PIN U14 IOSTANDARD LVCMOS33} [get_ports RESET]
# set_property -dict {PACKAGE_PIN U15 IOSTANDARD LVCMOS33} [get_ports DC]
