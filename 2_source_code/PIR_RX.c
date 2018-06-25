
sbit in_port at RB0_bit;
sbit button_1 at RB1_bit;  // заряд аккума, сброс тревоги, регистрация ID
sbit button_2 at RB2_bit;  // проверка ID, удаление ID из EEPROM, удаление всего EEPROM
sbit adc_connect at LATB3_bit;
sbit alarm_out at LATB5_bit;

// led alarm
sbit led_1 at LATB4_bit;
sbit led_2 at LATA5_bit;
sbit led_3 at LATA4_bit;
sbit led_4 at LATA3_bit;
sbit led_5 at LATA2_bit;
sbit led_6 at LATA1_bit;
sbit led_7 at LATC2_bit;
sbit led_8 at LATC3_bit;
sbit led_9 at LATC4_bit;
sbit led_10 at LATC5_bit;
sbit led_11 at LATC6_bit;
sbit led_12 at LATC7_bit;
sbit led_13 at LATD0_bit;
sbit led_14 at LATD1_bit;
sbit led_15 at LATD2_bit;
sbit led_16 at LATD3_bit;
sbit led_17 at LATD4_bit;
sbit led_18 at LATD5_bit;
sbit led_19 at LATD6_bit;
sbit led_20 at LATD7_bit;

char device_id;
char code_;
char adc_level;
char sum_check;
char i;
char b;
char e;
unsigned int counter;
char timer1_count;
char tmr1_flag;
char interrupt_done = 0;
char battery_level;
char temp_1;
char temp_2;
char temp_3;


unsigned int adc_data;
unsigned int bat_level_1;
unsigned int bat_level_2;
unsigned int bat_level_3;
unsigned int bat_level_4;
unsigned int bat_level_5;

char status_device[21];

char status_device_1;
char status_device_2;
char status_device_3;
char status_device_4;
char status_device_5;
char status_device_6;
char status_device_7;
char status_device_8;
char status_device_9;
char status_device_10;
char status_device_11;
char status_device_12;
char status_device_13;
char status_device_14;
char status_device_15;
char status_device_16;
char status_device_17;
char status_device_18;
char status_device_19;
char status_device_20;


led_on_off(char led_num, char on_off) {
  switch (led_num) {
    case 1: led_1 = on_off; break;
    case 2: led_2 = on_off; break;
    case 3: led_3 = on_off; break;
    case 4: led_4 = on_off; break;
    case 5: led_5 = on_off; break;
    case 6: led_6 = on_off; break;
    case 7: led_7 = on_off; break;
    case 8: led_8 = on_off; break;
    case 9: led_9 = on_off; break;
    case 10: led_10 = on_off; break;
    case 11: led_11 = on_off; break;
    case 12: led_12 = on_off; break;
    case 13: led_13 = on_off; break;
    case 14: led_14 = on_off; break;
    case 15: led_15 = on_off; break;
    case 16: led_16 = on_off; break;
    case 17: led_17 = on_off; break;
    case 18: led_18 = on_off; break;
    case 19: led_19 = on_off; break;
    case 20: led_20 = on_off; break;
    //default: break;
  }
}

reset_all_led(){
 char k = 1;
 for(k=1; k<21; k++){
  led_on_off(k, 0);
 }
}

//сбросить WDT   1024-8s, 4096-32s, 8195-1m, 16384-2m, 32768-4m21s
wdt_clear(){
 asm{
   CLRWDT
 }
}


/*TMR0_setup_registration(){
  T0CON = 0x07;
  TMR0IF_bit = 0; // флаг
  TMR0H = 0;
  TMR0L = 0;
}*/

TMR0_setup_20ms(){
  T0CON = 0x01;
  TMR0IF_bit = 0; // флаг
  TMR0H = 217;
  TMR0L = 0;
}

TMR0_setup_1ms(){
  T0CON = 0x01;
  TMR0IF_bit = 0; // флаг
  TMR0H = 253;
  TMR0L = 0;
}

TMR1_setup(){
  T1CON = 0x0E;
  T1CKPS1_bit = 1;
  T1CKPS0_bit = 1;
  TMR1L = 0x00;
  TMR1H = 0x00;
}

TMR1_start_stop(char start_bit){
  TMR1ON_bit = start_bit;
}

