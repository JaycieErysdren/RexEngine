// ========================================================
//
// FILE:			/source/core/rendering/fonts.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Embeded system fonts.
//
// LAST EDITED:		October 28th, 2022
//
// ========================================================

// Include engine header
#include "rex.h"

// Include embedded fonts
#include "embedded/ModernDOS8x8.h"
#include "embedded/ModernDOS8x14.h"
#include "embedded/ModernDOS8x16.h"
#include "embedded/ModernDOS9x14.h"
#include "embedded/ModernDOS9x16.h"

TTF_Font *font_dos_8x8;			// DOS standard font, 8x8
TTF_Font *font_dos_8x14;		// DOS standard font, 8x14
TTF_Font *font_dos_8x16;		// DOS standard font, 8x16
TTF_Font *font_dos_9x14;		// DOS standard font, 9x14
TTF_Font *font_dos_9x16;		// DOS standard font, 9x16

// Start up system fonts
void Rex_Fonts_Startup(void)
{
	if (TTF_Init() != 0)
		Rex_Failure("TTF_Init() failed: %s", SDL_GetError());

	SDL_RWops *_font_dos_8x8 = SDL_RWFromConstMem(ModernDOS8x8_ttf, ModernDOS8x8_ttf_len);
	SDL_RWops *_font_dos_8x14 = SDL_RWFromConstMem(ModernDOS8x14_ttf, ModernDOS8x14_ttf_len);
	SDL_RWops *_font_dos_8x16 = SDL_RWFromConstMem(ModernDOS8x16_ttf, ModernDOS8x16_ttf_len);
	SDL_RWops *_font_dos_9x14 = SDL_RWFromConstMem(ModernDOS9x14_ttf, ModernDOS9x14_ttf_len);
	SDL_RWops *_font_dos_9x16 = SDL_RWFromConstMem(ModernDOS9x16_ttf, ModernDOS9x16_ttf_len);

	//font_dos_8x8 = TTF_OpenFont("../fonts/ModernDOS8x8.ttf", 8);
	//font_dos_8x14 = TTF_OpenFont("../fonts/ModernDOS8x14.ttf", 28);
	//font_dos_8x16 = TTF_OpenFont("../fonts/ModernDOS8x16.ttf", 32);
	//font_dos_9x14 = TTF_OpenFont("../fonts/ModernDOS9x14.ttf", 28);
	//font_dos_9x16 = TTF_OpenFont("../fonts/ModernDOS9x16.ttf", 32);

	font_dos_8x8 = TTF_OpenFontRW(_font_dos_8x8, REX_TRUE, 16);
	font_dos_8x14 = TTF_OpenFontRW(_font_dos_8x14, REX_TRUE, 14);
	font_dos_8x16 = TTF_OpenFontRW(_font_dos_8x16, REX_TRUE, 16);
	font_dos_9x14 = TTF_OpenFontRW(_font_dos_9x14, REX_TRUE, 14);
	font_dos_9x16 = TTF_OpenFontRW(_font_dos_9x16, REX_TRUE, 16);

	//if (font_dos_8x8 == NULL) Rex_Failure("Failed to load /fonts/ModernDOS8x8.ttf");
	//if (font_dos_8x14 == NULL) Rex_Failure("Failed to load /fonts/ModernDOS8x14.ttf");
	//if (font_dos_8x16 == NULL) Rex_Failure("Failed to load /fonts/ModernDOS8x16.ttf");
	//if (font_dos_9x14 == NULL) Rex_Failure("Failed to load /fonts/ModernDOS9x14.ttf");
	//if (font_dos_9x16 == NULL) Rex_Failure("Failed to load /fonts/ModernDOS9x16.ttf");
}

// Shut down system fonts
void Rex_Fonts_Shutdown(void)
{
	TTF_CloseFont(font_dos_8x8);
	TTF_CloseFont(font_dos_8x14);
	TTF_CloseFont(font_dos_8x16);
	TTF_CloseFont(font_dos_9x14);
	TTF_CloseFont(font_dos_9x16);

	TTF_Quit();
}

// Generate an SDL Surface which contains a message's pixel data
void Rex_Fonts_GenerateTextSurface(SDL_Surface **surface, TTF_Font *font, rex_rgba color, rex_byte *s, ...)
{
	if (*surface != NULL) SDL_FreeSurface(*surface);

	va_list args;
	char text[512];

	va_start(args, s);
	vsprintf(text, s, args);
	va_end(args);

	*surface = TTF_RenderText_Blended_Wrapped(font, text, (SDL_Color){color.r, color.g, color.b, color.a}, rex_desktop_size[0]);
}

// Render text to the given Rex window
void Rex_Fonts_RenderText(rex_window *window, rex_int x, rex_int y, rex_rgba color, TTF_Font *font, rex_byte *s, ...)
{
	SDL_Surface *surface = TTF_RenderText_Blended(font, s, (SDL_Color){color.r, color.g, color.b, color.a});

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, window->width, window->height, 0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(x, y);
		glTexCoord2i(1, 0); glVertex2i(x + surface->w, y);
		glTexCoord2i(1, 1); glVertex2i(x + surface->w, y + surface->h);
		glTexCoord2i(0, 1); glVertex2i(x, y + surface->h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	SDL_FreeSurface(surface);
}

// Generate a BRender Pixelmap which contains a formatted message's pixel data.
br_pixelmap *Rex_Fonts_GenerateTextPixelmap(TTF_Font *font, rex_rgba color, rex_byte *s, ...)
{
	va_list args;
	rex_byte text[512];

	va_start(args, s);
	vsprintf(text, s, args);
	va_end(args);

	SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, (SDL_Color){color.r, color.g, color.b, color.a}, rex_desktop_size[0]);
	br_pixelmap *pixelmap = BrPixelmapAllocate(BR_PMT_ARGB_8888, surface->w, surface->h, surface->pixels, REX_FALSE);

	return pixelmap;
}
