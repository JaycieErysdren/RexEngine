// ========================================================
//
// FILE:			/source/rex3d/rend/spanner.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Spanner namespace implementation
//
// LAST EDITED:		December 1st, 2022
//
// ========================================================

// Rex3D headers
#include "rex3d/rex3d.hpp"

//
//
// Text macros
//
//

#define MAX_SPANS SCREEN_WIDTH

//
//
// Spanner namespace
//
//

// Spanner namespace definition (private)
namespace Spanner
{

//
//
// Types
//
//

typedef struct span_t span_t;
struct span_t
{
	int16_t x1, x2;
	span_t *next;
};

//
//
// Global variables
//
//

// Span pointers
static span_t spans[MAX_SPANS];
span_t *free_span_pointer = NULL;
span_t *span_buf_head = NULL;

// ??
uint16_t ytop[SCREEN_WIDTH] = {0};
uint16_t ybottom[SCREEN_WIDTH] = {SCREEN_HEIGHT - 1};

//
//
// Functions
//
//

//
// Rendering
//

void ClearClippingBuffers()
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		ytop[i] = 0;
		ybottom[i] = SCREEN_HEIGHT - 1;
	}
}

void DrawTwoSidedSpan(Picture::pic_t *dst, int16_t orig_x1, int16_t orig_x2,
					int16_t y1a, int16_t ny1a, int16_t y1b, int16_t ny1b, int16_t y2a, int16_t ny2a, int16_t y2b, int16_t ny2b,
					int16_t draw_x1, int16_t draw_x2, uint8_t ceil_col, uint8_t upper_col, uint8_t lower_col, uint8_t floor_col,
					uint8_t dither_wall, uint8_t dither_floor)
{
	draw_x2 -= 1;
	int16_t dx = orig_x2 - orig_x1;

	fix32_t fix_y1a = y1a<<16;
	fix32_t fix_y1b = y1b<<16;
	fix32_t fix_y2a = y2a<<16;
	fix32_t fix_y2b = y2b<<16;
	fix32_t fix_ny1a = ny1a<<16;
	fix32_t fix_ny1b = ny1b<<16;
	fix32_t fix_ny2a = ny2a<<16;
	fix32_t fix_ny2b = ny2b<<16;

	fix32_t top_dy = fix_y2a - fix_y1a;
	fix32_t bot_dy = fix_y2b - fix_y1b;
	fix32_t top_ndy = fix_ny2a - fix_ny1a;
	fix32_t bot_ndy = fix_ny2b - fix_ny1b;

	fix32_t top_slope = top_dy/dx;
	fix32_t top_nslope = top_ndy/dx;
	fix32_t bot_slope = bot_dy/dx;
	fix32_t bot_nslope = bot_ndy/dx;

	int16_t x1_diff = draw_x1 - orig_x1;
	fix32_t top_slope_diff = top_slope * x1_diff; // 16.16
	fix32_t bot_slope_diff = bot_slope * x1_diff;
	fix32_t top_nslope_diff = top_nslope * x1_diff;
	fix32_t bot_nslope_diff = bot_nslope * x1_diff;

	fix_y1a += top_slope_diff;
	fix_y1b += bot_slope_diff;
	fix_ny1a += top_nslope_diff;
	fix_ny1b += bot_nslope_diff;

	uint8_t lower_col2;
	uint8_t upper_col2;
	uint8_t floor_col2;
	uint8_t ceil_col2;

	#ifdef NOPE

	if (dither_wall)
	{
		lower_col2 = swap_nibbles(lower_col);
		upper_col2 = swap_nibbles(upper_col);
	}
	else
	{
		lower_col2 = lower_col;
		upper_col2 = upper_col;
	}

	if (dither_floor)
	{
		floor_col2 = swap_nibbles(floor_col);
		ceil_col2 = swap_nibbles(ceil_col);
	}
	else
	{
		floor_col2 = floor_col;
		ceil_col2 = ceil_col;
	}

	#else

	floor_col2 = floor_col;
	ceil_col2 = ceil_col;

	#endif

	for (int16_t x = draw_x1; x <= draw_x2; x++)
	{
		uint8_t border = (x == draw_x1 || x == draw_x2 || x == 0 || x == SCREEN_WIDTH - 1);
		int16_t cytop = ytop[x];
		int16_t cybottom = ybottom[x];

		int16_t ya = fix_y1a >> 16;
		int16_t cya = CLAMP(ya, cytop, cybottom);
		int16_t nya = fix_ny1a >> 16;
		int16_t cnya = CLAMP(nya, cytop, cybottom);

		int16_t yb = fix_y1b >> 16;
		int16_t cyb = CLAMP(yb, cytop, cybottom);
		int16_t nyb = fix_ny1b >> 16;
		int16_t cnyb = CLAMP(nyb, cytop, cybottom);

		if (cnyb < cya)
			cnyb = cya;

		if (cnya > cyb)
			cnya = cyb;


		#ifdef NOPE

		// draw ceiling
		vline_dither(x, cytop, cya, ceil_col, ceil_col2, fill ? 1 : 0);

		// draw floor
		vline_dither(x, cyb, cybottom, floor_col, floor_col2, fill ? 1 : 0);

		// draw step from ceiling
		vline_dither(x, cya, cnya, upper_col, upper_col2, fill ? 1 : border);

		// draw lower step
		vline_dither(x, cnyb, cyb, lower_col, lower_col2, fill ? 1 : border);

		#else

		// draw ceiling
		Picture::DrawVerticalLine(dst, x, cytop, cya, ceil_col);

		// draw floor
		Picture::DrawVerticalLine(dst, x, cyb, cybottom, floor_col);

		// draw step from ceiling
		Picture::DrawVerticalLine(dst, x, cya, cnya, upper_col);

		// draw lower step
		Picture::DrawVerticalLine(dst, x, cnyb, cyb, lower_col);

		#endif

		fix_y1a += top_slope;
		fix_y1b += bot_slope;
		fix_ny1a += top_nslope;
		fix_ny1b += bot_nslope;
		ytop[x] = CLAMP(MAX(cya, cnya), cytop, SCREEN_HEIGHT - 1);
		ybottom[x] = CLAMP(MIN(cyb, cnyb), 0, cybottom);
	}
}

