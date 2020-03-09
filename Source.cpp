//Characters of the West    | Pits 6 players against each other in a card game.
//Justin Elli               | CISP 400
//May 11                    | 2017

//RULES

//At the start of the game, the Bartender, Overlord, Veteran, and Psychic roles
//are assigned. Each player is given 3 cards; the Bartender does not recieve any
//cards since they "aren't playing" (everyone knows who the Bartender is for
//this reason).

//Each round, every player plays one of the cards from their hand. The Overlord,
//Veteran, and Psychic then choose targets if they wish. For each player
//targeted, the targeter must have a card value higher than their target for
//their ability to be executed, unless the targeter is the Psychic. In that
//case, the target must be cheating for the ability to be executed. The targeter
//isn't revealed unless their ability states otherwise. At the end of the round,
//the remaining players draw a new card, given the game isn't over.

//Each player has a chance of cheating by using the same card twice. This proba-
//bility is equal to the card value, plus 5% per drink.

//Once an ability is executed, it has a 95% chance of securing the kill, minus
//5% per drink.

//BARTENDER
//-The Bartender chooses the heaviness of each beer served. Heavy beers are
//twice as effective as light beers.
//-If the Bartender serves the Overlord a heavy beer, the Overlord targets him
//that round.

//AMATEUR
//Ability: None
//Objective: Outlive the Overlord.
//-An amateur is any player without a special role.

//OVERLORD
//Ability: Kill any person.
//Objective: Kill the Veteran.

//VETERAN
//Ability: Challenge any person to a duel. Success reveals the Veteran.
//Objective: Kill the Overlord.
//-Heavy beers do not affect the Veteran any more than light beers do.

//PSYCHIC
//Ability: Check if a player is cheating, and kill them if they are.
//Objective: Kill the Overlord or Veteran.

#include <iostream>
#include <string>
#include <queue>                                                                //BONUS -- Uses queues.
#include <cstdlib>                                                              //rand, srand
#include <time.h>                                                               //time(NULL) for generating multiple random values.
using namespace std;

//Class Prototypes

class LimitedProbability;                                                       //Probabilities that cannot reach 100%. Used for player targeting probabilities.
struct Probabilities;                                                           //A struct for each player's probabilities.
struct WinConditions;                                                           //A struct with booleans for key elements in the game pertaining to victory or defeat.

struct Beer;                                                                    //A struct for each beer served.

class Card;                                                                     //BONUS -- Card class that returns card information.
class Deck;                                                                     //A class for the deck holding all of the cards.

class Person;                                                                   //A class for each person.
class Player;                                                                   //A class for each player (which excludes the bartender).

class Bartender;                                                                //A class for the bartender.

class Amateur;                                                                  //A class for each amateur (amateurs are players without a special role).
class Overlord;                                                                 //A class for the overlord.
class Veteran;                                                                  //A class for the veteran.
class Psychic;                                                                  //A class for the psychic.

class Main;                                                                     //A class for the main program and members and methods specific to the program.

//Function Prototypes

bool decide(float p_probability);                                               //Returns a boolean value based on a probability percentage.

//Class Definitions

class LimitedProbability
{
    float value;                                                                //This is not the actual probability; it goes through a formula whenever called to generate the actual probability.
    
    public:
    LimitedProbability(){}
    LimitedProbability(float p_value);
    
    float limit(float p_i);                                                     //The formula that generates the actual probability. It makes sure probabilities cannot reach 100%.
    
    void mod_value(float p_i);                                                  //Modifies the value variable.
    void set_value(float p_i);                                                  //Sets the value variable.
    float get_value();                                                          //Returns the actual probability.
};

struct Probabilities
{
    float ability = 0.95;                                                       //The probability of the player securing a kill.
    float drink = 0.20;                                                         //The probability of the player drinking.
    float cheat = 0.05;                                                         //The extra probability of the player cheating. This is added on to a probability based on a card value.
    LimitedProbability target[6];                                               //The probability of the player targeting the other players.
};

