/**
 * Author: Hai Binh Nguyen #20020189
 * Description: This sub-exercise is the 3.2 exercise. In this sub-exercise, I
 * will implement a method to deserialize a byte array back to our packet
 * structure and a method to serialize a packet to a descriptive form to
 * eventually fulfill the requirements:
 * - Read the byte array in pkt_hdr.bin created from sub-exercise 1
 * - Print fileSize, fileType, fileNameLength, fileName
 */

#ifndef NGUYENHAIBINH_3_BAI2_H
#define NGUYENHAIBINH_3_BAI2_H

#include "bai1.h"

void bai2();

void packetToString(udphdr *packet, char *buf);

udphdr *packetFromBytes(char *buf, short packetSize);

#endif //NGUYENHAIBINH_3_BAI2_H
