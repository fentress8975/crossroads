#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <list>
#include <iterator>

const int window_w = 800;					//Ширина и высота окна
const int window_h = 800;
const int framerame_limit = 60;				//Лимит фпс
const sf::Vector2f centerMap(400, 400);		//Центр карты
sf::Vector2f roadSize(30, 30);				//Размер дороги(примерный)
sf::Vector2f carSize(16, 8);				//Размер машинки(не примерный)

// по этим координатом будем определять, где хотят заспаунить машинку,
// можно сказать это как невидимые квадратные кнопки
const sf::Vector2f spawnN0(	centerMap.x - roadSize.x, 
							centerMap.y - centerMap.y); //это например верхний левый угол Северного спауна
const sf::Vector2f spawnN1(	centerMap.x + roadSize.x, 
							centerMap.y - centerMap.y + roadSize.y * 2); //а это нижний правый
//////////////////////////////////// 
const sf::Vector2f spawnS0(	centerMap.x - roadSize.x, 
							centerMap.y + centerMap.y - roadSize.y * 2);
const sf::Vector2f spawnS1(	centerMap.x + roadSize.x, 
							centerMap.y + centerMap.y);
//////////////////////////////////// 
const sf::Vector2f spawnE0(	centerMap.x + centerMap.x - roadSize.x * 2,
							centerMap.y - roadSize.y);
const sf::Vector2f spawnE1(	centerMap.x + centerMap.x, 
							centerMap.y + roadSize.y);
//////////////////////////////////// 
const sf::Vector2f spawnW0(	centerMap.x - centerMap.x, 
							centerMap.y - roadSize.y);
const sf::Vector2f spawnW1(	centerMap.x - centerMap.x + roadSize.x * 2,
							centerMap.y + roadSize.y);
//////////////////////////////////// 
/// координаты спауна машин
const sf::Vector2f spawn_carN(	centerMap.x - carSize.x, 
								centerMap.y - centerMap.y);
const sf::Vector2f spawn_carS(	centerMap.x + carSize.x, 
								centerMap.y + centerMap.y);
const sf::Vector2f spawn_carE(	centerMap.x + centerMap.x, 
								centerMap.y - carSize.x);
const sf::Vector2f spawn_carW(	centerMap.x - centerMap.x, 
								centerMap.y + carSize.x);
//////////////////////////////////// 
/// координаты светофоров
const sf::Vector2f spawn_lightSN(440, 440);
const sf::Vector2f spawn_lightEW(350, 365);
////////////////////////////////////

void game();
enum direction { North, South, East, West, SN, EW, ERROR }; //направления
class Car;
class Cars;
class Traffic_Lights;
class Draw;


class Car
{
public:
	Car(direction);
	~Car();
	friend class Cars;				//Очень сильно решает проблему с доступом, так что не трогать
	sf::Sprite getSprite() const;	//Для отрисовщика
	::direction getDir() {
		return direction;
	}
	void setPos(sf::Vector2f);
	void update(bool, std::list<Car*>&);				//обновление координат
	sf::FloatRect getGlobalBoun(){ return sprite.getGlobalBounds(); }
	sf::FloatRect getBounds() {
		if (direction == North) {
			sf::Sprite gap = sprite;
			gap.move(0, -30);
			return gap.getGlobalBounds();
		}
		if (direction == South) {
			sf::Sprite gap = sprite;
			gap.move(0, 30);
			return gap.getGlobalBounds();
		}
		if (direction == East) {
			sf::Sprite gap = sprite;
			gap.move(30, 0);
			return gap.getGlobalBounds();
		}
		if (direction == West) {
			sf::Sprite gap = sprite;
			gap.move(-30, 0);
			return gap.getGlobalBounds();
		}
	}
private:
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	direction direction;			//Направление
	enum direction getDir(char);
	void destroy();
	float acceacceleration = 0.3;
	float breaks = 0.5;
	float speed = 0;
	float max_speed = 3.0;			//иногда миллиардные мешают циклу
	void go() {
		if (speed < max_speed) {
			speed += acceacceleration;
		}
		if (direction == North) {
			position.y -= speed;
		}
		if (direction == South) {
			position.y += speed;
		}
		if (direction == East) {
			position.x += speed;
		}
		if (direction == West) {
			position.x -= speed;
		}

	}
	void stop() {
		if (speed > 0) { speed -= breaks; }
		if (speed < 0) { speed = 0; }
	}
};

class Cars							//Машины, тут будет и обработка координат и передача спрайта
{
public:
	Cars();
	~Cars();
	bool isActive(direction);		//Проверка спауна, что там нету машинки
	void spawn_car(direction);		//создает машину по клику
	void destroy();					//Для удаление машинок за экраном
	std::list<Car*>* getCars();

private:
	int count;						//считаем машинки(и чтобы давать им имена)
	std::list<Car*> cars;			//Тут хранятся все машины
};

Cars::Cars()
{
	cars = {};
	count = 0;
}

Cars::~Cars()
{
	
}

std::list<Car*>* Cars::getCars() {
	return &cars;
}

