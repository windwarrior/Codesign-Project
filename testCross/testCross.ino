#define yellow1 3
#define yellow2 4
#define yellow0 9
#define blue1 5
#define blue2 6
#define red1 10
#define red2 11
#define green1 7
#define green2 8

void setup(){
  pinMode(yellow0, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(blue1, OUTPUT);
  pinMode(blue2, OUTPUT);
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
}

void loop(){
  digitalWrite(yellow0, HIGH);
  delay(200);
  
  digitalWrite(yellow1, HIGH);
  digitalWrite(green1, HIGH);
  digitalWrite(red1, HIGH);
  digitalWrite(blue1, HIGH);
  delay(200);
  
  digitalWrite(yellow1, LOW);
  digitalWrite(green1, LOW);
  digitalWrite(red1, LOW);
  digitalWrite(blue1, LOW);
  
  digitalWrite(yellow2, HIGH);
  digitalWrite(green2, HIGH);
  digitalWrite(red2, HIGH);
  digitalWrite(blue2, HIGH);
  delay(200);
  
  
  delay(200);
  digitalWrite(yellow2, LOW);
  
  delay(200);
  
  
  delay(200);
  digitalWrite(green2, LOW);
  
  delay(200);
  
  
  delay(200);
  digitalWrite(red2, LOW);
  
  delay(200);
  
  
  delay(200);
  digitalWrite(blue2, LOW);
}
  
  