void DrawOneSidedSpan(Picture::pic_t *dst, int16_t orig_x1, int16_t orig_x2,
						int16_t y1a, int16_t y1b, int16_t y2a, int16_t y2b,
						int16_t draw_x1, int16_t draw_x2,
						uint8_t ceil_col, uint8_t wall_col, uint8_t floor_col,
						uint8_t dither_wall, uint8_t dither_floor)
{
	draw_x2 -= 1;
	int16_t dx = orig_x2 - orig_x1;

	fix32_t fix_y1a = y1a << 8;
	fix32_t fix_y1b = y1b << 8;
	fix32_t fix_y2a = y2a << 8;
	fix32_t fix_y2b = y2b << 8;

	fix32_t top_dy = fix_y2a - fix_y1a;
	fix32_t bot_dy = fix_y2b - fix_y1b;

	fix32_t top_slope = top_dy/dx;
	fix32_t bot_slope = bot_dy/dx;

	int16_t x1_diff = draw_x1 - orig_x1;
	fix32_t top_slope_diff = top_slope * x1_diff; // 16.16
	fix32_t bot_slope_diff = bot_slope * x1_diff;

	fix_y1a += top_slope_diff;
	fix_y1b += bot_slope_diff;

	uint8_t wall_col2;
	uint8_t floor_col2;
	uint8_t ceil_col2;

	#ifdef NOPE

	if (dither_wall)
		wall_col2 = swap_nibbles(wall_col);
	else
		wall_col2 = wall_col;

	if (dither_floor)
	{
		floor_col2 = swap_nibbles(floor_col);
		ceil_col2 = swap_nibbles(ceil_col);
	}
	else
	{
		floor_col2 = floor_col;
		ceil_col2 = ceil_col;
	}

	#else

	floor_col2 = floor_col;
	ceil_col2 = ceil_col;

	#endif

	//uint8_t* bmp_ptr
	for(int16_t x = draw_x1; x <= draw_x2; x++)
	{
		uint8_t border = (x == draw_x1 || x == draw_x2 || x == 0 || x == SCREEN_WIDTH - 1);
		int16_t cytop = ytop[x];
		int16_t cybottom = ybottom[x];

		int16_t ya = fix_y1a >> 8;
		int16_t cya = CLAMP(ya, cytop, cybottom);

		int16_t yb = fix_y1b >> 8;
		int16_t cyb = CLAMP(yb, cytop, cybottom);

		#ifdef NOPE

		// draw ceiling
		vline_dither(x, cytop, cya-1, ceil_col, ceil_col2, fill ? 1 : 0);

		// draw floor
		vline_dither(x, cyb+1, cybottom, floor_col, floor_col2, fill ? 1 : 0);

		// draw wall
		vline_dither(x, cya, cyb, wall_col, wall_col2, fill ? 1 : border);

		#else

		// draw ceiling
		Picture::DrawVerticalLine(dst, x, cytop, cya - 1, ceil_col);

		// draw floor
		Picture::DrawVerticalLine(dst, x, cyb + 1, cybottom, floor_col);

		// draw wall
		Picture::DrawVerticalLine(dst, x, cya, cyb, wall_col);

		#endif

		ytop[x] = CLAMP(cya, cytop, SCREEN_HEIGHT - 1);
		ybottom[x] = CLAMP(cyb, 0, cybottom);

		fix_y1a += top_slope;
		fix_y1b += bot_slope;
	}
}

