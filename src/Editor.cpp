#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "headers/Editor.hpp"
#include "headers/Global.hpp"

Input::Input() {}

Input::Input(float x, float y)
{
	shape.setSize(sf::Vector2f(24, 12));
	shape.setOutlineThickness(-1);
	position.x = x;
	position.y = y;
}

int Input::getValue()
{
	return std::atoi(value.str().c_str());
}

Colors::Colors()
{
	background           = sf::Color(239, 202, 178);
	mapChecker           = sf::Color(235, 166, 169);
	mapBorder            = sf::Color(235, 166, 169);
	mapRestrictedArea    = sf::Color(255,  23,  68, 191);
	place                = sf::Color(  0, 255,   0,  63);
	erase                = sf::Color(255,   0,   0,  63);
	selectionOutline     = sf::Color(213,   0,   0, 191);
	selectionBackground  = sf::Color(255,  23,  68,  63);
	inputBackground      = sf::Color(127, 127, 127);
	inputOutline         = sf::Color( 63,  63,  63);
	inputHoverOutline    = sf::Color(  0, 230, 118);
	inputSelectedOutline = sf::Color(  0, 200,  83);
}

Sprites::Sprites()
{
	// textures
	tileset_normalTexture   .loadFromFile("assets/textures/tileset_normal.png");
	tileset_iceTexture      .loadFromFile("assets/textures/tileset_ice.png");
	tileset_bounceTexture   .loadFromFile("assets/textures/tileset_bounce.png");
	tileset_selectionTexture.loadFromFile("assets/textures/tileset_selection.png");
	sawbladeTexture         .loadFromFile("assets/textures/sawblade.png");
	slimeyTexture           .loadFromFile("assets/textures/slimey.png");
	exitSignTexture         .loadFromFile("assets/textures/exit-sign.png");
	// sprites
	tileset_normal   .setTexture(tileset_normalTexture);
	tileset_ice      .setTexture(tileset_iceTexture);
	tileset_bounce   .setTexture(tileset_bounceTexture);
	tileset_selection.setTexture(tileset_selectionTexture);
	sawblade         .setTexture(sawbladeTexture);
	slimey           .setTexture(slimeyTexture);
	exitSign         .setTexture(exitSignTexture);
}

Editor::Editor(sf::RenderWindow &_window, sf::View &_view)
{
	window = &_window;
	  view = &_view;
	
	text = Text(*window);

	tilesetSprites.push_back(&sprites.tileset_normal);
	tilesetSprites.push_back(&sprites.tileset_ice);
	tilesetSprites.push_back(&sprites.tileset_bounce);
	tilesetSprites.push_back(&sprites.tileset_selection);

	spawnTile = sf::Vector3i(4, 0, 3);
	exitTile  = sf::Vector3i(4, 0, 4);

	mapSize.x = initialMapWidth;
	mapSize.y = initialMapHeight;
	clearMap();

	mapWidthInput  = Input(16, viewHeight - 32);
	mapHeightInput = Input(16, viewHeight - 16);
	mapWidthInput.value  << mapSize.x;
	mapHeightInput.value << mapSize.y;
	sizeInputs.push_back(&mapWidthInput);
	sizeInputs.push_back(&mapHeightInput);

	tileDimension.x = tilesize;
	tileDimension.y = tilesize;

	mapChecker.setSize(tileDimension);

	mapBorder.setSize(sf::Vector2f(tilesize * mapSize.x, tilesize * mapSize.y));
	mapBorder.setOrigin(mapBorder.getSize().x / 2, mapBorder.getSize().y / 2);
	mapBorder.setPosition(viewWidth / 2, viewHeight / 2);
	mapBorder.setFillColor(sf::Color::Transparent);
	mapBorder.setOutlineColor(colors.mapBorder);
	mapBorder.setOutlineThickness(mapBorderThickness);

	mapRestrictedArea.setSize(tileDimension);
	mapRestrictedArea.setFillColor(colors.mapRestrictedArea);

	ghostTile.setSize(tileDimension);

	selectionTilesetBackground.setSize(sf::Vector2f(sprites.tileset_selectionTexture.getSize().x, sprites.tileset_selectionTexture.getSize().y));
	selectionTilesetBackground.setFillColor(colors.selectionBackground);
	
	selectionTilesetSelection.setSize(tileDimension);
	selectionTilesetSelection.setFillColor(sf::Color::Transparent);
	selectionTilesetSelection.setOutlineColor(colors.selectionOutline);
	selectionTilesetSelection.setOutlineThickness(-1);

	adjustZoom();
}



