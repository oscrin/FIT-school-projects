LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.ALL;
 
ENTITY bcd_tb IS
END bcd_tb;
 
ARCHITECTURE behavior OF bcd_tb IS 
 
    COMPONENT bcd_citac
    PORT(
         CLK : IN  std_logic;
         RESET : IN  std_logic;
         NUMBER1 : BUFFER  std_logic_vector(3 downto 0);
         NUMBER2 : BUFFER  std_logic_vector(3 downto 0);
         NUMBER3 : BUFFER  std_logic_vector(3 downto 0)
        );
    END COMPONENT;
    
   --Inputs
   signal CLK : std_logic := '0';
   signal RESET : std_logic := '0';

 	--Outputs
   signal NUMBER1 : std_logic_vector(3 downto 0);
   signal NUMBER2 : std_logic_vector(3 downto 0);
   signal NUMBER3 : std_logic_vector(3 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 500 ps;
 
BEGIN
	-- Instantiate the Unit Under Test (UUT)
   uut: bcd_citac PORT MAP (
          CLK => CLK,
          RESET => RESET,
          NUMBER1 => NUMBER1,
          NUMBER2 => NUMBER2,
          NUMBER3 => NUMBER3
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 
   -- Stimulus process
   stim_proc: process
   begin
		RESET <= '0';
		
		report "Clock period set to 500 ps."
			severity NOTE;
		
		assert not (NUMBER1 = 0000 and NUMBER2 = 0000 and NUMBER3 = 0000)
			report "All NUMBERS set to zeros." 
			severity NOTE;
		
		wait for 4 ns;
		
		assert not (RESET = '0')
			report "RESET set to 1." 
			severity NOTE;
		assert not (RESET = '1')
			report "RESET set to 0." 
			severity NOTE;
		assert (NUMBER1 = 0000)
			report "NUMBER1 set to zero." 
			severity NOTE;
		assert (NUMBER2 = 0000)
			report "NUMBER2 set to zero." 
			severity NOTE;
		assert (NUMBER3 = 0000)
			report "NUMBER3 set to zero." 
			severity NOTE;
		assert not (NUMBER1 = 0000 and NUMBER2 = 0000 and NUMBER3 = 0000)
			report "All NUMBERS set to zeros." 
			severity NOTE;
		assert not (NUMBER1 > 1001 or NUMBER2 > 1001 or NUMBER3 > 1001)
			report "Couter miscalculate"
			severity ERROR;
			
		RESET <= '1';
		
		wait for 1 ns;
		
		assert not (RESET = '0')
			report "RESET set to 1." 
			severity NOTE;
		assert not (RESET = '1')
			report "RESET set to 0." 
			severity NOTE;
		assert not (NUMBER1 > 1001 or NUMBER2 > 1001 or NUMBER3 > 1001)
			report "Couter miscalculate"
			severity ERROR;
			
		RESET <= '0';
		
		wait for 14 ns;
		
		assert not (RESET = '0')
			report "RESET set to 1." 
			severity NOTE;
		assert not (RESET = '1')
			report "RESET set to 0." 
			severity NOTE;
		assert (NUMBER1 = 0000)
			report "NUMBER1 set to zero." 
			severity NOTE;
		assert (NUMBER2 = 0000)
			report "NUMBER2 set to zero." 
			severity NOTE;
		assert (NUMBER3 = 0000)
			report "NUMBER3 set to zero." 
			severity NOTE;
		assert not (NUMBER1 = 0000 and NUMBER2 = 0000 and NUMBER3 = 0000)
			report "All NUMBERS set to zeros." 
			severity NOTE;
		assert not (NUMBER1 > 1001 or NUMBER2 > 1001 or NUMBER3 > 1001)
			report "Couter miscalculate"
			severity ERROR;
			
		RESET <= '1';
		
		wait for 1 ns;
		
		assert not (RESET = '0')
			report "RESET set to 1." 
			severity NOTE;
		assert not (RESET = '1')
			report "RESET set to 0." 
			severity NOTE;
		assert not (NUMBER1 > 1001 or NUMBER2 > 1001 or NUMBER3 > 1001)
			report "Couter miscalculate"
			severity ERROR;
			
		RESET <= '0';
		
		wait for 59 ns;
		
		assert not (RESET = '0')
			report "RESET set to 1." 
			severity NOTE;
		assert not (RESET = '1')
			report "RESET set to 0." 
			severity NOTE;
		assert (NUMBER1 = 0000)
			report "NUMBER1 set to zero." 
			severity NOTE;
		assert (NUMBER2 = 0000)
			report "NUMBER2 set to zero." 
			severity NOTE;
		assert (NUMBER3 = 0000)
			report "NUMBER3 set to zero." 
			severity NOTE;
		assert not (NUMBER1 = 0000 and NUMBER2 = 0000 and NUMBER3 = 0000)
			report "All NUMBERS set to zeros." 
			severity NOTE;
		assert not (NUMBER1 > 1001 or NUMBER2 > 1001 or NUMBER3 > 1001)
			report "Couter miscalculate"
			severity ERROR;
			
		RESET <= '1';
		
		wait for 12 ns;
		
		assert not (RESET = '0')
			report "RESET set to 1." 
			severity NOTE;
		assert not (RESET = '1')
			report "RESET set to 0." 
			severity NOTE;
		assert not (NUMBER1 > 1001 or NUMBER2 > 1001 or NUMBER3 > 1001)
			report "Couter miscalculate"
			severity ERROR;
		RESET <= '0';

		wait for 499 ns;
		
		assert (NUMBER1 /= 1001 and NUMBER2 /= 1001 and NUMBER3 /= 1001)
			report "Counter overflow." 
			severity WARNING;

		wait for 10 ns;
	
			report "Simulation completed."
			severity NOTE;
			
			wait;
	end process;
	
END;
