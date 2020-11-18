#include "Basics.hpp"
#include "Console.hpp"
#include "Inventory.hpp"
#include "Object.hpp"
#include "Map.hpp"
#include "Global_control.hpp"

int main(){
  Console console = Console();
  Map map1;
  Inventory inventory;
  Player player;
  Object merchant;
  int enemy_num = 1;//init to a dummy number
  enemy *enemy_array;//dynamic array
  int difficulty = 0, key, out_key, turn = 0;
  int *levelptr = &MapLevel;//pointer of the maplevel; to ensure immediate update

  //menu and game save control
  while ( key != KEY_SPACE ){
    console.showMenu();
    key = getch();
    if (key == KEY_ESC ){
      console.goodbye();
      console.quit();
      break;
    }
    if ( key == KEY_SPACE ){//new game LEVEL 0
      erase();
      //allow user to choose difficulty
      mvprintw(10,20,"Choose Difficulty:");
      mvprintw(12,20,"1. Easy");
      mvprintw(14,20,"2. Normal");
      mvprintw(16,20,"3. Hard");
      mvprintw(18,20,"4. Crazy");
      mvprintw(20,20,"5. Inhumane");
      difficulty = getch();
      switch (difficulty) {
        case KEY_ONE:
          enemy_num = 2;
          break;
        case KEY_TWO:
          enemy_num = 4;
          break;
        case KEY_THREE:
          enemy_num = 6;
          break;
        case KEY_FOUR:
          enemy_num = 8;
          break;
        case KEY_FIVE:
          enemy_num = 15;
          break;
      }
      enemy_array = new enemy[enemy_num]{};//init the dynamic array by the updated enemy_num
      NewGame(player, enemy_array, enemy_num, merchant, map1, "MAP0.txt");
      break;
    }
    if (key == KEY_TWO){//load presaved game
      //extract enemy_num first
      if( load_enemy_num() != -1 ) {
        enemy_num = load_enemy_num();
        enemy_array = new enemy[enemy_num]{}; //init dynamic array by the new keyword
      }
      if( LoadSave("MapSave.txt","PlayerSave.txt","EnemySave.txt",player, enemy_array, merchant, inventory, map1, MapLevel, difficulty) ) {
        console.console_output = "Save loaded.";
        break;
      }
      else{//if saves do not exist or corrupted
        console.draw_prompt(0,0,"Could not open save files. Either they do not exist or are corrupted. Please don't tamper with the filenames.\
        Try deleting the existing save(s) and resave again.");
        getch();
      }
    }
  }

  erase();
  refresh();

  display_all(console, map1, player, merchant, enemy_array, enemy_num);
  console.noticeBar();

  //below is the main game loop; controlled by the user's keyboard input
  while (console.quit_status() == false){
    console.noticeBar();//show noticeBar for each new loop
    //check if player dead or not
    if (player.gethp() <= 0){
      console.defeated();
      console.quit();
      break;//gameover
    }
    //check the status of the equipped weapon
    if(inventory.getweapon().name != "null"){
      if(!inventory.WeaponStatus()){
        inventory.equipped_weapon.clear();//automatically delete weapon if durability <= 0
        console.console_output = "Your equipped weapon has broken!!@.@";
      }
    }
    //draw console_output
    if(console.console_output != "")  {console.draw_prompt(0,0,console.console_output);}

    //show player hp and maplevel and difficulty
    console.draw_prompt(0,1,"Player HP = " + int_to_str(player.gethp()));
    console.draw_prompt(0,2,"Map Level " + int_to_str(*levelptr) + "          Difficulty: " + str_difficulty(difficulty));

    turn++; //increment of turn; this control the chasing pattern of the enemies

    //level progression; if player successfully interact with "G":
    if(lv_completed[MapLevel]){
      if(MapLevel == num_level-1){//if finished the last level
        console.wingame();
        break;
      }
      console.levelup();//this increment MapLevel; changes reflected by the levelptr

      //switch to new map by loading new template
      //also reset the positions of most objects
      string mapsavefilename = "MAP"+int_to_str(*levelptr)+".txt";
      SaveGame("temp_map.txt","temp_player.txt","temp_enemy.txt",player,inventory,map1,MapLevel,enemy_array,enemy_num,merchant,difficulty);
      LoadSave(mapsavefilename,"temp_player.txt","temp_enemy.txt",player, enemy_array, merchant, inventory, map1, MapLevel,difficulty);
      random_spawn("H", map1, 5);
      random_spawn("R", map1, 5);
      random_spawn("W", map1, 5);
      random_spawn("C", map1, 12);
      random_spawn("P", map1, 2);
      random_spawn("K", map1, 3);
      random_spawn("m", map1, 8);
      npc_setup(enemy_array, enemy_num, merchant,RNG(5,level_width[MapLevel]-5),RNG(5,level_height[MapLevel]-5),map1);
    }
    //control chase pattern of enemy
    //enemy moves per two move of player to reduce aggressiveness
    if( turn % 2 == 0 ){
      global_chase( player, enemy_array, enemy_num, map1 );//chase algorithm

      for ( int i = 0; i < enemy_num; i++ ){//check if any enemy near player
        if( enemy_array[i].attack(player,2) ){//if near, minus the player hp by 2 hp
          console.console_output = "You are being attacked!!!";
        }
      }
    }

    //redraw everything to reflect new situations
    refresh();
    display_all(console, map1, player, merchant, enemy_array, enemy_num);

    //getkey from user keyboard; make sure lowercase
    key = getch();//this is an integer ASCII value
    switch (key) {
      case M:
        erase();
        console.showMenu();
        out_key = getch();
        while (out_key != M){
          if(out_key == KEY_ESC){
            console.goodbye();
            console.quit();
            break;
          }
          if (out_key == KEY_ONE ){
            SaveGame("MapSave.txt","PlayerSave.txt","EnemySave.txt",player,inventory,map1,MapLevel,enemy_array,enemy_num,merchant,difficulty);
            console.draw_prompt(0,0,"Game saved. Press any key to continue.");
            getch();
            break;
          }
          console.showMenu();
          out_key = getch();
        }
        erase();
        turn = 0;
        refresh();
        break;
      case W:
        if(map1.checkSurrounding(player,NORTH) == ROAD && player.isOverlap(enemy_array, enemy_num,player.getx(), player.gety()-1)==0 ) {player.Move(NORTH);steps++;}
        break;
      case S:
        if(map1.checkSurrounding(player,SOUTH) == ROAD && player.isOverlap(enemy_array, enemy_num,player.getx(), player.gety()+1)==0) {player.Move(SOUTH);steps++;}
        break;
      case A:
        if(map1.checkSurrounding(player,WEST) == ROAD && player.isOverlap(enemy_array, enemy_num,player.getx()-1, player.gety())==0) {player.Move(WEST);steps++;}
        break;
      case D:
        if(map1.checkSurrounding(player,EAST) == ROAD && player.isOverlap(enemy_array, enemy_num,player.getx()+1, player.gety())==0) {player.Move(EAST);steps++;}
        break;
      case L:
        if(map1.interact(player, EAST, inventory, console)) {map1.wipe(player,EAST);}
        attack_control(player, enemy_array, enemy_num, EAST, inventory, console);
        break;
      case K:
        if(map1.interact(player, SOUTH, inventory, console)) {map1.wipe(player,SOUTH);}
        attack_control(player, enemy_array, enemy_num, SOUTH, inventory, console);
        break;
      case J:
        if(map1.interact(player, WEST, inventory, console)) {map1.wipe(player,WEST);}
        attack_control(player, enemy_array, enemy_num, WEST, inventory, console);
        break;
      case I:
        if(map1.interact(player, NORTH, inventory, console)) {map1.wipe(player,NORTH);}
        attack_control(player, enemy_array, enemy_num, NORTH, inventory, console);
        break;
      case E:
        if(player.isProximity(merchant)){
          erase();
          merchant_shop(inventory,console);
          erase();
          turn = 0;
        }
        break;
      case B:
        erase();
        inventory.showInv();
        out_key = getch();
        while (out_key != B && out_key != KEY_ESC){
          inventory.showInv();
          out_key = getch();
        }
        erase();
        turn=0;
        break;
      case C:
        inventory.showCrafted();
        out_key = getch();
        while (out_key != C && out_key != KEY_ESC){
          if(out_key == E){
            inventory.equip(console);
          }
          if(out_key == D){
            inventory.discard(console);
          }
          inventory.showCrafted();
          out_key = getch();
        }
        erase();
        turn=0;
        break;
      case Q:
        erase();
        inventory.craftingTable(console);
        erase();
        turn=0;
        break;
      case P:
        erase();
        console.showkeybind();
        out_key = getch();
        while (out_key != P && out_key != KEY_ESC){
          console.showkeybind();
          out_key = getch();
        }
        erase();
        turn=0;
        break;
      refresh();
    }
  }

  delete []enemy_array;//free memory
  system("CLS");//reset terminal
  return 0;
}
