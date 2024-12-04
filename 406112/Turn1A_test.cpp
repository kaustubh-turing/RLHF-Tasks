#include <iostream>
#include <vector>
#include <memory>

// Observer interface
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update() = 0;
};

// Subject class (Singleton) that manages game state
class GameState : public std::enable_shared_from_this<GameState> {
    using ObserverPtr = std::shared_ptr<IObserver>;

private:
    std::vector<ObserverPtr> observers;
    int gameState = 0;
    
    // Private constructor
    GameState() = default;
    
    // Delete copy constructor and assignment
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

public:
    // Public destructor
    ~GameState() = default;

    static std::shared_ptr<GameState> GetInstance() {
        static std::shared_ptr<GameState> instance{new GameState()};
        return instance;
    }

    void Attach(ObserverPtr observer) {
        observers.push_back(observer);
    }

    void NotifyAllObservers() {
        for (const ObserverPtr& observer : observers) {
            observer->Update();
        }
    }

    void SetGameState(int newState) {
        gameState = newState;
        NotifyAllObservers();
    }

    int GetGameState() const {
        return gameState;
    }
};

// Concrete Observer classes
class Player : public IObserver, public std::enable_shared_from_this<Player> {
public:
    static std::shared_ptr<Player> Create() {
        auto player = std::shared_ptr<Player>(new Player());
        GameState::GetInstance()->Attach(player);
        return player;
    }

    void Update() override {
        std::cout << "Player: Game state changed to " << GameState::GetInstance()->GetGameState() << std::endl;
    }

private:
    Player() = default;
};

class HUD : public IObserver, public std::enable_shared_from_this<HUD> {
public:
    static std::shared_ptr<HUD> Create() {
        auto hud = std::shared_ptr<HUD>(new HUD());
        GameState::GetInstance()->Attach(hud);
        return hud;
    }

    void Update() override {
        std::cout << "HUD: Updating display based on game state " << GameState::GetInstance()->GetGameState() << std::endl;
    }

private:
    HUD() = default;
};

int main() {
    auto player = Player::Create();
    auto hud = HUD::Create();

    GameState::GetInstance()->SetGameState(1); // Game started
    GameState::GetInstance()->SetGameState(2); // Game paused
    GameState::GetInstance()->SetGameState(3); // Game level completed

    return 0;
} 