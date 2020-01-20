#define BUTT 2
#define red 10
#define blue 11
#define green 9
#define time1 600   // час між другою і першою пульсаціями 
#define tspalax 25  // час пульсації
#define time2 150 // час між першою і другою пульсаціями
#include <EEPROM.h>
#include "GyverButton.h"
GButton butt1(BUTT, LOW_PULL, NORM_OPEN);
byte sp = 50;  //швидкість зміни кольору, менше == швидше, мінімум 1
const byte cpurple[] = {255, 0, 170};
const byte cred[] = {255, 0, 0};
const byte cblue[] = {0, 0, 255};
const byte cgreen[] = {0, 255, 0};
const byte cpink[] = {255, 0, 100};
const byte cyelow[] = {255, 180, 0};
byte color[] = {255, 0, 0};
byte color_ch, color_ch_pulse, color_ch_beng;
bool ch;
bool bright_mode;
bool br_down;
byte color_step = 1;
byte pulse_speed = 7;
long t1, t2, t3, t4, t5, t6, t7;
byte global_br = 150;
byte cr, cg, cb;
byte p, mode, last_mode;
bool br_mode;
bool temp;
bool pulse_done;
byte br, bg, bb;
byte control;
byte pr;
void setup() {
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  TCCR1A = 0b00000001; // 8bit
  TCCR1B = 0b00001010; // x8 fast pwm
  TCCR2B = 0b00000010; // x8
  TCCR2A = 0b00000011; // fast pwm
  butt1.setClickTimeout(600);
  last_mode = EEPROM.read(0);
  Serial.print(last_mode);
  global_br = EEPROM.read(1);
 // Serial.print(global_br);
}

void loop() {
  butt1.tick();
  
  if (br_mode == 1){
    bright();
  }
  switch(mode){
    case 0 : {
    off();
  }break;
    case 1 :{
      if (ch == 0){
      oneColorHeat();
    }
    if (ch == 1){
      colorChangeHeat();
    }
    last_mode = 1;} break;
    case 2 : {
      colorRotation();
      last_mode = 2;
      } break;
    case 3 : {
      oneColor();
      last_mode = 3;
      }break;
    case 4 : {
      if (color_ch_pulse == 0){
      pulse();}
      if (color_ch_pulse == 1){
        pulseRotation();
      }
      last_mode = 4;
      } break;
    case 5 : {
      white();
      last_mode = 5;
      }break;
    case 6 :{
      if(color_ch_beng == 0){
      bengRotation();
      }
      if (color_ch_beng == 1){
        beng();
      }
      last_mode = 6;
      } break;
    default : mode = 1; break;
  }
  
    if (butt1.isSingle()){
    mode = 0;
    }
    if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
    }
    if (butt1.getClicks() == 4){
    mode = 5;
    }
    
}

void white(){
  butt1.tick();
  if (temp == 0){
    cr = global_br;
    cg = global_br;
    cb = global_br;
  }
  if (temp == 1){
    cr = global_br;
    cg = global_br * 150 / 255;
    cb = global_br * 40 / 255;
  }
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
  if (butt1.isHolded()){
    br_mode = 1;
  }
  if (butt1.isTriple()){
    temp = !temp;
  }
//  Serial.println("White");

}

void off(){
  butt1.tick();
  analogWrite(red, 0);
  analogWrite(green, 0);
  analogWrite(blue, 0);
  if (butt1.isSingle()){
    mode = last_mode;
  }
  if (butt1.getClicks() == 4){
    mode = 5;
    }
}

void oneColorHeat(){
  butt1.tick();
   t1 = millis();
  if (t1 - t2 > time1){
    p = 200 * global_br / 255;
  if (t1 - t2 > time1 + tspalax){
    p = 100 * global_br / 255;
    if (t1 - t2 > time1 + tspalax + time2){
       p = 200 * global_br / 255;
       if (t1 - t2 > time1 + 2 * tspalax + time2){
        p = 100 * global_br / 255;
        t2 = t1;
        }
    }
  }
  }
  cr = p * cpurple[0] / 255;
  cg = p * cpurple[1] / 255;
  cb = p * cpurple[2] / 255;
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
  if (butt1.isTriple()){
    ch = !ch;
  }
  
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
//  Serial.println("Head");
}

