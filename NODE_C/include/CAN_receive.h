#ifndef CANRECEIVE_H
#define CANRECEIVE_H

#include <mcp_can.h>
#include <SPI.h>

extern long unsigned int rxId;
extern unsigned char len;
extern unsigned char rxBuf[8];
extern char msgString[128];

extern MCP_CAN CAN0;

void CAN_setup();
void CAN_loop();

#endif