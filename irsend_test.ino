#include <IRremote.h>

long mem = 0; 
IRrecv irrecv(8); // ir reciever data pin
// ДИОД ПОДКЛЮЧАЕТСЯ К ТРЕТЬЕМУ (3) ПИНУ! Захардкожено в либе
decode_results results; 
IRsend irsend; 


//холодильник
unsigned int powerOn[84] = { 9100, 4450, 600, 500, 650, 500, 650, 500, 650, 1600, 650, 500, 600, 550, 600, 550, 600, 550, 600, 1600, 650, 500, 650, 500, 650, 500, 650, 1600, 600, 550, 600, 550, 600, 550, 600, 1600, 650, 500, 650, 500, 650, 500, 600, 550, 600, 550, 600, 550, 600, 550, 600, 550, 600, 500, 650, 500, 650, 500, 650, 1600, 600, 550, 600, 550, 600, 550, 600, 550, 600, 500, 650, 1600, 650, 1600, 600, 550, 600, 550, 600, 550, 600, 550, 600};
unsigned int powerOff[84] = { 9100, 4400, 650, 500, 650, 500, 650, 500, 650, 500, 650, 500, 600, 550, 600, 550, 600, 550, 600, 1600, 650, 500, 650, 500, 650, 500, 650, 1600, 600, 550, 600, 550, 600, 550, 600, 1600, 650, 550, 600, 550, 600, 550, 600, 550, 550, 550, 600, 550, 600, 550, 600, 550, 600, 550, 600, 550, 600, 550, 600, 1600, 600, 600, 550, 550, 600, 550, 600, 550, 600, 550, 600, 1650, 600, 1600, 600, 600, 550, 600, 550, 550, 600, 550, 600};


void setup()
{
  Serial.begin(9600); 
  irrecv.enableIRIn(); 
  pinMode (9, OUTPUT); //red led
  pinMode (10, OUTPUT); //green led
}

void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 1; i < count; i++) {
    if (i & 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {

  if (Serial.available()!=0)
     {
       char val = Serial.read();
       if (val == '1')
       {
        //on
          for (int i = 0; i < 3; i++)
          {
             irsend.sendRaw(powerOn, 84, 32);
             digitalWrite (10, HIGH); 
             delay(200);
             digitalWrite (10, LOW); 
          }
       }
       //off
       if (val == '2')
       {
          for (int i = 0; i < 3; i++)
          {
             irsend.sendRaw(powerOff, 84, 32);
             digitalWrite (9, HIGH); 
             delay(200);
             digitalWrite (9, LOW); 
          }
       }
       irrecv.enableIRIn(); // Запустить приемник.
     }
   
///
  if (irrecv.decode(&results)) // Если получен какой-либо сигнал от приемника
  {
    if (results.value != -1) // и если это Не код повтора (-1)
    {
      mem = results.value;  // то код записывается в переменную mem
      
      dump(&results); //show all info
      
      Serial.println(mem, HEX);  // и в послед. порт передается значение этой переменной.
      digitalWrite (9, HIGH);
      delay(300);
      digitalWrite (9, LOW);
    }
    
    else Serial.println(mem, HEX); // Иначе(если это код повтора) передается значение mem без перезаписи.
    irrecv.resume(); // Получить следующее значение
    
    if (results.value == -1) // если код повтора
    {
      digitalWrite (10, HIGH);
      delay (3); 
      digitalWrite (10, LOW);
    }
  }
  delay(30);

}
