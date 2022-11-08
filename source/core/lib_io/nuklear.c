// ========================================================
//
// FILE:			/source/core/lib_io/nuklear.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Nuklear helpers.
//
// LAST EDITED:		November 7th, 2022
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

//
// Private functions
//

// Set pixel on the rendering pixelmap
static void nk_brender_setpixel(const rex_nuklear_context *context, const rex_short x, const rex_short y, const struct nk_color col)
{
	BrPixelmapRectangleFill(context->pm, x - (context->pm->width / 2), y - (context->pm->height / 2), 1, 1, BR_COLOUR_RGB(col.r, col.g, col.b));
}

// Draw a horizontal line on a BRender pixelmap
static void nk_brender_line_horizontal(const rex_nuklear_context *context, const rex_short x0, const rex_short y0, const rex_short x1, const struct nk_color col)
{
	BrPixelmapRectangleFill(context->pm, x0 - (context->pm->width / 2), y0 - (context->pm->height / 2), x1 - x0, 1, BR_COLOUR_RGB(col.r, col.g, col.b));
}

// Set pixel on an arbitrary pixelmap
static void nk_pixelmap_setpixel(br_pixelmap *pm, const rex_int x, const rex_int y, const struct nk_color col)
{
	BrPixelmapRectangleFill(pm, x - (pm->width / 2), y - (pm->height / 2), 1, 1, BR_COLOUR_RGB(col.r, col.g, col.b));
}

// Set pixel from an arbitrary pixelmap
static struct nk_color nk_pixelmap_getpixel(br_pixelmap *pm, const rex_int x, const rex_int y)
{
	br_colour src = BrPixelmapPixelGet(pm, x, y);
	return (struct nk_color){BR_RED(src), BR_GRN(src), BR_BLU(src), BR_ALPHA(src)};
}

// Set and blend the pixel on an arbitrary pixelmap
static void nk_pixelmap_blendpixel(br_pixelmap *pm, const rex_int x, const rex_int y, struct nk_color col)
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
static void nk_brender_scissor(rex_nuklear_context *context, const rex_float x, const rex_float y, const rex_float w, const rex_float h)
{
	context->scissors.x = REX_MIN(REX_MAX(x, 0), context->pm->width);
	context->scissors.y = REX_MIN(REX_MAX(y, 0), context->pm->height);
	context->scissors.w = REX_MIN(REX_MAX(w + x, 0), context->pm->width);
	context->scissors.h = REX_MIN(REX_MAX(h + y, 0), context->pm->height);
}

// Draw a line with stroke
static void nk_brender_stroke_line(const rex_nuklear_context *context, rex_short x0, rex_short y0, rex_short x1, rex_short y1, const rex_uint line_thickness, const struct nk_color col)
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

// Fill a polygon
static void nk_brender_fill_polygon(const rex_nuklear_context *context, const struct nk_vec2i *pnts, rex_int count, const struct nk_color col)
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

static void nk_brender_stroke_arc(const rex_nuklear_context *context, rex_short x0, rex_short y0, rex_short w, rex_short h, const rex_short s, const rex_short line_thickness, const struct nk_color col)
{
	/* Bresenham's ellipses - modified to draw one quarter */
	const rex_int a2 = (w * w) / 4;
	const rex_int b2 = (h * h) / 4;
	const rex_int fa2 = 4 * a2, fb2 = 4 * b2;
	int x, y, sigma;

	NK_UNUSED(line_thickness);

	if (s != 0 && s != 90 && s != 180 && s != 270)
		return;

	if (w < 1 || h < 1)
		return;

	/* Convert upper left to center */
	h = (h + 1) / 2;
	w = (w + 1) / 2;
	x0 += w; y0 += h;

	/* First half */
	for (x = 0, y = h, sigma = 2*b2+a2*(1-2*h); b2*x <= a2*y; x++)
	{
		if (s == 180)
			nk_brender_setpixel(context, x0 + x, y0 + y, col);
		else if (s == 270)
			nk_brender_setpixel(context, x0 - x, y0 + y, col);
		else if (s == 0)
			nk_brender_setpixel(context, x0 + x, y0 - y, col);
		else if (s == 90)
			nk_brender_setpixel(context, x0 - x, y0 - y, col);

		if (sigma >= 0)
		{
			sigma += fa2 * (1 - y);
			y--;
		}

		sigma += b2 * ((4 * x) + 6);
	}

	/* Second half */
	for (x = w, y = 0, sigma = 2*a2+b2*(1-2*w); a2*y <= b2*x; y++)
	{
		if (s == 180)
			nk_brender_setpixel(context, x0 + x, y0 + y, col);
		else if (s == 270)
			nk_brender_setpixel(context, x0 - x, y0 + y, col);
		else if (s == 0)
			nk_brender_setpixel(context, x0 + x, y0 - y, col);
		else if (s == 90)
			nk_brender_setpixel(context, x0 - x, y0 - y, col);

		if (sigma >= 0)
		{
			sigma += fb2 * (1 - x);
			x--;
		}

		sigma += a2 * ((4 * y) + 6);
	}
}

