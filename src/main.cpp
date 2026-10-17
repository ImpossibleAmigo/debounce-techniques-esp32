#include <Arduino.h>

#define BUTTON_PIN 4 

volatile int isrCounter = 0; 
volatile bool eventTriggered = false;

// Додамо невеликий програмний бар'єр всередині ISR,
// щоб повністю захистити плату від "зависання" та спаму
volatile unsigned long lastInterruptTime = 0;

void IRAM_ATTR buttonISR() {
    unsigned long interruptTime = millis();
    // Якщо переривання відбуваються швидше ніж 10 мс одне за одним — 
    // це 100% апаратний шум або брязкіт, ми їх ігноруємо на рівні заліза.
    if (interruptTime - lastInterruptTime > 10) { 
        isrCounter++;           
        eventTriggered = true;  
    }
    lastInterruptTime = interruptTime;
}

void setup() {
    Serial.begin(115200);
    
    // INPUT_PULLUP активує внутрішній резистор ESP32 на 30-50 кОм.
    // Це застрахує нас, якщо зовнішній резистор 10 кОм підключений неправильно.
    pinMode(BUTTON_PIN, INPUT_PULLUP); 
    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
    
    Serial.println("Систему запущено. Завдання 5: Апаратний RC-фільтр + Safe ISR.");
}

void loop() {
    if (eventTriggered) {
        eventTriggered = false; 
        Serial.printf("Кнопку натиснуто! Лічильник: %d\n", isrCounter);
    }
}