#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

Servo alfa;

//Informações sobre IP, Gateway, mascara de rede e mac
byte mac[] = { 0x70, 0xB3, 0xD5, 0x0A, 0xC6, 0x7B }; // <-- troque pelo seu MAC
byte ip[] = { 10, 0, 0, 155 }; // <-- seu ip
byte gateway[] = { 10, 0, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer server(80);
String readString;

int status = 0;

void setup() {
  
  
  
  //iniciar ethernet shield
  Ethernet.begin(mac, ip);
  server.begin();
  
  //coisas de serial
  Serial.begin(9600);
  Serial.println("Servidor Servo");

  //atribuição do servo
  alfa.attach(12);
  alfa.write(180);
  
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
          
          client.println("<a href=\"/?on\"\"><button>Ligar</button></a>"); 
          client.println("<a href=\"/?off\"\"><button>Desligar</button></a><br>"); 
          client.println("<br>: ");

          
          
 

              // controle do arduino
          if(readString.indexOf("?on") >0)//checks for on
          {
            alfa.write(80); // mude para o angulo de preferencia
            Serial.println("Energia Ligada");
            client.println("Voce ligou a energia");
            status = 1;
          }
          if(readString.indexOf("?off") >0)//checks for off
          {
            alfa.write(180); // mude para o angulo de preferencia
            Serial.println("Energia Desligada");
            client.println("Voce desligou a energia");
            status = 0;
          }

          client.println("<br>A energia esta atualmente: ");
          
          if(status == 0){
            client.println("<b>Desligada");
          } else {
            client.println("<b>Ligada");
          }
          
          
          client.println("</BODY>");
          client.println("</HTML>");
          
          delay(1);
          
          client.stop();
          readString="";

        }
      }
    }
  }
} 
