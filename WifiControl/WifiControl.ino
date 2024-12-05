// Load Wi-Fi library
#include <WiFi.h>

// Network credentials Here
const char* ssid     = "SmartShelves";
const char* password = "password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Output to Arduino
const int readyPin = 21;        // HIGH when button is pressed, Low when arduino sends an ACK
const int shelfPin = 22;        // LOW = Bottom, HIGH = TOP
const int directionPin = 19;    // LOW = Left, HIGH = Right

// Input from Arduino
const int isSpinningPin = 26;   // Indicates whether the shelf is currently spinning
const int ack = 25;             // Indicates whether the arduino has acknowledged the readyPin

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  // Set Input Pins
  pinMode(isSpinningPin, INPUT);
  pinMode(ack, INPUT);

  // Set Output Pins
  pinMode(readyPin, OUTPUT);
  pinMode(shelfPin, OUTPUT);
  pinMode(directionPin, OUTPUT);

  digitalWrite(readyPin, LOW);
  digitalWrite(shelfPin, LOW);
  digitalWrite(directionPin, LOW);


  WiFi.softAP(ssid,password);
  
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  // Sets readyPin low when it is acknowledged
  if (digitalRead(ack) == HIGH) {
    digitalWrite(readyPin, LOW);
  }

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Sets the outputs if any buttons are pressed when the shelf is not spinning
            if (digitalRead(isSpinningPin) == LOW) {
              if (header.indexOf("GET /topleft/on") >= 0) {
                digitalWrite(shelfPin, HIGH);
                digitalWrite(directionPin, LOW);
                digitalWrite(readyPin, HIGH);
              } else if (header.indexOf("GET /topRight/on") >= 0) {
                digitalWrite(shelfPin, HIGH);
                digitalWrite(directionPin, HIGH);
                digitalWrite(readyPin, HIGH);
              } else if (header.indexOf("GET /botLeft/on") >= 0) {
                digitalWrite(shelfPin, LOW);
                digitalWrite(directionPin, LOW);
                digitalWrite(readyPin, HIGH);
              } else if (header.indexOf("GET /botRight/on") >= 0) {
                digitalWrite(shelfPin, LOW);
                digitalWrite(directionPin, HIGH);
                digitalWrite(readyPin, HIGH);
              }
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; justify-content: center; }");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".full-width { grid-column: span 2; text-align: center;} </style></head>");

            client.println("<body><h1>Smart Shelves</h1>");
            
            if (digitalRead(isSpinningPin) == LOW) {
              // Displays buttons when shelf is not spinning
              client.println("<h2>Top Shelf<h2>");
              client.println("<div class=\"grid\">");
              client.println("<p><a href=\"/topleft/on\"><button class=\"button\"> &lt; </button></a></p>");
              client.println("<p><a href=\"/topRight/on\"><button class=\"button\"> &gt; </button></a></p>");
              client.println("</div>");
              client.println("<h2>Bottom Shelf<h2>");
              client.println("<div class=\"grid\">");
              client.println("<p><a href=\"/botLeft/on\"><button class=\"button\"> &lt; </button></a></p>");
              client.println("<p><a href=\"/botRight/on\"><button class=\"button\"> &gt; </button></a></p>");
              client.println("</div>");
            } else {
              // Displays Spinning text and refresh button when shelf is spinning
              client.println("<h2>Spinning...<h2>");
              client.println("<h3>Press the Refresh Button when the shelf has finished spinning<h3>");
              client.println("<p><a href=\"/refresh\"><button class=\"button\">Refresh</button></a></p>");
            }
            
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();

            // Sets readyPin low when it is acknowledged
            if (digitalRead(ack) == HIGH) {
              digitalWrite(readyPin, LOW);
            }

            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}