// Fill an arc
static void nk_brender_fill_arc(const rex_nuklear_context *context, rex_short x0, rex_short y0, rex_short w, rex_short h, const rex_short s, const struct nk_color col)
{
	/* Bresenham's ellipses - modified to fill one quarter */
	const rex_int a2 = (w * w) / 4;
	const rex_int b2 = (h * h) / 4;
	const rex_int fa2 = 4 * a2, fb2 = 4 * b2;
	rex_int x, y, sigma;
	struct nk_vec2i pnts[3];

	if (w < 1 || h < 1)
		return;

	if (s != 0 && s != 90 && s != 180 && s != 270)
		return;

	/* Convert upper left to center */
	h = (h + 1) / 2;
	w = (w + 1) / 2;
	x0 += w;
	y0 += h;

	pnts[0].x = x0;
	pnts[0].y = y0;
	pnts[2].x = x0;
	pnts[2].y = y0;

	/* First half */
	for (x = 0, y = h, sigma = 2 * b2 + a2 * (1 - 2 * h); b2 * x <= a2 * y; x++)
	{
		if (s == 180)
		{
			pnts[1].x = x0 + x; pnts[1].y = y0 + y;
		}
		else if (s == 270)
		{
			pnts[1].x = x0 - x; pnts[1].y = y0 + y;
		}
		else if (s == 0)
		{
			pnts[1].x = x0 + x; pnts[1].y = y0 - y;
		}
		else if (s == 90)
		{
			pnts[1].x = x0 - x; pnts[1].y = y0 - y;
		}

		nk_brender_fill_polygon(context, pnts, 3, col);

		pnts[2] = pnts[1];

		if (sigma >= 0)
		{
			sigma += fa2 * (1 - y);
			y--;
		}

		sigma += b2 * ((4 * x) + 6);
	}

	/* Second half */
	for (x = w, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * w); a2 * y <= b2 * x; y++)
	{
		if (s == 180)
		{
			pnts[1].x = x0 + x; pnts[1].y = y0 + y;
		}
		else if (s == 270)
		{
			pnts[1].x = x0 - x; pnts[1].y = y0 + y;
		}
		else if (s == 0)
		{
			pnts[1].x = x0 + x; pnts[1].y = y0 - y;
		}
		else if (s == 90)
		{
			pnts[1].x = x0 - x; pnts[1].y = y0 - y;
		}

		nk_brender_fill_polygon(context, pnts, 3, col);

		pnts[2] = pnts[1];

		if (sigma >= 0)
		{
			sigma += fb2 * (1 - x);
			x--;
		}

		sigma += a2 * ((4 * y) + 6);
	}
}

static void nk_brender_stroke_rect(const rex_nuklear_context *context, const rex_short x, const rex_short y, const rex_short w, const rex_short h, const rex_short r, const rex_short line_thickness, const struct nk_color col)
{
	if (r == 0)
	{
		nk_brender_stroke_line(context, x, y, x + w, y, line_thickness, col);
		nk_brender_stroke_line(context, x, y + h, x + w, y + h, line_thickness, col);
		nk_brender_stroke_line(context, x, y, x, y + h, line_thickness, col);
		nk_brender_stroke_line(context, x + w, y, x + w, y + h, line_thickness, col);
	}
	else
	{
		const rex_short xc = x + r;
		const rex_short yc = y + r;
		const rex_short wc = (rex_short)(w - 2 * r);
		const rex_short hc = (rex_short)(h - 2 * r);

		nk_brender_stroke_line(context, xc, y, xc + wc, y, line_thickness, col);
		nk_brender_stroke_line(context, x + w, yc, x + w, yc + hc, line_thickness, col);
		nk_brender_stroke_line(context, xc, y + h, xc + wc, y + h, line_thickness, col);
		nk_brender_stroke_line(context, x, yc, x, yc + hc, line_thickness, col);

		nk_brender_stroke_arc(context, xc + wc - r, y, (rex_uint)r * 2, (rex_uint)r * 2, 0 , line_thickness, col);
		nk_brender_stroke_arc(context, x, y, (rex_uint)r * 2, (rex_uint)r * 2, 90 , line_thickness, col);
		nk_brender_stroke_arc(context, x, yc + hc - r, (rex_uint)r * 2, (rex_uint)r * 2, 270 , line_thickness, col);
		nk_brender_stroke_arc(context, xc + wc - r, yc + hc - r, (rex_uint)r * 2, (rex_uint)r * 2, 180 , line_thickness, col);
	}
}

