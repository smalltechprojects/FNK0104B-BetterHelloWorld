/*simple hello world program for Freenove CYD ESP32S3 
displays text and then displays a file
*/

#include <SPI.h>
#include <FS.h>
#include <SD_MMC.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

// !! Replace with the SD pins from your FNK0104B schematic/example !!
#define SD_MMC_CLK  38
#define SD_MMC_CMD  40
#define SD_MMC_D0   39

TFT_eSPI tft = TFT_eSPI();

// Callback function used by the decoder to "push" image blocks to the screen
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  // Stop rendering if the image goes off-screen
  if (y >= tft.height()) return false;
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}

void setup() {
  Serial.begin(115200);

  // Turn on the Backlight (CYD pin 21)
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  // Initialize Display
  tft.begin();
  tft.setRotation(1); // 1 = Landscape (USB port on the right)
  tft.fillScreen(TFT_BLACK);

  // Initialize Freenove's 1-bit SDMMC Card Slot
  Serial.print("Initializing SD Card...");
// 1. Assign custom pins for FNK0104B
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

  // begin(mountpoint, mode1bit, format_if_mount_failed)
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD card mount failed");
    return;
  }

    // Configure the JPEG Decoder
  TJpgDec.setJpgScale(1);      // 1:1 scale
  TJpgDec.setSwapBytes(true);  // Corrects color byte order for ESP32
  TJpgDec.setCallback(tft_output);


  //print hello world
  Serial.println("Setup Complete.");
  tft.setCursor(10,50);
  tft.println("hello world");
  delay(4000);

//printer hello world centered
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  int screenWidth = tft.width();
  int screenHeight = tft.height();
  tft.drawCentreString("hello world centered",tft.width()/2, tft.height()/2-10, 2);
  delay(4000);

 // Display Image 1
  Serial.println("Loading helloworld.jpg...");
  tft.fillScreen(TFT_BLACK);
  // Parameters: X, Y, Filename
 
  TJpgDec.drawFsJpg(0, 0, "/helloworld.jpg", SD_MMC);
  delay(4000);

 
}

void loop() {
 
}