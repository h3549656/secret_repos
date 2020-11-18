#ifndef CONSOLE
#define CONSOLE
#include "Basics.hpp"

class Console{
private:
  bool _quit_flag;
  void Color_control(){
    //this function generate the color pair code for every possible combinations of foreground and background color
    //the function init_pair is a feature of ncurses
    string colorNames[] = {
      "black", "blue", "green", "cyan", "red", "magenta", "yellow", "white",
      "bright black", "bright blue", "bright green", "bright cyan", "bright red",
      "bright magenta", "bright yellow", "bright white"
    };
    //ofstream color_log("color_log.txt");
    int color_code = 100;
    for (int foreground = 0; foreground < 16; foreground++){
      for (int background = 0; background < 16; background++){
        init_pair(color_code,foreground,background);
        //color_log << "color code "<< color_code << " = " << colorNames[foreground] << "+" << colorNames[background] << '\n';
        color_code++;
      }
    }
  }
public:
  string console_output = "";

  Console(){//constructor
    startScreen();
    resize_term(console_height,console_width);
  }

  void startScreen(){
    initscr();//ncurses feature
    raw();//ncurses feature
    noecho();//ncurses feature
    keypad( stdscr, TRUE );//ncurses feature
    _quit_flag = false;//control the exit of game
    start_color();//ncurses feature
    Color_control();//private member
  }

  void endScreen(){
    endwin();//ncurses feature
  }

  void initColor(int color_pair){
    attron(COLOR_PAIR(color_pair));//ncurses feature
  }

  void endColor(int color_pair){
    attroff(COLOR_PAIR(color_pair));//ncurses feature
  }

  void draw(int x, int y, const string& symbol){
    //mvprintw is the basics of graphics Color_control
    //be careful the first arguement is y (column) and the second arguement is x (row)
    //2*x is to make the map wider-looking and not too crowded
    //the last argument is a C string, i.e. "A", "/", "E" etc.
    mvprintw(y, 2*x, symbol.c_str());
  }
  void draw(int x, int y, const string& symbol, const int& pair){
    //polymorphism of the above draw()
    //takes a fourth arguement which is the color_code
    //ncurses color display requires a startcolor and an endcolor function
    initColor(pair);
    mvprintw(y,2*x-1, " ");
    mvprintw(y, 2*x, symbol.c_str());
    endColor(pair);
  }
  void draw_interactables(int x, int y, const string& symbol, const int& pair){
    //mainly used to draw player, enemies and merchant
    initColor(pair);
    mvprintw(y, 2*x, symbol.c_str());
    endColor(pair);
  }
  void draw_prompt(int x, int y, const string& symbol){
    //draw reminder on the screen
    move(y,x);//move to coordinate (x,y)
    clrtoeol();//erase from (x,y) to the end of that line
    mvprintw(y, x, symbol.c_str());//print string
  }
  void draw_bound(int level){
    //draw the boundary of the map
    for (int line = header; line <= level_height[level]-1 ; line++){
      mvprintw(line,0,"o");
      mvprintw(line,level_width[level]*2,"o");
    }
    for (int column = 1; column < level_width[level]+1; column+=2){
      mvprintw(header, column*2,"o");
      mvprintw(level_height[level]-1, column*2,"o");
    }
  }
  void quit(){
    //calling this function sets the quit flag = true
    _quit_flag = true;
  }
  bool quit_status(){
    //let me extract the quit flag in main.cpp
    return _quit_flag;
  }
  void showMenu(){
    //menu of the starting screen and when you press m in-game
    erase();
    string list_of_lines[] = {"Welcome to Roguelike Crawler.", "Press Space to start new game",
    "Press 1 to save playing game if available.", "Press 2 to load from save (only at the start of the game).","Press Esc to quit the game."};
    int length = sizeof(list_of_lines)/sizeof(list_of_lines[0]);//gives the number of item in the list

    for(int i = 0; i < length; i++){
      mvprintw(2*i+10,console_width/3, list_of_lines[i].c_str());
    }
    refresh();
  }
  void console_prompt(){
    //this function allows other cpp files to change prompt by changing the public variable 'console_output'
    mvprintw(0,0, console_output.c_str());
  }
  void defeated(){
    //when player hp <=0, execute this function
    erase();
    mvprintw(10,12,"You have been defeated.....-.-.....GAMEOVER.....");
    getch();
    endScreen();
  }
  void goodbye(){
    //when user exited the game, this function will be shown
    erase();
    mvprintw(10,12,"See you next time......(, . ,)");
    getch();
    endScreen();
  }
  void wingame(){
    //when player wins level 5
    erase();
    mvprintw(10,12,"Congrats! You have finished the final level!!!");
    mvprintw(12,12,"You have Completed the game in %i steps! Challenge yourself to finish with the least steps!!", steps);
    mvprintw(14,12,"Credits to Joseph Ho Meng Chit and Leo Lee Man Yin from COMP2113 (_2020_S1)");
    getch();
    endScreen();
  }
  void levelup(){
    //when the player successfully interact with "G" on a map
    erase();
    mvprintw(10,12,"Level %i Completed. Press any key to proceed.", MapLevel);
    getch();
    MapLevel++;
    erase();
  }
  void noticeBar(){
    //keep track of the steps the player takes
    //other self-explanatory prompt
    mvprintw(level_height[MapLevel],0,"Game Presented By Joseph Ho Meng Chit and Leo Lee Man Yin.");
    mvprintw(level_height[MapLevel]+1,0,"Press p to show help.");
    mvprintw(level_height[MapLevel]+2,0,"Your steps: %i", steps);
  }
  void showkeybind(){
    //when user press p, call this function
    //acts like a in-game manual
    //structure similar to the showMenu function
    string list_of_keybind[] = {
    "w: move up", "a: move left", "s: move down", "d: move right",
    "b: view inventory", "m: menu", "c: view crafted items/equip weapons", "q: craft items",
    "i: interact with up", "k: interact with down", "l: interact with right", "j: interact with left",
    "e: interact with merchant"
    };
    string list_of_MapItems[] = {
      "M = merchant", "E = enemy", "# = wall", "C = chest with random resources", "R = rock", "W = wood",
      "P = plastic", "G = goal; the main objective", "/ = door", "{ or } = gate",
      "m = money", "H = health recover", "K = key", "'=' = window", " ~ = rope"
    };
    int length_keybind = sizeof(list_of_keybind)/sizeof(list_of_keybind[0]);
    int length_MapItems = sizeof(list_of_MapItems)/sizeof(list_of_MapItems[0]);
    mvprintw(0,0,"KEYBIND & MAP ITEMS");
    mvprintw(1,0,"Press p to return game.");
    for (int i = 0; i < length_keybind;i++){
      mvprintw(2*i+4,5, list_of_keybind[i].c_str());
    }
    for (int i = 0; i < length_MapItems; i++){
      mvprintw(2*i+4, 50, list_of_MapItems[i].c_str() );
    }

  }
};



#endif
