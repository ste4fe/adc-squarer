#include "util.h"

float util::calculateSqare(float value) {
    float sqrt = value;
    float temp = 0;

    while (sqrt != temp) {
        temp = sqrt;
        sqrt = (value / temp + temp) / 2;
    }

    return sqrt;
}

void util::halfWord2AsciiString(unsigned short number, char *dst) {
    int pos = 0;

    if (number == 0) {
        dst[pos++] = '0';
        dst[pos] = '\0';
        return;
    }

    while (number > 0) {
        dst[pos++] = (number % 10) + '0';
        number /= 10;
    }

    dst[pos] = '\0';

    for (int i = 0; i < pos / 2; i++) {
        char temp = dst[i];
        dst[i] = dst[pos - i - 1];
        dst[pos - i - 1] = temp;
    }
}

void util::float2Ascii(float number, char *dst) {
    int intPart = (int)number; 
    float fracPart = number - intPart;  
    int fracInt;
    int i = 0;

    if (intPart == 0) {
        dst[i++] = '0';
    } else {
        int temp = intPart;
        char tempStr[20];
        int j = 0;
        while (temp > 0) {
            tempStr[j++] = (temp % 10) + '0';
            temp /= 10;
        }
        while (j > 0) {
            dst[i++] = tempStr[--j];
        }
    }

    dst[i++] = '.';

    fracInt = (int)(fracPart * 1000000);
    if (fracInt == 0) {
        dst[i++] = '0';
    } else {
        int temp = fracInt;
        char fracStr[20];
        int j = 0;
        while (temp > 0) {
            fracStr[j++] = (temp % 10) + '0';
            temp /= 10;
        }
        while (j > 0) {
            dst[i++] = fracStr[--j];
        }
    }

    dst[i] = '\0';
}
