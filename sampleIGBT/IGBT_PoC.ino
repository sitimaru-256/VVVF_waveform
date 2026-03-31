#include <avr/io.h>
#include <avr/interrupt.h>
float frq; float frqUs;
float basfrq = 0.0;
float dutyu; float dutyv; float dutyw;
float usdutyu; float usdutyv; float usdutyw;
float ratio; float ratiob; float ratioe; float ratiobe;
float offset; float offsetb; float offsete; float offsetbe;
int i = 0; int k = 0; int u = 0; float t = 0;
int pulseMode = 0;
int motorState = 0;
int arraynum;

float sin9u[] = {0.065,0.18,0.33,0.5,0.67,0.82,0.935,0.99,0.99,0.935,0.82,0.67,0.5,0.33,0.18,0.065,0.01,0.01};
float sin9v[] = {0.935,0.99,0.99,0.935,0.82,0.67,0.5,0.33,0.18,0.065,0.01,0.01,0.065,0.18,0.33,0.5,0.67,0.82};
float sin9w[] = {0.5,0.33,0.18,0.065,0.01,0.01,0.065,0.18,0.33,0.5,0.67,0.82,0.935,0.99,0.99,0.935,0.82,0.67};
float sin33u[] = {0.005,0.015,0.025,0.045,0.065,0.095,0.12,0.155,0.19,0.23,0.27,0.315,0.36,0.405,0.45,0.5,0.55,0.595,0.64,0.685,0.73,0.77,0.81,0.845,0.88,0.905,0.935,0.955,0.975,0.985,0.995,1.0,1.0,0.995,0.985,0.975,0.955,0.935,0.905,0.88,0.845,0.81,0.77,0.73,0.685,0.64,0.595,0.55,0.5,0.45,0.405,0.36,0.315,0.27,0.23,0.19,0.155,0.12,0.095,0.065,0.045,0.025,0.015,0.005,0.0,0.0};
float sin33v[] = {0.77,0.81,0.845,0.88,0.905,0.935,0.955,0.975,0.985,0.995,1.0,1.0,0.995,0.985,0.975,0.955,0.935,0.905,0.88,0.845,0.81,0.77,0.73,0.685,0.64,0.595,0.55,0.5,0.45,0.405,0.36,0.315,0.27,0.23,0.19,0.155,0.12,0.095,0.065,0.045,0.025,0.015,0.005,0.0,0.0,0.005,0.015,0.025,0.045,0.065,0.095,0.12,0.155,0.19,0.23,0.27,0.315,0.36,0.405,0.45,0.5,0.55,0.595,0.64,0.685,0.73};
float sin33w[] = {0.73,0.685,0.64,0.595,0.55,0.5,0.45,0.405,0.36,0.315,0.27,0.23,0.19,0.155,0.12,0.095,0.065,0.045,0.025,0.015,0.005,0.0,0.0,0.005,0.015,0.025,0.045,0.065,0.095,0.12,0.155,0.19,0.23,0.27,0.315,0.36,0.405,0.45,0.5,0.55,0.595,0.64,0.685,0.73,0.77,0.81,0.845,0.88,0.905,0.935,0.955,0.975,0.985,0.995,1.0,1.0,0.995,0.985,0.975,0.955,0.935,0.905,0.88,0.845,0.81,0.77};

void setup() {
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(39, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);
  TIMSK3 |= (1 << ICIE3)|(1 << TOIE3);
  TCCR3A = 0b10101000; // 位相補償PWM,TOP値はICR指定
  TCCR3B = 0b00010010;
  TIMSK4 |= (1 << TOIE4);
  TCCR4A = 0b10101010; // 高速PWM,TOP値はICR指定
  TCCR4B = 0b00011010;
}

