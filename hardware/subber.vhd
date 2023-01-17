library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;


entity subber is 
  port (
    a : in std_logic_vector(31 downto 0);
    b : in std_logic_vector(31 downto 0);
    c : out std_logic_vector(31 downto 0)
  );
end entity;

architecture rtl of subber is 
begin
  main : process(a, b)
  begin 
    c <= a - b;
  end process;
end architecture;