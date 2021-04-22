#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>

const int window_w = 1280;
const int window_h = 720;
const int framerame_limit = 60;

void game();

class CrossRoad
{
private:
	
	class Cars;
	class Traffic_Lights;

public:
	CrossRoad();
	~CrossRoad();
	void update() {


		
	}

};

CrossRoad::CrossRoad()
{
	

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
	void change_light() { //смена цвета светофора
		time1 = clock.getElapsedTime();
		if (time1.asSeconds() > 5) {  //каждые 5 секунд менять цвет
			std::cout << "Прошло 5 секунд, меняем цвет светофора!" << std::endl;
			clock.restart();
			color = (color == lights_color::GREEN) ? lights_color::RED : lights_color::GREEN;
			std::cout << (color == lights_color::GREEN ? "GREEN" : "RED") << std::endl;
		} 

		//std::cout << time1.asSeconds()<<std::endl;  //Дебаги времени в консоли
	}

private:
	enum class lights_color {RED, GREEN}; // переключатель для смены цвета светофора
	lights_color color;
	sf::Clock clock; //активирую таймер для смены цвета
	sf::Time time1; 

};

Traffic_Lights::Traffic_Lights()
{
	lights_color color = lights_color::RED;
	std::cout << "RED" << std::endl;

	
}

Traffic_Lights::~Traffic_Lights()
{
}


