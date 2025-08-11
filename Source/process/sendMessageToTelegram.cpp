//
// updated by ...: Loreto Notarantonio
// Date .........: 11-08-2025 11.08.26
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <HTTPClient.h>

// #include "main.h"

#define __LN_TELEGRAM_CREDENTIALS_H__
    #include "@ln_esp32_telegram_credentials.h"
#undef __LN_TELEGRAM_CREDENTIALS_H__





void sendMessageToTelegram(const char* message) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Buffer per l'URL. Scegli una dimensione adeguata!
        char telegramApiUrl[256];

        // Formatta l'URL completo con sprintf
        snprintf(telegramApiUrl, 255, "https://api.telegram.org/bot%s/sendMessage?chat_id=%lld&text=%s", Loreto_Esp32_BotToken, pressControl_ChatID, message);

        http.begin(telegramApiUrl);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          Serial.print("Codice di risposta HTTP: ");
          Serial.println(httpResponseCode);
        } else {
          Serial.print("Errore nella richiesta HTTP: ");
          Serial.println(httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("WiFi disconnesso");
    }
}