#define EXT Serial2

ExternalTypes externalType = ETDISPENSER;
bool ExternalReady = false;

void ExternalCheck() {
  if (EXT.available()) {
    char cmd = EXT.read();
    switch (cmd) {
      case 'i': // i'm externalType
        externalType = EXT.read();
        if (externalType > externalTypesCount)
          externalType = ETNONE;
        break;
      case 'r': // ready?
        ExternalReady = (EXT.read() == 1);
        break;
      case 'a': // additional
        char a = EXT.read();
        // do something
        break;
    }
  }
  if (EXT.available())
    ExternalCheck(); // recursion for read all data
}


void ExternalInit() {
  EXT.begin(57600);
}
