#include <SFML/Graphics.hpp>

#include "headers/Editor.hpp"
#include "headers/Global.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Editor", sf::Style::Default);
	sf::View view(sf::FloatRect(0, 0, viewWidth, viewHeight));
	sf::Event event;
	window.setFramerateLimit(FPS);

	Editor editor(window, view);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				editor.handleZoom(event);
			}
			else if (event.type == sf::Event::TextEntered)
			{
				editor.handleTextEntered(event);
			}
		}

		window.clear(editor.colors.background);

		editor.update();

		window.setView(view);

		editor.draw();

		window.display();
	}

	return 0;
}
