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
rasterize(int scale)
{
	NSVGimage *image;
	struct NSVGrasterizer *rast;
	uchar *data;
	int w, h;
	float s;
	Rectangle r;

	image = nsvgParseFromFile(filename, "px", 96);
	if(image==nil)
		sysfatal("svg parse: %r");
	w = image->width;
	h = image->height;
	s = 1.0f;
	if(scale){
		r = insetrect(screen->r, 10);
		w = Dx(r);
		h = Dy(r);
		if(w < h)
			s = (float)w/image->width;
		else
			s = (float)h/image->height;
	}
	rast = nsvgCreateRasterizer();
	data = malloc(w*h*4);
	nsvgRasterize(rast, image, 0, 0, s, data, w, h, w*4);
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
	freeimage(svg);
	rasterize(1);
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
	if(nineflag){
		rasterize(0);
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