ISR (TIMER3_OVF_vect) {
  if (pulseMode == 0){
    arraynum = 63;
    frq = 1050;
    dutyu = usdutyu * ratio + offset;
    dutyv = usdutyv * ratio + offset;
    dutyw = usdutyw * ratio + offset;
    i = u;
  }
  else if (pulseMode == 1){
    arraynum = 63;
    frq = 1050;
    dutyu = usdutyu * ratiob + offsetb;
    dutyv = usdutyv * ratiob + offsetb;
    dutyw = usdutyw * ratiob + offsetb;
    i = u;
  }
  else if (pulseMode == 2){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = sin9u[i] * ratio + offset;
    dutyv = sin9v[i] * ratio + offset;
    dutyw = sin9w[i] * ratio + offset;
    i += 1;
  }
  else if (pulseMode == 3){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = sin9u[i] * ratiob + offsetb;
    dutyv = sin9v[i] * ratiob + offsetb;
    dutyw = sin9w[i] * ratiob + offsetb;
    i += 1;
  }
  else if (pulseMode == 4){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = constrain(sin9u[i] * ratioe + offsete, 0.0, 1.0);
    dutyv = constrain(sin9v[i] * ratioe + offsete, 0.0, 1.0);
    dutyw = constrain(sin9w[i] * ratioe + offsete, 0.0, 1.0);
    i += 1;
  }
  else if (pulseMode == 5){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = constrain(sin9u[i] * ratiobe + offsetbe, 0.0, 1.0);
    dutyv = constrain(sin9v[i] * ratiobe + offsetbe, 0.0, 1.0);
    dutyw = constrain(sin9w[i] * ratiobe + offsetbe, 0.0, 1.0);
    i += 1;
  }
  ICR3 = (unsigned int)(1000000 / frq);
  OCR3A = (unsigned int)(1000000 / frq * dutyu);
  OCR3B = (unsigned int)(1000000 / frq * dutyv);
  OCR3C = (unsigned int)(1000000 / frq * dutyw);
  TCCR3A = 0b10101010; //位相基準に変更
  if (i > arraynum){
    i = 0;
    if (ratio < 0.6296 && motorState == 1){pulseMode = 0;}
    if (ratiob < 0.3402 && motorState == -1){pulseMode = 1;u=0;}
    if (ratio >= 0.6296 && ratio < 1 && motorState == 1){pulseMode = 2;}
    if (ratiob >= 0.3402 && ratiob < 1 && motorState == -1){pulseMode = 3;}
    if (ratioe >= 1 && motorState == 1){pulseMode = 4;}
    if (ratiobe >= 1 && motorState == -1){pulseMode = 5;}
  }
}

ISR (TIMER3_CAPT_vect) {
  if (pulseMode == 0){
    arraynum = 63;
    frq = 1050;
    dutyu = usdutyu * ratio + offset;
    dutyv = usdutyv * ratio + offset;
    dutyw = usdutyw * ratio + offset;
    i = u;
  }
  else if (pulseMode == 1){
    arraynum = 63;
    frq = 1050;
    dutyu = usdutyu * ratiob + offsetb;
    dutyv = usdutyv * ratiob + offsetb;
    dutyw = usdutyw * ratiob + offsetb;
    i = u;
  }
  else if (pulseMode == 2){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = sin9u[i] * ratio + offset;
    dutyv = sin9v[i] * ratio + offset;
    dutyw = sin9w[i] * ratio + offset;
    i += 1;
  }
  else if (pulseMode == 3){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = sin9u[i] * ratiob + offsetb;
    dutyv = sin9v[i] * ratiob + offsetb;
    dutyw = sin9w[i] * ratiob + offsetb;
    i += 1;
  }
  else if (pulseMode == 4){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = constrain(sin9u[i] * ratioe + offsete, 0.0, 1.0);
    dutyv = constrain(sin9v[i] * ratioe + offsete, 0.0, 1.0);
    dutyw = constrain(sin9w[i] * ratioe + offsete, 0.0, 1.0);
    i += 1;
  }
  else if (pulseMode == 5){
    arraynum = 17;
    frq = basfrq * 9.0;
    dutyu = constrain(sin9u[i] * ratiobe + offsetbe, 0.0, 1.0);
    dutyv = constrain(sin9v[i] * ratiobe + offsetbe, 0.0, 1.0);
    dutyw = constrain(sin9w[i] * ratiobe + offsetbe, 0.0, 1.0);
    i += 1;
  }
  ICR3 = (unsigned int)(1000000 / frq);
  OCR3A = (unsigned int)(1000000 / frq * dutyu);
  OCR3B = (unsigned int)(1000000 / frq * dutyv);
  OCR3C = (unsigned int)(1000000 / frq * dutyw);
  TCCR3A = 0b10101000; //周波数基準に変更
}