struct WinConditions
{
    bool overlordDead = false;                                                  //Whether or not the overlord is dead.
    bool overlordKilledByVeteran = false;                                       //Whether or not the veteran killed the overlord.
    bool overlordKilledByPsychic = false;                                       //Whether or not the psychic killed the overlord.
    bool veteranRevealed = false;                                               //Whether or not the veteran is revealed.
    bool veteranKilledByOverlord = false;                                       //Whether or not the overlord killed the veteran.
    bool veteranKilledByPsychic = false;                                        //Whether or not the psychic killed the veteran.
    bool overlordDrunk = false;                                                 //Whether or not the overlord can hit targets.
    bool veteranDrunk = false;                                                  //Whether or not the veteran can hit targets.
    bool psychicDrunk = false;                                                  //Whether or not the psychic can hit targets.
    bool noOneCanShootStraight = false;                                         //Whether or not anyone can be killed.
};

struct Beer
{
    bool strong = false;                                                        //Strong beers are twice as effective against everyone except for the veteran.
};

class Card
{
    int value;                                                                  //The card's value.
    
    public:
    Card(int p_value);
    
    string get_name();                                                          //Returns the card's information.
    int get_value();
};

class Deck
{
    queue<Card*> cards;                                                         //The cards in the deck.
    
    public:
    Deck();
    ~Deck();                                                                    //Deallocates all of the cards.
    
    void shuffle();                                                             //Shuffles the deck.
    Card* draw();                                                               //Returns the card on the top of the deck.
    void add(Card* p_card);                                                     //Adds a card to the bottom of the deck.
    void distribute(Player** p_players);                                        //Distributes cards to all of the players.
};

class Person
{
    protected:
    string fname;                                                               //The person's first name.
    string lname;                                                               //The person's last name.
    bool alive = true;                                                          //Whether or not the person is alive.
    
    public:
    Person();
    
    virtual void die(string p_killer, bool p_spacing = true);                   //Kills the person.
    void show();                                                                //Shows the person's information.
    
    void set_name();                                                            //Sets the person's name to a random name. Names are checked to make sure there are no duplicates.
    string get_name();                                                          //Returns the person's first and last name.
    bool get_alive();
};

class Player : public Person
{
    protected:
    string role;                                                                //The player's role.
    Card* hand[3] =
    {
        nullptr,
        nullptr,
        nullptr
    };                                                                          //The cards in the player's hand.
    Probabilities probabilities;                                                //The player's probabilities.
    int playing;                                                                //The index of the card from the hand the player is playing.
    bool cheating = false;                                                      //Whether or not the player is cheating.
    
    public:
    Player();
    ~Player();
    
    virtual bool ability(Player** p_players, Bartender* p_bartender,
        WinConditions* p_winconditions) = 0;                                    //Works with the player's ability.
    virtual bool winner(WinConditions p_winconditions) = 0;                     //Whether or not the player is victorious.
    virtual bool drink(Bartender* p_bartender, WinConditions* p_winconditions); //Has the player drink if they wish.
    virtual void die(string p_killer, WinConditions* p_winconditions);          //Kills the player.
    void play();                                                                //Plays a card from the player's hand.
    void draw(Deck* p_deck, int p_insert);                                      //Draws a card from the deck at the beginning of the game to fill the player's hand.
    void draw(Deck* p_deck);                                                    //Draws a card to fill the player's hand.
    int order(Bartender* p_bartender);                                          //Orders a beer.
    void increaseCheat();                                                       //Increases the player's probability of cheating.
    void suspect(int p_player, float p_amount);                                 //Increases the player's probability of targeting a certain player.
    void kill(Player* p_target, WinConditions* p_winconditions);                //Kills a player.
    void kill(Person* p_target);                                                //Kills a person (which will always be the bartender).
    
    void roundIncrease();                                                       //Increases the player's suspicions of everyone at the end of a round.
    bool compare(Player* p_target);                                             //Checks whether the value of the card the player is playing is higher than that of their target's.
    Player* chooseTarget(Player** p_players);                                   //Chooses a target.
    void show();                                                                //Shows the player's information. This wasn't needed but the assignment required it.
    
