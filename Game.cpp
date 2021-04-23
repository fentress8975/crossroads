#include "Game.h"



void game() {
	setlocale(LC_ALL, "rus");
	sf::RenderWindow window(sf::VideoMode(window_w, window_h), "Crossroads");
	window.setFramerateLimit(framerame_limit);

	CrossRoad crossroad1;
	Traffic_Lights light1;

	while (window.isOpen())
	{
		light1.change_light();
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




