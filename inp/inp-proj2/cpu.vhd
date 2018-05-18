-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Jan Kubica (xkubic39) <xkubic AT stud.fit.vutbr.cz>
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

--------------------------
-- PC
--------------------------
   signal PC_cAddr : std_logic_vector(11 downto 0);
   signal PC_inc : std_logic;
   signal PC_dec : std_logic;
--------------------------
-- CNT
--------------------------
   signal CNT_inc : std_logic;
   signal CNT_dec : std_logic;
   signal CNT : std_logic_vector(7 downto 0);
   signal CNT_test : std_logic;
--------------------------
-- TMP
--------------------------
   signal TMP_ld : std_logic;
   signal TMP : std_logic_vector(7 downto 0);
   signal TMP_RDATA_test : std_logic;
--------------------------
-- PTR
--------------------------
   signal PTR_dAddr : std_logic_vector(9 downto 0);
   signal PTR_inc : std_logic;
   signal PTR_dec : std_logic;
--------------------------
-- MUX
--------------------------
   signal sel : std_logic_vector(1 downto 0);
   --0- IN_DATA
   --1- TMP
   --2- DATA_RDATA + 1
   --3- DATA_RDATA - 1
--------------------------
-- DEC - INSTRUCTION DECODER
--------------------------
   type I_type is (incPtr, decPtr, incVal, decVal, whileB, whileE, printVal, getVal, storeVal, loadVal, skip, halt);
   -- mozne instrukce
   signal I_dcode : I_type;
--------------------------
-- FSM
--------------------------
   type FSM_state is (sInit, sFetch, sDecode, sIncPtr, sDecPtr, sIncVal, sIncVal2, sDecVal, sDecVal2, sWhileB, sWhileB1, sWhileE, sWhileE1, sPrintVal, sPrintVal2, sGetVal, sStoreVal, sLoadVal, sSkip, sHalt);
   signal pState : FSM_state;
   signal nState : FSM_state;

   

 -- zde dopiste vlastni VHDL kod

