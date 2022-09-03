#pragma once

const unsigned int FPS = 60;

const unsigned int tilesize = 16;

const unsigned int viewWidth    = tilesize * 16;
const unsigned int viewHeight   = tilesize * 16;
const unsigned int windowWidth  = viewWidth  * 2;
const unsigned int windowHeight = viewHeight * 2;

void toggle(bool &boolean);
