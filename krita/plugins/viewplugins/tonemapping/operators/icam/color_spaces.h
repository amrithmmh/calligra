#include <math.h>
/*
 * sRGB transform
 */

double MXYZ[3][3]       = {{ 41.2381, 35.7573, 18.0452},
    { 21.1620, 71.5139,  7.2150},
    {  1.9343, 11.9212, 95.0507}
};
double MXYZinv[3][3]    = {{ 0.0324, -0.0154, -0.0050},
    { -0.0097,  0.0188,  0.0004},
    { 0.0006, -0.0020,  0.0106}
};

/*
 * Mcat02
 */

double Mcat[3][3]       = {{ 0.7328,    0.4296,   -0.1624},
    { -0.7036,    1.6975,    0.0061},
    { 0.0030,    0.0136,    0.9834}
};
double Mcatinv[3][3]    = {{ 1.096124, -0.278869,  0.182745},
    { 0.454369,  0.473533,  0.072098},
    { -0.009628, -0.005698,  1.015326}
};
/*
 * Mcat02 version b
 */

double Mcatb[3][3]       = {{ 0.8562,    0.3372,   -0.1934},
    { -0.8360,    1.8327,    0.0033},
    { 0.0357,   -0.0469,    1.0112}
};
double Mcatbinv[3][3]    = {{ 0.9874,   -0.1768,    0.1894},
    { 0.4504,    0.4649,    0.0846},
    { -0.0140,    0.0278,    0.9862}
};

/*
 * XYZ to LMS transform
 */

double LMS [3][3]       = {{ 0.4002,    0.7077,   -0.0807},
    { -0.2280,    1.1500,    0.0612},
    {      0,         0,    0.9184}
};
double LMSinv [3][3]    = {{ 1.8501,   -1.1385,    0.2384},
    { 0.3668,    0.6438,   -0.0107},
    {      0,         0,    1.0889}
};
