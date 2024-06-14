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
int nineflag, w, h, sz;
uchar *data;

void
rasterize(int scale)
{
	NSVGimage *image;
	NSVGrasterizer *rast;
	float s, sx, sy, tx, ty;
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
		sx = (float)w/(image->width+0.5);
		sy = (float)h/(image->height+0.5);
		s = (sx < sy) ? sx : sy;
	}
	rast = nsvgCreateRasterizer();
	if(rast==nil)
		sysfatal("create rasterizer: %r");
	sz = w*h*4;
	data = malloc(sz);
	if(data==nil)
		sysfatal("malloc: %r");
	nsvgRasterize(rast, image, 0, 0, s, data, w, h, w*4);
	nsvgDelete(image);
	nsvgDeleteRasterizer(rast);
	if(nineflag)
		return;
	svg = allocimage(display, Rect(0, 0, w, h), ABGR32, 0, DNofill);
	if(svg==nil)
		sysfatal("allocimage: %r");
	if(loadimage(svg, svg->r, data, sz)<sz)
		sysfatal("loadimage: %r");
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
	fprint(2, "usage: %s [-9] [file.svg]\n", argv0);
}

void
main(int argc, char *argv[])
{
	Event e;
	int ev;
	char buf[12+1];

	ARGBEGIN{
	case 't':	/* page compatibility */
		break;
	case '9':
		nineflag++;
		break;
	default:
		fprint(2, "invalid flag '%c'\n", ARGC());
		usage();
		exits("usage");
	}ARGEND
	if(*argv==nil)
		filename = "/fd/0";
	else
		filename = *argv;
	if(nineflag){
		rasterize(0);
		chantostr(buf, ABGR32);
		print("%11s %11d %11d %11d %11d ", buf, 0, 0, w, h);
		if(write(1, data, sz) != sz)
			sysfatal("write: %r\n");
		exits(nil);
	}
	if(initdraw(nil, nil, "svg")<0)
		sysfatal("initdraw: %r");
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
