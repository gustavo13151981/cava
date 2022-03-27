// cavacore standalone test app, build cava first and compile with:
// gcc -c -g cavacore_test.c
// gcc -o cavacore_test cavacore_test.o cava-cavacore.o -lm -lfftw3

#include "cavacore.h"
#include <math.h>
#include <stdio.h>

#include <stdlib.h>
#define PI 3.141592654

void main() {

    printf("welcome to cavalib standalone test app\n");
    printf("planning cava 20 bars (left+right) 44100 rate 2 cahnnels, 100 target height, 60 "
           "framerate \n");

    int bars_per_channel = 10;
    int channels = 2;
    int height = 100;
    int framerate = 60;
    int rate = 44100;

    struct cava_plan *plan = cava_init(bars_per_channel, rate, channels, height, framerate);

    printf("got lower cut off frequencies:\n");

    for (int i = 0; i < 10; i++) {
        printf("%.0f \t", plan->cut_off_frequency[i]);
    }
    printf("MHz\n\n");

    printf("allocating buffers and generating sine wave for test\n\n");

    double *cava_out;
    double *cava_in;

    cava_out = (double *)malloc(BUFFER_SIZE * 2 * sizeof(double));

    cava_in = (double *)malloc(BUFFER_SIZE * 2 * 4 * sizeof(double));

    for (int i = 0; i < BUFFER_SIZE * 2; i++) {
        cava_out[i] = 0;
    }
    for (int n = 0; n < BUFFER_SIZE * 4; n++) {
        cava_in[n * 2] = sin(2 * PI * 200 / 44100 * n) * 10000;
        cava_in[n * 2 + 1] = sin(2 * PI * 2000 / 44100 * n) * 10000;
    }

    printf("running cava execute 300 times (simulating 5 seconds run time)\n\n");
    for (int k = 0; k < 300; k++) {
        cava_execute(cava_in, BUFFER_SIZE * 4 * 2, cava_out, plan);
    }

    printf("\noutput left, max value should be at 2000Hz:\n");
    for (int i = 0; i < 10; i++) {
        printf("%.0f \t", cava_out[i]);
    }
    printf("MHz\n");

    printf("output right,  max value should be at 200Hz:\n");
    for (int i = 10; i < 20; i++) {
        printf("%.0f \t", cava_out[i]);
    }
    printf("MHz\n");
    cava_destroy(plan);
}