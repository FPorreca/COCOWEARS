library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

use work.parameters.all;

entity voter is
    Port (
        class_array : in min_array_class;
        clk : in std_logic;
        rst : in std_logic;
        en : in std_logic;
        predicted_class : out std_logic_vector(label_len-1 downto 0)
    );
end voter;

architecture Behavioral of voter is
type count_array is array(0 to k-1) of integer;
signal count : count_array := (others => 0);
signal max_count : integer := 0;

begin

    process(clk, en, rst, class_array, count)
        variable temp_count, temp_max_count : integer;
        variable temp_most_frequent_value : std_logic_vector(label_len-1 downto 0) := (others => '0');
        begin
            if(rising_edge(clk)) then
                if(rst = '1') then
                    predicted_class <= (others => '0');
                    max_count <= 0;
                    for i in 0 to k-1 loop
                        count(i) <= 0;
                    end loop;
                elsif(en = '1') then
                    for i in 0 to k-1 loop
                        temp_count := 0;
                        for j in 0 to n_class-1 loop
                            if class_array(i) = class_array(j) then
                                temp_count := temp_count + 1;
                            end if;
                        end loop;
                        count(i) <= temp_count;
                    end loop;
                    temp_max_count := 0;
                    for i in 0 to k-1 loop
                        if count(i) > temp_max_count then
                            temp_max_count := count(i);
                            temp_most_frequent_value := class_array(i);
                        end if;
                    end loop;
                    max_count <= temp_max_count;
                    predicted_class <= temp_most_frequent_value;
                end if;
            end if;
    end process;

end Behavioral;