void Editor::resetView()
{
	if (dragging) { return; }
	view->setCenter(viewWidth / 2, viewHeight / 2);
}

sf::Vector2f Editor::relativeViewPosition(float x, float y)
{
	return sf::Vector2f(viewBounds.left + x, viewBounds.top + y);
}



bool Editor::pressed(sf::Keyboard::Key key)
{
	if (!window->hasFocus()) { return false; }

	return sf::Keyboard::isKeyPressed(key);
}

void Editor::processKeyboardInput()
{
	if (pressed(keys.Brush))
	{ pressingBrush = true; }
	else
	{ pressingBrush = false; }

	if (pressed(keys.Fill))
	{ pressingFill = true; }
	else
	{ pressingFill = false; }

	if (pressed(keys.Erase))
	{ pressingErase = true; }
	else
	{ pressingErase = false; }

	if (pressed(keys.Reset))
	{ pressingReset = true; }
	else
	{ pressingReset = false; }

	if (pressed(keys.Save))
	{ pressingSave = true; }
	else
	{ pressingSave = false; }

	if (pressed(keys.LeftControl) || pressed(keys.RightControl))
	{ pressingControl = true; }
	else
	{ pressingControl = false; }

	if (pressed(keys.LeftShift) || pressed(keys.RightShift))
	{ pressingShift = true; }
	else
	{ pressingShift = false; }

	if (pressed(keys.LeftAlt) || pressed(keys.RightAlt))
	{ pressingAlt = true; }
	else
	{ pressingAlt = false; }

	handleKeyPresses();
}

void Editor::handleKeyPresses()
{
	if (pressingBrush)
	{
		if (!pressedBrush)
		{
			pressedBrush = true;
			method = Brush;
		}
	}
	else { pressedBrush = false; }

	if (pressingFill)
	{
		if (!pressedFill)
		{
			pressedFill = true;
			method = Fill;
		}
	}
	else { pressedFill = false; }

	if (pressingErase)
	{
		if (!pressedErase)
		{
			pressedErase = true;
			toggle(erase);
		}
	}
	else { pressedErase = false; }

	if (pressingReset)
	{
		if (!pressedReset)
		{
			pressedReset = true;
			if (pressingControl && pressingShift) { clearMap(); }
			else { resetView(); }
		}
	}
	else { pressedReset = false; }

	if (pressingSave)
	{
		if (!pressedSave)
		{
			if (pressingControl)
			{
				pressedSave = true;
				saveMap();
			}
		}
	}
	else { pressedSave = false; }
}

void removeLastCharFromStringstream(std::stringstream &stringstream)
{
	if (stringstream.str().empty()) { return; }
	std::string newString = stringstream.str();
	newString.pop_back();
	stringstream.str(newString);
	stringstream.seekp(stringstream.str().length());
}

void Editor::handleTextEntered(sf::Event event)
{
	if (!inputSelected) { return; }

	char character = static_cast<char>(event.text.unicode);

	// allowing only number inputs
	if ((int)character >= (int)'0' && (int)character <= (int)'9')
	{
		if (selectedInput->value.str().length() == 3)
		{
			removeLastCharFromStringstream(selectedInput->value);
		}
		selectedInput->value << character;
	}
	else if ((int)character == 8 && !selectedInput->value.str().empty()) // backspace to delete
	{
		removeLastCharFromStringstream(selectedInput->value);
	}
}



void Editor::processMouseInput()
{
	mousePosition     = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	prevMouseMapCoord = mouseMapCoord;
	mouseMapCoord.x   = std::floor((mousePosition.x - mapBorderBounds.left - mapBorderThickness * zoom) / (tilesize * zoom));
	mouseMapCoord.y   = std::floor((mousePosition.y - mapBorderBounds.top  - mapBorderThickness * zoom) / (tilesize * zoom));

	handleButtonPresses();
	handleDrag();
	updateViewBounds();
}

