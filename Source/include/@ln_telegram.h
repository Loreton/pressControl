/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-02-2025 19.14.52
*/
/*
 * Rui Santos
 * Complete Project Details http://randomnerdtutorials.com
*/

#include <Arduino.h>

#ifndef __TELEGRAM_H__
    #define __TELEGRAM_H__

    extern const int64_t Esp32_group_chatid;

    #define MAX_TELEGRAM_LENGTH 200
    typedef struct {
        char    PROGMEM data[MAX_TELEGRAM_LENGTH]; // buffer per preparare i dati da inviare a telegram
        bool    ready = false;
        int16_t len = 0; // usato per mantere il pointer nel buffer
    } telegram_send_struct_t;

    extern telegram_send_struct_t *tgMsg;

    void setupTelegram(void);
    void handleTelegram(void);
    void sendToTelegram(void); // manda di default al gruppo ed il messaggio in tgMsg->data

    void sendTelegramGroup(char *text_msg);
    void sendTelegramGroup(telegram_send_struct_t *text);

#endif


// #else