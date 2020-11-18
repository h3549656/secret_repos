#ifndef OBJECT
#define OBJECT
#include "Basics.hpp"
#include "Console.hpp"

class Object{
protected:
  int posx,posy;
  int obj_hp;
  string obj_symbol;
  int obj_color;
public:
  Object(){ }
  Object(const string& symbol, int color){
    obj_symbol = symbol;
    obj_color = color;
  }
  void setcolor(const int &color){
    obj_color = color;
  }
  int getcolor_code(){
    return obj_color;
  }
  void sethp(int hp){
    obj_hp = hp;
  }
  void minusHP(int dmg){
    obj_hp -= dmg;
  }
  int gethp(){
    return obj_hp;
  }
  void recoverhp(int amount){
    obj_hp += amount;
  }
  int gety(){
    return posy;
  }
  int getx(){
    return posx;
  }
  string getsym(){
    return obj_symbol;
  }
  void setpos(int x , int y){
    posx = x;
    posy = y;
  }
  void setsym(const string& symbol){
    obj_symbol = symbol;
  }
  void draw(Console console){
    console.draw(posx, posy, obj_symbol, obj_color);
  }
  void draw_interactables(Console console){
    console.draw_interactables(posx, posy, obj_symbol, obj_color);
  }
  void Move(direction direction ){
    switch (direction) {
      case NORTH:
        posy -= 1;
        break;
      case WEST:
        posx -= 1;
        break;
      case SOUTH:
        posy+=1;
        break;
      case EAST:
        posx +=1;
        break;
      default: break;
    }
  }
  void setup(int hp, const string &symbol, int posx, int posy, int color){
    setpos(posx,posy);
    setsym(symbol);
    sethp(hp);
    setcolor(color);
  }
  bool isProximity(Object target){
    int dx = abs(target.getx()-posx);
    int dy = abs(target.gety()-posy);
    if     (dx == 0 && dy == 1)   {return true;}
    else if(dx == 1 && dy == 0)   {return true;}
    else if(dx == 1 && dy == 1)   {return true;}
    return false;
  }
};

class enemy: public Object{
public:
  enemy(){}
  enemy(int hp){
    sethp(hp);
  }
  void follow(Object player){
    int dx = player.getx() - posx;
    int dy = player.gety() - posy;
    if(!isProximity(player)){
      if(dx > 0  ){//D
        Move(EAST);
      }
      else if (dx < 0 ){//A
        Move(WEST);
      }
      if (dy > 0 ){//S
        Move(SOUTH);
      }
      else if (dy < 0){//W
        Move(NORTH);
      }
    }
  }

  bool attack(Object &player, int dmg){
    if(isProximity(player)){
      player.minusHP(dmg);
      return true;
    }
    return false;
  }
};

class Player: public Object{
protected:
public:
  int isOverlap(Object obj_arr[],int arr_size,int newx, int newy){
    int flag = 0;
    for (int i = 0 ; i < arr_size; i++){
      int dx = abs(obj_arr[i].getx()-newx);
      int dy = abs(obj_arr[i].gety()-newy);
      if(dx == 0 && dy == 0){
        flag++;
      }
    }
    return flag;
  }
};

#endif
