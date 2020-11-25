#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

Display                 *dpy;
Window                  root;
int                     num_sizes;
XRRScreenSize           *xrrs;
XRRScreenConfiguration  *conf;
short                   possible_frequencies[64][64];
short                   original_rate;
Rotation                original_rotation;
SizeID                  original_size_id;

int main(int argc, char *argv[])
{
        //connect to x-server, get root window id
        dpy    = XOpenDisplay(NULL);
        root   = RootWindow(dpy, 0);
        //get possible screen resolutions
        xrrs   = XRRSizes(dpy, 0, &num_sizes);
        //loop through all possible resolutions,
        //getting the selectable display frequencies
        for(int i = 0; i < num_sizes; i ++)
        {
                short   *rates;
                int     num_rates;
                printf("\n\t%2i : %4i x %4i   (%4imm x%4imm ) ", i, xrrs[i].width, xrrs[i].height, xrrs[i].mwidth, xrrs[i].mheight);
                rates = XRRRates(dpy, 0, i, &num_rates);
                for(int j = 0; j < num_rates; j ++)
                {
                        possible_frequencies[i][j] = rates[j];
                        printf("%4i ", rates[j]);
                }
        }

        printf("\n\n");
        //get current resolution and frequency
        conf                   = XRRGetScreenInfo(dpy, root);
        original_rate          = XRRConfigCurrentRate(conf);
        original_size_id       = XRRConfigCurrentConfiguration(conf, &original_rotation);

        printf("\tCurrent size id    : %i\n", original_size_id);
        printf("\tCurrent rotation   : %i\n", original_rotation);
        printf("\tCurrent rate       : %i Hz\n\n", original_rate);
        //change resolution
        printf("\tChanged to %i x %i pixels, %i Hz\n\n", xrrs[1].width, xrrs[1].height, possible_frequencies[1][0]);
        XRRSetScreenConfigAndRate(dpy, conf, root, 1, RR_Rotate_0, possible_frequencies[1][0], CurrentTime);
        //sleep a while
        usleep(5000000);
        //restore original configuration
        printf("\tRestoring %i x %i pixels, %i Hz\n\n", xrrs[original_size_id].width, xrrs[original_size_id].height, original_rate);
        XRRSetScreenConfigAndRate(dpy, conf, root, original_size_id, original_rotation, original_rate, CurrentTime);
        //exit
        XCloseDisplay(dpy);
}
//on ubuntu: apt install libxrandr-dev
//gcc -o Xrandr Xrandr.c -lX11 -lXrandr -lstdc++