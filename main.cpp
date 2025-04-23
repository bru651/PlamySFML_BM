#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
//#include <cstdlib>
//#include <cmath>
#include <ctime>
#include <iostream>
int StainUpdate(std::vector<std::vector<bool>>& oilOld, std::vector<std::vector<int>>& oilNew) {
    int size = oilOld.size();   // Za³o¿enie ¿e tablica ma kszta³t kwadratu
    int rnx, rny;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (oilOld[x][y]) {
                for (int nx = -1; nx < 2; nx++) {
                    for (int ny = -1; ny < 2; ny++) {
                        rnx = x + nx;
                        rny = y + ny;
                        //if (rnx  < 0 || rny < 0 || rnx > size - 1 || rny > size - 1) continue;    // Krañce
                        

                        //
                        if (rnx < 0)rnx = size - 1;
                        if (rnx > size - 1)rnx = 0;
                        if (rny < 0)rny = size - 1;
                        if (rny > size - 1)rny = 0;
                        //std::cout << "rnx: " << rnx<< " rny: " << rny << std::endl;
                        if (!(nx == 0 && ny == 0)) oilNew[rnx][rny] += 1;
                        //
                    }
                }
            }
        }
    }
    int n;
    int d = 0;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            n = oilNew[x][y];
            if (n==3) oilOld[x][y]=true;
            else if (n!=2) { 
                oilOld[x][y] = false; 
            }
            if (oilOld[x][y] == true) d+=1;
            oilNew[x][y] = 0;
        }
    }
    return d;
}

int generateRandom(std::vector<std::vector<bool>>& oil, int size, int probability) {
    int d = 0;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if ((std::rand() % 1001) < probability) {
                oil[x][y] = true;
                d += 1;
            }
            else oil[x][y] = false;
        }
    }
    return d;
}

int main()
{
    std::srand(std::time({}));
    // Ustawienia
    int displaySize = 800;
    bool pause = true;
    int iterations = 100;     // Iloœæ iteracji
    int curIteration = 1;     // Obecna iteracja
    int probability = 500;  // Od 0 do 1000
    bool display = false; // Czy ma rysowaæ gry
    int size = 200;     // Wymiar tablicy
    int space = size * size;
    float blocksize = static_cast<float>(displaySize) / size;  // Graficzna wielkoœæ pola

    // Zbiorniki
    std::vector<int> density;
    std::vector<std::vector<bool>> oil(size, std::vector<bool>(size, false));
    std::vector<std::vector<int>> oilNew(size,std::vector<int>(size,0));
    std::vector<int> densityFinal;

    // Grafika
    sf::RenderWindow window(sf::VideoMode(displaySize, displaySize), "Plamy");
    //window.setFramerateLimit(30);
    sf::RectangleShape shape(sf::Vector2f(blocksize, blocksize));
    shape.setFillColor(sf::Color::Green);

    density.push_back(generateRandom(oil, size, probability));
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
                if (event.key.code == sf::Keyboard::I) {    // Wypisz gêstoœci w czasie
                    for (int i = 0; i < density.size(); i++) {
                        std::cout << ", " << density[i];
                    }
                    std::cout << std::endl;
                }
                if (event.key.code == sf::Keyboard::K) {    // Iloœæ iteracji
                    std::cout << "Krok nr: " << density.size() << std::endl;
                }
                if (event.key.code == sf::Keyboard::R) {    // Iloœæ iteracji
                    display = !display;
                }
            }
        }
        if (!pause) {
            density.push_back(StainUpdate(oil, oilNew));
        }
        if (density.size() == 1000) { 
            if (iterations<2) {
                pause = true;
                /*std::cout << "Krok nr: " << density.size() << std::endl;
                for (int i = 0; i < density.size(); i++) {
                    std::cout << ", " << density[i];
                }
                std::cout << std::endl;
                window.close();//*/
            }
            else {  // Powtarzanie symulacji
                densityFinal.push_back(density[density.size() - 1]);
                density.clear();
                std::cout << "Iteracja: " << curIteration << std::endl;
                if (iterations>curIteration) {
                    density.push_back(generateRandom(oil, size, probability));
                    curIteration += 1;
                }
                else {
                    // Policz œredni¹
                    int suma = 0;
                    for (int i=0; i < densityFinal.size(); i++) {
                        suma += densityFinal[i];
                    }
                    float mean = static_cast<float>(suma)/ densityFinal.size();
                    float meanDensity = mean / space;
                    // Policz odchylenie standardowe
                    float devSum = 0;
                    float i_dens;
                    for (int i = 0; i < densityFinal.size(); i++) {
                        i_dens = static_cast<float>(densityFinal[i]) / space;
                        devSum += std::pow(i_dens - meanDensity,2);
                    }
                    float standardDev = std::sqrt(devSum/(densityFinal.size()-1));
                    // Policz b³¹d standardowy
                    float standardError = devSum/ std::sqrt(densityFinal.size());
                    // Napisz raport
                    std::cout << "Rozmiar gry: " << size << std::endl;
                    std::cout << "Œrednia: " << meanDensity << std::endl;
                    std::cout << "Odchylenie standardowe: " << standardDev << std::endl;
                    std::cout << "B³¹d standardowy: " << standardError << std::endl;

                    window.close();
                }
            }
        }

        if (display) {
            window.clear();
            for (int x = 0; x < size; x++) {
                for (int y = 0; y < size; y++) {
                    if (oil[x][y]) {
                        //std::cout << "X: "<< x<<" Y: "<<y << std::endl;
                        shape.setPosition(sf::Vector2f(blocksize * x, blocksize * y));
                        window.draw(shape);
                    }
                    //else std::cout << "Nay" << std::endl;
                }
            }

            window.display();
        }//*/
    }

    return 0;
}