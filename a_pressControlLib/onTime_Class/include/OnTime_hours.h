/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 08.19.45
*/

// ORE
private:
    int8_t m_last_hour = 99;
    std::map<uint8_t, int8_t> m_at_last_hour_map;

public:
    bool onHour();