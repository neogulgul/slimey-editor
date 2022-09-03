#include <SFML/Graphics.hpp>

#include "headers/Text.hpp"

void loadPixelatedFont(sf::Font &font, int fontSize, std::string fontPath)
{
	font.loadFromFile(fontPath);
	sf::Texture& fontTexture = const_cast<sf::Texture&>(font.getTexture(fontSize));
	fontTexture.setSmooth(false);
}

Text::Text() {}

Text::Text(sf::RenderWindow &_window) {
	window = &_window;

	loadPixelatedFont(font, fontSize, "assets/fonts/FiveByFive.ttf");
}

void Text::draw(std::string string, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position)
{
	sf::Text text;
	text.setString(string);
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setOrigin(0, text.getCharacterSize() / 2 - 1);

	sf::Vector2f adjustment;
	sf::FloatRect textBounds = text.getGlobalBounds();

	switch (horizontalAlignment)
	{
		case Start:
			adjustment.x = 0;
			break;

		case Center:
			adjustment.x = textBounds.width / 2;
			break;

		case End:
			adjustment.x = textBounds.width;
			break;
	}

	switch (verticalAlignment)
	{
		case Start:
			adjustment.y = 0;
			break;

		case Center:
			adjustment.y = textBounds.height / 2;
			break;

		case End:
			adjustment.y = textBounds.height;
			break;
	}

	text.setPosition(position - adjustment);

	window->draw(text);
}
