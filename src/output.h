#define MBED_OUTPUT_H
#include <string.h>
#include <mbed.h>

class Output{
    public:
        Output(int valX, int valY, char boto);
        void printResult();

    private:
        char resultat[200];
        char x[10];
        char y[10];
        char xReal[10];
        char yReal[10];
        int valX;
        int valY;
        char boto;
};

