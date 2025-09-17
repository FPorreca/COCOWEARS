library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;
use ieee.std_logic_signed.all;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity A is
    Port ( 
        A2_x : in STD_LOGIC_VECTOR (7 downto 0);
        A1_x : in STD_LOGIC_VECTOR (7 downto 0);
        clk : in STD_LOGIC;
        rst : in STD_LOGIC;
        A : out STD_LOGIC_VECTOR (13 downto 0)
    );
end A;

architecture Behavioral of A is  
     
begin

    pip: process(clk)
            begin
                if (rising_edge(clk)) then
                    A <= ((13 downto  8 => A2_x(7)) & A2_x(6 downto 0) & '0') - ((13 downto  8 => A1_x(7)) & A1_x(6 downto 0) & '0');
                end if;          
    end process;
    
end Behavioral;
