
#define P_PWM 11
#define P_DIR 13
#define P_MOTOR A1

int velocidad = 5000;
int vmap;

char direccion = 0;
String s = "";
//char serial_str[50];

void setup(){
  Serial1.begin(9600);  Serial.begin(9600);
  while (!Serial1) {};
 // Serial1.println("Conectado");
  //analogReference(EXTERNAL);
  pinMode(P_MOTOR, OUTPUT); 
  pinMode(P_DIR, OUTPUT); 
  vmap = map(velocidad,0,10000,0,255); //Mapegem la velocitat del motor
  analogWrite(P_PWM,vmap); //El posem en marxa
    
}

void envia_info_motor(){
  int intensidad = analogRead(P_MOTOR); //llegim la intensitat del motor i l'enviem.
  Serial1.print("Motor ");
  Serial1.println(map(intensidad,0,1023,0,2000));
  //Serial.print(" ");
  //Serial.print(velocidad);
  //Serial.print(" ");
  //Serial.println((int)direccion);
}



void treu_string(String *s1){
  (*s1)="";
  //Traiem l'string del serial
  char c = Serial1.read();
  while (c!='\n'){
    s1->concat(c);
    while (!Serial1.available());
    c = Serial1.read();
  }
} 

void actualitza_valors(String s1){
  
    int i = 0;
    String s2 = "";
    //Parse del motor
    //Interpretem l'string rebut. Si s un - canvi de direccio, 
    //si es un nombre entre 0 i 9999 el convertim a int per canviar la velocitat
    if (s1.startsWith(String("Motor "))){
      i =  6; while (s1[i] != ' '){ s2.concat(s1[i]); i++;} i++;
      velocidad = s2.toInt();
      direccion = (s1.charAt(i));
    }

}

void loop(){
  
  if (Serial1.available() > 0) { //Si hi ha hagut un canvi al serial
   
    treu_string(&s);  //Capturem els valors
    actualitza_valors(s);//actualitzem els valors rebuts.
    Serial.println(s);
    Serial.println(direccion);
    
    vmap = map(velocidad,0,10000,0,255); //Mapegem la velocitat
    analogWrite(P_PWM,vmap); //Canviem la velocitat
    
    if (direccion == '1') {digitalWrite(13, HIGH); digitalWrite(P_DIR,HIGH);} //Canviem la direccio
    else digitalWrite(P_DIR,LOW); 
    
  envia_info_motor();
    
  }
  
  
  //delay(700);
}

  
