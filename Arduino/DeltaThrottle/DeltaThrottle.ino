#include <Dynamixel_Serial.h> 
#define HARDWARE_SERVOS_COUNT 1
#define PIN_SERVOS_CONTROL 4
#define PC Serial
unsigned int ServosPoses[HARDWARE_SERVOS_COUNT];
unsigned int ServosLoads[HARDWARE_SERVOS_COUNT];
unsigned int ServosReady[HARDWARE_SERVOS_COUNT];
unsigned int ServosV[HARDWARE_SERVOS_COUNT];
unsigned int ServosT[HARDWARE_SERVOS_COUNT];

void setup() {
  ServosInit();
  PC.begin(57600);
}

void loop() {
  ServosGo();
  delay(100);
  ServosGetPositions();
  ServosGetLoads();
  ServosGetReady();
  ServosGetV();
  ServosGetT();
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    PC.print("SERVO "); PC.print(i + 1); PC.print(": "); 
    if (ServosPoses[i] == 0xF000)
      PC.println("GOT ERROR");
    PC.print(ServosPoses[i]); PC.print(" "); 
    PC.print(ServosLoads[i]); PC.print(" ");
    PC.print(ServosReady[i]); PC.print(" ");
    PC.print(ServosV[i]); PC.print(" ");
    PC.print(ServosT[i]); PC.print(" ");
    PC.println();
  }
  PC.println();
}
