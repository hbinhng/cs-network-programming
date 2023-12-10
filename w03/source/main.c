/**
 * Author: Hai Binh Nguyen #20020189
 * Description and notice: This is the 3rd exercise in network programming
 * course. In this implementation, you (judge) will be able to execute each
 * sub-exercise using command line argument. You can have a failed run to
 * see the instruction. Each sub-exercise's description is listed in header
 * file ie. bai1.h, NOT in source file ie. bai1.c. This project runs best on
 * cmake, gcc and CLion.
 * Disclaimer: Comments like this will not be provided in Vietnamese to prevent
 * compiler failure.
 */

#include "bai1.h"
#include "bai2.h"
#include "bai3.h"
#include "bai4.h"

#include <stdio.h>
#include <stdlib.h>

short validateArgv(const char *argv)
{
    if (argv[0] != 'b') return 0;
    if (argv[1] != 'a') return 0;
    if (argv[2] != 'i') return 0;

    if (argv[3] < '1' || argv[3] > '4') return 0;

    return 1;
}

int main(int argc, const char *argv[])
{
    if (argc != 2 || !validateArgv(argv[1]))
    {
        fprintf(stderr, "No exercise specified. Usage: NguyenHaiBinh_3.exe <ten bai>\n");
        fprintf(stderr, "<ten bai> can be one of following values: bai1, bai2, bai3, bai4");
        exit(0x0001);
    }

    switch (argv[1][3])
    {
    case '1':
        bai1();
        break;

    case '2':
        bai2();
        break;

    case '3':
        bai3();
        break;

    case '4':
        bai4();
        break;

    default:
        fprintf(stderr, "Unknown exercise. Usage: NguyenHaiBinh_3.exe <ten bai>\n");
        fprintf(stderr, "<ten bai> can be one of following values: bai1, bai2, bai3, bai4");
        exit(0x0002);
    }

    return 0;
}
