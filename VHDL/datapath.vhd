--------------------------------------
--									--
--	file:       datapath.vhd		--
--	author:     Nicola Vianello		--
--	last edit:	Venice 25/02/2019 	--
--									--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity datapath is
	port(	clk, upn_down_addr_cnt, en_addr_cnt, PISO_load, PISO_shift, UART_tx_mux_sel, rstn_bit_cnt, en_bit_cnt, rstn_clk_cnt, en_clk_cnt, rstn_data_cnt, en_data_cnt: in std_logic;
			UART_tx, tc_10_bit_cnt, tc_50_clk_cnt, tc_bittime_clk_cnt, tc_memsize_data_cnt: out std_logic;
			addr_data_ssram: out std_logic_vector(9 downto 0);
			out_data_ssram: in std_logic_vector(7 downto 0));
end entity datapath;

architecture struct of datapath is

component counter is
	generic(Nbits: natural := 8);
	port(	en, clk, rstn, upn_down: in std_logic;
			cnt: out std_logic_vector(Nbits-1 downto 0));
end component counter;

component PISO_register is
	generic(Nbits: natural:=8);
	port(	clk, load, shift_enable: in std_logic;
			data_in: in std_logic_vector(Nbits-1 downto 0);
			data_out: out std_logic);
end component PISO_register;

component mux is
	generic(	Nbits: natural := 1;
				Nsel: natural := 1);		
	port(		inputs: in std_logic_vector((2**Nsel)*Nbits-1 downto 0);
				sel: in std_logic_vector(Nsel-1 downto 0);
				output: out std_logic_vector(Nbits-1 downto 0));
end component mux;

signal out_data_ssram_ext, data_cnt: std_logic_vector(9 downto 0);
signal clk_cnt: std_logic_vector(12 downto 0);
signal bit_cnt: std_logic_vector(3 downto 0);
signal PISO_out: std_logic;

constant baud_rate: integer := 1280000;
constant clock_frequency: integer := 50000000;
constant clock_cycles_per_bit_time: integer:= clock_frequency/baud_rate;

begin

	address_counter: counter
		generic map(Nbits=>10)
		port map(en=>en_addr_cnt, clk=>clk, rstn=>'1', upn_down=>upn_down_addr_cnt, cnt=>addr_data_ssram);
		
	out_data_ssram_ext(9)<='1';
	out_data_ssram_ext(8 downto 1)<=out_data_ssram(7 downto 0);
	out_data_ssram_ext(0)<='0';
		
	UART_tx_PISO_register: PISO_register
		generic map(Nbits=>10)
		port map(clk=>clk, load=>PISO_load, shift_enable=>PISO_shift, data_in=>out_data_ssram_ext, data_out=>PISO_out);

	UART_tx_mux: mux
		generic map(Nbits=>1, Nsel=>1)
		port map(inputs(0)=>'1', inputs(1)=>PISO_out, sel(0)=>UART_tx_mux_sel, output(0)=>UART_tx);
		
	bit_counter: counter
		generic map(Nbits=>4)
		port map(en=>en_bit_cnt, clk=>clk, rstn=>rstn_bit_cnt, upn_down=>'0', cnt=>bit_cnt);
	tc_10_bit_cnt<='1' when bit_cnt=std_logic_vector(to_unsigned(9, bit_cnt'length)) else '0';
		
	clock_counter: counter
		generic map(Nbits=>13)
		port map(en=>en_clk_cnt, clk=>clk, rstn=>rstn_clk_cnt, upn_down=>'0', cnt=>clk_cnt);
	tc_50_clk_cnt<='1' when clk_cnt=std_logic_vector(to_unsigned(49, clk_cnt'length)) else '0';
	tc_bittime_clk_cnt<='1' when clk_cnt=std_logic_vector(to_unsigned(clock_cycles_per_bit_time-2, clk_cnt'length)) else '0';
	
	data_counter: counter
		generic map(Nbits=>10)
		port map(en=>en_data_cnt, clk=>clk, rstn=>rstn_data_cnt, upn_down=>'0', cnt=>data_cnt);
	tc_memsize_data_cnt<='1' when data_cnt=std_logic_vector(to_unsigned(1023, data_cnt'length)) else '0';
		
end architecture struct;
