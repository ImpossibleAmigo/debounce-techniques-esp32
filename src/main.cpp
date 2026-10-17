#include <Arduino.h>

// Використовуємо пін 4, як домовлялися
#define BUTTON_PIN 4 

// volatile вказує компілятору, що ці змінні можуть змінюватися в перериванні
volatile int isrCounter = 0; 
volatile bool eventTriggered = false;

// Функція переривання (ISR)
// IRAM_ATTR завантажує функцію в оперативну пам'ять (RAM) для максимальної швидкості
void IRAM_ATTR buttonISR() {
    isrCounter++;           // Інкрементуємо лічильник
    eventTriggered = true;  // Сигналізуємо, що відбулася подія
}

void setup() {
    Serial.begin(115200);
    
    // Зовнішній резистор вже є, тому просто INPUT
    pinMode(BUTTON_PIN, INPUT); 
    
    // Налаштовуємо переривання на спад напруги (відпускання -> натискання)
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
    
    Serial.println("Систему запущено. Завдання 1: Базова реалізація (без debounce).");
}

void loop() {
    // Виводити лог всередині самої функції переривання (ISR) - це погана практика, 
    // яка може призвести до паніки ядра (Kernel Panic). 
    // Тому ми логуємо дані в основному циклі за допомогою прапорця.
    if (eventTriggered) {
        // Одразу скидаємо прапорець
        eventTriggered = false; 
        
        // Виводимо лічильник у лог
        Serial.printf("Кнопку натиснуто! Лічильник: %d\n", isrCounter);
    }
}