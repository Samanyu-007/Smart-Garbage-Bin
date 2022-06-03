#include <LiquidCrystal.h>
#include <Servo.h>

Servo servo;
int servoPin = 6;
int trigPin = 8;
int echoPin = 7;

int pirPin = 9;

int redPin = 11;
int bluePin = 10;

long duration;
long distance;
float cm;
int val;

//LCD is set to read and write with the pins in order as
//Register select, enable, DB4, DB5, DB6, DB7
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);


void setup()
{
  Serial.begin(9600);
  //sets serial port for communication serial Port begin
  
  //Define inputs and outputs
  servo.attach(6);
  
  //for the ultrasonic sensor detecting level of garbage
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   
  //initiating RGB LED colors as output
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  pinMode(pirPin, INPUT);

  //LCD is set to 16x2 matrix mode
  lcd.begin(16, 2);
}

void loop()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse to make sure it is in low:
  digitalWrite(trigPin, LOW);
  delay(500);
  digitalWrite(trigPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into centimeter distance (cm)
  cm = (duration/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
  
  val = digitalRead(pirPin); //Sets the variable val to read the digital HIGH/LOW by the pir
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println(); //serial.println prints the command after it in the next line
  delay(1000);
  
  //When a person or object moves close to the bin, the servo controls the opening of the lid by rotating an angle of 90deg
  if (val==HIGH)
  {
    servo.write(90);
  }
  //When the object moves away or stops moving, the servo controls the closing of the lid
  else 
  {
    servo.write(0);
  }
  
  Serial.println(val); //Shows status of the positional sensor
  
  //for RGB LED to indicate fullness of an average bin of 80cm height
  if(cm>70 && cm<=80)
  {
    rgbColor(255,0); //Red signifies harmful a full bin
  }
  else if(cm>40 && cm<=70)
  {
    rgbColor(100,0); //Light Pink signifies a 75% filled bin
  }
  else if(cm>20 && cm<=40)
  {
    rgbColor(255,255); //Magenta signifies 50% fullness
  }
  else if(cm>5 && cm<=20)
  {
    rgbColor(50,255); //Purple signifies a 25% filled bin
  }
  else if(cm<5)
  {
    rgbColor(0,255); //Blue signifies an empty bin
  }
  
  //When the detected level of garbage is above 70cm, it is projected as full in the LCD
  if (cm>=70)
  {    
  	lcd.setCursor(0,0);
  	lcd.print("   This bin is");
    lcd.setCursor(2,7);
    lcd.print("    full");
  }
  //When the detected level of garbage is below 65cm, it is projected to have space LCD
  else if (cm<=65)
  {
    lcd.setCursor(0,0);
  	lcd.print("  This bin has");
    lcd.setCursor(2,7);           
    lcd.print("   space");
  } 
}

//The class sets RGB LED colors to indicate fullness levels of the bin
void rgbColor(int redValue, int blueValue)
{
  analogWrite(redPin, redValue);
  analogWrite(bluePin, blueValue);
}
