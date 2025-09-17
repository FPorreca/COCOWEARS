library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

use work.parameters.all;

entity testbench is
--  Port ( );
end testbench;

architecture Behavioral of testbench is
constant clk_period : time := 20 ns;
constant start_offset : Time := 347 ns;
 
signal clk : std_logic;
signal control_signal : std_logic_vector(1 downto 0);
signal ACC_X : std_logic_vector(word_len-1 downto 0);
signal ACC_Y : std_logic_vector(word_len-1 downto 0);
signal ACC_Z : std_logic_vector(word_len-1 downto 0);
signal GYR_X : std_logic_vector(word_len-1 downto 0);
signal GYR_Y : std_logic_vector(word_len-1 downto 0);
signal GYR_Z : std_logic_vector(word_len-1 downto 0);
signal MAG_X : std_logic_vector(word_len-1 downto 0);
signal MAG_Y : std_logic_vector(word_len-1 downto 0);
signal MAG_Z : std_logic_vector(word_len-1 downto 0);
signal MDistance : STD_LOGIC_VECTOR(word_len-1 DOWNTO 0);
signal valid : std_logic;
           
component kNN is
    Port (
        clk : in std_logic;
        control_signal : in std_logic_vector(1 downto 0);
        ACC_X : in std_logic_vector(word_len-1 downto 0);
        ACC_Y : in std_logic_vector(word_len-1 downto 0);
        ACC_Z : in std_logic_vector(word_len-1 downto 0);
        GYR_X : in std_logic_vector(word_len-1 downto 0);
        GYR_Y : in std_logic_vector(word_len-1 downto 0);
        GYR_Z : in std_logic_vector(word_len-1 downto 0);
        MAG_X : in std_logic_vector(word_len-1 downto 0);
        MAG_Y : in std_logic_vector(word_len-1 downto 0);
        MAG_Z : in std_logic_vector(word_len-1 downto 0);
        MDistance : OUT STD_LOGIC_VECTOR(word_len-1 DOWNTO 0);
        valid: out std_logic
    );
end component;

begin
    uut: kNN
        port map (
            ACC_X => ACC_X,
            ACC_Y => ACC_Y,
            ACC_Z => ACC_Z,
            GYR_X => GYR_X,
            GYR_Y => GYR_Y,
            GYR_Z => GYR_Z,
            MAG_X => MAG_X,
            MAG_Y => MAG_Y,
            MAG_Z => MAG_Z,
            clk => clk,
            control_signal => control_signal,
            MDistance => MDistance,
            valid => valid
        );
    
    clk_process : process
        begin
            clk <= '0';
            wait for clk_period/2;  --for 0.5 ns signal is '0'.
            clk <= '1';
            wait for clk_period/2;  --for next 0.5 ns signal is '1'.
    end process;

    stimulus: process
        begin			
		--stimulus
		--wait for 127ns; 
		control_signal <= "01"; --reset
		ACC_X <= "1111111110101000111";
        ACC_Y <= "0000000000100010000";
        ACC_Z <= "0000000001111100010";
        GYR_X <= "0000000110000100111";
        GYR_Y <= "0000000010010100000";
        GYR_Z <= "0000000010001100111";
        MAG_X <= "0000000010101011000";
        MAG_Y <= "0000000010010000000";
        MAG_Z <= "1111110110000101000";
		wait for 55ns;
		control_signal <= "10"; -- enable
		--wait for 11ns;
--		wait for 81000ns;
--		control_signal <= "01"; --reset
--		wait for 348ns;
--		control_signal <= "10"; --enable
--		ACC_X <= "1111111111000101111";
--        ACC_Y <= "0000000000100000101";
--        ACC_Z <= "0000000010001100010";
--        GYR_X <= "1111111111110001000";
--        GYR_Y <= "1111111110100101000";
--        GYR_Z <= "1111111111111001000";
--        MAG_X <= "0000000000111000101";
--        MAG_Y <= "1111111100100010000";
--        MAG_Z <= "1111110111010111001";
--        wait for 72313ns;
--        control_signal <= "01";
--		wait for 129ns;
--		control_signal <= "10";
--		ACC_X <= "1111111110101100110";
--        ACC_Y <= "0000000000011001000";
--        ACC_Z <= "0000000010000010011";
--        GYR_X <= "0000000011001110000";
--        GYR_Y <= "0000000001101000111";
--        GYR_Z <= "0000000010110111000";
--        MAG_X <= "0000000010110000111";
--        MAG_Y <= "0000000010000000000";
--        MAG_Z <= "1111110110001100000";
		wait;
    end process;

end Behavioral;
