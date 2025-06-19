#include <Arduino.h>

class Led {
    private:
        int pin;
        bool stato;

    public:
        // Costruttore
        Led(int ledPin) {
            pin = ledPin;
            pinMode(pin, OUTPUT);
            stato = false;
        }

        // Metodo per accendere il LED
        void accendi() {
            digitalWrite(pin, HIGH);
            stato = true;
        }

        // Metodo per spegnere il LED
        void spegni() {
            digitalWrite(pin, LOW);
            stato = false;
        }

        // Metodo per invertire lo stato del LED
        void inverti() {
            stato = !stato;
            digitalWrite(pin, stato ? HIGH : LOW);
        }

        // Metodo per ottenere lo stato attuale del LED
        bool isAcceso() {
            return stato;
        }
};

// Definizione della classe LED
Led led1(2); // Crea un'istanza della classe LED, collegata al pin 2

void setup() {
    // Inizializzazione della porta seriale per il debug
    Serial.begin(115200);
    Serial.println("Inizio programma");
}

void loop() {
    // Aziona il LED
    led1.accendi();
    delay(1000);
    led1.spegni();
    delay(1000);
    led1.inverti();
    delay(1000);

    // Stampa lo stato del LED
    Serial.print("Stato del LED: ");
    Serial.println(led1.isAcceso() ? "Acceso" : "Spento");
    delay(1000);
}