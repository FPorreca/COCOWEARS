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

entity kNN is
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
        VALID : OUT std_logic
    );
end kNN;

architecture Behavioral of kNN is

signal d_dataset : std_logic_vector(total_len-1 downto 0) := (others => '0');
signal c_dataset : std_logic_vector(12 downto 0) := (others => '0');
signal distance : std_logic_vector(47 downto 0);
signal class : std_logic_vector(label_len-1 downto 0);
signal preorder_output : min_array_class;
signal en_test, en_out, en_cnt_dataset, en_distance, en_preorder, en_voter : std_logic := '0';
signal rst_out, rst_test, rst_cnt_dataset, rst_distance, rst_preorder, rst_voter : std_logic := '1';
signal test, test_reg : std_logic_vector(total_len-1 downto 0) := (others => '0');
signal predicted_class_reg : STD_LOGIC_VECTOR(label_len DOWNTO 0) := (others => '0');

component FSM is
    Port  (
        clk : IN STD_LOGIC;
        rst, en : in std_logic;
        en_test, en_cnt_dataset, en_distance : out STD_LOGIC;
        rst_test, rst_cnt_dataset, rst_distance : out STD_LOGIC;
        valid : out std_logic
    );
end component;

component dataset_counter is
    Port ( 
        CLK : IN STD_LOGIC;
        CE : IN STD_LOGIC;
        SCLR : IN STD_LOGIC;
        Q : OUT STD_LOGIC_VECTOR(12 DOWNTO 0)
    );
end component;

component dataset_mem is
    Port ( 
        clka : IN STD_LOGIC;
        ena : IN STD_LOGIC;
        addra : IN STD_LOGIC_VECTOR(12 DOWNTO 0);
        douta : OUT STD_LOGIC_VECTOR(172 DOWNTO 0)
    );
end component;

component ManhattanDistance is
    Port (
        dataset : in std_logic_vector(total_len-1 downto 0);
        ACC_X : in std_logic_vector(word_len-1 downto 0);
        ACC_Y : in std_logic_vector(word_len-1 downto 0);
        ACC_Z : in std_logic_vector(word_len-1 downto 0);
        GYR_X : in std_logic_vector(word_len-1 downto 0);
        GYR_Y : in std_logic_vector(word_len-1 downto 0);
        GYR_Z : in std_logic_vector(word_len-1 downto 0);
        MAG_X : in std_logic_vector(word_len-1 downto 0);
        MAG_Y : in std_logic_vector(word_len-1 downto 0);
        MAG_Z : in std_logic_vector(word_len-1 downto 0);
        MDistance : out std_logic_vector(47 downto 0);
        class : out std_logic_vector(label_len-1 downto 0);
        clk, en, rst : in std_logic
    );
end component;

begin
    
    finite_state_machine : FSM
        port map (
            clk => clk,
            en => control_signal(1),
            rst => control_signal(0),
            
            en_cnt_dataset => en_cnt_dataset,
            en_distance => en_distance,
            en_test => en_test,
            
            rst_cnt_dataset => rst_cnt_dataset,
            rst_distance => rst_distance,
            rst_test => rst_test,
            
            valid => valid
        );
    
    cnt_dataset : dataset_counter
        port map (
            CLK => clk,
            CE => en_cnt_dataset,
            SCLR => rst_cnt_dataset,
            Q => c_dataset
        );
        
    dataset : dataset_mem
        port map (
            clka => clk,
            ena => '1',
            addra => c_dataset,
            douta => d_dataset
        ); 
        
    distanceCalc : ManhattanDistance
        port map (
            dataset => d_dataset, 
            class => class,
            ACC_X => ACC_X,
            ACC_Y => ACC_Y,
            ACC_Z => ACC_Z,
            GYR_X => GYR_X,
            GYR_Y => GYR_Y,
            GYR_Z => GYR_Z,
            MAG_X => MAG_X,
            MAG_Y => MAG_Y,
            MAG_Z => MAG_Z,
            MDistance => distance,
            clk => clk,
            en => en_distance,
            rst => rst_distance
        ); 
      
     MDistance <= distance(25 downto 7);
        
end Behavioral;
