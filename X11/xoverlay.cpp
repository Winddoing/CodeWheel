//g++ xoverlay.cpp -o testxoverplay -lX11 -lXext -lm -lXfixes -std=c++11

#include <iostream>
#include <stdlib.h>
#include <climits>
#include <chrono>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xfixes.h>
#include <math.h>

// Events for normal windows
#define BASIC_EVENT_MASK (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask|KeymapStateMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)

using namespace std;

//#define COSNTANTWM
//#define LINESTYLE

#define FPSMETERSAMPLE 100
#define MAXFONTSNAMES  1000
#define INTERVALOFDRAWING  833*10  //833*1000  us

Display *g_display;
int      g_screen;
Window   g_win;
int      g_disp_width;
int      g_disp_height;
Pixmap   g_bitmap;
Colormap g_colormap;

XColor red;
XColor black;
XColor white;

std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
int fpsmeterc = 0;
auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
string fpsstring = "";

int     shape_event_base;
int     shape_error_base;

// The window size
int WIDTH  = 1920;
int HEIGHT = 1080;

long event_mask = (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyRelease | ButtonPress|ButtonRelease|KeymapStateMask);

void allow_input_passthrough (Window w) {
    XserverRegion region = XFixesCreateRegion (g_display, NULL, 0);

    //XFixesSetWindowShapeRegion (g_display, w, ShapeBounding, 0, 0, 0);
    XFixesSetWindowShapeRegion (g_display, w, ShapeInput, 0, 0, region);

    XFixesDestroyRegion (g_display, region);
}

//enumerate available fonts
void list_fonts() {
    char **fontlist;
    int num_fonts;
    //enumerate the scalable ones only
    //fontlist = XListFonts (g_display, "*--0-0-0-0-*", MAXFONTSNAMES, &num_fonts);
    fontlist = XListFonts (g_display, "*", MAXFONTSNAMES, &num_fonts);
    for (int i = 0; i < num_fonts; ++i) {
        fprintf(stderr, "> %s\n", fontlist[i]);
    }
}

// Create a XColor from 4 byte tuple (0 - 255, 0 - 255, 0 - 255).
XColor createXColorFromRGBA(short red, short green, short blue, short alpha) {
    XColor color;

    // m_color.red = red * 65535 / 255;
    color.red = (red * 0xFFFF) / 0xFF;
    color.green = (green * 0xFFFF) / 0xFF;
    color.blue = (blue * 0xFFFF) / 0xFF;
    color.flags = DoRed | DoGreen | DoBlue;

    if (!XAllocColor(g_display, DefaultColormap(g_display, g_screen), &color)) {
        std::cerr << "createXColorFromRGBA: Cannot create color" << endl;
        exit(-1);
    }

    *(&color.pixel) = ((*(&color.pixel)) & 0x00ffffff) | (alpha << 24);
    return color;
}

// Create a window
void createShapedWindow() {
    XSetWindowAttributes wattr;
    XColor bgcolor = createXColorFromRGBA(0, 0, 0, 0);

    Window root    = DefaultRootWindow(g_display);
    Visual *visual = DefaultVisual(g_display, g_screen);

    XVisualInfo vinfo;
    XMatchVisualInfo(g_display, DefaultScreen(g_display), 32, TrueColor, &vinfo);
    g_colormap = XCreateColormap(g_display, DefaultRootWindow(g_display), vinfo.visual, AllocNone);

    XSetWindowAttributes attr;
    attr.background_pixmap = None;
    attr.background_pixel = bgcolor.pixel;
    attr.border_pixel=0;
    attr.win_gravity=NorthWestGravity;
    attr.bit_gravity=ForgetGravity;
    attr.save_under=1;
    attr.event_mask=BASIC_EVENT_MASK;
    attr.do_not_propagate_mask=NOT_PROPAGATE_MASK;
    attr.override_redirect=1; // OpenGL > 0
    attr.colormap = g_colormap;

    //unsigned long mask = CWBackPixel|CWBorderPixel|CWWinGravity|CWBitGravity|CWSaveUnder|CWEventMask|CWDontPropagate|CWOverrideRedirect;
    unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity|CWBitGravity | CWSaveUnder | CWDontPropagate | CWOverrideRedirect;

    g_win = XCreateWindow(g_display, root, 0, 0, WIDTH, HEIGHT, 0, vinfo.depth, InputOutput, vinfo.visual, mask, &attr);

	//g_bitmap = XCreateBitmapFromData (g_display, RootWindow(g_display, g_screen), (char *)myshape_bits, myshape_width, myshape_height);

    //XShapeCombineMask(g_display, g_win, ShapeBounding, 900, 500, g_bitmap, ShapeSet);
    XShapeCombineMask(g_display, g_win, ShapeInput, 0, 0, None, ShapeSet );

    // We want shape-changed event too
    #define SHAPE_MASK ShapeNotifyMask
    XShapeSelectInput (g_display, g_win, SHAPE_MASK );

    // Tell the Window Manager not to draw window borders (frame) or title.
	wattr.override_redirect = 1;
    XChangeWindowAttributes(g_display, g_win, CWOverrideRedirect, &wattr);
    allow_input_passthrough(g_win);

    // Show the window
    XMapWindow(g_display, g_win);

    red = createXColorFromRGBA(255, 0, 0, 255);
    black = createXColorFromRGBA(0, 0, 0, 200);
    white = createXColorFromRGBA(255, 255, 255, 255);
}