void colorChangeHeat(){
  
  butt1.tick();
  t1 = millis();
  if (t1 - t2 > time1){
    p = 200 * global_br / 255;
  if (t1 - t2 > time1 + tspalax){
    p = 100 * global_br / 255;
    if (t1 - t2 > time1 + tspalax + time2){
       p = 200 * global_br / 255;
       if (t1 - t2 > time1 + 2 * tspalax + time2){
        p = 100 * global_br / 255;
        t2 = t1;
        }
    }
  }
  }
  cr = p * color[0] / 255;
  cg = p * color[1] / 255;
  cb = p * color[2] / 255;
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if(t1 - t3 >  sp){
        if (color[0] == 255 && color[1] < 255  && color[2] == 0){ //1
          color[1] = color[1] + color_step;
        }
        if (color[0] > 0 && color[1] == 255 && color[2] ==  0){   //2
          color[0] = color[0] - color_step;
        }
        if (color[0] == 0 && color[1] == 255 && color[2] < 255 ){  //3
          color[2] = color[2] + color_step;
        }
        if (color[0] == 0 && color[1] > 0 && color[2] == 255){   //4
          color[1] = color[1] - color_step;
        }
        if (color[0] < 255 && color[1] == 0 && color[2] == 255){  //5
          color[0] = color[0] + color_step; 
        }
        if (color[0] == 255 && color[1] == 0 && color[2] > 0){   //6
          color[2] = color[2] - color_step;
        }
        t3 = t1;
  }
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
  if (butt1.isTriple()){
    ch = !ch;
  }
  
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
//  Serial.println("Head");
}

void colorRotation(){
  butt1.tick();
  t1 = millis();
  if(t1 - t3 >  sp){
        if (color[0] == 255 && color[1] < 255  && color[2] == 0){ //1
          color[1] = color[1] + color_step;
        }
        if (color[0] > 0 && color[1] == 255 && color[2] ==  0){   //2
          color[0] = color[0] - color_step;
        }
        if (color[0] == 0 && color[1] == 255 && color[2] < 255 ){  //3
          color[2] = color[2] + color_step;
        }
        if (color[0] == 0 && color[1] > 0 && color[2] == 255){   //4
          color[1] = color[1] - color_step;
        }
        if (color[0] < 255 && color[1] == 0 && color[2] == 255){  //5
          color[0] = color[0] + color_step; 
        }
        if (color[0] == 255 && color[1] == 0 && color[2] > 0){   //6
          color[2] = color[2] - color_step;
        }
        t3 = t1;
  }
  cr = color[0] * global_br / 255;
  cg = color[1] * global_br / 255;
  cb = color[2] * global_br / 255;

  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
  
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
//  Serial.println("ROT");
}

void oneColor(){
  butt1.tick();
  switch(color_ch){
    case 0 : {}
      break;
    case 1 : {
      color[0] = cred[0];
      color[1] = cred[1];
      color[2] = cred[2];
      }
      break;
    case 2 : {
      color[0] = cblue[0];
      color[1] = cblue[1];
      color[2] = cblue[2];
      }
      break;
    case 3 : {
      color[0] = cgreen[0];
      color[1] = cgreen[1];
      color[2] = cgreen[2];
      }
      break;
    case 4 : {
      color[0] = cpink[0];
      color[1] = cpink[1];
      color[2] = cpink[2];
      }
      break;
    case 5 : {
      color[0] = cyelow[0];
      color[1] = cyelow[1];
      color[2] = cyelow[2];
      }
      break;
    default : { color_ch = 1; }
      break;
  }
  cr = color[0] * global_br / 255;
  cg = color[1] * global_br / 255;
  cb = color[2] * global_br / 255;
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
  
  if (butt1.isTriple()){
    color_ch++;
  }
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
//  Serial.println("One Color");
}

void pulseRotation(){
  butt1.tick();
  t1 = millis();
  t5 = millis();
  if (pulse_done == 0 && t5 - t4 > pulse_speed){
    p++;
    t4 = t5;
  }
  if (pulse_done == 1 && t5 - t4 > pulse_speed){
    p--;
    t4 = t5;
  }
  if (p == 255 && pulse_done == 0){
    pulse_done = 1;
  }
  if (p == 0 && pulse_done == 1){
    pulse_done = 0;
  }  
  if(t1 - t3 >  sp){
        if (color[0] == 255 && color[1] < 255  && color[2] == 0){ //1
          color[1] = color[1] + color_step;
        }
        if (color[0] > 0 && color[1] == 255 && color[2] ==  0){   //2
          color[0] = color[0] - color_step;
        }
        if (color[0] == 0 && color[1] == 255 && color[2] < 255 ){  //3
          color[2] = color[2] + color_step;
        }
        if (color[0] == 0 && color[1] > 0 && color[2] == 255){   //4
          color[1] = color[1] - color_step;
        }
        if (color[0] < 255 && color[1] == 0 && color[2] == 255){  //5
          color[0] = color[0] + color_step; 
        }
        if (color[0] == 255 && color[1] == 0 && color[2] > 0){   //6
          color[2] = color[2] - color_step;
        }
        t3 = t1;
  }
  pr = p * global_br/255;
  cr = color[0] *  pr / 255;
  cg = color[1] * pr / 255;
  cb = color[2] * pr / 255;
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
 if (butt1.isTriple()){
    color_ch_pulse = !color_ch_pulse;
  }  
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
  //Serial.println(global_br);
}

