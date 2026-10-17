#include <Arduino.h>

#define BUTTON_PIN 4
#define DEBOUNCE_DELAY_MS 20 // 20 мс зазвичай достатньо, щоб брязкіт вщух

volatile bool eventTriggered = false;

int validCounter = 0;
unsigned long debounceStartTime = 0;
bool isDebouncing = false;

// ISR лише сигналізує про подію
void IRAM_ATTR buttonISR() {
    eventTriggered = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
    
    Serial.println("Систему запущено. Завдання 3: State-based debounce.");
}

void loop() {
    // 1. Ловимо подію від ISR і починаємо відлік, якщо ми ще не в процесі перевірки
    if (eventTriggered && !isDebouncing) {
        eventTriggered = false; 
        isDebouncing = true; 
        debounceStartTime = millis(); 
    }

    // 2. Якщо йде перевірка і час (20 мс) вже пройшов
    if (isDebouncing && (millis() - debounceStartTime >= DEBOUNCE_DELAY_MS)) {
        isDebouncing = false; // Завершуємо очікування
        eventTriggered = false; // Очищаємо всі хибні прапорці, що нападали за ці 20 мс
        
        // 3. ПЕРЕВІРКА РІВНЯ: чи кнопка ДОСІ натиснута (LOW)?
        if (digitalRead(BUTTON_PIN) == LOW) {
            validCounter++;
            Serial.printf("Кнопку натиснуто! Валідний лічильник: %d\n", validCounter);
        } else {
            // Це був брязкіт або відпускання. Ми нічого не робимо.
            // Serial.println("-> Подію проігноровано (рівень HIGH)");
        }
    }
}