--------------------------------------
--									--
--	file:       SSRAM_dual_port.vhd	--
--	author:     Nicola Vianello		--
--	last edit:	Turin 19/12/2018 	--
--									--
--------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity SSRAM_dual_port is
	generic(	N_data_bit : natural := 8;
				N_address_bit : natural := 10);
	port( data_in : in std_logic_vector(N_data_bit-1 downto 0);
			data_out : out std_logic_vector(N_data_bit-1 downto 0);
			address : in std_logic_vector(N_address_bit-1 downto 0);
			rdn_wr, cs, clk : in std_logic);
end entity SSRAM_dual_port;

architecture behavioral of SSRAM_dual_port is
	type mem_type is array(0 to 2**N_address_bit-1) of std_logic_vector(N_data_bit-1 downto 0);
	signal mem : mem_type ;
	begin
		process(clk)
		begin
			if rising_edge(clk) then
				if cs='1' then
					if rdn_wr='0' then	--read
						data_out<=mem(to_integer(unsigned(address)));
					else	--write
						mem(to_integer(unsigned(address)))<=data_in;
					end if;
				end if; 
			end if;
		end process;
end architecture behavioral;
