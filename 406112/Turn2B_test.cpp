#include <iostream>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void updateGameState() = 0;
};

class GameStateSingleton {
private:
    GameStateSingleton() = default;
    ~GameStateSingleton() = default;

    std::string m_gameState = "Initial";
    int m_playerScore = 0;
    std::vector<Observer*> m_observers;

public:
    static GameStateSingleton& getInstance() {
        static GameStateSingleton instance;
        return instance;
    }

    void attachObserver(Observer* observer) {
        m_observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        auto it = std::find(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end()) {
            m_observers.erase(it);
        }
    }

    void notifyObservers() {
        for (auto observer : m_observers) {
            observer->updateGameState();
        }
    }

    void setGameState(const std::string& newState) {
        m_gameState = newState;
        notifyObservers();
    }

    void updateScore(int points) {
        m_playerScore += points;
        notifyObservers();
    }

    const std::string& getGameState() const { return m_gameState; }
    int getScore() const { return m_playerScore; }
};

class Player : public Observer {
public:
    Player() {
        GameStateSingleton::getInstance().attachObserver(this);
    }

    ~Player() {
        GameStateSingleton::getInstance().removeObserver(this);
    }

    void updateGameState() override {
        std::cout << "Player: State: " << GameStateSingleton::getInstance().getGameState() 
                  << ", Score: " << GameStateSingleton::getInstance().getScore() << std::endl;
    }
};

class ScoreDisplay : public Observer {
public:
    ScoreDisplay() {
        GameStateSingleton::getInstance().attachObserver(this);
    }

    ~ScoreDisplay() {
        GameStateSingleton::getInstance().removeObserver(this);
    }

    void updateGameState() override {
        std::cout << "Score Display: " << GameStateSingleton::getInstance().getScore() << std::endl;
    }
};

int main() {
    Player player;
    ScoreDisplay scoreDisplay;

    GameStateSingleton::getInstance().setGameState("Playing");
    GameStateSingleton::getInstance().updateScore(100);
    GameStateSingleton::getInstance().updateScore(50);
    GameStateSingleton::getInstance().setGameState("Paused");

    return 0;
} 