library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;

entity bcd_citac is
    generic (
	   WIDTH : integer := 4;
		MAX: integer := 9
	 );
    port (
		CLK : IN  STD_LOGIC;
		RESET : IN STD_LOGIC;
		NUMBER1 : BUFFER STD_LOGIC_VECTOR (WIDTH-1 downto 0) := (others=>'0');
		NUMBER2 : BUFFER STD_LOGIC_VECTOR (WIDTH-1 downto 0) := (others=>'0');
		NUMBER3 : BUFFER STD_LOGIC_VECTOR (WIDTH-1 downto 0) := (others=>'0')
	 );
end entity bcd_citac;

architecture Behavioral of bcd_citac is
	signal cnt: std_logic_vector(WIDTH-1 downto 0) := (others => '0');
begin

process(CLK)
begin
if (RESET = '1') then
	 NUMBER1 <= (others => '0');
	 NUMBER2 <= (others => '0');
    NUMBER3 <= (others => '0');
elsif (CLK'event) and (CLK='1') then
		 NUMBER1 <= NUMBER1 + 1;
		 if (NUMBER1 = MAX) then
			  NUMBER1 <= (others => '0');
		     NUMBER2 <= NUMBER2 + 1;
			  if (NUMBER2 = MAX) then
					NUMBER2 <= (others => '0');
					NUMBER3 <= NUMBER3 + 1;
					if (NUMBER3 = MAX) then
					    NUMBER3 <= (others => '0');
						 NUMBER2 <= (others => '0');
						 NUMBER1 <= (others => '0');
					end if;
			  end if;
		 end if;
end if;

end process;


end architecture Behavioral;

