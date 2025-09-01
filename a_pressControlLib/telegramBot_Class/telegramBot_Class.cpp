//
// updated by ...: Loreto Notarantonio
// Date .........: 01-09-2025 11.40.43
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>
#include <stdarg.h> // Per i parametri variabili di addFormattedString
#include <HTTPClient.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "telegramBot_Class.h"



TelegramBot_Class::TelegramBot_Class() {}



void TelegramBot_Class::init(const char* token, const char* chatId, const char *parseMode) {
    m_token = token;
    m_chatId = chatId;
    setParseMode(parseMode);
    clearMessage();
}





// #######################################################################
// # aggiunge una linea in formato HTML quindi aggiunge uno %0a finale
// # forse posso eliminarla mettendo lo '\n' nella addString()
// #######################################################################
void TelegramBot_Class::clearMessage(const char* title) {
    m_messageBuffer[0] = '\0';
    if (title) {
        addString(title);
    }
}

// #######################################################################
// # aggiunge una linea in formato HTML quindi aggiunge uno %0a finale
// # forse posso eliminarla mettendo lo '\n' nella addString()
// #######################################################################
void TelegramBot_Class::startNewMessage(const char* format, ...) {
    m_messageBuffer[0] = '\0';
    if (strlen(m_messageBuffer) < MAX_MESSAGE_SIZE) {
        char tempBuffer[MAX_MESSAGE_SIZE - strlen(m_messageBuffer)];
        va_list args;
        va_start(args, format);
        vsnprintf(tempBuffer, sizeof(tempBuffer), format, args);
        va_end(args);
        strcat(m_messageBuffer, tempBuffer);
    }
}


// #######################################################################
// # aggiunge una linea in formato HTML quindi aggiunge uno %0a finale
// # forse posso eliminarla mettendo lo '\n' nella addString()
// #######################################################################
void TelegramBot_Class::addLine(const char* text) {
    if (strlen(m_messageBuffer) + strlen(text) + 2 < MAX_MESSAGE_SIZE) { // +2 per il '\n' e il terminatore
        if (strlen(m_messageBuffer) > 0) {
            strcat(m_messageBuffer, "\n"); // Aggiunge il carattere di newline
        }
        strcat(m_messageBuffer, text);
    }
}


// #######################################################################
// # stringa
// #######################################################################
void TelegramBot_Class::addString(const char* text) {
    if (strlen(m_messageBuffer) + strlen(text) < MAX_MESSAGE_SIZE) {
        strcat(m_messageBuffer, text);
    }
}



// #######################################################################
// # stringa in formato printf()
// #######################################################################
void TelegramBot_Class::addFormattedString(const char* format, ...) {
    if (strlen(m_messageBuffer) < MAX_MESSAGE_SIZE) {
        char tempBuffer[MAX_MESSAGE_SIZE - strlen(m_messageBuffer)];
        va_list args;
        va_start(args, format);
        vsnprintf(tempBuffer, sizeof(tempBuffer), format, args);
        va_end(args);

        strcat(m_messageBuffer, tempBuffer);
    }
}

// #######################################################################
// # add "HH:MM:SS"
// #######################################################################
void TelegramBot_Class::addTime(void) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      // Fallback in caso di errore
      addString("TIME_ERROR");
      return;
    }

    char timeStr[9];

    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    addString(timeStr);
}


// #######################################################################
// # add "HH:MM:SS"
// #######################################################################
void TelegramBot_Class::addTime(const char *prefix, const char *suffix) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      // Fallback in caso di errore
      addString("TIME_ERROR");
      return;
    }


    // prendiamo il tempo;
    strftime(m_timeStamp, sizeof(m_timeStamp), "%H:%M:%S", &timeinfo);

    // uint8_t max_len=strlen(prefix) + strlen(m_timeStamp) + strlen(suffix) +2;

    addFormattedString("%s%s%s", prefix, m_timeStamp, suffix);


    // char buffer[max_len];
    // snprintf(buffer, max_len, "%s%s%s", prefix, m_timeStamp, suffix);
    // addString(buffer);
}



// #######################################################################
// # set parse mode
// #######################################################################
void TelegramBot_Class::setParseMode(const char* mode) {
    strncpy(m_parseMode, mode, sizeof(m_parseMode) - 1);
    m_parseMode[sizeof(m_parseMode) - 1] = '\0';
}




// #######################################################################
// # prima dell'invio passa il messaggio alla urlencode per convertice caratteri speciali
// #######################################################################
bool TelegramBot_Class::send() {
    if (WiFi.status() != WL_CONNECTED) {
        LOG_ERROR("WiFi non connected. Message non sent.");
        return false;
    }

    HTTPClient http;
    char urlBuffer[MAX_MESSAGE_SIZE + 100];
    char encodedMessage[MAX_MESSAGE_SIZE * 3 + 1]; // Buffer per il messaggio codificato

    urlEncode(m_messageBuffer, encodedMessage);

    // Costruisce l'URL completo con tutti i parametri
    snprintf(urlBuffer, sizeof(urlBuffer),
             "https://api.telegram.org/bot%s/sendMessage?chat_id=%s&parse_mode=%s&text=%s",
             m_token, m_chatId, m_parseMode, encodedMessage);

    LOG_TRACE("Sending msg: %s", urlBuffer);
    http.begin(urlBuffer);
    int httpResponseCode = http.GET();
    http.end();

    if (httpResponseCode > 0) {
        LOG_NOTIFY("Messaggio inviato con successo: %s", urlBuffer);
        return true;
    } else {
        LOG_ERROR("Invio messaggio fallito! %s", urlBuffer);
        return false;
    }
}




// | Carattere                   | Escape C/C++ | Decimale | Esadecimale | URL encoding                                                     |
// | --------------------------- | ------------ | -------- | ----------- | ---------------------------------------------------------------- |
// | **NUL**                     | `\0`         | 0        | 00          | `%00`                                                            |
// | **TAB**                     | `\t`         | 9        | 09          | `%09`                                                            |
// | **LF** (line feed, newline) | `\n`         | 10       | 0A          | `%0A`                                                            |
// | **CR** (carriage return)    | `\r`         | 13       | 0D          | `%0D`                                                            |
// | **SPACE**                   | `' '`        | 32       | 20          | `%20` *(oppure `+` solo in `application/x-www-form-urlencoded`)* |
// | **"** (doppio apice)        | `\"`         | 34       | 22          | `%22`                                                            |
// | **'** (apice singolo)       | `'`          | 39       | 27          | `%27`                                                            |
// | **/** (slash)               | `/`          | 47       | 2F          | `%2F`                                                            |
// | **\\** (backslash)          | `\\`         | 92       | 5C          | `%5C`                                                            |


void TelegramBot_Class::urlEncode(const char* src, char* dest) {
    const char *p = src;
    char *q = dest;
    while (*p) {
        if (isalnum((unsigned char)*p) || *p == '-' || *p == '_' || *p == '.' || *p == '~') {
            *q++ = *p;
        } else if (*p == ' ') {
            *q++ = '+';
        } else if (*p == '\n') { // <--- Aggiungi questa condizione
            *q++ = '%';
            *q++ = '0';
            *q++ = 'A';
        } else if (*p == '\t') { // <--- Aggiungi questa condizione
            *q++ = '%';
            *q++ = '0';
            *q++ = '9';
        } else {
            // Codifica i caratteri speciali in formato esadecimale %XX
            sprintf(q, "%%%02X", (unsigned char)*p);
            q += 3;
        }
        p++;
    }
    *q = '\0';
}