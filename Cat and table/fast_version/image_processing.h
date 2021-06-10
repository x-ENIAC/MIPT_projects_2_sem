#include <SFML/Graphics.hpp>
#include <emmintrin.h>
#include <immintrin.h>

enum Statuses_type {
	ALL_IS_OKEY  	    = 0,
	ESCAPE_PRESSED 	    = 1,
	BAD_POINTERS        = 2,
	TEXTURE_NOT_CREATED = 4,
	BAD_PICTURES_SIZE   = 8,
};

const char TEXT_STATUSES[][80] {
    "All is okey",
    "Escape pressed",
    "There was a confusion with pointers",
    "",
    "Texture not created",
    "Your pictures have bad size"
};

struct Colour {
	unsigned char alpha;	
	unsigned char red;		
	unsigned char green;	
	unsigned char blue;
};

struct Screen_type {
	Colour* pixels;
	int wigth_screen;
	int height_screen;
};

const int PACK = 4;
const int COUNT_REPEATS_IN_MEASUREMENTS = 2000;
const int MAX_COLOUR = 256;

const int X_OFFSET = 300;
const int Y_OFFSET = 215;

const __m128i CONSTANT_256 = _mm_set1_epi16(256);

const __m128i LOWER_MASK = _mm_set_epi8(-1, 7, -1, 6,
									    -1, 5, -1, 4,
										-1, 3, -1, 2,
										-1, 1, -1, 0);

const __m128i UPPER_MASK = _mm_set_epi8(-1, 15, -1, 14,
										-1, 13, -1, 12,
										-1, 11, -1, 10,
										-1,  9, -1,  8);	

const __m128i LOWER_MASK_FOR_ALPHA = _mm_set_epi8(-1,  4, -1, 4,
												  -1,  4, -1, 4,
												  -1,  0, -1, 0,
												  -1,  0, -1, 0);

const __m128i UPPER_MASK_FOR_ALPHA = _mm_set_epi8(-1, 12, -1, 12,
												  -1, 12, -1, 12,
												  -1,  8, -1,  8,
												  -1,  8, -1,  8);												

const __m128i MASK_FOR_RESULT = _mm_set_epi8(15, 13, 11,  9,
											  7,  5,  3,  1,
											 -1, -1, -1, -1,
											 -1, -1, -1, -1);

Screen_type* screen_new(const char* picture_name);

Statuses_type screen_construct(Screen_type* screen, const char* picture_name);

inline Statuses_type set_pixel_color(Screen_type* screen, const int line, const int column, const sf::Color color_now_pixel);

Statuses_type screen_delete(Screen_type* screen);

Statuses_type start_overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture);

inline Statuses_type measurements(Screen_type* background_picture, Screen_type* foreground_picture);

inline Statuses_type overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture, const int y_offset, const int x_offset);

inline bool is_correct_pictures_size(Screen_type* background_picture, Screen_type* foreground_picture);

Statuses_type show_result_image(Screen_type* background_picture, sf::Sprite* sprite, sf::Texture* texture);

void display_picture(sf::Sprite* sprite, const int wigth_screen, const int height_screen);