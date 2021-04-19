#include "SFML/Graphics.hpp"

const int window_w = 1280;
const int window_h = 720;

class Engine;
class Update;
class Track;
class Cars;
class Traffic_Lights;

int main()
{
	sf::Window window(sf::VideoMode(window_w, window_h), "Crossroads");
	
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
		
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}


class Engine
{
public:
	Engine();
	~Engine();

private:
	class Update;

};

Engine::Engine()
{
}

Engine::~Engine()
{
}

class Update
{
public:
	Update();
	~Update();

private:

};

Update::Update()
{

}

Update::~Update()
{

}

class Track
{
public:
	Track();
	~Track();

private:
	class Cars;
	class Traffic_Lights;

};

Track::Track()
{
}

Track::~Track()
{
}

class Cars
{
public:
	Cars();
	~Cars();

private:

};

Cars::Cars()
{
}

Cars::~Cars()
{
}

class Traffic_Lights
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