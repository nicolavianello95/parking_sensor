--------------------------------------
--									--
--	file:       SSRAM_dual_port.vhd	--
--	author:     Nicola Vianello		--
--	last edit:	Turin 02/01/2019 	--
--									--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity counter is
	generic(Nbits: natural := 8);
	port(	en, clk, rstn, upn_down: in std_logic;
			cnt: out std_logic_vector(Nbits-1 downto 0));
end entity counter;

architecture behavioral of counter is
begin
	process(clk)
	variable cnt_var : std_logic_vector(cnt'range) :=(others=>'0');
	begin
		if rising_edge(clk) then
			if rstn='0' then
				cnt_var:=(others=>'0');
			else
				if en='1' then
					if upn_down='0' then
						cnt_var:=cnt_var+1;
					else
						cnt_var:=cnt_var-1;
					end if;
				end if;
			end if;
		end if;
	cnt<=cnt_var;
	end process;
end architecture behavioral;