    string get_role();
    Card* get_playing();                                                        //Returns the card the player is playing.
    bool get_cheating();
};

class Bartender : public Person
{
    float strongBeer = 0.30;                                                    //The probability of the bartender serving a strong beer.
    
    public:
    Beer serve();                                                               //Returns a beer.
    void die(string p_killer, bool p_spacing);                                  //Kills the bartender.
    void revive();                                                              //Revives the bartender after a round of being dead.
};

class Amateur : public Player
{
    public:
    Amateur();
    
    bool ability(Player** p_players, Bartender* p_bartender, WinConditions*
        p_winconditions);                                                       //Does nothing. Only exists so the program doesn't crash when executing each player's ability.
    bool winner(WinConditions p_winconditions);                                 //Whether or not the amateur is victorious.
};

class Overlord : public Player
{
    bool targetBartender = false;                                               //Whether or not to target the bartender.
    
    public:
    Overlord();
    
    bool ability(Player** p_players, Bartender* p_bartender, WinConditions*
        p_winconditions);                                                       //Chooses a player to target (or the bartender) and tries to kill them.
    bool winner(WinConditions p_winconditions);                                 //Whether or not the overlord is victorious.
    bool drink(Bartender* p_bartender, WinConditions* p_winconditions);         //Has the overlord drink if they wish.
    void die(string p_killer, WinConditions* p_winconditions);                  //Kills the overlord and updates win conditions.
};

class Veteran : public Player
{
    public:
    Veteran();
    
    bool ability(Player** p_players, Bartender* p_bartender, WinConditions*
        p_winconditions);                                                       //Chooses a player to target (or the bartender) and tries to duel them.
    bool winner(WinConditions p_winconditions);                                 //Whether or not the veteran is victorious.
    bool drink(Bartender* p_bartender, WinConditions* p_winconditions);         //Has the veteran drink if they wish.
    void die(string p_killer, WinConditions* p_winconditions);                  //Kills the veteran and updates win conditions.
};

class Psychic : public Player
{
    public:
    Psychic();
    
    bool ability(Player** p_players, Bartender* p_bartender, WinConditions*
        p_winconditions);                                                       //Chooses a player to target (or the bartender) and tries to catch them cheating.
    bool winner(WinConditions p_winconditions);                                 //Whether or not the psychic is victorious.
};

//Main Class Definition

class Main
{
    int i, j;                                                                   //Iterators for Main class constructor and methods.
    bool r = true;                                                              //Whether or not the game is running.
    string c;                                                                   //For user input, used in this program for the Enter key.
    
    int currentRound;                                                           //The current round.
    WinConditions winconditions;                                                //The win conditions.
    
    Deck *deck;                                                                 //The deck.
    Bartender *bartender;                                                       //The bartender.
    Player* *players;                                                           //The players.
    
    public:
    Main();                                                                     //The main program.
    ~Main();                                                                    //Deallocates all of the members.
    
    int randomPlayer(bool p_unassigned = false);                                //Chooses a random player, and can choose from exclusively unassigned spots.
};

//Main Function

int main()
{
    Main *main = new Main;
    delete main;
}

//Function Definitions

bool decide(float p_probability)
{
    return ((float)(rand() % 100) < (p_probability * 100));
}

//Main Class Method Definitions

