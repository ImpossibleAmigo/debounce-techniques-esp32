#include <Arduino.h>

#define BUTTON_PIN 4 

volatile int isrCounter = 0; 
volatile bool eventTriggered = false;

void IRAM_ATTR buttonISR() {
    isrCounter++;           
    eventTriggered = true;  
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT); 
    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
    
    Serial.println("Систему запущено. Завдання 5: Апаратний RC-фільтр + Базове переривання.");
}

void loop() {
    if (eventTriggered) {
        eventTriggered = false; 
        Serial.printf("Кнопку натиснуто! Лічильник: %d\n", isrCounter);
    }
}