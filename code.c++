#include <LiquidCrystal.h>

// Definindo os pins
const int BUZZER_PIN = 10; 
const int GREEN_LED_PIN = 11; 
const int YELLOW_LED_PIN = 12; 
const int RED_LED_PIN = 13; 
const int LDR_PIN = A2;  
const int TEMP_SENSOR_PIN = A0;
const int HUMIDITY_SENSOR_PIN = A1;

// Níveis de luz
const int LDR_OK_LEVEL = 900; 
const int LDR_ALERT_LEVEL = 950;  

// Níveis de umidade
const int HUM_OK_MIN_LEVEL = 50;
const int HUM_OK_MAX_LEVEL = 70;

// Níveis de temperatura
const float TEMP_OK_MIN_LEVEL = 10;
const float TEMP_OK_MAX_LEVEL = 15;

// Configurações LCD
const int RS = 9, EN = 8, DB4 = 5, DB5 = 4, DB6 = 3, DB7 = 2;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

// Variáveis que guardam os valores
int ldrValue = 0;
int rawTempValue = 0;
int rawHumidityValue = 0;


// Medidas
float humidityMeasurements[6] = {0,0,0,0,0,0};
float tempMeasurements[6] = {0,0,0,0,0,0};
float luminosityMeasurements[6] = {0,0,0,0,0,0};

int order = 0;

void setup() {
// Confirar pins
pinMode(LDR_PIN, INPUT);
pinMode(GREEN_LED_PIN, OUTPUT);
pinMode(YELLOW_LED_PIN, OUTPUT);
pinMode(RED_LED_PIN, OUTPUT);
pinMode(BUZZER_PIN, OUTPUT);
  
// Desligar LEDs e buzzer
digitalWrite(GREEN_LED_PIN, LOW);
digitalWrite(YELLOW_LED_PIN, LOW);
digitalWrite(RED_LED_PIN, LOW);
noTone(BUZZER_PIN);
  
// Setup serial
Serial.begin(9600);
pinMode(HUMIDITY_SENSOR_PIN, INPUT);
  
// Iniciar LCD
lcd.begin(16,2);
}


