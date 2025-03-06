#include "wled.h"
#include "fcn_declare.h"
#include "wled_ethernet.h"
#include <SPI.h>
#include <Ethernet.h>

// W5500 SPI Configuration
#define W5500_CS_PIN   5   // Adjust to your board setup
#define W5500_RST_PIN  4   // Optional, only if reset pin is used
#define SPI_SCK        18  // Adjust to your board setup
#define SPI_MISO       19
#define SPI_MOSI       23

// Define MAC and IP settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Change if needed
IPAddress ip(192, 168, 1, 100);    // Static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// Initialize Ethernet
EthernetClient ethClient;

bool initEthernet() {
    static bool successfullyConfiguredEthernet = false;
  
    if (successfullyConfiguredEthernet) {
        return false;  // Already initialized
    }

    // Setup SPI for W5500
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, W5500_CS_PIN);
    pinMode(W5500_CS_PIN, OUTPUT);
    digitalWrite(W5500_CS_PIN, HIGH);

    // Reset W5500 (optional)
    if (W5500_RST_PIN > 0) {
        pinMode(W5500_RST_PIN, OUTPUT);
        digitalWrite(W5500_RST_PIN, LOW);
        delay(50);
        digitalWrite(W5500_RST_PIN, HIGH);
        delay(50);
    }

    // Start Ethernet with static IP
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    delay(1000); // Allow time for DHCP/static setup

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        DEBUG_PRINTLN(F("Ethernet shield not found!"));
        return false;
    }

    if (Ethernet.linkStatus() == LinkOFF) {
        DEBUG_PRINTLN(F("Ethernet cable not connected."));
        return false;
    }

    successfullyConfiguredEthernet = true;
    DEBUG_PRINTLN(F("Ethernet initialized successfully!"));
    return true;
}

// Get W5500 IP address
IPAddress getLocalIP() {
    return Ethernet.localIP();
}

// Ethernet event handling (simplified)
void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_ETH_CONNECTED:
            DEBUG_PRINTLN(F("Ethernet connected"));
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            DEBUG_PRINTLN(F("Ethernet disconnected"));
            break;
        default:
            DEBUG_PRINTF_P(PSTR("Ethernet Event: %d\n"), (int)event);
            break;
    }
}