void Editor::handleButtonPresses()
{
	leftClick = false;
	middleClick = false;

	if (!window->hasFocus()) { return; }

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{ leftClick = true; }

	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{ middleClick = true; }
}

void Editor::handleDrag()
{
	if (middleClick)
	{
		if (!dragging)
		{
			dragging = true;
			draggingStartPosition = mousePosition;
		}
		sf::Vector2f dragDelta = mousePosition - draggingStartPosition;
		view->setCenter(view->getCenter() - dragDelta);
	}
	else
	{
		dragging = false;
	}
}

void Editor::adjustZoom()
{
	mapChecker.setScale(zoom, zoom);
	mapBorder.setScale(zoom, zoom);
	ghostTile.setScale(zoom, zoom);

	mapBorderBounds = mapBorder.getGlobalBounds();
}

void Editor::handleZoom(sf::Event event)
{
	if (event.mouseWheel.delta == 1)
	{
		zoom += zoomStep;
	}
	else if (event.mouseWheel.delta == -1)
	{
		zoom -= zoomStep;
	}
	zoom = std::clamp(zoom, minZoom, maxZoom);

	adjustZoom();
}

void Editor::updateViewBounds()
{
	viewBounds = sf::FloatRect(view->getCenter().x - viewWidth / 2, view->getCenter().y - viewHeight / 2, viewWidth, viewHeight);
}

void Editor::eyedropper()
{
	int tileset = getTileset(mouseMapCoord.x, mouseMapCoord.y);
	if (tileset == Tilesets::Empty) { return; }

	if (tileset < Tilesets::Selection)
	{
		selectionCoord = sf::Vector2u(0, tileset - 1);
	}
	else
	{
		selectionCoord = getTileCrop(mouseMapCoord.x, mouseMapCoord.y);
	}
}



// this function got its own seperate file because it got so big
// sf::Vector3i Editor::determineTile()
// {
// 	...
// }

sf::Vector3i Editor::getTile(unsigned int x, unsigned int y)
{
	return map.at(x).at(y);
}

int Editor::getTileset(unsigned int x, unsigned int y)
{
	return getTile(x, y).x;
}

sf::Vector2u Editor::getTileCrop(unsigned int x, unsigned int y)
{
	return sf::Vector2u(getTile(x, y).y, getTile(x, y).z);
}

int Editor::getSelectedTileset()
{
	if (erase)
	{
		return Tilesets::Empty;
	}
	else if (selectionCoord == sf::Vector2u(0, 0))
	{
		return Tilesets::Normal;
	}
	else if (selectionCoord == sf::Vector2u(0, 1))
	{
		return Tilesets::Ice;
	}
	else if (selectionCoord == sf::Vector2u(0, 2))
	{
		return Tilesets::Bounce;
	}

	return Tilesets::Selection;
}

bool Editor::invalidTile(unsigned int x, unsigned int y)
{
	if (x < 0 || x >= mapSize.x
	    ||
	    y < 0 || y >= mapSize.y)
	{
		return true;
	}

	return false;
}

void Editor::placeTile(unsigned int x, unsigned int y, sf::Vector3i newTile)
{
	if (invalidTile(x, y)
	    ||
	    x == spawnPosition.x && y == spawnPosition.y - 1 && getTile(spawnPosition.x, spawnPosition.y) == spawnTile
	    ||
	    x == exitPosition.x && y == exitPosition.y - 1 && getTile(exitPosition.x, exitPosition.y) == exitTile)
	{
		return;
	}

	if (newTile == spawnTile)
	{
		if (getTile(spawnPosition.x, spawnPosition.y) == spawnTile)
		{
			placeTile(spawnPosition.x, spawnPosition.y, emptyTile);
		}
		placeTile(x, y - 1, emptyTile);
		spawnPosition.x = x;
		spawnPosition.y = y;
	}
	else if (newTile == exitTile)
	{
		if (getTile(exitPosition.x, exitPosition.y) == exitTile)
		{
			placeTile(exitPosition.x, exitPosition.y, emptyTile);
		}
		placeTile(x, y - 1, emptyTile);
		exitPosition.x = x;
		exitPosition.y = y;
	}

	map.at(x).at(y) = newTile;
	adjustAdjacentTiles(x, y);
}