void loop() {
  int inicio = millis();
	while(true) { // Loop infinito 
  
  // Ler valores dos sensores
  ldrValue = analogRead(LDR_PIN);
  rawTempValue = analogRead(TEMP_SENSOR_PIN);
  rawHumidityValue = analogRead(HUMIDITY_SENSOR_PIN);
  
  // Converter a temperatura do sensor para celsius
  float voltage = (rawTempValue / 1023.0) * 5000.0;
  float temperatureC = (voltage - 500.0) * 0.1;
  
  // Converter a umidade do sensor para porcentagem
  int humidityPercentage = map(rawHumidityValue, 0, 1023, 10, 100);
  
      
  // Medidas umidade
  humidityMeasurements[int(humidityMeasurements[0])+1] = float(humidityPercentage);
    
  if(humidityMeasurements[0] < 4){
    humidityMeasurements[0]++;
  } else {
  	humidityMeasurements[0] = 0;	
  }
      
  // Medidas temperatura
  tempMeasurements[int(tempMeasurements[0])+1] = float(temperatureC);
  if(tempMeasurements[0] < 4){
    tempMeasurements[0]++;
  } else {
  	tempMeasurements[0] = 0;	
  }
      
  // Medidas luminosidade
  luminosityMeasurements[int(luminosityMeasurements[0])+1] = float(ldrValue);
  if(luminosityMeasurements[0] < 4){
    luminosityMeasurements[0]++;
    continue;
  } else {
  	luminosityMeasurements[0] = 0;	
  }  
      
      
  // Print os valores dos sensores pro LCD
  Serial.print(" | Tempetura: ");
  Serial.print(temperatureC, 1);
  Serial.print("C | Umidade: ");
  Serial.print(humidityPercentage);
  Serial.println("%");
      
   int agora = millis();
  if(agora - inicio > 5000){
    Serial.print("order: "); 
  	Serial.println(order);
    if(order < 3){
    	order++;
    } else {
    	order = 0;
    }
    inicio = millis();       
  }
      
      
      if(order == 0) {  
        // Printa o valor de l uz pro LCD
        if (ldrValue <= LDR_OK_LEVEL) {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Nivel de luz: OK");
          digitalWrite(GREEN_LED_PIN, HIGH);
    	  digitalWrite(YELLOW_LED_PIN, LOW);
    	  digitalWrite(RED_LED_PIN, LOW);
    	  noTone(BUZZER_PIN);
        }
        else if (ldrValue <= LDR_ALERT_LEVEL) {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Nivel de luz:");
          lcd.setCursor(0,1);
          lcd.print("CUIDADO");
          digitalWrite(GREEN_LED_PIN, LOW);
    	  digitalWrite(YELLOW_LED_PIN, HIGH);
    	  digitalWrite(RED_LED_PIN, LOW);
    	  noTone(BUZZER_PIN);
        }
        else {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Nivel de luz:");
          lcd.setCursor(0,1);
          lcd.print("CRITICO");
          digitalWrite(GREEN_LED_PIN, LOW);
    	  digitalWrite(YELLOW_LED_PIN, LOW);
    	  digitalWrite(RED_LED_PIN, HIGH);
    	  tone(BUZZER_PIN, 1000); // Ligar buzzer 
  		}
      }

      if(order == 1){
        // Printa a temperatura pro LCD
        if (temperatureC >= TEMP_OK_MIN_LEVEL && temperatureC <= TEMP_OK_MAX_LEVEL) {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Temperatura:");
          lcd.setCursor(0,1);
          lcd.print(temperatureC, 1);
          lcd.print("C  OK");
          digitalWrite(GREEN_LED_PIN, HIGH);
    	  digitalWrite(YELLOW_LED_PIN, LOW);
    	  digitalWrite(RED_LED_PIN, LOW);
    	  noTone(BUZZER_PIN);
        }
        else {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Temperatura:");
          lcd.setCursor(0,1);
          lcd.print(temperatureC, 1);
          lcd.print("C  ");
          if (temperatureC < TEMP_OK_MIN_LEVEL) {
            lcd.print("BAIXISSIMO");
            digitalWrite(GREEN_LED_PIN, LOW);
    	  	digitalWrite(YELLOW_LED_PIN, HIGH);
    	  	digitalWrite(RED_LED_PIN, LOW);
    	  	tone(BUZZER_PIN, 1000);
          }
          else {
            lcd.print("ALTISSIMO");
            digitalWrite(GREEN_LED_PIN, LOW);
    	  	digitalWrite(YELLOW_LED_PIN, HIGH);
    	  	digitalWrite(RED_LED_PIN, LOW);
    	  	tone(BUZZER_PIN, 1000);
          }
        }
      }

      if(order == 2) {
      	// Printa a umidade pro LCD
        if (humidityPercentage >= HUM_OK_MIN_LEVEL && humidityPercentage <= HUM_OK_MAX_LEVEL) {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Umidade:");
          lcd.setCursor(0,1);
          lcd.print(humidityPercentage, 1);
          lcd.print("%  OK");
          digitalWrite(GREEN_LED_PIN, HIGH);
    	  digitalWrite(YELLOW_LED_PIN, LOW);
    	  digitalWrite(RED_LED_PIN, LOW);
    	  noTone(BUZZER_PIN);
        }
        else {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Umidade:");
          lcd.setCursor(0,1);
          lcd.print(humidityPercentage, 1);
          lcd.print("%  ");
          if (humidityPercentage < HUM_OK_MIN_LEVEL) {
            lcd.print("BAIXISSIMO");
          	digitalWrite(GREEN_LED_PIN, LOW);
    	  	digitalWrite(YELLOW_LED_PIN, LOW);
    	  	digitalWrite(RED_LED_PIN, HIGH);
    	  	tone(BUZZER_PIN, 1000);
          }
          else {
            lcd.print("ALTISSIMO");
            digitalWrite(GREEN_LED_PIN, LOW);
    		digitalWrite(YELLOW_LED_PIN, LOW);
    		digitalWrite(RED_LED_PIN, HIGH);
    		tone(BUZZER_PIN, 1000);
          }
        } 
      }

 } 
}
