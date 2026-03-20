#include <WiFi.h>

#define ENA 33
#define IN1 25
#define IN2 26

#define ENB 32
#define IN3 27
#define IN4 14

const char* ssid = "Submarine";
const char* password = "12345678";

WiFiServer server(80);

int speedA = 200;
int speedB = 200;

void setup()
{
Serial.begin(115200);

pinMode(ENA,OUTPUT);
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);

pinMode(ENB,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);

WiFi.softAP(ssid,password);
server.begin();

Serial.println(WiFi.softAPIP());
}


// -------- MOTOR FUNCTIONS --------

void forward()
{
analogWrite(ENA,speedA);
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);
}

void backward()
{
analogWrite(ENA,speedA);
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);
}

void left()
{
analogWrite(ENB,speedB);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
}

void right()
{
analogWrite(ENB,speedB);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
}


// -------- STOP FUNCTIONS --------

void stopA()
{
analogWrite(ENA,0);
digitalWrite(IN1,LOW);
digitalWrite(IN2,LOW);
}

void stopB()
{
analogWrite(ENB,0);
digitalWrite(IN3,LOW);
digitalWrite(IN4,LOW);
}

void stopAll()
{
stopA();
stopB();
}


// -------- LOOP --------

void loop()
{
WiFiClient client = server.available();

if(client)
{
String request = client.readStringUntil('\r');
client.flush();


// movement control
if(request.indexOf("/SA")!=-1) stopA();
else if(request.indexOf("/SB")!=-1) stopB();
else if(request.indexOf("/S")!=-1) stopAll();
else if(request.indexOf("/F")!=-1) forward();
else if(request.indexOf("/B")!=-1) backward();
else if(request.indexOf("/L")!=-1) left();
else if(request.indexOf("/R")!=-1) right();


// speed A
int indexA = request.indexOf("speedA=");
if(indexA != -1)
{
String value = request.substring(indexA + 7);
speedA = value.toInt();

if(speedA > 255) speedA = 255;
if(speedA < 0) speedA = 0;
}


// speed B
int indexB = request.indexOf("speedB=");
if(indexB != -1)
{
String value = request.substring(indexB + 7);
speedB = value.toInt();

if(speedB > 255) speedB = 255;
if(speedB < 0) speedB = 0;
}


// -------- WEB PAGE --------

client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println();

client.println("<h1>ESP32 Submarine Control</h1>");

client.println("<a href=\"/F\"><button style='width:200px;height:60px'>Forward</button></a><br><br>");
client.println("<a href=\"/B\"><button style='width:200px;height:60px'>Backward</button></a><br><br>");

client.println("<a href=\"/L\"><button style='width:200px;height:60px'>Left</button></a><br><br>");
client.println("<a href=\"/R\"><button style='width:200px;height:60px'>Right</button></a><br><br>");

client.println("<a href=\"/SA\"><button style='width:200px;height:60px;background:red'>Stop Motor A</button></a><br><br>");
client.println("<a href=\"/SB\"><button style='width:200px;height:60px;background:red'>Stop Motor B</button></a><br><br>");

client.println("<a href=\"/S\"><button style='width:200px;height:60px;background:black;color:white'>Stop ALL</button></a><br><br>");

client.println("<h3>Back Motor Speed (0-255)</h3>");
client.println("<form action=\"/\" method=\"GET\">");
client.println("<input type=\"number\" name=\"speedA\" min=\"0\" max=\"255\" value=\"200\">");
client.println("<input type=\"submit\" value=\"Set Speed A\">");
client.println("</form>");

client.println("<h3>Turning Motor Speed (0-255)</h3>");
client.println("<form action=\"/\" method=\"GET\">");
client.println("<input type=\"number\" name=\"speedB\" min=\"0\" max=\"255\" value=\"200\">");
client.println("<input type=\"submit\" value=\"Set Speed B\">");
client.println("</form>");

client.stop();
}
}
hii
