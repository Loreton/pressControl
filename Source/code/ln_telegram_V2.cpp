/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-05-2025 11.10.14
*/
//  telegram2 async https://github.com/cotestatnt/AsyncTelegram2/tree/master
//                  https://github.com/cotestatnt/AsyncTelegram/blob/master/REFERENCE.md



/* strutture
    #include "DataStructures.h"
    /media/loreto/LnDisk_SD_ext4/Filu/GIT-REPO/ESP32/esp32LnLibrary/extraLibraries/AsyncTelegram2-master/src/DataStructures.h
    struct TBMessage {
        MessageType   messageType;
        bool          isHTMLenabled = true;
        bool          isMarkdownEnabled = false;
        bool          disable_notification = false;
        bool          force_reply = false;
        int32_t       date;
        int32_t       chatInstance;
        int64_t       chatId;
        int32_t       messageID;
        TBUser        sender {
                          bool     isBot;
                          int64_t  id = 0;
                          String   firstName;
                          String   lastName;
                          String   username;
                        };
        TBUser        member {     // A user enter or leave a group
                          bool     isBot;
                          int64_t  id = 0;
                          String   firstName;
                          String   lastName;
                          String   username;
                        };
        TBLocation    location;
        TBContact     contact;
        TBDocument    document;
        int64_t       callbackQueryID;
        String        callbackQueryData;
        String        text;
    };
*/

/*
    Set true if you want use external library for SSL connection instead ESP32@WiFiClientSecure
    For example https://github.com/OPEnSLab-OSU/SSLClient/ is very efficient BearSSL library.
    You can use AsyncTelegram2 even with other MCUs or transport layer (ex. Ethernet)
    With SSLClient, be sure "certificates.h" file is present in sketch folder
*/


#define USE_CLIENTSSL false
#include <AsyncTelegram2.h>

#include <WiFi.h>
#include <WiFiClient.h>
#if USE_CLIENTSSL
    #include <SSLClient.h>
    #include "tg_certificate.h"
    WiFiClient base_client;
    SSLClient client(base_client, TAs, (size_t)TAs_NUM, A0, 1, SSLClient::SSL_ERROR);
#else
    #include <WiFiClientSecure.h>
    WiFiClientSecure client;
#endif







// #include <AsyncTelegram2.h>
// #include <WiFiClientSecure.h>
// #define USE_CLIENTSSL false

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#include "@logMacros.h"
#include "@mainStructures.h"

// ---------------------------------
// - project headers files
// ---------------------------------s
#include "@ln_telegram.h" // /home/loreto/lnProfile/envars/include/@ln_esp32_telegram_definitions.h
#include "@ln_esp32_telegram_credentials.h" // /home/loreto/lnProfile/envars/include/@ln_esp32_telegram_credentials.h
#include "@pressControl.h"



#define BOTtoken        LoretoEsp32BotToken
#define GROUP_CHAT_ID   Esp32_group_chatid
#define fDEBUG false
#define fTelegramNotify true



// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"





// WiFiClientSecure client;
AsyncTelegram2 myBot(client);

// local variable to store telegram message data
TBMessage msg;
telegram_send_struct_t tgMSG;
telegram_send_struct_t *tgMsg = &tgMSG;

// String PROGMEM  welcome = "Welcome, " + from_name + ".\n";
// const PROGMEM char *help_text_0 = "<b>Available commands</b>:\n\n"\
//                                 "/pc on [hh:mm:ss|secs]\n"\
//                                 "/pc off\n"\
//                                 "/pc status\n"\
//                                 "/whoami\n"\
//                                 "/help\n";

const PROGMEM char *help_text = "<b>Available commands</b>:\n\n"\
                                "/on [hh:mm:ss|secs]\n"\
                                "/off\n"\
                                "/status\n"\
                                "/whoami\n"\
                                "/h[elp]\n";







