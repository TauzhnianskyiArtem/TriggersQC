#define MODE 12

#define DP_H PC4
#define DP_L PC3
#define DM_H PC6
#define DM_L PC5

boolean supportsQC();
void DP_0V();
void DP_06V();
void DP_33V();
void DM_0V();
void DM_06V();
void DM_33V();
boolean isSupportsQC;

void setup(){
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  if(!(isSupportsQC = supportsQC())){
    return;
  }
  pinMode(DP_H, OUTPUT);   
  pinMode(DP_L, OUTPUT);
  DP_06V();
  delay(1500);

  pinMode(DM_H, OUTPUT);
  pinMode(DM_L, OUTPUT);
  DM_0V();
  delay(3);
  
  switch(MODE) {
      case 9:
        DP_33V();
        DM_06V();
        break;
      case 12:
        DP_06V();
        DM_06V();
        break;
      case 20:
        DP_33V();
        DM_33V();
        break; 
    }  
}

void loop(){
  /*
      | (D+) | (D-) |   Mode   |
      | 0.6V |  0V  |   5V     |
      | 3.3V | 0.6V |   9V     |
      | 0.6V | 0.6V |   12V    |
      | 3.3V | 3.3V |   20V    |
      | 0.6V | 3.3V | VARIABLE |
      */
  if(!isSupportsQC) {
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(500);                      
    digitalWrite(LED_BUILTIN, LOW); 
    delay(500);
  }
}

boolean supportsQC() {
  pinMode(DM_H, INPUT);
  pinMode(DM_L, INPUT);
  pinMode(DP_H, OUTPUT);   
  pinMode(DP_L, OUTPUT);
  // Проверяем, соеденены ли (D+) и (D-) внутри адаптера, чтобы определить поддержку QC
  // Кратковременно устанавливаем (D+) в 3.3В
  DP_33V();          
  delayMicroseconds(10);  // Задержка в 10 микросекунд
  if (!digitalRead(DM_H)) {// Проверяем (D-), если LOW - (D+) и (D-) внутри адаптера НЕ соеденены
      return false;               // QC не поддерживается
  }

  // Проверка на поддержку QC2.0 или QC3.0 
  DP_06V();
  delay(1500);
  DP_33V();
  delayMicroseconds(10);
  if (!digitalRead(DM_H)) {      // Проверяем (D-), если LOW - (D+) и (D-) внутри адаптера НЕ соеденены
      DP_06V();        // Возвращаем предыдущий логический уровень на (D+)
      return true;  // Поддерживается QC2.0 или QC3.0
  }
  return false; // Поддерживается QC1.0
  
}

void DP_0V(){
  digitalWrite(DP_H, LOW); 
  digitalWrite(DP_L, LOW); 
}

void DP_06V(){
  digitalWrite(DP_H, HIGH); 
  digitalWrite(DP_L, LOW); 
}

void DP_33V(){
  digitalWrite(DP_H, HIGH); 
  digitalWrite(DP_L, HIGH); 
}

void DM_0V(){
  digitalWrite(DM_H, LOW); 
  digitalWrite(DM_L, LOW); 
}

void DM_06V(){
  digitalWrite(DM_H, HIGH); 
  digitalWrite(DM_L, LOW); 
}

void DM_33V(){
  digitalWrite(DM_H, HIGH); 
  digitalWrite(DM_L, HIGH); 
}