void Cars::spawn_car(direction dir) {
	if (count < 100) {
		if (isActive(dir)) {
			Car* car = new Car(dir);
			if (dir == North) car->setPos(spawn_carS);
			if (dir == South) car->setPos(spawn_carN);
			if (dir == East) car->setPos(spawn_carW);
			if (dir == West) car->setPos(spawn_carE);
			count++;
			cars.push_back(car);
		}
	}
}

void Cars::destroy() {
	for (auto iter = cars.begin(); iter != cars.end(); iter++) {
		Car* car = *iter;
		if (car->position.x > centerMap.x+centerMap.x+carSize.x || 
			car->position.y > centerMap.y + centerMap.y + carSize.x || 
			car->position.x < centerMap.x - centerMap.x - carSize.x || 
			car->position.y < centerMap.y - centerMap.y - carSize.x) { //если машинка за экраном, удаляем объект и указатель
			car->destroy();
			cars.erase(iter);
			--count;
			break; //чтобы не ломался итератор от потери в пространстве
		}
	}
}

bool Cars::isActive(direction dir) {
	if (cars.empty()) return true;		//если список пуст
	for (auto iter = cars.begin(); iter != cars.end(); iter++) {
		Car* car = *iter;
		if (dir == South) {
			if ((car->position.x >= spawnN0.x && car->position.x <= spawnN1.x) && 
				(car->position.y >= spawnN0.y && car->position.y <= spawnN1.y)) return false;
		}
		else if (dir == North) {
			if ((car->position.x >= spawnS0.x && car->position.x <= spawnS1.x) && 
				(car->position.y >= spawnS0.y && car->position.y <= spawnS1.y)) return false;
		}
		else if (dir == West) {
			if ((car->position.x >= spawnE0.x && car->position.x <= spawnE1.x) && 
				(car->position.y >= spawnE0.y && car->position.y <= spawnE1.y)) return false;
		}
		else if (dir == East) {
			if ((car->position.x >= spawnW0.x && car->position.x <= spawnW1.x) && 
				(car->position.y >= spawnW0.y && car->position.y <= spawnW1.y)) return false;
		}
	}
	return true;
}
 


Car::Car(::direction dir)
{
	texture.loadFromFile("img/car.png");
	sprite.setTexture(texture);
	direction = dir;
	if (direction == North) { sprite.setRotation(90); }
	if (direction == South) { sprite.setRotation(-90); }
	if (direction == East) { sprite.setRotation(180); }
	std::cout << "Sozdal mashiny" << std::endl;

}

Car::~Car()
{
	std::cout << "ybil mashiny" << std::endl;
}

void Car::update(bool canGo, std::list<Car*>& cars) {
	sf::FloatRect car_in_front;
	for (auto iter = cars.begin(); iter != cars.end(); iter++) { //Проверяем, есть ли спереди машина
		car_in_front = (*iter)->getGlobalBoun();
		if (car_in_front.intersects(getBounds())) { stop(); return; }
	}
	if ((position.x > (centerMap.x + roadSize.x)) && (position.x < (centerMap.x - roadSize.x)) &&	//если машина уже на перекрестке, пускай едет 
		(position.y > (centerMap.y + roadSize.y)) && (position.y < (centerMap.y - roadSize.y))) {	//без остановки
		go();
		return;
	}

	if ((position.x <= (centerMap.x + roadSize.x)) && (position.x >= (centerMap.x - roadSize.x)) && //проверяем, что машина не рядом с перекрестком
		(position.y <= (centerMap.y + roadSize.y)) && (position.y >= (centerMap.y - roadSize.y))) {
		go();
	}
	else { 
		if (direction == North) {
			if ((position.y <= centerMap.y) ||
				(position.y >= (centerMap.y + (roadSize.y + 20)))) { //проверяем, когда машина уже рядом с перекрестком
				go();
			}
			else if (canGo) { //сюда приходит значение светофора
				go();
			}
			else { stop(); }
		}
		if (direction == South) {
			if ((position.y <= (centerMap.y - (roadSize.y + 20))) ||
				(position.y >= centerMap.y)) {
				go();
			}
			else if (canGo) {
				go();
			}
			else { stop(); }
		}
		if (direction == East) {
			if ((position.x <= (centerMap.x - (roadSize.x + 20))) || 
				(position.x >= centerMap.x)) {
				go();
			}
			else if (canGo) {
				go();
			}
			else { stop(); }
		}
		if (direction == West) {
			if ((position.x <= centerMap.x) || 
				(position.x >= (centerMap.x + (roadSize.x + 20)))) {
				go();
			}
			else if (canGo) {
				go();
			}
			else { stop(); }
		}
	}
	
	sprite.setPosition(position);
}

void Car::destroy() {

		delete this;
}

sf::Sprite Car::getSprite() const{
	return sprite;
}

void Car::setPos(sf::Vector2f spawn) {
	this->position.x = spawn.x;
	this->position.y = spawn.y;
}