ADC_setup(){
   // настройка АЦП
 ADCON0 = 0x00;
 ADCON1 = 0x0E;
 ADFM_bit = 1; // правое выравнивание
}

ADC_on_off(char on_off_bit){
 ADON_bit = on_off_bit; // включить АЦП
 adc_connect = on_off_bit;
}

unsigned int ADC_get(){
 unsigned int adc_d;
 GO_DONE_bit = 1; // выполнить преобразование АЦП  0- после завершения
 while(GO_DONE_bit == 1){
  // wdt_clear();   // сброс WDT
 }
 delay_ms(10);
 adc_d = 0x0000 | ADRESH;
 adc_d = adc_d<<8;
 adc_d = adc_d | ADRESL;
 return adc_d;
}

/*eeprom_write_adc(){
  char H_1 = 0x02;
  char L_1 = 0xAD;
  char H_2 = 0x02;
  char L_2 = 0x9B;
  char H_3 = 0x02;
  char L_3 = 0x8A;
  char H_4 = 0x02;
  char L_4 = 0x7A;
  char H_5 = 0x02;
  char L_5 = 0x6A;
  EEPROM_Write(0x01, H_1);
  delay_ms(10);
  EEPROM_Write(0x02, L_1);
  delay_ms(10);
  EEPROM_Write(0x03, H_2);
  delay_ms(10);
  EEPROM_Write(0x04, L_2);
  delay_ms(10);
  EEPROM_Write(0x05, H_3);
  delay_ms(10);
  EEPROM_Write(0x06, L_3);
  delay_ms(10);
  EEPROM_Write(0x07, H_4);
  delay_ms(10);
  EEPROM_Write(0x08, L_4);
  delay_ms(10);
  EEPROM_Write(0x09, H_5);
  delay_ms(10);
  EEPROM_Write(0x0A, L_5);
  delay_ms(10);
}*/

eeprom_read_adc(){
  //wdt_clear();   // сброс WDT
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
}

battery_test(){    // проверка батареи
   ADC_on_off(1);  // вкл-выкл АЦП
   delay_ms(100);
   while(button_1 == 0){
     wdt_clear();   // сброс WDT
     adc_data = 0;
     adc_data = ADC_get();
     delay_ms(100);
     if(adc_data <= bat_level_1) led_on_off(1, 1);
     if(adc_data <= bat_level_2) led_on_off(2, 1);
     if(adc_data <= bat_level_3) led_on_off(3, 1);
     if(adc_data <= bat_level_4) led_on_off(4, 1);
     if(adc_data <= bat_level_5) led_on_off(5, 1);
        if(adc_data > bat_level_1) {
          for(i=5; i>=1; i--){
            led_on_off(i, 1);
            delay_ms(20);
            led_on_off(i, 0);
          }
          led_on_off(1, 1);
          delay_ms(100);
        }
     delay_ms(50);
     reset_all_led();
  }
  
  ADC_on_off(0);  // вкл-выкл АЦП
}


char battery_test_0(){    // проверка батареи
   ADC_on_off(1);  // вкл-выкл АЦП
   delay_ms(5);
   wdt_clear();   // сброс WDT
   adc_data = 0;
   adc_data = ADC_get();
   ADC_on_off(0);  // вкл-выкл АЦП
   if(adc_data > bat_level_1) {
      return 1;
    }
   else {
      return 0;
   }
  
}


char preamb_receive(){
  char result = 0;
  TMR0_setup_20ms();
  delay_ms(5);
   if(in_port == 1) {
    result = 1;
    TMR0ON_bit = 1; // включить таймер
    while (in_port == 1){
      if(TMR0IF_bit == 1){
        TMR0ON_bit = 0; // включить таймер
        TMR0IF_bit = 0; // флаг
        result = 0;
        break;
      }
     // wdt_clear();   // сброс WDT
    }
  }
  return result;
}


