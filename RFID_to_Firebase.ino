  #include <ESP8266WiFi.h>//for connecting to internet
  #include <SPI.h>//for rfid scanner hardware
  #include <MFRC522.h>//for rfid scanner hardware
  #include <ESP8266Firebase.h>//for firebase
  #define REFERENCE_URL "https://flashtank-ye2736-ad4r5-rtdb.firebaseio.com/"//link copied from firebase realtime database page
  Firebase firebase(REFERENCE_URL);

  // below 3 lines for rfid
  String lastCardUID = "";
  int card = 0;
  int precard = 0;

  const char* ssid = "esphotspot";//my wifi  name
  const char* password = "qwertyuiop";//my wifi password
  
  // below 3 lines for rfid
  #define SS_PIN D8
  #define RST_PIN D0
  MFRC522 mfrc522(SS_PIN, RST_PIN); 

  void setup() { // Connecting to wifi
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

    SPI.begin();           
    mfrc522.PCD_Init(); 
  }

  void loop() {
    //scanning for card
    if (mfrc522.PICC_IsNewCardPresent()) {//rfid card found
      if (mfrc522.PICC_ReadCardSerial()) {
        // create a lastCardUID to store data
        lastCardUID = "";
        //verify :
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          lastCardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
          lastCardUID += String(mfrc522.uid.uidByte[i], HEX);
        }
        lastCardUID.toUpperCase();//save the id everything on caps
        
      }
    }
    
    if (lastCardUID == "716BA810") {//if it is card1
      card = 1;
    } 
    
    else if (lastCardUID == "6312F150") {//if it is card2
      card = 2;
      
    } 
    
    else if (lastCardUID == "7058C714") {//if it is card3
      card = 3;
    } 
    if (card != precard)
    {
      //for creating a folder named cards in realtime database set 1/2/3 based on card
      firebase.setInt("cards/", card); 
      precard = card;
    }

    delay(1000);
  }