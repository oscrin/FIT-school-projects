----------------------------------------------------------------------------------
-- Engineer: Jan Kubica (xkubic39)
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.math_pack.ALL; -- vysledek z prvniho ukolu


entity cell is
   GENERIC (
      MASK              : mask_t := (others => '1') -- mask_t definovano v baliku math_pack
   );
   Port ( 
      INVERT_REQ_IN     : in   STD_LOGIC_VECTOR (3 downto 0);
      INVERT_REQ_OUT    : out  STD_LOGIC_VECTOR (3 downto 0);
      
      KEYS              : in   STD_LOGIC_VECTOR (4 downto 0);
      
      SELECT_REQ_IN     : in   STD_LOGIC_VECTOR (3 downto 0);
      SELECT_REQ_OUT    : out  STD_LOGIC_VECTOR (3 downto 0);
      
      INIT_ACTIVE       : in   STD_LOGIC;
      ACTIVE            : out  STD_LOGIC;
      
      INIT_SELECTED     : in   STD_LOGIC;
      SELECTED          : out  STD_LOGIC;

      CLK               : in   STD_LOGIC;
      RESET             : in   STD_LOGIC
   );
end cell;

architecture Behavioral of cell is
  constant IDX_TOP    : NATURAL := 0; -- index sousedni bunky nachazejici se nahore v signalech *_REQ_IN a *_REQ_OUT, index klavesy posun nahoru v KEYS
  constant IDX_LEFT   : NATURAL := 1; -- ... totez        ...                vlevo
  constant IDX_RIGHT  : NATURAL := 2; -- ... totez        ...                vpravo
  constant IDX_BOTTOM : NATURAL := 3; -- ... totez        ...                dole
  
  constant IDX_ENTER  : NATURAL := 4; -- index klavesy v KEYS, zpusobujici inverzi bunky (enter, klavesa 5)

  signal SEL : std_logic;
  signal ACT : std_logic;
  signal INVERT_OUT : std_logic_vector (3 downto 0);
  signal SELECT_OUT : std_logic_vector (3 downto 0);
begin

process(CLK,RESET)
begin

	if (RESET='1') then
		ACT <= INIT_ACTIVE;
		SEL <= INIT_SELECTED;
	elsif (CLK'event) and (CLK='1') then
		
		INVERT_OUT <="0000";
		SELECT_OUT <="0000";
		
		if (SEL = '1') then
		
			if (KEYS(IDX_ENTER) = '1') then
				ACT <= not ACT;
				if (MASK.top = '1') then
					INVERT_OUT(IDX_TOP) <= '1';
				end if;
				if (MASK.left = '1') then
					INVERT_OUT(IDX_LEFT) <= '1';
				end if;
				if (MASK.right = '1') then
					INVERT_OUT(IDX_RIGHT) <= '1';
				end if;
				if (MASK.bottom = '1') then
					INVERT_OUT(IDX_BOTTOM) <= '1';
				end if;
			end if;
			
			if (KEYS(IDX_TOP) = '1' and MASK.top = '1') then
				SEL <= '0';
				SELECT_OUT(IDX_TOP) <= '1';
			elsif (KEYS(IDX_LEFT) = '1' and MASK.left = '1') then
				SEL <= '0';
				SELECT_OUT(IDX_LEFT) <= '1';
			elsif (KEYS(IDX_RIGHT) = '1' and MASK.right = '1') then
				SEL <= '0';
				SELECT_OUT(IDX_RIGHT) <= '1';
			elsif (KEYS(IDX_BOTTOM) = '1' and MASK.bottom = '1') then
				SEL <= '0';
				SELECT_OUT(IDX_BOTTOM) <= '1';
			end if;
					
		else --SEL = '0'
		
			if (INVERT_REQ_IN(IDX_TOP) = '1' and MASK.top = '1') then
				ACT <= not ACT;
			elsif (INVERT_REQ_IN(IDX_LEFT) = '1' and MASK.left = '1') then
				ACT <= not ACT;
			elsif (INVERT_REQ_IN(IDX_RIGHT) = '1' and MASK.right = '1') then
				ACT <= not ACT;
			elsif (INVERT_REQ_IN(IDX_BOTTOM) = '1' and MASK.bottom = '1') then
				ACT <= not ACT;
			end if;
			
			if (SELECT_REQ_IN(IDX_TOP) = '1' and MASK.top = '1') then
				SEL <= '1';
			elsif (SELECT_REQ_IN(IDX_LEFT) = '1' and MASK.left = '1') then
				SEL <= '1';
			elsif (SELECT_REQ_IN(IDX_RIGHT) = '1' and MASK.right = '1') then
				SEL <= '1';
			elsif (SELECT_REQ_IN(IDX_BOTTOM) = '1' and MASK.bottom = '1') then
				SEL <= '1';
			end if;
		end if;
	end if;
end process;

  SELECTED <= SEL;
  ACTIVE <= ACT;
  INVERT_REQ_OUT <= INVERT_OUT;
  SELECT_REQ_OUT <= SELECT_OUT;
  
end Behavioral;

-- Pozadavky na funkci (sekvencni chovani vazane na vzestupnou hranu CLK)
--   pri resetu se nastavi ACTIVE a SELECTED na vychozi hodnotu danou signaly INIT_ACTIVE a INIT_SELECTED
--   pokud je bunka vybrana a prijde signal z klavesnice, tak se bud presune vyber pomoci SELECT_REQ na dalsi bunky nebo se invertuje stav bunky a jejiho okoli pomoci INVERT_REQ (klavesa ENTER)
--   pokud bunka neni vybrana a prijde signal INVERT_REQ, invertuje svuj stav
--   pozadavky do okolnich bunek se posilaji a z okolnich bunek prijimaji, jen pokud je maska na prislusne pozici v '1'

