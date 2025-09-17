library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
library UNISIM;
use UNISIM.VComponents.all;

use work.parameters.all;

entity ManhattanDistance is
    Port (
        dataset : in std_logic_vector(total_len-1 downto 0);
        ACC_X : in std_logic_vector(word_len-1 downto 0);
        ACC_Y : in std_logic_vector(word_len-1 downto 0);
        ACC_Z : in std_logic_vector(word_len-1 downto 0);
        GYR_X : in std_logic_vector(word_len-1 downto 0);
        GYR_Y : in std_logic_vector(word_len-1 downto 0);
        GYR_Z : in std_logic_vector(word_len-1 downto 0);
        MAG_X : in std_logic_vector(word_len-1 downto 0);
        MAG_Y : in std_logic_vector(word_len-1 downto 0);
        MAG_Z : in std_logic_vector(word_len-1 downto 0);
        MDistance : out std_logic_vector(47 downto 0);
        class : out std_logic_vector(label_len-1 downto 0);
        clk, en, rst : in std_logic
    );
end ManhattanDistance;

architecture Behavioral of ManhattanDistance is
type data_array is array(0 to 8) of std_logic_vector(word_len-1 downto 0); 
type DSP48_data_array is array(0 to 8) of std_logic_vector(47 downto 0); 
type DSP48_minuend is array(0 to 8) of std_logic_vector(24 downto 0); 
type DSP48_subtrahend is array(0 to 8) of std_logic_vector(29 downto 0); 
type DSP48_INMODE_vector is array(0 to 8) of std_logic_vector(4 downto 0); 
type class_pip_array is array(0 to pipeline_len_MD) of std_logic_vector(label_len-1 downto 0); 

type CIN_vector is array(0 to 8) of std_logic; 
signal a : data_array;
signal b : data_array;
signal subtrahend : data_array;
signal minuend : data_array;
signal distX :  DSP48_data_array;
signal subtrahend_pad : DSP48_subtrahend;
signal minuend_pad : DSP48_minuend;
signal inmode : DSP48_INMODE_vector;
signal inmode_reg : DSP48_INMODE_vector;
signal cin : CIN_vector;
signal cin_reg : CIN_vector;
signal class_pip : class_pip_array;

signal pip_1_a : std_logic_vector(18 downto 0); 
signal pip_1_b : std_logic_vector(18 downto 0); 

type pip_2_array is array(0 to 1) of std_logic_vector(18 downto 0); 
signal pip_2_a : pip_2_array;
signal pip_2_b : pip_2_array;

type pip_3_array is array(0 to 2) of std_logic_vector(18 downto 0); 
signal pip_3_a : pip_3_array;
signal pip_3_b : pip_3_array;

type pip_4_array is array(0 to 3) of std_logic_vector(18 downto 0); 
signal pip_4_a : pip_4_array;
signal pip_4_b : pip_4_array;

type pip_5_array is array(0 to 4) of std_logic_vector(18 downto 0); 
signal pip_5_a : pip_5_array;
signal pip_5_b : pip_5_array;

type pip_6_array is array(0 to 5) of std_logic_vector(18 downto 0); 
signal pip_6_a : pip_6_array;
signal pip_6_b : pip_6_array;

type pip_7_array is array(0 to 6) of std_logic_vector(18 downto 0); 
signal pip_7_a : pip_7_array;
signal pip_7_b : pip_7_array;

type pip_8_array is array(0 to 7) of std_logic_vector(18 downto 0); 
signal pip_8_a : pip_8_array;
signal pip_8_b : pip_8_array;


    
component comparator is
    Port (
        a : in std_logic_vector(word_len-1 downto 0);
        b : in std_logic_vector(word_len-1 downto 0);
        minuend : out std_logic_vector(word_len-1 downto 0);
        subtrahend : out std_logic_vector(word_len-1 downto 0);
        clk : in std_logic;
        clr : in std_logic;
        en : in std_logic;
        inmode : out std_logic_vector(4 downto 0);
        cin : out std_logic
    );
end component;