Main::Main()
{
    deck = new Deck;
    bartender = new Bartender;
    players = new Player*[6];
    
    bool lv_namesCleared;
    bool lv_someoneDied = false;
    bool lv_drankThisRound[6] =
    {
        true,
        true,
        true,
        true,
        true,
        true
    };
    
    srand (time(NULL));                                                         //Initialize srand so we can generate multiple random values.
    
    cout << "Characters of the West" << endl;
    cout << endl;
    
    for (i = 0; i < 6; i++)
    {
        players[i] = nullptr;
    }
    
    Overlord *overlord = new Overlord;
    players[randomPlayer(true)] = overlord;
    cout << "Overlord selected." << endl;
    
    Veteran *veteran = new Veteran;
    players[randomPlayer(true)] = veteran;
    cout << "Veteran selected." << endl;
    
    Psychic *psychic = new Psychic;
    players[randomPlayer(true)] = psychic;
    cout << "Psychic selected." << endl;
    
    
    for (i = 0; i < 6; i++)
    {
        if (players[i] == nullptr)
        {
            players[i] = new Amateur;
        }
    }
    
    do
    {
        lv_namesCleared = true;
        
        for (i = 0; i < 6; i++)
        {
            if (players[i]->get_name() == bartender->get_name())
            {
                lv_namesCleared = false;
                
                do
                {
                    players[i]->set_name();
                } while (players[i]->get_name() == bartender->get_name());
            }
            
            for (j = (i + 1); j < 6; j++)
            {
                if (players[i]->get_name() == players[j]->get_name())
                {
                    lv_namesCleared = false;
                    
                    do
                    {
                        players[j]->set_name();
                    } while (players[i]->get_name() == players[j]->get_name());
                }
            }
        }
    } while (!lv_namesCleared);
    
    cout << endl;
    cout << "Players:" << endl;
    
    for (i = 0; i < 6; i++)
    {
        players[i]->Person::show();
    }
    
    cout << endl;
    cout << "Bartender:" << endl;
    bartender->show();
    cout << endl;
    
    deck->distribute(players);
    
    cout << endl;
    
    for (currentRound = 1; r && currentRound <= 50; currentRound++)
    {
        cout << "Press Enter to begin Round " << currentRound << endl;
        
        getline(cin, c);
        
        cout << "Round " << currentRound << endl;
        cout << endl;
        
        if (winconditions.veteranRevealed)
        {
            veteran->increaseCheat();
        }
        
        for (i = 0; i < 6; i++)
        {
            if (players[i]->get_alive())
            {
                players[i]->play();
                
                if (bartender->get_alive())
                {
                    if (!players[i]->drink(bartender, &winconditions))
                    {
                        lv_drankThisRound[i] = false;
                        
                        for (j = 0; j < 6; j++)
                        {
                            if (i != j)
                            {
                                players[j]->suspect(i, 0.05);                   //Players suspect other players that don't drink because they might be saving their soberness for better aim.
                            }
                        }
                    }
                }
                else
                {
                    bartender->revive();
                }
            }
        }
        
        cout << endl;
        
        for (i = 0; r && i < 6; i++)
        {
            if (players[i]->get_alive())
            {
                if (players[i]->ability(players, bartender, &winconditions))
                {
                    lv_someoneDied = true;
                }
                
                for (j = 0; r && j < 6; j++)
                {
                    if (players[j]->winner(winconditions))
                    {
                        r = false;
                    }
                }
            }
        }
        
        if (r && winconditions.overlordDrunk && winconditions.veteranDrunk &&
            winconditions.psychicDrunk)
        {
            winconditions.noOneCanShootStraight = true;
            r = false;
            
            cout << "Game ended because everyone with killing potential is too "
                "drunk to shoot straight." << endl;
            cout << endl;
        }
        
        if (currentRound == 50)
        {
            r = false;
            
            cout << "Game ended because it went too long." << endl;
            cout << endl;
        }
        
        if (!r)
        {
            for (i = 0; i < 6; i++)
            {
                if (players[i]->winner(winconditions))
                {
                    cout << players[i]->get_name() << " (" << players[i]->
                        get_role() << ") wins!" << endl;
                }
                else
                {
                    cout << players[i]->get_name() << " (" << players[i]->
                        get_role() << ") loses!" << endl;
                }
            }
        }
        
        if (r)                                                                  //BONUS -- Semi-advanced AI for the players.
        {
            for (i = 0; i < 6; i++)
            {
                players[i]->draw(deck);
                players[i]->roundIncrease();                                    //Each player's suspicions of everyone else increases naturally over time. This helps the game end more quickly.
            }
            
            for (i = 0; i < 6; i++)
            {
                if (lv_someoneDied && !lv_drankThisRound[i])
                {
                    veteran->suspect(i, 0.10);                                  //Veteran additionally suspects those who didn't drink this round if someone died this round.
                }
                
                lv_drankThisRound[i] = true;
            }
            
            if (currentRound > 1 && lv_someoneDied)
            {
                for (i = 0; players[i] != overlord && i < 6; i++){}
                psychic->suspect(i, (0.15 * currentRound));                     //Psychic works out who the overlord is throughout the game.
                
                for (i = 0; players[i] != veteran && i < 6; i++){}
                psychic->suspect(i, (0.70 / currentRound));                     //Psychic is adept at finding the veteran early on because they are generally less suspicious to begin with.
            }
            
            lv_someoneDied = false;
        }
    }
}