static void nk_brender_fill_rect(const rex_nuklear_context *context, const rex_short x, const rex_short y, const rex_short w, const rex_short h, const rex_short r, const struct nk_color col)
{
	rex_int i;

	if (r == 0)
	{
		for (i = 0; i < h; i++)
		{
			nk_brender_stroke_line(context, x, y + i, x + w, y + i, 1, col);
		}
	}
	else
	{
		const rex_short xc = x + r;
		const rex_short yc = y + r;
		const rex_short wc = (rex_short)(w - 2 * r);
		const rex_short hc = (rex_short)(h - 2 * r);

		struct nk_vec2i pnts[12];
		pnts[0].x = x;
		pnts[0].y = yc;
		pnts[1].x = xc;
		pnts[1].y = yc;
		pnts[2].x = xc;
		pnts[2].y = y;

		pnts[3].x = xc + wc;
		pnts[3].y = y;
		pnts[4].x = xc + wc;
		pnts[4].y = yc;
		pnts[5].x = x + w;
		pnts[5].y = yc;

		pnts[6].x = x + w;
		pnts[6].y = yc + hc;
		pnts[7].x = xc + wc;
		pnts[7].y = yc + hc;
		pnts[8].x = xc + wc;
		pnts[8].y = y + h;

		pnts[9].x = xc;
		pnts[9].y = y + h;
		pnts[10].x = xc;
		pnts[10].y = yc + hc;
		pnts[11].x = x;
		pnts[11].y = yc + hc;

		nk_brender_fill_polygon(context, pnts, 12, col);

		nk_brender_fill_arc(context, xc + wc - r, y, (rex_uint)r * 2, (rex_uint)r * 2, 0 , col);
		nk_brender_fill_arc(context, x, y, (rex_uint)r * 2, (rex_uint)r * 2, 90 , col);
		nk_brender_fill_arc(context, x, yc + hc - r, (rex_uint)r * 2, (rex_uint)r * 2, 270 , col);
		nk_brender_fill_arc(context, xc + wc - r, yc + hc - r, (rex_uint)r * 2, (rex_uint)r * 2, 180 , col);
	}
}

