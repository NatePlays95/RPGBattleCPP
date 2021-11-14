//VERSION 1
#include <iostream>
using namespace std;

//entity includes both player and enemies
class Battler{
    protected:
        string name;
        int mhp; //max health pool
        int hp; //current health ponts
        int atk; 
        int def;
    //getters and setters
    public:
        Battler(){} //constructor, leave empty
        string getName(){
            return this->name;
        }
        void setName(string nameIn){
            this->name = nameIn;
        }
        int getMaxHp(){
            return this->mhp;
        }
        void setMaxHp(int value){
            this->mhp = value;
            this->hp = value; //refill hp
        }
        int getHp(){
            return this->hp;
        }
        void setHp(int value){
            this->hp = value;
        }
        int getAtk(){
            return this->atk;
        }
        void setAtk(int value){
            this->atk = value;
        }
        int getDef(){
            return this->def;
        }
        void setDef(int value){
            this->def = value;
        }
    //methods
    public:
        void processDamage(int value){
            this->setHp(this->hp - value);
        }
        void heal(){
            int amount = 2;
            this->processDamage(-amount);
            cout << this->getName() << " heals themself for " << amount << " points of damage!" << endl;
        }
        


};

class Player : public Battler{
    protected:
        int exp;
    public:
       //constructor 
        Player(string nameIn){
            setName(nameIn);
            setMaxHp(20);
            setAtk(3);
            setDef(1);
        }
        void commemorate(){
            cout << name << " is celebrating!" << endl;
        }
};

class Enemy : public Battler{
    public:
        //constructor
        Enemy(string nameIn, int mhpIn, int atkIn, int defIn){
            setName(nameIn);
            setMaxHp(mhpIn);
            setAtk(atkIn);
            setDef(defIn);
        }
};

class BattleManager{
    public:
        string phase; //not_started, turn_shift, actor_phase, enemy_phase, end.
        int result; //0 for defeat, 1 for victory, 2 for error.
        int turnCount;
        Battler actor;
        Battler enemy;
    
    //constructor
    public:
        BattleManager(Battler actorIn, Battler enemyIn){
            this->actor = actorIn; this->enemy = enemyIn;
            this->turnCount = 0;
            this->result = 2;
            this->phase = "not_started";
        }

        void start(){  
            phase = "turn_shift";
            processBattleLoop();
        }
    private:
        void end(){
            if (result == 0) cout << "Game Over.";
            else if (result == 1){
                cout << actor.getName() << " won! Good job!";
            }
            else cout << "An unexpected error stopped the battle.";
        }
        void processBattleLoop(){
            while (phase != "end"){
                cout << "DEBUG " << phase << " " << enemy.getName() << enemy.getHp() << " turn " << turnCount << endl;
                nextPhase();
            }
            end();
            //processing won't stop until battle ends.
        }
        void nextPhase(){
            if (phase == "turn_shift"){
                turnCount++;
                cout << "\nTURN " << turnCount << "\n" << endl;//debug
                phase = "actor_phase";
                //if (turnCount > 9) phase = "end";
                return;
            } else if (phase == "actor_phase"){
                ActorPhase();
                if (phase != "end") phase = "enemy_phase";
                return;
            } else if (phase == "enemy_phase"){
                EnemyPhase();
                if (phase != "end") phase = "turn_shift";
                return;
            } else {
                cout << "error" << endl;
                this->result = 2;
                phase = "end";
                return;
            }
        }
        void ActorPhase(){
            cout << actor.getName() << ", HP = " << actor.getHp() << "/" << actor.getMaxHp() << endl;
            cout << "What does " << actor.getName() << " do?" << endl;
            cout << "[Attack][Heal][Observe]: ";
            string action;
            cin >> action;
            if (action == "q") phase = "end"; //escape word.
            else if (action == "Attack") processAttack(actor, enemy);
            else if (action == "Heal") actor.heal();
            else if (action == "Observe") {
                cout << "Enemy " << enemy.getName() << " has " << enemy.getHp() << " HP left." << endl;
            }
            else {
                cout << actor.getName() << " does nothing.\n(make sure to type the action exactly as described)" << endl;
            }
        }
        void EnemyPhase(){
            cout << "\nIt is " << enemy.getName() << "'s time to act!" << endl;
            if (enemy.getHp() < enemy.getMaxHp()/3){ //enemy heals
                enemy.heal();
            } else { //enemy attacks
                processAttack(enemy, actor);
            }
        }

        void processAttack(Battler &attacker, Battler &defender){ //move to BattleManager later
            //assume hit
            int amount = max(0,attacker.getAtk() - defender.getDef());
            defender.processDamage(amount); //damage formula: atk - def
            cout << attacker.getName() << " hits " << defender.getName() << " for " << amount << " points of damage!" << endl;
            if (defender.getHp() <= 0) this->processDefeat();
        }

        void processDefeat(){
            if (this->actor.getHp() <= 0){
                result = 0;
                cout << actor.getName() << " was defeated." << endl;
            } else if (this->enemy.getHp() <= 0){
                result = 1;
                cout << enemy.getName() << " was defeated." << endl;
            }
            this->phase = "end";
        }
};

//main process: battle!

int main(){
    string inp; //handles terminal input
    cout << "Welcome adventurer! Please tell us more about yourself:\n-Name-: ";
    cin >> inp;
    Player player(inp);
    Enemy enemy("",1,1,1);
    cout << "Excellent! Now choose which enemy you want to challenge:\n"
    << "[Slime][Zombie][Behemoth]: ";
    cin >> inp;
    
    if (inp == "Slime") enemy = Enemy("Slime",5,2,0);
    else if (inp == "Zombie") enemy = Enemy("Zombie",8,3,1);
    else if (inp == "Behemoth") enemy = Enemy("Behemoth",30,5,2);
    else{
        cout << "I'm afraid we don't have that enemy at our disposal.\nType the enemy name correctly.";
        return 0;
    }

    BattleManager bm(player, enemy);
    //cout << "DEBUG " << bm.phase << " " << bm.enemy.getName() << bm.enemy.getHp() << " turn " << bm.turnCount << endl;
    bm.start();





    return 0;
}
