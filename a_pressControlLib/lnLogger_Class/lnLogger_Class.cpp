/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-08-2025 15.38.51
*/

#include <Arduino.h>
#include <ESP32Time.h> // ESP32Time.cpp
#include <freertos/semphr.h> // Required here for mutex implementation

#include <lnLogger_Class.h> // Changed to new header file name

ESP32Time       this_rtc;


// Constructor: Initializes the mutex
ESP32Logger::ESP32Logger(void) { // Changed class name
    // The constructor does the bare minimum, or nothing.
    // The mutex will be created in init().
    m_logMutex = NULL; // Initialize to NULL to indicate it hasn't been created yet
};

// Used so I can send a message if I can't initialize the mutex
void ESP32Logger::init() { // Changed class name
    if (!m_mutexInitialized) {
        m_logMutex = xSemaphoreCreateMutex();
        if (m_logMutex == NULL) {
            // If mutex creation fails, it's a serious error,
            // but at least Serial is now initialized to try.
            Serial.println("\n\nERRORE: Impossibile creare mutex per il logger! Logging non protetto.\n\n");
        }
        else {
            m_mutexInitialized = true;
            Serial.println("\n\tOK: Logger Mutex inizializzato.\n");
        }
    }
}





/**
 * @brief Formats the file name and line number.
 * The file name is truncated to a maximum length and padded with dots if shorter.
 * @param file The full path of the file (usually __FILE__).
 * @param function  (usually __FUNCTION__).
 * @param line The line number (usually __LINE__).
 * @return A constant string containing the formatted file name, function name and line number.
 */

// per eventuali test:
//     /media/loreto/LnDisk_SD_ext4/Filu/GIT-REPO/c-cpp/variEsempi/formatLogFname.cpp
const char* ESP32Logger::getFileLineInfo(char *outBUFFER, const uint8_t outBUFFER_LEN, const char* file, const char* function, int line) {

    // Estrai il nome del file (senza percorso)
   const char *filename = strrchr(file, '/');
   filename = filename ? filename + 1 : file;


    const char paddingChar      = '.'; // carattere di padding
    const uint8_t func_len      = strlen(function);
    const uint8_t line_len      = 1 + 3; // 1 per ':' + 3 per il numero di riga
    const uint8_t fixed_len     = func_len + line_len  + 1;
    const uint8_t file_len      = outBUFFER_LEN - fixed_len; // 1 per il terminatore '\0'

    char lineBuff[line_len+1];
    snprintf(lineBuff, line_len+1 ,":%3d", line);
    // printf("lineBuff: [%s]\n", lineBuff);



    // printf("outBUFFER_LEN: %d\n", outBUFFER_LEN);
    // printf("func_len:      %d\n", func_len);
    // printf("line_len:      %d\n", line_len);
    // printf("file_len:      %d\n", file_len);
    // printf("somma:         %d\n", file_len + fixed_len);

    int8_t j;

    // --- fill in buffer whith paddingChar
    for (j = 0; j < outBUFFER_LEN; j++) { outBUFFER[j] = paddingChar; }
    outBUFFER[outBUFFER_LEN] = '\0'; // terminatore buffer
    // printf("outBUFFER filled    [%s] len: %ld\n", outBUFFER, strlen(outBUFFER));


    // copiamo nome del file in outBUFFER
    char *ptr = outBUFFER;
    for (j = 0; j < file_len && *ptr != '\0'; j++) { // --- verifichiamo anche non superare il limite del buffer '\0' messo precedentemente
        if (filename[j] == '\0' || filename[j] == '_' || filename[j] == '.') {
            break;
        }
        *ptr++ = filename[j]; // riempi con il carattere di padding
    }
    *ptr++ = '.'; // separator file.function
    // printf("outBUFFER file      [%s] len: %ld\n", outBUFFER, strlen(outBUFFER));


    // --- copiamo function name to outBUFFER
    for (j = 0; function[j] != '\0' && *ptr != '\0'; j++) {  // --- verifichiamo anche non superare il limite del buffer '\0' messo precedentemente
        *ptr++ = function[j];
    }
    // printf("outBUFFER function  [%s] len: %ld\n", outBUFFER, strlen(outBUFFER));



    // --- copiamo function name to outBUFFER
    ptr = &outBUFFER[outBUFFER_LEN - line_len];
    for (j = 0; lineBuff[j] != '\0' && *ptr != '\0'; j++) {  // --- verifichiamo anche non superare il limite del buffer '\0' messo precedentemente
        *ptr++ = lineBuff[j];
    }
   *ptr = '\0'; // terminatore della stringa (per sicurezza.....)
    // printf("outBUFFER completed [%s] len: %ld\n", outBUFFER, strlen(outBUFFER));


    return outBUFFER;
}


/**
 * @brief Internal function for actual log output.
 * This function is called by the logging macros.
 * @param color ANSI color code for output.
 * @param tag Log level label (e.g. "ERR", "INF").
 * @param file The source file path.
 * @param line The line number in the source file.
 * @param format Printf-like format string.
 * @param ... Variable arguments for the format string.
 */
// void ESP32Logger::write(const char* color, const char* tag, const char* file, int line, const char* format, ...) { // Changed class name
void ESP32Logger::write(const char* color, const char* tag, const char* file, const char* function, int line, const char* format, ...) { // Changed class name
    if (!m_mutexInitialized) {
        // If not initialized, we cannot use the mutex or Serial.
        // You might print a crude warning message or discard the log.
        // This is the most critical situation: a log before Serial.init() and myLog.init().
        // For now, we print it to Serial but without guarantees of visibility.
        // For very early logs, you might consider a FIFO buffer or JTAG/SWD.
        Serial.printf("AVVISO: Logging prima dell'inizializzazione: (lanciare il init())");
        // Serial.printf(format, ##__VA_ARGS__);
        Serial.println();
        return; // Exit to avoid issues
    }

    // Try to acquire the mutex. Wait indefinitely (portMAX_DELAY) if it's already locked.
    if (m_logMutex != NULL && xSemaphoreTake(m_logMutex, portMAX_DELAY) == pdTRUE) {
        char nowTimeBUFFER[16];
        char fnameBUFFER[25];
        char logLineBUFFER[256];

        va_list args;
        va_start(args, format);
        int len = vsnprintf(logLineBUFFER, sizeof(logLineBUFFER), format, args);
        va_end(args);

        if (len >= sizeof(logLineBUFFER)) {
            logLineBUFFER[sizeof(logLineBUFFER) - 1] = '\0';
        }

        struct tm timeinfo = this_rtc.getTimeStruct();
        snprintf(nowTimeBUFFER, sizeof(nowTimeBUFFER), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

        Serial.printf("%s[%s][%s][%s] %s%s\n",
                      color,
                      nowTimeBUFFER,
                      this->getFileLineInfo(fnameBUFFER, sizeof(fnameBUFFER), file, function, line),
                      tag,
                      logLineBUFFER,
                      LogColors::RESET);

        xSemaphoreGive(m_logMutex); // Release the mutex, allowing other tasks to log
    } else {
        // This part of the code is executed only if the mutex was not created
        // or if mutex acquisition fails (extremely rare with portMAX_DELAY).
        // You might still print a basic message to not lose the log,
        // but it won't be thread-safe.
        Serial.printf("!!! Failed to acquire log mutex or mutex not initialized: ");
        // Serial.printf(format, ##__VA_ARGS__); // Print the original unformatted message
        Serial.println();
    }
}

ESP32Logger lnLog; // Changed class name