NK_API void nk_brender_draw_rect_multi_color(const rex_nuklear_context *context, const rex_short x, const rex_short y, const rex_short w, const rex_short h, struct nk_color tl, struct nk_color tr, struct nk_color br, struct nk_color bl)
{
	rex_int i, j;
	struct nk_color *edge_buf;
	struct nk_color *edge_t;
	struct nk_color *edge_b;
	struct nk_color *edge_l;
	struct nk_color *edge_r;
	struct nk_color pixel;

	edge_buf = malloc(((2 * w) + (2 * h)) * sizeof(struct nk_color));

	if (edge_buf == NULL)
		return;

	edge_t = edge_buf;
	edge_b = edge_buf + w;
	edge_l = edge_buf + (w*2);
	edge_r = edge_buf + (w*2) + h;

	/* Top and bottom edge gradients */
	for (i = 0; i < w; i++)
	{
		edge_t[i].r = (((((rex_float)tr.r - tl.r)/(w-1))*i) + 0.5) + tl.r;
		edge_t[i].g = (((((rex_float)tr.g - tl.g)/(w-1))*i) + 0.5) + tl.g;
		edge_t[i].b = (((((rex_float)tr.b - tl.b)/(w-1))*i) + 0.5) + tl.b;
		edge_t[i].a = (((((rex_float)tr.a - tl.a)/(w-1))*i) + 0.5) + tl.a;

		edge_b[i].r = (((((rex_float)br.r - bl.r)/(w-1))*i) + 0.5) + bl.r;
		edge_b[i].g = (((((rex_float)br.g - bl.g)/(w-1))*i) + 0.5) + bl.g;
		edge_b[i].b = (((((rex_float)br.b - bl.b)/(w-1))*i) + 0.5) + bl.b;
		edge_b[i].a = (((((rex_float)br.a - bl.a)/(w-1))*i) + 0.5) + bl.a;
	}

	/* Left and right edge gradients */
	for (i = 0; i < h; i++)
	{
		edge_l[i].r = (((((rex_float)bl.r - tl.r)/(h-1))*i) + 0.5) + tl.r;
		edge_l[i].g = (((((rex_float)bl.g - tl.g)/(h-1))*i) + 0.5) + tl.g;
		edge_l[i].b = (((((rex_float)bl.b - tl.b)/(h-1))*i) + 0.5) + tl.b;
		edge_l[i].a = (((((rex_float)bl.a - tl.a)/(h-1))*i) + 0.5) + tl.a;

		edge_r[i].r = (((((rex_float)br.r - tr.r)/(h-1))*i) + 0.5) + tr.r;
		edge_r[i].g = (((((rex_float)br.g - tr.g)/(h-1))*i) + 0.5) + tr.g;
		edge_r[i].b = (((((rex_float)br.b - tr.b)/(h-1))*i) + 0.5) + tr.b;
		edge_r[i].a = (((((rex_float)br.a - tr.a)/(h-1))*i) + 0.5) + tr.a;
	}

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			if (i == 0)
			{
				nk_pixelmap_blendpixel(context->pm, x+j, y+i, edge_t[j]);
			}
			else if (i == h - 1)
			{
				nk_pixelmap_blendpixel(context->pm, x+j, y+i, edge_b[j]);
			}
			else
			{
				if (j == 0)
				{
					nk_pixelmap_blendpixel(context->pm, x+j, y+i, edge_l[i]);
				}
				else if (j == w - 1)
				{
					nk_pixelmap_blendpixel(context->pm, x+j, y+i, edge_r[i]);
				}
				else
				{
					pixel.r = (((((rex_float)edge_r[i].r - edge_l[i].r)/(w-1))*j) + 0.5) + edge_l[i].r;
					pixel.g = (((((rex_float)edge_r[i].g - edge_l[i].g)/(w-1))*j) + 0.5) + edge_l[i].g;
					pixel.b = (((((rex_float)edge_r[i].b - edge_l[i].b)/(w-1))*j) + 0.5) + edge_l[i].b;
					pixel.a = (((((rex_float)edge_r[i].a - edge_l[i].a)/(w-1))*j) + 0.5) + edge_l[i].a;
					nk_pixelmap_blendpixel(context->pm, x+j, y+i, pixel);
				}
			}
		}
	}

	free(edge_buf);
}

static void nk_brender_fill_triangle(const rex_nuklear_context *context, const rex_short x0, const rex_short y0, const rex_short x1, const rex_short y1, const rex_short x2, const rex_short y2, const struct nk_color col)
{
	struct nk_vec2i pnts[3];

	pnts[0].x = x0;
	pnts[0].y = y0;
	pnts[1].x = x1;
	pnts[1].y = y1;
	pnts[2].x = x2;
	pnts[2].y = y2;

	nk_brender_fill_polygon(context, pnts, 3, col);
}

static void nk_brender_stroke_triangle(const rex_nuklear_context *context, const rex_short x0, const rex_short y0, const rex_short x1, const rex_short y1, const rex_short x2, const rex_short y2, const rex_ushort line_thickness, const struct nk_color col)
{
	nk_brender_stroke_line(context, x0, y0, x1, y1, line_thickness, col);
	nk_brender_stroke_line(context, x1, y1, x2, y2, line_thickness, col);
	nk_brender_stroke_line(context, x2, y2, x0, y0, line_thickness, col);
}

static void nk_brender_stroke_polygon(const rex_nuklear_context *context, const struct nk_vec2i *pnts, const rex_int count, const rex_ushort line_thickness, const struct nk_color col)
{
	rex_int i;

	for (i = 1; i < count; ++i)
	{
		nk_brender_stroke_line(context, pnts[i-1].x, pnts[i-1].y, pnts[i].x, pnts[i].y, line_thickness, col);
	}

	nk_brender_stroke_line(context, pnts[count-1].x, pnts[count-1].y, pnts[0].x, pnts[0].y, line_thickness, col);
}

