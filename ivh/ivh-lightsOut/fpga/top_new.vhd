library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use work.vga_controller_cfg.all;
use work.m_pack.all;
use work.modulo.all;

architecture main of tlv_pc_ifc is

   --- VGA signals
   signal vga_mode  : std_logic_vector(60 downto 0); -- default 640x480x60

   signal row : std_logic_vector(11 downto 0);
   signal col : std_logic_vector(11 downto 0);

   signal irgb : std_logic_vector(8 downto 0);
   signal crgb : std_logic_vector(8 downto 0);

   alias red is irgb(8 downto 6);
   alias green is irgb(5 downto 3);
   alias blue is irgb(2 downto 0);

   signal gray : std_logic_vector(8 downto 0);
   signal yellow : std_logic_vector(8 downto 0);

   --- KEYBOARD
   signal kbrd_data_out : std_logic_vector(15 downto 0);
   signal kbrd_data_vld : std_logic;

   --- CELL
   signal KEYS            : STD_LOGIC_VECTOR (4 downto 0);

   signal NUMBER1         : STD_LOGIC_VECTOR (2 downto 0);
   signal NUMBER2         : STD_LOGIC_VECTOR (2 downto 0);
   signal NUMBER3         : STD_LOGIC_VECTOR (2 downto 0);

   signal CELL_RST           : STD_LOGIC;
   signal INV_REQ            : STD_LOGIC_VECTOR (99 downto 0);
   signal SEL_REQ            : STD_LOGIC_VECTOR (99 downto 0);
   signal ARRAY_INIT_ACTIVE   : STD_LOGIC_VECTOR (24 downto 0);
   signal ARRAY_INIT_SELECTED : STD_LOGIC_VECTOR (24 downto 0);
   signal ARRAY_ACTIVE        : STD_LOGIC_VECTOR (24 downto 0);
   signal ARRAY_SELECTED      : STD_LOGIC_VECTOR (24 downto 0);

   constant IDX_TOP    : NATURAL := 0; -- index sousedni bunky nachazejici se nahore v signalech *_REQ_IN a *_REQ_OUT, index klavesy posun nahoru v KEYS
   constant IDX_LEFT   : NATURAL := 1; -- ... totez        ...                vlevo
   constant IDX_RIGHT  : NATURAL := 2; -- ... totez        ...                vpravo
   constant IDX_BOTTOM : NATURAL := 3; -- ... totez        ...                dole

   constant IDX_ENTER  : NATURAL := 4; -- index klavesy v KEYS, zpusobujici inverzi bunky (enter, klavesa 5)
  
   -- ----------------------------------------------------------
   -- ------------- GLOBAL MODULES -----------------------------
   -- ----------------------------------------------------------

begin

   -- Nastaveni grafickeho rezimu (640x480, 60 Hz refresh)
   setmode(r640x480x60, vga_mode);

      -- VGA controller
   vga: entity work.vga_controller(arch_vga_controller) 
      generic map (REQ_DELAY => 1)
      port map (
         CLK    => CLK, 
         RST    => RESET,
         ENABLE => '1',
         MODE   => vga_mode,

         DATA_RED    => red,
         DATA_GREEN  => irgb(5 downto 3),
         DATA_BLUE   => irgb(2 downto 0),
         ADDR_COLUMN => col,
         ADDR_ROW    => row,

         VGA_RED   => RED_V,
         VGA_BLUE  => BLUE_V,
         VGA_GREEN => GREEN_V,
         VGA_HSYNC => HSYNC_V,
         VGA_VSYNC => VSYNC_V
      );

      -- Keyboard controller
   kbrd_ctrl: entity work.keyboard_controller(arch_keyboard)
      port map (
         CLK => SMCLK,
         RST => RESET,

         DATA_OUT => kbrd_data_out(15 downto 0),
         DATA_VLD => kbrd_data_vld,
         
         KB_KIN   => KIN,
         KB_KOUT  => KOUT
      );

      -- BCD counter
   bcd: entity work.bcd
      port map ( 
      CLK => CLK,
      RESET => RESET,
      NUMBER1 => NUMBER1,
      NUMBER2 => NUMBER2,
      NUMBER3 => NUMBER3
      );

--- FOR GENERATE 25 CELLS

   column : for y in 0 to 4 generate begin
      row : for x in 0 to 4 generate begin

         cell_xy:  entity work.cell
                   generic map ( MASK => getmask(x,y,5,5) )
                   port map ( KEYS => KEYS, RESET => CELL_RST, CLK => clk,

                              INVERT_REQ_IN(IDX_TOP)     => INV_REQ(cnt_index(x,     y - 1, IDX_BOTTOM)),
                              INVERT_REQ_IN(IDX_LEFT)    => INV_REQ(cnt_index(x - 1, y,     IDX_RIGHT)),
                              INVERT_REQ_IN(IDX_RIGHT)   => INV_REQ(cnt_index(x + 1, y,     IDX_LEFT)),
                              INVERT_REQ_IN(IDX_BOTTOM)  => INV_REQ(cnt_index(x,     y + 1, IDX_TOP)),

                              INVERT_REQ_OUT => INV_REQ(((x + y * 5) * 4 + 3) downto ((x + y * 5)*4)),

                              SELECT_REQ_IN(IDX_TOP)     => SEL_REQ(cnt_index(x,     y - 1, IDX_BOTTOM)),
                              SELECT_REQ_IN(IDX_LEFT)    => SEL_REQ(cnt_index(x - 1, y,     IDX_RIGHT)),
                              SELECT_REQ_IN(IDX_RIGHT)   => SEL_REQ(cnt_index(x + 1, y,     IDX_LEFT)),
                              SELECT_REQ_IN(IDX_BOTTOM)  => SEL_REQ(cnt_index(x,     y + 1, IDX_TOP)),

                              SELECT_REQ_OUT => SEL_REQ(((x + y * 5) * 4 + 3) downto ((x + y * 5) * 4)),

                              INIT_ACTIVE => ARRAY_INIT_ACTIVE(x + y * 5),
                              ACTIVE => ARRAY_ACTIVE(x + y * 5),

                              INIT_SELECTED => ARRAY_INIT_SELECTED (x + y * 5),
                              SELECTED => ARRAY_SELECTED(x + y * 5));
      end generate row;
   end generate column;


   crgb <= "000000000";

