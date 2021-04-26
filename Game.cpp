#include "Game.h"

void initWindow(sf::RenderWindow& window) {
	sf::VideoMode videoMode(window_w, window_h);
	const std::string title = "Crossroads";
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(videoMode, title, sf::Style::Default, settings);
}

void game() {

	setlocale(LC_ALL, "rus");

	sf::RenderWindow window;
	window.setFramerateLimit(framerame_limit);
	initWindow(window);

	std::list<Traffic_Lights*> ::iterator iterL;

	///////////////////////////////
	CrossRoad crossroad1;
	Traffic_Lights* lightSN = new Traffic_Lights('G', 'S');
	Traffic_Lights* lightEW = new Traffic_Lights('R', 'E');
	

	std::list<Traffic_Lights*> lights;
	lights.push_back(lightSN);
	lights.push_back(lightEW);
	
	Cars cars;
	std::list<Car*>* car_list = cars.getCars();
	Draw cross(car_list,lights);

	///////////////////////////////

	sf::Vector2i mousePos;

	while (window.isOpen())
	{
		lightSN->change_light();
		lightEW->change_light();
		cars.destroy();
		crossroad1.update(car_list, lights);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::Closed)
				window.close();

			mousePos = sf::Mouse::getPosition(window);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//std::cout << "ЛКМ" << std::endl;
				if ((mousePos.x >= spawnN0.x && mousePos.x <= spawnN1.x) && (mousePos.y >= spawnN0.y && mousePos.y <= spawnN1.y)) { 
					std::cout << "Спаун Северный" << std::endl; 
					cars.spawn_car(South);
				}
				else if ((mousePos.x >= spawnS0.x && mousePos.x <= spawnS1.x) && (mousePos.y >= spawnS0.y && mousePos.y <= spawnS1.y)) { 
					std::cout << "Спаун Юг" << std::endl; 
					cars.spawn_car(North);
				}
				else if ((mousePos.x >= spawnE0.x && mousePos.x <= spawnE1.x) && (mousePos.y >= spawnE0.y && mousePos.y <= spawnE1.y)) {
					std::cout << "Спаун Восток" << std::endl; 
					cars.spawn_car(West);
				}
				else if ((mousePos.x >= spawnW0.x && mousePos.x <= spawnW1.x) && (mousePos.y >= spawnW0.y && mousePos.y <= spawnW1.y)) {
					std::cout << "Спаун Запад" << std::endl; 
					cars.spawn_car(East);
				}
				std::cout << "x = " << mousePos.x << " y = " << mousePos.y << std::endl;
			}

		}
		
		window.clear(sf::Color::Black);
		window.draw(crossroad1.start());
		window.draw(cross);
		window.display();
	}



}




