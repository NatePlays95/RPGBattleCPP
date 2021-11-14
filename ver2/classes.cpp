#include <random>
#include "classes.hpp"
using namespace std;

//----------------------------------------------------------
//                       Battler
Battler::Battler(){}
Battler::~Battler(){}

bool Battler::isPlayer(){ return false; } 
bool Battler::isEnemy(){ return false; }

string Battler::getName(){ return this->name; }
void Battler::setName(string nameIn){ this->name = nameIn; }
int Battler::getExp(){ return this->exp; }
void Battler::setExp(int expIn){ this->exp = expIn; }
int Battler::getMaxHp(){ return this->mhp; }
void Battler::setMaxHp(int mhpIn){ this->mhp = mhpIn; this->hp = mhpIn; }
int Battler::getHp(){ return this->hp; }
void Battler::setHp(int hpIn){ this->hp = hpIn; }
int Battler::getParam(int index){ return this->params[index]; }
void Battler::setParam(int index, int value){ this->params[index] = value; }
void Battler::setParams(int p0, int p1, int p2, int p3){
    this->params[0] = p0; this->params[1] = p1; this->params[2] = p2; this->params[3] = p3;}
int Battler::getAccuracy(){ return this->acc; }
void Battler::setAccuracy(int accIn){ this->acc = accIn; }

void Battler::setActionState(string stateIn){
    this->actionState = stateIn;
}
int Battler::restoreActionState(){
    if (isGuarding()){
        this->setActionState("normal"); return 0; //continue as normal
    } else if (isCharged()){
        this->setActionState("normal"); return 0;
    } else if (isCharging()){
        this->setActionState("charged"); return 1; //forced attack
    } else return 0;
}
bool Battler::isGuarding(){ return (this->actionState == "guarding"); }
bool Battler::isCharging(){ return (this->actionState == "charging"); }
bool Battler::isCharged(){ return (this->actionState == "charged"); }

void Battler::damage(int amount){
    this->setHp(min(this->mhp, max(this->hp - amount, 0)));
}
void Battler::heal(int amount){
    this->damage(-amount);
}
void Battler::gainExp(int amount){
    this->setExp(this->exp + amount);
}
int Battler::getAttackRoll(){
    return this->params[0];
}






//----------------------------------------------------------
//                       Actor
Player::Player(string nameIn){
    setName(nameIn); setExp(0); setLevel(1); setMaxHp(20);
    setParams(2, 1, 1, 1); setAccuracy(7);
    setActionState("normal");
}
int Player::getAttackRoll(){
    //cout << "player" << endl;
    return Battler::getAttackRoll() + 1+(rand()%4); //strength + (1 to 4)
}
bool Player::isPlayer(){ return true; }

int Player::getLevel(){ return this->level; }
void Player::setLevel(int levelIn){ this->level = levelIn; }
// int Player::getExpToLevelUp(){
//     int value = 3; //formula is 3 * 2^(level-1), so 3, 6, 12, 24...
//     for(int i=0;i < level-1;i++){ value *= 2; }
//     return value;
// }
int Player::getExpToLevelUp(){
    //formula 0.5*(level+1)^2, so 2,4,8,12,18,24,32
    return (level+1)*(level+1)/2;
}
void Player::levelUp(){
    this->level += 1;
    cout << "Level Up! Choose a stat to increase" << endl;
    while(true){
        cout << "[Str][Dex][Int][Con]: ";
        string inp; cin >> inp;
        if        (inp == "Str"){
            this->setParam(0, this->getParam(0)+1); break;
        } else if (inp == "Dex") {
            this->setParam(1, this->getParam(1)+1); break;
        } else if (inp == "Int") {
            this->setParam(2, this->getParam(2)+1); break;
        } else if (inp == "Con") {
            this->setParam(3, this->getParam(3)+1); break;
        }
    }
    this->setMaxHp(this->getMaxHp()+2);
    if (this->exp >= this->getExpToLevelUp()) levelUp(); //recursive
}
void Player::gainExp(int amount){
    this->setExp(this->exp + amount);
    if (this->exp >= this->getExpToLevelUp()) levelUp();
}
//save player
ostream& operator<<(ostream& os, const Player& p){
    // write out individual members of s with an end of line between each one
    os << p.name << "\n" << p.level << "\n" << p.exp << "\n" << p.mhp << "\n";
    os << p.params[0] << "\n" << p.params[1] << "\n" << p.params[2] << "\n"<< p.params[3] << "\n";
    os << p.acc << endl;
    return os;
}
istream& operator>>(istream& is, Player& p){
    // read in individual members of s
    is >> p.name >> p.level >> p.exp >> p.mhp >> p.params[0] >> p.params[1] >> p.params[2] >> p.params[3] >> p.acc;
    return is;
}


//----------------------------------------------------------
//                       Enemy
Enemy::Enemy(string nameIn, int expIn, int mhpIn, int p1, int p2, int p3, int p4){
    setName(nameIn);setExp(expIn);setMaxHp(mhpIn);
    setParams(p1,p2,p3,p4); setAccuracy(3);
    setActionState("normal");
}
Enemy::Enemy(string nameIn){
    this->~Enemy();
    if (nameIn == "Slime"){
        new (this) Enemy("Slime",1,5, 1,0,0,0);

    } else if (nameIn == "Zombie"){
        new (this) Enemy("Zombie",3,10, 3,2,0,2);

    } else if (nameIn == "Skeleton"){
        new (this) Enemy("Skeleton",4,8, 3,5,1,1);

    } else if (nameIn == "Fighter"){
        new (this) Enemy("Fighter",9,20, 4,3,3,3);

    } else if (nameIn == "Behemoth"){
        new (this) Enemy("Behemoth",20,50, 8,4,5,2);

    } else {
        new (this) Enemy("",0,0,0,0,0,0);
    }
}
bool Enemy::isEnemy(){ return true; }

int Enemy::getAttackRoll(){
    //cout << "enemy" << endl;
    return Battler::getAttackRoll() + 1+(rand()%2); //strength + (1 to 2) 
}

int Enemy::chooseAction(Battler *opponent, int turnCount){ //0=wait, 1=attack, 2=charge, 3=defend, 4=heal;
    int ret = 1;
    if(name == "Behemoth"){
        if (this->hp > 20) ret = 2; //if high HP, charge
        else if (this->hp < 15 && turnCount%4 == 0) ret = 4; //if low HP, heal (1 to 10)
        else ret = 1; //attack
    }
    else if (name == "Fighter"){
        if (turnCount == 1){
            cout << name + " is observing your moves carefully." << endl;
            ret = 0;
        }
        else if (opponent->isCharging()) ret = 3; //always block charges.
        else ret = 1;
    }
    else if (name == "Skeleton"){
        if (this->hp >= 3) ret = 4;
        else ret = 1;
    } else if (name == "Zombie"){
        if (turnCount < 4){
            ret = 2;
        } else ret = 1;
    }
    return ret;
}