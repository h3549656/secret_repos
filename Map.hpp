#ifndef MAP
#define MAP
#include "Basics.hpp"
#include "Console.hpp"
#include "Object.hpp"
#include "Inventory.hpp"

class Map {
public:
  Object envir[MAXX][MAXY];
  STRUCTURE checkTile(Object obj){
    if (obj.getsym() == ".")      return ROAD;
    else if (obj.getsym() == "#") return WALL;
    else if (obj.getsym() == "/") return DOOR;
    else if (obj.getsym() == "E") return ENEMY;
    else if (obj.getsym() == "C") return CHEST;
    else if (obj.getsym() == "G") return GOAL;
    else if (obj.getsym() == "H") return RECOVER;
    else if (obj.getsym() == "K") return DOOR_KEY;
    else if (obj.getsym() == "W") return GWOOD;
    else if (obj.getsym() == "R") return GROCK;
    else if (obj.getsym() == "m") return GMONEY;
    else if (obj.getsym() == "P") return GPLASTIC;
    else if (obj.getsym() == "!") return GCHEMCIALS;
    else if (obj.getsym() == "M") return MERCHANT;
    else if (obj.getsym() == "=") return GWINDOW;
    else if (obj.getsym() == "~") return GROPE;
    else if (obj.getsym() == "{" || obj.getsym() == "}") return GATE;
  }
  STRUCTURE checkSurrounding(Object obj, direction dir){
    int x = obj.getx();
    int y = obj.gety();
    switch (dir) {
      case NORTH:
        y--;
        break;
      case WEST:
        x--;
        break;
      case SOUTH:
        y++;
        break;
      case EAST:
        x++;
        break;
    }
    if (x < 0 || y < 0 || x > level_width[MapLevel]-1 || y > level_height[MapLevel]-1) return BOUNDARY;
    else if (envir[x][y].getsym() == ".")return ROAD;
    else if (envir[x][y].getsym() == "#")return WALL;
    else if (envir[x][y].getsym() == "/")return DOOR;
    else if (envir[x][y].getsym() == "E")return ENEMY;
    else if (envir[x][y].getsym() == "C")return CHEST;
    else if (envir[x][y].getsym() == "G")return GOAL;
    else if (envir[x][y].getsym() == "H")return RECOVER;
    else if (envir[x][y].getsym() == "K")return DOOR_KEY;
    else if (envir[x][y].getsym() == "W")return GWOOD;
    else if (envir[x][y].getsym() == "R")return GROCK;
    else if (envir[x][y].getsym() == "m")return GMONEY;
    else if (envir[x][y].getsym() == "P")return GPLASTIC;
    else if (envir[x][y].getsym() == "!")return GCHEMCIALS;
    else if (envir[x][y].getsym() == "M")return MERCHANT;
    else if (envir[x][y].getsym() == "=")return GWINDOW;
    else if (envir[x][y].getsym() == "~")return GROPE;
    else if (envir[x][y].getsym() == "{" || envir[x][y].getsym() == "}") return GATE;
  }
  bool interact(Object &obj, direction dir, Inventory &inv, Console &console){
    if (checkSurrounding(obj,dir) == CHEST){
      refresh();
      int item = RNG(0,6);
      inv.addInv(item);
      console.console_output = "You have picked up a " + resource_str(RESOURCES(item)) + "!";
      return true;
    }
    else if(checkSurrounding(obj,dir) == GOAL){
      console.console_output = "Level Completed!";
      lv_completed[MapLevel] = true;
      return true;
    }
    else if(checkSurrounding(obj,dir) == ENEMY){
      console.console_output = "You attacked an enemy!";
      return true;
    }
    else if(checkSurrounding(obj,dir) == WALL){
      return false;
    }
    else if(checkSurrounding(obj,dir) == MERCHANT){
      return false;
    }
    else if(checkSurrounding(obj,dir) == RECOVER){
      obj.recoverhp(10);
      console.console_output = "You have picked up a health portion! Hp increased!";
      return true;
    }
    else if(checkSurrounding(obj,dir) == GPLASTIC){
      inv.addInv(PLASTIC,3);
      console.console_output = "You have picked up some plastic!";
      return true;
    }
    else if(checkSurrounding(obj,dir) == GCHEMCIALS){
      inv.addInv(CHEMICALS);
      inv.addInv(CHEMICALS);
      console.console_output = "You have picked up some chemcials!";
      return true;
    }
    else if(checkSurrounding(obj,dir) == GATE){
      if(inv.getweapon().name == "axe"){
        inv.equipped_weapon[0].durability-=50;
        console.console_output = "You have cracked open the gate!";
        return true;
      }
      else{
        console.console_output = "You haven't equip an axe to crack the gate!";
        return false;
      }
    }
    else if (checkSurrounding(obj,dir)==GWINDOW){
      if(inv.getweapon().name == "slinger"){
        inv.equipped_weapon[0].durability-=10;
        console.console_output = "You have cracked open the window!";
        return true;
      }
      else{
        console.console_output = "You haven't equip a slinger to crack the window!";
        return false;
      }
    }
    else if(checkSurrounding(obj,dir) == GROPE){
      if(inv.getweapon().name == "knife"){
        inv.equipped_weapon[0].durability-=15;
        console.console_output = "You have cut the rope!";
        return true;
      }
      else{
        console.console_output = "You haven't equip a knife to cut the rope!";
        return false;
      }
    }
    else if (checkSurrounding(obj,dir)==DOOR_KEY){
      inv.addInv(KEY);
      console.console_output = "You have picked up a door key!";
      return true;
    }
    else if (checkSurrounding(obj,dir)==GWOOD){
      inv.addInv(WOOD);
      console.console_output = "You have picked up a wood!";
      return true;
    }
    else if (checkSurrounding(obj,dir)==GROCK){
      inv.addInv(ROCK);
      console.console_output = "You have picked up a rock!";
      return true;
    }
    else if (checkSurrounding(obj,dir)==GMONEY){
      int random_money = RNG(20,50);
      inv.addInv(MONEY,random_money);
      console.console_output = "You have picked up $" + int_to_str(random_money) + "!";
      return true;
    }
    else if (checkSurrounding(obj, dir) == DOOR){
      if(inv.checkKey()){
        console.console_output = "You have opened a door!";
        inv.removeKey();
        return true;
      }
      else{
        console.console_output = "You do not have any keys!";
        return false;
      }
    }
  }
  void wipe(Object obj, direction dir){
    int x,y;
    x = obj.getx();
    y = obj.gety();
    switch (dir) {
      case NORTH:
        envir[x][y-1].setsym(".");
        envir[x][y-1].setcolor(ROAD);
        break;
      case WEST:
        envir[x-1][y].setsym(".");
        envir[x-1][y].setcolor(ROAD);
        break;
      case SOUTH:
        envir[x][y+1].setsym(".");
        envir[x][y+1].setcolor(ROAD);
        break;
      case EAST:
        envir[x+1][y].setsym(".");
        envir[x+1][y].setcolor(ROAD);
        break;
    }
  }
  void display(Console &console, int level){
    for (int i = header + 1; i < level_height[level] + 2; i++){
      for (int j = 1; j < level_width[level];j++){
        envir[j][i].setcolor(checkTile(envir[j][i]));
        envir[j][i].draw_interactables(console);
      }
    }
  }
};
#endif
