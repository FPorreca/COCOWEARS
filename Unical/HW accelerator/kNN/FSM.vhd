library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity FSM is
    Port  (
        clk : IN STD_LOGIC;
        rst, en : in std_logic;
        en_test, en_cnt_dataset, en_distance : out STD_LOGIC;
        rst_test, rst_cnt_dataset, rst_distance : out STD_LOGIC;
        valid : out std_logic
    );
end FSM;

architecture Behavioral of FSM is
type state is (idle, S0, S1, S2, S3);

signal present_state : state;
signal next_state : state;
signal count : unsigned(12 downto 0) := (others => '0');

begin

    counter : process(clk, rst, en)
        begin
            if rising_edge(clk) then
                if rst = '1' then
                    count <= (others => '0');
                elsif en = '1' then
                    count <= count  + 1;
                end if;        
            end if;
    end process;
    
                
    -- PRESENT STATE REGISTER
    fsm_present_state : process(clk, rst, en)
        begin
            if rising_edge(clk) then
                if rst = '1' then
                    present_state <= idle;
                elsif en = '1' then
                    present_state <= next_state;
                end if;
            end if;
    end process;
    
    --NEXT STATE LOGIC
    fsm_next_state : process(present_state, count, rst)
        begin
            case present_state is
                when idle =>
                        next_state <= S0;
                when S0 =>
                    if(count = "0000000000010") then --2
                        next_state <= S1;
                    else 
                        next_state <= S0;
                    end if;
                when S1 =>
                    if(count = "0000000010001") then
                        next_state <= S2;
                    else 
                        next_state <= S1;
                    end if;
                when S2 =>
                    if(rst = '1') then
                        next_state <= idle;
                    else 
                        next_state <= S2;
                    end if;
                when others =>
                    next_state <= idle;
            end case;
    end process;
            
    -- OUTPUT LOGIC
    fsm_outputs : process(clk, present_state)
        begin
            if rising_edge(clk) then
                if rst = '1' then
                    en_cnt_dataset <= '0';
                    en_distance <= '0';
                    en_test <= '0';

                    rst_cnt_dataset <= '1';
                    rst_distance <= '1';
                    rst_test <= '1';
                elsif en = '1' then
                    case present_state is
                    
                        when idle =>
                            en_cnt_dataset <= '0';
                            en_distance <= '0';
                            en_test <= '1';
                            
                            rst_cnt_dataset <= '1';
                            rst_distance <= '1';
                            rst_test <= '0';
    
                        when S0 => 
    
                            en_cnt_dataset <= '1';
                            en_distance <= '0';
                            en_test <= '0';
                            
                            rst_cnt_dataset <= '0';
                            rst_distance <= '1';
                            rst_test <= '0';
                            
                            valid <= '0';
    
                        when S1 => 
                            en_cnt_dataset <= '1';
                            en_distance <= '1';
                            en_test <= '0';
                            
                            rst_cnt_dataset <= '0';
                            rst_distance <= '0';
                            rst_test <= '0';
                            
                            valid <= '0';
                            
                        when S2 => 
                            en_cnt_dataset <= '1';
                            en_distance <= '1';
                            en_test <= '0';
                            
                            rst_cnt_dataset <= '0';
                            rst_distance <= '0';
                            rst_test <= '0';
                            
                            valid <= '1';
                            
                        when others =>
                            en_cnt_dataset <= '0';
                            en_distance <= '0';
                            en_test <= '0';
        
                            rst_cnt_dataset <= '1';
                            rst_distance <= '1';
                            rst_test <= '1';
                            
                            valid <= '0';
                    end case;
                end if;
            end if;
    end process;

end Behavioral;
