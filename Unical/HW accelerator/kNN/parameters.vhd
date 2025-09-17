library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package parameters is
    constant attributes : integer := 9;
    constant word_len : integer := 19;
    constant label_len : integer := 2;
    constant k : integer := 5;
    constant n_class : integer := 2**label_len;
    constant total_len : integer := word_len*attributes+label_len;
    
    constant pipeline_len_MD : integer := 13;
    type min_array_class is array (0 to k-1) of std_logic_vector(label_len-1 downto 0);    
end parameters;

package body parameters is

end parameters;