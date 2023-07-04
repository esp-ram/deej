const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3};

int analogSliderValues[NUM_SLIDERS];

const int buttonInputs[NUM_SLIDERS] = {2, 3, 4, 5};

int buttonValues[NUM_SLIDERS];
unsigned long buttonsLastUpdated[NUM_SLIDERS];

unsigned long debounceDelay = 250;

const int leds[NUM_SLIDERS] = {6, 7, 8, 9};
int ledStatus[NUM_SLIDERS];


void setup() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    pinMode(buttonInputs[i], INPUT);
    buttonValues[i] = 0;
    buttonsLastUpdated[i] = millis();
    pinMode(leds[i], OUTPUT);
    ledStatus[i] = LOW;
  }
  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  updateButtonValues();
  sendSliderValues(); // Actually send data (all the time)
  //printSliderValues(); // For debug
  delay(10);
}

void updateButtonValues1(){
  for (int i =0; i < NUM_SLIDERS; i++){
    buttonValues[i] = digitalRead(buttonInputs[i]);
  }
}

void updateButtonValues(){
  for (int i =0; i < NUM_SLIDERS; i++){
    if ((millis() - buttonsLastUpdated[i]) > debounceDelay) {
      if (digitalRead(buttonInputs[i]) == 1){
        buttonValues[i] = !buttonValues[i];
        buttonsLastUpdated[i] = millis();
        ledStatus[i] = !ledStatus[i];
        digitalWrite(leds[i], ledStatus[i]);
      }
    }
  }
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
} 

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);
    builtString += String("|");
  }

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)buttonValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}