void Editor::adjustTile(unsigned int x, unsigned int y)
{
	if (invalidTile(x, y)) { return; }

	if (getTileset(x, y) != Tilesets::Empty && getTileset(x, y) != Tilesets::Selection)
	{
		map.at(x).at(y) = determineTile(sf::Vector2i(x, y), getTileset(x, y));
	}
}

void Editor::adjustAdjacentTiles(unsigned int x, unsigned int y)
{
	adjustTile(x - 1, y - 1);
	adjustTile(x + 0, y - 1);
	adjustTile(x + 1, y - 1);

	adjustTile(x - 1, y + 0);
	adjustTile(x + 0, y + 0);
	adjustTile(x + 1, y + 0);

	adjustTile(x - 1, y + 1);
	adjustTile(x + 0, y + 1);
	adjustTile(x + 1, y + 1);
}

void Editor::clearMap()
{
	map.clear();

	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		map.push_back({});
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			map.at(x).push_back(emptyTile);
		}
	}
}

void Editor::saveMap()
{
	std::stringstream mapVector;

	mapVector << "{\n\t{ ";
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			sf::Vector3i tile = getTile(x, y);
			mapVector << "{" << tile.x << "," << tile.y << "," << tile.z << "}";

			if (y == mapSize.y - 1)
			{
				mapVector << " },\n";
				if (x < mapSize.x - 1)
				{
					mapVector << "\t{ ";
				}
			}
			else
			{
				mapVector << ", ";
			}
		}
	}
	mapVector << "};";

	std::ofstream output("map.txt");
	output << mapVector.str();
	output.close();
}

void Editor::changeMapSize(unsigned int newWidth, unsigned int newHeight)
{
	unsigned int oldWidth  = mapSize.x;
	unsigned int oldHeight = mapSize.y;

	mapSize.x = newWidth;
	mapSize.y = newHeight;
	mapBorder.setSize(sf::Vector2f(tilesize * mapSize.x, tilesize * mapSize.y));
	mapBorder.setOrigin(mapBorder.getSize().x / 2, mapBorder.getSize().y / 2);

	adjustZoom();

	// reseting spawn and exit positions if any of them are out of bounds
	if (spawnPosition.x > newWidth || spawnPosition.y > newHeight)
	{ spawnPosition = sf::Vector2u(); }
	if (exitPosition.x > newWidth || exitPosition.y > newHeight)
	{ exitPosition = sf::Vector2u(); }

	// creating new map
	std::vector<std::vector<sf::Vector3i>> newMap;
	for (unsigned int x = 0; x < newWidth; x++)
	{
		newMap.push_back({});
		for (unsigned int y = 0; y < newHeight; y++)
		{
			newMap.at(x).push_back(emptyTile);
		}
	}

	// populating new map
	for (unsigned int x = 0; x < oldWidth; x++)
	{
		for (unsigned int y = 0; y < oldHeight; y++)
		{
			if (x < newWidth && y < newHeight)
			{
				newMap.at(x).at(y) = map.at(x).at(y);
			}
		}
	}

	map = newMap;

	// adjusting tiles that has been or is adjacent around the bottom and right sides
	if (newHeight > oldHeight)
	{
		for (unsigned int x = 0; x < newWidth; x++) {
			adjustTile(x, oldHeight - 1);
		}
	}
	else if (newHeight < oldHeight)
	{
		for (unsigned int x = 0; x < newWidth; x++) {
			adjustTile(x, newHeight - 1);
		}
	}
	if (newWidth > oldWidth)
	{
		for (unsigned int y = 0; y < newHeight; y++) {
			adjustTile(oldWidth - 1, y);
		}
	}
	else if (newWidth < oldWidth)
	{
		for (unsigned int y = 0; y < newHeight; y++) {
			adjustTile(newWidth - 1, y);
		}
	}
}