char bit_receive(){
  char result = 0;
  TMR0_setup_1ms();
  TMR0ON_bit = 1; // включить таймер
  while(in_port == 0){
    if(TMR0IF_bit == 1){
        TMR0ON_bit = 0; // включить таймер
        TMR0IF_bit = 0; // флаг
        result = 2; // ошибка
        break;
     }
    // wdt_clear();   // сброс WDT
  }
  TMR0_setup_1ms();
  TMR0ON_bit = 1; // включить таймер
  delay_us(500);
  //wdt_clear();   // сброс WDT
  if(in_port == 1){
     delay_us(1000);
    // wdt_clear();   // сброс WDT
     if(in_port == 0){
        result = 0; // 0
        TMR0ON_bit = 0; // включить таймер
        TMR0IF_bit = 0; // флаг
     }
     else {
        result = 1; // 0
        while(in_port == 1){
          if(TMR0IF_bit == 1){
            TMR0ON_bit = 0; // включить таймер
            TMR0IF_bit = 0; // флаг
            result = 2; // ошибка
            break;
           }
          // wdt_clear();   // сброс WDT
        }
        TMR0ON_bit = 0; // включить таймер
     }
   }
   else{
     result = 2; // ошибка
   }

  return result;
}

char byte_receive(){
 char byte_data = 0;
 char bit_data = 0;
 char tmp_count = 0;
 while(tmp_count < 8){
   byte_data = byte_data >> 1;
   bit_data = bit_receive();
   
   if(bit_data == 1){
     byte_data = byte_data | 0x80;
   }
   tmp_count++;
 }
 return byte_data;
}

eeprom_all_status_id_del(){
  for(i=11; i<31; i++){
   EEPROM_Write(i, 0);
   delay_ms(10);
  }
}


 led_alarm(char alarm_code, char alarm_id){
    if(alarm_code == 11){
      led_on_off(alarm_id, 1);
      delay_ms(10);
      led_on_off(alarm_id, 0);
    }
    if(alarm_code == 22){
      for(i=1; i<3; i++){
       led_on_off(alarm_id, 1);
       delay_ms(100);
       led_on_off(alarm_id, 0);
       delay_ms(100);
      }
      delay_ms(200);

    }
    if(alarm_code == 33){
      led_on_off(alarm_id, 1);
      delay_ms(500);
      led_on_off(alarm_id, 0);
      Sound_Play(700, 500);
      //delay_ms(500);
    }
    if(alarm_code == 88){
      for(i=1; i<4; i++){
       led_on_off(alarm_id, 1);
       delay_ms(100);
       led_on_off(alarm_id, 0);
       delay_ms(100);
      }
      delay_ms(400);
    }


 }


interrupt_on_off(char on_off){
  GIE_GIEH_bit = on_off; // разрешить прерывания
}

interrupt_setup(){
  RCON = 0x00;
  INTCON = 0x00;
  IPEN_bit = 0;
  INTCON = 0x00;
  GIE_GIEH_bit = 0;
  PEIE_GIEL_bit = 1;
  TMR0IE_bit = 0; // разрешить прерывание
  TMR1IE_bit = 0; // разрешить прерывание
  INT0IE_bit = 0; // разрешить прерывание
  INT1IE_bit = 0; // разрешить прерывание
  INT2IE_bit = 0; // разрешить прерывание
  RBIE_bit = 0;
  RBIF_bit = 0;
  TMR0IF_bit = 0; // флаг
  TMR1IF_bit = 0; // флаг
  INT0IF_bit = 0; // флаг
  INT1IF_bit = 0; // флаг
  INT2IF_bit = 0; // флаг
}



void interrupt() {
  GIE_GIEH_bit = 0; // запретить прерывания
  tmr1_flag = 0;
  if(TMR1IF_bit == 1) {; // флаг
     TMR1IF_bit = 0;
     tmr1_flag = 1;
  }
 // TMR1IF_bit = 0; // флаг
  INT0IF_bit = 0; // флаг
  INT1IF_bit = 0; // флаг
  INT2IF_bit = 0; // флаг
 // interrupt_done = 1;
 //led_2 = 0;
 //TMR1ON_bit = 0;
}






