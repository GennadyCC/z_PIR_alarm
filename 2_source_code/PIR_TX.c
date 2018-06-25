

sbit Data_ at GP0_bit;    //
sbit ADC_in at GP4_bit; 
sbit ADC_on_off at GP5_bit;   //
sbit PIR_ at GP2_bit;  //
sbit TX_EN at GP1_bit;  //

unsigned int i;
unsigned int b;
unsigned int adc_data;
char id_;
char code_;
char bat_level;
char sum_;
char adc_hi_bit;
char adc_lo_bit;
unsigned int sleep_timer;
unsigned int sleep_timer_2;
unsigned int delay_time;
             
unsigned int bat_level_1;
unsigned int bat_level_2;
unsigned int bat_level_3;
unsigned int bat_level_4;
unsigned int bat_level_5;

void preamb_send(){
  Data_ = 1;
  delay_ms(10);    //15 или 18
  Data_ = 0;
  delay_us(1000);  //
}

void code_send(char code_num){
 char i=0;
 while(i<8){       //8

  if(code_num>>i & 0x01 == 1){
   Data_ = 1;
   delay_us(2000);
  }
  else {
   Data_ = 1;
   delay_us(1000);
  }
  Data_ = 0;
  delay_us(1000);
  i++;
 }
}

wdt_clear(){
 asm{
   CLRWDT
 }

}


unsigned int get_ADC_data(){
  unsigned int adc_data_int = 0;
  unsigned int adc_data_temp = 0;
  ADC_on_off = 1;
  delay_ms(1);
  for(i=0; i<5; i++){
   adc_data_temp = ADC_Get_Sample(3);
   delay_ms(1);
   wdt_clear();
   adc_data_int = adc_data_int + adc_data_temp;
  }
  ADC_on_off = 0;
  adc_data_int = adc_data_int /5;
 // adc_lo_bit = adc_data_int;
 // adc_hi_bit = adc_data_int>>8;
  /*EEPROM_Write(0x01, adc_hi_bit);
  delay_ms(10);
  EEPROM_Write(0x02, adc_lo_bit);
  delay_ms(10);*/
  return adc_data_int;
}

void send_code(char code_send){
     code_ = code_send;
     sum_ = id_ + code_+ bat_level;
     preamb_send();
     code_send(id_);
     code_send(code_);
     code_send(bat_level);
     code_send(sum_);

}

void PIR_detect(){
  if(PIR_ == 1){
     wdt_clear();
     //code_ = 33;
     bat_level = 0;
     
     /*sum_ = id_ + code_+ bat_level;

     preamb_send();
     code_send(id_);
     code_send(code_);
     code_send(bat_level);
     code_send(sum_);
     delay_ms(100);   // ???????
     wdt_clear();*/
     
     for(b=0; b<6; b++){
      send_code(33);
      delay_ms(500);
      wdt_clear();
     
     }
     
     
     
     
   }
}

void ADC_rutine(){
  ADC_Init();
  adc_data = get_ADC_data();
  bat_level = 8;
  if(adc_data <= bat_level_1) bat_level = 11;
  if(adc_data <= bat_level_2) bat_level = 22;
  if(adc_data <= bat_level_3) bat_level = 33;
  if(adc_data <= bat_level_4) bat_level = 44;
  if(adc_data <= bat_level_5) bat_level = 55;


}


void interrupt() {
 GIE_bit = 0; //разрешить глобальные прерывания
 INTF_bit = 0; //сбросить флаги прерывания INT
 //GPIF_bit = 0; //сбросить флаги прерывания портов
 //GPIE_bit = 1; //разрешить прерывания от портов
 //PEIE_bit = 1; // разрешить прерывания переферии


}



