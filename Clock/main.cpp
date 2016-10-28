////////////////////////////////////////////////////////////
// Headers:
// ctime for getting system time and
// cmath for sin and cos functions
////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <string>

const int screenWidth = 800;
const int screenHeight = 600;
const auto PI = M_PI;
const int clockCircleSize = 250;
const int clockCircleThickness = 2;

void Draw() 
{
	
}

int main()
{
	// Define some variables and constants
	
	int x, y;
	float angle = 0.0;

	// Set multisampling level
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML Analog Clock", sf::Style::Close, settings);

	// Define windowCenter which gets the center of the window here, right after creating window
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	// Create a list for clock's dots
	sf::CircleShape hoursMarker[60];
	sf::RectangleShape minutsMarker[60];

	// load hours font from file
	sf::Font font;
	if (!font.loadFromFile("resources/arial.ttf"))
	{
		return EXIT_FAILURE;
	}
	sf::Text digits[12];

	// Create dots and place them to very right positions
	for  (int i = 0; i < 60; i++)
	{
		x = (clockCircleSize - 10) * cos(angle);
		y = (clockCircleSize - 10) * sin(angle);

		if (i % 5 == 0)
		{
			hoursMarker[i] = sf::CircleShape(3);
			hoursMarker[i].setFillColor(sf::Color::Black);
			hoursMarker[i].setOrigin(hoursMarker[i].getGlobalBounds().width / 2, hoursMarker[i].getGlobalBounds().height / 2);
			hoursMarker[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);
			digits[i/5].setFont(font);
			digits[i/5].setColor(sf::Color::Black);
			digits[i/5].setString(std::to_string(i/5 + 1));
			digits[i/5].setCharacterSize(20);
			digits[i/5].setOrigin(digits[i/5].getGlobalBounds().width / 2, digits[i/5].getGlobalBounds().height / 2);
			digits[i/5].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);
		}			
		else
		{
			minutsMarker[i] = sf::RectangleShape(sf::Vector2f(8, 3));
			minutsMarker[i].setFillColor(sf::Color::Black);
			minutsMarker[i].setOrigin(minutsMarker[i].getGlobalBounds().width / 2, minutsMarker[i].getGlobalBounds().height / 2);
			minutsMarker[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);	
			minutsMarker[i].setRotation(i*6);
		}
		
		angle = angle + ((2 * PI) / 60);
	}

	// Create outline of the clock
	sf::CircleShape clockCircle(clockCircleSize);

	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(clockCircleThickness);
	clockCircle.setOutlineColor(sf::Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);

	// Crate another circle for center
	sf::CircleShape centerCircle(10);

	centerCircle.setPointCount(100);
	centerCircle.setFillColor(sf::Color::Red);
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
	centerCircle.setPosition(windowCenter);

	// Create hour, minute, and seconds hands
	sf::RectangleShape hourHand(sf::Vector2f(5, 180));
	sf::RectangleShape minuteHand(sf::Vector2f(3, 240));
	sf::RectangleShape secondsHand(sf::Vector2f(2, 250));

	hourHand.setFillColor(sf::Color::Black);
	minuteHand.setFillColor(sf::Color::Black);
	secondsHand.setFillColor(sf::Color::Red);

	hourHand.setOrigin(hourHand.getGlobalBounds().width / 2, hourHand.getGlobalBounds().height - 25);
	minuteHand.setOrigin(minuteHand.getGlobalBounds().width / 2, minuteHand.getGlobalBounds().height - 25);
	secondsHand.setOrigin(secondsHand.getGlobalBounds().width / 2, secondsHand.getGlobalBounds().height - 25);

	hourHand.setPosition(windowCenter);
	minuteHand.setPosition(windowCenter);
	secondsHand.setPosition(windowCenter);

	// Create sound effect
	sf::Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();

	// Create clock background
	sf::Texture clockImage;
	if (!clockImage.loadFromFile("resources/clock-image.png"))
	{
		return EXIT_FAILURE;
	}

	clockCircle.setTexture(&clockImage);
	clockCircle.setTextureRect(sf::IntRect(40, 0, 500, 500));

	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Get system time
		std::time_t currentTime = std::time(NULL);

		struct tm * ptm = localtime(&currentTime);

		hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
		minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
		secondsHand.setRotation(ptm->tm_sec * 6);

		// Clear the window
		window.clear(sf::Color::White);

		// Draw all parts of clock
		window.draw(clockCircle);

		for (int i = 0; i < 60; i++)
		{
			window.draw(hoursMarker[i]);
			window.draw(minutsMarker[i]);
			if (i % 5 == 0) 
				window.draw(digits[i/5]);
		}

		window.draw(hourHand);
		window.draw(minuteHand);
		window.draw(secondsHand);
		window.draw(centerCircle);

		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}