#include <iostream>
#include <vector>
#include <algorithm>

// Observer interface
class Observer {
public:
    virtual ~Observer() = default;
    virtual void updateGameState() = 0;
};

// GameStateSingleton class using Singleton pattern
class GameStateSingleton {
private:
    GameStateSingleton() = default;  // Private constructor
    ~GameStateSingleton() = default;

    std::string m_gameState = "Initial";
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

    const std::string& getGameState() const {
        return m_gameState;
    }
};

// Example Player class implementing Observer
class Player : public Observer {
public:
    Player() {
        GameStateSingleton::getInstance().attachObserver(this);
    }

    ~Player() {
        GameStateSingleton::getInstance().removeObserver(this);
    }

    void updateGameState() override {
        std::cout << "Player: Game state updated to: " 
                  << GameStateSingleton::getInstance().getGameState() << std::endl;
    }
};

int main() {
    Player player;

    GameStateSingleton::getInstance().setGameState("Playing");
    GameStateSingleton::getInstance().setGameState("Paused");
    GameStateSingleton::getInstance().setGameState("Won");

    return 0;
} 