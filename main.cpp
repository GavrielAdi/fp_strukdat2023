#include <bits/stdc++.h>

using namespace std;

class GameEntity {
  public: virtual~GameEntity() {}

  virtual string getName() const = 0;
  virtual void displayInfo() const = 0;
  virtual int attack() const = 0;
  virtual void takeDamage(int damage) = 0;
  virtual void defend() = 0;
  virtual bool isDefeated() const = 0;
  virtual void addConnection(GameEntity * entity) = 0;
  virtual void displayConnections() const = 0;
};

class Player: public GameEntity {
  public: Player(string name, int health): name(name),
  health(health),
  healthPotion(3) {}

  string getName() const override {
    return name;
  }

  void displayInfo() const override {
    cout << "Player: " << getName() << ", Health: " << health << endl;
  }

  int attack() const override {
    return rand() % 11 + 10;
  }

  int attack(int bonusDamage) const {
    int baseDamage = rand() % 11 + 10;
    int totalDamage = baseDamage + bonusDamage;

    cout << getName() << " attacks with damage: " << totalDamage;
    if (bonusDamage > 0) {
      cout << " (Bonus damage: " << bonusDamage << ")";
    }
    cout << endl;

    return totalDamage;
  }

  void takeDamage(int damage) override {
    if (defending) {
      damage /= 2;
      defending = false;
    }
    health -= damage;
    if (health < 0) {
      health = 0;
    }
  }

  void defend() override {
    defending = true;
  }

  bool isDefeated() const override {
    return health == 0;
  }

  void addConnection(GameEntity * entity) override {
    connections.push_back(entity);
  }

  void displayConnections() const override {
    for (const GameEntity * connectedEntity: connections) {
      cout << connectedEntity -> getName() << " ";
    }
    cout << endl;
  }

  int getHealth() const {
    return health;
  }

  void useHealthPotion() {
    if (healthPotion > 0) {
      health += 20; // Atur sesuai dengan jumlah kesehatan yang ingin ditambahkan
      healthPotion--;
      cout << getName() << " uses a health potion. Health restored!" << endl;
      cout << "Current health: " << health << endl;
    } else {
      cout << "No health potions left." << endl;
    }
  }

  private: string name;
  int health;
  int healthPotion;
  bool defending = false;
  vector < GameEntity * > connections;
};

class NonPlayerCharacter: public GameEntity {
  public: NonPlayerCharacter(string name, string role, int health): name(name),
  role(role),
  health(health) {}

  string getName() const override {
    return name;
  }

  void displayInfo() const override {
    cout << "NPC: " << getName() << ", Role: " << role << ", Health: " << health << endl;
  }

  int attack() const override {
    return rand() % 11 + 5;
  }

  void takeDamage(int damage) override {
    health -= damage;
    if (health < 0) {
      health = 0;
    }
  }

  void defend() override {
    // NPC doesn't defend
  }

  bool isDefeated() const override {
    return health == 0;
  }

  void addConnection(GameEntity * entity) override {
    connections.push_back(entity);
  }

  void displayConnections() const override {
    for (const GameEntity * connectedEntity: connections) {
      cout << connectedEntity -> getName() << " ";
    }
    cout << endl;
  }

  int getHealth() const {
    return health;
  }

  private: string name;
  string role;
  int health;
  vector < GameEntity * > connections;
};

class Area: public GameEntity {
  public: Area(string name): name(name) {}

  string getName() const override {
    return name;
  }

  void displayInfo() const override {
    cout << "Area: " << getName() << endl;
  }

  int attack() const override {
    // Areas don't attack in this example
    return 0;
  }

  void takeDamage(int damage) override {
    // Areas can't take damage in this example
  }

  void defend() override {
    // Areas don't defend in this example
  }

  bool isDefeated() const override {
    return false;
  }

  void addConnection(GameEntity * entity) override {
    // Areas can have connections to other entities (Players, NPCs, or other Areas)
    connections.push_back(entity);
  }

  void displayConnections() const override {
    for (const GameEntity * connectedEntity: connections) {
      cout << connectedEntity -> getName() << " ";
    }
    cout << endl;
  }

  const vector < GameEntity * > & getConnections() const {
    return connections;
  }
  private: string name;
  vector < GameEntity * > connections;
};

class Graph {
  public: void addArea(Area * area) {
    areas.push_back(area);
  }

  void connectAreas(Area * area1, Area * area2) {
    if (area1 && area2) {
      // Check if the areas are not already connected
      if (find(area1 -> getConnections().begin(), area1 -> getConnections().end(), area2) == area1 -> getConnections().end()) {
        area1 -> addConnection(area2);
      }

      if (find(area2 -> getConnections().begin(), area2 -> getConnections().end(), area1) == area2 -> getConnections().end()) {
        area2 -> addConnection(area1);
      }
    }
  }

  void connectEntities(GameEntity * entity1, GameEntity * entity2) {
    if (entity1 && entity2) {
      entities[entity1].push_back(entity2);
      entities[entity2].push_back(entity1);
    }
  }

