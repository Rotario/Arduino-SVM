#include <avr/pgmspace.h>
#define VEC_DIM 0

#define SVM_TYPE one_class
#define KERNEL_TYPE rbf
#define GAMMA 0.27000000000000002
#define NR_CLASS 2
#define TOTAL_SV 5
const  PROGMEM float rho[] = {1.3592287781644834};
 const PROGMEM float yalpha1[0 * (NR_CLASS-1)] = {}; 

 const PROGMEM float sv1[0 * VEC_DIM] = {}; 

 const PROGMEM float yalpha2[0 * (NR_CLASS-1)] = {}; 

 const PROGMEM float sv2[0 * VEC_DIM] = {}; 

const int scalePar[] = {-1,1};
const int low[] = {-8.1453999999999996e-05,-0.0017057699999999999,-0.17382800000000001,0.17322499999999999,-11.431100000000001};
const int high[] = {3.6015700000000001e-05,0.00638238,0.017949099999999999,2.13971,-3.1751800000000001};  
int result[NR_CLASS]={0};

 float const* const supportVectors[NR_CLASS] PROGMEM = {
sv1, sv2, };
 const float* const valuesForSupport[NR_CLASS] PROGMEM = {
yalpha1, yalpha2, };

void scale(const int* sensor, float* scaledSensor){
  for(int p=0; p<VEC_DIM;p++){
    scaledSensor[p] = (float)scalePar[0] +((float)scalePar[1] -((float)scalePar[0]))*((float)sensor[p]-(float)low[p])/((float)high[p]-(float)low[p]);
  }
}

inline void svm_predict(int sensor[]){
  int recognizedClass = 1;
  float scaledSensor[VEC_DIM];
  scale(sensor,scaledSensor);
  int rhoCounter = 0;


  for(int i=0; i<NR_CLASS; i++){
    for(int j=i+1; j<NR_CLASS; j++){
      float accumulator = 0;


      float* sv_class1 = (float*) pgm_read_word(supportVectors + i);
      float* sv_class2 = (float*) pgm_read_word(supportVectors + j);
      float* coeffs1   = (float*) pgm_read_word(valuesForSupport + i) + nr_sv[i] * (j-1);
     float* coeffs2   = (float*) pgm_read_word(valuesForSupport + j) + nr_sv[j] * i;


      accumulator += svm_evaluate(nr_sv[i], coeffs1, sv_class1, scaledSensor);
      accumulator += svm_evaluate(nr_sv[j], coeffs2, sv_class2, scaledSensor);


      float rhoNr = pgm_read_float(rho + rhoCounter);
      accumulator -= rhoNr;


      if (accumulator > 0) {
        result[i]++;
      } else {
        result[j]++;
      }
      rhoCounter++;
    }
  }


  int temp = 0;
  for(int t = 0; t < NR_CLASS; t++){
    if(result[temp] <= result[t]){
      recognizedClass = t;
      temp = t;
    }
  }


  Serial.println(recognizedClass, DEC);
  delay(500);
  for(int q = 0; q < NR_CLASS; q++){
    result[q]=0;
  }
}