void main() {
  // порты направление
  TRISA = 0x01;
  TRISB = 0x07;
  TRISC = 0x00;
  TRISD = 0x00;
  
  alarm_out = 0;
  reset_all_led();
  
  // аналог,цифра
  ADCON1 = 0x0E;
  
  // порты подтяжка
  RBPU_bit = 1; // отключить подтяжку
  
  // настроить прерывания
  interrupt_setup();
  //interrupt_on_off(1); // включение прерывания
  
   // настройка АЦП
  ADC_setup();
  //eeprom_write_adc(); // только для теста ========================================================= НЕ ЗАБЫТЬ ВЫКЛЮЧИТЬ!!!
  eeprom_read_adc();  // загрузить значения переменных для измерения батареи
  ADC_on_off(0);  // вкл-выкл АЦП
  adc_connect = 0;
 
  Sound_Init(&PORTB, 7);
  reset_all_led();
  alarm_out = 0;
  wdt_clear();   // сброс WDT
 
   // настроить таймеры
  TMR1_setup();  // настроить таймер ожидания
  TMR1_start_stop(0);  // управление таймером ожидания
  interrupt_on_off(0); // включение прерывания
  timer1_count = 0;
  tmr1_flag = 0;
  
  for(i=1; i<=5; i++){    // 5
  led_on_off(i, 1);
   delay_ms(80);
  led_on_off(i, 0);
  }
  //delay_ms(8000);
  wdt_clear();   // сброс WDT
  
  
/*while(1){
     adc_connect = 1;
     wdt_clear();   // сброс WDT
     delay_ms(1000);
     wdt_clear();   // сброс WDT
     delay_ms(1000);
     wdt_clear();   // сброс WDT
     delay_ms(1000);
     wdt_clear();   // сброс WDT
     delay_ms(1000);
     adc_connect = 0;
     wdt_clear();   // сброс WDT
     delay_ms(1000);
     wdt_clear();   // сброс WDT
     delay_ms(1000);

  }*/
  

 // ################################################################################
 // ЧТЕНИЕ EEPROM И КНОПКИ ПРИ ВКЛЮЧЕНИИ
 // ################################################################################

  for(i=11; i<31; i++){
   status_device[i-10] = EEPROM_Read(i);
   delay_ms(10);
  }
 
  if(button_2 == 0) {
   eeprom_all_status_id_del(); // если кнопка_2 нажата при включении стереть eeprom id датчиков
   Sound_Play(600, 100);
   while (button_2 == 0){
  
   }
 }

 // ################################################################################
 // ОСНОВНОЙ ЦИКЛ
 // ################################################################################
 while(1){
 
   wdt_clear();   // сброс WDT

   if(button_1 == 0) {  // если кнопка_1 проверка батареи
     Sound_Play(600, 100);
     while(button_1 == 0){
       battery_test();
     }
   }

   
   if(button_2 == 0) {  // если кнопка_2 проверка ID
     Sound_Play(600, 100);
     for(b=11; b<31; b++){
       if(status_device[b-10] > 0) {
        led_alarm(88, b-10);
        delay_ms(200);
       }
     }
   }
   
   
   if(tmr1_flag == 1) {  // если переполнение таймера ожидания  1круг = 16сек
      //TMR1IF_bit = 0; // флаг
      timer1_count++;
   }

   if(timer1_count == 4){   // 1=16сек  1минута = 60/16 = 3,75 (4)=1:04мин
   
      /*led_on_off(1, 1);
      delay_ms(500);
      led_on_off(1, 0);*/
      
      timer1_count = 0;
      interrupt_on_off(0); // включение прерывания
      TMR1_setup();  // настроить таймер ожидания
      TMR1_start_stop(0);  // управление таймером ожидания
     
     for(b=11; b<31; b++){
       if(status_device[b-10] > 0) {
          temp_1 = status_device[b-10];
          temp_1--;
          if(temp_1 > 1) {
           status_device[b-10] = temp_1;
          }
          else {

            while(1){
              if(button_1 == 0){  // сбросить тревогу
                Sound_Play(500, 200);
                status_device[b-10] = 4;
                delay_ms(500);
                break;
              }

              if(button_2 == 0){  // удалить ID из EEPROM
                Sound_Play(500, 200);
                status_device[b-10] = 0;
                EEPROM_Write(b, 0);
                delay_ms(1500);
                break;
              }
             Sound_Play(600, 100);
             led_alarm(88, b-10);
             wdt_clear();   // сброс WDT
            }
          
          }
       }
     }
     
     // проверить уровень заряда батареи
    battery_level = 0;
    battery_level = battery_test_0();

     if(battery_level == 1){
       battery_level = 0;

       while(1){

         for(i=5; i>=1; i--){
            led_on_off(i, 1);
            Sound_Play(100*i, 30);
            led_on_off(i, 0);
          }
         led_on_off(1, 1);
         Sound_Play(600, 200);
         reset_all_led();

         if(button_1 == 0){
           break;
         }
         wdt_clear();   // сброс WDT
       }

       Sound_Play(600, 200);
       delay_ms(500);
     }
     
     
     

   }


   if(in_port == 1){  // если пришел код
   
     if(preamb_receive() == 1) {   // если преамбула принята

       device_id = byte_receive(); // принимаем посылку
       code_ = byte_receive();
       adc_level = byte_receive();
       sum_check = byte_receive();

       if(device_id + code_ + adc_level == sum_check) { // если контрольная сумма верна

             // =====================================================================
             // если получен код регистрации = 88
             // =====================================================================
             if(code_ == 88){

               if(status_device[device_id] == 0) { // если датчик не зарегистрирован
                 delay_ms(10);
                 counter = 0;

                 while(counter < 20){
                   if(button_1 == 0){
                     Sound_Play(500, 200);
                     status_device[device_id] = 4;
                     EEPROM_Write(device_id + 10, 4);
                     delay_ms(10);
                     break;
                     
                   }
                   led_alarm(code_, device_id);
                   counter++;
                   wdt_clear();   // сброс WDT
                 }
                 // добавить звук по нажатию кнопки
                 delay_ms(1000);
               }
             }

             if(status_device[device_id] > 0) {  // если ID зарегистрирован в EEPROM

                 // =====================================================================
                 // если получен код ОК = 11
                 // =====================================================================
                 if(code_ == 11){
                   status_device[device_id] = 4;
                   led_alarm(code_, device_id);
                   //delay_ms(5);
                   reset_all_led();
                   
                   // =====================================================================
                   // если тревога разряженная батарея(adc_level == 11)
                   // =====================================================================
                   if(adc_level == 8){
                    alarm_out = 1;
                     while(1){
                      if(button_1 == 0){  // сбросить тревогу
                        Sound_Play(600, 100);
                        status_device[device_id] = 4;
                        break;
                       }

                       if(button_2 == 0){  // удалить ID из EEPROM
                        Sound_Play(600, 100);
                        status_device[device_id] = 0;
                        EEPROM_Write(device_id + 10, 0);
                        delay_ms(10);
                        break;
                       }
                       
                      Sound_Play(700, 100);
                      led_alarm(22, device_id);
                      wdt_clear();   // сброс WDT
                     }
                    // добавить звук по нажатию кнопки
                    alarm_out = 0;
                    delay_ms(1500);
                    
                   }

                   /*if(adc_level >= 11) led_on_off(1, 1);
                   if(adc_level >= 22) led_on_off(2, 1);
                   if(adc_level >= 33) led_on_off(3, 1);
                   if(adc_level >= 44) led_on_off(4, 1);
                   if(adc_level >= 55) led_on_off(5, 1);
                   delay_ms(500);
                   reset_all_led();*/
                   
                 }

                 // =====================================================================
                 // если пришел код тревога PIR = 33
                 // =====================================================================
                 if(code_ == 33){
                   alarm_out = 1;
                   while(1){
                     if(button_1 == 0){  // сбросить тревогу
                        Sound_Play(600, 100);
                        status_device[device_id] = 4;
                        break;
                      }

                      if(button_2 == 0){  // удалить ID из EEPROM
                        Sound_Play(600, 100);
                        status_device[device_id] = 0;
                        EEPROM_Write(device_id + 10, 0);
                        delay_ms(10);
                        break;
                      }

                    led_alarm(code_, device_id);
                    wdt_clear();   // сброс WDT
                   }

                   alarm_out = 0;
                   delay_ms(1500);
                 }


             }

        }


     }


   }



   
   wdt_clear();   // сброс WDT
   //TMR1IF_bit = 0; // флаг
   INT0IF_bit = 0; // флаг
   INT1IF_bit = 0; // флаг
   INT2IF_bit = 0; // флаг

   TMR0IE_bit = 0; // разрешить прерывание
   TMR1IE_bit = 1; // разрешить прерывание
   INT0IE_bit = 1; // разрешить прерывание
   INT1IE_bit = 1; // разрешить прерывание
   INT2IE_bit = 1; // разрешить прерывание

   TMR1_start_stop(1);  // управление таймером ожидания
   interrupt_on_off(1); // включение прерывания
   
  asm{
    sleep;
   }
   asm{
    nop;
   }

 }
 
 
 
 
 
 
 



}