ISR(TIMER4_OVF_vect){
  frqUs = basfrq * 33;
  ICR4 = (unsigned int)(1000000 / frqUs);
  usdutyu = sin33u[u];
  usdutyv = sin33v[u];
  usdutyw = sin33w[u];
  u += 1;
  if (u > 65){u = 0;}
}

void loop() {
  if(digitalRead(39) == LOW){
    motorState = 1;
    if(t<basfrq && basfrq > 0.1){
      k = 0;
      t += (basfrq*0.001);
      ratio = constrain(t * 0.0185, 0.0, 1.0);
      offset = constrain(0.5 - t * 0.00925, 0.0, 0.5);
      ratioe = constrain(t * 0.6667 - 35, 0.0, 3.0);
      offsete = constrain(18 - t * 0.3333, -1.0, 0.5);
    }
    else{
      k = 1;
      t = basfrq + 0.1;
      ratio = constrain(basfrq * 0.0185, 0.0, 1.0);
      offset = constrain(0.5 - basfrq * 0.00925, 0.0, 0.5);
      ratioe = constrain(basfrq * 0.6667 - 35, 0.0, 3.0);
      offsete = constrain(18 - basfrq * 0.3333, -1.0, 0.5);
    }
  }
  if(digitalRead(40) == LOW){
    motorState = -1;
    if(t<basfrq){
      k = 0;
      t += (basfrq*0.001);
      ratiob = constrain(t * 0.0103, 0.0, 1.0);
      offsetb = constrain(0.5 - t * 0.00515, 0.0, 0.5);
      ratiobe = constrain(t * 0.5 - 47.5, 0.0, 3.0);
      offsetbe = constrain(24.25 - t * 0.25, -1.0, 0.5);
    }
    else{
      k= -1;
      t = basfrq;
      ratiob = constrain(basfrq * 0.0103, 0.0, 1.0);
      offsetb = constrain(0.5 - basfrq * 0.00515, 0.0, 0.5);
      ratiobe = constrain(basfrq * 0.5 - 47.5, 0.0, 3.0);
      offsetbe = constrain(24.25 - basfrq * 0.25, -1.0, 0.5);
    }
  }
  if(digitalRead(39) == HIGH && digitalRead(40) == HIGH){
    if(t > 0.0){
      t -= (basfrq*0.001);
      ratio = constrain(t * 0.0185, 0.0, 1.0);
      offset = constrain(0.5 - t * 0.00925, 0.0, 0.5);
      ratioe = constrain(t * 0.6667 - 35, 0.0, 3.0);
      offsete = constrain(18 - t * 0.3333, -1.0, 0.5);
      ratiob = constrain(t * 0.0103, 0.0, 1.0);
      offsetb = constrain(0.5 - t * 0.00515, 0.0, 0.5);
      ratiobe = constrain(t * 0.5 - 47.5, 0.0, 3.0);
      offsetbe = constrain(24.25 - t * 0.25, -1.0, 0.5);
      }
    else{
      motorState = 0;
      t = 0;
      k = 0;
     }
  }
  basfrq += (0.005*k);
  if(basfrq < 0.0){
    basfrq = 0.0;
  }
  delay(1);
}
