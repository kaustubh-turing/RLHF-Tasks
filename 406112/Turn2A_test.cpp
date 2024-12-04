#include <iostream>
#include <vector>
#include <memory>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update() = 0;
};

class GameState : public std::enable_shared_from_this<GameState> {
    using ObserverPtr = std::shared_ptr<IObserver>;

private:
    std::vector<ObserverPtr> observers;
    int gameState = 0;
    int playerScore = 0;
    
    GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

public:
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

    void UpdateScore(int points) {
        playerScore += points;
        NotifyAllObservers();
    }

    int GetGameState() const { return gameState; }
    int GetScore() const { return playerScore; }
};

class Player : public IObserver, public std::enable_shared_from_this<Player> {
public:
    static std::shared_ptr<Player> Create() {
        auto player = std::shared_ptr<Player>(new Player());
        GameState::GetInstance()->Attach(player);
        return player;
    }

    void Update() override {
        std::cout << "Player: Game state: " << GameState::GetInstance()->GetGameState() 
                  << ", Score: " << GameState::GetInstance()->GetScore() << std::endl;
    }

private:
    Player() = default;
};

class Scoreboard : public IObserver, public std::enable_shared_from_this<Scoreboard> {
public:
    static std::shared_ptr<Scoreboard> Create() {
        auto scoreboard = std::shared_ptr<Scoreboard>(new Scoreboard());
        GameState::GetInstance()->Attach(scoreboard);
        return scoreboard;
    }

    void Update() override {
        std::cout << "Scoreboard: Current Score: " << GameState::GetInstance()->GetScore() << std::endl;
    }

private:
    Scoreboard() = default;
};

int main() {
    auto player = Player::Create();
    auto scoreboard = Scoreboard::Create();

    GameState::GetInstance()->SetGameState(1);  // Game started
    GameState::GetInstance()->UpdateScore(100); // Player scored 100 points
    GameState::GetInstance()->UpdateScore(50);  // Player scored 50 more points
    GameState::GetInstance()->SetGameState(2);  // Game paused

    return 0;
} 