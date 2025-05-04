#include "FPToolkit.c"

typedef struct {
    int depth;
    int max_depth;
    double x;
    double y;
    double theta;
} Recursion_State;

int rand_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}
double rand_doub() {
    return (double)rand()/RAND_MAX;
}

//takes a coordinate, and returns a new coordinate translated by dist at angle theta, where upward is 0 degrees
void polar_movement(double coord[2], double dist, double theta) {
    coord[0] = coord[0] + dist * cos(theta * (M_PI/180) + M_PI/2);
    coord[1] = coord[1] + dist * sin(theta * (M_PI/180) + M_PI/2);
}

//takes startpoint as a parameter, and returns in place the output 
void branch(double coord[2], double width, double length, double this_theta) {
    double current[2] = {coord[0], coord[1]};
    
    //return in place
    polar_movement(coord, length-1, this_theta);

    double branch_x[4];
    double branch_y[4]; 

    polar_movement(current, width/2, this_theta + 90);
    branch_x[0] = current[0]; branch_y[0] = current[1];
    polar_movement(current, length, this_theta);
    branch_x[1] = current[0]; branch_y[1] = current[1];
    polar_movement(current, width, this_theta - 90);
    branch_x[2] = current[0]; branch_y[2] = current[1];
    polar_movement(current, length, this_theta - 180);
    branch_x[3] = current[0]; branch_y[3] = current[1];

    double mod = 0.1*rand_doub();
    G_rgb(0.514 + mod, 0.263 + mod, 0.200 + mod);
    G_fill_polygon(branch_x, branch_y, 4);
}

int leaf(double x, double y, double size, double this_theta) {
    double left[2] = {x, y};
    double right[2] = {x, y};
    double center[2] = {x, y};

    int shape_mod = rand_int(-10, 10);
    polar_movement(left, size + shape_mod, this_theta + 20);
    polar_movement(right, size - shape_mod, this_theta - 20);
    polar_movement(center, 2*size + shape_mod, this_theta);

    double leaf_x[4] = {x, left[0], center[0], right[0]};
    double leaf_y[4] = {y, left[1], center[1], right[1]}; 

    double color_mod = 0.1*rand_doub();
    G_rgb(0, 0.4 + color_mod, 0);
    G_fill_polygon(leaf_x, leaf_y, 4);
}

void tree(Recursion_State r_state) {
    if (r_state.depth == r_state.max_depth) { //|| rand_doub() < r_state.depth/20.
        for (int i=0; i < rand_int(4, 7); i++) {
            leaf(r_state.x, r_state.y, 30, r_state.theta + rand_int(-120, 120));  
        }
        return;
    }
    r_state.depth++;
    
    for (int i=0; i < fmin(2, r_state.depth * rand_doub() * 1.2); i++) {
        double coord[2] = {r_state.x, r_state.y};
        
        int spread = fmin(70, r_state.depth * 30);
        if (r_state.depth == 1) spread = rand_int(0, 20);
        double new_theta = rand_int(-spread, spread);
        branch(coord, 120/r_state.depth, 330/r_state.depth, r_state.theta + new_theta); //this modifies in place coord

        //show each step of the generation10
        G_display_image();
        usleep(5000);
    
        Recursion_State new_r_state = r_state;
        new_r_state.x = coord[0];
        new_r_state.y = coord[1];
        new_r_state.theta += new_theta;

        tree(new_r_state);
    }
}

void main() {
    //makes rand less deterministic
    srand(time(NULL));

    //graphics initialization
    int screen_x = 1500;
    int screen_y = 1500;
    G_init_graphics(screen_x, screen_y);
    
    while(1) {
        //sky
        G_rgb(0.529, 0.808, 0.97);
        G_clear();
        
        //grass
        G_rgb(0, 1, 0);
        G_fill_rectangle(0, 0, screen_x, screen_y/2);

        //cloud
        G_rgb(1, 1, 0.95);
        G_fill_circle(1200, 1200, 100);
        G_fill_circle(1100, 1180, 100);
        G_fill_circle(1000, 1180, 100);
        G_fill_circle(1300, 1300, 100);
        G_fill_circle(1000, 1350, 100);
        G_fill_circle(900, 1250, 100);
        G_fill_circle(1200, 1400, 100);
        G_fill_circle(1100, 1400, 100);
        G_fill_circle(1100, 1300, 100);

        //house
        G_rgb(1, 1, 0.85);
        G_fill_rectangle(50, 250, 500, 600);
        G_rgb(0.5, 0.03, 0.03);
        G_fill_triangle(50, 850, 300, 1000, 550, 850);
        //draws recursive tree
        Recursion_State r_state = {
            .max_depth = 9,
            .x = screen_x/2. + screen_x/18.,
            .y = 50
        }; 
        tree(r_state);

        if (G_wait_key() == 'q') {
            break;
        }
    }
}