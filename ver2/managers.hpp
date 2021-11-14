#pragma once 
//to be used on VSCode

#include <iostream>
#include "classes.hpp"
using namespace std;

class BattleManager{
    public:
        string phase; //not_started, turn_shift, actor_phase, enemy_phase, end.
        int result; //0 for defeat, 1 for escape/forfeit, 2 for victory.
        int turnCount;

        Battler * actor;
        Battler * enemy;
        BattleManager(Battler &b1, Battler &b2);
        ~BattleManager();
        Battler& getActor(); Battler& getEnemy();

        void start(); void battleLoop();
        void currentPhase(); void nextPhase();
        void end();

        void actorPhase(); void enemyPhase();
        void processAttack(Battler *attacker, Battler *defender);
        void processHeal(Battler *user);
        void processDefeat();
};