begin
    
    COMP_FIRST : comparator  
                port map (
                    a => a(0), 
                    b => b(0),
                    minuend => minuend(0),
                    subtrahend => subtrahend(0),
                    clk => clk,
                    clr => rst,
                    en => en,
                    inmode => inmode(0),
                    cin => cin(0)
                );
                
    gen_comparator:
        for i in 1 to 8 generate
            COMPX : comparator  
                port map (
                    a => pip_8_a(i-1), 
                    b => pip_8_b(i-1),
                    minuend => minuend(i),
                    subtrahend => subtrahend(i),
                    clk => clk,
                    clr => rst,
                    en => en,
                    inmode => inmode(i),
                    cin => cin(i)
                );
    end generate gen_comparator;
                                              
    process(clk, rst, en)
        begin
            if(rising_edge(clk)) then
                if rst = '1' then
                    MDistance <= (others => '0');
                    class <= (others => '0');
                elsif en = '1' then
                    MDistance <= DistX(8);
                    
                    pip_8_a(0) <= a(1);
                    pip_8_b(0) <= b(1);
                    pip_7_a(0) <= a(2);
                    pip_7_b(0) <= b(2);
                    pip_6_a(0) <= a(3);
                    pip_6_b(0) <= b(3);
                    pip_5_a(0) <= a(4);
                    pip_5_b(0) <= b(4);
                    pip_4_a(0) <= a(5);
                    pip_4_b(0) <= b(5);
                    pip_3_a(0) <= a(6);
                    pip_3_b(0) <= b(6);      
                    pip_2_a(0) <= a(7);
                    pip_2_b(0) <= b(7);
                    pip_2_a(1) <= pip_1_a;
                    pip_2_b(1) <= pip_1_b;
                    pip_1_a <= a(8);
                    pip_1_b <= b(8);
                    
                    class_pip(0) <= dataset(total_len-1 downto total_len-label_len);
                    for i in 1 to pipeline_len_MD loop
                        class_pip(i) <= class_pip(i-1);
                    end loop;
                    class <= class_pip(pipeline_len_MD);
                    
                    b(0) <= ACC_X;
                    b(1) <= ACC_Y;
                    b(2) <= ACC_Z;
                    b(3) <= GYR_X;
                    b(4) <= GYR_Y;
                    b(5) <= GYR_Z;
                    b(6) <= MAG_X;
                    b(7) <= MAG_Y;
                    b(8) <= MAG_Z;
                    
                    for i in 0 to 8 loop
                        a(i) <= dataset(((total_len-label_len-1)-(i*word_len)) downto ((total_len-label_len-word_len)-(i*word_len)));                 
                        subtrahend_pad(i) <= (29 downto word_len => subtrahend(i)(18)) & subtrahend(i);
                        minuend_pad(i) <= (24 downto word_len => minuend(i)(18)) & minuend(i);
                        inmode_reg(i) <= inmode(i);
                        cin_reg(i) <= cin(i);
                    end loop;
                    
                   
                    for i in 1 to 7 loop
                        pip_8_a(i) <= pip_7_a(i-1);
                        pip_8_b(i) <= pip_7_b(i-1);
                    end loop;
                    
                    for i in 1 to 6 loop
                        pip_7_a(i) <= pip_6_a(i-1);
                        pip_7_b(i) <= pip_6_b(i-1);
                    end loop;
                    
                    for i in 1 to 5 loop
                        pip_6_a(i) <= pip_5_a(i-1);
                        pip_6_b(i) <= pip_5_b(i-1);
                    end loop;
                    
                    for i in 1 to 4 loop
                        pip_5_a(i) <= pip_4_a(i-1);
                        pip_5_b(i) <= pip_4_b(i-1);
                    end loop;
                    
                    for i in 1 to 3 loop
                        pip_4_a(i) <= pip_3_a(i-1);
                        pip_4_b(i) <= pip_3_b(i-1);
                    end loop;
                   
                    for i in 1 to 2 loop
                        pip_3_a(i) <= pip_2_a(i-1);
                        pip_3_b(i) <= pip_2_b(i-1);
                    end loop;
                end if;   
           end if;
    end process;
    
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_FIRST : DSP48E1
        generic map (
          -- Feature Control Attributes: Data Path Selection
          A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
          B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
          USE_DPORT => TRUE,                -- Select D port usage (TRUE or FALSE)
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
          AREG => 1,                         -- Number of pipeline stages for A (0, 1 or 2)
          BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
          BREG => 1,                         -- Number of pipeline stages for B (0, 1 or 2)
          CARRYINREG => 1,                   -- Number of pipeline stages for CARRYIN (0 or 1)
          CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
          CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
          DREG => 1,                         -- Number of pipeline stages for D (0 or 1)
          INMODEREG => 1,                    -- Number of pipeline stages for INMODE (0 or 1)
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
          PCOUT => distX(0),                   -- 48-bit output: Cascade output
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
          INMODE => inmode_reg(0),                 -- 5-bit input: INMODE control input
          OPMODE => "0000101",                 -- 7-bit input: Operation mode input
          -- Data: 30-bit (each) input: Data Ports
          A => subtrahend_pad(0),                           -- 30-bit input: A data input
          B => "000000000010000000",                           -- 18-bit input: B data input
          C => (others => '0'),                           -- 48-bit input: C data input
          CARRYIN => cin_reg(0),               -- 1-bit input: Carry input signal
          D => minuend_pad(0),                           -- 25-bit input: D data input
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
    -- End of DSP48E1_FIRST instantiation
    
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    gen_DSP48E1: for i in 1 to 7 generate
        DSP48E1_MIMU : DSP48E1
            generic map (
              -- Feature Control Attributes: Data Path Selection
              A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
              B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
              USE_DPORT => TRUE,                -- Select D port usage (TRUE or FALSE)
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
              AREG => 1,                         -- Number of pipeline stages for A (0, 1 or 2)
              BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
              BREG => 1,                         -- Number of pipeline stages for B (0, 1 or 2)
              CARRYINREG => 1,                   -- Number of pipeline stages for CARRYIN (0 or 1)
              CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
              CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
              DREG => 1,                         -- Number of pipeline stages for D (0 or 1)
              INMODEREG => 1,                    -- Number of pipeline stages for INMODE (0 or 1)
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
              PCOUT => distX(i),                   -- 48-bit output: Cascade output
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
              PCIN => distX(i-1),                     -- 48-bit input: P cascade input
              -- Control: 4-bit (each) input: Control Inputs/Status Bits
              ALUMODE => "0000",               -- 4-bit input: ALU control input
              CARRYINSEL => "000",         -- 3-bit input: Carry select input
              CLK => clk,                       -- 1-bit input: Clock input
              INMODE => inmode_reg(i),                 -- 5-bit input: INMODE control input
              OPMODE => "0010101",                 -- 7-bit input: Operation mode input
              -- Data: 30-bit (each) input: Data Ports
              A => subtrahend_pad(i),                           -- 30-bit input: A data input
              B => "000000000010000000",                           -- 18-bit input: B data input
              C => (others => '0'),                           -- 48-bit input: C data input
              CARRYIN => cin_reg(i),               -- 1-bit input: Carry input signal
              D => minuend_pad(i),                           -- 25-bit input: D data input
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
            -- End of DSP48E1_ACC_X instantiation
        end generate;
        
    -- DSP48E1: 48-bit Multi-Functional Arithmetic Block
    --          Artix-7
    -- Xilinx HDL Language Template, version 2018.3
    DSP48E1_LAST : DSP48E1
        generic map (
          -- Feature Control Attributes: Data Path Selection
          A_INPUT => "DIRECT",               -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
          B_INPUT => "DIRECT",               -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
          USE_DPORT => TRUE,                -- Select D port usage (TRUE or FALSE)
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
          AREG => 1,                         -- Number of pipeline stages for A (0, 1 or 2)
          BCASCREG => 1,                     -- Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
          BREG => 1,                         -- Number of pipeline stages for B (0, 1 or 2)
          CARRYINREG => 1,                   -- Number of pipeline stages for CARRYIN (0 or 1)
          CARRYINSELREG => 0,                -- Number of pipeline stages for CARRYINSEL (0 or 1)
          CREG => 0,                         -- Number of pipeline stages for C (0 or 1)
          DREG => 1,                         -- Number of pipeline stages for D (0 or 1)
          INMODEREG => 1,                    -- Number of pipeline stages for INMODE (0 or 1)
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
          P => DistX(8),                           -- 48-bit output: Primary data output
          -- Cascade: 30-bit (each) input: Cascade Ports
          ACIN => (others => '0'),                     -- 30-bit input: A cascade data input
          BCIN => (others => '0'),                     -- 18-bit input: B cascade input
          CARRYCASCIN => '0',       -- 1-bit input: Cascade carry input
          MULTSIGNIN => '0',         -- 1-bit input: Multiplier sign input
          PCIN => distX(7),                     -- 48-bit input: P cascade input
          -- Control: 4-bit (each) input: Control Inputs/Status Bits
          ALUMODE => "0000",               -- 4-bit input: ALU control input
          CARRYINSEL => "000",         -- 3-bit input: Carry select input
          CLK => clk,                       -- 1-bit input: Clock input
          INMODE => inmode_reg(8),                 -- 5-bit input: INMODE control input
          OPMODE => "0010101",                 -- 7-bit input: Operation mode input
          -- Data: 30-bit (each) input: Data Ports
          A => subtrahend_pad(8),                           -- 30-bit input: A data input
          B => "000000000010000000",                           -- 18-bit input: B data input
          C => (others => '0'),                           -- 48-bit input: C data input
          CARRYIN => cin_reg(8),               -- 1-bit input: Carry input signal
          D => minuend_pad(8),                           -- 25-bit input: D data input
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
    -- End of DSP48E1_LAST instantiation
    
end Behavioral;
