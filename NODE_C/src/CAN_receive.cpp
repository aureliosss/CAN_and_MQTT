#include "CAN_receive.h"

long unsigned int rxId;
unsigned char len;
unsigned char rxBuf[8];
char msgString[128];

#define CAN0_INT 21


void CAN_setup() 
{
  MCP_CAN CAN0(5);
  Serial.begin(115200);
  
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);
  pinMode(CAN0_INT, INPUT);
  
  Serial.println("MCP2515 Library Receive Example...");
}

// void CAN_loop() {
//   if (!digitalRead(CAN0_INT)) {
//     CAN0.readMsgBuf(&rxId, &len, rxBuf);
    
//     if ((rxId & 0x80000000) == 0x80000000)
//       sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
//     else
//       sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
//     Serial.print(msgString);
  
//     if ((rxId & 0x40000000) == 0x40000000) {
//       sprintf(msgString, " REMOTE REQUEST FRAME");
//       Serial.print(msgString);
//     } else {
//       for (byte i = 0; i < len; i++) {
//         sprintf(msgString, " 0x%.2X", rxBuf[i]);
//         Serial.print(msgString);
//       }
//     }
        
//     Serial.println();
//   }
// }
