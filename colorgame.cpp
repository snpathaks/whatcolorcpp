#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>  
#include <ctime>    
#include <limits>
using namespace std;

class ColorGame {
private:
    struct Player {
        string name;
        int score;
        string currentColor;
        
        Player() : score(0) {}  
    };

    struct Category {
        string name;
        map<std::string, std::vector<std::string> > colorToItems;  
    };

    vector<Player> players;
    vector<Category> categories;
    vector<std::string> colors;

    
    const string RESET;
    const string RED;
    const string GREEN;
    const string YELLOW;
    const string BLUE;

    map<string, string> colorMap;

public:
    ColorGame() : 
        RESET("\033[0m"), RED("\033[31m"), GREEN("\033[32m"), YELLOW("\033[33m"), BLUE("\033[34m")
    {
        // Initialize colors
        colors.push_back("Red");
        colors.push_back("Blue");
        colors.push_back("Green");
        colors.push_back("Yellow");

        
        colorMap["Red"] = RED;
        colorMap["Blue"] = BLUE;
        colorMap["Green"] = GREEN;
        colorMap["Yellow"] = YELLOW;

        
        srand(static_cast<unsigned int>(time(NULL)));
        
        initializeCategories();
    }

    std::string getColorCode(const std::string& colorName) {
        if (colorMap.find(colorName) != colorMap.end()) {
            return colorMap[colorName];
        }
        return RESET;
    }

    void initializeCategories() {
        
        Category fruits;
        fruits.name = "Fruits";
        fruits.colorToItems["Red"].push_back("Apple");
        fruits.colorToItems["Red"].push_back("Strawberry");
        fruits.colorToItems["Blue"].push_back("Blueberry");
        fruits.colorToItems["Green"].push_back("Kiwi");
        fruits.colorToItems["Yellow"].push_back("Banana");
        categories.push_back(fruits);
    }

    void clearScreen() {
        system("cls");  
    }

    void setupPlayers() {
        players.clear();
        
        std::cout << "\nEnter name for Player 1: ";
        Player p1;
        std::cin >> p1.name;
        players.push_back(p1);

        std::cout << "Enter name for Player 2: ";
        Player p2;
        std::cin >> p2.name;
        players.push_back(p2);

        clearScreen();
        std::cout << GREEN << "\nWelcome " << p1.name << " and " << p2.name << " to 'What Color Do You Choose?'!\n\n" << RESET;
    }

    void displayRules() {
        clearScreen();
        std::cout << YELLOW << "====================================\n";
        std::cout << "   WHAT COLOR DO YOU CHOOSE?\n";
        std::cout << "====================================\n\n" << RESET;
        std::cout << "Rules:\n1. Choose a color.\n2. Name an item in the category matching the color.\n3. Earn points for correct answers.\n4. 3 rounds.\n";
        std::cout << GREEN << "Let's begin!\n" << RESET;
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void displayColors() {
        std::cout << "Available colors: \n";
        for (size_t i = 0; i < colors.size(); ++i) {
            std::cout << getColorCode(colors[i]) << (i + 1) << ") " << colors[i] << RESET << " ";
        }
        std::cout << std::endl;
    }

    int getColorChoice(const std::string& playerName) {
        int choice;
        while (true) {
            std::cout << playerName << ", choose a color (1-" << colors.size() << "): ";
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << RED << "Invalid input. Enter a number.\n" << RESET;
            } 
            else if (choice < 1 || choice > static_cast<int>(colors.size())) {
                std::cout << RED << "Choose between 1 and " << colors.size() << ".\n" << RESET;
            } 
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return choice - 1;
            }
        }
    }

    Category getRandomCategory() {
        int index = rand() % categories.size();
        return categories[index];
    }

    bool validateAnswer(const std::string& answer, const std::vector<std::string>& validItems) {
        for (size_t i = 0; i < validItems.size(); ++i) {
            if (toLower(validItems[i]) == toLower(answer)) {
                return true;
            }
        }
        return false;
    }

    std::string toLower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);  
        return str;
    }

    void playRound(int roundNum, int totalRounds) {
        clearScreen();
        std::cout << YELLOW << "\n===================\n";
        std::cout << "   ROUND " << roundNum << " of " << totalRounds << "\n";
        std::cout << "===================\n\n" << RESET;

        Category category = getRandomCategory();
        std::cout << BLUE << "Category: " << category.name << "\n" << RESET;

        for (size_t i = 0; i < players.size(); ++i) {
            std::cout << GREEN << players[i].name << "'s turn:\n" << RESET;
            displayColors();
            int colorIndex = getColorChoice(players[i].name);
            players[i].currentColor = colors[colorIndex];
            
            std::cout << getColorCode(players[i].currentColor) << players[i].name << " chose: " << players[i].currentColor << RESET << "\n";
            
            const std::vector<std::string>& items = category.colorToItems[players[i].currentColor];
            if (items.empty()) {
                std::cout << RED << "No " << players[i].currentColor << " " << category.name << " available.\nNo point this round.\n" << RESET;
                continue;
            }
            
            std::cout << "Name a " << players[i].currentColor << " " << category.name.substr(0, category.name.size() - 1) << ": ";
            std::string answer;
            std::getline(std::cin, answer);
            
            if (validateAnswer(answer, items)) {
                std::cout << GREEN << "Correct! +1 point for " << players[i].name << "!\n" << RESET;
                players[i].score++;
            } else {
                std::cout << RED << "Wrong or not in list. No point.\n" << RESET;
            }
            std::cout << "\n";
        }

        std::cout << YELLOW << "Scores:\n" << RESET;
        for (size_t i = 0; i < players.size(); ++i) {
            std::cout << players[i].name << ": " << players[i].score << "\n";
        }
        std::cout << "Press Enter for next round...";
        std::cin.get();
    }

    void displayFinalResults() {
        clearScreen();
        std::cout << YELLOW << "\n===================\n";
        std::cout << "   FINAL RESULTS\n";
        std::cout << "===================\n\n" << RESET;
        
        for (size_t i = 0; i < players.size(); ++i) {
            std::cout << players[i].name << ": " << GREEN << players[i].score << " points\n" << RESET;
        }
        
        if (players[0].score > players[1].score) {
            std::cout << GREEN << "Winner: " << players[0].name << "!\n" << RESET;
        } else if (players[1].score > players[0].score) {
            std::cout << GREEN << "Winner: " << players[1].name << "!\n" << RESET;
        } else {
            std::cout << BLUE << "It's a tie!\n" << RESET;
        }
    }

    void playGame() {
        displayRules();
        setupPlayers();
        
        const int totalRounds = 3;  
        for (int round = 1; round <= totalRounds; ++round) {
            playRound(round, totalRounds);
        }
        
        displayFinalResults();
    }
};

int main() {
    std::cout << "\033[1;33m" << "======================\n";
    std::cout << " COLOR GAME\n";
    std::cout << "======================\n" << "\033[0m";
    
    ColorGame game;
    game.playGame();
    
    return 0;
}
