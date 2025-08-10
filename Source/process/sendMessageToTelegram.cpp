//
// updated by ...: Loreto Notarantonio
// Date .........: 10-08-2025 18.49.08
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <HTTPClient.h>

// #include "main.h"

#define __LN_TELEGRAM_CREDENTIALS_H__
    #include "@ln_esp32_telegram_credentials.h"
#undef __LN_TELEGRAM_CREDENTIALS_H__




/*
void sendMessageToTelegram(String message) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("Sending Telegram messae: %s", message);
        HTTPClient http;
        String telegramApiUrl = "https://api.telegram.org/bot" + telegramBotToken + "/sendMessage?chat_id=" + telegramChatId + "&text=" + message;
        // String telegramApiUrl = "https://api.telegram.org/bot" + LORETO_ESP32_BOT_TOKEN + "/sendMessage?chat_id=" + nLoreto_CHAT_ID + "&text=" + message;

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
*/


// const char *telegramBotToken = LORETO_ESP32_BOT_TOKEN;
// int64_t     telegramChatId = Esp32_PressControl_id;

void sendMessageToTelegram(const char* message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Buffer per l'URL. Scegli una dimensione adeguata!
    char telegramApiUrl[256];

    // Formatta l'URL completo con sprintf
    snprintf(telegramApiUrl, 255, "https://api.telegram.org/bot%s/sendMessage?chat_id=%lld&text=%s", telegramBotToken, telegramChatId, message);

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