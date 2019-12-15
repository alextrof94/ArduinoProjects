
    byte Speed_H,Speed_L;
    Speed_L = Speed;
    if (Rotation == 0){                         // Move Left
        Speed_H = Speed >> 8;

    }else if (Rotation == 1){                    // Move Right
        Speed_H = (Speed >> 8)+4;
    }

    Instruction_Packet_Array[0] = ID;
    Instruction_Packet_Array[1] = WHEEL_LENGTH;
    Instruction_Packet_Array[2] = COMMAND_WRITE_DATA;
    Instruction_Packet_Array[3] = RAM_GOAL_SPEED_L;
    Instruction_Packet_Array[4] = Speed_L;
    Instruction_Packet_Array[5] = Speed_H;

    clearRXbuffer();

    transmitInstructionPacket();
}