static void nk_brender_stroke_polyline(const rex_nuklear_context *context, const struct nk_vec2i *pnts, const rex_int count, const rex_ushort line_thickness, const struct nk_color col)
{
	rex_int i;

	for (i = 0; i < count-1; ++i)
	{
		nk_brender_stroke_line(context, pnts[i].x, pnts[i].y, pnts[i+1].x, pnts[i+1].y, line_thickness, col);
	}
}

static void nk_brender_fill_circle(const rex_nuklear_context *context, rex_short x0, rex_short y0, rex_short w, rex_short h, const struct nk_color col)
{
	/* Bresenham's ellipses */
	const rex_int a2 = (w * w) / 4;
	const rex_int b2 = (h * h) / 4;
	const rex_int fa2 = 4 * a2, fb2 = 4 * b2;
	rex_int x, y, sigma;

	/* Convert upper left to center */
	h = (h + 1) / 2;
	w = (w + 1) / 2;
	x0 += w;
	y0 += h;

	/* First half */
	for (x = 0, y = h, sigma = 2*b2+a2*(1-2*h); b2*x <= a2*y; x++)
	{
		nk_brender_stroke_line(context, x0 - x, y0 + y, x0 + x, y0 + y, 1, col);
		nk_brender_stroke_line(context, x0 - x, y0 - y, x0 + x, y0 - y, 1, col);

		if (sigma >= 0)
		{
			sigma += fa2 * (1 - y);
			y--;
		}

		sigma += b2 * ((4 * x) + 6);
	}

	/* Second half */
	for (x = w, y = 0, sigma = 2*a2+b2*(1-2*w); a2*y <= b2*x; y++)
	{
		nk_brender_stroke_line(context, x0 - x, y0 + y, x0 + x, y0 + y, 1, col);
		nk_brender_stroke_line(context, x0 - x, y0 - y, x0 + x, y0 - y, 1, col);

		if (sigma >= 0)
		{
			sigma += fb2 * (1 - x);
			x--;
		}

		sigma += a2 * ((4 * y) + 6);
	}
}

static void nk_brender_stroke_circle(const rex_nuklear_context *context, rex_short x0, rex_short y0, rex_short w, rex_short h, const rex_short line_thickness, const struct nk_color col)
{
	/* Bresenham's ellipses */
	const rex_int a2 = (w * w) / 4;
	const rex_int b2 = (h * h) / 4;
	const rex_int fa2 = 4 * a2, fb2 = 4 * b2;
	rex_int x, y, sigma;

	NK_UNUSED(line_thickness);

	/* Convert upper left to center */
	h = (h + 1) / 2;
	w = (w + 1) / 2;
	x0 += w;
	y0 += h;

	/* First half */
	for (x = 0, y = h, sigma = 2*b2+a2*(1-2*h); b2*x <= a2*y; x++)
	{
		nk_brender_setpixel(context, x0 + x, y0 + y, col);
		nk_brender_setpixel(context, x0 - x, y0 + y, col);
		nk_brender_setpixel(context, x0 + x, y0 - y, col);
		nk_brender_setpixel(context, x0 - x, y0 - y, col);
	
		if (sigma >= 0)
		{
			sigma += fa2 * (1 - y);
			y--;
		}

		sigma += b2 * ((4 * x) + 6);
	}

	/* Second half */
	for (x = w, y = 0, sigma = 2*a2+b2*(1-2*w); a2*y <= b2*x; y++)
	{
		nk_brender_setpixel(context, x0 + x, y0 + y, col);
		nk_brender_setpixel(context, x0 - x, y0 + y, col);
		nk_brender_setpixel(context, x0 + x, y0 - y, col);
		nk_brender_setpixel(context, x0 - x, y0 - y, col);

		if (sigma >= 0)
		{
			sigma += fb2 * (1 - x);
			x--;
		}

		sigma += a2 * ((4 * y) + 6);
	}
}

