//Pin defines for Board Version 0.1  -  23/11/14

//PB0-7
#define KP_Col3        0
#define despenceStatus 1 //24v so INVERTED
#define coinIn         2
#define trayStatus     3 //24v so INVERTED
#define RFID_RST       4 //MUST BE OUTPUT
#define HW_MOSI        5
#define HW_MISO        6
#define HW_SCK         7

//PD0-7
#define HW_Rx          8
#define HW_Tx          9
#define ESP_Rx         10
#define ESP_Tx         11
#define Shift_DATA     12
#define Shift_ENABLE   13
#define Shift_LATCH    14
#define Shift_CLOCK    15

//PC0-7
#define ESP_ENABLE     16
#define SDCard_CS      17
#define RFID_CS        18
#define QVGA_CS        19
#define QVGA_RST       20
#define QVGA_DC        21
#define KP_Col2        22
#define KP_Col1        23

//PA0-7
#define KP_RowG        24//A0
#define KP_RowF        25//A1
#define KP_RowE        26//A2
#define KP_RowD        27//A3
#define KP_RowC        28//A4
#define KP_RowB        29//A5
#define KP_RowA        30//A6
#define doorStatus     31//A7
