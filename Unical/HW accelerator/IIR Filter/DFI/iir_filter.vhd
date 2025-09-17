library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package FSM_Package is
    type dyn_opmode_array is array (0 to 8) of STD_LOGIC_VECTOR(6 downto 0);
end FSM_Package;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;



use IEEE.NUMERIC_STD.ALL;



library UNISIM;
use UNISIM.VComponents.all;

use work.FSM_Package.all;

entity iir_filter is
    Port (
        clk : in std_logic;
        rst : in std_logic;
        x : in std_logic_vector(17 downto 0);
        y : out std_logic_vector(47 downto 0)
   ); 
end iir_filter;

architecture Behavioral of iir_filter is

    constant N : integer := 9;
    
    
    type KERNEL_CONSTANTS is array (0 to N-1) of std_logic_vector(29 downto 0);
    constant a : KERNEL_CONSTANTS :=  (
        "000000000000100000000000000000", 
        "111111111100111100011010001000", 
        "000000001000010100110001110101", 
        "111111110010110001011111110000", 
        "000000001101011011001110000111", 
        "111111110111000101001010010011", 
        "000000000011110010011010011000", 
        "111111111111000011110101111101", 
        "000000000000000110101011101110" 
    );

    constant b : KERNEL_CONSTANTS :=  (
        "000000000000000000000111111011", 
        "000000000000000000000000000000", 
        "111111111111111111100000010100", 
        "000000000000000000000000000000", 
        "000000000000000000101111100011", 
        "000000000000000000000000000000", 
        "111111111111111111100000010100", 
        "000000000000000000000000000000", 
        "000000000000000000000111111011"  
    );
    
    type feedback is array (1 to N) of std_logic_vector(47 downto 0);
    signal w : feedback;
    type feedforward is array (0 to N) of std_logic_vector(47 downto 0);
    signal k : feedforward;  
    
    type pip_output_array is array (0 to N - 3) of std_logic_vector(17 downto 0);
    signal pip_B_outputs : pip_output_array;
    signal pip_A_outputs : pip_output_array;
    

    signal x_reg : std_logic_vector(17 downto 0);
    



    
    signal filter_out : std_logic_vector(47 downto 0);

    signal temp : std_logic_vector(17 downto 0);
    
    signal z : std_logic_vector(47 downto 0);

    signal opmodes : dyn_opmode_array;
    
    component FSM is
        Generic (
            N : integer := 9
        );
        Port (
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            opmodes : out dyn_opmode_array
        );
    end component;

    begin
    
        filter_FSM : FSM  
            Generic map (
                N => 9
            )
            port map (
                clk => clk,
                rst => rst,
                opmodes => opmodes
            );
    








    
        k(9) <= (others => '0');
        w(9) <= (others => '0');



        temp <= filter_out(30 downto 13);
        z(47 downto 44) <= (others => w(1)(47));
        z(43 downto 0) <= w(1)(47 downto 4);

        process(clk)
            begin
                if(rising_edge(clk)) then
                    if rst = '1' then

                         x_reg <= (others => '0');
                         y <= (others => '0');
                         pip_A_outputs <= (others => (others => '0'));
                         pip_B_outputs <= (others => (others => '0'));
                    else
                         y <= filter_out;
                         



                         x_reg <= x;
                         
                         pip_A_outputs(0) <= temp;
                         

                         pip_B_outputs(0) <= x_reg;
                         for i in 1 to 6 loop
                             pip_B_outputs(i) <= pip_B_outputs(i - 1); 
                             pip_A_outputs(i) <= pip_A_outputs(i - 1); 
                         end loop;   
                    end if;
                end if;
        end process;
        
        
        
        
        DSP48E1_Y : DSP48E1
            generic map (
              
              A_INPUT => "DIRECT",               
              B_INPUT => "DIRECT",               
              USE_DPORT => FALSE,                
              USE_MULT => "NONE",            
              USE_SIMD => "ONE48",               
              
              AUTORESET_PATDET => "NO_RESET",    
              MASK => X"3fffffffffff",           
              PATTERN => X"000000000000",        
              SEL_MASK => "MASK",                
              SEL_PATTERN => "PATTERN",          
              USE_PATTERN_DETECT => "NO_PATDET", 
              
             ACASCREG => 0,                     
             ADREG => 0,                        
             ALUMODEREG => 0,                   
             AREG => 0,                         
             BCASCREG => 0,                     
             BREG => 0,                         
             CARRYINREG => 0,                   
             CARRYINSELREG => 0,                
             CREG => 0,                         
             DREG => 0,                         
             INMODEREG => 0,                    
             MREG => 0,                         
             OPMODEREG => 0,                    
             PREG => 0                          
            )
            port map (
              
              ACOUT => open,                   
              BCOUT => open,                   
              CARRYCASCOUT => open,     
              MULTSIGNOUT => open,       
              PCOUT => open,                   
              
              OVERFLOW => open,             
              PATTERNBDETECT => open, 
              PATTERNDETECT => open,   
              UNDERFLOW => open,           
              
              CARRYOUT => open,             
              P => filter_out,                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => (others => '0'),                     
              
              ALUMODE => "0011",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "00000",                 
              OPMODE => "0110011",                 
              
              A => z(47 downto 18),                           
              B => z(17 downto 0),                           
              C => k(0),                           
              CARRYIN => '0',               
              D => (others => '0'),                           
              
              CEA1 => '1',                     
              CEA2 => '1',                     
              CEAD => '1',                     
              CEALUMODE => '1',           
              CEB1 => '1',                     
              CEB2 => '1',                     
              CEC => '1',                       
              CECARRYIN => '1',           
              CECTRL => '1',                 
              CED => '1',                       
              CEINMODE => '1',             
              CEM => '1',                       
              CEP => '1',                       
              RSTA => rst,                     
              RSTALLCARRYIN => rst,   
              RSTALUMODE => rst,         
              RSTB => rst,                     
              RSTC => rst,                     
              RSTCTRL => rst,               
              RSTD => rst,                     
              RSTINMODE => rst,           
              RSTM => rst,                     
              RSTP => rst                      
            );
            
            
        
        
        
        DSP48E1_B0 : DSP48E1
            generic map (
              
              A_INPUT => "DIRECT",               
              B_INPUT => "DIRECT",               
              USE_DPORT => FALSE,                
              USE_MULT => "MULTIPLY",            
              USE_SIMD => "ONE48",               
              
              AUTORESET_PATDET => "NO_RESET",    
              MASK => X"3fffffffffff",           
              PATTERN => X"000000000000",        
              SEL_MASK => "MASK",                
              SEL_PATTERN => "PATTERN",          
              USE_PATTERN_DETECT => "NO_PATDET", 
              
             ACASCREG => 1,                     
             ADREG => 0,                        
             ALUMODEREG => 0,                   
             AREG => 1,                         
             BCASCREG => 1,                     
             BREG => 1,                         
             CARRYINREG => 0,                   
             CARRYINSELREG => 0,                
             CREG => 0,                         
             DREG => 0,                         
             INMODEREG => 0,                    
             MREG => 1,                         
             OPMODEREG => 0,                    
             PREG => 0                          
            )
            port map (
              
              ACOUT => open,                   
              BCOUT => open,                   
              CARRYCASCOUT => open,     
              MULTSIGNOUT => open,       
              PCOUT => open,                   
              
              OVERFLOW => open,             
              PATTERNBDETECT => open, 
              PATTERNDETECT => open,   
              UNDERFLOW => open,           
              
              CARRYOUT => open,             
              P => k(0),                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => k(1),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "10001",                 
              OPMODE => opmodes(0),                 
              
              A => b(0),                           
              B => x_reg,                           
              C => (others => '0'),                           
              CARRYIN => '0',               
              D => (others => '0'),                           
              
              CEA1 => '1',                     
              CEA2 => '1',                     
              CEAD => '1',                     
              CEALUMODE => '1',           
              CEB1 => '1',                     
              CEB2 => '1',                     
              CEC => '1',                       
              CECARRYIN => '1',           
              CECTRL => '1',                 
              CED => '1',                       
              CEINMODE => '1',             
              CEM => '1',                       
              CEP => '1',                       
              RSTA => rst,                     
              RSTALLCARRYIN => rst,   
              RSTALUMODE => rst,         
              RSTB => rst,                     
              RSTC => rst,                     
              RSTCTRL => rst,               
              RSTD => rst,                     
              RSTINMODE => rst,           
              RSTM => rst,                     
              RSTP => rst                      
            );
            

        
        
        
        DSP48E1_B1 : DSP48E1
            generic map (
              
              A_INPUT => "DIRECT",               
              B_INPUT => "DIRECT",               
              USE_DPORT => FALSE,                
              USE_MULT => "MULTIPLY",            
              USE_SIMD => "ONE48",               
              
              AUTORESET_PATDET => "NO_RESET",    
              MASK => X"3fffffffffff",           
              PATTERN => X"000000000000",        
              SEL_MASK => "MASK",                
              SEL_PATTERN => "PATTERN",          
              USE_PATTERN_DETECT => "NO_PATDET", 
              
             ACASCREG => 2,                     
             ADREG => 0,                        
             ALUMODEREG => 0,                   
             AREG => 2,                         
             BCASCREG => 2,                     
             BREG => 2,                         
             CARRYINREG => 0,                   
             CARRYINSELREG => 0,                
             CREG => 0,                         
             DREG => 0,                         
             INMODEREG => 0,                    
             MREG => 1,                         
             OPMODEREG => 0,                    
             PREG => 0                          
            )
            port map (
              
              ACOUT => open,                   
              BCOUT => open,                   
              CARRYCASCOUT => open,     
              MULTSIGNOUT => open,       
              PCOUT => k(1),                   
              
              OVERFLOW => open,             
              PATTERNBDETECT => open, 
              PATTERNDETECT => open,   
              UNDERFLOW => open,           
              
              CARRYOUT => open,             
              P => open,                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => k(2),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "00000",                 
              OPMODE => opmodes(1),                 
              
              A => b(1),                           
              B => x_reg,                           
              C => (others => '0'),                           
              CARRYIN => '0',               
              D => (others => '0'),                           
              
              CEA1 => '1',                     
              CEA2 => '1',                     
              CEAD => '1',                     
              CEALUMODE => '1',           
              CEB1 => '1',                     
              CEB2 => '1',                     
              CEC => '1',                       
              CECARRYIN => '1',           
              CECTRL => '1',                 
              CED => '1',                       
              CEINMODE => '1',             
              CEM => '1',                       
              CEP => '1',                       
              RSTA => rst,                     
              RSTALLCARRYIN => rst,   
              RSTALUMODE => rst,         
              RSTB => rst,                     
              RSTC => rst,                     
              RSTCTRL => rst,               
              RSTD => rst,                     
              RSTINMODE => rst,           
              RSTM => rst,                     
              RSTP => rst                      
            );
            
            
        
        
        
        gen_DSP48E1_B: for i in  2 to N-1 generate
            DSP48E1_B : DSP48E1
                generic map (
                  
                  A_INPUT => "DIRECT",               
                  B_INPUT => "DIRECT",               
                  USE_DPORT => FALSE,                
                  USE_MULT => "MULTIPLY",            
                  USE_SIMD => "ONE48",               
                  
                  AUTORESET_PATDET => "NO_RESET",    
                  MASK => X"3fffffffffff",           
                  PATTERN => X"000000000000",        
                  SEL_MASK => "MASK",                
                  SEL_PATTERN => "PATTERN",          
                  USE_PATTERN_DETECT => "NO_PATDET", 
                  
                 ACASCREG => 2,                     
                 ADREG => 0,                        
                 ALUMODEREG => 0,                   
                 AREG => 2,                         
                 BCASCREG => 2,                     
                 BREG => 2,                         
                 CARRYINREG => 0,                   
                 CARRYINSELREG => 0,                
                 CREG => 0,                         
                 DREG => 0,                         
                 INMODEREG => 0,                    
                 MREG => 1,                         
                 OPMODEREG => 0,                    
                 PREG => 0                          
                )
                port map (
                  
                  ACOUT => open,                   
                  BCOUT => open,                   
                  CARRYCASCOUT => open,     
                  MULTSIGNOUT => open,       
                  PCOUT => k(i),                   
                  
                  OVERFLOW => open,             
                  PATTERNBDETECT => open, 
                  PATTERNDETECT => open,   
                  UNDERFLOW => open,           
                  
                  CARRYOUT => open,             
                  P => open,                           
                  
                  ACIN => (others => '0'),                     
                  BCIN => (others => '0'),                     
                  CARRYCASCIN => '0',       
                  MULTSIGNIN => '0',         
                  PCIN => k(i+1),                     
                  
                  ALUMODE => "0000",               
                  CARRYINSEL => "000",         
                  CLK => clk,                       
                  INMODE => "00000",                 
                  OPMODE => opmodes(i),                 
                  
                  A => b(i),                           
                  B => pip_B_outputs(i-2),                           
                  C => (others => '0'),                           
                  CARRYIN => '0',               
                  D => (others => '0'),                           
                  
                  CEA1 => '1',                     
                  CEA2 => '1',                     
                  CEAD => '1',                     
                  CEALUMODE => '1',           
                  CEB1 => '1',                     
                  CEB2 => '1',                     
                  CEC => '1',                       
                  CECARRYIN => '1',           
                  CECTRL => '1',                 
                  CED => '1',                       
                  CEINMODE => '1',             
                  CEM => '1',                       
                  CEP => '1',                       
                  RSTA => rst,                     
                  RSTALLCARRYIN => rst,   
                  RSTALUMODE => rst,         
                  RSTB => rst,                     
                  RSTC => rst,                     
                  RSTCTRL => rst,               
                  RSTD => rst,                     
                  RSTINMODE => rst,           
                  RSTM => rst,                     
                  RSTP => rst                      
                );
                
            end generate;  

        
        
        
        DSP48E1_A1 : DSP48E1
            generic map (
              
              A_INPUT => "DIRECT",               
              B_INPUT => "DIRECT",               
              USE_DPORT => FALSE,                
              USE_MULT => "MULTIPLY",            
              USE_SIMD => "ONE48",               
              
              AUTORESET_PATDET => "NO_RESET",    
              MASK => X"3fffffffffff",           
              PATTERN => X"000000000000",        
              SEL_MASK => "MASK",                
              SEL_PATTERN => "PATTERN",          
              USE_PATTERN_DETECT => "NO_PATDET", 
              
             ACASCREG => 0,                     
             ADREG => 0,                        
             ALUMODEREG => 0,                   
             AREG => 0,                         
             BCASCREG => 0,                     
             BREG => 0,                         
             CARRYINREG => 0,                   
             CARRYINSELREG => 0,                
             CREG => 0,                         
             DREG => 0,                         
             INMODEREG => 0,                    
             MREG => 1,                         
             OPMODEREG => 0,                    
             PREG => 0                          
            )
            port map (
              
              ACOUT => open,                   
              BCOUT => open,                   
              CARRYCASCOUT => open,     
              MULTSIGNOUT => open,       
              PCOUT => open,                   
              
              OVERFLOW => open,             
              PATTERNBDETECT => open, 
              PATTERNDETECT => open,   
              UNDERFLOW => open,           
              
              CARRYOUT => open,             
              P => w(1),                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => w(2),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "00000",                 
              OPMODE => opmodes(0),                 
              
              A => a(1),                           
              B => temp,                           
              C => (others => '0'),                           
              CARRYIN => '0',               
              D => (others => '0'),                           
              
              CEA1 => '1',                     
              CEA2 => '1',                     
              CEAD => '1',                     
              CEALUMODE => '1',           
              CEB1 => '1',                     
              CEB2 => '1',                     
              CEC => '1',                       
              CECARRYIN => '1',           
              CECTRL => '1',                 
              CED => '1',                       
              CEINMODE => '1',             
              CEM => '1',                       
              CEP => '1',                       
              RSTA => rst,                     
              RSTALLCARRYIN => rst,   
              RSTALUMODE => rst,         
              RSTB => rst,                     
              RSTC => rst,                     
              RSTCTRL => rst,               
              RSTD => rst,                     
              RSTINMODE => rst,           
              RSTM => rst,                     
              RSTP => rst                      
            );
            
            
        
        
        
        DSP48E1_A2 : DSP48E1
            generic map (
              
              A_INPUT => "DIRECT",               
              B_INPUT => "DIRECT",               
              USE_DPORT => FALSE,                
              USE_MULT => "MULTIPLY",            
              USE_SIMD => "ONE48",               
              
              AUTORESET_PATDET => "NO_RESET",    
              MASK => X"3fffffffffff",           
              PATTERN => X"000000000000",        
              SEL_MASK => "MASK",                
              SEL_PATTERN => "PATTERN",          
              USE_PATTERN_DETECT => "NO_PATDET", 
              
             ACASCREG => 1,                     
             ADREG => 0,                        
             ALUMODEREG => 0,                   
             AREG => 1,                         
             BCASCREG => 1,                     
             BREG => 1,                         
             CARRYINREG => 0,                   
             CARRYINSELREG => 0,                
             CREG => 0,                         
             DREG => 0,                         
             INMODEREG => 0,                    
             MREG => 1,                         
             OPMODEREG => 0,                    
             PREG => 0                          
            )
            port map (
              
              ACOUT => open,                   
              BCOUT => open,                   
              CARRYCASCOUT => open,     
              MULTSIGNOUT => open,       
              PCOUT => w(2),                   
              
              OVERFLOW => open,             
              PATTERNBDETECT => open, 
              PATTERNDETECT => open,   
              UNDERFLOW => open,           
              
              CARRYOUT => open,             
              P => open,                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => w(3),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "00000",                 
              OPMODE => opmodes(1),                 
              
              A => a(2),                           
              B => temp,                           
              C => (others => '0'),                           
              CARRYIN => '0',               
              D => (others => '0'),                           
              
              CEA1 => '1',                     
              CEA2 => '1',                     
              CEAD => '1',                     
              CEALUMODE => '1',           
              CEB1 => '1',                     
              CEB2 => '1',                     
              CEC => '1',                       
              CECARRYIN => '1',           
              CECTRL => '1',                 
              CED => '1',                       
              CEINMODE => '1',             
              CEM => '1',                       
              CEP => '1',                       
              RSTA => rst,                     
              RSTALLCARRYIN => rst,   
              RSTALUMODE => rst,         
              RSTB => rst,                     
              RSTC => rst,                     
              RSTCTRL => rst,               
              RSTD => rst,                     
              RSTINMODE => rst,           
              RSTM => rst,                     
              RSTP => rst                      
            );
        
            
        
        
        
        gen_DSP48E1_A: for i in  3 to N-1 generate
            DSP48E1_A : DSP48E1
                generic map (
                  
                  A_INPUT => "DIRECT",               
                  B_INPUT => "DIRECT",               
                  USE_DPORT => FALSE,                
                  USE_MULT => "MULTIPLY",            
                  USE_SIMD => "ONE48",               
                  
                  AUTORESET_PATDET => "NO_RESET",    
                  MASK => X"3fffffffffff",           
                  PATTERN => X"000000000000",        
                  SEL_MASK => "MASK",                
                  SEL_PATTERN => "PATTERN",          
                  USE_PATTERN_DETECT => "NO_PATDET", 
                  
                 ACASCREG => 1,                     
                 ADREG => 0,                        
                 ALUMODEREG => 0,                   
                 AREG => 1,                         
                 BCASCREG => 1,                     
                 BREG => 1,                         
                 CARRYINREG => 0,                   
                 CARRYINSELREG => 0,                
                 CREG => 0,                         
                 DREG => 0,                         
                 INMODEREG => 0,                    
                 MREG => 1,                         
                 OPMODEREG => 0,                    
                 PREG => 0                          
                )
                port map (
                  
                  ACOUT => open,                   
                  BCOUT => open,                   
                  CARRYCASCOUT => open,     
                  MULTSIGNOUT => open,       
                  PCOUT => w(i),                   
                  
                  OVERFLOW => open,             
                  PATTERNBDETECT => open, 
                  PATTERNDETECT => open,   
                  UNDERFLOW => open,           
                  
                  CARRYOUT => open,             
                  P => open,                           
                  
                  ACIN => (others => '0'),                     
                  BCIN => (others => '0'),                     
                  CARRYCASCIN => '0',       
                  MULTSIGNIN => '0',         
                  PCIN => w(i+1),                     
                  
                  ALUMODE => "0000",               
                  CARRYINSEL => "000",         
                  CLK => clk,                       
                  INMODE => "00000",                 
                  OPMODE => opmodes(i-1),                 
                  
                  A => a(i),                           
                  B => pip_A_outputs(i-3),                           
                  C => (others => '0'),                           
                  CARRYIN => '0',               
                  D => (others => '0'),                           
                  
                  CEA1 => '1',                     
                  CEA2 => '1',                     
                  CEAD => '1',                     
                  CEALUMODE => '1',           
                  CEB1 => '1',                     
                  CEB2 => '1',                     
                  CEC => '1',                       
                  CECARRYIN => '1',           
                  CECTRL => '1',                 
                  CED => '1',                       
                  CEINMODE => '1',             
                  CEM => '1',                       
                  CEP => '1',                       
                  RSTA => rst,                     
                  RSTALLCARRYIN => rst,   
                  RSTALUMODE => rst,         
                  RSTB => rst,                     
                  RSTC => rst,                     
                  RSTCTRL => rst,               
                  RSTD => rst,                     
                  RSTINMODE => rst,           
                  RSTM => rst,                     
                  RSTP => rst                      
                );
                
            end generate;
        
end Behavioral;
