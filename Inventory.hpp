#ifndef INVENTORY
#define INVENTORY
#include "Basics.hpp"
#include "Console.hpp"

class crafted_items{
public:
  string name;
  int durability;
  int dmg;
  crafted_items(string x, int y, int z){//constructor
    name = x.c_str();
    durability = y;
    dmg = z;
  }
  crafted_items(const crafted_items &item){
    //this copy constructor of the class
    //it is used when I want to copy an existing object (crafted_items) to another object
      name = item.name;
      durability = item.durability;
      dmg = item.dmg;
  }
};

class Inventory{
public:
  static const int size = 8;//size of the content; there are 8 things the player can collect on the map
  int content[size] = {3};//each of the item in the content array is an integer, which track the amount of that resource

  std::vector<crafted_items> craft_storage;
  std::vector<crafted_items> equipped_weapon;

  bool WeaponStatus(){
    //check whether weapon reaches 0% durability
    //return a boolean; quite self-explanatory
    if(equipped_weapon[0].durability <= 0){
      return false;
    }
    return true;
  }
  int weaponDmg(string weapon){
    //return an integer corresponding to the name of the weapons
    //that integer is the damage the weapon can deal to enemy per hit
    if ( weapon == "gun" )          {return 90;}
    else if ( weapon == "knife" )   {return 12;}
    else if ( weapon == "axe" )     {return 25;}
    else if ( weapon == "slinger" ) {return 40;}
  }

  crafted_items getweapon(){
    //check whether the weapon slot is occupied
    //if yes, return that weapon
    //if no, return a dummy null item
    crafted_items null("null",0,0);
    if(equipped_weapon.size()>0){
      return equipped_weapon[0];
    }
    else{
      return null;
    }
  }
  int getsize(){
    return size;//return size of the content array
  }
  int getmoney(){
    //return the current amount of money held
    //used to check if player can purchase anything from the merchant
    return content[MONEY];
  }
  void showInv(){
    //when user press b, call this function
    //shows each of the item in the content array, with their amount respectively
    refresh();
    for (int i = 0; i < size; i++){
      move(10+2*i,10);
      clrtoeol();
      mvprintw(10+2*i,10,"%s = %d", resource_str(RESOURCES(i)).c_str() ,content[i]);
    }
  }

  void showCrafted(){
    //when user press c, show this function
    //show the craft_storage if the vector is not empty
    //else show empty prompt
    //also control whether any weapon is equipped in slot
    erase();
    refresh();
    if( craft_storage.size() < 1 ){
      mvprintw( 10, 25, "Crafted toolbox is empty. Press c to return to game" );
    }
    else{
      mvprintw( 10, 25, "Press e to choose weapon to equip. Press d to choose weapon to delete. Press c to return to game." );
      for (int i = 0; i < craft_storage.size(); i++){
        mvprintw(12+2*i, 25, "You have a %s (%i%%)", craft_storage[i].name.c_str(), craft_storage[i].durability);
      }
    }
    if(getweapon().name == "null"){
      mvprintw(0,0, "You have not equipped any weapons.");
    }
    else{
      mvprintw(0,0, "You are equipping %s (%i%%) as your weapon.", getweapon().name.c_str(), getweapon().durability);
    }
  }
  void addInv(const int &item){
    //increase the counter of the item tracked by index; which works well with the enum structure
    //pass in a enum RESOURCES item and the corresponding amount will be incremented by one
    content[item]++;
  }
  void addInv(const int &item, int how_many){
    //polymorphic of the above function
    //also can specify add how many
    content[item]+=how_many;
  }
  bool checkKey(){
    //return true if the amount of key is >=1
    //return false if no key
    if( content[KEY] > 0)  {return true;}
    else                   {return false;}
  }
  void removeKey(){
    //remove a key (usually after opened a door)
    content[KEY] -= 1;
  }

  void craft(const string &name){
    //craft a weapon and store it in the craft_storage vector
    //only need to specify the name of the weapon; dmg initialization is taken care of by the weaponDmg function above
    crafted_items Item(name.c_str(), 100, weaponDmg(name.c_str()));//a direct application of the constructor
    craft_storage.push_back(Item);
  }

  int checkCrafted(string item){
    //takes in the name of the item. i.e. "axe" or "gun"
    //return the index of that crafted_items in the vector
    //if no match in the entire vector, return -1
    for (int i = 0; i < craft_storage.size();i++){
      if(craft_storage[i].name == item){
        return i; //return the index of the first match
      }
    }
    return -1;
  }

