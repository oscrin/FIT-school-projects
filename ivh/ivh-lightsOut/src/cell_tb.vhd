--------------------------------------------------------------------------------
-- Engineer: Jan Kubica (xkubic39)
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE work.math_pack.ALL;
 
ENTITY cell_tb IS
END cell_tb;
 
ARCHITECTURE behavior OF cell_tb IS 

   --Inputs
   signal INVERT_REQ_IN : std_logic_vector(3 downto 0) := (others => '0');
   signal SELECT_REQ_IN : std_logic_vector(3 downto 0) := (others => '0');
   signal INIT_ACTIVE : std_logic := '0';
   signal INIT_SELECTED : std_logic := '0';
	
	signal KEYS : std_logic_vector(4 downto 0) := (others => '0');
	
   signal CLK : std_logic := '0';
   signal RESET : std_logic := '0';

 	--Outputs
   signal INVERT_REQ_OUT : std_logic_vector(3 downto 0);
   signal SELECT_REQ_OUT : std_logic_vector(3 downto 0);
	
   signal ACTIVE : std_logic;
   signal SELECTED : std_logic;

	constant IDX_TOP    : NATURAL := 0;
   constant IDX_LEFT   : NATURAL := 1;
   constant IDX_RIGHT  : NATURAL := 2;
   constant IDX_BOTTOM : NATURAL := 3;
   constant IDX_ENTER  : NATURAL := 4;
 
BEGIN
   cell: entity work.cell
	  PORT MAP (
          INVERT_REQ_IN => INVERT_REQ_IN,
          INVERT_REQ_OUT => INVERT_REQ_OUT,
          KEYS => KEYS,
          SELECT_REQ_IN => SELECT_REQ_IN,
          SELECT_REQ_OUT => SELECT_REQ_OUT,
          INIT_ACTIVE => INIT_ACTIVE,
          ACTIVE => ACTIVE,
          INIT_SELECTED => INIT_SELECTED,
          SELECTED => SELECTED,
          CLK => CLK,
          RESET => RESET
        );

	CLK <= not CLK after 10 ns; 

	rst:process
    BEGIN
		
		wait for 5 ns;
		report "RESET - INIT_values testing." severity NOTE;
		
      INIT_ACTIVE <= '0'; INIT_SELECTED <= '0';
      wait for 5 ns; 
		RESET <= not RESET; 
		wait for 20 ns; 
		RESET <= not RESET; 
		wait for 5 ns;
      assert(ACTIVE = '0' and SELECTED = '0') 
		report "INIT_VALUES(0,0) are not set after RESET." severity ERROR;

      INIT_ACTIVE <= '1'; INIT_SELECTED <= '0';
      wait for 5 ns; 
		RESET <= not RESET; 
		wait for 20 ns; 
		RESET <= not RESET; 
		wait for 5 ns;
      assert(ACTIVE = '1' and SELECTED = '0') 
		report "INIT_VALUES(1,0) are not set after RESET." severity ERROR;

      INIT_ACTIVE <= '1'; INIT_SELECTED <= '1';
      wait for 5 ns; 
		RESET <= not RESET; 
		wait for 20 ns; 
		RESET <= not RESET; 
		wait for 5 ns;
      assert(ACTIVE = '1' and SELECTED = '1') 
		report "INIT_VALUES(1,1) are not set after RESET." severity ERROR;
      
      INIT_ACTIVE <= '0'; INIT_SELECTED <= '1';
      wait for 5 ns; 
		RESET <= not RESET; 
		wait for 20 ns; 
		RESET <= not RESET; 
		wait for 5 ns;
      assert(ACTIVE = '0' and SELECTED = '1') 
		report "INIT_VALUES(0,1) are not set after RESET." severity ERROR;
      
		wait for 60 ns;
		report "Sending cursor (SELECT_OUT) testing." severity NOTE;
 
      for i in 0 to 3 loop       
        KEYS(i) <= '1';
        wait until rising_edge(CLK);
		  wait for 5 ns; 
        assert(SELECT_REQ_OUT(i) = '1') 
		  report "SELECT_REQ_OUT miscalculate on 1." severity ERROR;
        KEYS(i) <= '0'; 
        
        KEYS(i) <= '0';
        wait until rising_edge(CLK); wait for 5 ns; 
        assert(SELECT_REQ_OUT(i) = '0')
		  report "SELECT_REQ_OUT miscalculate on 0.";
        KEYS(i) <= '0';

        INIT_ACTIVE <= '1'; INIT_SELECTED <= '1';
        wait for 5 ns;
		  RESET <= not RESET;
		  wait for 20 ns;
		  RESET <= not RESET;
		  wait for 5 ns;
        assert(ACTIVE = '1' and SELECTED = '1')
		  report "Final RESET ERROR" severity ERROR;
      end loop;
		
		wait for 60 ns;
		report "ENTER invertion testing." severity NOTE;
		wait for 5 ns;
		
		KEYS(4) <= '1';
		wait until rising_edge(CLK);
		wait for 5 ns;
		assert(INVERT_REQ_OUT = "1111") 
		report "Invertion after ENTER not set.";
		wait for 25 ns;
		KEYS(4) <= '0';
		
		wait for 10 ns;
		
		report "Simulation Completed" severity note;

      wait;
    END process;
END;