void main() {

  TRISIO0_bit = 0;
  TRISIO1_bit = 0;
  TRISIO2_bit = 1;
  //TRISIO3_bit = 0;

  TRISIO4_bit = 1;
  TRISIO5_bit = 0;



  OPTION_REG = 0x8F; //0x8F;
  NOT_GPPU_bit = 1; //0= разрешить подтягивающие резисторы
  WPU = 0x00;    // выбрать порты для подтягивающих резисторов
  CMCON = 0x07;
  VREN_bit = 0;
  VCFG_bit = 0;


  ADCON0 = 0x8D; //AN3
  ANSEL = 0x50;   /// или 58 проверить!!!

  T1CON = 0;

  Data_ = 0;
  TX_EN = 0;
  //GP2_bit = 0;
  //GP4_bit = 0;
  ADC_on_off = 0;

  

 INTEDG_bit = 1;// прерывания по ИНТ 0->1
 INTE_bit = 1;  //разрешить прерывания от INT
 INTF_bit = 0; //сбросить флаги прерывания INT
 PEIE_bit = 1; // разрешить прерывания переферии
 
 sleep_timer = 0;
 sleep_timer_2 = 0;
 
 Data_ = 0;
 ADC_Init();
 
 id_ = EEPROM_Read(0x00);
 
 bat_level_1 = EEPROM_Read(0x01);
 delay_ms(10);
 bat_level_1 = bat_level_1<<8;
 bat_level_1 = bat_level_1 | EEPROM_Read(0x02);
 delay_ms(10);
 bat_level_2 = EEPROM_Read(0x03);
 delay_ms(10);
 bat_level_2 = bat_level_2<<8;
 bat_level_2 = bat_level_2 | EEPROM_Read(0x04);
 delay_ms(10);
 bat_level_3 = EEPROM_Read(0x05);
 delay_ms(10);
 bat_level_3 = bat_level_3<<8;
 bat_level_3 = bat_level_3 | EEPROM_Read(0x06);
 delay_ms(10);
 bat_level_4 = EEPROM_Read(0x07);
 delay_ms(10);
 bat_level_4 = bat_level_4<<8;
 bat_level_4 = bat_level_4 | EEPROM_Read(0x08);
 delay_ms(10);
 bat_level_5 = EEPROM_Read(0x09);
 delay_ms(10);
 bat_level_5 = bat_level_5<<8;
 bat_level_5 = bat_level_5 | EEPROM_Read(0x0A);
 delay_ms(10);

  delay_ms(500);
  wdt_clear();
  delay_ms(500);
  wdt_clear();
  
  Data_ = 0;
  bat_level = 0;
  wdt_clear();
  
  TX_EN = 1;
  delay_ms(100);
  send_code(88); // отправить посылку с кодом 88
  TX_EN = 0;
  
  delay_ms(500);
  wdt_clear();
  delay_ms(500);
  wdt_clear();
  
   TX_EN = 1;
   ADC_rutine(); // Получить данные АЦП
   delay_ms(100);
     for(b=0; b<3; b++){
          send_code(11);
          delay_ms(50);
          wdt_clear();
     }
   TX_EN = 0;
  
/*while(1){
    ADC_rutine(); // Получить данные АЦП

    for(b=0; b<3; b++){
      send_code(11);
      delay_ms(50);
      wdt_clear();
     }
     delay_ms(1000);
     wdt_clear();
  }*/

  while(1){
  
    wdt_clear();
    Data_ = 0;
    GIE_bit = 1; //разрешить глобальные прерывания*/
    asm{sleep};
    asm{NOP};
    GIE_bit = 0; //разрешить глобальные прерывания*/
    PIR_detect(); //проверить не сработал ли ПИР датчик/
    

     sleep_timer++;
     if(sleep_timer == 24){    // один круг wdt = 2.5s  1минута = 60/2,5 = 24круга
        sleep_timer = 0;

        TX_EN = 1;
        ADC_rutine(); // Получить данные АЦП

        for(b=0; b<3; b++){
          send_code(11);
          delay_ms(50);
          wdt_clear();
        }
        TX_EN = 0;
        sleep_timer_2++;
        
     }
     


     if(sleep_timer == sleep_timer_2){    // один круг wdt = 2.5s  1минута = 60/2,5 = 24круга
        //sleep_timer_2 = 0;
        //id_
        TX_EN = 1;
        ADC_rutine(); // Получить данные АЦП
        for(b=0; b<3; b++){
          send_code(11);
          delay_ms(50);
          wdt_clear();
        }
        TX_EN = 0;
        if(sleep_timer_2 == id_ + 1) sleep_timer_2 = 0;

     }
     
  }



}