Main::~Main()
{
    delete deck;
    delete bartender;
    
    for (i = 0; i < 6; i++)
    {
        delete players[i];
    }
    
    delete [] players;
}

int Main::randomPlayer(bool p_unassigned)
{
    int o;
    
    do
    {
        o = (rand() % 6);
    } while (p_unassigned && players[o] != nullptr);
    
    return o;
}

//Class Method Definitions

LimitedProbability::LimitedProbability(float p_value)
{
    value = p_value;
}

float LimitedProbability::limit(float p_i)
{
    float o;
    
    o = (0.99 - 2.20523 / ((p_i * p_i * p_i) + 2.71656 * (p_i * p_i) + 2.45991 *
        p_i + 2.2275));                                                         //BONUS -- Spent an hour on this killer formula that makes sure probabilities don't reach 100%. It graphs as a curve.
    return o;
}

void LimitedProbability::mod_value(float p_i)
{
    value += p_i;
}

void LimitedProbability::set_value(float p_i)
{
    value = p_i;
}

float LimitedProbability::get_value()
{
    return limit(value);
}

Card::Card(int p_value)
{
    value = p_value;
}

string Card::get_name()
{
    string o = "";
    
    const string lv_suits[4] =
    {
        "Clubs",
        "Diamonds",
        "Hearts",
        "Spades"
    };
    
    int lv_face = (value / 4);
    int lv_suit = (value % 4);
    
    lv_face += 1;
    
    if (lv_face > 1 && lv_face < 11)
    {
        o += to_string(lv_face);
    }
    else
    {
        if (lv_face == 1)
        {
            o += "Ace";
        }
        if (lv_face == 11)
        {
            o += "Jack";
        }
        if (lv_face == 12)
        {
            o += "Queen";
        }
        if (lv_face == 13)
        {
            o += "King";
        }
    }
    
    o += " of ";
    o += lv_suits[lv_suit];
    
    return o;
}

int Card::get_value()
{
    return value;
}

Deck::Deck()
{
    int i;
    
    for (i = 0; i < 52; i++)
    {
        add(new Card(i));
    }
    
    shuffle();
}

Deck::~Deck()
{
    while (!cards.empty())
    {
        delete cards.front();
        cards.pop();
    }
}

void Deck::shuffle()
{
    int i;
    
    vector<Card*> lv_cards;
    int lv_choice;
    
    while (!cards.empty())
    {
        lv_cards.push_back(cards.front());
        cards.pop();
    }
    
    for (i = 0; i < 52; i++)
    {
        lv_choice = (rand() % lv_cards.size());
        
        add(lv_cards[lv_choice]);
        lv_cards.erase(lv_cards.begin() + lv_choice);
    }
}

Card* Deck::draw()
{
    Card *o = cards.front();
    cards.pop();
    
    return o;
}

void Deck::add(Card* p_card)
{
    cards.push(p_card);
}

void Deck::distribute(Player** p_players)
{
    int i, j;
    
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 3; j++)
        {
            p_players[i]->draw(this, j);
        }
    }
    
    cout << "Cards distributed." << endl;
}

Person::Person()
{
    set_name();
}

