#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
#define sensorTensaoDC  A5

Servo alfa;

//Informações sobre IP, Gateway, mascara de rede e mac
byte mac[] = { 0x70, 0xB3, 0xD5, 0x0A, 0xC6, 0x7B }; // <-- troque pelo seu MAC
byte ip[] = { 192, 168, 1, 155 }; // <-- seu ip
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer server(80);
String readString;

//variáveis
int status = 1;
float valorTensaoDC;
int amostragem = 1000;
float mediaTotalTensaoDC = 0;
float valorFinalTensaoDC = 0;
//valor resistor
float voltsporUnidade = 0.004700000;
float R1 = 30000.0;
float R2 = 7500.0;

void setup() {
  
  
  
  //iniciar ethernet shield
  Ethernet.begin(mac, ip);
  server.begin();
  
  //coisas de serial
  Serial.begin(9600);
  Serial.println("Servidor Servo");

  //atribuição do servo
  alfa.attach(12);
  alfa.write(80);
  
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
          client.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js'></script>");
          //client.println("<script src='https://raw.githubusercontent.com/Luizfilipels/Abertura-e-fechamento-de-chave-faca/master/atualizarpagina.js'</script>");
          client.println("<TITLE>Controle de energia Arduino</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Controle de energia e monitoramento de voltagem</H1>");
                     
                      //conta do voltimetro  
          mediaTotalTensaoDC = 0;
            for (int i = 0; i < amostragem ; i++) {
              valorTensaoDC = analogRead(sensorTensaoDC);
              valorTensaoDC = (valorTensaoDC * voltsporUnidade);
              mediaTotalTensaoDC = mediaTotalTensaoDC + (valorTensaoDC / (R2 / (R1 + R2)));
              
            }
          
          valorFinalTensaoDC = mediaTotalTensaoDC / amostragem;
            Serial.print("Valor: ");
            Serial.print(valorFinalTensaoDC);
            Serial.print(" V");
          
          
          client.println("________________________________");

          //scripts e sla oq eu to fazendo
          client.println("<script>");
          client.println("$(document).ready(function(){");
          client.println("setInterval(function(){");
          client.println("$('#here').load(window.location.href + ' #here' );");
          client.println("}, 3000);");
          client.println("});");
          client.println("</script>");
          //chupa, eu consegui :)
          
          client.println("<div id='here'>");
          client.println("<br>");
          //voltimentro
          client.println("<br>Voltimentro: ");
          client.println(valorFinalTensaoDC);
          client.println(" V");
          client.println("</div>");
          client.println("<br>________________________________");
          client.println("<br>");
          client.println("<br>Controle de energia");
          client.println("<br>");
          client.println("<a href=\"/?on\"\"><button>Ligar</button></a>"); 
          client.println("<a href=\"/?off\"\"><button>Desligar</button></a><br>"); 
          client.println("<br> ");
          client.println("");
          
          
 

              // controle do arduino
          if(readString.indexOf("?on") >0)//checks for on
          {
            client.println("Voce ligou a energia");
            status = 1;
            alfa.write(80); // mude para o angulo de preferencia
            

          }
          if(readString.indexOf("?off") >0)//checks for off
          {
            client.println("Voce desligou a energia");
            status = 0;
            alfa.write(180); // mude para o angulo de preferencia
          }

          client.println("<br>A energia esta atualmente: ");
          
          if(status == 0){
            client.println("<b>Desligada");
          } else {
            client.println("<b>Ligada");
          }
          
          
          client.println("</BODY>");
          client.println("</HTML>");
          
          delay(100);
          
          client.stop();
          readString="";
          //COMANDOS QUE EU TIREI DO CLIENT:
          //Serial.println("Energia Ligada");
          //Serial.println("Energia Desligada");
        }
      }
    }
  }
} 
