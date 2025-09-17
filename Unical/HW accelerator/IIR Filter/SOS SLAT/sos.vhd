library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity sos is
    Port (
        clk : in std_logic;
        rst : in std_logic;
        x : in std_logic_vector(17 downto 0);
        y : out std_logic_vector(47 downto 0)
   ); 
end sos;

architecture Behavioral of sos is
	type K_array is array (0 to 4) of std_logic_vector(47 downto 0);
    signal k : K_array;
	
	type W_array is array (0 to 1) of std_logic_vector(47 downto 0);
    signal w : K_array;
	
	signal link : std_logic_vector(47 downto 0);
	signal link_reg : std_logic_vector(47 downto 0);
	signal x_reg : std_logic_vector(17 downto 0);
	
	
    type KERNEL_CONSTANT_A is array (0 to 2) of std_logic_vector(29 downto 0);
    constant a : KERNEL_CONSTANT_A :=  (
        "000000000000100000000000000000", 
        "000000000000011000100010100010", 
        "000000000000011010111011011011" 
    );
    
    type KERNEL_CONSTANT_B is array (0 to 4) of std_logic_vector(29 downto 0);
    constant b : KERNEL_CONSTANT_B :=  (
        "000000000000000000000111111011", 
        "000000000000000000011001100011", 
        "000000000000000000100011101111", 
        "000000000000000000011010100001", 
        "000000000000000000001000011010"  
    );

begin

    process(clk)
		begin
			if(rising_edge(clk)) then
				if rst = '1' then
					 x_reg <= (others => '0');
					 y <= (others => '0');
					 link_reg <= (others => '0');
				else
					 y <= k(0);
					 x_reg <= x;
					 link_reg <= link;
				end if;
			end if;
	end process;

	DSP48E1_A0 : DSP48E1
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
			PREG => 1                          
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
			P => link,                           
			
			ACIN => (others => '0'),                     
			BCIN => (others => '0'),                     
			CARRYCASCIN => '0',       
			MULTSIGNIN => '0',         
			PCIN => w(0),                     
			
			ALUMODE => "0000",               
			CARRYINSEL => "000",         
			CLK => clk,                       
			INMODE => "00000",                 
			OPMODE => "0010101",                 
			
			A => "000000000000100000000000000000",  
			B => x_reg,                           
			C => (others => '0'),                           
			CARRYIN => '0',               
			D => (others => '0'),                           
			
			CEA1 => '0',                     
			CEA2 => '0',                     
			CEAD => '0',                     
			CEALUMODE => '0',           
			CEB1 => '0',                     
			CEB2 => '0',                     
			CEC => '0',                       
			CECARRYIN => '0',           
			CECTRL => '0',                 
			CED => '0',                       
			CEINMODE => '0',             
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
			PREG => 1                          
			)
			port map (
			
			ACOUT => open,                   
			BCOUT => open,                   
			CARRYCASCOUT => open,     
			MULTSIGNOUT => open,       
			PCOUT => w(0),                   
			
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
			PCIN => w(1),                     
			
			ALUMODE => "0000",               
			CARRYINSEL => "000",         
			CLK => clk,                       
			INMODE => "00000",                 
			OPMODE => "0010101",                 
			
			A => a(1),  
			B => link(35 downto 18),                           
			C => (others => '0'),                           
			CARRYIN => '0',               
			D => (others => '0'),                           
			
			CEA1 => '0',                     
			CEA2 => '0',                     
			CEAD => '0',                     
			CEALUMODE => '0',           
			CEB1 => '0',                     
			CEB2 => '0',                     
			CEC => '0',                       
			CECARRYIN => '0',           
			CECTRL => '0',                 
			CED => '0',                       
			CEINMODE => '0',             
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
			MREG => 0,                         
			OPMODEREG => 0,                    
			PREG => 1                          
			)
			port map (
			
			ACOUT => open,                   
			BCOUT => open,                   
			CARRYCASCOUT => open,     
			MULTSIGNOUT => open,       
			PCOUT => w(1),                   
			
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
			B => link_reg(35 downto 18),                           
			C => (others => '0'),                           
			CARRYIN => '0',               
			D => (others => '0'),                           
			
			CEA1 => '1',                     
			CEA2 => '1',                     
			CEAD => '0',                     
			CEALUMODE => '0',           
			CEB1 => '1',                     
			CEB2 => '1',                     
			CEC => '0',                       
			CECARRYIN => '0',           
			CECTRL => '0',                 
			CED => '0',                       
			CEINMODE => '0',             
			CEM => '0',                       
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
			PREG => 1                          
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
			INMODE => "00000",                 
			OPMODE => "0010101",                 
			
			A => b(0),                           
			B => link(35 downto 18),                           
			C => (others => '0'),                           
			CARRYIN => '0',               
			D => (others => '0'),                           
			
			CEA1 => '0',                     
			CEA2 => '0',                     
			CEAD => '0',                     
			CEALUMODE => '0',           
			CEB1 => '0',                     
			CEB2 => '0',                     
			CEC => '0',                       
			CECARRYIN => '0',           
			CECTRL => '0',                 
			CED => '0',                       
			CEINMODE => '0',             
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





gen_DSP48E1_B: for i in  1 to 3 generate
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
			PREG => 1                          
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
			OPMODE => "0010101",                 
			
			A => b(i),                           
			B => link(35 downto 18),                           
			C => (others => '0'),                           
			CARRYIN => '0',               
			D => (others => '0'),                           
			
			CEA1 => '0',                     
			CEA2 => '0',                     
			CEAD => '0',                     
			CEALUMODE => '0',           
			CEB1 => '0',                     
			CEB2 => '0',                     
			CEC => '0',                       
			CECARRYIN => '0',           
			CECTRL => '0',                 
			CED => '0',                       
			CEINMODE => '0',             
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

	
	DSP48E1_B4 : DSP48E1
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
			PREG => 1                          
			)
			port map (
			
			ACOUT => open,                   
			BCOUT => open,                   
			CARRYCASCOUT => open,     
			MULTSIGNOUT => open,       
			PCOUT => k(4),                   
			
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
			
			A => b(4),                           
			B => link(35 downto 18),                           
			C => (others => '0'),                           
			CARRYIN => '0',               
			D => (others => '0'),                           
			
			CEA1 => '0',                     
			CEA2 => '0',                     
			CEAD => '0',                     
			CEALUMODE => '0',           
			CEB1 => '0',                     
			CEB2 => '0',                     
			CEC => '0',                       
			CECARRYIN => '0',           
			CECTRL => '0',                 
			CED => '0',                       
			CEINMODE => '0',             
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

