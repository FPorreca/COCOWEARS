library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
library UNISIM;
use UNISIM.VComponents.all;



entity C is
    Port ( 
        dist_1 : in STD_LOGIC_VECTOR (7 downto 0);
        dist_2 : in STD_LOGIC_VECTOR (7 downto 0);
        A1_x : in STD_LOGIC_VECTOR (7 downto 0);
        A2_x : in STD_LOGIC_VECTOR (7 downto 0);
        A1_y : in STD_LOGIC_VECTOR (7 downto 0);
        A2_y : in STD_LOGIC_VECTOR (7 downto 0);
        clk : in STD_LOGIC;
        rst : in STD_LOGIC;
        C : out STD_LOGIC_VECTOR (13 downto 0)
    );
end C;

architecture Behavioral of C is
    signal dist_1_pad_opA : STD_LOGIC_VECTOR (29 downto 0);
    signal dist_1_pad_opB : STD_LOGIC_VECTOR (17 downto 0);
    
    signal dist_2_pad_opA : STD_LOGIC_VECTOR (29 downto 0);  
    signal dist_2_pad_opB : STD_LOGIC_VECTOR (17 downto 0);
    
    signal A1_x_pad_opA : STD_LOGIC_VECTOR (29 downto 0);  
    signal A1_x_pad_opB : STD_LOGIC_VECTOR (17 downto 0);
    
    signal A2_x_pad_opA : STD_LOGIC_VECTOR (29 downto 0);  
    signal A2_x_pad_opB : STD_LOGIC_VECTOR (17 downto 0);
    
    signal A1_y_pad_opA : STD_LOGIC_VECTOR (29 downto 0);  
    signal A1_y_pad_opB : STD_LOGIC_VECTOR (17 downto 0);
    
    signal A2_y_pad_opA : STD_LOGIC_VECTOR (29 downto 0);  
    signal A2_y_pad_opB : STD_LOGIC_VECTOR (17 downto 0);
    
    signal dist_1_out : STD_LOGIC_VECTOR (47 downto 0);
    signal dist_2_out : STD_LOGIC_VECTOR (47 downto 0);
    signal A1_x_out : STD_LOGIC_VECTOR (47 downto 0);
    signal A2_x_out : STD_LOGIC_VECTOR (47 downto 0);
    signal A1_y_out : STD_LOGIC_VECTOR (47 downto 0);
    signal DSP_OUT : STD_LOGIC_VECTOR (47 downto 0) := (others => '0');
    
    type dist_2_opA_pip_array is array(0 to 0) of std_logic_vector(29 downto 0); -- # of stage = pipeline_stage+2
    signal dist_2_opA_pip: dist_2_opA_pip_array;
    type dist_2_opB_pip_array is array(0 to 0) of std_logic_vector(17 downto 0); -- # of stage = pipeline_stage+2
    signal dist_2_opB_pip: dist_2_opB_pip_array;
    
