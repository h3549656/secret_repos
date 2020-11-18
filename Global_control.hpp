#ifndef GLOBAL_CONTROL
#define GLOBAL_CONTROL
#include "Basics.hpp"
#include "Console.hpp"
#include "Inventory.hpp"
#include "Object.hpp"
#include "Map.hpp"

int load_enemy_num(){
  string str_enemy_num, str_difficulty;
  ifstream EnemySave("EnemySave.txt");
  if(!EnemySave.good()){
    return -1;
  }
  else{
    EnemySave >> str_difficulty >> str_enemy_num;
    int enemy_num = stoi(str_enemy_num);
    EnemySave.close();
    return enemy_num;
  }
}
bool LoadSave(string mfile, string pfile, string efile, Player &player, enemy arr[], Object &merchant, Inventory &inventory,Map &map1,int &level, int &difficulty){
  //takes in 3 txt filenames, and load back relevant saves for player, enemy, merchant, inventory and map1
  ifstream MapSave, PlayerSave, EnemySave;
  string s1, s2, s3, s4, str_resource, str_lv, str_hp, str_num_craft, str_item, str_enemy_hp, str_durability, str_dmg;
  string lines,str_num_equipped,str_steps;
  int num_craft;

  MapSave.open(mfile.c_str());
  PlayerSave.open(pfile.c_str());
  EnemySave.open(efile.c_str());

  if (!MapSave.good() || !PlayerSave.good() || !EnemySave.good()) {
      return false;
  }

  else{
      erase();
      //player attributes
      PlayerSave >> s1 >> s2 >> str_hp >> str_lv >> str_steps;
      player.setup(stoi(str_hp),"A",stoi(s1),stoi(s2),254);
      level = stoi(str_lv);
      steps = stoi(str_steps);
      //resources
      for (int i = 0; i < inventory.getsize(); i++){
        PlayerSave >> str_resource;
        inventory.content[i] = stoi(str_resource);
      }
      //merchant
      PlayerSave >> s3 >> s4;
      merchant.setup(100,"M", stoi(s3),stoi(s4),221);

      //crafted items
      PlayerSave >> str_num_craft;
      num_craft = stoi(str_num_craft);
      inventory.craft_storage.clear();
      for (int i = 0; i < num_craft; i++){
        PlayerSave >> str_item >> str_durability >> str_dmg;
        crafted_items item(str_item.c_str(), stoi(str_durability), stoi(str_dmg));
        inventory.craft_storage.push_back(item);
      }
      PlayerSave >> str_num_equipped;
      inventory.equipped_weapon.clear();
      if(stoi(str_num_equipped) > 0){
        PlayerSave >> str_item >> str_durability >> str_dmg;
        crafted_items item(str_item.c_str(), stoi(str_durability), stoi(str_dmg));
        inventory.equipped_weapon.push_back(item);
      }

      //MapSave
      int i = header+1, j = 1;
      while(getline(MapSave, lines)){
        istringstream temp(lines);
        string symbol;

        while(temp >> symbol){
          map1.envir[j][i].setsym(symbol.c_str());
          map1.envir[j][i].setpos(j,i);
          map1.envir[j][i].setcolor(map1.checkTile(map1.envir[j][i]));
          j++;
        }
        i++;
        j=1;
      }
      //enemy save
      string num_enemy_str,x,y, difficulty_str;
      EnemySave >> difficulty_str >> num_enemy_str;
      difficulty = stoi(difficulty_str);

      for (int i = 0; i < stoi(num_enemy_str);i++){
        EnemySave >> x >> y >> str_enemy_hp;
        arr[i].setup(stoi(str_enemy_hp), "E", stoi(x), stoi(y), 224);
      }

      EnemySave.close();
      MapSave.close();
      PlayerSave.close();


      return true;
    }
}
void SaveGame(string mfile, string pfile, string efile,Object player,Inventory inventory,Map &map1,int level,enemy arr[], int num_enemy, Object merchant, int difficulty){
  //takes in 3 filenames and save all the current gameplay in mostly integers and char form
  ofstream MapSave;
  ofstream PlayerSave;
  ofstream EnemySave;
  MapSave.open(mfile.c_str());
  PlayerSave.open(pfile.c_str());
  EnemySave.open(efile.c_str());

  PlayerSave << player.getx() << " " << player.gety() << " ";
  PlayerSave << player.gethp() << " ";
  PlayerSave << level << " ";
  PlayerSave << steps << " ";
  for (int i = 0; i < inventory.size; i++){
    PlayerSave << inventory.content[i] << " " ;
  }
  PlayerSave << merchant.getx() << " " << merchant.gety()<< " " << inventory.craft_storage.size() << " ";

  for (int i = 0; i < inventory.craft_storage.size(); i++){
    PlayerSave << inventory.craft_storage[i].name.c_str() << " ";
    PlayerSave << inventory.craft_storage[i].durability << " ";
    PlayerSave << inventory.craft_storage[i].dmg << " ";
  }
  if(inventory.equipped_weapon.size() == 0){
    PlayerSave << 0;
  }
  else{
    PlayerSave << 1 << " ";
    PlayerSave << inventory.equipped_weapon[0].name.c_str() << " ";
    PlayerSave << inventory.equipped_weapon[0].durability << " ";
    PlayerSave << inventory.equipped_weapon[0].dmg << " ";
  }

  ///
  for (int i = header + 1; i <= level_height[level]; i++){
    for (int j = 1; j <= level_width[level]; j++){
        MapSave << map1.envir[j][i].getsym() << ' ';
    }
    MapSave << '\n';
  }
  ///

  EnemySave << difficulty << " " << num_enemy <<" ";
  for (int i = 0; i < num_enemy; i++){
    EnemySave << arr[i].getx() << " " << arr[i].gety() << " " << arr[i].gethp() << " ";
  }

  EnemySave.close();
  MapSave.close();
  PlayerSave.close();
}
int attack_enemy(Player player, enemy enemy_array[], int arr_size, direction dir, Inventory &inv){
  //takes in player and enemy_array, then compare if any of the enemies are in the direction of key pressed
  //if yes, then returns the position of that enemy in the dynamic array; else return -1
  //if yes, then check if player has equipped any weapons. If weapon slot is not null, the corresponding enemy will also has its hp minused
  //if the attack reduces the enemy hp below 0, then the enemy is 'defeated'----its position is set to a boundary values so it will disappear on screen
  int px,py;
  px = player.getx();
  py = player.gety();
  for (int i = 0; i < arr_size; i++){
    int x,y;
    x = enemy_array[i].getx();
    y = enemy_array[i].gety();
    switch (dir) {
      case NORTH:
        y++;
        break;
      case WEST:
        x++;
        break;
      case SOUTH:
        y--;
        break;
      case EAST:
        x--;
        break;
    }
    if (px == x && py == y && inv.getweapon().name != "null"){
      enemy_array[i].minusHP(inv.getweapon().dmg);
      inv.equipped_weapon[0].durability -= 1;
      if(enemy_array[i].gethp() <= 0) {
        enemy_array[i].setpos(100,100);
      }
      return i;
    }
  }
  return -1;
}
void attack_control(Player player, enemy enemy_array[], int enemy_num, direction dir, Inventory &inv, Console &console){
  int attack_flag = attack_enemy(player,enemy_array, enemy_num, dir, inv);
  if(attack_flag != -1 ){
    if(enemy_array[attack_flag].gethp() <=0 ){
      int random_money = RNG(30,50);
      inv.addInv(MONEY,random_money);
      console.console_output = "You have defeated an enemy! Rewarded with $"+ int_to_str(random_money)+ "!";
    }
    else{
      console.console_output = "You are attacking an enemy in " + dir_str(dir) + " direction. Enemy hp = " + int_to_str(enemy_array[attack_flag].gethp());
    }
  }
}
void global_chase(Player player, enemy arr[], int arr_size, Map map1){
  //takes in player, enemy_array and size and map1
  //compute whether each of the enemy in the array is near the player.
  //for each of the enemy, if not near player, then move near player by 1 step (in x and/or in y)
  for(int i = 0; i < arr_size; i++){
    int dx = player.getx() - arr[i].getx();
    int dy = player.gety() - arr[i].gety();
    int flag = 0;
    if(!arr[i].isProximity(player)){
      if(dx > 0 && map1.checkSurrounding(arr[i], EAST) == ROAD ){
        for(int j = 0; j < arr_size&&j!=i; j++){
          int dx = abs(arr[j].getx()-arr[i].getx()-1);
          int dy = abs(arr[j].gety()-arr[i].gety());
          if(dx == 0 && dy == 0){
            flag++;
          }
        }
        if(flag==0){
          arr[i].Move(EAST);
        }
      }
      else if (dx < 0 && map1.checkSurrounding(arr[i], WEST) == ROAD){//A
        for(int j = 0; j < arr_size&&j!=i; j++){
          int dx = abs(arr[j].getx()-arr[i].getx()+1);
          int dy = abs(arr[j].gety()-arr[i].gety());
          if(dx == 0 && dy == 0){
            flag++;
          }
        }
        if(flag==0){
          arr[i].Move(WEST);
        }
      }
      if (dy > 0 && map1.checkSurrounding(arr[i], SOUTH) == ROAD){//S
        for(int j = 0; j < arr_size&&j!=i; j++){
          int dx = abs(arr[j].getx()-arr[i].getx());
          int dy = abs(arr[j].gety()-arr[i].gety()-1);
          if(dx == 0 && dy == 0){
            flag++;
          }
        }
        if(flag==0){
          arr[i].Move(SOUTH);
        }
      }
      else if (dy < 0&& map1.checkSurrounding(arr[i], NORTH) == ROAD){//W
        for(int j = 0; j < arr_size&&j!=i; j++){
          int dx = abs(arr[j].getx()-arr[i].getx());
          int dy = abs(arr[j].gety()-arr[i].gety()+1);
          if(dx == 0 && dy == 0){
            flag++;
          }
        }
        if(flag==0){
          arr[i].Move(NORTH);
        }
      }
    }
  }
}
void merchant_shop(Inventory &inv, Console &console){
  //takes in the inventory of the Player
  //provide 3 random choices of items for player to exchange with money
  inv.showInv();
  const int item_num = 3;
  int cost[item_num] =   { RNG(120,150), RNG(150,200), RNG(300,400) };
  int amount[item_num] = { RNG(2,3), RNG(4,5), RNG(6,8) };
  int item[item_num] =   { RNG(0,3), RNG(4,6), RNG(0,6) };
  for (int i = 0; i < item_num; i++){
      mvprintw(4+2*i, 10, "%i) $ %i : %i %ss " ,i+1, cost[i], amount[i], resource_str(RESOURCES(item[i])).c_str() );
  }

  int key = getch();

  while(key != E && key!= KEY_ESC ){
    if( key == R && inv.getmoney() >= 50 ){
      //shop_refresh();
      inv.content[MONEY]-=50;
    }
    if(KEY_ONE <= key && key <= KEY_THREE){
      if(inv.getmoney() >= cost[key-KEY_ONE]){
        inv.content[MONEY]-=cost[key-KEY_ONE];
        inv.addInv(item[key-KEY_ONE],amount[key-KEY_ONE]);
        console.draw_prompt(0,0,"You have successfully purchased " + int_to_str(amount[key-KEY_ONE]) + " " + resource_str(RESOURCES(item[key-KEY_ONE])).c_str());
      }
      else{
        console.draw_prompt(0,0,"Not enough money!");
      }
    }
    inv.showInv();
    key = getch();
  }
}
void random_spawn(const string& symbol, Map &map1, int item_num){
  //takes in symbol representing in STRUCTURE, map, and integer item_num
  //randomly spawn that STRUCTURE on the map until there are (item_num) of them
  while(item_num > 0){
    int x = RNG(5,level_width[MapLevel]-5);
    int y = RNG(5,level_height[MapLevel]-5);
    if(map1.envir[x][y].getsym() == "."){
      map1.envir[x][y].setsym(symbol.c_str());
      map1.envir[x][y].setpos(x,y);
      item_num--;
    }
  }
}
void npc_setup(enemy enemy_array[], int enemy_num, Object &merchant, int merchantx, int merchanty, Map &map1){
  //takes in enemy_array, size, merchant and its positions and map
  //setup the positions, hp, symbol, color of each one of the objects
  //override by the positions of merchant on the map so enemies and player will recognise to not overlap.
  for (int i = 0; i < enemy_num; i++){
    enemy_array[i].setup(100,"E",RNG(5,level_width[MapLevel]-5),RNG(5,level_height[MapLevel]-5),224);
  }
  merchant.setup(100,"M",merchantx,merchanty,221);
  map1.envir[merchantx][merchanty].setsym("M");
}
void display_all(Console &console, Map &map1, Object player, Object merchant, enemy enemy_array[], int enemy_num){
  //takes in console, map1, player, merchant,enemy_array
  //display everything by the console member functions and its derived/polymorphic forms
  console.draw_bound(MapLevel);
  map1.display(console, MapLevel);
  player.draw_interactables(console);
  merchant.draw_interactables(console);
  for (int i = 0; i < enemy_num;i++){
    enemy_array[i].draw_interactables(console);
  }
}
void NewGame(Player &player, enemy enemy_array[], int enemy_num, Object &merchant, Map &map1, string fn){
  ifstream MapSave(fn.c_str());
  string lines;
  int i = header+1, j = 1;
  while(getline(MapSave, lines)){
    istringstream temp(lines);
    string symbol;
    while(temp >> symbol){
      map1.envir[j][i].setsym(symbol.c_str());
      map1.envir[j][i].setpos(j,i);
      map1.envir[j][i].setcolor(map1.checkTile(map1.envir[j][i]));
      j++;
    }
    i++;
    j=1;
  }
  MapSave.close();
  player.setup(100, "A", 1, RNG(5,level_height[MapLevel]-1), 254);
  random_spawn("R", map1, 5);
  random_spawn("W", map1, 5);
  random_spawn("C", map1, 5);
  random_spawn("m",map1, RNG(3,6));
  int merchantx = RNG(5,level_width[MapLevel]-5);
  int merchanty = RNG(5,level_height[MapLevel]-5);
  npc_setup(enemy_array, enemy_num, merchant, merchantx, merchanty, map1);
}

#endif
