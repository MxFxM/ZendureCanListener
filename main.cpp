#include <Arduino.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> myCan;
// FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> myCan;

void canSniff(const CANFD_message_t &msg);

void setup()
{
    // Serial connection to PC
    Serial.begin(9600);

    myCan.begin();
    myCan.setBaudRate(500000); // 500kbps
    // myCan.setMaxMB(16);
    // CANFD_timings_t config;
    // config.clock = CLK_60MHz;
    // config.baudrate = 500000;
    // config.baudrateFD = 1000000;
    // config.propdelay = 190;
    // config.bus_length = 1;
    // config.sample = 70;
    // myCan.setBaudRate(config);
    myCan.enableMBInterrupts(); // enables all mailboxes to be interrupt enabled
    myCan.onReceive(canSniff);  // allows all FIFO/message box messages to be received in the supplied callback.
    myCan.mailboxStatus();
}

void loop()
{
    // put your main code here, to run repeatedly:
    myCan.events();
    delay(1000);
}

void canSniff(const CANFD_message_t &msg)
{
    Serial.print("MB ");
    Serial.print(msg.mb);
    Serial.print("  OVERRUN: ");
    Serial.print(msg.flags.overrun);
    Serial.print("  LEN: ");
    Serial.print(msg.len);
    Serial.print(" EXT: ");
    Serial.print(msg.flags.extended);
    Serial.print(" TS: ");
    Serial.print(msg.timestamp);
    Serial.print(" ID: ");
    Serial.print(msg.id, HEX);
    Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < msg.len; i++)
    {
        Serial.print(msg.buf[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}