sf::Vector2f Editor::relativeMapPosition(float x, float y)
{
	return sf::Vector2f(mapBorderBounds.left + mapBorderThickness * zoom + x * zoom, mapBorderBounds.top + mapBorderThickness * zoom + y * zoom);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , int newTileset)
{
	if (invalidTile(x, y) || localMap.at(x).at(y).x != oldTileset) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y).x = newTileset;

	// expand up
	fill(localMap, x, y - 1, oldTileset, newTileset);
	// expand down
	fill(localMap, x, y + 1, oldTileset, newTileset);
	// expand left
	fill(localMap, x - 1, y, oldTileset, newTileset);
	// expand right
	fill(localMap, x + 1, y, oldTileset, newTileset);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, int newTileset)
{
	if (invalidTile(x, y) || localMap.at(x).at(y) != oldTile) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y).x = newTileset;

	// expand up
	fill(localMap, x, y - 1, oldTile, newTileset);
	// expand down
	fill(localMap, x, y + 1, oldTile, newTileset);
	// expand left
	fill(localMap, x - 1, y, oldTile, newTileset);
	// expand right
	fill(localMap, x + 1, y, oldTile, newTileset);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , sf::Vector3i newTile)
{
	if (invalidTile(x, y) || localMap.at(x).at(y).x != oldTileset) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y) = newTile;

	// expand up
	fill(localMap, x, y - 1, oldTileset, newTile);
	// expand down
	fill(localMap, x, y + 1, oldTileset, newTile);
	// expand left
	fill(localMap, x - 1, y, oldTileset, newTile);
	// expand right
	fill(localMap, x + 1, y, oldTileset, newTile);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, sf::Vector3i newTile)
{
	if (invalidTile(x, y) || localMap.at(x).at(y) != oldTile) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y) = newTile;

	// expand up
	fill(localMap, x, y - 1, oldTile, newTile);
	// expand down
	fill(localMap, x, y + 1, oldTile, newTile);
	// expand left
	fill(localMap, x - 1, y, oldTile, newTile);
	// expand right
	fill(localMap, x + 1, y, oldTile, newTile);
}

void Editor::fillArea()
{
	if (invalidTile(mouseMapCoord.x, mouseMapCoord.y)) { return; }

	fillTiles.clear();

	std::vector<std::vector<sf::Vector3i>> localMap = map;

	if (getSelectedTileset() < Tilesets::Selection)
	{
		// fill (int oldTileset, int newTileset)
		if (getTileset(mouseMapCoord.x, mouseMapCoord.y) < Tilesets::Selection)
		{
			if (getTileset(mouseMapCoord.x, mouseMapCoord.y) != getSelectedTileset())
			{
				fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTileset(mouseMapCoord.x, mouseMapCoord.y), getSelectedTileset());
			}
		}
		// fill (vector3i oldTile, int newTileset)
		else
		{
			fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTile(mouseMapCoord.x, mouseMapCoord.y), getSelectedTileset());
		}
	}
	else
	{
		// fill (int oldTileset, vector3i newTile)
		if (getTileset(mouseMapCoord.x, mouseMapCoord.y) < Tilesets::Selection)
		{
			fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTileset(mouseMapCoord.x, mouseMapCoord.y), sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y));
		}
		// fill (vector3i oldTile, vector3i newTile)
		else
		{
			if (getTile(mouseMapCoord.x, mouseMapCoord.y) != sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y))
			{
				fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTile(mouseMapCoord.x, mouseMapCoord.y), sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y));
			}
		}
	}
}

void Editor::tiling()
{
	if (invalidTile(mouseMapCoord.x, mouseMapCoord.y)) { return; }

	switch (method)
	{
		case Brush:
			if (erase)
			{
				placeTile(mouseMapCoord.x, mouseMapCoord.y, emptyTile);
			}
			else
			{
				placeTile(mouseMapCoord.x, mouseMapCoord.y, determineTile(mouseMapCoord, getSelectedTileset()));
			}
			break;

		case Fill:
			sf::Vector3i tile(getSelectedTileset(), selectionCoord.x, selectionCoord.y);
			if (!erase && (tile == spawnTile || tile == exitTile))
			{
				placeTile(mouseMapCoord.x, mouseMapCoord.y, determineTile(mouseMapCoord, getSelectedTileset()));
			}
			else
			{
				for (sf::Vector2u mapCoord : fillTiles)
				{
					if (erase)
					{
						placeTile(mapCoord.x, mapCoord.y, emptyTile);
					}
					else
					{
						placeTile(mapCoord.x, mapCoord.y, determineTile(mouseMapCoord, getSelectedTileset()));
					}
				}
			}
			break;
	}
}

