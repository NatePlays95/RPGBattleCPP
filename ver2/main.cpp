#include <iostream>
#include <fstream>
#include <ctime> 
#include "classes.hpp"
#include "managers.hpp"
using namespace std;
  

int main(){
    //variable setup
    srand(time(0));
    
    string inp; //handles terminal input
    cout << "Welcome adventurer! Please tell us more about yourself:\n-Name-: ";
    cin >> inp;
    Player player(inp);
    while(true){
        cout << "\nDo you wish to battle against a monster?" << endl << "[Battle][Exit]: ";
        cin >> inp;
        if (inp == "Exit") break;
        else if (inp != "Battle") cout << "I'll take that as a yes..." << endl;
        cout << "Excellent! Now choose which enemy you want to challenge:\n"
        << "[Slime][Zombie][Skeleton][Fighter][Behemoth]: ";
        cin >> inp;
        
        Enemy enemy("",0,0,0,0,0,0);
        if (inp == "Slime" || inp == "Zombie" || inp == "Skeleton" ||
            inp == "Fighter" || inp == "Behemoth") {
                 enemy = Enemy(inp);
        } else cout << "I'm afraid we don't have that enemy at our disposal.\nType the enemy name correctly.";
        
    if (enemy.getName()!=""){
            BattleManager* bm = new BattleManager(player, enemy);
            //cout << "DEBUG " << bm.phase << " " << bm.enemy.getName() << bm.enemy.getHp() << " turn " << bm.turnCount << endl;
            bm->start();
            if (player.getHp() <= 0) break;
        }
    }
    //string savename = player.getName()+".txt";
    ofstream savefile("savefile.txt");
    savefile << player << endl;
    savefile.close();

    cout << "Your character was saved to a text file.\nHope to see you soon!";
    return 0;
}