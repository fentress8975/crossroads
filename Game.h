#include "SFML/Graphics.hpp"
#include <iostream>

const int window_w = 1280;
const int window_h = 720;
const int framerame_limit = 60;

void game();

class CrossRoad
{
private:
	sf::RenderWindow window;
	class Cars;
	class Traffic_Lights;

public:
	CrossRoad();
	~CrossRoad();
	void update() {
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					window.close();
				if (event.type == sf::Event::Closed)
					window.close();


				window.clear(sf::Color::Black);

				//window.draw();

				window.display();
			}
		}
	}

};

CrossRoad::CrossRoad()
{
	sf::RenderWindow window(sf::VideoMode(window_w, window_h), "Crossroads");
	window.setFramerateLimit(framerame_limit);

}

CrossRoad::~CrossRoad()
{
}

class Cars : CrossRoad
{
public:
	Cars();
	~Cars();

private:
	sf::Texture texture;
	int car_number;

};

Cars::Cars()
{
}

Cars::~Cars()
{
}

class Traffic_Lights : CrossRoad
{
public:
	Traffic_Lights();
	~Traffic_Lights();

private:

};

Traffic_Lights::Traffic_Lights()
{
}

Traffic_Lights::~Traffic_Lights()
{
}


