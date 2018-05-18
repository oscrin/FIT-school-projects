library IEEE;
use IEEE.STD_LOGIC_1164.all;

package modulo is

function cnt_index(x,y,idx : natural) return natural;
end modulo;

package body modulo is

function cnt_index(x,y,idx : natural) return natural is
begin
	return (((x mod 5) + (y mod 5)*5)*4 + (idx mod 4));
end cnt_index;

end modulo;