static void nk_brender_stroke_curve(const rex_nuklear_context *context, const struct nk_vec2i p1, const struct nk_vec2i p2, const struct nk_vec2i p3, const struct nk_vec2i p4, const rex_uint num_segments, const rex_ushort line_thickness, const struct nk_color col)
{
	rex_uint i_step, segments;
	rex_float t_step;
	struct nk_vec2i last = p1;

	segments = REX_MAX(num_segments, 1);

	t_step = 1.0f / (rex_float)segments;

	for (i_step = 1; i_step <= segments; ++i_step)
	{
		rex_float t = t_step * (rex_float)i_step;
		rex_float u = 1.0f - t;
		rex_float w1 = u*u*u;
		rex_float w2 = 3 * u * u * t;
		rex_float w3 = 3 * u * t * t;
		rex_float w4 = t * t *t;
		rex_float x = w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x;
		rex_float y = w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y;

		nk_brender_stroke_line(context, last.x, last.y, (rex_short)x, (rex_short)y, line_thickness,col);

		last.x = (rex_short)x; last.y = (rex_short)y;
	}
}

static void nk_brender_clear(const rex_nuklear_context *context, const struct nk_color col)
{
	nk_brender_fill_rect(context, 0, 0, context->pm->width, context->pm->height, 0, col);
}

static void nk_brender_stretch_image(struct br_pixelmap *dst, struct br_pixelmap *src, const struct nk_rect *dst_rect, const struct nk_rect *src_rect, const struct nk_rect *dst_scissors, const struct nk_color *fg)
{
	rex_short i, j;
	struct nk_color col;
	rex_float xinc = src_rect->w / dst_rect->w;
	rex_float yinc = src_rect->h / dst_rect->h;
	rex_float xoff = src_rect->x, yoff = src_rect->y;

	/* Simple nearest filtering rescaling */
	/* TODO: use bilinear filter */
	for (j = 0; j < (rex_short)dst_rect->h; j++)
	{
		for (i = 0; i < (rex_short)dst_rect->w; i++)
		{
			if (dst_scissors)
			{
				if (i + (rex_int)(dst_rect->x + 0.5f) < dst_scissors->x || i + (rex_int)(dst_rect->x + 0.5f) >= dst_scissors->w)
					continue;
				if (j + (rex_int)(dst_rect->y + 0.5f) < dst_scissors->y || j + (rex_int)(dst_rect->y + 0.5f) >= dst_scissors->h)
					continue;
			}

			col = nk_pixelmap_getpixel(src, (rex_int)xoff, (rex_int) yoff);

			if (col.r || col.g || col.b)
			{
				col.r = fg->r;
				col.g = fg->g;
				col.b = fg->b;
			}

			nk_pixelmap_blendpixel(dst, i + (rex_int)(dst_rect->x + 0.5f), j + (rex_int)(dst_rect->y + 0.5f), col);

			xoff += xinc;
		}

		xoff = src_rect->x;
		yoff += yinc;
	}
}

static void nk_brender_font_query_font_glyph(nk_handle handle, const float height, struct nk_user_font_glyph *glyph, const nk_rune codepoint, const nk_rune next_codepoint)
{
	float scale;
	const struct nk_font_glyph *g;
	struct nk_font *font;
	NK_ASSERT(glyph);
	NK_UNUSED(next_codepoint);

	font = (struct nk_font*)handle.ptr;
	NK_ASSERT(font);
	NK_ASSERT(font->glyphs);
	if (!font || !glyph)
		return;

	scale = height/font->info.height;
	g = nk_font_find_glyph(font, codepoint);
	glyph->width = (g->x1 - g->x0) * scale;
	glyph->height = (g->y1 - g->y0) * scale;
	glyph->offset = nk_vec2(g->x0 * scale, g->y0 * scale);
	glyph->xadvance = (g->xadvance * scale);
	glyph->uv[0] = nk_vec2(g->u0, g->v0);
	glyph->uv[1] = nk_vec2(g->u1, g->v1);
}

