#include <ESP8266WiFi.h>
const char* ssid = "Prototype";
const char* password = "Passcode";
//In password You Can enter your own password.


WiFiServer server(80);

String header;

String outputRedState = "off";
String outputGreenState = "off";
String outputYellowState = "off";


const int redLED = 2;
const int greenLED = 4;
const int yellowLED = 5;

unsigned long currentTime = millis();

unsigned long previousTime = 0; 

const long timeoutTime = 2000;

void setup() {
Serial.begin(115200);

pinMode(redLED, OUTPUT);
pinMode(greenLED, OUTPUT);
pinMode(yellowLED,OUTPUT);
// Set outputs to LOW
digitalWrite(redLED, LOW);
digitalWrite(greenLED, LOW);
digitalWrite(yellowLED, LOW);

Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){
WiFiClient client = server.available(); // Listen for incoming clients

if (client) { // If a new client connects,
Serial.println("New Client."); // print a message out in the serial port
String currentLine = ""; // make a String to hold incoming data from the client
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
currentTime = millis(); 
if (client.available()) { // if there's bytes to read from the client,
char c = client.read(); // read a byte, then
Serial.write(c); // print it out the serial monitor
header += c;
if (c == '\n')
{
if (currentLine.length() == 0)
{
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

// turns the GPIOs on and off
if (header.indexOf("GET /2/on") >= 0) {
Serial.println("RED LED is on");
outputRedState = "on";
digitalWrite(redLED, HIGH);
}
else if (header.indexOf("GET /2/off") >= 0) {
Serial.println("RED LED is off");
outputRedState = "off";
digitalWrite(redLED, LOW);
}
else if (header.indexOf("GET /4/on") >= 0) {
Serial.println("Green LED is on");
outputGreenState = "on";
digitalWrite(greenLED, HIGH);
}
else if (header.indexOf("GET /4/off") >= 0) {
Serial.println("Green LED is off");
outputGreenState = "off";
digitalWrite(greenLED, LOW);
}
else if (header.indexOf("GET /5/on") >= 0) {
Serial.println("Yellow LED is on");
outputYellowState = "on";
digitalWrite(yellowLED, HIGH);
}
else if (header.indexOf("GET /5/off") >= 0) {
Serial.println("Yellow LED is off");
outputYellowState = "off";
digitalWrite(yellowLED, LOW);
}
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
// CSS to style the on/off buttons 
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;background-color: #808080;}");
client.println(".buttonRed { background-color: #ff0000; border: none; color: black; padding: 16px 40px; border-radius: 80%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".buttonGreen { background-color: #00ff00; border: none; color: black; padding: 16px 40px; border-radius: 80%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".buttonYellow { background-color: #feeb36; border: none; color: black; padding: 16px 40px; border-radius: 80%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".buttonOff { background-color: #77878A; border: none; color: black; padding: 16px 40px; border-radius: 80%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");


client.println("<body><h1>Rana's Home</h1>");
client.println("<body><h2>Developed By Nikhilesh Rana</h2>");

// Display current state, and ON/OFF buttons for GPIO 2 Red LED 
client.println("<p>Red LED is " + outputRedState + "</p>");
// If the outputRedState is off, it displays the OFF button
if (outputRedState=="off") {
client.println("<p><a href=\"/2/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/2/off\"><button class=\"button buttonRed\">ON</button></a></p>");
} 

// Display current state, and ON/OFF buttons for GPIO 4 Green LED 
client.println("<p>Green LED is " + outputGreenState + "</p>");
// If the outputGreenState is off, it displays the OFF button 
if (outputGreenState =="off") {
client.println("<p><a href=\"/4/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
}
else
{
client.println("<p><a href=\"/4/off\"><button class=\"button buttonGreen\">ON</button></a></p>");
}
client.println("</body></html>");

// Display current state, and ON/OFF buttons for GPIO 5 Yellow LED 
client.println("<p>Yellow LED is " + outputYellowState + "</p>");
// If the outputYellowState is off, it displays the OFF button 
if (outputYellowState =="off") {
client.println("<p><a href=\"/5/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/5/off\"><button class=\"button buttonYellow\">ON</button></a></p>");
}
client.println("</body></html>");


client.println();

break;
} else { // if you got a newline, then clear currentLine
currentLine = "";
}
} else if (c != '\r') { // if you got anything else but a carriage return character,
currentLine += c; // add it to the end of the currentLine
}
}
}

header = "";

client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}
