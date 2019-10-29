--------------------------------------
--									--
--	file:       testbench.vhd		--
--	author:     Nicola Vianello		--
--	last edit:	Turin 04/01/2019 	--
--									--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;

entity testbench is
end entity testbench;

architecture tb of testbench is

constant clk_period: time :=20 ns;

component top_level is
port(	start_writing, UART_rx, clk, rstn : in std_logic;
		in_data_ssram : in std_logic_vector(7 downto 0);
		UART_tx : out std_logic
);
end component top_level;

signal start_writing, UART_rx, UART_tx, clk, rstn : std_logic;
signal in_data_ssram : std_logic_vector(7 downto 0);

begin

	dut: top_level port map(start_writing=>start_writing, UART_rx=>UART_rx, clk=>clk, rstn=>rstn, in_data_ssram=>in_data_ssram, UART_tx=>UART_tx);

	clk_gen: process is
	begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
	end process clk_gen;
	
	stim_gen: process is
	begin
	
		--init
		rstn<='0';
		UART_rx<='1';
		start_writing<='1';
		wait for clk_period;
		rstn<='1';
		wait for 2*clk_period;
		
		--write data 1
		start_writing<='0';
		wait for 5*clk_period;
		start_writing<='1';
		wait for 5*clk_period;
		in_data_ssram<="11110000";
		wait for 50*clk_period;
		
		--write data 2
		start_writing<='0';
		wait for 5*clk_period;
		start_writing<='1';
		wait for 5*clk_period;
		in_data_ssram<="00001111";
		wait for 50*clk_period;
		
		--write data 3
		start_writing<='0';
		wait for 5*clk_period;
		start_writing<='1';
		wait for 5*clk_period;
		in_data_ssram<="01010101";
		wait for 50*clk_period;
		
		--outuput request
		uart_rx<='0';
		wait for 30*clk_period;
		uart_rx<='1';
		wait;
	end process stim_gen;
	
end architecture tb;