NK_API void nk_brender_draw_text(const rex_nuklear_context *context, const struct nk_user_font *font, const struct nk_rect rect, const rex_byte *text, const rex_int len, const rex_float font_height, const struct nk_color fg)
{
	rex_float x = 0;
	rex_int text_len = 0;
	nk_rune unicode = 0;
	nk_rune next = 0;
	rex_int glyph_len = 0;
	rex_int next_glyph_len = 0;
	struct nk_user_font_glyph g;

	if (!len || !text)
		return;

	x = 0;
	glyph_len = nk_utf_decode(text, &unicode, len);

	if (!glyph_len)
		return;

	/* draw every glyph image */
	while (text_len < len && glyph_len)
	{
		struct nk_rect src_rect;
		struct nk_rect dst_rect;
		float char_width = 0;

		if (unicode == NK_UTF_INVALID)
			break;

		/* query currently drawn glyph information */
		next_glyph_len = nk_utf_decode(text + text_len + glyph_len, &next, (rex_int)len - text_len);
		nk_brender_font_query_font_glyph(font->userdata, font_height, &g, unicode, (next == NK_UTF_INVALID) ? '\0' : next);

		/* calculate and draw glyph drawing rectangle and image */
		char_width = g.xadvance;
		src_rect.x = g.uv[0].x * context->font_tex->width;
		src_rect.y = g.uv[0].y * context->font_tex->height;
		src_rect.w = g.uv[1].x * context->font_tex->width - g.uv[0].x * context->font_tex->width;
		src_rect.h = g.uv[1].y * context->font_tex->height - g.uv[0].y * context->font_tex->height;

		dst_rect.x = x + g.offset.x + rect.x;
		dst_rect.y = g.offset.y + rect.y;
		dst_rect.w = ceil(g.width);
		dst_rect.h = ceil(g.height);

		/* Use software rescaling to blit glyph from font_text to framebuffer */
		nk_brender_stretch_image(context->pm, context->font_tex, &dst_rect, &src_rect, &context->scissors, &fg);

		/* offset next glyph */
		text_len += glyph_len;
		x += char_width;
		glyph_len = next_glyph_len;
		unicode = next;
	}
}

NK_API void nk_brender_drawimage(const rex_nuklear_context *context, const rex_int x, const rex_int y, const rex_int w, const rex_int h, const struct nk_image *img, const struct nk_color *col)
{
	struct nk_rect src_rect;
	struct nk_rect dst_rect;

	src_rect.x = img->region[0];
	src_rect.y = img->region[1];
	src_rect.w = img->region[2];
	src_rect.h = img->region[3];

	dst_rect.x = x;
	dst_rect.y = y;
	dst_rect.w = w;
	dst_rect.h = h;

	nk_brender_stretch_image(context->pm, context->font_tex, &dst_rect, &src_rect, &context->scissors, col);
}

//
// Public functions
//

// Initialize Nuklear with source pixelmap, font and font size.
rex_nuklear_context *Rex_Nuklear_Init(br_pixelmap *pm, const rex_byte *font_filename, rex_float font_size)
{
	const void *tex;
	rex_int texh, texw;
	rex_nuklear_context *context;

	//assert((pm->type == BR_PMT_ARGB_8888) || (pm->type == BR_PMT_RGBA_8888));

	context = malloc(sizeof(rex_nuklear_context));

	if (!context)
		return NULL;

	memset(context, 0, sizeof(rex_nuklear_context));

	context->pm = pm;

	if (nk_init_default(&context->ctx, 0) == 0)
	{
		free(context);
		return NULL;
	}

	nk_font_atlas_init_default(&context->atlas);
	nk_font_atlas_begin(&context->atlas);

	struct nk_font *font = nk_font_atlas_add_from_file(&context->atlas, font_filename, font_size, 0);
	context->atlas.default_font = nk_font_atlas_add_default(&context->atlas, font_size, 0);
	tex = nk_font_atlas_bake(&context->atlas, &texw, &texh, NK_FONT_ATLAS_RGBA32);

	if (!tex)
	{
		free(context);
		return NULL;
	}

	context->font_tex = BrPixelmapAllocate(BR_PMT_RGBA_8888, texw, texh, NULL, 0);

	memcpy(context->font_tex->pixels, tex, texw * texh * 4);

	nk_font_atlas_end(&context->atlas, nk_handle_ptr(NULL), NULL);

	if (font)
		nk_style_set_font(&context->ctx, &font->handle);
	else if (context->atlas.default_font)
		nk_style_set_font(&context->ctx, &context->atlas.default_font->handle);

	//nk_style_load_all_cursors(&context->ctx, context->atlas.cursors);
	nk_brender_scissor(context, 0, 0, context->pm->width, context->pm->height);

	return context;
}

// Shutdown the given Nuklear context.
NK_API void Rex_Nuklear_Shutdown(rex_nuklear_context *context)
{
	if (context)
	{
		BrPixelmapFree(context->font_tex);
		nk_free(&context->ctx);
		memset(context, 0, sizeof(rex_nuklear_context));
		free(context);
	}
}

