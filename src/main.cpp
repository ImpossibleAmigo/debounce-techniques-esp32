#include <Arduino.h>

#define BUTTON_PIN 4 
#define DEBOUNCE_DELAY_MS 50 // Затримка антибрязкоту в мілісекундах

volatile bool eventTriggered = false;

int validCounter = 0; // Лічильник успішних (відфільтрованих) натискань
unsigned long lastDebounceTime = 0; // Час останнього успішного натискання

// ISR тепер робить мінімум роботи
void IRAM_ATTR buttonISR() {
    eventTriggered = true; 
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT); 
    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
    
    Serial.println("Систему запущено. Завдання 2: Time-based debounce (поза ISR).");
}

void loop() {
    if (eventTriggered) {
        eventTriggered = false; // Одразу скидаємо прапорець
        
        unsigned long currentTime = millis(); // Фіксуємо поточний час
        
        // Якщо з моменту попереднього натискання пройшло більше 50 мс
        if (currentTime - lastDebounceTime >= DEBOUNCE_DELAY_MS) {
            lastDebounceTime = currentTime; // Оновлюємо таймер
            validCounter++;
            
            Serial.printf("Кнопку натиснуто! Валідний лічильник: %d\n", validCounter);
        } else {
            // Тут ми ігноруємо брязкіт (час < 50 мс)
            // Serial.println("-> Відфільтровано брязкіт"); // Розкоментуйте, щоб побачити як це працює
        }
    }
}