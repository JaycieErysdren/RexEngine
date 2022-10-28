// ========================================================
//
// FILE:			/source/core/rendering/nuklear.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Nuklear helpers.
//
// LAST EDITED:		October 27th, 2022
//
// ========================================================

// Include Rex Engine header
#include "rex.h"

// Include Nuklear header
#define NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_SOFTWARE_FONT
#include "nuklear.h"

// Rex Nuklear Context
struct rex_nuklear_context {
	struct nk_context ctx;
	struct nk_rect scissors;
	struct br_pixelmap *pm;
	struct br_pixelmap *font_tex;
	struct nk_font_atlas atlas;
};

//
// Private functions
//

// Set pixel on the rendering pixelmap
static void nk_brender_setpixel(const struct rex_nuklear_context *context, const rex_short x, const rex_short y, const struct nk_color col)
{
	BrPixelmapPixelSet(context->pm, x, y, BR_COLOUR_RGB(col.r, col.g, col.b));
}

// Draw a horizontal line on a BRender pixelmap
static void nk_brender_line_horizontal(const struct rex_nuklear_context *context, const rex_short x0, const short y, const short x1, const struct nk_color col)
{
	BrPixelmapLine(context->pm, x0, y, x1, y, BR_COLOUR_RGB(col.r, col.g, col.b));
}

// Set pixel on an arbitrary pixelmap
static void nk_pixelmap_setpixel(br_pixelmap *pm, const int x, const int y, const struct nk_color col)
{
	BrPixelmapPixelSet(pm, x, y, BR_COLOUR_RGB(col.r, col.g, col.b));
}

// Set pixel from an arbitrary pixelmap
static struct nk_color nk_pixelmap_getpixel(br_pixelmap *pm, const int x, const int y)
{
	br_colour src = BrPixelmapPixelGet(pm, x, y);
	return (struct nk_color){BR_RED(src), BR_GRN(src), BR_BLU(src), 255};
}

// Set and blend the pixel on an arbitrary pixelmap
static void nk_pixelmap_blendpixel(br_pixelmap *pm, const int x, const int y, struct nk_color col)
{
	struct nk_color col2;
	rex_ubyte inv_a;

	if (col.a == 0)
		return;

	inv_a = 0xff - col.a;
	col2 = nk_pixelmap_getpixel(pm, x, y);
	col.r = (col.r * col.a + col2.r * inv_a) >> 8;
	col.g = (col.g * col.a + col2.g * inv_a) >> 8;
	col.b = (col.b * col.a + col2.b * inv_a) >> 8;
	nk_pixelmap_setpixel(pm, x, y, col);
}

// Set scissor area
static void nk_brender_scissor(struct rex_nuklear_context *context, const rex_float x, const rex_float y, const rex_float w, const rex_float h)
{
	context->scissors.x = REX_MIN(REX_MAX(x, 0), context->pm->width);
	context->scissors.y = REX_MIN(REX_MAX(y, 0), context->pm->height);
	context->scissors.w = REX_MIN(REX_MAX(w + x, 0), context->pm->width);
	context->scissors.h = REX_MIN(REX_MAX(h + y, 0), context->pm->height);
}

// Draw a line with stroke
static void nk_brender_stroke_line(const struct rex_nuklear_context *context, rex_short x0, rex_short y0, rex_short x1, rex_short y1, const rex_uint line_thickness, const struct nk_color col)
{
	rex_short tmp;
	rex_int dy, dx, stepx, stepy;

	NK_UNUSED(line_thickness);

	dy = y1 - y0;
	dx = x1 - x0;

	/* fast path */
	if (dy == 0)
	{
		if (dx == 0 || y0 >= context->scissors.h || y0 < context->scissors.y)
			return;

		if (dx < 0)
		{
			/* swap x0 and x1 */
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		x1 = REX_MIN(context->scissors.w, x1);
		x0 = REX_MIN(context->scissors.w, x0);
		x1 = REX_MAX(context->scissors.x, x1);
		x0 = REX_MAX(context->scissors.x, x0);

		nk_brender_line_horizontal(context, x0, y0, x1, col);

		return;
	}

	if (dy < 0)
	{
		dy = -dy;
		stepy = -1;
	}
	else
	{
		stepy = 1;
	}

	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
	{
		stepx = 1;
	}

	dy <<= 1;
	dx <<= 1;

	nk_brender_setpixel(context, x0, y0, col);

	if (dx > dy)
	{
		rex_int fraction = dy - (dx >> 1);

		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx;
			}

			x0 += stepx;
			fraction += dy;

			nk_brender_setpixel(context, x0, y0, col);
		}
	}
	else
	{
		rex_int fraction = dx - (dy >> 1);

		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}

			y0 += stepy;
			fraction += dx;

			nk_brender_setpixel(context, x0, y0, col);
		}
	}
}

static void nk_brender_fill_polygon(const struct rex_nuklear_context *context, const struct nk_vec2i *pnts, rex_int count, const struct nk_color col)
{
	rex_int i = 0;
	rex_int max_points = 64;
	rex_int left = 10000, top = 10000, bottom = 0, right = 0;
	rex_int nodes, nodeX[max_points], pixelX, pixelY, j, swap ;

	if (count == 0) return;
	if (count > max_points)
		count = max_points;

	/* Get polygon dimensions */
	for (i = 0; i < count; i++)
	{
		if (left > pnts[i].x)
			left = pnts[i].x;
		if (right < pnts[i].x)
			right = pnts[i].x;
		if (top > pnts[i].y)
			top = pnts[i].y;
		if (bottom < pnts[i].y)
			bottom = pnts[i].y;
	}

	bottom++;
	right++;

	/* Polygon scanline algorithm released under public-domain by Darel Rex Finley, 2007 */
	/*  Loop through the rows of the image. */
	for (pixelY = top; pixelY < bottom; pixelY++)
	{
		nodes = 0; /*  Build a list of nodes. */
		j = count - 1;

		for (i = 0; i < count; i++)
		{
			if (((pnts[i].y < pixelY) && (pnts[j].y >= pixelY)) || ((pnts[j].y < pixelY) && (pnts[i].y >= pixelY)))
			{
				nodeX[nodes++] = (rex_int)((rex_float)pnts[i].x
					+ ((rex_float)pixelY - (rex_float)pnts[i].y) / ((rex_float)pnts[j].y - (rex_float)pnts[i].y)
					* ((rex_float)pnts[j].x - (rex_float)pnts[i].x));
			}

			j = i;
		}

		/*  Sort the nodes, via a simple “Bubble” sort. */
		i = 0;
		while (i < nodes - 1)
		{
			if (nodeX[i] > nodeX[i+1])
			{
				swap = nodeX[i];
				nodeX[i] = nodeX[i+1];
				nodeX[i+1] = swap;

				if (i)
					i--;
			}
			else
			{
				i++;
			}
		}

		/*  Fill the pixels between node pairs. */
		for (i = 0; i < nodes; i += 2)
		{
			if (nodeX[i+0] >= right)
				break;

			if (nodeX[i+1] > left)
			{
				if (nodeX[i+0] < left) nodeX[i+0] = left ;
				if (nodeX[i+1] > right) nodeX[i+1] = right;
				for (pixelX = nodeX[i]; pixelX < nodeX[i + 1]; pixelX++)
				{
					nk_brender_setpixel(context, pixelX, pixelY, col);
				}
			}
		}
	}
}

//
// Public functions
//


