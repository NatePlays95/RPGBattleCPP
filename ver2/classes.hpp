#pragma once 
//to be used on VSCode

#include <iostream>
using namespace std;

class Battler{
    protected:
        string name;
        int exp; //yield for enemies, total for players.
        int mhp; //max health pool
        int hp; //current health ponts

        int params[4]; //strength, dexterity, inteligence, constitution
        int acc; //accuracy, 0 to 20

        string actionState; //normal, guarding, charging, charged.
    public:
        Battler();//constructor
        ~Battler();//destructor
        
        virtual bool isPlayer(); virtual bool isEnemy();
        //getters and setters
        string getName(); void setName(string nameIn);
        int getExp(); void setExp(int expIn);
        int getMaxHp(); void setMaxHp(int mhpIn);
        int getHp(); void setHp(int hpIn);
        int getParam(int index); void setParam(int index, int value);
        void setParams(int p0, int p1, int p2, int p3);
        int getAccuracy(); void setAccuracy(int accIn);

        void setActionState(string stateIn);
        int restoreActionState(); //0 for normal, 1 for forced attack...
        bool isGuarding(); bool isCharging(); bool isCharged();

        void damage(int amount);
        void heal(int amount);
        virtual void gainExp(int amount);
        virtual int getAttackRoll();
};

class Player : public Battler{
    protected:
        int level;
    public:
        Player(string nameIn);
        bool isPlayer();
        int getAttackRoll();

        int getLevel(); void setLevel(int levelIn);
        int getExpToLevelUp(); void levelUp();
        void gainExp(int amount);

        //friend func has access to private vars.
        friend ostream& operator<<(ostream& os, const Player& player);
        friend istream& operator>>(istream& is, Player& player);
};

class Enemy : public Battler{
    public:
        //default constructor
        Enemy(string nameIn, int expIn, int mhpIn, int p1, int p2, int p3, int p4);
        //custom constructor from name
        Enemy(string nameIn);
        bool isEnemy();
        int getAttackRoll();
        int chooseAction(Battler *opponent, int turnCount); //0=wait, 1=attack, 2=charge, 3=defend, 4=heal;
};