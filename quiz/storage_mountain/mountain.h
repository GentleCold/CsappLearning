//
// Created by GentleCold on 2022/11/7.
//

#ifndef CSAPPLEARNING_MOUNTAIN_H
#define CSAPPLEARNING_MOUNTAIN_H

#include <stdio.h>

#define MAXELEMS 10000
long data[MAXELEMS];

int read(int elems, int stride) {
    long i, sx2 = stride * 2, sx3 = stride * 3, sx4 = stride * 4;
    long acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
    long length = elems;
    long limit = length - sx4;

    for (i = 0; i < limit; i += sx4) {
        acc0 += data[i];
        acc1 += data[i + stride];
        acc2 += data[i + sx2];
        acc3 += data[i + sx3];
    }

    for (; i < length; i += stride) {
        acc0 += data[i];
    }

    return ((acc0 + acc1) + (acc2 + acc3));
}

double run(int size, int stride, double Mhz) {
    double cycles;
    int elems = size / sizeof(double);

    read(elems, stride);
    cycles = fcyc2();
}

int mountain() {

}

#endif //CSAPPLEARNING_MOUNTAIN_H
