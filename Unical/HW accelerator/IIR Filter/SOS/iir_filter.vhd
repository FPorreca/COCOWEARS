library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.kernel_types.all;

entity iir_filter is
    Port (
        clk : in std_logic;
        rst : in std_logic;
        x : in std_logic_vector(17 downto 0); 
        y : out std_logic_vector(17 downto 0)
    ); 
end iir_filter;

architecture Behavioral of iir_filter is
    constant section : integer := 4;
    
    type intermediate_signal is array (0 to section) of std_logic_vector(17 downto 0);
    signal intermediate_y : intermediate_signal;
    signal registered_y : intermediate_signal;


constant A_MATRIX : COEFF_MATRIX := (
    0 => (
        "000000000000100000000000000000",  
        "111111111111011000110101111000",  
        "000000000000001101111011101010"  
    ),
    1 => (
        "000000000000100000000000000000",  
        "111111111111010110111010111000",  
        "000000000000010110100000100111"  
    ),
    2 => (
        "000000000000100000000000000000",  
        "111111111111001001011110110010",  
        "000000000000010111101000111011"  
    ),
    3 => (
        "000000000000100000000000000000",  
        "111111111111000011001010101000",  
        "000000000000011101100010100001"  
    )
);

constant B_MATRIX : COEFF_MATRIX := (
    0 => (
        "000000000000000000000111111011",  
        "000000000000000000001111110110",  
        "000000000000000000000111111011"  
    ),
    1 => (
        "000000000000100000000000000000",  
        "000000000001000000000000000000",  
        "000000000000100000000000000000"  
    ),
    2 => (
        "000000000000100000000000000000",  
        "111111111111000000000000000000",  
        "000000000000100000000000000000"  
    ),
    3 => (
        "000000000000100000000000000000",  
        "111111111111000000000000000000",  
        "000000000000100000000000000000"  
    )
);
       
    component sos_section is
        generic (
            a : COEFF_ARRAY;
            b : COEFF_ARRAY
        );
        Port (
            clk, rst : in std_logic;
            
            x : in std_logic_vector(17 downto 0);
            y : out std_logic_vector(17 downto 0)
        ); 
    end component;

begin
     
    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                registered_y(0) <= (others => '0');
            else
                registered_y(0) <= x;
            end if;
        end if;
    end process;
    
    GEN_SECTIONS: for i in 0 to section - 1 generate
    sos_inst : sos_section
        generic map (
            a => A_MATRIX(i),
            b => B_MATRIX(i)
        )
        port map (
            clk => clk,
            rst => rst,
            x   => registered_y(i),
            y   => intermediate_y(i+1)
        );

    REG_STAGE : process(clk)
    begin
        if rising_edge(clk) then
            registered_y(i+1) <= intermediate_y(i+1);
        end if;
    end process REG_STAGE;

end generate GEN_SECTIONS;
    
    
    process(clk)
    begin
        if rising_edge(clk) then
            y <= registered_y(section);
        end if;
    end process;

end Behavioral;
