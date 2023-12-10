/**
 * Author: Hai Binh Nguyen #20020189
 * Description: This sub-exercise is the 3.4 exercise. In this sub-exercise, I
 * will implement a method to deserialize a JSON string back to our packet
 * structure to eventually fulfill the requirements:
 * - Read data from pkt_hdr_json.txt file from sub-exercise 4
 * - Print data from the file specified in fileName field of the packet
 */

#ifndef NGUYENHAIBINH_3_BAI4_H
#define NGUYENHAIBINH_3_BAI4_H

#include "bai1.h"

void bai4();

udphdr *packetFromJson(char *buf);

#endif //NGUYENHAIBINH_3_BAI4_H
