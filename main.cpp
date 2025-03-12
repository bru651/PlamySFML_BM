#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
int StainUpdate(std::vector<std::vector<bool>>& oilOld, std::vector<std::vector<int>>& oilNew) {
    int size = oilOld.size();   // Za³o¿enie ¿e tablica ma kszta³t kwadratu
    //std::vector<std::vector<bool>> oilNew(size);
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (x==0||y==0||x==size-1||y==size-1) oilNew[x][y] = -9;    // Puste krañce
            else if (oilOld[x][y]) {
                for (int nx = -1; nx < 2; nx++) {
                    for (int ny = -1; ny < 2; ny++) {
                        oilNew[x + nx][y + ny] += 1;
                    }
                }
            }
        }
        //oilNew[x] = smear;
    }
    int n;
    int d = 0;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            n = oilNew[x][y];
            if (n<6&&n!=4) oilOld[x][y]=false;
            else { oilOld[x][y] = true; d += 1; }
        }
    }
    return d;
}
int main()
{
    std::srand(std::time({}));
    bool pause = true;
    int size = 200;
    float blocksize = 4.f;
    std::vector<int> density(1,0);
    std::vector<std::vector<bool>> oil;
    for (int x = 0; x < size; x++) {
        std::vector<bool> smear;
        for (int y = 0; y < size; y++) {
            if (x == 0 || y == 0 || x == size - 1 || y == size - 1) smear.push_back(false);    // Puste krañce
            else {
                int seed = (x - y) ^ 2;
                if ((std::rand() % 601) - (seed) < 250) { smear.push_back(true); density[0] += 1;}
                else smear.push_back(false);
            }
        }
        oil.push_back(smear);
    }
    std::vector<std::vector<int>> oilNew(size,std::vector<int>(size,0));
    sf::RenderWindow window(sf::VideoMode(800, 800), "Plamy");
    window.setFramerateLimit(30);
    sf::RectangleShape shape(sf::Vector2f(blocksize, blocksize));
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased) {    // Keyboard input
                if (event.key.code == sf::Keyboard::Space) {    // Pause button (Space)
                    pause = !pause;
                }
                if (event.key.code == sf::Keyboard::Escape) {    // Close program
                    window.close();
                }
                if (event.key.code == sf::Keyboard::K) {    // Wypisz gêstoœci w czasie
                    for (int i = 0; i < density.size(); i++) {
                        std::cout << ", " << density[i];
                    }
                }
            }
        }
        if (!pause) {
            density.push_back(StainUpdate(oil,oilNew));
        }
        window.clear();
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                if (oil[x][y]) {
                    //std::cout << "X: "<< x<<" Y: "<<y << std::endl;
                    shape.setPosition(sf::Vector2f(blocksize * x, blocksize * y));
                    window.draw(shape);
                }//*/
                //else std::cout << "Nay" << std::endl;
            }
        }
        
        window.display();
    }

    return 0;
}