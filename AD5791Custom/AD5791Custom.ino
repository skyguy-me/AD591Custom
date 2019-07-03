//Author: Gokul 


#define MISO 22 //SDOUT
#define MOSI 23 //SDIN
#define SCK 24 //CLK
#define SS 25 //Sync
#define LDAC 26 //ldac: 
#define RESET 27 //reset
#define CLEAR 28 //clear 


static const int V_refn =  -15;  
static const int V_refp =   15; 


static const uint32_t WRITE_CONTROL_REGISTER_INTIALIZE = 0B001000000000001010000010;
static const uint32_t WRITE_DAC_REGISTER_INTIALIZE = 0B0001; //(<20) | databits;


void setup() {
  pinMode(MISO,INPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(SCK,OUTPUT);
  pinMode(SS,OUTPUT);
  pinMode(LDAC,OUTPUT);
  pinMode(RESET,OUTPUT);
  pinMode(CLEAR,OUTPUT);
  
  digitalWrite(RESET,HIGH);
  digitalWrite(CLEAR,HIGH);
  Serial.begin(9600);
  
 setControlReg();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  sendVoltageDACRegister(-10);

}

void sendVoltageDACRegister(double voltage){

  uint32_t dataStream = ((voltage-V_refn)*(1048575))/(V_refp-V_refn);
  Serial.println(dataStream,BIN);
  
  dataStream = (dataStream) & (0xfffff);
  Serial.println(dataStream,BIN);
  
  uint32_t signalStream = ((WRITE_DAC_REGISTER_INTIALIZE & 0B00001111) << 20) |  dataStream;
  Serial.println(signalStream,BIN);

  uint8_t first =   (uint8_t)((signalStream & 0B111111110000000000000000) >>16);  //mask and extract first 8 bits
  Serial.println(first,BIN);
  uint8_t second =  (uint8_t)((signalStream & 0B000000001111111100000000) >>8);   //mask and extract second 8 bits
  Serial.println(second,BIN);
  uint8_t third =   (uint8_t)((signalStream & 0B000000000000000011111111));       //mask and extract third 8 bits
  Serial.println(third,BIN);

  
  digitalWrite(SS,LOW);
  digitalWrite(LDAC,LOW);
  
  sendyte(first);
  sendyte(second);
  sendyte(third);

  digitalWrite(SS,HIGH);
  digitalWrite(LDAC,HIGH);
  
}

void setControlReg(){
  
  uint32_t signalStream = WRITE_CONTROL_REGISTER_INTIALIZE;
  Serial.println(signalStream,BIN);

  uint8_t first =   (uint8_t)((signalStream & 0B111111110000000000000000) >>16);  //mask and extract first 8 bits
  Serial.println(first,BIN);
  uint8_t second =  (uint8_t)((signalStream & 0B000000001111111100000000) >>8);   //mask and extract second 8 bits
  Serial.println(second,BIN);
  uint8_t third =   (uint8_t)((signalStream & 0B000000000000000011111111));       //mask and extract third 8 bits
  Serial.println(third,BIN);

  
  digitalWrite(SS,LOW);
  digitalWrite(LDAC,LOW);
  
  sendyte(first);
  sendyte(second);
  sendyte(third);

  digitalWrite(SS,HIGH);
  digitalWrite(LDAC,HIGH);
  
}



void sendyte(uint8_t stream){
  int i = 0;
  for (i; i<8; i++){
    digitalWrite(SCK, HIGH);
    if ((stream & 0x80)==0x80){
      digitalWrite(MOSI, HIGH);
     
    } else {
      digitalWrite(MOSI, LOW);
    }
    stream = stream << 1;
    digitalWrite(SCK, LOW);
  }
}
