#include <iostream>
#include <cmath>
#include <Windows.h>
#include "classes.hpp"
#include "managers.hpp"
using namespace std;

//----------------------------------------------------------
//                   BattleManager
BattleManager::BattleManager(Battler &b1, Battler &b2){
    this->actor = &b1; this->enemy = &b2; this->phase = "not_started"; this->turnCount = 0;
}
BattleManager::~BattleManager(){};
Battler& BattleManager::getActor(){ return *actor; }
Battler& BattleManager::getEnemy(){ return *enemy; }

// void BattleManager::start(){ //debug start
//     int a = actor->getAttackRoll();
//     int b = enemy->getAttackRoll();

//     cout << a << " " << b << endl;
// }
void BattleManager::start(){
    this->phase = "turn_shift";
    currentPhase();
    // while(this->phase != "end"){}
}

void BattleManager::currentPhase(){
    //cout << "debug exp" << actor->getExp() << " " << enemy->getExp() << endl;
    Sleep(800);
    if (this->phase == "end"){
        end(); return;
    } else if (this->phase == "turn_shift"){
        turnCount++;//hard coded turn shift logic
        cout << "\nTURN " << turnCount << "\n" << endl;
    } else if (this->phase == "actor_phase"){
        actorPhase();
    } else if (this->phase == "enemy_phase"){
        enemyPhase();
    }
    nextPhase();
}

void BattleManager::nextPhase(){
    if (this->phase == "end"){
        end(); return;//both on NextPhase and CurrentPhase to prevent errors.
    } else if (this->phase == "turn_shift"){
        this->phase = "actor_phase";
    } else if (this->phase == "actor_phase"){
        this->phase = "enemy_phase";
    } else if (this->phase == "enemy_phase"){
        this->phase = "turn_shift";
    }
    currentPhase();
}

void BattleManager::end(){
    Sleep(400);
    actor->setActionState("normal");
    enemy->setActionState("normal");
    if (result == 0){
        cout << "Game Over." << endl;
    }
    else if (result == 1){
        cout << actor->getName() << " escaped the battle." << endl;
    } 
    else if (result == 2){
        //award xp
        int expYield = enemy->getExp();
        cout << actor->getName() << " gained " << expYield << " exp, for a total of " <<
            actor->getExp()+expYield << " points!" << endl;
        actor->gainExp(expYield);
    }
    else cout << "An unexpected error stopped the battle.";
    Sleep(1000);
}

void BattleManager::actorPhase(){
    cout << actor->getName() << ", HP = " << actor->getHp() << "/" << actor->getMaxHp() << endl;
    
    int state = actor->restoreActionState();
    if (state == 1) processAttack(actor, enemy);
    else if (state == 0){
        cout << "What does " << actor->getName() << " do?" << endl;
        cout << "[Attack][Charge][Guard][Heal][Scan][Flee]: "; string action; cin >> action;
        if (action == "Flee"){
            result = 1;
            phase = "end";
        } else if (action == "Scan"){
            cout << "Enemy " << enemy->getName() << " has " << enemy->getHp() << " HP left." << endl;
        } else if (action == "Heal"){
            processHeal(actor);
        } else if (action == "Guard"){
            actor->setActionState("guarding");
            cout << actor->getName() << " prepares themself for incoming attacks." << endl;
        } else if (action == "Charge"){
            actor->setActionState("charging");
            cout << actor->getName() << " charges a strong attack..." << endl;
        } else if (action == "Attack"){
            processAttack(actor, enemy);
        } 
    }
}
void BattleManager::enemyPhase(){
    //AI goes here
    int state = enemy->restoreActionState();
    //cout << "debug state " << state;
    if (state == 1) processAttack(enemy, actor); //forced attack
    else if (state == 0){
        int choice = 1;
        if (enemy->isEnemy()){
            Enemy * _e = (Enemy*)enemy;
            choice = _e->chooseAction(actor, turnCount);
            //cout << " debug choice " << choice << endl;
        }
        if (choice == 0){
            //cout << "WAIT";
            cout << enemy->getName() << " waits..." << endl;
        } else if (choice == 1) {
            //cout << "ATK";
            processAttack(enemy, actor);
        } else if (choice == 2) {
            //cout << "CHARGE";
            enemy->setActionState("charging");
            cout << enemy->getName() << " charges a strong attack..." << endl;
        } else if (choice == 3) {
            //cout << "BLOCK";
            enemy->setActionState("guarding");
            cout << enemy->getName() << " prepares itself for incoming attacks." << endl;
        } else if (choice == 4) {
            //cout << "HEAL";
            processHeal(enemy);
        }
    }
}

void BattleManager::processAttack(Battler *attacker, Battler *defender){
    cout << attacker->getName() << " attacks!" << endl;
    Sleep(400);
    //hit formula: d20 >= 10 + acc + dex - dex
    int hitChance = 10 + attacker->getAccuracy() + attacker->getParam(1) - defender->getParam(1);
    int hitDice = 20-rand()%20; //cout << "debug hit " << hitChance << " " << hitDice << endl;
    if(hitDice <= hitChance){
        //damage formula: str + roll - con
        int amount = max(0,attacker->getAttackRoll() - defender->getParam(3)); 
        //multipliers Guard *0.5, Charge Attack *2
        amount = floor(float(amount) * (1.0+attacker->isCharged())/(1.0+defender->isGuarding()));
        
        defender->damage(amount);
        cout << attacker->getName() << " hits " << defender->getName() << " for " << amount << " points of damage!" << endl;
        
        if (defender->getHp() <= 0) this->processDefeat();
    } else {
        //miss
        cout << attacker->getName() << "'s attack misses... " << endl;
    }
    Sleep(400);
}
void BattleManager::processHeal(Battler *user){
    //heal formula: roll (1 to 2*int)
    int amount = 1+rand()%(max(2 * user->getParam(2),1));
    user->heal(amount);
    cout << user->getName() << " heals themself for " << amount << " HP." << endl;
}

void BattleManager::processDefeat(){
    Sleep(400);
    if(actor->getHp()<=0){
        result = 0;
        cout << actor->getName() << " was defeated." << endl;
    } else if(enemy->getHp()<=0){
        result = 2;
        cout << enemy->getName() << " was defeated." << endl;
    }
    this->phase = "end";
}