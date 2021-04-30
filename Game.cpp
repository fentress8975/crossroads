#include "Game.h"

void initWindow(sf::RenderWindow& window) { //Иницилизация окна
	sf::VideoMode videoMode(window_w, window_h);
	const std::string title = "Crossroads";
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(videoMode, title, sf::Style::Default, settings);
}

void game() {
	system("chcp 65001");	//Убираем кракозябры в консоле
	sf::RenderWindow window;
	window.setFramerateLimit(framerame_limit);
	initWindow(window);

	///////////////////////////////
	CrossRoad crossroad1;									//Создаем перекресток
	Traffic_Lights* lightSN = new Traffic_Lights('G', 'S'); //Создаем светофоры с нужным цветом (Red Green и направлением N S или E W
	Traffic_Lights* lightEW = new Traffic_Lights('R', 'E');
	std::list<Traffic_Lights*> lights;						//Запихиваем их в список, чтобы передать их
	lights.push_back(lightSN);
	lights.push_back(lightEW);
	
	Cars cars;												//Создаем контроллер машин
	std::list<Car*>* car_list = cars.getCars();				//Список машин, чтобы передавать их
	Draw cross(car_list,lights);

	///////////////////////////////

	sf::Vector2f mousePos;

	while (window.isOpen())
	{
		lightSN->change_light();		//Включаем таймер светофорам
		lightEW->change_light();
		crossroad1.update(car_list, lights);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::Closed)
				window.close();
			mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)); //Методы не принимаю в int, поэтому переводим позицию в float
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {	//Проверяем на какой спаун нажали
				if (spawnN_button.contains(mousePos)) { 
					std::cout << "Спаун Северный" << std::endl; 
					cars.spawn_car(South);
				}
				else if (spawnS_button.contains(mousePos)) {
					std::cout << "Спаун Юг" << std::endl; 
					cars.spawn_car(North);
				}
				else if (spawnE_button.contains(mousePos)) {
					std::cout << "Спаун Восток" << std::endl; 
					cars.spawn_car(West);
				}
				else if (spawnW_button.contains(mousePos)) {
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