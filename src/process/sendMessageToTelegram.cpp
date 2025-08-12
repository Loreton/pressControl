//
// updated by ...: Loreto Notarantonio
// Date .........: 12-08-2025 16.38.53
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <HTTPClient.h>

#include <lnLogger_Class.h>
// #include "main.h"

#define __LN_TELEGRAM_CREDENTIALS_H__
    #include "@ln_esp32_telegram_credentials.h"
#undef __LN_TELEGRAM_CREDENTIALS_H__




void sendMessageToTelegram(const char* message, const char* parseMode) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Buffer per l'URL completo
        char telegramApiUrl[512];

        // Codifica il messaggio per l'URL. In questo esempio semplice non lo facciamo per semplicità,
        // ma in un'applicazione reale sarebbe necessario per caratteri speciali.

        // Formatta l'URL completo con il parse_mode
        snprintf(telegramApiUrl, 512, "https://api.telegram.org/bot%s/sendMessage?chat_id=%s&parse_mode=%s&text=%s",
                Loreto_Esp32_BotToken, pressControl_ChatID, parseMode, message);

        http.begin(telegramApiUrl);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            LOG_NOTIFY("Codice di risposta HTTP: %d", httpResponseCode);
        } else {
            LOG_ERROR("Errore nella richiesta HTTP: %d", httpResponseCode);
        }

        http.end();
    }

    else {
        LOG_ERROR("msg non sent: WiFi not active.");
    }
}


void sendMessageToTelegram(const char* message) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        LOG_NOTIFY("telegram message: %s", message);

        // Buffer per l'URL. Scegli una dimensione adeguata!
        char telegramApiUrl[256];

        // Formatta l'URL completo con sprintf
        snprintf(telegramApiUrl, 255, "https://api.telegram.org/bot%s/sendMessage?chat_id=%lld&text=%s", Loreto_Esp32_BotToken, pressControl_ChatID, message);

        http.begin(telegramApiUrl);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          LOG_NOTIFY("Codice di risposta HTTP: %d", httpResponseCode);
        } else {
          LOG_ERROR("Errore nella richiesta HTTP: %d", httpResponseCode);
        }
        http.end();
    } else {
        LOG_ERROR("msg non sent: WiFi not active.");
    }
}