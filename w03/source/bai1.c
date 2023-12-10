//
// Created by hbinhng on 10/9/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bai1.h"

udphdr *createPacket() {
    udphdr *packet = (udphdr *) malloc(sizeof(udphdr));

    packet->fileType = FILE_TYPE_BIN;
    packet->fileNameLength = 0;

    memset(packet->fileName, 0, MAX_NAME_LENGTH);

    return packet;
}

void setPacketFileType(udphdr *packet, byte type) {
    switch (type) {
        case 't':
        case 'T':
            packet->fileType = FILE_TYPE_TEXT;
            break;

        case 'b':
        case 'B':
            packet->fileType = FILE_TYPE_BIN;
            break;

        default:
            fprintf(stderr, "\nUnknown file type");
            exit(0x0101);
    }
}

void setPacketFileSize(udphdr *packet, ushort fileSize) {
    packet->fileSize = fileSize;
}

void setPacketFileName(udphdr *packet, byte fileNameLength, byte *buf) {
    memcpy(packet->fileName, buf, fileNameLength);
    packet->fileNameLength = fileNameLength;
}

void packetToBytes(udphdr *packet, byte *buf) {
    memcpy(buf, packet, 4 + packet->fileNameLength);
}

void readFileName_b1(udphdr *packet) {
    byte buf[MAX_NAME_LENGTH], ch;
    short cursor = 0;

    while (cursor <= MAX_NAME_LENGTH) {
        ch = getchar();

        if (ch == '\n' || ch == '\r') {
            if (cursor == 0) continue;
            break;
        }

        if (cursor == MAX_NAME_LENGTH) {
            fprintf(stderr, "\nFile name exceeds %d characters", MAX_NAME_LENGTH);
            exit(0x0102);
        }

        buf[cursor++] = ch;
    }

    setPacketFileName(packet, cursor, buf);
}

void bai1() {
    udphdr *packet = createPacket();

    byte fileType;
    ushort fileSize;

    printf("File type (T - text/B - binary): ");
    scanf("%c", &fileType);

    setPacketFileType(packet, fileType);

    printf("File size: ");
    scanf("%hu", &fileSize);

    setPacketFileSize(packet, fileSize);

    printf("File name: ");

    readFileName_b1(packet);

    short packetSize = sizeof(udphdr) - MAX_NAME_LENGTH + packet->fileNameLength;
    char buf[packetSize];
    memset(buf, 0, sizeof(char) * packetSize);
    packetToBytes(packet, (byte *) buf);

    FILE *file = fopen("pkt_hdr.bin", "w");

    for (int i = 0; i < packetSize; i++)
        fputc(buf[i], file);

    printf("Packet size (file size): %d\n", packetSize);

    fclose(file);
    free(packet);
}