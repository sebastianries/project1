
// Configure Pins
int pin_distance_trigger=7; //pin_distance_trigger-Pin des Ultraschallsensors an Pin7 des Arduino-Boards 
int pin_distance_echo=6; // pin_distance_echo-Pim des Ultraschallsensors an Pin6 des Arduino-Boards
int pin_relais=8; //Relais-Trigger on Pin8 des Arduino-Boards

// define constants
int messintervall=10000; //Messintervall in Millisekunden
int mindistance=30; //Minimaldistanz zum Entfernungsmesser in cm

// define variables
long dauer=0; // Das Wort dauer ist jetzt eine Variable, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt. Startwert ist hier 0.
long entfernung=0; // Das Wort „entfernung“ ist jetzt die variable, unter der die berechnete Entfernung gespeichert wird. Info: Anstelle von „int“ steht hier vor den beiden Variablen „long“. Das hat den Vorteil, dass eine größere Zahl gespeichert werden kann. Nachteil: Die Variable benötigt mehr Platz im Speicher.


void setup()
{
  Serial.begin (9600); //Serielle kommunikation starten, damit man sich später die Werte am serial monitor ansehen kann.
  pinMode(pin_distance_trigger, OUTPUT); // pin_distance_trigger-Pin ist ein Ausgang
  pinMode(pin_distance_echo, INPUT); // pin_distance_echo-Pin ist ein Eingang
  pinMode(pin_relais, OUTPUT);
  digitalWrite(pin_relais, LOW); //disable pump on startup
}


void loop()
{
  digitalWrite(pin_distance_trigger, LOW); //Hier nimmt man die Spannung für kurze Zeit vom pin_distance_trigger-Pin, damit man später beim senden des pin_distance_trigger-Signals ein rauschfreies Signal hat.
  delay(5); //Dauer: 5 Millisekunden
  digitalWrite(pin_distance_trigger, HIGH); //Jetzt sendet man eine Ultraschallwelle los.
  delay(10); //Dieser „Ton“ erklingt für 10 Millisekunden.
  digitalWrite(pin_distance_trigger, LOW);//Dann wird der „Ton“ abgeschaltet.
  dauer = pulseIn(pin_distance_echo, HIGH); //Mit dem Befehl „pulseIn“ zählt der Mikrokontroller die Zeit in Mikrosekunden, bis der Schall zum Ultraschallsensor zurückkehrt.
  entfernung = (dauer/2) * 0.03432; //Nun berechnet man die Entfernung in Zentimetern. Man teilt zunächst die Zeit durch zwei (Weil man ja nur eine Strecke berechnen möchte und nicht die Strecke hin- und zurück). Den Wert multipliziert man mit der Schallgeschwindigkeit in der Einheit Zentimeter/Mikrosekunde und erhält dann den Wert in Zentimetern.
  if (entfernung >= 500 || entfernung <= 0) //Wenn die gemessene Entfernung über 500cm oder unter 0cm liegt,…
  {
    Serial.println("Kein Messwert"); //dann soll der serial monitor ausgeben „Kein Messwert“, weil Messwerte in diesen Bereichen falsch oder ungenau sind.
  }
  else //  Ansonsten…
  {
    Serial.print("Entfernung: ");
    Serial.print(entfernung); //…soll der Wert der Entfernung an den serial monitor hier ausgegeben werden.
    Serial.print(" cm - Pumpe "); // Hinter dem Wert der Entfernung soll auch am Serial Monitor die Einheit "cm" angegeben werden.
    if (entfernung < mindistance) 
    {
      Serial.println("AN");
      // starte Pumpe
      digitalWrite(pin_relais, HIGH);
    }
    else
    {
      Serial.println("AUS");
      // stoppe Pumpe
      digitalWrite(pin_relais, LOW);
    }
  }
  delay(messintervall); //Das delay von einer Sekunde sorgt in ca. jeder neuen Sekunde für einen neuen Messwert.
}

