library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity wrapper_adder is 
  port(
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_cas_n : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC
  );

end entity;
architecture rlt of wrapper_adder is 
  component design_1_wrapper is
    port (
      DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
      DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
      DDR_cas_n : inout STD_LOGIC;
      DDR_ck_n : inout STD_LOGIC;
      DDR_ck_p : inout STD_LOGIC;
      DDR_cke : inout STD_LOGIC;
      DDR_cs_n : inout STD_LOGIC;
      DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
      DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
      DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
      DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
      DDR_odt : inout STD_LOGIC;
      DDR_ras_n : inout STD_LOGIC;
      DDR_reset_n : inout STD_LOGIC;
      DDR_we_n : inout STD_LOGIC;
      FIXED_IO_ddr_vrn : inout STD_LOGIC;
      FIXED_IO_ddr_vrp : inout STD_LOGIC;
      FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
      FIXED_IO_ps_clk : inout STD_LOGIC;
      FIXED_IO_ps_porb : inout STD_LOGIC;
      FIXED_IO_ps_srstb : inout STD_LOGIC;
      a : out STD_LOGIC_VECTOR ( 31 downto 0 );
      b : out STD_LOGIC_VECTOR ( 31 downto 0 );
      c : in STD_LOGIC_VECTOR ( 31 downto 0 );
      reset : out std_logic_vector(0 downto 0)
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
  signal reset : std_logic_vector(0 downto 0);

  begin 
    processing_module : design_1_wrapper 
      port map (
        DDR_addr =>DDR_addr,
        DDR_ba =>DDR_ba, 
        DDR_cas_n =>DDR_cas_n, 
        DDR_ck_n =>DDR_ck_n, 
        DDR_ck_p =>DDR_ck_p, 
        DDR_cke =>DDR_cke, 
        DDR_cs_n =>DDR_cs_n, 
        DDR_dm =>DDR_dm, 
        DDR_dq =>DDR_dq, 
        DDR_dqs_n =>DDR_dqs_n, 
        DDR_dqs_p =>DDR_dqs_p, 
        DDR_odt =>DDR_odt, 
        DDR_ras_n =>DDR_ras_n, 
        DDR_reset_n =>DDR_reset_n, 
        DDR_we_n =>DDR_we_n, 
        FIXED_IO_ddr_vrn =>FIXED_IO_ddr_vrn, 
        FIXED_IO_ddr_vrp =>FIXED_IO_ddr_vrp, 
        FIXED_IO_mio =>FIXED_IO_mio, 
        FIXED_IO_ps_clk =>FIXED_IO_ps_clk, 
        FIXED_IO_ps_porb =>FIXED_IO_ps_porb, 
        FIXED_IO_ps_srstb =>FIXED_IO_ps_srstb, 
        a => a, 
        b => b,
        c => c,
        reset => reset
      );
    reconfigure_module : adder 
      port map(
        a => a, 
        b => b, 
        c => c
      );
end architecture;