begin
-- ----------------------------------------------------------------------------
--                          PC - PROGRAM COUNTER
-- ----------------------------------------------------------------------------

   PC: process(CLK, RESET)
   begin
      if (RESET = '1') then 
         PC_cAddr <= (others => '0');
      elsif (CLK'event) and (CLK = '1') then
         if (PC_inc = '1') and (PC_dec = '0')  then
            PC_cAddr <= PC_cAddr + 1;
         end if;
         if (PC_inc = '0') and (PC_dec = '1') then
            PC_cAddr <= PC_cAddr - 1;
         end if;
      end if;
   end process;

   CODE_ADDR <= PC_cAddr;

-- ----------------------------------------------------------------------------
--                                   CNT
-- ----------------------------------------------------------------------------

   CNT_process: process(CLK, RESET)
   begin
      if (RESET = '1') then
         CNT <= (others => '0');
      elsif (CLK'event) and (CLK = '1') then
         if (CNT_inc = '1') and (CNT_dec = '0') then
            CNT <= CNT + 1;
         end if;
         if (CNT_inc = '0') and (CNT_dec = '1') then
            CNT <= CNT - 1;
         end if;
      end if;         
   end process;

   CNT_comp: process(CNT)
   begin
      if (CNT = "0000000") then
         CNT_test <= '1';
      else
         CNT_test <= '0';
      end if;
   end process;

-- ----------------------------------------------------------------------------
--                                   TMP
-- ----------------------------------------------------------------------------
   TMP_process: process(CLK, RESET)
   begin
      if (RESET = '1') then 
         TMP <= (others => '0');
      elsif (CLK'event) and (CLK = '1') then
         if (TMP_ld = '1') then
            TMP <= DATA_RDATA;
         end if;
      end if;
   end process;

   TMP_comp: process(DATA_RDATA)
   begin
      if (DATA_RDATA = "00000000") then
         TMP_RDATA_test <= '1';
      else
         TMP_RDATA_test <= '0';
      end if;
   end process;

-- ----------------------------------------------------------------------------
--                                PTR DATA
-- ---------------------------------------------------------------------------- 
   PTR: process (CLK, RESET)
   begin
      if (RESET = '1') then 
         PTR_dAddr <= (others => '0');
      elsif (CLK'event) and (CLK = '1') then
         if (PTR_inc = '1') and (PTR_dec = '0') then
            PTR_dAddr <= PTR_dAddr + 1;
         end if;
         if (PTR_inc = '0') and (PTR_dec = '1') then
            PTR_dAddr <= PTR_dAddr - 1;
         end if;
      end if;
   end process;   

   DATA_ADDR <= PTR_dAddr;

-- ----------------------------------------------------------------------------
--                                    MUX
-- ----------------------------------------------------------------------------
   MUX: process(sel, TMP, DATA_RDATA, IN_DATA)
   begin
      case sel is
         when "00" => DATA_WDATA <= IN_DATA;
         when "01" => DATA_WDATA <= TMP;
         when "10" => DATA_WDATA <= DATA_RDATA - 1;
         when "11" => DATA_WDATA <= DATA_RDATA + 1;
         when others => 
      end case;
   end process;

-- ----------------------------------------------------------------------------
--                       DEC - INSTRUCTION DECODER
-- ----------------------------------------------------------------------------
   ID: process (CODE_DATA)
   begin
      case ( CODE_DATA ) is
         when X"3E" => I_dcode <= incPtr;     --">"
         when X"3C" => I_dcode <= decPtr;     --"<"
         when X"2B" => I_dcode <= incVal;     --"+"
         when X"2D" => I_dcode <= decVal;     --"-"
         when X"5B" => I_dcode <= whileB;     --"["
         when X"5D" => I_dcode <= whileE;     --"]"
         when X"2E" => I_dcode <= printVal;   --"."
         when X"2C" => I_dcode <= getVal;     --","
         when X"24" => I_dcode <= storeVal;   --"$"
         when X"21" => I_dcode <= loadVal;    --"!"
         when X"00" => I_dcode <= halt;      --"\0"
         when others => I_dcode <= skip;
      end case;
   end process;

-- ----------------------------------------------------------------------------
--                                I/O - RAM
-- ----------------------------------------------------------------------------

   OUT_DATA <= DATA_RDATA;

-- ----------------------------------------------------------------------------
--                                   FSM
-- ----------------------------------------------------------------------------
   FSM_logic: process(EN, RESET, CLK)
   begin
     if (RESET = '1') then
       pState <= sInit;
      elsif (CLK'event) and (CLK = '1') then
         if (EN = '1') then
            pState <= nState;
         end if;
     end if;
   end process;


   FSM_run: process(pState, I_dcode, TMP_RDATA_test, CNT_test, PC_cAddr, OUT_BUSY, IN_VLD)
      variable start : std_logic_vector(11 downto 0);
   begin

      DATA_RDWR <= '1';
      DATA_EN <= '0';
      CODE_EN <= '0';
      PC_inc <= '0';
      PC_dec <= '0';

      CNT <= (others => '0');
      CNT_inc <= '0';
      CNT_dec <= '0';
      PTR_dec <= '0';
      PTR_inc <= '0';

      TMP_ld <= '0';

      sel <= (others => '0');
      IN_REQ <= '0';
      OUT_WE <= '0';

      case ( pState ) is
      
         when sInit =>
            nState <= sFetch;
         
         when sFetch =>
            CODE_EN <= '1';
            nState <= sDecode;

         when sDecode =>
            case ( I_dcode ) is                    
               when incPtr   => nState <= sIncPtr;    --">"
               when decPtr   => nState <= sDecPtr;    --"<"
               when incVal   => nState <= sIncVal;    --"+"
               when decVal   => nState <= sDecVal;    --"-"
               when whileB   => nState <= sWhileB;    --"["
               when whileE   => nState <= sWhileE;    --"]"
               when printVal => nState <= sPrintVal;  --"."
               when getVal   => nState <= sGetVal;    --","
               when storeVal => nState <= sStoreVal;  --"$"
               when loadVal  => nState <= sLoadVal;   --"!"
               when halt     => nState <= sHalt;      --"\0"
               when skip     => nState <= sSkip;      --"else"
               when others   => nState <= sFetch;
            end case;

         --">"
         when sIncPtr =>
            PTR_inc <= '1';
            PTR_dec <= '0';
            PC_inc <= '1';
            PC_dec <= '0';
            nState <= sFetch;

         --"<"
         when sDecPtr =>
            PTR_inc <= '0';
            PTR_dec <= '1';
            PC_inc <= '1';
            PC_dec <= '0';
            nState <= sFetch; 

         --"+"
         when sIncVal =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            nState <= sIncVal2;
         when sIncVal2 =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            sel <= "11";
            PC_inc <= '1';
            PC_dec <= '0';
            nState <= sFetch;

         --"-"
         when sDecVal =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            nState <= sDecVal2;
         when sDecVal2 =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            sel <= "10";
            PC_inc <= '1';
            PC_dec <= '0';
            nState <= sFetch;

         --"["
            when sWhileB =>
               PC_inc <= '1';
               DATA_EN <= '1';
               DATA_RDWR <= '1';
               nstate <= sWhileB1;
            when sWhileB1 =>
               start := PC_cAddr;
               nstate <= sFetch;

         --"]"
            when sWhileE =>
               if (TMP_RDATA_test = '0') then
                  nState <= sWhileE1;
               else
                  PC_inc <= '1';
                  nstate <= sFetch;
               end if;
            when sWhileE1 =>
               if (PC_cAddr /= start) then
                  PC_dec <= '1';
                  nState <= sWhileE1;
               else
                  nState <= sFetch;
               end if;
         --"."
         when sPrintVal =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            nState <= sPrintVal2;
         when sPrintVal2 =>
            if (OUT_BUSY = '0') then
               OUT_WE <= '1';
               PC_inc <= '1';
               PC_dec <= '0';
               nState <= sFetch;
            else 
               nState <= sPrintVal2;
            end if;  

         --","
         when sGetVal =>
            IN_REQ <= '1';
            if (IN_VLD = '1') then
               DATA_EN <= '1';
               DATA_RDWR <= '0';
               sel <= "00";
               PC_inc <= '1';
               nstate <= sFetch;
            else
               nstate <= sGetVal;
            end if;

         --"$"
         when sStoreVal =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            TMP_ld <= '1';
            PC_inc <= '1';
            nstate <= SFetch;

         --"!"
         when sLoadVal =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            sel <= "01";
            PC_inc <= '1';
            nstate <= SFetch;

         --"\0"
         when sHalt =>
            nState <= sHalt;

         --"else"
         when sSkip =>
            PC_inc <= '1';
            PC_dec <= '0';
            nState <= sFetch;

         when others =>
            nState <= sFetch;

      end case;
   end process;
end behavioral;
