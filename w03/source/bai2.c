//
// Created by hbinhng on 10/10/2023.
//

#include "bai2.h"
#include <stdio.h>
#include <string.h>

udphdr *packetFromBytes(char *buf, short packetSize) {
    udphdr *packet = createPacket();

    memcpy(&packet->fileSize, buf, 2);
    memcpy(&packet->fileType, buf + 2, 1);
    memcpy(&packet->fileNameLength, buf + 3, 1);
    memcpy(packet->fileName, buf + 4, packet->fileNameLength);

    return packet;
}

void packetToString(udphdr *packet, char *buf) {
    sprintf(
            buf,
            "udphdr(File size=%d, File type=%c, File name length=%d, File name=\"%s\")",
            packet->fileSize,
            packet->fileType == FILE_TYPE_TEXT ? 'T' : 'B',
            packet->fileNameLength,
            packet->fileName
    );
}

void bai2() {
    int MAX_PACKET_SIZE = MAX_NAME_LENGTH + 4;
    char buf[MAX_PACKET_SIZE + 1];
    FILE *file = fopen("pkt_hdr.bin", "r");

    short packetSize = (short) fread(buf, sizeof(byte), MAX_PACKET_SIZE, file);

    udphdr *packet = packetFromBytes(buf, packetSize);
    memset(buf, 0, sizeof(buf));

    packetToString(packet, buf);

    printf("Packet: %s", buf);

    fclose(file);
}