  void equipWeapon(int index){
    //takes in the index of the item in the craft_storage
    crafted_items to_be_equipped = craft_storage[index];//direct use of copy constructor
    if(equipped_weapon.size() > 0){

      crafted_items previous = equipped_weapon[0];
      craft_storage.erase(craft_storage.begin() + index);
      equipped_weapon.pop_back();

      craft_storage.push_back(previous);
      equipped_weapon.push_back(to_be_equipped);
    }
    else{//if no weapon is equipped
      equipped_weapon.push_back(to_be_equipped);
      craft_storage.erase(craft_storage.begin() + index);
    }
  }

  void equip(Console &console){
    if( craft_storage.size() > 0 ){
      console.draw_prompt(0,2,"which weapons do you want to equip? Press the corresponding digit.");
      for(int i = 0;i < craft_storage.size();i++){
        mvprintw(4+i, 0, "%i) %s (%i%%)",i+1,craft_storage[i].name.c_str(),craft_storage[i].durability);
      }
      int equip = getch();
      if(KEY_ONE<=equip && equip <= KEY_ONE+craft_storage.size()){
        equipWeapon(equip - KEY_ONE);
      }
    }
    else{
      console.draw_prompt(0,2,"You have no weapons available.");
    }
  }

  void discard(Console &console){
    if(craft_storage.size() > 0){
      console.draw_prompt(0,2,"which weapons do you want to discard? Press the corresponding digit.");
      for(int i = 0;i < craft_storage.size();i++){
        mvprintw(4+i, 0, "%i) %s (%i%%)", i+1, craft_storage[i].name.c_str(), craft_storage[i].durability);
      }
      int equip = getch();
      if(KEY_ONE <= equip && equip <= KEY_ONE+craft_storage.size()){
        //delete that weapon
        craft_storage.erase(craft_storage.begin() + equip - KEY_ONE);
        console.draw_prompt(0,2,"Weapon deleted.");
      }
    }
    else{
      console.draw_prompt(0,2,"You have no weapons available.");
    }
  }
  void manual(){
    showInv();
    mvprintw(2, 0,"Press q to return to game.");
    mvprintw(10, 50,"Press 1 to craft an axe (need 2 WOOD and 2 ROCK).");
    mvprintw(12, 50,"Press 2 to craft a knife (need 2 SILVER).");
    mvprintw(14, 50,"Press 3 to craft a slinger (need 1 ROCK and 1 WOOD)");
    mvprintw(16, 50,"Press 4 to craft a gun (need 3 CHEMCIALS, 2 GOLD, 2 SILVER, 2 PLASTIC)");
  }
  void craftingTable(Console &console){
    //if user press q, show this function
    //has an argument console passed by reference since the screen will be updated whenever sth is crafted and resources are reduced
    manual();
    int key = getch();
    while(key != Q){

      if( craft_storage.size() >= 9 ){
        console.draw_prompt(0,1,"Craft_storage full! You cannot hold more than 9 crafts. Consider discarding unwanted items!");
      }

      if(key == KEY_ONE && craft_storage.size() < 9){
        if( content[WOOD] >= 2 && content[ROCK] >= 2 ){
          craft("axe");
          content[WOOD]-=2;
          content[ROCK]-=2;
          console.draw_prompt(0,0,"You have crafted an axe!");
        }
        else{console.draw_prompt(0,0,"You do not have enough materials!");}
      }
      if(key == KEY_TWO && craft_storage.size() < 9){
        if(content[SILVER]>=2){
          craft("knife");
          content[SILVER]-=2;
          console.draw_prompt(0,0,"You have crafted a knife!");
        }
        else{console.draw_prompt(0,0,"You do not have enough materials!");}
      }
      if(key == KEY_THREE && craft_storage.size() < 9){
        if(content[WOOD]>=1&&content[ROCK]>=1){
          craft("slinger");
          content[WOOD]--;
          content[ROCK]--;
          console.draw_prompt(0,0,"You have crafted a slinger!");
        }
        else{console.draw_prompt(0,0,"You do not have enough materials!");}
      }
      if(key == KEY_FOUR && craft_storage.size() < 9){
        if(content[CHEMICALS]>=3 && content[GOLD]>=2 && content[SILVER]>=2 && content[PLASTIC]>=2){
          craft("gun");
          content[CHEMICALS]-=3;
          content[GOLD]-=2;
          content[SILVER]-=2;
          content[PLASTIC]-=2;
          console.draw_prompt(0,0,"You have crafted a gun!");
        }
        else{console.draw_prompt(0,0,"You do not have enough materials!");}
      }
      manual();
      key = getch();
    }
  }
};
#endif
