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


#include <screener.h>


screener *screener_new() {
	// Allocate object
	screener *_screener = (screener *)malloc(sizeof(screener));
	if (_screener == NULL) return NULL;
	
	// Open /dev/fb0
	_screener->framebuffer = open("/dev/fb0", O_RDWR);
	if (_screener->framebuffer == -1) goto failed_to_open_framebuffer;
	
	// Open /dev/tty0
	_screener->teletype = open("/dev/tty1", O_RDWR);
	if (_screener->teletype == -1) goto failed_to_open_teletype;
	
	// Turn on graphics mode
	ioctl(_screener->teletype, KDSETMODE, KD_TEXT);
	if (ioctl(_screener->teletype, KDSETMODE, KD_GRAPHICS) != 0) goto failed_to_turn_on_graphics_mode;
		
	// Get screen profile and necessary value(s)
	if (ioctl(_screener->framebuffer, FBIOGET_FSCREENINFO, &_screener->profileFixed) != 0) goto failed_to_get_profile;
	if (ioctl(_screener->framebuffer, FBIOGET_VSCREENINFO, &_screener->profileVariable) != 0) goto failed_to_get_profile;
	_screener->width = _screener->profileFixed.line_length / 4;
	_screener->height = _screener->profileVariable.yres;
	_screener->nPixels = _screener->profileFixed.line_length * _screener->height;
	
	// Map screen on memory
	_screener->pixels = mmap(NULL, _screener->nPixels, PROT_READ | PROT_WRITE, MAP_SHARED, _screener->framebuffer, 0);
	if (_screener->pixels == MAP_FAILED) goto failed_to_map_screen;
	
	// Happy ending
	return _screener;
	
	// What if...
	failed_to_map_screen:
	failed_to_get_profile:
	ioctl(_screener->teletype, KDSETMODE, KD_TEXT);
	failed_to_turn_on_graphics_mode:
	close(_screener->teletype);
	failed_to_open_teletype:
	close(_screener->framebuffer);
	failed_to_open_framebuffer:
	free(_screener);
	return NULL;
}
void screener_draw(screener *_screener, int x, int y, int color) {
	if (_screener == NULL) return;
	if ((x < 0) || (y < 0) || (x >= _screener->width) || (y >= _screener->height)) return;
	
	((int *)_screener->pixels)[x + (_screener->width * y)] = color;
}
void screener_dispose(screener *_screener) {
	// Sanity check
	if (_screener == NULL) return;
	
	munmap(_screener->pixels, _screener->nPixels);
	ioctl(_screener->teletype, KDSETMODE, KD_TEXT);
	close(_screener->teletype);
	close(_screener->framebuffer);
	free(_screener);
}