void DrawSpan(Picture::pic_t *dst, int16_t orig_x1, int16_t orig_x2,
				int16_t y1a, int16_t ny1a, int16_t y1b, int16_t ny1b, int16_t y2a, int16_t ny2a, int16_t y2b, int16_t ny2b,
				int16_t draw_x1, int16_t draw_x2,
				uint8_t ceil_col, uint8_t upper_col, uint8_t wall_col, uint8_t lower_col, uint8_t floor_col,
				uint8_t update_vertical_clipping,
				uint8_t dither_wall, uint8_t dither_floor)
{
	if (update_vertical_clipping)
		DrawTwoSidedSpan(dst, orig_x1, orig_x2, y1a, ny1a, y1b, ny1b, y2a, ny2a, y2b, ny2b, draw_x1, draw_x2, ceil_col, upper_col, lower_col, floor_col, dither_wall, dither_floor);
	else
		DrawOneSidedSpan(dst, orig_x1, orig_x2, y1a, y1b, y2a, y2b, draw_x1, draw_x2, ceil_col, wall_col, floor_col, dither_wall, dither_floor);
}

//
// Span
//

static span_t *SpanAllocate() {

	if (free_span_pointer != NULL)
	{
		span_t *sp = free_span_pointer;
		free_span_pointer = sp->next;
		return sp;
	}

	while (1)
	{
		cout << "Out of spans!" << endl;
	}

	return NULL;
}

static void SpanFree(span_t *sp)
{
	sp->x1 = -1;
	sp->x2 = -1;
	sp->next = free_span_pointer;
	free_span_pointer = sp;
}

int InsertSpan(Picture::pic_t *dst, int16_t x1, int16_t x2, int16_t y1a, int16_t ny1a, int16_t y2a, int16_t ny2a, int16_t y1b, int16_t ny1b, int16_t y2b, int16_t ny2b,
				uint8_t ceil_col, uint8_t upper_col, uint8_t wall_col, uint8_t lower_col, uint8_t floor_col,
				uint8_t insert, uint8_t dither_wall, uint8_t dither_floor)
{
	int16_t orig_x1 = x1;
	int16_t orig_x2 = x2;
	span_t *old, *current, *n;
	uint8_t update_vert_clipping = !insert;

	if(span_buf_head == NULL) { return 1; }

	for (old = NULL, current = span_buf_head; current != NULL; old = current, current = current->next)
	{

		// case 1
		if (current->x2 <= x1)
		{
			continue;
		}

		if (current->x1 < x1)
		{
			if (current->x2 <= x2) // case 2
			{
				DrawSpan(dst, orig_x1, orig_x2, y1a, ny1a, y1b, ny1b, y2a, ny2a, y2b, ny2b, x1, current->x2, ceil_col, upper_col, wall_col, lower_col, floor_col, update_vert_clipping, dither_wall, dither_floor);

				if (insert)
					current->x2 = x1;
			}
			else // case 3
			{
				DrawSpan(dst, orig_x1, orig_x2, y1a, ny1a, y1b, ny1b, y2a, ny2a, y2b, ny2b, x1, x2, ceil_col, upper_col, wall_col, lower_col, floor_col, update_vert_clipping, dither_wall, dither_floor);

				if (insert)
				{
						n = SpanAllocate();
						n->next = current->next;
						current->next = n;
						n->x2 = current->x2;
						current->x2 = x1;
						n->x1 = x2;
				}

				return 0;
			}
		}
		else
		{
			if (current->x1>=x2) // case 6
				return 0;

			if (current->x2<=x2) // case 4
			{

				DrawSpan(dst, orig_x1, orig_x2, y1a, ny1a, y1b, ny1b, y2a, ny2a, y2b, ny2b, current->x1,current->x2, ceil_col, upper_col, wall_col, lower_col, floor_col, update_vert_clipping, dither_wall, dither_floor);

				n = current->next;

				if(insert)
				{
					SpanFree(current);

					if (old)
						old->next=n;
					else
						span_buf_head = n;
				}

				current = n;

				if (current==NULL)
					return 0;
			}
			else // case 5
			{
				DrawSpan(dst, orig_x1, orig_x2, y1a, ny1a, y1b, ny1b, y2a, ny2a, y2b, ny2b, current->x1, x2, ceil_col, upper_col, wall_col, lower_col, floor_col, update_vert_clipping, dither_wall, dither_floor);

				if (insert)
					current->x1 = x2;
			}
		}
	}

	return 0;
}

void ClearSpanBuffer()
{
	for (int i = 0; i < MAX_SPANS-1; i++)
	{
		spans[i].next = &(spans[i + 1]);
	}

	spans[MAX_SPANS - 1].next = NULL;

	free_span_pointer = &(spans[0]);

	// initialize span buffer to contain the whole screen
	span_buf_head = SpanAllocate();
	span_buf_head->x1 = 0;
	span_buf_head->x2 = SCREEN_WIDTH - 1;
	span_buf_head->next = NULL;
}

} // namespace Spanner
