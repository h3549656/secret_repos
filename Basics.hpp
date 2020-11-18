#ifndef BASICS
#define BASICS
#include <iostream>
#include <ctime>
#include <array>
#include <sstream>
#include <string.h>
#include <fstream>
#include <curses.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

#define KEY_ESC 27
#define KEY_ONE 49
#define KEY_TWO (KEY_ONE+1)
#define KEY_THREE (KEY_TWO+1)
#define KEY_FOUR (KEY_THREE+1)
#define KEY_FIVE (KEY_FOUR+1)
#define KEY_SPACE 32
//max terminal handle
const int MAXX = 200;
const int MAXY = 100;
//set terminal dimension
const int console_width = 180;
const int console_height= 40;
const int header = 3;
//size of each sub room
const int level_width[5] = {35,55,55,70,80};
const int level_height[5] = {33,33,33,33,34};
//Global map level control
bool lv_completed[5] = {false,false,false,false,false};
int num_level = sizeof(lv_completed)/sizeof(lv_completed[0]);
int MapLevel = 0;
int steps = 0;
long oldseed;//For random number generation

enum STRUCTURE { ROAD=0, DOOR=148, ENEMY, CHEST=114, WALL = 109 , BOUNDARY, GOAL=186, RECOVER=230, GATE=10,
                 DOOR_KEY=17, GWOOD=267, GROCK=220, GPLASTIC, GDIAMOND=223 , GMONEY=207, GCHEMCIALS, MERCHANT, GWINDOW, GROPE};
enum RESOURCES { KEY=0, WOOD, ROCK, PLASTIC, SILVER, GOLD, CHEMICALS, MONEY };
enum GETCH     { A=97,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};
enum direction { NAN=0, NORTH, WEST, SOUTH, EAST };

string resource_str(RESOURCES resource){
    if      ( resource == KEY  )      { return "key"; }
    else if ( resource == WOOD )      { return "wood"; }
    else if ( resource == GOLD )      { return "gold"; }
    else if ( resource == SILVER )    { return "silver"; }
    else if ( resource == ROCK )      { return "rock"; }
    else if ( resource == PLASTIC )   { return "plastic"; }
    else if ( resource == CHEMICALS ) { return "chemical"; }
    else if ( resource == MONEY )     { return "money"; }
}
string int_to_str( int value ){
    stringstream ss;
    ss << value;
    return ss.str();
}
int RNG(int min , int max){
  time_t seed;
  seed = time(NULL) + oldseed;
  oldseed = seed;
  srand(seed);
  int n = max - min + 1;
  int i = rand() % n;
  if(i < 0){ i = -i;}
  return min + i;
}
string str_difficulty(int key){
  switch (key) {
    case KEY_ONE:
      return "Easy";
      break;
    case KEY_TWO:
      return "Normal";
      break;
    case KEY_THREE:
      return "Hard";
      break;
    case KEY_FOUR:
      return "Crazy";
      break;
    case KEY_FIVE:
      return "Inhumane";
      break;
  }
}
string dir_str(direction dir){
  switch (dir) {
    case NORTH:
      return "north";
      break;
    case SOUTH:
      return "south";
      break;
    case WEST:
      return "west";
      break;
    case EAST:
      return "east";
      break;
  }
}
#endif
