
#define valueOf(key) atof(textOf(key))

const byte led = 5;

const byte numChars = 70;
char receivedChars[numChars];

boolean newData = false;
boolean RecAtLeast1Time = false;
char *input;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
    recvWithStartEndMarkers();

    if(RecAtLeast1Time==true){
        input = textOf("CUSTOM");
        analogWrite(led,valueOf("Y"));
        Serial.println(input);
        newData = false;
    }
}


char* textOf(const char *key){
    char* pivot = strstr (receivedChars,key);
    char* result;

    if(pivot != NULL){
        result = strtok(pivot+strlen(key)+1, ",}");    
    }
    return result;
}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '{';
    char endMarker = '}';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
                RecAtLeast1Time = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}
