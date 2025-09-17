library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity testbench is
end entity testbench;

architecture sim of testbench is
    
    component sos
        port (
            clk : in std_logic;
            rst : in std_logic;
            x : in std_logic_vector(17 downto 0);
            y : out std_logic_vector(47 downto 0)
        );
    end component;
    
    
    signal clk : std_logic := '0';
    signal rst : std_logic := '1';
    signal x : std_logic_vector(17 downto 0) := (others => '0');
    signal y : std_logic_vector(47 downto 0);

    
    constant clk_period : time := 5 ns;
    
begin
    
    clk_process : process
    begin
        clk <= '0';
        wait for clk_period/2;
        clk <= '1';
        wait for clk_period/2; 
    end process;

    
    uut: sos
        port map (
            clk => clk,
            rst => rst,
            x => x,
            y => y
        );

    
    stim_proc: process
    begin
        
        wait for 102 ns;
        rst <= '0';
        x <= "000000010000000000"; 
        wait for clk_period;
        x <= "000000100000000000"; 
        wait for clk_period;
        x <= "000001000000000000"; 
        wait for clk_period;
        x <= "000010000000000000"; 
        wait for clk_period;
        
        x <= "000000010000000000"; 
        wait for clk_period;
        x <= "000000100000000000"; 
        wait for clk_period;
        x <= "000001000000000000"; 
        wait for clk_period;
        x <= "000010000000000000"; 
        
        wait;
    end process;
end architecture sim;
