library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


library UNISIM;
use UNISIM.VComponents.all;

use work.kernel_types.all;


entity sos_section is
    generic (
        a : COEFF_ARRAY := (others => (others => '0'));
        b : COEFF_ARRAY := (others => (others => '0'))
    );
    port (
        clk : in std_logic;
        rst : in std_logic;
        x   : in std_logic_vector(17 downto 0);
        y   : out std_logic_vector(17 downto 0)
    );
end entity sos_section;

architecture Behavioral of sos_section is

    constant N : integer := 3;
    
    type W_array is array (2 to N) of std_logic_vector(47 downto 0);
    signal w : W_array;
    type K_array is array (1 to N) of std_logic_vector(47 downto 0);
    signal k : K_array;  
    
    type pip_output_array is array (0 to N - 3) of std_logic_vector(17 downto 0);
    signal pip_B_outputs : pip_output_array;
    
    signal x_reg : std_logic_vector(17 downto 0);
    
    signal feedforward : std_logic_vector(47 downto 0);
    signal feedback : std_logic_vector(47 downto 0);
    
    signal filter_out : std_logic_vector(47 downto 0);
    signal temp : std_logic_vector(17 downto 0);
    
    signal z : std_logic_vector(47 downto 0);

    begin
        k(N) <= (others => '0');
        w(N) <= (others => '0');
        temp <= filter_out(30 downto 13);
        z(47 downto 44) <= (others => feedback(47));
        z(43 downto 0) <= feedback(47 downto 4);

        process(clk)
            begin
                if(rising_edge(clk)) then
                    if rst = '1' then
                         x_reg <= (others => '0');
                         y <= (others => '0');
                         pip_B_outputs <= (others => (others => '0'));
                    else
                         y <= temp;
                         x_reg <= x;
                         pip_B_outputs(0) <= x_reg; 
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
              C => feedforward,                           
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
              P => feedforward,                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => k(1),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "10001",                 
              OPMODE => "0010101",                 
              
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
              OPMODE => "0010101",                 
              
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
                  PCIN => (others => '0'),                     
                  
                  ALUMODE => "0000",               
                  CARRYINSEL => "000",         
                  CLK => clk,                       
                  INMODE => "00000",                 
                  OPMODE => "0000101",                 
                  
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
              P => feedback,                           
              
              ACIN => (others => '0'),                     
              BCIN => (others => '0'),                     
              CARRYCASCIN => '0',       
              MULTSIGNIN => '0',         
              PCIN => w(2),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "00000",                 
              OPMODE => "0010101",                 
              
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
              PCIN => (others => '0'),                     
              
              ALUMODE => "0000",               
              CARRYINSEL => "000",         
              CLK => clk,                       
              INMODE => "00000",                 
              OPMODE => "0000101",                 
              
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
        
     
end Behavioral;