void Person::die(string p_killer, bool p_spacing)
{
    alive = false;
    
    cout << get_name() << " has been killed by " << p_killer << "!" << endl;
    
    if (p_spacing)
    {
        cout << endl;
    }
}

void Person::show()
{
    cout << get_name() << endl;
}

void Person::set_name()
{
    const string lv_fnames[10] =
    {
        "Justin",
        "John",
        "Brian",
        "Caleb",
        "Donald",
        "Tyler",
        "Jesse",
        "Shane",
        "David",
        "Rick"
    };
    
    const string lv_lnames[10] =
    {
        "Brown",
        "Goodman",
        "Smith",
        "Clark",
        "Trump",
        "Harris",
        "McCree",
        "Johnson",
        "Jones",
        "Davis"
    };
    
    fname = lv_fnames[rand() % 10];
    lname = lv_lnames[rand() % 10];
}

string Person::get_name()
{
    return (fname + " " + lname);
}

bool Person::get_alive()
{
    return alive;
}

Player::Player() : Person()
{
    int i;
    
    for (i = 0; i < 6; i++)
    {
        probabilities.target[i].set_value(0.0921);
    }
}

Player::~Player()
{
    int i;
    
    for (i = 0; i < 3; i++)
    {
        if (hand[i] != nullptr)
        {
            delete hand[i];
        }
    }
}

void Player::play()
{
    if (!cheating)
    {
        int i;
        
        int lv_sum = 0;
        int lv_rand;
        
        for (i = 0; i < 3; i++)
        {
            lv_sum += hand[i]->get_value();
        }
        
        lv_rand = (rand() % lv_sum);
        
        for (i = 2; lv_rand < lv_sum; i--)
        {
            lv_sum -= hand[i]->get_value();
        }
        
        i++;
        
        playing = i;
    }
    
    cout << get_name() << " plays " << get_playing()->get_name() << "." << endl;
}

void Player::draw(Deck* p_deck, int p_insert)
{
    hand[p_insert] = p_deck->draw();
}

void Player::draw(Deck* p_deck)
{
    cheating = decide(((float)get_playing()->get_value() / (float)104) + probabilities.cheat);
    
    if (!cheating)
    {
        p_deck->add(get_playing());
        hand[playing] = p_deck->draw();
    }
}

int Player::order(Bartender* p_bartender)
{
    int o = 1;
    Beer lv_beer = p_bartender->serve();
    
    if (lv_beer.strong)
    {
        cout << "The beer is strong." << endl;
        o = 2;
    }
    
    return o;
}

bool Player::drink(Bartender* p_bartender, WinConditions* p_winconditions)
{
    bool o = false;
    
    if (decide(probabilities.drink))
    {
        o = true;
        
        cout << get_name() << " has a beer." << endl;
        
        int lv_multiplier = order(p_bartender);
        
        probabilities.ability -= (0.05 * lv_multiplier);
        probabilities.cheat += (0.05 * lv_multiplier);
        
        if (role == "Psychic" && probabilities.ability <= 0.0 &&
            !p_winconditions->psychicDrunk)
        {
            p_winconditions->psychicDrunk = true;
            
            cout << "The Psychic is too drunk to shoot straight." << endl;
        }
    }
    
    return o;
}

void Player::increaseCheat()
{
    probabilities.cheat += 0.2;
}

void Player::suspect(int p_player, float p_amount)
{
    probabilities.target[p_player].mod_value(p_amount);
}

void Player::kill(Player* p_target, WinConditions* p_winconditions)
{
    p_target->die(role, p_winconditions);
}

void Player::kill(Person* p_target)
{
    p_target->die(role);
}

void Player::die(string p_killer, WinConditions* p_winconditions)
{
    Person::die(p_killer, false);
    
    cout << get_name() << "'s role was " << role << "." << endl;
    
    cout << endl;
}

void Player::roundIncrease()
{
    int i;
    
    for (i = 0; i < 6; i++)
    {
        probabilities.target[i].mod_value(0.04);
    }
}

