/*
Somfy Shade Control
by Bithead942
 
 Remote control of the Somfy Shades hanging in the front room and dining room.
 
 All shaded run from a single channel controler.
 
 To control, a command in the following format must be sent:
 
 Request: 2 integer values
 First value:  The number of the shade to control (for now, only 3)
 Second value:  0:  Down pulse (Down and open),  1:  Middle pulse(Down and closed),  2:  Up pulse
 Termination value:  Chr(10) or Chr(13)
 
 Response: 1 integer value
 Echo back request (followed by a Chr(13))
 Prints a 0 when complete.  Prints a non-zero when an error occurs
 Error 1:  Invalid input
 
 PINS USED:
 Programming
 Tx       0
 Rx       1
 
 Shade Control
 Shade    3     4     5
 Up:      A0    7     A3  (White)
 Middle:  A1          A4  (Red)
 Down:    A2    6     A5  (Black)
 
 Ethernet control
 SD card: 4
 SPI:     10
 SPI:     11
 SPI:     12
 */
#include <SPI.h>
#include <Ethernet.h>

const int pShade3Up = A0;    
const int pShade3Middle = A1;  
const int pShade3Down = A2;
const int pShade4Up = 6;
const int pShade4Down = 7;
const int pShade5Up = A3;    
const int pShade5Middle = A4;  
const int pShade5Down = A5;
const int pLED = 9;
const int iShortDelay = 300;

String inString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int BlindNo;
int BlindUD;
int i;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
// address modified for security reasons - replace ?? with hex/dec values
byte mac[] = { 
  0x90, 0xA2, 0x??, 0x0F, 0x8A, 0x?? };
IPAddress ip(192,168,???,???);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
EthernetServer server(8888);
EthernetClient client;

void setup() {
  pinMode(pShade3Up, OUTPUT);
  pinMode(pShade3Middle, OUTPUT);
  pinMode(pShade3Down, OUTPUT);
  pinMode(pShade4Up, OUTPUT);
  pinMode(pShade4Down, OUTPUT);
  pinMode(pShade5Up, OUTPUT);
  pinMode(pShade5Middle, OUTPUT);
  pinMode(pShade5Down, OUTPUT);
  pinMode(pLED, OUTPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  inString.reserve(3);
  inString = "";

  Blink(3);
}

void loop() {
  // listen for incoming clients
  client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char inChar = client.read();
        // add it to the inputString:
        inString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == 13 or inChar == 10) 
        {
          stringComplete = true;
        } 
      }

      if (stringComplete) 
      {    
        BlindNo = int(inString.charAt(0)) - 48;
        BlindUD = int(inString.charAt(1)) - 48;
        if (BlindNo != -38 and BlindUD != -48) 
        {
          client.print(BlindNo, DEC);
          client.println(BlindUD, DEC);
          if ((BlindNo >= 3 and BlindNo <= 5) and (BlindUD >= 0 and BlindUD <= 2))
          {
            moveBlind();
          }
          else
          {
            client.println("1");  //Error, invalid input
          }
        }
        // clear the string:
        inString = "";
        stringComplete = false;
      }

    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

void moveBlind()
{

  if (BlindNo == 3)  //Living Room Shades
  {
    if (BlindUD == 0)  //Need to go down:  50%
    {
      digitalWrite(pShade3Down, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade3Down, LOW);
    }
    if (BlindUD == 1)  //Need to go middle:  100%
    {
      digitalWrite(pShade3Middle, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade3Middle, LOW);
    }
    if (BlindUD == 2)  //Need to go up:  0%
    {
      digitalWrite(pShade3Up, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade3Up, LOW);
    }
    client.println("0");
    Blink(2);
  }

  if (BlindNo == 4)  //Great Room Blinds
  {
    if (BlindUD == 1)  //Need to go up:  0%
    {
      digitalWrite(pShade4Up, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade4Up, LOW);
    }
    if (BlindUD == 2)  //Need to go down:  100%
    {
      digitalWrite(pShade4Down, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade4Down, LOW);
    }
    client.println("0");
    Blink(2);
  }
  
  if (BlindNo == 5)  //Dining Room Shades
  {
    if (BlindUD == 0)  //Need to go down:  50%
    {
      digitalWrite(pShade5Down, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade5Down, LOW);
    }
    if (BlindUD == 1)  //Need to go middle:  100%
    {
      digitalWrite(pShade5Middle, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade5Middle, LOW);
    }
    if (BlindUD == 2)  //Need to go up:  0%
    {
      digitalWrite(pShade5Up, HIGH);
      delay(iShortDelay);
      digitalWrite(pShade5Up, LOW);
    }
    client.println("0");
    Blink(2);
  }
}

void Blink(int iTimes)
{
  for (int i = 1; i <= iTimes; i++)
  {
    digitalWrite(pLED, HIGH);
    delay(100);
    digitalWrite(pLED, LOW);
    delay(100);
  }
}