class Traffic_Lights										//Светофор, он будет давать сигналы перекрестку о том, какая дорога закрыта
{
public:
	Traffic_Lights(char,char);
	~Traffic_Lights();
	void change_light();
	void update();
	bool canGo();
	direction getDir() {
		if (direction == SN) return SN;
		else return EW;
	}
	sf::Sprite getSprite() const;
private:
	enum class lights_color {RED, GREEN};					// переключатель для смены цвета светофора
	lights_color color;
	sf::Clock clock;										//таймер для смены цвета
	sf::Time time1; 
	sf::Texture texture_green;
	sf::Texture texture_red;
	sf::Sprite sprite;
	direction direction;

};

Traffic_Lights::Traffic_Lights(char x, char y)
{
	texture_green.loadFromFile("img/traff_light_G.png");
	texture_red.loadFromFile("img/traff_light_R.png");
	switch (x)												 //Определим, c каким цветом будет светофор
	{
	case 'R': 
		color = lights_color::RED;
		sprite.setTexture(texture_red);
		break;
	case 'G': 
		color = lights_color::GREEN;
		sprite.setTexture(texture_green);
		break;
	default:
		std::cout << "Не понимаю, какой цвет?" << std::endl;
		break;
	}
	std::cout << (color == lights_color::GREEN ? "GREEN" : "RED") << std::endl;
	switch (y)												//Определим, на какое направление будет светофор
	{
	case 'S': 
		sprite.setPosition(spawn_lightSN.x, spawn_lightSN.y);
		direction = SN;
		break;
	case 'E': 
		sprite.setPosition(spawn_lightEW.x, spawn_lightEW.y);
		sprite.setRotation(-90);
		direction = EW;
		break;
	default:
		std::cout << "Не понимаю, какое направление?" << std::endl;
		break;
	}
}

Traffic_Lights::~Traffic_Lights()
{
}

void Traffic_Lights::change_light() {									//смена цвета светофора
	time1 = clock.getElapsedTime();
	if (time1.asSeconds() > 10) {										//каждые 5 секунд менять цвет
		std::cout << "Прошло 10 секунд, меняем цвет светофора!" << std::endl;
		clock.restart();
		color = (color == lights_color::GREEN) ? lights_color::RED : lights_color::GREEN;
		std::cout << (color == lights_color::GREEN ? "GREEN" : "RED") << std::endl;
	}
	//std::cout << time1.asSeconds()<<std::endl;						//Дебаги времени в консоли
}

void Traffic_Lights::update() {
	if (color == lights_color::GREEN) {
		sprite.setTexture(texture_green);
	}
	else {
		sprite.setTexture(texture_red);
	}
}

sf::Sprite Traffic_Lights::getSprite()const{
	return sprite;
}

bool Traffic_Lights::canGo() {
	return color == lights_color::GREEN ? true : false;
}


class CrossRoad							//Перекресток, все обновляет
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

public:
	CrossRoad();
	~CrossRoad();
	sf::Sprite start() { //отрисовка БГ
		texture.loadFromFile("img/Crossroad(1).png");
		sprite.setTexture(texture);
		return sprite;
	}
	void update(std::list<Car*>*, std::list<Traffic_Lights*>&) ; //Обновляем все
};

CrossRoad::CrossRoad()
{

}

CrossRoad::~CrossRoad()
{
}

void CrossRoad::update(std::list<Car*>* cars, std::list<Traffic_Lights*>& traffic_lights) {
	Traffic_Lights* light_SN = nullptr;
	Traffic_Lights* light_EW = nullptr;
	std::list<Traffic_Lights*> ::iterator iterL;
	std::list<Car*> ::iterator iterC;
	for (iterL = traffic_lights.begin(); iterL != traffic_lights.end(); ++iterL) {
		if ((*iterL)->getDir() == SN) {
			light_SN = *iterL;
			(*iterL)->update();
		}
		else {
			light_EW = *iterL;
			(*iterL)->update();
		}
	}
	for (iterC = cars->begin(); iterC != cars->end(); ++iterC) {
		if ((*iterC)->getDir() == North || (*iterC)->getDir() == South) {
			(*iterC)->update(light_SN->canGo(),*cars);
		}

		else if ((*iterC)->getDir() == East || (*iterC)->getDir() == West) {
			(*iterC)->update(light_EW->canGo(),*cars);
		}
	}
}


class Draw :public sf::Drawable {				//отрисовка всего и вся
public:
	Draw(std::list<Car*>*, std::list<Traffic_Lights*>&);
	std::list<Car*>* cars;
	std::list<Traffic_Lights*> traffic_lights;
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;  //перезагрузка отрисовка
};

Draw::Draw(std::list<Car*>* cars, std::list<Traffic_Lights*>& traffic_lights) :cars(cars), traffic_lights(traffic_lights) {

}

void Draw::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	for (auto iter = traffic_lights.begin(); iter != traffic_lights.end(); iter++) {
		target.draw((*iter)->getSprite(), states);
	}
	for (auto iter = cars->begin(); iter != cars->end(); iter++) {
		target.draw((*iter)->getSprite());
	}
}