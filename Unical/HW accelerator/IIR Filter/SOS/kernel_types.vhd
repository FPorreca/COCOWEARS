library ieee;
use ieee.std_logic_1164.all;

package kernel_types is
    type COEFF_ARRAY is array (0 to 2) of std_logic_vector(29 downto 0);
    type COEFF_MATRIX is array (0 to 3) of COEFF_ARRAY;
end package;

package body kernel_types is
end package body;
