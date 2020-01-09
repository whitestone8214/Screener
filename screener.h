/*
	Copyright (C) 2019-2020 Minho Jo <whitestone8214@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* gcc -I. screener.c -shared -o libscreener.so */


#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/kd.h>
#include <sys/mman.h>


#ifndef TURNED_ON_SCREENER
#define TURNED_ON_SCREENER

typedef struct screener screener;

struct screener {
	int framebuffer;
	int teletype;
	struct fb_fix_screeninfo profileFixed;
	struct fb_var_screeninfo profileVariable;
	void *pixels;
	int nPixels;
	int width;
	int height;
};


screener *screener_new();
void screener_draw(screener *screener, int x, int y, int color);
void screener_dispose(screener *screener);

#endif