// Draw on the shaped window.
// Yes it's possible, but only pixels that hits the mask are visible.
// A hint: You can change the mask during runtime if you like.
void draw()
{
    fpsmeterc++;
    if(fpsmeterc == FPSMETERSAMPLE) {
        fpsmeterc = 0;
        t1 = t2;
        t2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        fpsstring = /*to_string(duration) + " / " +*/ to_string(1000000*FPSMETERSAMPLE/duration);
    }

    GC gc;
    XGCValues gcv;

#ifdef LINESTYLE
    // Line width and style
    gcv.line_width = 1;
    gcv.line_style = LineSolid; //ÊµÏß
    gcv.foreground = red.pixel;

    unsigned long mask = GCLineWidth | GCLineStyle | GCForeground;
    gc = XCreateGC(g_display, g_win, mask, &gcv);
#else  
    gc = XCreateGC (g_display, g_win, 0, 0);
#endif

    XSetBackground (g_display, gc, white.pixel); 
    XSetForeground (g_display, gc, red.pixel);

    XFontStruct * font;
    //const char * fontname = "-misc-fixed-bold-r-normal--18-120-100-100-c-90-iso8859-2";
    //const char * fontname = "-adobe-utopia-bold-i-normal--150-0-0-0-p-90-iso8859-1";
    //const char * fontname = "rk24"; // Chinese
    //const char * fontname = "9x15bold";
    const char * fontname = "6x13";

    font = XLoadQueryFont (g_display, fontname);
    /* If the font could not be loaded, revert to the "fixed" font. */
    if (!font) {
        fprintf (stderr, "unable to load font %s > using fixed\n", fontname);
        font = XLoadQueryFont (g_display, "fixed");
    }
    XSetFont (g_display, gc, font->fid);

    XSetForeground (g_display, gc, black.pixel);
    XFillRectangle(g_display, g_win, gc, 0, 0, 100, 30);

    XSetForeground (g_display, gc, red.pixel);
#ifdef COSNTANTWM
    XDrawString(g_display, g_win, gc, 10, 20, "@Waterprint!", 12);
#else
    if(duration > 0.0f) {
        const char * text = fpsstring.c_str();
        XDrawString(g_display, g_win, gc, 10, 20, text, strlen(text));
    }
#endif

    XFreeFont(g_display, font);
    XFreeGC(g_display, gc);
}

void openDisplay() {
    g_display = XOpenDisplay(0);

    if (!g_display) {
        cerr << "Failed to open X display" << endl;
        exit(-1);
    }

    g_screen    = DefaultScreen(g_display);

    g_disp_width  = DisplayWidth(g_display, g_screen);
    g_disp_height = DisplayHeight(g_display, g_screen);

    // Has shape extions?
    if (!XShapeQueryExtension (g_display, &shape_event_base, &shape_error_base)) {
       cerr << "NO shape extension in your system !" << endl;
       exit (-1);
    }
}

int main() {
    openDisplay();

    createShapedWindow();

    //list_fonts();

    while (1)
    {
        draw();
        usleep(INTERVALOFDRAWING);
    }

    XCloseDisplay(g_display);
    return 0;
}

