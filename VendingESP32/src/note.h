/* MQTT
New product created on APP/Website with an ID of row/column ie A1 or C3
If machine is online it gets sent to machine for local storage.
Local secure webServer with INPUT field to enter commands

So if someone enters A8 on the screen, I publish "A8" to "techspace/vending/getproduct"
Change the QR code to "https://gctechspace.org/shop/vending/A8"
Listen on "techspace/vending/product" for "name|price(in 0.50cents)|stocklevel|VERY short description" ie "Resistor Pack|2|4|10k Ohm"
I might see how much info I can store locally too, and only query if necessary.
And after every vend, publish the itemLocation to either...
VENDING_TOPIC Success or Fail
*/

/* vending tray setup
A5
B5
C5
D10
E10
F5
*/

/* Nextion Screen


HOME
prints 'H' to Serial
home.pic=10 //Red Screen
home.pic=5 //Blue Screen
home.creditText.txt="OUT OF ORDER"
touch goes to row screen

ROW
Prints A-F for row, goes to column screen
Prints R for redeem voucher, goes to redeem screen

COLUMN/COLUMN5
Prints 0-9 for column, goes to info screen
Back goes to row screen

INFO
info.itemNameText.txt
info.infoText.txt
info.priceText.txt
info.qrCode.txt="https://gctechspace.org/"
touch goes to home screen

tm0.tim=5000 //sesonds
//counts down to zero are sets page to Home

REDEEM
prints a 6 digit voucher code

MAINTANCE
"Add Credit", prints 'M' goes to money page
"Set Price", prints 'P' goes to row/column/money page
"Test Motor", prints 'T' goes to row/column page
"Sensors", prints 'S' goes to info page
"Info", prints 'I' goes to info page

MONEY
prints int Money Value




* WiFi Events
0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
2  SYSTEM_EVENT_STA_START                < ESP32 station start
3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
25 SYSTEM_EVENT_MAX

*/
