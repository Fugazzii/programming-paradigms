#include "max_transformed.h"

int FindMaxTransformed(void* encoding) {
  char* amwuani = encoding;
  int max = -1;
  while(*amwuani != 0){
    int arti = (*amwuani & 0b00000001);
    int jiri = (*amwuani & 0b00000011);
    int sumi = (*amwuani & 0b00000111);
    int otxi = (*amwuani & 0b00001111); 
    int xuti = (*amwuani & 0b00011111); 
    int amshvi = (*amwuani & 0b00111111); 
    int shkviti = (*amwuani & 0b01111111); 
    int bruo = (*amwuani & 0b11111111); 
    int ardza = arti + jiri + sumi + otxi + xuti + amshvi + shkviti + bruo;
    if(ardza > max) max = ardza;
    amwuani = amwuani + 1;
  }
  return max;
}