//Author		: Xiao Tuzi
//Date(Create)	: 08/28/2022
//Description	: this program is definition of calculating strain extrapolation matrix(3D8)


#include <cmath>
#include "Element/Tools/calStrainExtraMat.h"

void calStrainExtraMat(dMatrix& strainExtraMat) {
	// calculate strain extrapolation matrix(integra point to node)
	double a, b, c, d;

    a = (5.0 + 3.0 * sqrt(3.0)) / 4.0;
    b = -(sqrt(3.0) + 1.0) / 4.0;
    c = (sqrt(3.0) - 1.0) / 4.0;
    d = (5.0 - 3.0 * sqrt(3.0)) / 4.0;

    strainExtraMat(0, 0) = a;
    strainExtraMat(0, 1) = b;
    strainExtraMat(0, 2) = c;
    strainExtraMat(0, 3) = b;
    strainExtraMat(0, 4) = b;
    strainExtraMat(0, 5) = c;
    strainExtraMat(0, 6) = d;
    strainExtraMat(0, 7) = c;
    strainExtraMat(1, 1) = a;
    strainExtraMat(1, 2) = b;
    strainExtraMat(1, 3) = c;
    strainExtraMat(1, 4) = c;
    strainExtraMat(1, 5) = b;
    strainExtraMat(1, 6) = c;
    strainExtraMat(1, 7) = d;
    strainExtraMat(2, 2) = a;
    strainExtraMat(2, 3) = b;
    strainExtraMat(2, 4) = d;
    strainExtraMat(2, 5) = c;
    strainExtraMat(2, 6) = b;
    strainExtraMat(2, 7) = c;
    strainExtraMat(3, 3) = a;
    strainExtraMat(3, 4) = c;
    strainExtraMat(3, 5) = d;
    strainExtraMat(3, 6) = c;
    strainExtraMat(3, 7) = b;
    strainExtraMat(4, 4) = a;
    strainExtraMat(4, 5) = b;
    strainExtraMat(4, 6) = c;
    strainExtraMat(4, 7) = b;
    strainExtraMat(5, 5) = a;
    strainExtraMat(5, 6) = b;
    strainExtraMat(5, 7) = c;
    strainExtraMat(6, 6) = a;
    strainExtraMat(6, 7) = b;
    strainExtraMat(7, 7) = a;

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < i; j++) {
            strainExtraMat(i, j) = strainExtraMat(j, i);
        }
    }
}