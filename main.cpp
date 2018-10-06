#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include <SFML/Graphics.hpp>

#define RESOLUTION_X 3000
#define RESOLUTION_Y 2000

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

//Function prototypes
int** Create();
float ComplexAbs(complexNumber z);
complexNumber Calculate(complexNumber z, complexNumber point, int counter, int *iterationsTillAbort);
sf::Sprite CreateSprite(int **matrix);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mandelbrotmenge");

    //Create the mandelbrot set and an image to render it
    sf::Sprite sprite = CreateSprite(Create());    

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
    matrix = new int*[RESOLUTION_X];
    for (unsigned int i = 0; i < RESOLUTION_X; i++)
    {
        matrix[i] = new int[RESOLUTION_Y];
    }

    //Iterate through the whoer matrix
    for (int y = -(RESOLUTION_Y / 2.0); y < (RESOLUTION_Y / 2.0); y++)
    {
        for (int x = -(RESOLUTION_X / 1.5); x < (RESOLUTION_X / 3.0); x++)
        {
            //Create a point er z0s in the mandelbrot set
            complexNumber point;
            point.rePart = static_cast<float>(x / (RESOLUTION_X / 3.0));
            point.imPart = static_cast<float>(y / (RESOLUTION_Y / 2.0));

            //Start with 0 at first
            complexNumber null;
            null.rePart = 0;
            null.imPart = 0;

            int *iterationsTillAbort = new int; //If a number is NOT in the mandelbrot set, the amount of iteration it has taken to find this out is stored here 

            float resultAbs = ComplexAbs(Calculate(null, point, 0, iterationsTillAbort)); //If the absolute value after 20 iterations is smaller than 2, the number is in the mandelbrot set         
            if (resultAbs < 2.0)
            {
                matrix[x + static_cast<int>((RESOLUTION_X / 1.5))][y + (RESOLUTION_Y / 2)] = 0;
            }
            else                                                                        //Otherwise it's not
            {
                //If the number is not in the mandelbrot set look how many iterations we had to do until the number went to high
                matrix[x + static_cast<int>((RESOLUTION_X / 1.5))][y + (RESOLUTION_Y / 2)] = *iterationsTillAbort + 1;
            }

            delete iterationsTillAbort;
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

    //If we did 20 iterations or the absolute value of the number is already to high -> exit the function
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
    return sqrt(pow(z.rePart, 2) + pow(z.imPart, 2));       //Return the absolute value of the complex number
}

sf::Sprite CreateSprite(int **matrix)
{
    //Create a image which can be rendered
    sf::Image image;
    image.create(RESOLUTION_X, RESOLUTION_Y);

    for (unsigned int y = 0; y < RESOLUTION_Y; y++) //Go through the whole matrix
    {
        for (unsigned int x = 0; x < RESOLUTION_X; x++)
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

    //Save to file
    image.saveToFile("Mandelbrotset.jpg");

    //Create texture and sprite
    sf::Texture *texture = new sf::Texture;
    texture->loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setScale(sf::Vector2f(0.54, 0.54));
    sprite.setPosition(150, 0);

    sf::Vector2f scale((3000.0 / RESOLUTION_X) * 0.54, (2000.0 / RESOLUTION_Y) * 0.54);
    sprite.setScale(scale);

    return sprite;
}