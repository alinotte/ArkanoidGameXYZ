#pragma once
#include <string>

namespace Arkanoid
{
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";
	const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
	const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
	const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";

	// Game settings constants
	const float PADDLE_WIDTH = 100.f;
	const float PADDLE_HEIGHT = 20.f;
	const float PADDLE_SPEED = 400.f;
	const float PADDLE_BOTTOM_MARGIN = 20.f;
	const float BALL_RADIUS = 10.f;
	const float BALL_INITIAL_SPEED = 300.f;
	const float MAX_BOUNCE_ANGLE = 1.30899694f; // 75 degrees in radians


	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;
	const float TIME_PER_FRAME = 1.f / 60.f; // 60 fps

	const int MAX_RECORDS_TABLE_SIZE = 5;
	extern const char* PLAYER_NAME; // We need to define this constant in some SPP
}
