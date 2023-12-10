//
// Created by hbinhng on 10/10/2023.
//

#include "bai3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void packetToJson(udphdr *packet, char *buf) {
    sprintf(
            buf,
            "{\"fileSize\":%d,\"fileType\":\"%c\",\"fileName\":\"%s\"}",
            packet->fileSize,
            packet->fileType == FILE_TYPE_TEXT ? 'T' : 'B',
            packet->fileName
    );
}

void readFileName_b3(udphdr *packet) {
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
            exit(0x0301);
        }

        buf[cursor++] = ch;
    }

    setPacketFileName(packet, cursor, buf);
}

void bai3() {
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

    readFileName_b3(packet);

    short packetSize = sizeof(udphdr) - MAX_NAME_LENGTH + packet->fileNameLength;
    char buf[200];
    memset(buf, 0, 200);
    packetToJson(packet, buf);

    FILE *file = fopen("pkt_hdr_json.txt", "w");

    fprintf(file, "%s", buf);

    printf(
            "File size (JSON): %d - compared to packet size (binary): %d\n",
            (int) strlen(buf),
            packetSize
    );

    fclose(file);
    free(packet);
}