void pulse(){
  butt1.tick();
  t1 = millis();
  if (pulse_done == 0 && t1 - t4 > pulse_speed){
    p++;
    t4 = t1;
  }
  if (pulse_done == 1 && t1 - t4 > pulse_speed){
    p--;
    t4 = t1;
  }
  if (p == 255 && pulse_done == 0){
    pulse_done = 1;
  }
  if (p == 0 && pulse_done == 1){
    pulse_done = 0;
  }
  pr = p * global_br/255;
  cr = color[0] *  pr / 255;
  cg = color[1] * pr / 255;
  cb = color[2] * pr / 255;

  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
  if (butt1.isTriple()){
    color_ch_pulse = !color_ch_pulse;
  }
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
    }
  control = color[1]*p*global_br/255/255;
  /*Serial.print(p);
  Serial.print("  ");
  Serial.println(cr);*/
}

void bengRotation(){
  butt1.tick();
  t1 = millis();
  t5 = millis();
  if (pulse_done == 0 && t5 - t4 > pulse_speed){
    p = p + 51;
    t4 = t5;
  }
  if (pulse_done == 1 && t5 - t4 > pulse_speed){
    p--;
    t4 = t5;
  }
  if (p == 255 && pulse_done == 0){
    pulse_done = 1;
  }
  if (p == 0 && pulse_done == 1){
    pulse_done = 0;
  }  
  if(t1 - t3 >  sp){
        if (color[0] == 255 && color[1] < 255  && color[2] == 0){ //1
          color[1] = color[1] + color_step;
        }
        if (color[0] > 0 && color[1] == 255 && color[2] ==  0){   //2
          color[0] = color[0] - color_step;
        }
        if (color[0] == 0 && color[1] == 255 && color[2] < 255 ){  //3
          color[2] = color[2] + color_step;
        }
        if (color[0] == 0 && color[1] > 0 && color[2] == 255){   //4
          color[1] = color[1] - color_step;
        }
        if (color[0] < 255 && color[1] == 0 && color[2] == 255){  //5
          color[0] = color[0] + color_step; 
        }
        if (color[0] == 255 && color[1] == 0 && color[2] > 0){   //6
          color[2] = color[2] - color_step;
        }
        t3 = t1;
  }
  pr = p * global_br/255;
  cr = color[0] *  pr / 255;
  cg = color[1] * pr / 255;
  cb = color[2] * pr / 255;
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
 if (butt1.isTriple()){
    color_ch_beng = !color_ch_beng;
  }  
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
}

void beng(){
  butt1.tick();
  t1 = millis();
  t5 = millis();
  if (pulse_done == 0 && t5 - t4 > pulse_speed){
    p = p + 51;
    t4 = t5;
  }
  if (pulse_done == 1 && t5 - t4 > pulse_speed){
    p--;
    t4 = t5;
  }
  if (p == 255 && pulse_done == 0){
    pulse_done = 1;
  }
  if (p == 0 && pulse_done == 1){
    pulse_done = 0;
  }  
  
  pr = p * global_br/255;
  cr = color[0] *  pr / 255;
  cg = color[1] * pr / 255;
  cb = color[2] * pr / 255;
  analogWrite(red, cr);
  analogWrite(green, cg);
  analogWrite(blue, cb);
  if (butt1.isSingle()){
    mode = 0;
  }
  if (butt1.isDouble()){
    mode++;
    EEPROM.update(0,mode);
  }
 if (butt1.isTriple()){
    color_ch_beng = !color_ch_beng;
  }  
  if (butt1.getClicks() == 4){
    mode = 5;
    }
  if (butt1.isHolded()){
    br_mode = 1;
  }
}

void bright(){
  butt1.tick();
  bright_mode = 0;
  while (bright_mode == 0){
  butt1.tick();
  
  if (butt1.isClick()){
    global_br = global_br + 15;
  }

  if (butt1.isHolded()){
    bright_mode = 1;
    br_mode = 0;
  }
  br = color[0] * global_br / 255;
  bg = color[1] * global_br / 255;
  bb = color[2] * global_br / 255;
  analogWrite(red, br);
  analogWrite(green, bg);
  analogWrite(blue, bb);
 // Serial.println("bright_while");
  }
  EEPROM.update(1,global_br);

}
