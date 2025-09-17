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

entity preorder is
    Port (
        distance : in std_logic_vector(word_len-1 downto 0);
        class :  in std_logic_vector(label_len-1 downto 0);
        clk : in std_logic;
        rst : in std_logic;
        en : in std_logic;
        ordered_class : out min_array_class
    );
end preorder;

architecture Behavioral of preorder is
type min_array_distances is array (0 to k-1) of std_logic_vector(word_len-1 downto 0);
signal min_distances : min_array_distances;
signal preordered_class : min_array_class;

begin
    
     process(clk, en, rst, distance, min_distances)
        begin
            if(rising_edge(clk)) then
                if(rst = '1') then
                    for i in 0 to k-1 loop
                        min_distances(i) <= (others => '1');
                        preordered_class(i) <= (others => '0');
                        ordered_class(i) <= (others => '0');
                    end loop;
                elsif(en = '1') then
                    ordered_class <= preordered_class;
                    for j in 0 to k-1 loop
                        if distance < min_distances(j) then
                            for i in k-1 downto j+1 loop
                                min_distances(i) <= min_distances(i-1);
                                preordered_class(i) <= preordered_class(i-1);
                            end loop;
                            min_distances(j) <= distance;
                            preordered_class(j) <= class;
							exit;   
                        end if;
                    end loop;
                end if;
            end if;
    end process;

end Behavioral;
