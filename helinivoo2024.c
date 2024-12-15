
const int micPin = A0;
const int sampleWindow = 50; // mitu millisekundit loeb andmeid
unsigned int sample;

void setup() {	// sätestab seriali & pinid 
  Serial.begin(9600);
  for (int i = 2; i <= 9; i++) { //ledid
    pinMode(i, OUTPUT);
  }
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow) { //signaali "lugemine"
    sample = analogRead(micPin);				  //analoogsignaal
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;  // leiab maksimaalse
      } else if (sample < signalMin) {
        signalMin = sample;  // leiab miinimumi
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max ja min vahe (täissignaal)

  //logaritmi kasutamine, kuna heliskaala on logaritmiline
  float logValue = log10(peakToPeak + 1) * 100;

  Serial.println(logValue); //konsooli printimine ("lõpptulemus")

  //kasutades map() funktsiooni, teame mitu LEDi peab süttima
  int ledCount = map(logValue, 0, log10(1024) * 100, 0, 9);

  for (int i = 2; i <= 9; i++) { //LEDidele (D2-D9) lülitus
    if (i - 2 < ledCount) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
  delay(50);	//oota 50ms
}

/*
Allikad/kaste:
	analogRead():
https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/
	millis():
https://reference.arduino.cc/reference/en/language/functions/time/millis/
https://forum.arduino.cc/t/millis-and-analogread-compatibility/65494/3
	log10()
https://forum.arduino.cc/t/maths-help-log/339211/2
	map()
https://docs.arduino.cc/language-reference/en/functions/math/map/
*/
