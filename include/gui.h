#ifndef GUI_H_
#define GUI_H_

typedef struct color {
    unsigned char r, g, b, a;
} Colour;

// this holds the position and button of the most recent mouse click
extern struct mouse {
    int x, y; // position of most recent click
    int button; // whether the most recent click was leftclick
} mouse;

// holds a number of boolean values that represent input states relevant to the game
extern struct input_states
{
    int closed;        // set to 1 if the window's close button is pressed
    int mouse_handled; // whether the last mouse press was handled
    int step;          // whether the game should be advanced by a single step (set to 1 when SPACE is pressed)
    int play;          // whether the game should be advanced on every iteration (set to 1 so long as the P key is held)
    int save;          // signals the game should be saved to a savefile (set to 1 when S is pressed)
} input_states;


// initialises the SDL window and renderer
void gui_init(const char *title, int w, int h);

// destroys the window and renderer, quits SDL
void gui_quit();

// handles the relevant input events, updates mouse and input_states
void read_input();

// fills the display with a flat color
void gui_clear(Colour c);

// draws a rectangle on the display
void draw_rectangle(Colour c, int x, int y, int width, int height);

// updates the display
void gui_update();

// waits a number of milliseconds
void wait(int ms);

#endif