/**
 * Author: Hai Binh Nguyen #20020189
 * Description: This sub-exercise is the 3.1 exercise. In this sub-exercise, I
 * will declare the packet structure, implement packet setters and a method to
 * serialize a packet to a byte array to eventually fulfill the requirements:
 * - Input packet information
 * - Save inputted information to a packet structure
 * - Serialize the packet and write to pkt_hdr.bin file
 * - Print pkt_hdr.bin's size
 */

#ifndef NGUYENHAIBINH_3_BAI1_H
#define NGUYENHAIBINH_3_BAI1_H

#define MAX_NAME_LENGTH 100
#define ushort unsigned short
#define byte unsigned char

void bai1();

static const int FILE_TYPE_TEXT = 0x00;
static const int FILE_TYPE_BIN = 0x01;

typedef struct {
    ushort fileSize;
    byte fileType;
    byte fileNameLength;
    char fileName[MAX_NAME_LENGTH];
} udphdr;

udphdr *createPacket();

void packetToBytes(udphdr *packet, byte *buf);

void setPacketFileType(udphdr *packet, byte type);

void setPacketFileSize(udphdr *packet, ushort fileSize);

void setPacketFileName(udphdr *packet, byte fileNameLength, byte *buf);

#endif //NGUYENHAIBINH_3_BAI1_H
