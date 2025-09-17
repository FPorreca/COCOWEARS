library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_signed.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

use work.parameters.all;

entity comparator is
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
end comparator;

architecture Behavioral of comparator is

signal minuend_reg : std_logic_vector(word_len-1 downto 0);
signal subtrahend_reg : std_logic_vector(word_len-1 downto 0);
signal c_reg : std_logic;


begin 
    process(clk, en, clr, a, b)  
        begin
            if(rising_edge(clk)) then
                cin <= a(18) and b(18);
                if(clr = '1') then 
                    minuend <= (others => '0');
                    subtrahend <= (others => '0');
                elsif(en = '1') then 
                    if(a >= b) then
                        if(a(word_len-1) = '1' and b(word_len-1) = '1') then
                            minuend <= a;
                            subtrahend <= not(b);
                            inmode <= "10100";
                        else
                            minuend <= a;
                            subtrahend <= b;
                            inmode <= "11100";
                        end if;
                    else
                        if(a(word_len-1) = '1' and b(word_len-1) = '1') then
                            minuend <= b;
                            subtrahend <= not(a);
                            inmode <= "10100";
                        else
                            minuend <= b;
                            subtrahend <= a;
                            inmode <= "11100";
                        end if;
                    end if;
                end if;
            end if;
    end process;

end Behavioral;
