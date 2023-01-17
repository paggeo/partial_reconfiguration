library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;


entity mpsoc_wrapper is 
end entity;

architecture rlt of mpsoc_wrapper is 

component design_1_wrapper is
  port (
    a : out std_logic_vector(31 downto 0);
    b : out std_logic_vector(31 downto 0);
    c : in std_logic_vector(31 downto 0)
  );
end component;

component adder is 
  port (
    a : in std_logic_vector(31 downto 0);
    b : in std_logic_vector(31 downto 0);
    c : out std_logic_vector(31 downto 0)
  );
end component;

signal a : std_logic_vector(31 downto 0);
signal b : std_logic_vector(31 downto 0);
signal c : std_logic_vector(31 downto 0);

begin 

processing_module: design_1_wrapper
  port map(
    a => a, 
    b => b, 
    c => c
  );

reconfigurable_logic : adder
  port map(
    a => a, 
    b => b, 
    c => c
  );

end architecture;