void Editor::updateSawblades()
{
	sawbladeTimer++;
	if (sawbladeTimer % sawbladeFrameDuration == 0)
	{
		sawbladeFrame++;
	}
	if (sawbladeFrame == sawbladeFrameCount)
	{
		sawbladeTimer = 0;
		sawbladeFrame = 0;
	}
}

void Editor::drawMapBorder()
{
	window->draw(mapBorder);
}

void Editor::drawMapCheckers()
{
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			if (x % 2 == 0 && y % 2 == 0
			    ||
			    x % 2 != 0 && y % 2 != 0)
			{
				mapChecker.setPosition(relativeMapPosition(x * tilesize, y * tilesize));
				mapChecker.setFillColor(colors.mapChecker);
				window->draw(mapChecker);
			}
		}
	}
}

void Editor::drawMapTiles()
{
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			sf::Vector3i tile = getTile(x, y);
			int tileset = tile.x;
			if (tileset != 0)
			{
				sf::IntRect tilesetCrop;
				tilesetCrop.width  = tilesize;
				tilesetCrop.height = tilesize;

				sf::Sprite* tilesetSprite;
				if (tileset == Tilesets::Selection && tile.y == 1 && tile.z == 4)
				{
					tilesetSprite = &sprites.sawblade;
					tilesetCrop.left = sawbladeFrame * tilesize;
					tilesetCrop.top  = 0;
				}
				else
				{
					tilesetSprite = tilesetSprites.at(tileset - 1);
					tilesetCrop.left = tile.y * tilesize;
					tilesetCrop.top  = tile.z * tilesize;
				}
				tilesetSprite->setPosition(relativeMapPosition(x * tilesize, y * tilesize));
				tilesetSprite->setTextureRect(tilesetCrop);
				tilesetSprite->setScale(zoom, zoom);
				window->draw(*tilesetSprite);
			}
		}
	}
}

void Editor::drawMapRestrictedAreas()
{
	if (spawnPosition.y != 0 && getTile(spawnPosition.x, spawnPosition.y) == spawnTile)
	{
		sf::Vector2f position = relativeMapPosition(spawnPosition.x * tilesize, (spawnPosition.y - 1) * tilesize);
		mapRestrictedArea.setPosition(position);
		mapRestrictedArea.setScale(zoom, zoom);
		sprites.slimey.setPosition(position);
		sprites.slimey.setScale(zoom, zoom);
		window->draw(mapRestrictedArea);
		window->draw(sprites.slimey);
	}
	if (exitPosition.y != 0 && getTile(exitPosition.x, exitPosition.y) == exitTile)
	{
		sf::Vector2f position = relativeMapPosition(exitPosition.x * tilesize, (exitPosition.y - 1) * tilesize);
		mapRestrictedArea.setPosition(position);
		mapRestrictedArea.setScale(zoom, zoom);
		sprites.exitSign.setPosition(position);
		sprites.exitSign.setScale(zoom, zoom);
		window->draw(mapRestrictedArea);
		window->draw(sprites.exitSign);
	}
}

void Editor::drawMap()
{
	drawMapBorder();
	drawMapCheckers();
	drawMapTiles();
	drawMapRestrictedAreas();
}

void Editor::drawGhostTiles()
{
	if (invalidTile(mouseMapCoord.x, mouseMapCoord.y)) { return; }

	if (erase)
	{
		ghostTile.setFillColor(colors.erase);
	}
	else
	{
		ghostTile.setFillColor(colors.place);
	}

	switch (method)
	{
		case Brush:
			ghostTile.setPosition(relativeMapPosition(mouseMapCoord.x * tilesize, mouseMapCoord.y * tilesize));
			window->draw(ghostTile);
			break;

		case Fill:
			for (sf::Vector2u mapCoord : fillTiles)
			{
				ghostTile.setPosition(relativeMapPosition(mapCoord.x * tilesize, mapCoord.y * tilesize));
				window->draw(ghostTile);
			}
			break;
	}
}



