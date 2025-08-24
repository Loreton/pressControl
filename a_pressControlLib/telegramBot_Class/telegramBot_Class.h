//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 09.55.38
//


#pragma once
    // #include <Arduino.h>    // in testa anche per le definizioni dei type
    // #include <HTTPClient.h>


    #define MAX_MESSAGE_SIZE 512

    class TelegramBot_Class {
        public:
            // Costruttore
            TelegramBot_Class();

            // TelegramBot_Class(const char* token, const char* chatId, const char *parseMode="HTML");
            void init(const char* token, const char* chatId, const char *parseMode="HTML");

            // Metodi per la preparazione del messaggio
            void clearMessage(const char* title=nullptr);
            void startNewMessage(const char* format, ...);
            void addLine(const char* text);
            void addString(const char* text);
            void addFormattedString(const char* format, ...);
            void addTime(void);
            void addTime(const char *prefix, const char *suffix); // nel centro mettiamo TIME
            void setParseMode(const char* mode);

            // Metodo per l'invio
            bool send();

        private:

            const char* m_token;
            const char* m_chatId;
            char m_messageBuffer[MAX_MESSAGE_SIZE];
            char m_parseMode[10];
            char m_timeStamp[16];

            // Funzione helper per l'URL-encoding
            void urlEncode(const char* src, char* dest);

            // Funzione helper per l'invio HTTP
            bool sendHttpRequest(const char* url);
    };