bool Player::compare(Player* p_target)
{
    return (get_playing()->get_value() > p_target->get_playing()->get_value());
}

Player* Player::chooseTarget(Player** p_players)
{
    int i;
    Player *o = nullptr;
    
    for (i = 0; i < 6 && o == nullptr; i++)
    {
        if (p_players[i] != this && p_players[i]->get_alive() && decide(
                probabilities.target[i].get_value()))
        {
            o = p_players[i];
        }
    }
    
    return o;
}

void Player::show()
{
    Person::show();
    
    cout << role << endl;
    
    if (cheating)
    {
        cout << "Cheating" << endl;
    }
}

string Player::get_role()
{
    return role;
}

Card* Player::get_playing()
{
    return hand[playing];
}

bool Player::get_cheating()
{
    return cheating;
}

Beer Bartender::serve()
{
    Beer o;
    o.strong = decide(strongBeer);
    
    return o;
}

void Bartender::die(string p_killer, bool p_spacing)
{
    alive = false;
    
    if (p_killer == "Overlord")
    {
        cout << get_name() << " has been killed due to unknown causes." << endl;
    }
    
    if (p_spacing)
    {
        cout << endl;
    }
}

void Bartender::revive()
{
    alive = true;
    
    cout << endl;
    cout << "The bartender has magically been revived." << endl;
    cout << endl;
}

Amateur::Amateur() : Player()
{
    role = "Amateur";
}

bool Amateur::ability(Player** p_players, Bartender* p_bartender, WinConditions*
    p_winconditions)
{
    return false;
}

bool Amateur::winner(WinConditions p_winconditions)
{
    return ((p_winconditions.overlordDead && alive) || p_winconditions.
        noOneCanShootStraight);
}

Overlord::Overlord()
{
    role = "Overlord";
}

bool Overlord::ability(Player** p_players, Bartender* p_bartender,
    WinConditions* p_winconditions)
{
    bool o = false;
    int i;
    
    if (targetBartender)
    {
        cout << "The Overlord is annoyed at the bartender for serving him a "
            "heavy beer." << endl;
        
        if (decide(probabilities.ability))
        {
            kill(p_bartender);
        }
        else
        {
            cout << endl;
        }
        
        targetBartender = false;
    }
    else
    {
        if (p_winconditions->veteranRevealed)
        {
            for (i = 0; i < 6; i++)
            {
                if (p_players[i]->get_role() == "Veteran")
                {
                    if (compare(p_players[i]))
                    {
                        cout << "The Overlord has targeted Veteran!" << endl;
                        
                        if (decide(probabilities.ability))
                        {
                            kill(p_players[i], p_winconditions);
                            o = true;
                        }
                        else
                        {
                            cout << endl;
                        }
                    }
                    else
                    {
                        cout << "The Overlord tried to target Veteran but "
                            "failed." << endl;
                        cout << endl;
                    }
                }
            }
        }
        else
        {
            Player *lv_target = chooseTarget(p_players);
            
            if (lv_target != nullptr)
            {
                if (compare(lv_target))
                {
                    cout << "The Overlord has targeted " << lv_target->
                        get_name() << "!" << endl;
                    
                    if (decide(probabilities.ability))
                    {
                        kill(lv_target, p_winconditions);
                        o = true;
                    }
                    else
                    {
                        cout << endl;
                    }
                }
                else
                {
                    cout << "The Overlord tried to target " << lv_target->
                        get_name() << " but failed." << endl;
                    cout << endl;
                }
                
            }
        }
    }
    
    return o;
}

bool Overlord::winner(WinConditions p_winconditions)
{
    return p_winconditions.veteranKilledByOverlord;
}

