//
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 19.23.35
//

#pragma once




private:
    bool           m_ntp_active        = false;
    uint32_t       m_lastNtpAttempt    = 0;
    uint32_t       m_NTP_SYNC_INTERVAL = 2*60*1000UL;
    bool           m_NTP_synched       = false;    // comodo da utilizzare esternamente

    const char*    m_ntpServer1        = "pool.ntp.org";
    const char*    m_ntpServer2        = "time.google.com";
    const char*    m_ntpServer3        = "br.pool.ntp.org";
    const char*    m_ntpServer4        = "time.nist.gov";
    const char*    m_ntpServer5        = "2.br.pool.ntp.org";
    const char*    m_ntpServer6        = "time.windows.com";


    // --- methods
            bool checkNtpSynched(void);
    static  void cbSyncTime(struct timeval *tv) ;




public:
    void initNTP(); // Nuovo metodo pubblico per la sincronizzazione
    bool isNtpSynched(void) const { return m_NTP_synched; }