-- cursor controller, move to CLK
   cursor: process
      variable in_access : std_logic := '0';
   begin
      if CLK'event and CLK='1' then
         KEYS <= "00000";
         CELL_RST <= '0';
                  
         if in_access='0' then
            if kbrd_data_vld='1' then 
               in_access:='1';
               if kbrd_data_out(9)='1' then  -- key 6
                  KEYS(IDX_RIGHT) <= '1';
               elsif kbrd_data_out(1)='1' then  -- key 4
                  KEYS(IDX_LEFT) <= '1';
               elsif kbrd_data_out(4)='1' then  -- key 2
                  KEYS(IDX_TOP) <= '1';
               elsif kbrd_data_out(6)='1' then  -- key 8
                  KEYS(IDX_BOTTOM) <= '1';
               elsif kbrd_data_out(5)='1' then     -- key 5
                  KEYS(IDX_ENTER) <= '1';                
               
               elsif kbrd_data_out(12)='1' then    -- key A
                  ARRAY_INIT_ACTIVE <= "1101110101011101010111011";
                  ARRAY_INIT_SELECTED <= "0000000000001000000000000";
                  CELL_RST <= '1';
                              
               end if;
            end if;
         else
            if kbrd_data_vld='0' then 
               in_access:='0';
            end if;
         end if;
      end if;
      
   end process;


--- VGA process
process(CLK)
begin
   gray <= "010010010";
   yellow <= "111111001";

   if CLK'event and CLK='1' then

      if (row >= 80  and row <= 144 and col >= 160 and col <= 224) then
         irgb <= gray;
      elsif (row >= 80  and row <= 144 and col >= 225 and col <= 288) then
         irgb <= yellow;
      elsif (row >= 80  and row <= 144 and col >= 289 and col <= 352) then
         irgb <= gray;
      elsif (row >= 80  and row <= 144 and col >= 353 and col <= 416) then
         irgb <= yellow;
      elsif (row >= 80  and row <= 144 and col >= 417 and col <= 480) then
         irgb <= gray;
      elsif (row >= 145  and row <= 208 and col >= 160 and col <= 224) then
         if (ARRAY_ACTIVE(5) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 145  and row <= 208 and col >= 225 and col <= 288) then
         if (ARRAY_ACTIVE(6) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 145  and row <= 208 and col >= 289 and col <= 352) then
         if (ARRAY_ACTIVE(7) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 145  and row <= 208 and col >= 353 and col <= 416) then
         if (ARRAY_ACTIVE(8) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 145  and row <= 208 and col >= 417 and col <= 480) then
         if (ARRAY_ACTIVE(9) = '1') then irgb <= yellow; else irgb <= gray; end if;

      elsif (row >= 209  and row <= 272 and col >= 160 and col <= 224) then
         if (ARRAY_ACTIVE(10) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 209  and row <= 272 and col >= 225 and col <= 288) then
         if (ARRAY_ACTIVE(11) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 209  and row <= 272 and col >= 289 and col <= 352) then
         if (ARRAY_ACTIVE(12) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 209  and row <= 272 and col >= 353 and col <= 416) then
         if (ARRAY_ACTIVE(13) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 209  and row <= 272 and col >= 417 and col <= 480) then
         if (ARRAY_ACTIVE(14) = '1') then irgb <= yellow; else irgb <= gray; end if;

      elsif (row >= 273  and row <= 334 and col >= 160 and col <= 224) then
        if (ARRAY_ACTIVE(15) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 273  and row <= 334 and col >= 225 and col <= 288) then
        if (ARRAY_ACTIVE(16) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 273  and row <= 334 and col >= 289 and col <= 352) then
        if (ARRAY_ACTIVE(17) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 273  and row <= 334 and col >= 353 and col <= 416) then
        if (ARRAY_ACTIVE(18) = '1') then irgb <= yellow; else irgb <= gray; end if;
      elsif (row >= 273  and row <= 334 and col >= 417 and col <= 480) then
        if (ARRAY_ACTIVE(19) = '1') then irgb <= yellow; else irgb <= gray; end if;

      elsif (row >= 335  and row <= 400 and col >= 160 and col <= 224) then
         irgb <= gray;
      elsif (row >= 335  and row <= 400 and col >= 225 and col <= 288) then
         irgb <= yellow;
      elsif (row >= 335  and row <= 400 and col >= 289 and col <= 352) then
         irgb <= gray;
      elsif (row >= 335  and row <= 400 and col >= 353 and col <= 416) then
         irgb <= yellow;
      elsif (row >= 335  and row <= 400 and col >= 417 and col <= 480) then
         irgb <= gray;

--- BG color
      elsif (kbrd_data_out(5) = '1') then
         irgb <= "111111111";
      else
         irgb <= "000000000";
      end if;
   end if;

end process;

end main;
