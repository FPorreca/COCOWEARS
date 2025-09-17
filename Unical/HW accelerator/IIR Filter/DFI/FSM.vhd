library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package FSM_Package is
    type dyn_opmode_array is array (0 to 8) of STD_LOGIC_VECTOR(6 downto 0);
end FSM_Package;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.FSM_Package.all;

entity FSM is
    Generic (
        N : integer := 9 
    );
    Port ( 
       clk : in STD_LOGIC;
       rst : in STD_LOGIC;
       opmodes : out dyn_opmode_array
    );
end FSM;

architecture Behavioral of FSM is
    signal opmodes_internal : dyn_opmode_array := (others => "0000000");
    
    signal index : integer range 1 to N-1 := 1;
    
    type state is (idle, S0, S1);
    
    signal present_state : state := idle;
    signal next_state : state := idle;
    
    begin
        
        fsm_present_state : process(clk, rst)
            begin
                if rising_edge(clk) then
                    if rst = '1' then
                        present_state <= idle;
                    else
                        present_state <= next_state;
                    end if;
                end if;
        end process;
        
        
        fsm_next_state : process(present_state, index, rst)
            begin
                if rst = '1' then
                    next_state <= idle; 
                else
                    case present_state is
                        when idle =>
                            next_state <= S0;
                        when S0 =>
                            if index = N then
                                next_state <= S1;
                            else
                                next_state <= S0;
                            end if;
                        when S1 =>
                            next_state <= S1;
                        when others =>
                            next_state <= idle;
                    end case;
                end if;
            end process;
        
        
        fsm_outputs : process(clk, rst)
            begin
                if rst = '1' then
                    opmodes_internal <= (others => "0000000"); 
                    opmodes_internal(0) <= "0010101";
                    index <= 1;
                elsif rising_edge(clk) then
                    case present_state is
                        when idle =>
                            opmodes_internal <= (others => "0000000"); 
                            opmodes_internal(0) <= "0010101";
                            index <= 1;
                        when S0 =>
                            if index < N then
                                opmodes_internal(index) <= "0010101"; 
                                index <= index + 1;
                            end if;
                        when S1 =>
                            opmodes_internal <= (others => "0010101"); 
                        when others =>
                            opmodes_internal <= (others => "0000000"); 
                            opmodes_internal(0) <= "0010101";
                    end case;
                end if;
            end process;
        opmodes <= opmodes_internal;
end Behavioral;