void Editor::selectTile()
{
	selectionCoord.x = std::floor((mousePosition.x - selectionTilesetBounds.left) / tilesize);
	selectionCoord.y = std::floor((mousePosition.y - selectionTilesetBounds.top)  / tilesize);
}

void Editor::drawSelectionTileset()
{
	selectionTilesetBackground.setPosition(relativeViewPosition(0, 0));
	window->draw(selectionTilesetBackground);

	sprites.tileset_selection.setTextureRect(sf::IntRect(0, 0, sprites.tileset_selectionTexture.getSize().x, sprites.tileset_selectionTexture.getSize().y));
	sprites.tileset_selection.setScale(1, 1);
	sprites.tileset_selection.setPosition(relativeViewPosition(0, 0));
	selectionTilesetBounds = sprites.tileset_selection.getGlobalBounds();
	window->draw(sprites.tileset_selection);

	selectionTilesetSelection.setPosition(relativeViewPosition(selectionCoord.x * tilesize, selectionCoord.y * tilesize));
	window->draw(selectionTilesetSelection);
}



void Editor::updateSizeInputs()
{
	inputHovering = false;

	for (Input *input : sizeInputs)
	{
		input->shape.setFillColor(colors.inputBackground);
		input->shape.setOutlineColor(colors.inputOutline);
		if (input->bounds.contains(mousePosition))
		{
			inputHovering = true;
			input->shape.setOutlineColor(colors.inputHoverOutline);
			if (leftClick)
			{
				inputSelected = true;
				selectedInput = input;
			}
		}
	}
}

void Editor::clampSizeInputs()
{
	// clamp width
	int width = std::atoi(mapWidthInput.value.str().c_str());        // string to int
	width = std::clamp((unsigned int)width, minMapSize, maxMapSize);
	mapWidthInput.value.str(std::to_string(width));                  // int to string

	// clamp height
	int height = std::atoi(mapHeightInput.value.str().c_str());        // string to int
	height = std::clamp((unsigned int)height, minMapSize, maxMapSize);
	mapHeightInput.value.str(std::to_string(height));                  // int to string
}

void Editor::drawSizeInputs()
{
	for (Input *input : sizeInputs)
	{
		input->shape.setPosition(relativeViewPosition(input->position.x, input->position.y));
		input->bounds = input->shape.getGlobalBounds();
		if (inputSelected && selectedInput == input)
		{
			input->shape.setOutlineColor(colors.inputSelectedOutline);
		}
		window->draw(input->shape);

		text.draw(input->value.str(), Text::Alignment::Start, Text::Alignment::Center, sf::Vector2f(input->bounds.left + 3.5, input->bounds.top + input->bounds.height / 2));
	}

	using align = Text::Alignment;
	text.draw("Size", align::Center, align::Center, relativeViewPosition(28, viewHeight - 40));
	text.draw("x", align::Center, align::Center, sf::Vector2f(relativeViewPosition(8, 0).x, mapWidthInput.bounds.top + mapWidthInput.bounds.height / 2));
	text.draw("y", align::Center, align::Center, sf::Vector2f(relativeViewPosition(8, 0).x, mapHeightInput.bounds.top + mapHeightInput.bounds.height / 2));
}



void Editor::update()
{
	processKeyboardInput();
	processMouseInput();

	updateSizeInputs();

	if (!inputHovering)
	{
		if (method == Fill && mapBorderBounds.contains(mousePosition))
		{
			fillArea();
		}

		if (leftClick)
		{
			if (inputSelected)
			{
				inputSelected = false;
				clampSizeInputs();
				changeMapSize(mapWidthInput.getValue(), mapHeightInput.getValue());
			}

			if (pressingAlt)
			{
				eyedropper();
			}
			else
			{
				if (selectionTilesetBounds.contains(mousePosition))
				{
					selectTile();
				}
				else if (mapBorderBounds.contains(mousePosition))
				{
					tiling();
				}
			}
		}
	}

	updateSawblades();
}

void Editor::draw()
{
	drawMap();

	if (mapBorderBounds.contains(mousePosition) && !selectionTilesetBounds.contains(mousePosition) && !inputHovering)
	{
		drawGhostTiles();
	}

	drawSelectionTileset();

	drawSizeInputs();
}
