#include <SFML/Graphics.hpp>

enum Statuses_type {
	ALL_IS_OKEY    = 0,
	ESCAPE_PRESSED = 1,
	BAD_POINTERS   = 2,
};

struct Colour {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

struct Screen_type {
	Colour** pixels;
	int wigth_screen;
	int height_screen;
};

Screen_type* screen_new(const char* picture_name);

Statuses_type screen_construct(Screen_type* screen, const char* picture_name);

Statuses_type set_pixel_color(Colour** pixels, const int line, const int column, sf::Color color_now_pixel);