// Render the given Nuklear context to the defined pixelmap.
NK_API void Rex_Nuklear_Render(const rex_nuklear_context *context, const struct nk_color clear, const rex_ubyte enable_clear)
{
	const struct nk_command *cmd;
	if (enable_clear)
		nk_brender_clear(context, clear);

	nk_foreach(cmd, (struct nk_context*)&context->ctx)
	{
		switch (cmd->type)
		{
			case NK_COMMAND_NOP:
				break;

			case NK_COMMAND_SCISSOR:
			{
				const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
				nk_brender_scissor((rex_nuklear_context *)context, s->x, s->y, s->w, s->h);
				break;
			}

			case NK_COMMAND_LINE:
			{
				const struct nk_command_line *l = (const struct nk_command_line *)cmd;
				nk_brender_stroke_line(context, l->begin.x, l->begin.y, l->end.x, l->end.y, l->line_thickness, l->color);
				break;
			}

			case NK_COMMAND_RECT:
			{
				const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
				nk_brender_stroke_rect(context, r->x, r->y, r->w, r->h, (rex_ushort)r->rounding, r->line_thickness, r->color);
				break;
			}

			case NK_COMMAND_RECT_FILLED:
			{
				const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
				nk_brender_fill_rect(context, r->x, r->y, r->w, r->h, (rex_ushort)r->rounding, r->color);
				break;
			}

			case NK_COMMAND_CIRCLE:
			{
				const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
				nk_brender_stroke_circle(context, c->x, c->y, c->w, c->h, c->line_thickness, c->color);
				break;
			}

			case NK_COMMAND_CIRCLE_FILLED:
			{
				const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
				nk_brender_fill_circle(context, c->x, c->y, c->w, c->h, c->color);
				break;
			}

			case NK_COMMAND_TRIANGLE:
			{
				const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
				nk_brender_stroke_triangle(context, t->a.x, t->a.y, t->b.x, t->b.y, t->c.x, t->c.y, t->line_thickness, t->color);
				break;
			}

			case NK_COMMAND_TRIANGLE_FILLED:
			{
				const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
				nk_brender_fill_triangle(context, t->a.x, t->a.y, t->b.x, t->b.y, t->c.x, t->c.y, t->color);
				break;
			}

			case NK_COMMAND_POLYGON:
			{
				const struct nk_command_polygon *p =(const struct nk_command_polygon*)cmd;
				nk_brender_stroke_polygon(context, p->points, p->point_count, p->line_thickness,p->color);
				break;
			}

			case NK_COMMAND_POLYGON_FILLED:
			{
				const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
				nk_brender_fill_polygon(context, p->points, p->point_count, p->color);
				break;
			}

			case NK_COMMAND_POLYLINE:
			{
				const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
				nk_brender_stroke_polyline(context, p->points, p->point_count, p->line_thickness, p->color);
				break;
			}

			case NK_COMMAND_TEXT:
			{
				const struct nk_command_text *t = (const struct nk_command_text*)cmd;
				nk_brender_draw_text(context, t->font, nk_rect(t->x, t->y, t->w, t->h), t->string, t->length, t->height, t->foreground);
				break;
			}

			case NK_COMMAND_CURVE:
			{
				const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
				nk_brender_stroke_curve(context, q->begin, q->ctrl[0], q->ctrl[1], q->end, 22, q->line_thickness, q->color);
				break;
			}

			case NK_COMMAND_RECT_MULTI_COLOR:
			{
				const struct nk_command_rect_multi_color *q = (const struct nk_command_rect_multi_color *)cmd;
				nk_brender_draw_rect_multi_color(context, q->x, q->y, q->w, q->h, q->left, q->top, q->right, q->bottom);
				break;
			}

			case NK_COMMAND_IMAGE:
			{
				const struct nk_command_image *q = (const struct nk_command_image *)cmd;
				nk_brender_drawimage(context, q->x, q->y, q->w, q->h, &q->img, &q->col);
				break;
			}

			case NK_COMMAND_ARC:
			{
				assert(0 && "NK_COMMAND_ARC not implemented\n");
				break;
			}

			case NK_COMMAND_ARC_FILLED:
			{
				assert(0 && "NK_COMMAND_ARC_FILLED not implemented\n");
				break;
			}

			default:
				break;
		}
	}

	nk_clear((struct nk_context*)&context->ctx);
}
