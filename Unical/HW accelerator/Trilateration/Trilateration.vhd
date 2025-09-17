library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Trilateration is
    Port ( 
        dist_1 : in STD_LOGIC_VECTOR (7 downto 0);
        dist_2 : in STD_LOGIC_VECTOR (7 downto 0);
        dist_3 : in STD_LOGIC_VECTOR (7 downto 0);
        A1_x : in STD_LOGIC_VECTOR (7 downto 0);
        A1_y : in STD_LOGIC_VECTOR (7 downto 0);
        A2_x : in STD_LOGIC_VECTOR (7 downto 0);
        A2_y : in STD_LOGIC_VECTOR (7 downto 0);
        A3_x : in STD_LOGIC_VECTOR (7 downto 0);
        A3_y : in STD_LOGIC_VECTOR (7 downto 0);
        clk : in STD_LOGIC;
        gen_rst : in STD_LOGIC;
        X_coord : out STD_LOGIC_VECTOR (12 downto 0);
        Y_coord : out STD_LOGIC_VECTOR (12 downto 0)
    );
end Trilateration;

architecture Behavioral of Trilateration is

    component A is
        Port ( 
            A2_x : in STD_LOGIC_VECTOR (7 downto 0);
            A1_x : in STD_LOGIC_VECTOR (7 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            A : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;
    
    component B is
        Port ( 
            A2_y : in STD_LOGIC_VECTOR (7 downto 0);
            A1_y : in STD_LOGIC_VECTOR (7 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            B : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;

    component C is
        Port ( 
            dist_1 : in STD_LOGIC_VECTOR (7 downto 0);
            dist_2 : in STD_LOGIC_VECTOR (7 downto 0);
            A1_x : in STD_LOGIC_VECTOR (7 downto 0);
            A2_x : in STD_LOGIC_VECTOR (7 downto 0);
            A1_y : in STD_LOGIC_VECTOR (7 downto 0);
            A2_y : in STD_LOGIC_VECTOR (7 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            C : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;
    
    component D is
        Port (   
            A3_x : in STD_LOGIC_VECTOR (7 downto 0);
            A2_x : in STD_LOGIC_VECTOR (7 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            D : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;
    
    component E is
        Port ( 
            A2_y : in STD_LOGIC_VECTOR (7 downto 0);
            A3_y : in STD_LOGIC_VECTOR (7 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            E : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;
    
    component F is
        Port ( 
            dist_2 : in STD_LOGIC_VECTOR (7 downto 0);
            dist_3 : in STD_LOGIC_VECTOR (7 downto 0);
            A2_x : in STD_LOGIC_VECTOR (7 downto 0);
            A3_x : in STD_LOGIC_VECTOR (7 downto 0);
            A2_y : in STD_LOGIC_VECTOR (7 downto 0);
            A3_y : in STD_LOGIC_VECTOR (7 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            F : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;
    
    component X_calc is
        Port ( 
            B : in STD_LOGIC_VECTOR (13 downto 0);
            C : in STD_LOGIC_VECTOR (13 downto 0);
            E : in STD_LOGIC_VECTOR (13 downto 0);
            F : in STD_LOGIC_VECTOR (13 downto 0);
            det : in STD_LOGIC_VECTOR (13 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            X_coord : out std_logic_vector(12 downto 0)
        );
    end component;
    
    component Y_calc is
        Port ( 
            A : in STD_LOGIC_VECTOR (13 downto 0);
            C : in STD_LOGIC_VECTOR (13 downto 0);
            D : in STD_LOGIC_VECTOR (13 downto 0);
            F : in STD_LOGIC_VECTOR (13 downto 0);
            det : in STD_LOGIC_VECTOR (13 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            Y_coord : out std_logic_vector(12 downto 0)
        );
    end component;
    
    component determinant is
        Port ( 
            A : in STD_LOGIC_VECTOR (13 downto 0);
            B : in STD_LOGIC_VECTOR (13 downto 0);
            D : in STD_LOGIC_VECTOR (13 downto 0);
            E : in STD_LOGIC_VECTOR (13 downto 0);
            clk : in STD_LOGIC;
            rst : in STD_LOGIC;
            det : out STD_LOGIC_VECTOR (13 downto 0)
        );
    end component;
   
    signal A_out: std_logic_vector(13 downto 0);
    signal B_out: std_logic_vector(13 downto 0);
    signal C_out: std_logic_vector(13 downto 0);
    signal D_out: std_logic_vector(13 downto 0);
    signal E_out: std_logic_vector(13 downto 0);
    signal F_out: std_logic_vector(13 downto 0);
    signal det_out : std_logic_vector(13 downto 0);
	
	constant pipeline_stage_A : integer := 7;
	
	type A1_x_pip_array is array(0 to pipeline_stage_A) of std_logic_vector(7 downto 0); 
	signal A1_x_pip: A1_x_pip_array;
	type A2_x_pip_array is array(0 to pipeline_stage_A) of std_logic_vector(7 downto 0); 
	signal A2_x_pip: A2_x_pip_array; 
	type A3_x_pip_array is array(0 to pipeline_stage_A) of std_logic_vector(7 downto 0); 
	signal A3_x_pip: A3_x_pip_array;
	type A1_y_pip_array is array(0 to pipeline_stage_A) of std_logic_vector(7 downto 0); 
	signal A1_y_pip: A1_y_pip_array;
	type A2_y_pip_array is array(0 to pipeline_stage_A) of std_logic_vector(7 downto 0); 
	signal A2_y_pip: A2_y_pip_array; 
	type A3_y_pip_array is array(0 to pipeline_stage_A) of std_logic_vector(7 downto 0); 
	signal A3_y_pip: A3_y_pip_array;
       
begin

	pip: process(clk)
            begin               
                if (rising_edge(clk)) then
                    A1_x_pip(0) <= A1_x;
					A2_x_pip(0) <= A2_x;
					A3_x_pip(0) <= A3_x;
					A1_y_pip(0) <= A1_y;
					A2_y_pip(0) <= A2_y;
					A3_y_pip(0) <= A3_y;
                end if;
                
                loop_pipeline: for i in 0 to pipeline_stage_A-1 loop
                    if (rising_edge(clk)) then
						A1_x_pip(i+1) <= A1_x_pip(i);
                        A2_x_pip(i+1) <= A2_x_pip(i);
                        A3_x_pip(i+1) <= A3_x_pip(i);
						A1_y_pip(i+1) <= A1_y_pip(i);
                        A2_y_pip(i+1) <= A2_y_pip(i);
                        A3_y_pip(i+1) <= A3_y_pip(i);
                    end if;
                end loop loop_pipeline;           
    end process;
	
    coeff_A : A port map (clk => clk, rst => gen_rst, A2_x => A2_x_pip(pipeline_stage_A), A1_x => A1_x_pip(pipeline_stage_A), A => A_out);
    coeff_B : B port map (clk => clk, rst => gen_rst, A2_y => A2_y_pip(pipeline_stage_A), A1_y => A1_y_pip(pipeline_stage_A), B => B_out);
    coeff_C : C port map (clk => clk, rst => gen_rst, A2_x => A2_x_pip(2), A1_y => A1_y_pip(3), A1_x => A1_x_pip(1),  A2_y => A2_y_pip(4), dist_2 => dist_2, dist_1 => dist_1, C => C_out);
    coeff_D : D port map (clk => clk, rst => gen_rst, A3_x => A3_x_pip(pipeline_stage_A), A2_x => A2_x_pip(pipeline_stage_A), D => D_out);
    coeff_E : E port map (clk => clk, rst => gen_rst, A3_y => A3_y_pip(pipeline_stage_A), A2_y => A2_y_pip(pipeline_stage_A), E => E_out);
    coeff_F : F port map (clk => clk, rst => gen_rst, A2_x => A2_x_pip(1), A3_y => A3_y_pip(4), A2_y => A2_y_pip(3), A3_x => A3_x_pip(2), dist_2 => dist_2, dist_3 => dist_3, F => F_out);
    det : determinant port map(clk => clk, rst => gen_rst, A => A_out, B => B_out, D => D_out, E => E_out, det => det_out);
    X : X_calc port map (clk => clk, rst => gen_rst, B => B_out, C => C_out, E => E_out, F => F_out, det => det_out, X_coord => X_coord);
    Y : Y_calc port map (clk => clk, rst => gen_rst, A => A_out, C => C_out, D => D_out, F => F_out, det => det_out, Y_coord => Y_coord);
        
end Behavioral;
