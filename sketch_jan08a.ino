#include "ESP32_MailClient.h"
#include "time.h"

// Identifiants de la wifi
const char* ssid = "P30 Lite";
const char* password = "14233221";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// To send Email using Gmail use port 465 (SSL) and SMTP Server smtp.gmail.com
// YOU MUST ENABLE less secure app option https://myaccount.google.com/lesssecureapps?pli=1
#define emailSenderAccount    "esp32.n7.g8@gmail.com"    
#define emailSenderPassword   "gentianbg"
#define emailRecipient        "lotfi.benabdallah1001@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"
#define nomfichiertexte       "/test_genial.txt"
#define nomimage              "/moon.jpg"
#define timer                 true
// The Email Sending data object contains config and data to send
SMTPData smtpData;

// Callback function to get the Email sending status
void sendCallback(SendStatus info);

bool isEqualTime(){
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  if ((timeinfo.tm_hour == 20) and (timeinfo.tm_min == 23)) {
    return true;
  } else {
    return false;
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println();
  
  Serial.print("Connecting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();
  
  // Synchronisation de l'horloge interne 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (!timer) {
    Serial.println("Preparing to send email");
      Serial.println();
      
      // Set the SMTP Server Email host, port, account and password
      smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
    
      // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
      // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
      //smtpData.setSTARTTLS(true);
    
      // Set the sender name and Email
      smtpData.setSender("ESP32", emailSenderAccount);
    
      // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
      smtpData.setPriority("High");
    
      // Set the subject
      smtpData.setSubject(emailSubject);
    
      // Set the message with HTML format
      smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP32 board</p></div>", true);
      // Set the email message in text format (raw)
      //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);
    
      // Add recipients, you can add more than one recipient
      smtpData.addRecipient(emailRecipient);
      //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
    
      // Add attach files from SD card or SPIFFS
      // Comment the next two lines, if no SPIFFS files created or SD card connected
      smtpData.addAttachFile(nomimage, "image/jpg");
      smtpData.addAttachFile(nomfichiertexte);
      smtpData.setSendCallback(sendCallback);
    
      // Set the storage type to attach files in your email (SPIFFS or SD Card)
      smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
      //smtpData.setFileStorageType(MailClientStorageType::SD);
    
      //Start sending Email, can be set callback function to track the status
      if (!MailClient.sendMail(smtpData))
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    
      //Clear all data from Email object to free memory
      smtpData.empty();
  }
}

void loop() {
  if (timer) {
    if (isEqualTime()){
      Serial.println("Preparing to send email");
      Serial.println();
      
      // Set the SMTP Server Email host, port, account and password
      smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
    
      // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
      // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
      //smtpData.setSTARTTLS(true);
    
      // Set the sender name and Email
      smtpData.setSender("ESP32", emailSenderAccount);
    
      // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
      smtpData.setPriority("High");
    
      // Set the subject
      smtpData.setSubject(emailSubject);
    
      // Set the message with HTML format
      smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP32 board</p></div>", true);
      // Set the email message in text format (raw)
      //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);
    
      // Add recipients, you can add more than one recipient
      smtpData.addRecipient(emailRecipient);
      //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
    
      // Add attach files from SD card or SPIFFS
      // Comment the next two lines, if no SPIFFS files created or SD card connected
      smtpData.addAttachFile(nomimage, "image/jpg");
      smtpData.addAttachFile(nomfichiertexte);
      smtpData.setSendCallback(sendCallback);
    
      // Set the storage type to attach files in your email (SPIFFS or SD Card)
      smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
      //smtpData.setFileStorageType(MailClientStorageType::SD);
    
      //Start sending Email, can be set callback function to track the status
      if (!MailClient.sendMail(smtpData))
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    
      //Clear all data from Email object to free memory
      smtpData.empty();
      delay(60000);
    }
  }
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
