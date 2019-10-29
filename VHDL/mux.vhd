--------------------------------------
--												--
--	file:       mux.vhd					--
--	author:     Nicola Vianello		--
--	last edit:	Turin 29/12/2018		--
--												--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
				
entity mux is
	generic(	Nbits: natural := 1;
				Nsel: natural := 1);		
	port(		inputs: in std_logic_vector((2**Nsel)*Nbits-1 downto 0);
				sel: in std_logic_vector(Nsel-1 downto 0);
				output: out std_logic_vector(Nbits-1 downto 0));
end entity mux;

architecture behavioural of mux is
begin
		output <= inputs((to_integer(unsigned(sel))+1)*Nbits-1 downto to_integer(unsigned(sel))*Nbits);
end architecture behavioural;
