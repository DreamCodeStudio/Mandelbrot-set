#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include <SFML/Graphics.hpp>

/* ##################################
*   Bereich -2 < x < 1
*           -1 < y < 1
*   1Le = 100
*
*  ################################# */

struct complexNumber
{
    float rePart;
    float imPart;

    std::string complexZ;
};

int** Create();
complexNumber Calculate(complexNumber z, complexNumber point, int counter);
float ComplexAbs(complexNumber z);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mandelbrotmenge");

    //Create the mandelbrot set and an image to render it
    std::cout << "Creating..." << std::endl;
    int **matrix = Create();
    std::cout << "Finished!" << std::endl;
    
    sf::Image image;
    image.create(3000, 2000);
    for (unsigned int y = 0; y < 2000; y++)
    {
        for (unsigned int x = 0; x < 3000; x++)
        {
            if (matrix[x][y] == 1)
            {
                image.setPixel(x, y, sf::Color(0, 0, 0));
            }
            else
            {
                image.setPixel(x, y, sf::Color(255, 255, 255));
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(0.5f, 0.5f));

    while(window.isOpen())
    {
        window.clear();

        //Draw everything here
        window.draw(sprite);

        window.display();

        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                return 1;
            }
        }
    }
}

int** Create()
{
    //Create matrix 
    int **matrix;
    matrix = new int*[3000];
    for (unsigned int i = 0; i < 3000; i++)
    {
        matrix[i] = new int[2000];
    }

    //Iterate through the whoer matrix
    for (int y = -1000; y < 1000; y++)
    {
        for (int x = -2000; x < 1000; x++)
        {
            //Create a point er z0s in the mandelbrot set
            complexNumber point;
            point.rePart = static_cast<float>(x / 1000.0f);
            point.imPart = static_cast<float>(y / 1000.0f);

            complexNumber null;
            null.rePart = 0;
            null.imPart = 0;

            complexNumber result = Calculate(null, point, 0);
            float resultAbs = ComplexAbs(result);
            
            if (resultAbs < 2.0)
            {
                matrix[x + 2000][y + 1000] = 1;
            }
            else
            {
                matrix[x + 2000][y + 1000] = 0;
            }
        }
    }  

    return matrix;
}

complexNumber Calculate(complexNumber z, complexNumber point, int counter)
{
    complexNumber result;

    //Sqare the complexNumber
    result.rePart = pow(z.rePart, 2) - pow(z.imPart, 2);
    result.imPart = 2 * z.rePart * z.imPart;
    
    //Now add the point to the result
    result.rePart = result.rePart + point.rePart;
    result.imPart = result.imPart + point.imPart;

    if (counter == 20)
    {
        return result;
    }

    return Calculate(result, point, counter + 1);
}

float ComplexAbs(complexNumber z)
{
    return sqrt(pow(z.rePart, 2) + pow(z.imPart, 2));
}