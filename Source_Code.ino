#include <Ethernet.h>
#include <Servo.h>

Servo alfa;

//Informações sobre IP, Gateway, mascara de rede e mac
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F }; // <-- troque pelo seu MAC
byte ip[] = { 127, 0, 0, 2 }; // <-- seu ip
byte gateway[] = { 127, 0, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer server(80);
String readString;

void setup() {
  
  //atribuição do servo
  alfa.attach(9);
  
  //iniciar ethernet shield
  Ethernet.begin(mac, ip);
  server.begin();
  
  //coisas de serial
  Serial.begin(9600);
  Serial.println("Servidor Servo");
  
}

void loop() {
  // Essa parte eu não alterei nada
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        
        if (readString.length() < 100) {

          
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); 

      client.println("HTTP/1.1 200 OK"); 
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Controle de energia Arduino</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>Controle de energia</H1>");
          
          client.println("<a href=\"/?on\"\">Ligado</a>"); 
          client.println("<a href=\"/?off\"\">Desligado</a>"); 

          client.println("</BODY>");
          client.println("</HTML>");
 
          delay(1);
          
          client.stop();

              // controle do arduino
          if(readString.indexOf("?on") >0)//checks for on
          {
            alfa.write(80); // mude para o angulo de preferencia
            Serial.println("Energia Ligada");
          }
          if(readString.indexOf("?off") >0)//checks for off
          {
            alfa.write(180); // mude para o angulo de preferencia
            Serial.println("Energia Desligada");
          }
          
          readString="";

        }
      }
    }
  }
} 
