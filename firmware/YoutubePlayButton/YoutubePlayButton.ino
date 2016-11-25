
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>


#define DEBUG true
Adafruit_IS31FL3731 ledmatrix = Adafruit_IS31FL3731();



int GPIO0 = 3;
int ESP_RST = 4;
int CH_PD = 9;
int GPIO2 = 8;
int photocell = A3;
int but1 = A5;
int but2 = 10;
int pageTime = 3000;
String titleWord = "YouTube Play Button";
String Connect = "AT+CWJAP=";
String NETWORK = "XXXXXXXXXXXX"; ///Put your network name here
String PASS = "XXXXXXXXX";      ///Put network pass here



// The lookup table to make the brightness changes be more visible
uint8_t sweep[] = {1, 2, 3, 4, 6, 8, 10, 15, 20, 30, 40, 60, 60, 40, 30, 20, 15, 10, 8, 6, 4, 3, 2, 1};

void setup() {
  SerialUSB.begin(115200);
  Serial1.begin(115200);
  //while (!SerialUSB);

  pinMode(GPIO0, OUTPUT);
  pinMode(GPIO2, INPUT);
  pinMode(CH_PD, OUTPUT);
  pinMode(ESP_RST, INPUT_PULLUP);
  pinMode(photocell, INPUT);
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  attachInterrupt(but2, esp_RST, FALLING);


  //esp_RST();

  digitalWrite(GPIO0, LOW);
  digitalWrite(CH_PD, LOW);
  delay(1000);
  digitalWrite(CH_PD, HIGH);
  //digitalWrite(ESP_RST, HIGH);

  
  ledmatrix.begin();
  ledmatrix.setTextSize(1);
  ledmatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  ledmatrix.setTextColor(50);
  ledmatrix.clear();
  ledmatrix.setCursor(0, 0);
  ledmatrix.print("AYPB");

                  sendData("AT+RST\r\n", 2000, DEBUG); // reset module
                  sendData("AT+CWMODE=3\r\n", 1000, DEBUG); // configure as access point

                  Connect += "\"";
                  Connect += NETWORK;
                  Connect += ",\"";
                  Connect += "\r\n";
                  sendData(Connect,10000,DEBUG);
                  sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
                  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
                  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80

                  randomSeed(analogRead(0));

                }

                  void loop() {
                  // animate over all the pixels, and set the brightness from the sweep table
                  //for (uint8_t incr = 0; incr < 24; incr++)
                  int x = random(15);
                  int y = random(7);
                  int x1 = random(15);
                  int y1 = random(7);

                  //checkConnect();
                  checkValue();


                  //ledmatrix.drawPixel(x, y, 255);
                  //pixelFlash(x,y);
                  //ledmatrix.drawChar(x,y,'a',255, 10, 5);
                  //ledmatrix.drawLine(x,y,x1,y1,105);
                  //delay(250);
                  //ledmatrix.drawLine(x,y,x1,y1,0);
                  //ledmatrix.fillScreen(10);
                  int bright = analogRead(photocell);
                  bright = map(bright, 1024, 0, 150, 10);

                  ledmatrix.setTextSize(1);
                  ledmatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
                  ledmatrix.setTextColor(bright);
                  int displength = titleWord.length();
                  for (int8_t x = 18; x >= (-1*displength*6); x--) {
                  ledmatrix.clear();
                  ledmatrix.setCursor(x, 0);
                  ledmatrix.print(titleWord);
                  delay(50);
                }
                }

                  void pixelFlash(int x, int y) {
                  ledmatrix.drawPixel(x, y, 10);
                  ledmatrix.drawPixel(x, y, 20);
                  ledmatrix.drawPixel(x, y, 50);
                  ledmatrix.drawPixel(x, y, 100);
                  ledmatrix.drawPixel(x, y, 150);
                  ledmatrix.drawPixel(x, y, 200);
                  ledmatrix.drawPixel(x, y, 255);
                  ledmatrix.drawPixel(x, y, 255);
                  ledmatrix.drawPixel(x, y, 255);
                  ledmatrix.drawPixel(x, y, 200);
                  ledmatrix.drawPixel(x, y, 100);
                  ledmatrix.drawPixel(x, y, 50);
                  ledmatrix.drawPixel(x, y, 0);

                }

                  void esp_UART(){
                  if (SerialUSB.available()) // If data is sent to the monitor
                  {
                  String toSend = ""; // Create a new string
                  while (SerialUSB.available()) // While data is available
                  {
                  // Read from SerialUSB and add to the string:
                  toSend += (char)SerialUSB.read();
                }
                  // Print a message stating what we're sending:
                  //SerialUSB.print(toSend);

                  // Send the assembled string out over the hardware
                  // Serial1 port (TX pin 1).
                  Serial1.print(toSend);
}

if (Serial1.available()) // If data is sent from device
{
  String toSend = ""; // Create a new string
  while (Serial1.available()) // While data is available
  {
    // Read from hardware port and add to the string:
    toSend += (char)Serial1.read();
  }
  // Print a message stating what we've received:
  SerialUSB.print(toSend);
}
//delay(1000);
}

void esp_RST() {
  pinMode(ESP_RST, INPUT);
  pinMode(ESP_RST, OUTPUT);
  digitalWrite(ESP_RST, LOW);
  delay(100);
  titleWord = "RESET!!!";
  pinMode(ESP_RST, INPUT);
  delay(500);
}


void checkConnect() {
  if (Serial1.available()) // check if the esp is sending a message
  {
    /*
      while(Serial1.available())
      {
      // The esp has data so display its output to the serial window
      char c = Serial1.read(); // read the next character.
      SerialUSB.write(c);
      } */

    if (Serial1.find("+IPD,"))
    {
      delay(1000);

      int connectionId = Serial1.read() - 48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48

      String webpage = "<html><h1>Hello</h1><h2>World!</h2><button>LED1</button></html>";

      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";

      sendData(cipSend, pageTime, DEBUG);
      sendData(webpage, pageTime, DEBUG);

      webpage = "<html><button>LED2</button></html>";

      cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";

      sendData(cipSend, pageTime, DEBUG);
      sendData(webpage, pageTime, DEBUG);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";

      sendData(closeCommand, 3000, DEBUG);
    }
  }
}

void checkValue() {
  if (Serial1.available()) // check if the esp is sending a message
  {

    /* while(Serial1.available())
      {
      // The esp has data so display its output to the serial window
      char c = Serial1.read(); // read the next character.
      SerialUSB.write(c);
      }
    */
    if (Serial1.find("+IPD,"))
    {
      delay(1000);

      int connectionId = Serial1.read() - 48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48
      //SerialUSB.print(connectionId);
      //int forwardslash = connectionId.indexOf('/');
      //int space = connectionId.indexOf(' ',forwardslash);
      //String subs = connectionId;
      //subs.remove(space);
      //subs.remove(1,forwardslash);
      //titleWord = subs;
      String webpage = "<html>Value Stored</html>";

      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";

      String subs = sendData(cipSend, pageTime, DEBUG);
      int forwardslash = subs.indexOf('/');
      int space = subs.indexOf(' ', forwardslash);

      subs.remove(space);
      subs.remove(0, forwardslash + 1);
      titleWord = subs;

      sendData(webpage, pageTime, DEBUG);

      String closeCommand = "AT+CIPCLOSE=0";
      //closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";

      sendData(closeCommand, 3000, DEBUG);
    }
  }
}


String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  Serial1.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {

      // The esp has data so display its output to the serial window
      char c = Serial1.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    SerialUSB.print(response);
  }

  return response;
}


