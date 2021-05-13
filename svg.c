#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>
#include <stdio.h>
#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

char* filename;
Image *svg;

void
rasterize(void)
{
	NSVGimage *image;
	struct NSVGrasterizer *rast;
	uchar *data;
	int w, h;

	image = nsvgParseFromFile(filename, "px", 96);
	if(image==nil)
		sysfatal("svg parse: %r");
	w = image->width;
	h = image->height;
	rast = nsvgCreateRasterizer();
	data = malloc(w*h*4);
	nsvgRasterize(rast, image, 0, 0, 1, data, w, h, w*4);
	nsvgDelete(image);
	nsvgDeleteRasterizer(rast);
	svg = allocimage(display, Rect(0, 0, w, h), ABGR32, 0, DNofill);
	loadimage(svg, svg->r, data, w*h*4);
}

void
eresized(int new)
{
	Point p;

	if(new && getwindow(display, Refnone)<0)
		sysfatal("cannot reattach: %r");
	p.x = (Dx(screen->r) - Dx(svg->r))/2;
	p.y = (Dy(screen->r) - Dy(svg->r))/2;
	draw(screen, screen->r, display->white, nil, ZP);
	draw(screen, rectaddpt(screen->r, p), svg, nil, ZP);
}

void
usage(void)
{
	fprint(2, "usage: %s [-9] file.svg\n", argv0);
}

void
main(int argc, char *argv[])
{
	Event e;
	int ev, nineflag;

	ARGBEGIN{
	case '9':
		nineflag++;
		break;
	default:
		fprint(2, "invalid flag '%c'\n", ARGC());
		usage();
		exits("usage");
	}ARGEND
	if(*argv==nil){
		usage();
		exits("usage");
	}
	filename = *argv;
	if(initdraw(nil, nil, "svg")<0)
		sysfatal("initdraw: %r");
	rasterize();
	if(nineflag){
		writeimage(1, svg, 0);
		freeimage(svg);
		exits(nil);
	}
	einit(Emouse|Ekeyboard);
	eresized(0);
	for(;;){
		ev = event(&e);
		switch(ev){
		case Ekeyboard:
			if(e.kbdc=='q' || e.kbdc==Kdel)
				exits(nil);
			break;
		case Emouse:
			break;
		}
	}
}
