#pragma once

struct Text
{
	sf::RenderWindow *window;

	sf::Font font;
	int fontSize = 8;

	enum Alignment { Start, Center, End };

	Text();
	Text(sf::RenderWindow &_window);

	void draw(std::string string, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position);
};
