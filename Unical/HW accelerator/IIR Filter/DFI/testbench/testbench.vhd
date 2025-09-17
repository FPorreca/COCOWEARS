library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity testbench is
end entity testbench;

architecture sim of testbench is
    -- Component declaration for the Unit Under Test (UUT)
    component iir_filter
        port (
            clk : in std_logic;
            rst : in std_logic;
            x : in std_logic_vector(17 downto 0);
            y : out std_logic_vector(47 downto 0)
        );
    end component;
    
    -- Signals to connect to UUT
    signal clk : std_logic := '0';
    signal rst : std_logic := '1';
    signal x : std_logic_vector(17 downto 0) := (others => '0');
    signal y : std_logic_vector(47 downto 0);

    -- Clock period
    constant clk_period : time := 24 ns;
    
begin
    -- Clock generation
    clk_process : process
    begin
        clk <= '0';
        wait for clk_period/2;
        clk <= '1';
        wait for clk_period/2; 
    end process;

    -- UUT instantiation
    uut: iir_filter
        port map (
            clk => clk,
            rst => rst,
            x => x,
            y => y
        );

    -- Stimulus process
    stim_proc: process
    begin
        -- Initialize reset
        wait for 102 ns;
        rst <= '0';
        
        -- Apply test vectors
        -- Feel free to modify these values based on your specific filter configuration
        x <= "000000010000000000"; -- x[0] = 1
        wait for clk_period;
        x <= "000000100000000000"; -- x[1] = 2
        wait for clk_period;
        x <= "000001000000000000"; -- x[2] = 4
        wait for clk_period;
        x <= "000010000000000000"; -- x[3] = 8
        wait for clk_period;
        
        x <= "000000010000000000"; -- x[0] = 1
        wait for clk_period;
        x <= "000000100000000000"; -- x[1] = 2
        wait for clk_period;
        x <= "000001000000000000"; -- x[2] = 4
        wait for clk_period;
        x <= "000010000000000000"; -- x[3] = 8
        
--        x <= "000100000000000000"; -- x[4] = 16
--        wait for clk_period;
--        x <= "001000000000000000"; -- x[5] = 32
--        wait for clk_period;
--        x <= "010000000000000000"; -- x[6] = 64
--        wait for clk_period;
--        x <= "111111110000000000"; -- x[8] = -1
--        wait for clk_period;
--        x <= "000011110000000000"; -- x[9] = 15
--        wait for clk_period;
--        x <= "111100000000000000"; -- x[10] = -16
--        wait for clk_period;
--        x <= "101010110000000000"; -- x[11] = -86
--        wait for clk_period;
--        x <= "010101010000000000"; -- x[12] = 85
--        wait for clk_period;
--        x <= "001100110000000000"; -- x[13] = 51
--        wait for clk_period;
--        x <= "110011000000000000"; -- x[14] = -52
--        wait for clk_period*2;
        --rst <= '1';
        -- End simulation
        wait;
    end process;
end architecture sim;