  void displayAreas() const {
    cout << "Areas in the game:" << endl;
    for (const auto & area: areas) {
      cout << area -> getName() << endl;
    }
    cout << endl;
  }

  void displayConnections() const {
    for (const auto & entry: areas) {
      for (const auto & connectedArea: areas) {}
    }
  }

  const std::vector < GameEntity * > & getEntities(const GameEntity * entity) const {
    auto it = entities.find(const_cast < GameEntity * > (entity));
    if (it != entities.end()) {
      return it -> second;
    } else {
      // Return an empty vector if the entity is not found
      static std::vector < GameEntity * > emptyVector;
      return emptyVector;
    }
  }

  const vector < Area * > & getAreas() const {
    return areas;
  }

  private: vector < Area * > areas;
  unordered_map < GameEntity * ,
  vector < GameEntity * >> entities;
};

class Game {
  public: Game(Graph & gameGraph, Area * startingArea): player(nullptr),
  gameGraph(gameGraph),
  currentArea(startingArea) {}

  void start() {
    cout << "Welcome to Xeno RPG!" << endl;

    // Create player and get player's name
    string playerName;
    cout << "What is your name? ";
    cin >> playerName;

    player = new Player(playerName, 100);
    player -> addConnection(currentArea);

    cout << "Hello, " << playerName << "! Let's start the game." << endl;

    while (!player -> isDefeated()) {
      // Display information about the current area
      cout << "Current Area: " << currentArea -> getName() << endl;
      currentArea -> displayInfo();

      // Display available connections
      gameGraph.displayConnections();

      // Display entities in the current area
      displayEntities();

      // Player's turn
      playerTurn();

      // Check if player defeated all enemies
      if (allEnemiesDefeated()) {
        break;
      }

      // Choose the next area to explore
      currentArea = getNextArea();

      cout << endl;
    }

    endGame();
  }

  ~Game() {
    delete player;
  }

  private: Player * player;
  Graph & gameGraph;
  Area * currentArea;

  void displayEntities() const {
    const vector < GameEntity * > & entities = gameGraph.getEntities(currentArea);

    cout << "Entities in this area:" << endl;
    for (const GameEntity * entity: entities) {
      entity -> displayInfo();
    }
    cout << endl;
  }

  void playerTurn() {
    bool allEnemiesDefeated = false;

    do {
      cout << "Choose your action:" << endl;
      if (currentArea -> getName() != "Hometown") {
        cout << "1. Attack" << endl;
        cout << "2. Defend" << endl;
        cout << "3. Use Health Potion" << endl;
      }
      cout << "4. Move to a different area" << endl;

      int choice;
      cout << "Enter your choice (1-4): ";
      cin >> choice;

      if (currentArea -> getName() == "Hometown" && choice == 1) {
        cout << "Invalid choice. You can only move to a different area." << endl;
      } else if (choice == 1 && currentArea -> getName() != "Hometown") {
        // Player attacks each enemy in the current area
        const vector < GameEntity * > & entities = gameGraph.getEntities(currentArea);
        allEnemiesDefeated = true; // Asumsikan semua musuh telah dikalahkan, kecuali ditemukan musuh yang belum dikalahkan
        for (GameEntity * entity: entities) {
          NonPlayerCharacter * npc = dynamic_cast < NonPlayerCharacter * > (entity);
          if (npc && !npc -> isDefeated()) {
            if (rand() % 2 == 0) {
              int damage = player -> attack();
              cout << player -> getName() << " attacks " << entity -> getName() << " with damage: " << damage << endl;
              entity -> takeDamage(damage);
              cout << entity -> getName() << "'s health: " << npc -> getHealth() << endl;
            } else {
              int bonusDamage = (rand() % 2 == 0) ? rand() % 6 + 5 : 0; // 50% chance for bonus damage
              int damage = player -> attack(bonusDamage);
              cout << player -> getName() << " attacks " << entity -> getName() << " with damage: " << damage + bonusDamage << endl;
              entity -> takeDamage(damage);
              cout << entity -> getName() << "'s health: " << npc -> getHealth() << endl;
            }
            if (entity -> isDefeated()) {
              cout << entity -> getName() << " is defeated!" << endl;
            }
            // Periksa apakah masih ada musuh yang belum dikalahkan
            if (!entity -> isDefeated()) {
              allEnemiesDefeated = false;
            }
          }
        }
        // Enemy Turn
        npcTurn();

        // Cek apakah kesehatan pemain mencapai 0
        if (player -> isDefeated()) {
          endGame();
          return;
        }
      } else if (choice == 2 && currentArea -> getName() != "Hometown") {
        player -> defend();
        cout << player -> getName() << " is defending." << endl;
      } else if (choice == 3 && currentArea -> getName() != "Hometown") {
        // Opsi untuk menggunakan health potion
        player -> useHealthPotion();
      } else if (choice == 4) {
        break; // Keluar dari loop dan pindah ke area berikutnya setelah pemain menyelesaikan tindakannya
      } else {
        cout << "Invalid choice. Player loses turn." << endl;
      }
    } while (!allEnemiesDefeated); // Ulangi tindakan pemain jika masih ada musuh yang belum dikalahkan
  }

