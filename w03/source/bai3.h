/**
 * Author: Hai Binh Nguyen #20020189
 * Description: This sub-exercise is the 3.3 exercise. In this sub-exercise, I
 * will implement a method to serialize a packet to its JSON presentation to
 * eventually fulfill the requirements:
 * - Input packet information
 * - Serialize the packet and write to pkt_hdr_json.txt file
 * - Compare file size of pkt_hdr_json.txt with its corresponding pkt_hdr.bin
 * file
 */

#ifndef NGUYENHAIBINH_3_BAI3_H
#define NGUYENHAIBINH_3_BAI3_H

#include "bai1.h"

void bai3();

void packetToJson(udphdr *packet, char *buf);

#endif //NGUYENHAIBINH_3_BAI3_H
