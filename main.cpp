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
complexNumber Calculate(complexNumber z, complexNumber point, int counter, int *iterationsTillAbort);
float ComplexAbs(complexNumber z);

sf::Image CreateImage(int **matrix);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mandelbrotmenge");

    //Create the mandelbrot set and an image to render it
    int **matrix = Create();
    
    sf::Image image = CreateImage(matrix);    

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(0.54f, 0.54f));
    sprite.setPosition(150, 0);

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

            int *iterationsTillAbort = new int; //If a number is NOT in the mandelbrot set, the amount of iteration it has taken to find this out is stored here 

            float resultAbs = ComplexAbs(Calculate(null, point, 0, iterationsTillAbort)); //If the absolute value after 20 iterations is smaller than 2, the number is in the mandelbrot set         
            if (resultAbs < 2.0)
            {
                matrix[x + 2000][y + 1000] = 0;
            }
            else                                                                        //Otherwise it's not
            {
                //If the number is not in the mandelbrot set look how many iterations we had to do until the number went to high
                matrix[x + 2000][y + 1000] = *iterationsTillAbort + 1;
            }
        }
    }  

    return matrix;
}

complexNumber Calculate(complexNumber z, complexNumber point, int counter, int* iterationsTillAbort)
{
    complexNumber result;

    //Sqare the complexNumber
    result.rePart = pow(z.rePart, 2) - pow(z.imPart, 2);
    result.imPart = 2 * z.rePart * z.imPart;
    
    //Now add the point to the result
    result.rePart = result.rePart + point.rePart;
    result.imPart = result.imPart + point.imPart;

    if (counter == 20 || ComplexAbs(result) >= 2.0)
    {
        if (ComplexAbs(result) >= 2.0) //If the caculation stopped because the absolute value of the number is already to high, we need to remember how manny iterations we did, till the number went to high
        {
            *iterationsTillAbort = counter;
        }

        return result;
    }

    return Calculate(result, point, counter + 1, iterationsTillAbort);
}

float ComplexAbs(complexNumber z)
{
    return sqrt(pow(z.rePart, 2) + pow(z.imPart, 2));
}

sf::Image CreateImage(int **matrix)
{
    sf::Image image;
    image.create(3000, 2000);
    for (unsigned int y = 0; y < 2000; y++)
    {
        for (unsigned int x = 0; x < 3000; x++)
        {
            if (matrix[x][y] == 0)      //If the number is in the mandelbrotset
            {
                image.setPixel(x, y, sf::Color(0, 0, 0));
            }
            else
            {
                int redValue = matrix[x][y] * 9;
                image.setPixel(x, y, sf::Color(redValue, 0, 0));
            }
                     

        }
    }

    return image;
}