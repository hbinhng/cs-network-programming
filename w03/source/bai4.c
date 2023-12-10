#include "bai4.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY_NEXT 0
#define FILE_SIZE_NEXT 1
#define FILE_TYPE_NEXT 2
#define FILE_NAME_NEXT 3
#define FILE_SIZE 0
#define FILE_TYPE 1
#define FILE_NAME 2

udphdr *packetFromJson(char *buf)
{
    static const char *delimiter = "{}:\",";
    static const char *fields[3] = {
            "fileSize",
            "fileType",
            "fileName"
    };

    udphdr *packet = (udphdr*) malloc(sizeof(udphdr));

    char *token;
    char *bufPtr = buf;
    int whatNext = KEY_NEXT;

    do
    {
        token = strtok_r(buf, delimiter, &buf);

        if (token == NULL) break;

        if (whatNext != KEY_NEXT)
        {
            if (whatNext == FILE_SIZE_NEXT)
                packet->fileSize = atoi(token);
            if (whatNext == FILE_TYPE_NEXT)
                packet->fileType = token[0] == 'T' ? FILE_TYPE_TEXT : FILE_TYPE_BIN;
            if (whatNext == FILE_NAME_NEXT)
            {
                strcpy(packet->fileName, token);
                packet->fileNameLength = strlen(token);
            }

            whatNext = KEY_NEXT;
        }
        else {
            if (strcmp(token, fields[FILE_SIZE]) == 0)
                whatNext = FILE_SIZE_NEXT;
            else if (strcmp(token, fields[FILE_TYPE]) == 0)
                whatNext = FILE_TYPE_NEXT;
            else if (strcmp(token, fields[FILE_NAME]) == 0)
                whatNext = FILE_NAME_NEXT;
            else {
                fprintf(stderr, "Unknown property %s\n", token);
                exit(0x0401);
            }
        }
    } while (1);

    return packet;
}

void printFile(udphdr *packet)
{
    FILE *target = fopen(packet->fileName, "r");

    if (target == NULL)
    {
        fprintf(stderr, "Can not open target file (%s).\n", packet->fileName);
        exit(0x0402);
    }

    char buf[(1 << 16) + 1];

    short realFileSize = (short) fread(buf, sizeof(char), packet->fileSize, target);

    printf("File type of %s is %s, printing in %s format:\n",
           packet->fileName,
           packet->fileType == FILE_TYPE_TEXT ? "TEXT" : "BIN",
           packet->fileType == FILE_TYPE_TEXT ? "ASCII" : "HEX");

    if (packet->fileType == FILE_TYPE_TEXT)
    {
        buf[realFileSize] = '\0';

        printf("%s\n", buf);
    }
    else
    {
        for (int i = 0; i < realFileSize; i++)
            printf("%02x ", buf[i]);

        putchar('\n');
    }

    fclose(target);
}

void bai4()
{
    FILE *file = fopen("pkt_hdr_json.txt", "r");

    char buf[200];
    memset(buf, 0, sizeof(char) * 200);
    fread(buf, sizeof(char), 200, file);

    udphdr *packet = packetFromJson(buf);

    printFile(packet);

    fclose(file);
    free(packet);
}