bool Overlord::drink(Bartender* p_bartender, WinConditions* p_winconditions)
{
    bool o = false;
    
    if (decide(probabilities.drink))
    {
        o = true;
        
        cout << get_name() << " has a beer." << endl;
        
        int lv_multiplier = order(p_bartender);
        
        if (lv_multiplier == 2)
        {
            targetBartender = true;
        }
        
        probabilities.ability -= (0.05 * lv_multiplier);
        probabilities.cheat += (0.05 * lv_multiplier);
        
        if (probabilities.ability <= 0.0 && !p_winconditions->overlordDrunk)
        {
            p_winconditions->overlordDrunk = true;
            
            cout << "The Overlord is too drunk to shoot straight." << endl;
        }
    }
    
    return o;
}

void Overlord::die(string p_killer, WinConditions* p_winconditions)
{
    Player::die(p_killer, p_winconditions);
    
    p_winconditions->overlordDead = true;
    
    if (p_killer == "Veteran")
    {
        p_winconditions->overlordKilledByVeteran = true;
    }
    if (p_killer == "Psychic")
    {
        p_winconditions->overlordKilledByPsychic = true;
    }
}

Veteran::Veteran() : Player()
{
    int i;
    
    role = "Veteran";
    
    for (i = 0; i < 6; i++)
    {
        probabilities.target[i].set_value(0.07);
    }
}

bool Veteran::ability(Player** p_players, Bartender* p_bartender, WinConditions*
    p_winconditions)
{
    bool o = false;
    
    Player *lv_target = chooseTarget(p_players);
    
    if (lv_target != nullptr)
    {
        if (compare(lv_target))
        {
            cout << "The Veteran challenges " << lv_target->get_name() << " to "
                "a duel!" << endl;
            
            if (!p_winconditions->veteranRevealed)
            {
                p_winconditions->veteranRevealed = true;
                
                cout << get_name() << " has been revealed as Veteran!" << endl;
            }
            
            if (decide(probabilities.ability))
            {
                kill(lv_target, p_winconditions);
                o = true;
            }
            else
            {
                cout << endl;
            }
        }
        else
        {
            cout << "The Veteran tried to challenge " << lv_target->get_name()
                << " but failed." << endl;
            cout << endl;
        }
    }
    
    return o;
}

bool Veteran::winner(WinConditions p_winconditions)
{
    return p_winconditions.overlordKilledByVeteran;
}

bool Veteran::drink(Bartender* p_bartender, WinConditions* p_winconditions)
{
    bool o = false;
    
    if (decide(probabilities.drink))
    {
        o = true;
        
        cout << get_name() << " has a beer." << endl;
        
        int lv_multiplier = order(p_bartender);
        
        probabilities.ability -= 0.05;
        probabilities.cheat += 0.05;
        
        if (probabilities.ability <= 0.0 && !p_winconditions->veteranDrunk)
        {
            p_winconditions->veteranDrunk = true;
            
            cout << "The Veteran is too drunk to shoot straight." << endl;
        }
    }
    
    return o;
}

void Veteran::die(string p_killer, WinConditions* p_winconditions)
{
    Player::die(p_killer, p_winconditions);
    
    if (p_killer == "Overlord")
    {
        p_winconditions->veteranKilledByOverlord = true;
    }
    if (p_killer == "Psychic")
    {
        p_winconditions->veteranKilledByPsychic = true;
    }
}

Psychic::Psychic() : Player()
{
    role = "Psychic";
}

bool Psychic::ability(Player** p_players, Bartender* p_bartender, WinConditions*
    p_winconditions)
{
    bool o = false;
    
    Player *lv_target = chooseTarget(p_players);
    
    if (lv_target != nullptr)
    {
        if (lv_target->get_cheating())
        {
            cout << "The Psychic has caught " << lv_target->get_name() <<
                " cheating!" << endl;
            
            if (decide(probabilities.ability))
            {
                kill(lv_target, p_winconditions);
                o = true;
            }
            else
            {
                cout << "The Psychic investigated " << lv_target->get_name() <<
                    " but found no evidence of cheating." << endl;
                cout << endl;
            }
        }
        else
        {
            cout << endl;
        }
    }
    
    return o;
}

bool Psychic::winner(WinConditions p_winconditions)
{
    return (p_winconditions.overlordKilledByPsychic || p_winconditions.
        veteranKilledByPsychic);
}