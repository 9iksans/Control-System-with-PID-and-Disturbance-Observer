  #include <LiquidCrystal.h>
  #include <Encoder.h>
  #include <CaptureTimer.h>
  #define button1 1
  #define button2 2

  #define ctINPin     52 
  #define samplingPer   10
  #define ENCODER_OPTIMIZE_INTERRUPS
  #define gndMotor 2
  #define pwmMotor 3 

  float rpmp, periode, freq = 0;
  Encoder encModule(24,25);
  LiquidCrystal lcd (8 , 9, 4, 5, 6, 7);
  float Voltage = 0;
  float vRead = 0;
  float R1  = 30000;
  float R2  = 3300;
  float duty;
  static int mode = 2;
  char inbyte;  
  long positionModule = 0;
  long newEncModule;
  long prevMillis = 0;
  long SetPoint;
  int adc_key_in = 0;
  float Proportional, Integral, Derivative, PID;
  static float Err = 0, LastErr = 0;
  static float IntegralErr = 0, DerivativeErr = 0;
  static float KP = 0.0289;
  static float KI = 1.81;
  static float KD = 0.000116;
  static float TimeSampling = 0.01; // 10 ms

double hasilDobJunc[3] = {0,0,0};
double outDob = 0; 
double lowpassOut[3]={0,0,0};
double outFilterV[4] = {0,0,0,0} , rpmf[4] = {0,0,0,0};

int read_LCD_buttons(){
  adc_key_in  = analogRead(0);
  if (adc_key_in < 250)  return button1;
  if (adc_key_in < 450)  return button2;

  return mode;
}



void lowPass(){
  lowpassOut[2] = lowpassOut[1];
  lowpassOut[1] = lowpassOut[0];
  lowpassOut[0] = (hasilDobJunc[0] * 0.02041) + (hasilDobJunc[1] * 0.04082) + (hasilDobJunc[2] * 0.02041) + (lowpassOut[1] * 1.429) + (lowpassOut[2] * (-0.5102));

  return;
}


void dobJunc(){
  hasilDobJunc[2] = hasilDobJunc[1];
  hasilDobJunc[1] = hasilDobJunc[0];
  hasilDobJunc[0] = PID - outDob;

  if (hasilDobJunc[0] > 255) hasilDobJunc[0] = 255;
  else if (hasilDobJunc[0] < 0) hasilDobJunc[0] = 0;

  analogWrite (pwmMotor, (int)hasilDobJunc[0]);

  delay(10);
  return;
}



uint16_t ticks;
  void readMotorFrequency(){
    CaptureTimer::getFreq(&ticks);
    return;
    }
    
void frekuensi(){
  rpmf[3] = rpmf[2];
  rpmf[2] = rpmf[1];
  rpmf[1] = rpmf[0];
  rpmf[0] = ((float)ticks * 60) / 200;

  return;
}


void outFilter(){
  outFilterV[3] = outFilterV[2];
  outFilterV[2] = outFilterV[1];
  outFilterV[1] = outFilterV[0];
  outFilterV[0] = (rpmf[0] * 0.008424) + (rpmf[1] * (-0.001956)) + (rpmf[2] * (-0.00597)) + (rpmf[3] * 0.00441) + (outFilterV[1] * 1.3859) + (outFilterV[2] * (-0.4477)) + (outFilterV[3] * (-0.02231));  
  
  return;
}

void dobJunc2(){
  outDob = outFilterV[0] - lowpassOut[0];
  return;
}

void SetPID(){
    
    Err = newEncModule - rpmf[0];
    
    IntegralErr += (Err * TimeSampling);
    if (IntegralErr >= 1000) IntegralErr = 1000;
    else if(IntegralErr <= -1000) IntegralErr = -1000;
    
    DerivativeErr = (Err - LastErr) / TimeSampling;
    LastErr = Err;
    
    
    
    Proportional = Err * KP;
    Integral = IntegralErr * KI;
    Derivative = DerivativeErr * KD;
    
    PID = Proportional + Integral + Derivative;

return;
    //Serial.print("PID = "); Serial.println(PID);
}


void cetak(){
  Serial.print("PWM = ");
  Serial.print(newEncModule);
  Serial.print(" | Duty Cycle = ");
  Serial.print(duty);
  Serial.print(" | RPMF = ");
  Serial.print((int)rpmf);
  Serial.print(" | RPMP = ");
  Serial.print((int)rpmp);
  Serial.print(" | TEGANGAN = ");
  Serial.print(Voltage);
  Serial.println(' ');
  
  }
void cetakplot(){
  Serial.print((int)rpmf[0]);
  Serial.print(" ");
  Serial.println(newEncModule);
  //Serial.print(" ");
  //Serial.println(PID); 
 
  }

/*void cetakplot(){
  Serial.print(rpmf[0]);
  Serial.print(" ");
  Serial.print(newEncModule); 
  Serial.print(" "); 
  Serial.print(lowpassOut[0]);
  Serial.print(" ");
  Serial.print(hasilDobJunc[0]);
  Serial.print(" ");
  Serial.print(outFilterV[0]);
  Serial.print(" ");
  Serial.println(outDob);

  

  }*/

void cetakmfc(){

 Serial.print('`');
 Serial.print(newEncModule);
 Serial.print('$');
 Serial.print((int)rpmf);
 Serial.print('~');
 Serial.print((int)rpmp);
 Serial.print('\n');

 }
void setup() {

  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("ENC   = ");
  lcd.setCursor(0,1);
  lcd.print("RPM   = ");
  digitalWrite (gndMotor, LOW);
  // initialize the capture timer pin & period
  CaptureTimer::initCapTicks(samplingPer, ctINPin);
  CaptureTimer::initCapTime(ctINPin);

}





void loop() {

      if(Serial.available()){
        inbyte = Serial.read();
         if (inbyte == '1'){
           mode = 1;
           newEncModule =0;
         }else if(inbyte == '2'){
           mode = 2;
         }
      }else{
          mode = read_LCD_buttons();
      }
      

if (mode == 1){
    
     newEncModule = encModule.read() * 5;
  
    if (newEncModule != positionModule){
        if (newEncModule >= 4500 ){
            newEncModule = 4500;
        } else if(newEncModule <=0){
            newEncModule = 0;
        }
        
        //duty = (float)newEncModule / 255 * 100;
            
        positionModule = newEncModule;
      
      }
      
      
      SetPID();
      dobJunc();
      //motorInp();
      lowPass();
      readMotorFrequency();
      frekuensi();
      outFilter();
      dobJunc2();
    

      cetakplot();
      
  
      
      //periodes();
      //vRead  = analogRead(10) * 3.3 /1024;
      //Voltage = vRead / (R2 / (R1+R2));
      //Voltage = Voltage * ((float)newEncModule/255);


  }else{
    newEncModule = 0;
    PID = 0;  
    analogWrite (pwmMotor, LOW); 
  }
}


/*
Err = SP -PF;


interror += Err * TS;
deferror = Err - Lasterror / ts;
lasterror = Err*/

