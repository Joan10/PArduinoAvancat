#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x79, 0x7E }; //MAC de mi placa
IPAddress ip(192,168,1,2); //IP de la Arduino

EthernetServer server = EthernetServer(80); //Asignación del puerto
EthernetClient client;

int velocidad = 0, intensidad = 0;
boolean direccion = true; //false = horario, true = antihorario

void mandarPaginaWeb(){
  String dir = "horario";
  if (direccion) dir = "antihorario";
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv=\"refresh\" content=\"30\" />");
  client.println("<title>");
  client.println("Arduino - Controlador de Motor");
  client.println("</title>");
  client.println("<body>");
  client.println("<h2>Motor Arduino!</h2>\n<u>Estado</u><br />");
  client.print("Velocidad actual: ");
  client.print((String)velocidad);
  client.println("<br />Intensidad actual: ");
  client.print((String)intensidad);
  client.println("<br />Direccion actual: ");
  client.print(dir);
  client.println("<br /><br />");
  client.println("<form method=\"get\">");
  client.println("Selecciona el nuevo estado del motor:<br />");
  client.print("<input name=\"v\" type=\"text\" value=\"");
  client.print(velocidad);
  client.println("\" />  Velocidad<br />");
  client.print("<input name=\"d\" type=\"checkbox\"");
  if (!direccion) client.print(" checked=\"checked\"");
  client.println(" /> Sentido horario<br />");
  client.println("<input name=\"e\" type=\"submit\" value=\"Enviar\" />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
}

void setup() {
  // inicializar la shield
  Ethernet.begin(mac,ip);
  // empezar a escuchar a la espera de clientes
  server.begin();
  Serial.begin(9600);
  Serial.println("Motor 0 1");
}

void loop() {
  //A la espera de un nuevo cliente
  client = server.available();
  char c;
  int index;
  String s("");
 //Si el cliente envia un byte o más entramos
  if (client.connected()){
    while (client.available() > 0) s += (char)client.read();
    if (s!="") index = s.indexOf('?');
    if (index > 0){
      index++;
      c = s.charAt(index);
      if (c=='v'){
        index += 2;
        velocidad = 0;
        char n = s.charAt(index);
        while (n>='0' and n<='9'){
          velocidad = velocidad*10 + n - '0';
          index++;
          n = s.charAt(index);
        }
        if (velocidad>10000) velocidad = 10000;
      }
      index++;
      c = s.charAt(index);
      if (c=='d') direccion = false;
      else direccion = true;
    }
    if (index > 0){
      Serial.print("Motor ");
      Serial.print(velocidad);
      Serial.print(" ");
      Serial.println(direccion);
      delay(50);
      String s2("");
      char u = Serial.read();
      while (u!=' '){
        delay(1);
        u = Serial.read();
      }
      while (u!='\n'){
        s2 += u;
        delay(1);
        u = Serial.read();
      }
      intensidad = s2.toInt();
    }
    if (s!="") mandarPaginaWeb();
    delay(10);
    client.stop();
  }
}
