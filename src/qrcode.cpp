#include <Arduino.h>
#include "qrcode.h"
#include "qrencode.h"

int offsetsX = 42;
int offsetsY = 10;
int screenwidth = 128;
int screenheight = 64;
bool QRDEBUG = false;

QRcode::QRcode(OLEDDisplay *display){
	this->display = display;
  this->model = -1;
}

QRcode::QRcode(Adafruit_ST7735 *tft, uint8_t model){
	this->tft = tft;
  this->model = model;
  offsetsX = 20;
  offsetsY = 20;
}

void QRcode::init(){
	if (this->model==-1){
    display->init();
    display->flipScreenVertically();
    display->setColor(WHITE);
  } else {
    screenwidth = tft->width();
    screenheight = tft->height(); 
    tft->initR(model);
    tft->setRotation(1);
    tft->fillScreen(ST77XX_WHITE);
  }
}

void QRcode::debug(){
	QRDEBUG = true;
}

void QRcode::render(int x, int y, int color){
  int multiply = 1;
  if (model != -1)
    multiply = 2;
  x=(x*multiply)+offsetsX;
  y=(y*multiply)+offsetsY;
  if(color==1) {
    if (model==-1) {
	    display->setColor(BLACK);
      display->setPixel(x, y);
    } else {
      tft->drawPixel(x,y,ST77XX_BLACK);
      if (multiply>1) {
        tft->drawPixel(x+1,y,ST77XX_BLACK);
        tft->drawPixel(x+1,y+1,ST77XX_BLACK);
        tft->drawPixel(x,y+1,ST77XX_BLACK);
      }
    }
  }
  else {
    if (model==-1) {
	    display->setColor(WHITE);
      display->setPixel(x, y);
    } else {
      tft->drawPixel(x,y,ST77XX_WHITE);
      if (multiply>1) {
        tft->drawPixel(x+1,y,ST77XX_WHITE);
        tft->drawPixel(x+1,y+1,ST77XX_WHITE);
        tft->drawPixel(x,y+1,ST77XX_WHITE);
      }
    }
  }
}

void QRcode::screenwhite(){
   if (model==-1) {
      display->clear();
      display->setColor(WHITE);
      display->fillRect(0, 0, screenwidth, screenheight);
      display->display();
   } else {
      tft->fillScreen(ST77XX_WHITE);
   }
}

void QRcode::create(String message) {

  // create QR code
  message.toCharArray((char *)strinbuf,260);
  qrencode();
  screenwhite();

  
  // print QR Code
  for (byte x = 0; x < WD; x+=2) {
    for (byte y = 0; y < WD; y++) {
      if ( QRBIT(x,y) &&  QRBIT((x+1),y)) {
        // black square on top of black square
        render(x, y, 1);
        render((x+1), y, 1);
      }
      if (!QRBIT(x,y) &&  QRBIT((x+1),y)) {
        // white square on top of black square
        render(x, y, 0);
        render((x+1), y, 1);
      }
      if ( QRBIT(x,y) && !QRBIT((x+1),y)) {
        // black square on top of white square
        render(x, y, 1);
        render((x+1), y, 0);
      }
      if (!QRBIT(x,y) && !QRBIT((x+1),y)) {
        // white square on top of white square
        render(x, y, 0);
        render((x+1), y, 0);
      }
    }
  }
  if (model==-1)
    display->display();
}
