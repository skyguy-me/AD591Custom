//Author: Gokul 


#define MISO_91 22 //SDOUT
#define MOSI_91 23 //SDIN
#define SCK_91 24 //CLK
#define SS_91 25 //Sync
#define LDAC_91 26 //ldac: 
#define RESET_91 27 //reset
#define CLEAR_91 28 //clear 


static const int V_refn =  -10;  
static const int V_refp =  10; 


static const uint32_t WRITE_CONTROL_REGISTER_INTIALIZE = 0B001000000000001010000010;
static const uint32_t WRITE_DAC_REGISTER_INTIALIZE = 0B0001; //(<20) | databits;


void setup() {
  pinMode(MISO_91,INPUT);
  pinMode(MOSI_91,OUTPUT);
  pinMode(SCK_91,OUTPUT);
  pinMode(SS_91,OUTPUT);
  pinMode(LDAC_91,OUTPUT);
  pinMode(RESET_91,OUTPUT);
  pinMode(CLEAR_91,OUTPUT);
  
  digitalWrite(RESET_91,HIGH);
  digitalWrite(CLEAR_91,HIGH);
  Serial.begin(9600);
  
 setControlReg_91();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  sendVoltageDACRegister(0);

}

void sendVoltageDACRegister(double voltage){

  uint32_t dataStream = ((voltage)*(1048575))/(V_refp-V_refn);
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

  
  digitalWrite(SS_91,LOW);
  digitalWrite(LDAC_91,LOW);
  
  sendyte(first);
  sendyte(second);
  sendyte(third);

  digitalWrite(SS_91,HIGH);
  digitalWrite(LDAC_91,HIGH);
  
}

void setControlReg_91(){
  
  uint32_t signalStream = WRITE_CONTROL_REGISTER_INTIALIZE;
  Serial.println(signalStream,BIN);

  uint8_t first =   (uint8_t)((signalStream & 0B111111110000000000000000) >>16);  //mask and extract first 8 bits
  Serial.println(first,BIN);
  uint8_t second =  (uint8_t)((signalStream & 0B000000001111111100000000) >>8);   //mask and extract second 8 bits
  Serial.println(second,BIN);
  uint8_t third =   (uint8_t)((signalStream & 0B000000000000000011111111));       //mask and extract third 8 bits
  Serial.println(third,BIN);

  
  digitalWrite(SS_91,LOW);
  digitalWrite(LDAC_91,LOW);
  
  sendyte(first);
  sendyte(second);
  sendyte(third);

  digitalWrite(SS_91,HIGH);
  digitalWrite(LDAC_91,HIGH);
  
}



void sendyte(uint8_t stream){
  int i = 0;
  for (i; i<8; i++){
    digitalWrite(SCK_91, HIGH);
    if ((stream & 0x80)==0x80){
      digitalWrite(MOSI_91, HIGH);
     
    } else {
      digitalWrite(MOSI_91, LOW);
    }
    stream = stream << 1;
    digitalWrite(SCK_91, LOW);
  }
}