  void npcTurn() {
    // NPCs attack the player
    const vector < GameEntity * > & entities = gameGraph.getEntities(currentArea);
    for (GameEntity * entity: entities) {
      if (!entity -> isDefeated()) {
        int damage = entity -> attack();
        cout << entity -> getName() << " attacks " << player -> getName() << " with damage: " << damage << endl;
        player -> takeDamage(damage);
        cout << player -> getName() << "'s health: " << player -> getHealth() << endl;
      }
    }
  }

  Area * getNextArea() {
    cout << "Choose the next area to explore:" << endl;
    currentArea -> displayConnections(); // Display connections for the current area

    string chosenArea;
    cout << "Enter the name of the area ('exit' to end the game): ";
    cin >> chosenArea;

    if (chosenArea == "exit") {
      // Player chooses to exit the game
      endGame();
      return nullptr;
    }

    const vector < GameEntity * > & connections = currentArea -> getConnections();
    bool moveSuccessful = false;

    for (const GameEntity * entity: connections) {
      const Area * connectedArea = dynamic_cast <
        const Area * > (entity);
      if (connectedArea && connectedArea -> getName() == chosenArea) {
        currentArea = const_cast < Area * > (connectedArea); // Move to the new area
        moveSuccessful = true;
        break;
      }
    }

    if (!moveSuccessful) {
      cout << "Invalid choice. Moving to a random connected area." << endl;

      // Select a random connected area
      const vector < Area * > & connectedAreas = gameGraph.getAreas();
      currentArea = connectedAreas[rand() % connectedAreas.size()];
    }

    return currentArea;

  }

  bool allEnemiesDefeated() const {
    const vector < GameEntity * > & entities = gameGraph.getEntities(currentArea);

    for (const GameEntity * entity: entities) {
      const NonPlayerCharacter * npc = dynamic_cast <
        const NonPlayerCharacter * > (entity);

      if (npc && !npc -> isDefeated()) {
        return false; // Ada setidaknya satu musuh yang masih belum dikalahkan
      }
    }

    // Cek semua area untuk melihat apakah ada musuh yang belum dikalahkan
    for (const Area * area: gameGraph.getAreas()) {
      const vector < GameEntity * > & areaEntities = gameGraph.getEntities(area);
      for (const GameEntity * entity: areaEntities) {
        const NonPlayerCharacter * npc = dynamic_cast <
          const NonPlayerCharacter * > (entity);

        if (npc && !npc -> isDefeated()) {
          return false; // Ada setidaknya satu musuh yang masih belum dikalahkan di area lain
        }
      }
    }

    return true; // Semua musuh sudah dikalahkan
  }

  void endGame() {
    cout << "Game Over!" << endl;
    if (player -> isDefeated()) {
      cout << "You have been defeated. Better luck next time!" << endl;
    } else {
      cout << "Congratulations! You have defeated all enemies and won the game!" << endl;
    }
  }
};

int main() {
  srand(time(0));

  // Create areas
  Area * hometown = new Area("Hometown"); // Added hometown
  Area * barren = new Area("Barren");
  Area * sea = new Area("Sea");
  Area * volcano = new Area("Volcano");

  // Create enemies and player in each area
  NonPlayerCharacter * scorpionKing = new NonPlayerCharacter("Scorpion King", "Boss", 80);
  NonPlayerCharacter * kraken = new NonPlayerCharacter("Kraken", "Sea Monster", 60);
  NonPlayerCharacter * fireDragon = new NonPlayerCharacter("Fire Dragon", "Dragon", 120);

  // Create graph and add areas
  Graph gameGraph;
  gameGraph.addArea(hometown); // Added hometown
  gameGraph.addArea(barren);
  gameGraph.addArea(sea);
  gameGraph.addArea(volcano);

  // Connect entities within the graph
  gameGraph.connectEntities(hometown, nullptr); // Hometown has no enemies, so connect it with nullptr
  gameGraph.connectEntities(barren, scorpionKing);
  gameGraph.connectEntities(sea, kraken);
  gameGraph.connectEntities(volcano, fireDragon);

  // Connect areas in the graph
  gameGraph.connectAreas(hometown, barren);
  gameGraph.connectAreas(hometown, sea);

  gameGraph.connectAreas(barren, hometown);
  gameGraph.connectAreas(barren, sea);
  gameGraph.connectAreas(barren, volcano);

  gameGraph.connectAreas(sea, hometown);
  gameGraph.connectAreas(sea, barren);
  gameGraph.connectAreas(sea, volcano);

  gameGraph.connectAreas(volcano, barren);
  gameGraph.connectAreas(volcano, sea);

  // Start the game
  Game game(gameGraph, hometown); // Set hometown as the starting area
  game.start();

  // Clean up memory
  delete hometown;
  delete barren;
  delete sea;
  delete volcano;
  delete scorpionKing;
  delete kraken;
  delete fireDragon;

  return 0;
}