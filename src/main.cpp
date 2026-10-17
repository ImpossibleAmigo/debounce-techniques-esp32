#include <Arduino.h>

#define BUTTON_PIN 4
#define POLLING_INTERVAL_MS 5  // Опитуємо кожні 5 мс
#define DEBOUNCE_CHECKS 4      // 4 перевірки * 5 мс = 20 мс стабільного сигналу

// Перелік можливих станів нашої машини
enum ButtonState {
    RELEASED,           // Відпущено (стабільний стан)
    DEBOUNCE_PRESS,     // Потенційне натискання (йде перевірка)
    PRESSED,            // Натиснуто (стабільний стан)
    DEBOUNCE_RELEASE    // Потенційне відпускання (йде перевірка)
};

ButtonState currentState = RELEASED;

unsigned long lastPollTime = 0;
int checkCounter = 0;
int validCounter = 0;

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT);
    Serial.println("Систему запущено. Завдання 4: Polling + State Machine.");
}

void loop() {
    unsigned long currentTime = millis();
    
    // Виконуємо код лише раз на 5 мс
    if (currentTime - lastPollTime >= POLLING_INTERVAL_MS) {
        lastPollTime = currentTime;
        
        // Зчитуємо фізичний рівень на піні
        bool currentLevel = digitalRead(BUTTON_PIN);
        
        // Машина станів
        switch (currentState) {
            case RELEASED:
                if (currentLevel == LOW) { // Почалося натискання
                    currentState = DEBOUNCE_PRESS;
                    checkCounter = 1;
                }
                break;
                
            case DEBOUNCE_PRESS:
                if (currentLevel == LOW) {
                    checkCounter++;
                    if (checkCounter >= DEBOUNCE_CHECKS) { // Сигнал стабільний 20 мс
                        currentState = PRESSED;
                        validCounter++;
                        Serial.printf("Кнопку натиснуто! Валідний лічильник: %d\n", validCounter);
                    }
                } else {
                    currentState = RELEASED; // Сигнал зник - це був брязкіт
                }
                break;
                
            case PRESSED:
                if (currentLevel == HIGH) { // Почалося відпускання
                    currentState = DEBOUNCE_RELEASE;
                    checkCounter = 1;
                }
                break;
                
            case DEBOUNCE_RELEASE:
                if (currentLevel == HIGH) {
                    checkCounter++;
                    if (checkCounter >= DEBOUNCE_CHECKS) { // Сигнал стабільний 20 мс
                        currentState = RELEASED;
                    }
                } else {
                    currentState = PRESSED; // Сигнал зник - брязкіт при відпусканні
                }
                break;
        }
    }
}