--------------------------------------
--												--
--	file:       PISO_register.vhd		--
--	author:     Nicola Vianello		--
--	last edit:	Turin 22/12/2018		--
--												--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;

entity PISO_register is
	generic(Nbits: natural:=8);
	port(	clk, load, shift_enable: in std_logic;
			data_in: in std_logic_vector(Nbits-1 downto 0);
			data_out: out std_logic);
end entity PISO_register;

architecture behavioral of PISO_register is

signal data: std_logic_vector(data_in'range);

begin
	process(clk)
	begin
		if rising_edge(clk) then
			if load='1' then
				data<=data_in;
			elsif shift_enable='1' then
				data(Nbits-2 downto 0)<=data(Nbits-1 downto 1);
				data(Nbits-1)<='0';
			end if;
		end if;
	end process;
	
	data_out<=data(0);
	
end architecture behavioral;
