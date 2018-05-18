library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( SMCLK : IN std_logic;
	   RESET : IN std_logic;
	   ROW : OUT std_logic_vector (7 downto 0) := (others => '0');
	   LED : OUT std_logic_vector (7 downto 0) := (others => '0')
);
end ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu.

	signal SMCLK_256: std_logic := '0';
	signal HZ_CLK: std_logic := '0';

	signal row_cnt: std_logic_vector (7 downto 0) := "00000001";

    type char_matrix is array (7 downto 0) of std_logic_vector(7 downto 0);
	constant j_char : char_matrix := ( 
				"11000111",
				"11110111",
				"11110111",
				"11110111",
				"11110111",
				"01110111",
				"01110111",
				"10001111" );

	constant k_char : char_matrix := ( 
				"01110111",
				"01101111",
				"01011111",
				"00111111",
				"00111111",
				"01011111",
				"01101111",
				"01110111" );

begin

    -- Sem doplnte popis funkce obvodu (zakladni konstrukce VHDL jako napr.
    -- prirazeni signalu, multiplexory, dekodery, procesy...).
    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL UVEDENE NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html

    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.

gen_SMCLK_256: process(SMCLK, RESET)
	variable smclk256_count : std_logic_vector (8 downto 0) := "000000000"; 
begin
	if (RESET = '1') then
		smclk256_count := "000000000";
		SMCLK_256 <= '0';
	elsif (SMCLK'event) and (SMCLK = '1') then
		smclk256_count := smclk256_count + 1;
		if (smclk256_count = "011111111") then
			 SMCLK_256 <= '1';
			 smclk256_count := "000000000";
		else
			 SMCLK_256 <= '0'; 
		end if;
	end if;
end process;

gen_HZ_CLK: process(SMCLK, RESET)
	variable hz_count : integer := 0;
begin
	if (RESET = '1') then
		hz_count := 0;
		HZ_CLK <= '0';
	elsif (SMCLK'event) and (SMCLK = '1') then
		hz_count := hz_count + 1;
		if (hz_count = 3686400) then
			 HZ_CLK <= not HZ_CLK;
			 hz_count := 0;
		end if;
	end if;
end process;

gen_ROW_cnt: process(SMCLK, RESET)
begin
	if (RESET = '1') then
		row_cnt <= "00000001";
	elsif (SMCLK'event and SMCLK = '1') then
		if (SMCLK_256 = '1') then
			row_cnt <= row_cnt(6 downto 0) & "0";
			if (row_cnt = "10000000") then
				row_cnt <= "00000001";
			end if;
		end if;
	end if;
end process;

	ROW <= row_cnt;

char_chng: process(HZ_CLK, row_cnt)
begin
	if (HZ_CLK = '1') then
		case row_cnt is
	  		when "00000001" => LED <= k_char(0);
	  		when "00000010" => LED <= k_char(1);
	  		when "00000100" => LED <= k_char(2);
	  		when "00001000" => LED <= k_char(3);
	  		when "00010000" => LED <= k_char(4);
	  		when "00100000" => LED <= k_char(5);
	  		when "01000000" => LED <= k_char(6);
	  		when "10000000" => LED <= k_char(7);
	  		when others => LED <= "11111111";
		end case;
	else
		case row_cnt is
	  		when "00000001" => LED <= j_char(0);
	  		when "00000010" => LED <= j_char(1);
	  		when "00000100" => LED <= j_char(2);
	  		when "00001000" => LED <= j_char(3);
	  		when "00010000" => LED <= j_char(4);
	  		when "00100000" => LED <= j_char(5);
	  		when "01000000" => LED <= j_char(6);
	  		when "10000000" => LED <= j_char(7);
	  		when others => LED <= "11111111";
		end case;
	end if;
end process;
end main;