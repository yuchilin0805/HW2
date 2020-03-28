#include "mbed.h"
#define SAMPLENUM 1024
Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int sample = SAMPLENUM;
int i;
int j=0;

float biggestarr[10];
int biggestarr_ind[10];
float ADCdata[SAMPLENUM];
int freq;
int freq_d[3];
int sum=0;

int main(){
  
  for (i = 0; i < sample; i++){
    Aout=Ain;
    ADCdata[i] = Ain;
    wait(1./sample);
  } 
  for(i=0;i<SAMPLENUM;i++){
    if(i==0 || i==SAMPLENUM-1)
      continue;
    if(ADCdata[i]>ADCdata[i-1] && ADCdata[i]>ADCdata[i+1] ){
      biggestarr[j]=ADCdata[i];
      biggestarr_ind[j]=i;
      j++;
    }
    if(j==10)
      break;
  }   
  /*for (i = 0; i < sample; i++){
    if(ADCdata[i]>biggest){
        biggest=ADCdata[i];
        biggest_ind=i;
    }
  }
  for(i=0;i<sample;i++){
    if(biggest-ADCdata[i]<0.02 && (biggest_ind-i>5 || i-biggest_ind>5) ){
      if(j!=0 && i-biggestarr_ind[j-1]<5 )
        continue;
      biggestarr[j]=ADCdata[i];
      biggestarr_ind[j]=i;
      j++;
    }
    if(j==10)
      break;
  }*/
  sum=0;
  for(i=0;i<9;i++){
      sum=sum+(biggestarr_ind[i+1]-biggestarr_ind[i]);
  }
  
  freq=9*SAMPLENUM/sum;

  freq_d[0]=freq/100;
  freq_d[1]=freq/10%10;
  freq_d[2]=freq%10;

  for (i = 0; i < sample; i++){
    pc.printf("%1.3f\r\n", ADCdata[i]);
    wait(0.01);
  }
  float k;
  while(1){
    for(k=0; k<2; k+=0.1 ){
        Aout = 0.5 + 0.5*sin(k*3.14159);
        redLED = 1;
        greenLED = 0;
        display=0;
        if(Switch==0){
          redLED=0;
          greenLED=1;
          display=table[freq_d[0]];
          wait(1);
          display=0;
          wait(0.01);
          display=table[freq_d[1]];
          wait(1);
          display=0;
          wait(0.01);
          display=table[freq_d[2]]+0x80;
          wait(1);
          display=0;
          wait(0.01);
        }
        wait(1./freq/20);
      }
  }
  //pc.printf(" %1.3f ",biggest);

}