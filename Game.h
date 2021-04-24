#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <list>

const int window_w = 800;   //Ширина и высота окна
const int window_h = 800;
const int framerame_limit = 60;   //Лимит фпс
const sf::Vector2i centerMap(499, 499);  //Центр карты, к нему будет все привязано, так что очень аккуратно

// по этим координатом будем определять, где хотят заспаунить машинку, в своем роде позиции к
const sf::Vector2i spawnN0(370, 0);
const sf::Vector2i spawnN1(429,48);
const sf::Vector2i spawnS0(371,751);
const sf::Vector2i spawnS1(429,799);
const sf::Vector2i spawnE0(749,371);
const sf::Vector2i spawnE1(799,429);
const sf::Vector2i spawnW0(0,371);
const sf::Vector2i spawnW1(45,429);

void game();


class CrossRoad //Перекресток, с доступок к светофорам и машинам
{
private:
	
	class Cars;
	class Traffic_Lights;
	sf::Texture texture;
	sf::Sprite sprite;
	
public:
	CrossRoad();
	~CrossRoad();
	sf::Sprite update() {
		texture.loadFromFile("img/Crossroad(1).png");

		sprite.setTexture(texture);

		return sprite;
	}

};

CrossRoad::CrossRoad()
{
	
	

}

CrossRoad::~CrossRoad()
{
}


class Cars //Машины, тут будет и обработка координат и передача спрайта
{
public:
	Cars(char);
	
	~Cars();
	
protected:
	sf::Vector2f position;
private:
	sf::Texture texture;
	enum class direction {North, South, East, West};
	direction direction; //Узнаем направление(и чтобы давать имена)
	static int count;   //считаем машинки(и чтобы давать им имена)
	std::string car_number;



	void spawn_car();
};

Cars::Cars(char x)
{
	
	car_number = count + "_" + x;
	count++;
	switch (x) //Определим, в какую сторону поедет машинка
	{
	case 'N': direction = direction::North; break;
	case 'S': direction = direction::South; break;
	case 'E': direction = direction::East; break;
	case 'W': direction = direction::West; break;
	default:
		std::cout << "Не понимаю. в какую сторону едем?" << std::endl;
		break;
	}
}

Cars::~Cars()
{
}

void Cars::spawn_car() {

}
 

class Traffic_Lights //Светофор, он будет давать сигналы перекрестку о том, какая дорога закрыта
{
public:
	Traffic_Lights(char);
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

Traffic_Lights::Traffic_Lights(char x)
{
	lights_color color = lights_color::RED;
	switch (x) //Определим, c каким цветом будет светофор
	{
	case 'R': color = lights_color::RED; break;
	case 'G': color = lights_color::GREEN; break;
	default:
		std::cout << "Не понимаю, какой цвет?" << std::endl;
		break;
	}
	std::cout << (color == lights_color::GREEN ? "GREEN" : "RED") << std::endl;
}

Traffic_Lights::~Traffic_Lights()
{
}


int Cars::count = 0;