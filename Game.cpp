#include "Game.h"



void game() {
	setlocale(LC_ALL, "rus");

	sf::RenderWindow window(sf::VideoMode(window_w, window_h), "Crossroads");
	window.setFramerateLimit(framerame_limit);

	///////////////////////////////

	CrossRoad crossroad1;
	Traffic_Lights lightSN('G','S');
	Traffic_Lights lightEW('R', 'E');
	std::list<sf::Sprite> toDraw; //В этот массив скидываем все на отрисовку


	///////////////////////////////

	sf::Vector2i mousePos;


	while (window.isOpen())
	{
		lightSN.change_light();
		lightEW.change_light();
		//crossroad1.update();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::Closed)
				window.close();

			mousePos = sf::Mouse::getPosition(window);
			//std::cout << "x = " << mousePos.x << " y = " << mousePos.y << std::endl;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//std::cout << "ЛКМ" << std::endl;
				if ((mousePos.x >= spawnN0.x && mousePos.x <= spawnN1.x) && (mousePos.y >= spawnN0.y && mousePos.y <= spawnN1.y)) { std::cout << "Спаун Северный" << std::endl; }
				else if ((mousePos.x >= spawnS0.x && mousePos.x <= spawnS1.x) && (mousePos.y >= spawnS0.y && mousePos.y <= spawnS1.y)) { std::cout << "Спаун Юг" << std::endl; }
				else if ((mousePos.x >= spawnE0.x && mousePos.x <= spawnE1.x) && (mousePos.y >= spawnE0.y && mousePos.y <= spawnE1.y)) { std::cout << "Спаун Восток" << std::endl; }
				else if ((mousePos.x >= spawnW0.x && mousePos.x <= spawnW1.x) && (mousePos.y >= spawnW0.y && mousePos.y <= spawnW1.y)) { std::cout << "Спаун Запад" << std::endl; }
				std::cout << "x = " << mousePos.x << " y = " << mousePos.y << std::endl;
			}

		}
		window.clear(sf::Color::Black);
		window.draw(crossroad1.update());
		window.draw(lightEW.update());
		window.draw(lightSN.update());

		

		//for (auto iter = toDraw.begin(); iter != toDraw.end(); iter++) window.draw(*iter); // отрисовка спрайтов
		window.display();
	}



}




