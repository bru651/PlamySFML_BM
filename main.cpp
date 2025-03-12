#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
void StainUpdate(std::vector<std::vector<bool>>& oilOld, std::vector<std::vector<bool>> oilNew) {
    int size = oilOld.size();   // Za³o¿enie ¿e tablica ma kszta³t kwadratu
    //std::vector<std::vector<bool>> oilNew(size);
    int neighbours;
    for (int x = 0; x < size; x++) {
        std::vector<bool> smear(size);
        for (int y = 0; y < size; y++) {
            if (x==0||y==0||x==size-1||y==size-1)smear[y] = false;    // Puste krañce
            else {
                neighbours = 0;
                for (int nx = -1; nx < 2; nx++) {
                    for (int ny = -1; ny < 2; ny++) {
                        if (oilOld[x+nx][y+ny]) neighbours += 1;
                    }
                }
                //std::cout << "X: " << x << " Y: " << y <<" N: " << neighbours << std::endl;
                if (neighbours<6&&neighbours!=4)smear[y] = false;
                else smear[y] = true;
            }
        }
        oilNew[x] = smear;
    }
    oilOld = oilNew;
}
int main()
{
    std::srand(std::time({}));
    bool pause = true;
    int size = 160;
    float blocksize = 5.f;
    std::vector<std::vector<bool>> oil;
    for (int x = 0; x < size; x++) {
        std::vector<bool> smear;
        for (int y = 0; y < size; y++) {
            int seed = x+y;
            if ((std::rand()%501)-x<175)smear.push_back(true);
            else smear.push_back(false);
        }
        oil.push_back(smear);
    }
    std::vector<std::vector<bool>> oilNew(size);
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
                    //std::cout << "Input: Space" << std::endl;
                    pause = !pause;
                    //StainUpdate(oil);
                }
                if (event.key.code == sf::Keyboard::Escape) {    // Close program
                    window.close();
                }
            }
        }
        if (!pause) {
            StainUpdate(oil,oilNew);
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