void setupTelegram() {
    #if USE_CLIENTSSL == false
        client.setCACert(telegram_cert);
    #endif
    // Set the Telegram bot properies
    myBot.setUpdateTime(2000);
    myBot.setTelegramToken(BOTtoken);
    printf1_NFN("\nTelegram connection... ");
    myBot.begin() ? printf0_NFN("OK\n") : printf0_NFN("NOK\n");;

    char welcome_msg[128];
    snprintf(welcome_msg, 128, "BOT @%s online\n/help all commands avalaible.", myBot.getBotName());

    // Send a message to specific user who has started your bot
    myBot.sendTo(nLoreto_id, welcome_msg);
    printf0_NFN("Bot name: @%s\n", myBot.getBotName());
}



#define MAX_USERNAME 10
char username[MAX_USERNAME+1];
#define MAX_COMMAND 20
char command[MAX_COMMAND+1];

void handleTelegram() {
    // if there is an incoming message...
    if (myBot.getNewMessage(msg)) {
        String msgText = msg.text;
        int64_t chatid = msg.chatId;
        int64_t userid = msg.sender.id;
        int64_t groupid = msg.member.id; // check if the message comes from a chat group (the group id is negative)

        snprintf(username, MAX_USERNAME, msg.sender.username.c_str());

        printf1_NFN("received msg: %s\n", msgText.c_str());
        printf2_NFN("User:  %s - chatid: %lld - senderid: %lld\n", username, msg.chatId, msg.sender.id);

        if ((chatid != Esp32_PressControl_id) and (chatid != nLoreto_id)) {
            myBot.sendMessage(msg, "Unauthorized user");       // notify the sender
            return;
        }


        uint8_t index=msgText.indexOf("@");
        msgText=msgText.substring(1, index); // cut first chat '/'
        printf2_NFN("cleaned message: %s\n", msgText.c_str());

        // preapre telegram message
        tgMsg->len = sprintf(tgMsg->data, "<b>command:</b>%s\n", msgText.c_str());

        // ----------------------------
        // - check received message
        // ----------------------------
        if (msgText.startsWith("pc ")) {                 // pressControl
            pressControlProcessTelegramMessage((char *)msgText.substring(3).c_str());  // skip 'pc '
        }


        // ----------------------------
        // - check received message
        // ----------------------------
        else if (msgText.startsWith("on") || msgText.startsWith("off") || msgText.startsWith("status") ) {                 // pressControl
            pressControlProcessTelegramMessage((char *)msgText.c_str());
        }


        else if (msgText.startsWith("whoami")) {
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "username: %s\n"  , username);
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "chatId: %lld\n"  , msg.chatId);
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "userId: %lld\n"  , msg.sender.id);
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "firstName: %s\n" , msg.sender.firstName.c_str());
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "LastName: %s\n"  , msg.sender.lastName.c_str());
        }

        else if (msgText.startsWith("h") ) {           // help
            tgMsg->len = 0;
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "%s\n", help_text);
        }


        else {                                              // otherwise...
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "username: %s\n", msg.sender.username);
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "msg: %s\nrequest not valid!\n", msgText.c_str());
            tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "/h for help\n");
        }

        if (tgMsg->len > 0 && tgMsg->len < MAX_TELEGRAM_LENGTH) {
            printf2_NFN("[%d] sending msg: %s\n", tgMsg->len, tgMsg->data);
            myBot.sendMessage(msg, tgMsg->data);       // notify the sender
            tgMsg->len=0;
        }
    }
}





void sendTelegramGroup(char *text_msg) {
    myBot.sendTo(Esp32_PressControl_id, text_msg);
}

void sendTelegramGroup(telegram_send_struct_t *text) {
    myBot.sendTo(Esp32_PressControl_id, text->data);
    text->len=0;
}

void sendToTelegram(void) {
    // printf1_NFN("[%d] sendToTelegram: %s\n", tgMsg->len, tgMsg->data);
    if (tgMsg->len > 0 && tgMsg->len < MAX_TELEGRAM_LENGTH) {
        myBot.sendTo(Esp32_PressControl_id, tgMsg->data);
        tgMsg->len=0;
    }
}

