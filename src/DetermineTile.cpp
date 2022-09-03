#include <SFML/Graphics.hpp>

#include "headers/Editor.hpp"

bool sameTile(int tile0, int tile1)
{
	if (tile0 == tile1)
	{
		return true;
	}

	return false;
}

std::vector<std::vector<int>> region(int TL, int TC, int TR, int ML, int MC, int MR, int BL, int BC, int BR)
{
	return
	{
		{TL, TC, TR},
		{ML, MC, MR},
		{BL, BC, BR}
	};
}

bool equalRegion(std::vector<std::vector<int>> region0, std::vector<std::vector<int>> region1)
{
	for (unsigned int x = 0; x < 3; x++)
	{
		for (unsigned int y = 0; y < 3; y++)
		{
			if (region1.at(x).at(y) != 2)
			{
				if (region0.at(x).at(y) != region1.at(x).at(y))
				{
					return false;
				}
			}
		}
	}

	return true;
}

sf::Vector3i Editor::determineTile(sf::Vector2i mapCoord, int tileset)
{
	sf::Vector2u crop;

	if (tileset < 4)
	{
		// T = top;  M = middle; B = bottom;
		// L = left; C = center; R = right;
		int TL = 1;
		int TC = 1;
		int TR = 1;
		int ML = 1;
		int MC = 1;
		int MR = 1;
		int BL = 1;
		int BC = 1;
		int BR = 1;

		if (mapCoord.y > 0)
		{
			if (mapCoord.x > 0)
			{
				TL = sameTile(getTileset(mapCoord.x - 1, mapCoord.y - 1), tileset);
			}
				TC = sameTile(getTileset(mapCoord.x + 0, mapCoord.y - 1), tileset);
			if (mapCoord.x < mapSize.x - 1)
			{
				TR = sameTile(getTileset(mapCoord.x + 1, mapCoord.y - 1), tileset);
			}
		}

			if (mapCoord.x > 0)
			{
				ML = sameTile(getTileset(mapCoord.x - 1, mapCoord.y + 0), tileset);
			}
				MC = sameTile(getTileset(mapCoord.x + 0, mapCoord.y + 0), tileset);
			if (mapCoord.x < mapSize.x - 1)
			{
				MR = sameTile(getTileset(mapCoord.x + 1, mapCoord.y + 0), tileset);
			}

		if (mapCoord.y < mapSize.y - 1)
		{
			if (mapCoord.x > 0)
			{
				BL = sameTile(getTileset(mapCoord.x - 1, mapCoord.y + 1), tileset);
			}
				BC = sameTile(getTileset(mapCoord.x + 0, mapCoord.y + 1), tileset);
			if (mapCoord.x < mapSize.x - 1)
			{
				BR = sameTile(getTileset(mapCoord.x + 1, mapCoord.y + 1), tileset);
			}
		}

		std::vector<std::vector<int>> tileRegion = region(TL, TC, TR,
	                                                      ML, MC, MR,
	                                                      BL, BC, BR);

		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   0, 1, 1,
		                                   2, 1, 1)))
		{
			crop.x = 0;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 1,
		                                   1, 1, 1)))
		{
			crop.x = 1;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 0,
		                                   1, 1, 2)))
		{
			crop.x = 2;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   0, 1, 0,
		                                   2, 1, 2)))
		{
			crop.x = 3;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 1,
		                                   0, 1, 1,
		                                   2, 1, 0)))
		{
			crop.x = 4;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 2,
		                                   1, 1, 0,
		                                   0, 1, 2)))
		{
			crop.x = 5;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 1,
		                                   1, 1, 1,
		                                   1, 1, 0)))
		{
			crop.x = 6;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 1,
		                                   1, 1, 1,
		                                   0, 1, 1)))
		{
			crop.x = 7;
			crop.y = 0;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 1,
		                                   0, 1, 1,
		                                   2, 1, 1)))
		{
			crop.x = 0;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 1,
		                                   1, 1, 1,
		                                   1, 1, 1)))
		{
			crop.x = 1;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 2,
		                                   1, 1, 0,
		                                   1, 1, 2)))
		{
			crop.x = 2;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 2,
		                                   0, 1, 0,
		                                   2, 1, 2)))
		{
			crop.x = 3;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 0,
		                                   0, 1, 1,
		                                   2, 1, 0)))
		{
			crop.x = 4;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 2,
		                                   1, 1, 0,
		                                   0, 1, 2)))
		{
			crop.x = 5;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 0,
		                                   1, 1, 1,
		                                   1, 1, 1)))
		{
			crop.x = 6;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 1,
		                                   1, 1, 1,
		                                   1, 1, 1)))
		{
			crop.x = 7;
			crop.y = 1;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 1,
		                                   0, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 0;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 1,
		                                   1, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 1;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 2,
		                                   1, 1, 0,
		                                   2, 0, 2)))
		{
			crop.x = 2;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 2,
		                                   0, 1, 0,
		                                   2, 0, 2)))
		{
			crop.x = 3;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 0,
		                                   0, 1, 1,
		                                   2, 1, 1)))
		{
			crop.x = 4;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 2,
		                                   1, 1, 0,
		                                   1, 1, 2)))
		{
			crop.x = 5;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 0,
		                                   1, 1, 1,
		                                   0, 1, 1)))
		{
			crop.x = 6;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 0,
		                                   1, 1, 1,
		                                   1, 1, 0)))
		{
			crop.x = 7;
			crop.y = 2;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   0, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 0;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 1;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 0,
		                                   2, 0, 2)))
		{
			crop.x = 2;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   0, 1, 0,
		                                   2, 0, 2)))
		{
			crop.x = 3;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 0,
		                                   1, 1, 1,
		                                   1, 1, 0)))
		{
			crop.x = 4;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 1,
		                                   1, 1, 1,
		                                   0, 1, 1)))
		{
			crop.x = 5;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 1,
		                                   1, 1, 1,
		                                   0, 1, 0)))
		{
			crop.x = 6;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 0,
		                                   1, 1, 1,
		                                   0, 1, 0)))
		{
			crop.x = 7;
			crop.y = 3;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 1,
		                                   1, 1, 0)))
		{
			crop.x = 0;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 1,
		                                   0, 1, 0)))
		{
			crop.x = 1;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 1,
		                                   0, 1, 1)))
		{
			crop.x = 2;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 1,
		                                   1, 1, 1,
		                                   0, 1, 0)))
		{
			crop.x = 3;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   0, 1, 1,
		                                   2, 1, 0)))
		{
			crop.x = 4;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(2, 0, 2,
		                                   1, 1, 0,
		                                   0, 1, 2)))
		{
			crop.x = 5;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 1,
		                                   1, 1, 1,
		                                   1, 1, 0)))
		{
			crop.x = 6;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 0,
		                                   1, 1, 1,
		                                   0, 1, 1)))
		{
			crop.x = 7;
			crop.y = 4;
		}
		else
		if (equalRegion(tileRegion, region(1, 1, 0,
		                                   1, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 0;
			crop.y = 5;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 0,
		                                   1, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 1;
			crop.y = 5;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 1,
		                                   1, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 2;
			crop.y = 5;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 0,
		                                   1, 1, 1,
		                                   1, 1, 1)))
		{
			crop.x = 3;
			crop.y = 5;
		}
		else
		if (equalRegion(tileRegion, region(2, 1, 0,
		                                   0, 1, 1,
		                                   2, 0, 2)))
		{
			crop.x = 4;
			crop.y = 5;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 2,
		                                   1, 1, 0,
		                                   2, 0, 2)))
		{
			crop.x = 5;
			crop.y = 5;
		}
		else
		if (equalRegion(tileRegion, region(0, 1, 0,
		                                   1, 1, 1,
		                                   0, 1, 0)))
		{
			crop.x = 6;
			crop.y = 5;
		}
		else // null
		{
			crop.x = 7;
			crop.y = 5;
		}
	}
	else
	{
		crop = selectionCoord;
	}

	return sf::Vector3i(tileset, crop.x, crop.y);
}
