--------------------------------------
--									--
--	file:       top_level.vhd		--
--	author:     Nicola Vianello		--
--	last edit:	Turin 04/03/2019 	--
--									--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;

entity top_level is
port(	start_writing, UART_rx, clk, rstn : in std_logic;
		in_data_ssram : in std_logic_vector(7 downto 0);
		UART_tx : out std_logic);
end entity top_level;

architecture struct of top_level is

component control_unit is
port( start_writing, clk, rstn, UART_rx, tc_10_bit_cnt, tc_50_clk_cnt, tc_bittime_clk_cnt, tc_memsize_data_cnt : in std_logic;
		rdn_wr_data_ssram, en_addr_cnt, upn_down_addr_cnt, PISO_load, PISO_shift, uart_tx_mux_sel, rstn_bit_cnt, en_bit_cnt, rstn_clk_cnt, en_clk_cnt, rstn_data_cnt, en_data_cnt : out std_logic);
end component control_unit;

component datapath is
	port(	clk, upn_down_addr_cnt, en_addr_cnt, PISO_load, PISO_shift, UART_tx_mux_sel, rstn_bit_cnt, en_bit_cnt, rstn_clk_cnt, en_clk_cnt, rstn_data_cnt, en_data_cnt: in std_logic;
			UART_tx, tc_10_bit_cnt, tc_50_clk_cnt, tc_bittime_clk_cnt, tc_memsize_data_cnt: out std_logic;
			addr_data_ssram: out std_logic_vector(9 downto 0);
			out_data_ssram: in std_logic_vector(7 downto 0));
end component datapath;

component SSRAM_dual_port is
	generic(	N_data_bit : natural := 8;
				N_address_bit : natural := 10);
	port( data_in : in std_logic_vector(N_data_bit-1 downto 0);
			data_out : out std_logic_vector(N_data_bit-1 downto 0);
			address : in std_logic_vector(N_address_bit-1 downto 0);
			rdn_wr, cs, clk : in std_logic);
end component SSRAM_dual_port;

signal addr_data_ssram: std_logic_vector(9 downto 0);
signal out_data_ssram: std_logic_vector(7 downto 0);
signal rdn_wr_data_ssram, en_addr_cnt, upn_down_addr_cnt, PISO_load, PISO_shift, UART_tx_mux_sel, rstn_bit_cnt, en_bit_cnt, tc_10_bit_cnt, rstn_clk_cnt, tc_50_clk_cnt, tc_bittime_clk_cnt, en_clk_cnt, tc_memsize_data_cnt, en_data_cnt, rstn_data_cnt: std_logic;

signal start_writing_conditioned: std_logic;

begin

	control_unit_inst: control_unit
		port map(start_writing=>start_writing_conditioned, clk=>clk, rstn=>rstn, UART_rx=>UART_rx, tc_10_bit_cnt=>tc_10_bit_cnt, tc_50_clk_cnt=>tc_50_clk_cnt, tc_bittime_clk_cnt=>tc_bittime_clk_cnt, tc_memsize_data_cnt=>tc_memsize_data_cnt, rdn_wr_data_ssram=>rdn_wr_data_ssram, en_addr_cnt=>en_addr_cnt, upn_down_addr_cnt=>upn_down_addr_cnt, PISO_load=>PISO_load, PISO_shift=>PISO_shift, uart_tx_mux_sel=>uart_tx_mux_sel, rstn_bit_cnt=>rstn_bit_cnt, en_bit_cnt=>en_bit_cnt, rstn_clk_cnt=>rstn_clk_cnt, en_clk_cnt=>en_clk_cnt, rstn_data_cnt=>rstn_data_cnt, en_data_cnt=>en_data_cnt);

	datapath_inst: datapath
		port map(clk=>clk, upn_down_addr_cnt=>upn_down_addr_cnt, en_addr_cnt=>en_addr_cnt, PISO_load=>PISO_load, PISO_shift=>PISO_shift, UART_tx_mux_sel=>UART_tx_mux_sel, rstn_bit_cnt=>rstn_bit_cnt, en_bit_cnt=>en_bit_cnt, rstn_clk_cnt=>rstn_clk_cnt, en_clk_cnt=>en_clk_cnt, rstn_data_cnt=>rstn_data_cnt, en_data_cnt=>en_data_cnt, UART_tx=>UART_tx, tc_10_bit_cnt=>tc_10_bit_cnt, tc_50_clk_cnt=>tc_50_clk_cnt, tc_bittime_clk_cnt=>tc_bittime_clk_cnt, tc_memsize_data_cnt=>tc_memsize_data_cnt, addr_data_ssram=>addr_data_ssram, out_data_ssram=>out_data_ssram);
	
	data_SSRAM: SSRAM_dual_port
		generic map(N_data_bit=>8, N_address_bit=>10)
		port map(data_in=>in_data_ssram,	data_out=>out_data_ssram, address=>addr_data_ssram, rdn_wr=>rdn_wr_data_ssram, cs=>'1', clk=>clk);
		
	start_writing_conditioning: process(clk)
	variable previous_value: std_logic;
	begin
		if rising_edge(clk) then
			if(start_writing='0' and previous_value='1') then
				start_writing_conditioned<='0';
			else
				start_writing_conditioned<='1';
			end if;
			previous_value:= start_writing;
		end if;
	end process start_writing_conditioning;

end architecture struct;
