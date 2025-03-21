#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
int StainUpdate(std::vector<std::vector<bool>>& oilOld, std::vector<std::vector<int>>& oilNew) {
    int size = oilOld.size();   // Za�o�enie �e tablica ma kszta�t kwadratu
    //std::vector<std::vector<bool>> oilNew(size);
    int rnx, rny;       // Prawdziwe s�siedztwa
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (oilOld[x][y]) {
                for (int nx = -1; nx < 2; nx++) {
                    for (int ny = -1; ny < 2; ny++) {
                        rnx = nx;
                        rny = ny;
                        if (x == 0 && nx == -1)rnx = size - 1;
                        if (x == size - 1 && nx == 1)rnx = -(size-1);
                        if (y == 0 && ny == -1)rny = size - 1;
                        if (y == size - 1 && ny == 1)rny = -(size - 1);
                        //std::cout << "rnx: " << rnx<< " rny: " << rny << std::endl;
                        oilNew[x + rnx][y + rny] += 1;
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
            else { 
                oilOld[x][y] = true; 
                d += 1;
            }
            oilNew[x][y] = 0;
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
            //int seed = (x - y) ^ 2 - std::abs(y - x);
            if ((std::rand() % 1001) < 500) {
                smear.push_back(true);
                density[0] += 1;
            }
            else smear.push_back(false);
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
                if (event.key.code == sf::Keyboard::I) {    // Wypisz g�sto�ci w czasie
                    for (int i = 0; i < density.size(); i++) {
                        std::cout << ", " << density[i];
                    }
                    std::cout<<std::endl;
                }
                if (event.key.code == sf::Keyboard::K) {    // Ilo�� iteracji
                    std::cout << "Krok nr: " << density.size() << std::endl;
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
                    //shape.setFillColor(sf::Color::Green);
                    shape.setPosition(sf::Vector2f(blocksize * x, blocksize * y));
                    window.draw(shape);
                }
                /*else {
                    shape.setFillColor(sf::Color::Blue);
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