#pragma once

#include <sstream>

#include "Text.hpp"

struct Input
{
	sf::RectangleShape shape;
	sf::FloatRect bounds;
	sf::Vector2f position;
	std::stringstream value;
	bool selected = false;

	Input();
	Input(float x, float y);

	int getValue();
};

struct Colors
{
	sf::Color background;
	sf::Color mapChecker;
	sf::Color mapBorder;
	sf::Color mapRestrictedArea;
	sf::Color place;
	sf::Color erase;
	sf::Color selectionOutline;
	sf::Color selectionBackground;
	sf::Color inputBackground;
	sf::Color inputOutline;
	sf::Color inputHoverOutline;
	sf::Color inputSelectedOutline;

	Colors();
};

struct Sprites
{
	// textures
	sf::Texture tileset_normalTexture;
	sf::Texture tileset_iceTexture;
	sf::Texture tileset_bounceTexture;
	sf::Texture tileset_selectionTexture;
	sf::Texture sawbladeTexture;
	sf::Texture slimeyTexture;
	sf::Texture exitSignTexture;
	// sprites
	sf::Sprite tileset_normal;
	sf::Sprite tileset_ice;
	sf::Sprite tileset_bounce;
	sf::Sprite tileset_selection;
	sf::Sprite sawblade;
	sf::Sprite slimey;
	sf::Sprite exitSign;

	Sprites();
};

struct Keys
{
	using keyboard = sf::Keyboard;

	keyboard::Key Brush        = keyboard::B;
	keyboard::Key Fill         = keyboard::F;
	keyboard::Key Erase        = keyboard::E;
	keyboard::Key Reset        = keyboard::R;
	keyboard::Key Save         = keyboard::S;
	keyboard::Key LeftControl  = keyboard::LControl;
	keyboard::Key RightControl = keyboard::RControl;
	keyboard::Key LeftShift    = keyboard::LShift;
	keyboard::Key RightShift   = keyboard::RShift;
	keyboard::Key LeftAlt      = keyboard::LAlt;
	keyboard::Key RightAlt     = keyboard::RAlt;
};

struct Editor
{
	sf::RenderWindow *window;
	sf::View *view;
	sf::FloatRect viewBounds;

	Colors colors;
	Sprites sprites;
	Text text;

	// editor related
	enum Tilesets { Empty, Normal, Ice, Bounce, Selection };
	std::vector<sf::Sprite*> tilesetSprites;
	sf::Vector3i emptyTile;
	sf::Vector3i spawnTile;
	sf::Vector3i exitTile;
	sf::Vector2u selectionCoord;
	sf::Vector2u spawnPosition;
	sf::Vector2u exitPosition;
	std::vector<sf::Vector2u> fillTiles;
	enum Method { Brush, Fill };
	Method method = Brush;
	bool erase = false;
	int sawbladeTimer         = 0;
	int sawbladeFrame         = 0;
	int sawbladeFrameCount    = 8;
	int sawbladeFrameDuration = 2;

	// inputs related
	Input mapWidthInput;
	Input mapHeightInput;
	std::vector<Input*> sizeInputs;
	Input *selectedInput;
	bool inputHovering = false;
	bool inputSelected = false;

	// keyboard related
	Keys keys;
	// B
	bool pressingBrush   = false;
	bool pressedBrush    = false;
	// F
	bool pressingFill    = false;
	bool pressedFill     = false;
	// E
	bool pressingErase   = false;
	bool pressedErase    = false;
	// R
	bool pressingReset   = false;
	bool pressedReset    = false;
	// S
	bool pressingSave    = false;
	bool pressedSave     = false;
	// Control
	bool pressingControl = false;
	// Shift
	bool pressingShift   = false;
	// Alt
	bool pressingAlt     = false;

	// mouse related
	sf::Vector2f mousePosition;
	sf::Vector2i prevMouseMapCoord;
	sf::Vector2i     mouseMapCoord;
	bool leftClick   = false;
	bool middleClick = false;
	// drag
	sf::Vector2f draggingStartPosition;
	bool dragging = false;

	// zoom related
	float zoom     = 1.0;
	float zoomStep = 0.1;
	float minZoom  = 0.1;
	float maxZoom  = 2.0;

	// map related
	std::vector<std::vector<sf::Vector3i>> map;
	/*
	one 3D vector has 3 values (x, y, z) which in this case represents one tile
	the x value represents which tileset to draw
	the y value represents the horizontal (x) crop of the respective tileset
	the z value represents the vertical (y) crop of the respective tileset

	could change it so that the z represents the tileset
	because that would make more sense
	but i am to lazy to do so
	*/
	unsigned int minMapSize = 16;
	unsigned int maxMapSize = 64;
	unsigned int initialMapWidth  = minMapSize;
	unsigned int initialMapHeight = minMapSize;
	sf::Vector2u mapSize;

	sf::Vector2f tileDimension;

	sf::RectangleShape mapChecker;
	sf::RectangleShape mapBorder;
	sf::RectangleShape mapRestrictedArea;

	sf::FloatRect mapBorderBounds;
	int mapBorderThickness = 4;

	sf::RectangleShape ghostTile;

	sf::FloatRect      selectionTilesetBounds;
	sf::RectangleShape selectionTilesetBackground;
	sf::RectangleShape selectionTilesetSelection;

	// ----- FUNCTIONS ----- //

	Editor(sf::RenderWindow &_window, sf::View &_view);

	// view related
	void resetView();
	sf::Vector2f relativeViewPosition(float x, float y);

	// keyboard related
	bool pressed(sf::Keyboard::Key key);
	void processKeyboardInput();
	void handleKeyPresses();
	void handleTextEntered(sf::Event event);

	// mouse related
	void processMouseInput();
	void handleButtonPresses();
	void handleDrag();
	void adjustZoom();
	void handleZoom(sf::Event event);
	void updateViewBounds();
	void eyedropper();

	// map related
	sf::Vector3i determineTile(sf::Vector2i mapCoord, int tileset);
	sf::Vector3i getTile(unsigned int x, unsigned int y);
	int getTileset(unsigned int x, unsigned int y);
	sf::Vector2u getTileCrop(unsigned int x, unsigned int y);
	int getSelectedTileset();
	bool invalidTile(unsigned int x, unsigned int y);
	void placeTile(unsigned int x, unsigned int y, sf::Vector3i newTile);
	void adjustTile(unsigned int x, unsigned int y);
	void adjustAdjacentTiles(unsigned int x, unsigned int y);
	void clearMap(); // also works for initializing the map
	void saveMap();
	void changeMapSize(unsigned int newWidth, unsigned int newHeight);
	sf::Vector2f relativeMapPosition(float x, float y);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , int newTileset);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, int newTileset);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , sf::Vector3i newTile);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, sf::Vector3i newTile);
	void fillArea();
	void tiling();
	void updateSawblades();
	void drawMapBorder();
	void drawMapCheckers();
	void drawMapTiles();
	void drawMapRestrictedAreas();
	void drawMap();
	void drawGhostTiles();

	// tile selection related
	void selectTile();
	void drawSelectionTileset();

	// inputs related
	void updateSizeInputs();
	void clampSizeInputs();
	void drawSizeInputs();

	void update();
	void draw();
};