begin

    dist_1_pad_opA <= (29 downto 25 => '0') & (24 downto 19 => '0') & dist_1 & (10 downto 0 => '0');
    dist_1_pad_opB <= (17 downto 12 => '0') & dist_1 & (3 downto 0 => '0');
    
    PIP_dist2: process(clk)
            begin
                if (rising_edge(clk)) then
                    dist_2_pad_opA <= (29 downto 25 => '0') & (24 downto 19 => '0') & dist_2 & (10 downto 0 => '0');
                    dist_2_pad_opB <= (17 downto 12 => '0') & dist_2 & (3 downto 0 => '0');                  
                end if;
    end process; 
    
    A1_x_pad_opA <= (29 downto 25 => '0') & (24 downto 19 => '0') & A1_x & (10 downto 0 => '0');
    A1_x_pad_opB <= (17 downto 12 => '0') & A1_x & (3 downto 0 => '0');
   
    A2_x_pad_opA <= (29 downto 25 => '0') & (24 downto 19 => '0') & A2_x & (10 downto 0 => '0');
    A2_x_pad_opB <= (17 downto 12 => '0') & A2_x & (3 downto 0 => '0');
    
    A1_y_pad_opA <= (29 downto 25 => '0') & (24 downto 19 => '0') & A1_y & (10 downto 0 => '0');
    A1_y_pad_opB <= (17 downto 12 => '0') & A1_y & (3 downto 0 => '0');
    
    A2_y_pad_opA <= (29 downto 25 => '0') & (24 downto 19 => '0') & A2_y & (10 downto 0 => '0');
    A2_y_pad_opB <= (17 downto 12 => '0') & A2_y & (3 downto 0 => '0');
       
    C <= DSP_OUT(32 downto 19);
     
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_dist_1 : DSP48E1
    generic map (
      -- Feature Control Attributes: Data Path Selection
      A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
      B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
      USE_DPORT => FALSE,                -- Select D port usage (TRUE or FALSE)
      USE_MULT => "MULTIPLY",            -- Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
      USE_SIMD => "ONE48",               -- SIMD selection ("ONE48", "TWO24", "FOUR12")
      -- Pattern Detector Attributes: Pattern Detection Configuration
      AUTORESET_PATDET => "NO_RESET",    -- "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH" 
      MASK => X"3fffffffffff",           -- 48-bit mask value for pattern detect (1=ignore)
      PATTERN => X"000000000000",        -- 48-bit pattern match for pattern detect
      SEL_MASK => "MASK",                -- "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2" 
      SEL_PATTERN => "PATTERN",          -- Select pattern value ("PATTERN" or "C")
      USE_PATTERN_DETECT => "NO_PATDET", -- Enable pattern detect ("PATDET" or "NO_PATDET")
      -- Register Control Attributes: Pipeline Register Configuration
      ACASCREG => 1,                     -- Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
      ADREG => 0,                        -- Number of pipeline stages for pre-adder (0 or 1)
      ALUMODEREG => 1,                   -- Number of pipeline stages for ALUMODE (0 or 1)
      AREG => 2,                         -- Number of pipeline stages for A (0, 1 or 2)
      BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
      BREG => 2,                         -- Number of pipeline stages for B (0, 1 or 2)
      CARRYINREG => 0,                   -- Number of pipeline stages for CARRYIN (0 or 1)
      CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
      CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
      DREG => 0,                         -- Number of pipeline stages for D (0 or 1)
      INMODEREG => 1,                    -- Number of pipeline stages for INMODE (0 or 1)
      MREG => 1,                         -- Number of multiplier pipeline stages (0 or 1)
      OPMODEREG => 1,                    -- Number of pipeline stages for OPMODE (0 or 1)
      PREG => 1                          -- Number of pipeline stages for P (0 or 1)
    )
    port map (
      -- Cascade: 30-bit (each) output: Cascade Ports
      ACOUT => open,                   -- 30-bit output: A port cascade output
      BCOUT => open,                   -- 18-bit output: B port cascade output
      CARRYCASCOUT => open,     -- 1-bit output: Cascade carry output
      MULTSIGNOUT => open,       -- 1-bit output: Multiplier sign cascade output
      PCOUT => dist_1_out,                   -- 48-bit output: Cascade output
      -- Control: 1-bit (each) output: Control Inputs/Status Bits
      OVERFLOW => open,             -- 1-bit output: Overflow in add/acc output
      PATTERNBDETECT => open, -- 1-bit output: Pattern bar detect output
      PATTERNDETECT => open,   -- 1-bit output: Pattern detect output
      UNDERFLOW => open,           -- 1-bit output: Underflow in add/acc output
      -- Data: 4-bit (each) output: Data Ports
      CARRYOUT => open,             -- 4-bit output: Carry output
      P => open,                           -- 48-bit output: Primary data output
      -- Cascade: 30-bit (each) input: Cascade Ports
      ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
      BCIN => (others => '0'),                     -- 18-bit input: B cascade input
      CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
      MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
      PCIN => (others => '0'),                     -- 48-bit input: P cascade input
      -- Control: 4-bit (each) input: Control Inputs/Status Bits
      ALUMODE => "0000",               -- 4-bit input: ALU control input
      CARRYINSEL => "000",         -- 3-bit input: Carry select input
      CLK => clk,                       -- 1-bit input: Clock input
      INMODE => "00000",                 -- 5-bit input: INMODE control input
      OPMODE => "0000101",                 -- 7-bit input: Operation mode input
      -- Data: 30-bit (each) input: Data Ports
      A => dist_1_pad_opA,                           -- 30-bit input: A data input
      B => dist_1_pad_opB,                           -- 18-bit input: B data input
      C => (others => '0'),                           -- 48-bit input: C data input
      CARRYIN => '0',               -- 1-bit input: Carry input signal
      D => (others => '0'),                           -- 25-bit input: D data input
      -- Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
      CEA1 => '1',                     -- 1-bit input: Clock enable input for 1st stage AREG
      CEA2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage AREG
      CEAD => '0',                     -- 1-bit input: Clock enable input for ADREG
      CEALUMODE => '1',           -- 1-bit input: Clock enable input for ALUMODE
      CEB1 => '1',                     -- 1-bit input: Clock enable input for 1st stage BREG
      CEB2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage BREG
      CEC => '0',                       -- 1-bit input: Clock enable input for CREG
      CECARRYIN => '1',           -- 1-bit input: Clock enable input for CARRYINREG
      CECTRL => '1',                 -- 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
      CED => '0',                       -- 1-bit input: Clock enable input for DREG
      CEINMODE => '1',             -- 1-bit input: Clock enable input for INMODEREG
      CEM => '1',                       -- 1-bit input: Clock enable input for MREG
      CEP => '1',                       -- 1-bit input: Clock enable input for PREG
      RSTA => '0',                     -- 1-bit input: Reset input for AREG
      RSTALLCARRYIN => '0',   -- 1-bit input: Reset input for CARRYINREG
      RSTALUMODE => '0',         -- 1-bit input: Reset input for ALUMODEREG
      RSTB => '0',                     -- 1-bit input: Reset input for BREG
      RSTC => '0',                     -- 1-bit input: Reset input for CREG
      RSTCTRL => '0',               -- 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
      RSTD => '0',                     -- 1-bit input: Reset input for DREG and ADREG
      RSTINMODE => '0',           -- 1-bit input: Reset input for INMODEREG
      RSTM => '0',                     -- 1-bit input: Reset input for MREG
      RSTP => '0'                      -- 1-bit input: Reset input for PREG
    );
    -- End of DSP48E1_dist_1 instantiation
    
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_dist_2 : DSP48E1
    generic map (
      -- Feature Control Attributes: Data Path Selection
      A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
      B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
      USE_DPORT => FALSE,                -- Select D port usage (TRUE or FALSE)
      USE_MULT => "MULTIPLY",            -- Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
      USE_SIMD => "ONE48",               -- SIMD selection ("ONE48", "TWO24", "FOUR12")
      -- Pattern Detector Attributes: Pattern Detection Configuration
      AUTORESET_PATDET => "NO_RESET",    -- "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH" 
      MASK => X"3fffffffffff",           -- 48-bit mask value for pattern detect (1=ignore)
      PATTERN => X"000000000000",        -- 48-bit pattern match for pattern detect
      SEL_MASK => "MASK",                -- "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2" 
      SEL_PATTERN => "PATTERN",          -- Select pattern value ("PATTERN" or "C")
      USE_PATTERN_DETECT => "NO_PATDET", -- Enable pattern detect ("PATDET" or "NO_PATDET")
      -- Register Control Attributes: Pipeline Register Configuration
      ACASCREG => 1,                     -- Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
      ADREG => 0,                        -- Number of pipeline stages for pre-adder (0 or 1)
      ALUMODEREG => 0,                   -- Number of pipeline stages for ALUMODE (0 or 1)
      AREG => 2,                         -- Number of pipeline stages for A (0, 1 or 2)
      BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
      BREG => 2,                         -- Number of pipeline stages for B (0, 1 or 2)
      CARRYINREG => 0,                   -- Number of pipeline stages for CARRYIN (0 or 1)
      CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
      CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
      DREG => 0,                         -- Number of pipeline stages for D (0 or 1)
      INMODEREG => 0,                    -- Number of pipeline stages for INMODE (0 or 1)
      MREG => 1,                         -- Number of multiplier pipeline stages (0 or 1)
      OPMODEREG => 0,                    -- Number of pipeline stages for OPMODE (0 or 1)
      PREG => 1                          -- Number of pipeline stages for P (0 or 1)
    )
    port map (
      -- Cascade: 30-bit (each) output: Cascade Ports
      ACOUT => open,                   -- 30-bit output: A port cascade output
      BCOUT => open,                   -- 18-bit output: B port cascade output
      CARRYCASCOUT => open,     -- 1-bit output: Cascade carry output
      MULTSIGNOUT => open,       -- 1-bit output: Multiplier sign cascade output
      PCOUT => dist_2_out,                   -- 48-bit output: Cascade output
      -- Control: 1-bit (each) output: Control Inputs/Status Bits
      OVERFLOW => open,             -- 1-bit output: Overflow in add/acc output
      PATTERNBDETECT => open, -- 1-bit output: Pattern bar detect output
      PATTERNDETECT => open,   -- 1-bit output: Pattern detect output
      UNDERFLOW => open,           -- 1-bit output: Underflow in add/acc output
      -- Data: 4-bit (each) output: Data Ports
      CARRYOUT => open,             -- 4-bit output: Carry output
      P => open,                           -- 48-bit output: Primary data output
      -- Cascade: 30-bit (each) input: Cascade Ports
      ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
      BCIN => (others => '0'),                     -- 18-bit input: B cascade input
      CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
      MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
      PCIN => dist_1_out,                     -- 48-bit input: P cascade input
      -- Control: 4-bit (each) input: Control Inputs/Status Bits
      ALUMODE => "0011",               -- 4-bit input: ALU control input
      CARRYINSEL => "000",         -- 3-bit input: Carry select input
      CLK => clk,                       -- 1-bit input: Clock input
      INMODE => "00000",                 -- 5-bit input: INMODE control input
      OPMODE => "0010101",                 -- 7-bit input: Operation mode input
      -- Data: 30-bit (each) input: Data Ports
      A => dist_2_pad_opA,                           -- 30-bit input: A data input
      B => dist_2_pad_opB,                           -- 18-bit input: B data input
      C => (others => '0'),                           -- 48-bit input: C data input
      CARRYIN => '0',               -- 1-bit input: Carry input signal
      D => (others => '0'),                           -- 25-bit input: D data input
      -- Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
      CEA1 => '1',                     -- 1-bit input: Clock enable input for 1st stage AREG
      CEA2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage AREG
      CEAD => '1',                     -- 1-bit input: Clock enable input for ADREG
      CEALUMODE => '1',           -- 1-bit input: Clock enable input for ALUMODE
      CEB1 => '1',                     -- 1-bit input: Clock enable input for 1st stage BREG
      CEB2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage BREG
      CEC => '1',                       -- 1-bit input: Clock enable input for CREG
      CECARRYIN => '1',           -- 1-bit input: Clock enable input for CARRYINREG
      CECTRL => '1',                 -- 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
      CED => '1',                       -- 1-bit input: Clock enable input for DREG
      CEINMODE => '1',             -- 1-bit input: Clock enable input for INMODEREG
      CEM => '1',                       -- 1-bit input: Clock enable input for MREG
      CEP => '1',                       -- 1-bit input: Clock enable input for PREG
      RSTA => '0',                     -- 1-bit input: Reset input for AREG
      RSTALLCARRYIN => '0',   -- 1-bit input: Reset input for CARRYINREG
      RSTALUMODE => '0',         -- 1-bit input: Reset input for ALUMODEREG
      RSTB => '0',                     -- 1-bit input: Reset input for BREG
      RSTC => '0',                     -- 1-bit input: Reset input for CREG
      RSTCTRL => '0',               -- 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
      RSTD => '0',                     -- 1-bit input: Reset input for DREG and ADREG
      RSTINMODE => '0',           -- 1-bit input: Reset input for INMODEREG
      RSTM => '0',                     -- 1-bit input: Reset input for MREG
      RSTP => '0'                      -- 1-bit input: Reset input for PREG
    );
    -- End of DSP48E1_dist_2 instantiation
    
     -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_A1_x : DSP48E1
    generic map (
      -- Feature Control Attributes: Data Path Selection
      A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
      B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
      USE_DPORT => FALSE,                -- Select D port usage (TRUE or FALSE)
      USE_MULT => "MULTIPLY",            -- Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
      USE_SIMD => "ONE48",               -- SIMD selection ("ONE48", "TWO24", "FOUR12")
      -- Pattern Detector Attributes: Pattern Detection Configuration
      AUTORESET_PATDET => "NO_RESET",    -- "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH" 
      MASK => X"3fffffffffff",           -- 48-bit mask value for pattern detect (1=ignore)
      PATTERN => X"000000000000",        -- 48-bit pattern match for pattern detect
      SEL_MASK => "MASK",                -- "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2" 
      SEL_PATTERN => "PATTERN",          -- Select pattern value ("PATTERN" or "C")
      USE_PATTERN_DETECT => "NO_PATDET", -- Enable pattern detect ("PATDET" or "NO_PATDET")
      -- Register Control Attributes: Pipeline Register Configuration
      ACASCREG => 1,                     -- Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
      ADREG => 0,                        -- Number of pipeline stages for pre-adder (0 or 1)
      ALUMODEREG => 0,                   -- Number of pipeline stages for ALUMODE (0 or 1)
      AREG => 2,                         -- Number of pipeline stages for A (0, 1 or 2)
      BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
      BREG => 2,                         -- Number of pipeline stages for B (0, 1 or 2)
      CARRYINREG => 0,                   -- Number of pipeline stages for CARRYIN (0 or 1)
      CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
      CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
      DREG => 0,                         -- Number of pipeline stages for D (0 or 1)
      INMODEREG => 0,                    -- Number of pipeline stages for INMODE (0 or 1)
      MREG => 1,                         -- Number of multiplier pipeline stages (0 or 1)
      OPMODEREG => 0,                    -- Number of pipeline stages for OPMODE (0 or 1)
      PREG => 1                          -- Number of pipeline stages for P (0 or 1)
    )
    port map (
      -- Cascade: 30-bit (each) output: Cascade Ports
      ACOUT => open,                   -- 30-bit output: A port cascade output
      BCOUT => open,                   -- 18-bit output: B port cascade output
      CARRYCASCOUT => open,     -- 1-bit output: Cascade carry output
      MULTSIGNOUT => open,       -- 1-bit output: Multiplier sign cascade output
      PCOUT => A1_x_out,                   -- 48-bit output: Cascade output
      -- Control: 1-bit (each) output: Control Inputs/Status Bits
      OVERFLOW => open,             -- 1-bit output: Overflow in add/acc output
      PATTERNBDETECT => open, -- 1-bit output: Pattern bar detect output
      PATTERNDETECT => open,   -- 1-bit output: Pattern detect output
      UNDERFLOW => open,           -- 1-bit output: Underflow in add/acc output
      -- Data: 4-bit (each) output: Data Ports
      CARRYOUT => open,             -- 4-bit output: Carry output
      P => open,                           -- 48-bit output: Primary data output
      -- Cascade: 30-bit (each) input: Cascade Ports
      ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
      BCIN => (others => '0'),                     -- 18-bit input: B cascade input
      CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
      MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
      PCIN => dist_2_out,                     -- 48-bit input: P cascade input
      -- Control: 4-bit (each) input: Control Inputs/Status Bits
      ALUMODE => "0011",               -- 4-bit input: ALU control input
      CARRYINSEL => "000",         -- 3-bit input: Carry select input
      CLK => clk,                       -- 1-bit input: Clock input
      INMODE => "00000",                 -- 5-bit input: INMODE control input
      OPMODE => "0010101",                 -- 7-bit input: Operation mode input
      -- Data: 30-bit (each) input: Data Ports
      A => A1_x_pad_opA,                           -- 30-bit input: A data input
      B => A1_x_pad_opB,                           -- 18-bit input: B data input
      C => (others => '0'),                           -- 48-bit input: C data input
      CARRYIN => '0',               -- 1-bit input: Carry input signal
      D => (others => '0'),                           -- 25-bit input: D data input
      -- Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
      CEA1 => '1',                     -- 1-bit input: Clock enable input for 1st stage AREG
      CEA2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage AREG
      CEAD => '1',                     -- 1-bit input: Clock enable input for ADREG
      CEALUMODE => '1',           -- 1-bit input: Clock enable input for ALUMODE
      CEB1 => '1',                     -- 1-bit input: Clock enable input for 1st stage BREG
      CEB2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage BREG
      CEC => '1',                       -- 1-bit input: Clock enable input for CREG
      CECARRYIN => '1',           -- 1-bit input: Clock enable input for CARRYINREG
      CECTRL => '1',                 -- 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
      CED => '1',                       -- 1-bit input: Clock enable input for DREG
      CEINMODE => '1',             -- 1-bit input: Clock enable input for INMODEREG
      CEM => '1',                       -- 1-bit input: Clock enable input for MREG
      CEP => '1',                       -- 1-bit input: Clock enable input for PREG
      RSTA => '0',                     -- 1-bit input: Reset input for AREG
      RSTALLCARRYIN => '0',   -- 1-bit input: Reset input for CARRYINREG
      RSTALUMODE => '0',         -- 1-bit input: Reset input for ALUMODEREG
      RSTB => '0',                     -- 1-bit input: Reset input for BREG
      RSTC => '0',                     -- 1-bit input: Reset input for CREG
      RSTCTRL => '0',               -- 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
      RSTD => '0',                     -- 1-bit input: Reset input for DREG and ADREG
      RSTINMODE => '0',           -- 1-bit input: Reset input for INMODEREG
      RSTM => '0',                     -- 1-bit input: Reset input for MREG
      RSTP => '0'                      -- 1-bit input: Reset input for PREG
    );
    -- End of DSP48E1_A1_x instantiation
    
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_A2_x : DSP48E1
    generic map (
      -- Feature Control Attributes: Data Path Selection
      A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
      B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
      USE_DPORT => FALSE,                -- Select D port usage (TRUE or FALSE)
      USE_MULT => "MULTIPLY",            -- Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
      USE_SIMD => "ONE48",               -- SIMD selection ("ONE48", "TWO24", "FOUR12")
      -- Pattern Detector Attributes: Pattern Detection Configuration
      AUTORESET_PATDET => "NO_RESET",    -- "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH" 
      MASK => X"3fffffffffff",           -- 48-bit mask value for pattern detect (1=ignore)
      PATTERN => X"000000000000",        -- 48-bit pattern match for pattern detect
      SEL_MASK => "MASK",                -- "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2" 
      SEL_PATTERN => "PATTERN",          -- Select pattern value ("PATTERN" or "C")
      USE_PATTERN_DETECT => "NO_PATDET", -- Enable pattern detect ("PATDET" or "NO_PATDET")
      -- Register Control Attributes: Pipeline Register Configuration
      ACASCREG => 1,                     -- Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
      ADREG => 0,                        -- Number of pipeline stages for pre-adder (0 or 1)
      ALUMODEREG => 0,                   -- Number of pipeline stages for ALUMODE (0 or 1)
      AREG => 2,                         -- Number of pipeline stages for A (0, 1 or 2)
      BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
      BREG => 2,                         -- Number of pipeline stages for B (0, 1 or 2)
      CARRYINREG => 0,                   -- Number of pipeline stages for CARRYIN (0 or 1)
      CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
      CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
      DREG => 0,                         -- Number of pipeline stages for D (0 or 1)
      INMODEREG => 0,                    -- Number of pipeline stages for INMODE (0 or 1)
      MREG => 1,                         -- Number of multiplier pipeline stages (0 or 1)
      OPMODEREG => 0,                    -- Number of pipeline stages for OPMODE (0 or 1)
      PREG => 1                          -- Number of pipeline stages for P (0 or 1)
    )
    port map (
      -- Cascade: 30-bit (each) output: Cascade Ports
      ACOUT => open,                   -- 30-bit output: A port cascade output
      BCOUT => open,                   -- 18-bit output: B port cascade output
      CARRYCASCOUT => open,     -- 1-bit output: Cascade carry output
      MULTSIGNOUT => open,       -- 1-bit output: Multiplier sign cascade output
      PCOUT => A2_x_out,                   -- 48-bit output: Cascade output
      -- Control: 1-bit (each) output: Control Inputs/Status Bits
      OVERFLOW => open,             -- 1-bit output: Overflow in add/acc output
      PATTERNBDETECT => open, -- 1-bit output: Pattern bar detect output
      PATTERNDETECT => open,   -- 1-bit output: Pattern detect output
      UNDERFLOW => open,           -- 1-bit output: Underflow in add/acc output
      -- Data: 4-bit (each) output: Data Ports
      CARRYOUT => open,             -- 4-bit output: Carry output
      P => open,                           -- 48-bit output: Primary data output
      -- Cascade: 30-bit (each) input: Cascade Ports
      ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
      BCIN => (others => '0'),                     -- 18-bit input: B cascade input
      CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
      MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
      PCIN => A1_x_out,                     -- 48-bit input: P cascade input
      -- Control: 4-bit (each) input: Control Inputs/Status Bits
      ALUMODE => "0000",               -- 4-bit input: ALU control input
      CARRYINSEL => "000",         -- 3-bit input: Carry select input
      CLK => clk,                       -- 1-bit input: Clock input
      INMODE => "00000",                 -- 5-bit input: INMODE control input
      OPMODE => "0010101",                 -- 7-bit input: Operation mode input
      -- Data: 30-bit (each) input: Data Ports
      A => A2_x_pad_opA,                           -- 30-bit input: A data input
      B => A2_x_pad_opB,                           -- 18-bit input: B data input
      C => (others => '0'),                           -- 48-bit input: C data input
      CARRYIN => '0',               -- 1-bit input: Carry input signal
      D => (others => '0'),                           -- 25-bit input: D data input
      -- Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
      CEA1 => '1',                     -- 1-bit input: Clock enable input for 1st stage AREG
      CEA2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage AREG
      CEAD => '1',                     -- 1-bit input: Clock enable input for ADREG
      CEALUMODE => '1',           -- 1-bit input: Clock enable input for ALUMODE
      CEB1 => '1',                     -- 1-bit input: Clock enable input for 1st stage BREG
      CEB2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage BREG
      CEC => '1',                       -- 1-bit input: Clock enable input for CREG
      CECARRYIN => '1',           -- 1-bit input: Clock enable input for CARRYINREG
      CECTRL => '1',                 -- 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
      CED => '1',                       -- 1-bit input: Clock enable input for DREG
      CEINMODE => '1',             -- 1-bit input: Clock enable input for INMODEREG
      CEM => '1',                       -- 1-bit input: Clock enable input for MREG
      CEP => '1',                       -- 1-bit input: Clock enable input for PREG
      RSTA => '0',                     -- 1-bit input: Reset input for AREG
      RSTALLCARRYIN => '0',   -- 1-bit input: Reset input for CARRYINREG
      RSTALUMODE => '0',         -- 1-bit input: Reset input for ALUMODEREG
      RSTB => '0',                     -- 1-bit input: Reset input for BREG
      RSTC => '0',                     -- 1-bit input: Reset input for CREG
      RSTCTRL => '0',               -- 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
      RSTD => '0',                     -- 1-bit input: Reset input for DREG and ADREG
      RSTINMODE => '0',           -- 1-bit input: Reset input for INMODEREG
      RSTM => '0',                     -- 1-bit input: Reset input for MREG
      RSTP => '0'                      -- 1-bit input: Reset input for PREG
    );
    -- End of DSP48E1_A2_x instantiation
    
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_A1_y : DSP48E1
    generic map (
      -- Feature Control Attributes: Data Path Selection
      A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
      B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
      USE_DPORT => FALSE,                -- Select D port usage (TRUE or FALSE)
      USE_MULT => "MULTIPLY",            -- Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
      USE_SIMD => "ONE48",               -- SIMD selection ("ONE48", "TWO24", "FOUR12")
      -- Pattern Detector Attributes: Pattern Detection Configuration
      AUTORESET_PATDET => "NO_RESET",    -- "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH" 
      MASK => X"3fffffffffff",           -- 48-bit mask value for pattern detect (1=ignore)
      PATTERN => X"000000000000",        -- 48-bit pattern match for pattern detect
      SEL_MASK => "MASK",                -- "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2" 
      SEL_PATTERN => "PATTERN",          -- Select pattern value ("PATTERN" or "C")
      USE_PATTERN_DETECT => "NO_PATDET", -- Enable pattern detect ("PATDET" or "NO_PATDET")
      -- Register Control Attributes: Pipeline Register Configuration
      ACASCREG => 1,                     -- Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
      ADREG => 0,                        -- Number of pipeline stages for pre-adder (0 or 1)
      ALUMODEREG => 0,                   -- Number of pipeline stages for ALUMODE (0 or 1)
      AREG => 2,                         -- Number of pipeline stages for A (0, 1 or 2)
      BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
      BREG => 2,                         -- Number of pipeline stages for B (0, 1 or 2)
      CARRYINREG => 0,                   -- Number of pipeline stages for CARRYIN (0 or 1)
      CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
      CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
      DREG => 0,                         -- Number of pipeline stages for D (0 or 1)
      INMODEREG => 0,                    -- Number of pipeline stages for INMODE (0 or 1)
      MREG => 1,                         -- Number of multiplier pipeline stages (0 or 1)
      OPMODEREG => 0,                    -- Number of pipeline stages for OPMODE (0 or 1)
      PREG => 1                          -- Number of pipeline stages for P (0 or 1)
    )
    port map (
      -- Cascade: 30-bit (each) output: Cascade Ports
      ACOUT => open,                   -- 30-bit output: A port cascade output
      BCOUT => open,                   -- 18-bit output: B port cascade output
      CARRYCASCOUT => open,     -- 1-bit output: Cascade carry output
      MULTSIGNOUT => open,       -- 1-bit output: Multiplier sign cascade output
      PCOUT => A1_y_out,                   -- 48-bit output: Cascade output
      -- Control: 1-bit (each) output: Control Inputs/Status Bits
      OVERFLOW => open,             -- 1-bit output: Overflow in add/acc output
      PATTERNBDETECT => open, -- 1-bit output: Pattern bar detect output
      PATTERNDETECT => open,   -- 1-bit output: Pattern detect output
      UNDERFLOW => open,           -- 1-bit output: Underflow in add/acc output
      -- Data: 4-bit (each) output: Data Ports
      CARRYOUT => open,             -- 4-bit output: Carry output
      P => open,                           -- 48-bit output: Primary data output
      -- Cascade: 30-bit (each) input: Cascade Ports
      ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
      BCIN => (others => '0'),                     -- 18-bit input: B cascade input
      CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
      MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
      PCIN => A2_x_out,                     -- 48-bit input: P cascade input
      -- Control: 4-bit (each) input: Control Inputs/Status Bits
      ALUMODE => "0011",               -- 4-bit input: ALU control input
      CARRYINSEL => "000",         -- 3-bit input: Carry select input
      CLK => clk,                       -- 1-bit input: Clock input
      INMODE => "00000",                 -- 5-bit input: INMODE control input
      OPMODE => "0010101",                 -- 7-bit input: Operation mode input
      -- Data: 30-bit (each) input: Data Ports
      A => A1_y_pad_opA,                           -- 30-bit input: A data input
      B => A1_y_pad_opB,                           -- 18-bit input: B data input
      C => (others => '0'),                           -- 48-bit input: C data input
      CARRYIN => '0',               -- 1-bit input: Carry input signal
      D => (others => '0'),                           -- 25-bit input: D data input
      -- Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
      CEA1 => '1',                     -- 1-bit input: Clock enable input for 1st stage AREG
      CEA2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage AREG
      CEAD => '1',                     -- 1-bit input: Clock enable input for ADREG
      CEALUMODE => '1',           -- 1-bit input: Clock enable input for ALUMODE
      CEB1 => '1',                     -- 1-bit input: Clock enable input for 1st stage BREG
      CEB2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage BREG
      CEC => '1',                       -- 1-bit input: Clock enable input for CREG
      CECARRYIN => '1',           -- 1-bit input: Clock enable input for CARRYINREG
      CECTRL => '1',                 -- 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
      CED => '1',                       -- 1-bit input: Clock enable input for DREG
      CEINMODE => '1',             -- 1-bit input: Clock enable input for INMODEREG
      CEM => '1',                       -- 1-bit input: Clock enable input for MREG
      CEP => '1',                       -- 1-bit input: Clock enable input for PREG
      RSTA => '0',                     -- 1-bit input: Reset input for AREG
      RSTALLCARRYIN => '0',   -- 1-bit input: Reset input for CARRYINREG
      RSTALUMODE => '0',         -- 1-bit input: Reset input for ALUMODEREG
      RSTB => '0',                     -- 1-bit input: Reset input for BREG
      RSTC => '0',                     -- 1-bit input: Reset input for CREG
      RSTCTRL => '0',               -- 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
      RSTD => '0',                     -- 1-bit input: Reset input for DREG and ADREG
      RSTINMODE => '0',           -- 1-bit input: Reset input for INMODEREG
      RSTM => '0',                     -- 1-bit input: Reset input for MREG
      RSTP => '0'                      -- 1-bit input: Reset input for PREG
    );
    -- End of DSP48E1_A1_y instantiation
    
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_A2_y : DSP48E1
    generic map (
      -- Feature Control Attributes: Data Path Selection
      A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
      B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
      USE_DPORT => FALSE,                -- Select D port usage (TRUE or FALSE)
      USE_MULT => "MULTIPLY",            -- Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
      USE_SIMD => "ONE48",               -- SIMD selection ("ONE48", "TWO24", "FOUR12")
      -- Pattern Detector Attributes: Pattern Detection Configuration
      AUTORESET_PATDET => "NO_RESET",    -- "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH" 
      MASK => X"3fffffffffff",           -- 48-bit mask value for pattern detect (1=ignore)
      PATTERN => X"000000000000",        -- 48-bit pattern match for pattern detect
      SEL_MASK => "MASK",                -- "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2" 
      SEL_PATTERN => "PATTERN",          -- Select pattern value ("PATTERN" or "C")
      USE_PATTERN_DETECT => "NO_PATDET", -- Enable pattern detect ("PATDET" or "NO_PATDET")
      -- Register Control Attributes: Pipeline Register Configuration
      ACASCREG => 1,                     -- Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
      ADREG => 0,                        -- Number of pipeline stages for pre-adder (0 or 1)
      ALUMODEREG => 0,                   -- Number of pipeline stages for ALUMODE (0 or 1)
      AREG => 2,                         -- Number of pipeline stages for A (0, 1 or 2)
      BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
      BREG => 2,                         -- Number of pipeline stages for B (0, 1 or 2)
      CARRYINREG => 0,                   -- Number of pipeline stages for CARRYIN (0 or 1)
      CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
      CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
      DREG => 0,                         -- Number of pipeline stages for D (0 or 1)
      INMODEREG => 0,                    -- Number of pipeline stages for INMODE (0 or 1)
      MREG => 1,                         -- Number of multiplier pipeline stages (0 or 1)
      OPMODEREG => 0,                    -- Number of pipeline stages for OPMODE (0 or 1)
      PREG => 1                          -- Number of pipeline stages for P (0 or 1)
    )
    port map (
      -- Cascade: 30-bit (each) output: Cascade Ports
      ACOUT => open,                   -- 30-bit output: A port cascade output
      BCOUT => open,                   -- 18-bit output: B port cascade output
      CARRYCASCOUT => open,     -- 1-bit output: Cascade carry output
      MULTSIGNOUT => open,       -- 1-bit output: Multiplier sign cascade output
      PCOUT => open,                   -- 48-bit output: Cascade output
      -- Control: 1-bit (each) output: Control Inputs/Status Bits
      OVERFLOW => open,             -- 1-bit output: Overflow in add/acc output
      PATTERNBDETECT => open, -- 1-bit output: Pattern bar detect output
      PATTERNDETECT => open,   -- 1-bit output: Pattern detect output
      UNDERFLOW => open,           -- 1-bit output: Underflow in add/acc output
      -- Data: 4-bit (each) output: Data Ports
      CARRYOUT => open,             -- 4-bit output: Carry output
      P => DSP_OUT,                           -- 48-bit output: Primary data output
      -- Cascade: 30-bit (each) input: Cascade Ports
      ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
      BCIN => (others => '0'),                     -- 18-bit input: B cascade input
      CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
      MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
      PCIN => A1_y_out,                     -- 48-bit input: P cascade input
      -- Control: 4-bit (each) input: Control Inputs/Status Bits
      ALUMODE => "0000",               -- 4-bit input: ALU control input
      CARRYINSEL => "000",         -- 3-bit input: Carry select input
      CLK => clk,                       -- 1-bit input: Clock input
      INMODE => "00000",                 -- 5-bit input: INMODE control input
      OPMODE => "0010101",                 -- 7-bit input: Operation mode input
      -- Data: 30-bit (each) input: Data Ports
      A => A2_y_pad_opA,                           -- 30-bit input: A data input
      B => A2_y_pad_opB,                           -- 18-bit input: B data input
      C => (others => '0'),                           -- 48-bit input: C data input
      CARRYIN => '0',               -- 1-bit input: Carry input signal
      D => (others => '0'),                           -- 25-bit input: D data input
      -- Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
      CEA1 => '1',                     -- 1-bit input: Clock enable input for 1st stage AREG
      CEA2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage AREG
      CEAD => '1',                     -- 1-bit input: Clock enable input for ADREG
      CEALUMODE => '1',           -- 1-bit input: Clock enable input for ALUMODE
      CEB1 => '1',                     -- 1-bit input: Clock enable input for 1st stage BREG
      CEB2 => '1',                     -- 1-bit input: Clock enable input for 2nd stage BREG
      CEC => '1',                       -- 1-bit input: Clock enable input for CREG
      CECARRYIN => '1',           -- 1-bit input: Clock enable input for CARRYINREG
      CECTRL => '1',                 -- 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
      CED => '1',                       -- 1-bit input: Clock enable input for DREG
      CEINMODE => '1',             -- 1-bit input: Clock enable input for INMODEREG
      CEM => '1',                       -- 1-bit input: Clock enable input for MREG
      CEP => '1',                       -- 1-bit input: Clock enable input for PREG
      RSTA => '0',                     -- 1-bit input: Reset input for AREG
      RSTALLCARRYIN => '0',   -- 1-bit input: Reset input for CARRYINREG
      RSTALUMODE => '0',         -- 1-bit input: Reset input for ALUMODEREG
      RSTB => '0',                     -- 1-bit input: Reset input for BREG
      RSTC => '0',                     -- 1-bit input: Reset input for CREG
      RSTCTRL => '0',               -- 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
      RSTD => '0',                     -- 1-bit input: Reset input for DREG and ADREG
      RSTINMODE => '0',           -- 1-bit input: Reset input for INMODEREG
      RSTM => '0',                     -- 1-bit input: Reset input for MREG
      RSTP => '0'                      -- 1-bit input: Reset input for PREG
    );
    -- End of DSP48E1_A2_y instantiation
    
end Behavioral;

