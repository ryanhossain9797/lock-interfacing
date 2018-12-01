//Progress = Hardware Ok, PIN Basic OK

#include <string>
#include "mbed.h"
#include "TextLCD.h"
#include "Keypad.h"
#define PWM_PERIOD_SERVO_MS  20

PwmOut servo(PA_13);
DigitalOut buzzer(D13);
TextLCD lcd(PC_3, PC_2, A2, A3, A4, A5); // rs, e, d4-d7


//COLUMNS 1 - 4, ROWS 1 - 4
Keypad kpad(D2, D3, D4, D5, D8, D9, D10, D11);

//COLUMNS REVERSED
//Keypad kpad(PC_3, PC_2, PF_1, PF_0, PB_7, PC_13, PC_14, PC_15);

//ROWS REVERSED
//Keypad kpad(PF_0, PF_1, PC_2, PC_3, PC_15, PC_14, PC_13, PB_7);

//FULL REVERSE
//Keypad kpad(PC_3, PC_2, PF_1, PF_0, PC_15, PC_14, PC_13, PB_7 );

string PIN = "1234";

Serial pc(USBTX,USBRX);
int released = 1;
char key;

void beep(float time){
    buzzer = 1;
    wait(time);
    buzzer = 0;
}

char readChar(){
    char newKey;
    while(1){
        newKey = kpad.ReadKey();
        if(newKey == '\0'){
                released = 1; //set the flag when all keys are released  
        }                    
               
        if((newKey != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
            pc.printf("%c\n", newKey);
            beep(0.05);            
            released = 0;                       //clear the flag to indicate that key is still pressed
            return newKey;
        }
    }
}

string readPin(){
    int charCount = 0;
    string input = "";
    string encrypt = "";
    char digit;
    while(charCount<4){
        digit = readChar();
        if(digit>='0' && digit <='9'){
            input += digit;
            encrypt += '*';
            charCount++;
            lcd.cls();
            lcd.printf("%s",encrypt);
        }
        if(digit == 'C'){
            if(input.length()>0){
                input = input.substr(0,input.length()-1);
                encrypt = encrypt.substr(0,encrypt.length()-1);
                charCount--;
                lcd.cls();
                lcd.printf("%s",encrypt);    
            }
        }
    }
    return input;
}

void unlock(){
    lcd.cls();
    lcd.printf("LOCKED\nEnter Pin");
    string pinIn = readPin();
        
    wait(0.2);
    if(PIN.compare(pinIn)==0){
        beep(0.1);
        wait(0.1);
        beep(0.1);
        lcd.cls();
        lcd.printf("Unlocked\nPress B to Lock");
        servo.pulsewidth_us(10);
        wait(1.0);    
    }
    else{
        beep(1.0);
        lcd.cls();
        lcd.printf("Wrong\nPress B to Exit");
                
    }
    while(1){
        char Pressed= readChar();                   
    
        if(Pressed=='B'){
            beep(0.1);
            servo.pulsewidth_us(1200);
            break;
        }
    }
}

void changePin(){
    lcd.cls();
    lcd.printf("Enter Current\nPIN");
    string pinIn = readPin();
    if(PIN.compare(pinIn)==0){
        beep(0.1);
        wait(0.1);
        beep(0.1);
        lcd.cls();
        lcd.printf("PIN Matched\nEnter New PIN");
        PIN = readPin();
        lcd.cls();
        lcd.printf("Succees\nReturning");
        wait(1.0);    
    }
    else{
        lcd.cls();
        lcd.printf("Wrong PIN\nReturning");
        beep(1.0);
        wait(1.0);
    }    
}

int main() {
    servo.period_ms(PWM_PERIOD_SERVO_MS);
    servo.pulsewidth_us(1200);
    wait(1.0);
    pc.printf("Serial OK");
    
    while(1){
        lcd.cls();
        lcd.printf("A -> Unlock\nD -> Change Pin");
        char select = readChar();
        if(select == 'A'){
            unlock();
        }
        if(select=='D'){
            changePin();
        }
    }
}

/*
BACKUP
int main() {
    //towerProSG90.period_ms(PWM_PERIOD_FOR_SG90_IN_MS);
    //towerProSG90.pulsewidth_us(1500);
    //wait(1.0);
    //towerProSG90.pulsewidth_us(10);
    char key;
    int released = 1;
    lcd.printf("LCD OK");
    wait(1.0);
    pc.printf("Serial OK");
    int chars = 0;
    while(1){
        key = kpad.ReadKey();                   //read the current key pressed

        if(key == '\0'){
            //led=0;
            released = 1;   
        }                    //set the flag when all keys are released
           
        if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
            //led = 1;
            lcd.cls();
            lcd.printf("%c\n", key);
            pc.printf("%c\n", key);            
            released = 0;                       //clear the flag to indicate that key is still pressed
        }
    }
}
*/




/*
BACKUP 2
SERVO, LCD, KEYPAD OK. CODE BASIC OK
#include <string>
#include "mbed.h"
#include "TextLCD.h"
#include "Keypad.h"
#define PWM_PERIOD_SERVO_MS  20

PwmOut servo(PA_13);
TextLCD lcd(PC_3, PC_2, A2, A3, A4, A5); // rs, e, d4-d7


//COLUMNS 1 - 4, ROWS 1 - 4
Keypad kpad(D2, D3, D4, D5, D8, D9, D10, D11);

//COLUMNS REVERSED
//Keypad kpad(PC_3, PC_2, PF_1, PF_0, PB_7, PC_13, PC_14, PC_15);

//ROWS REVERSED
//Keypad kpad(PF_0, PF_1, PC_2, PC_3, PC_15, PC_14, PC_13, PB_7);

//FULL REVERSE
//Keypad kpad(PC_3, PC_2, PF_1, PF_0, PC_15, PC_14, PC_13, PB_7 );

string PIN = "1234";

Serial pc(USBTX,USBRX);

int main() {
    servo.period_ms(PWM_PERIOD_SERVO_MS);
    servo.pulsewidth_us(1200);
    //buzzer.beep(2000,2.0);
    wait(1.0);
    
    char key;
    int released = 1;
    lcd.printf("LOCKED\nEnter Pin");
    wait(1.0);
    pc.printf("Serial OK");
    while(1){
        int chars = 0;
        string input = "";
        while(chars<4){
            key = kpad.ReadKey();                   //read the current key pressed
    
            if(key == '\0'){
                //led=0;
                released = 1;   
            }                    //set the flag when all keys are released
               
            if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
                //led = 1;
                lcd.cls();
                pc.printf("%c\n", key);
                input += key;
                lcd.printf("%s", input);
                chars++;            
                released = 0;                       //clear the flag to indicate that key is still pressed
            }
        }
        lcd.cls();
        if(PIN.compare(input)==0){
            lcd.printf("Unlocked\nPress B to Lock",input);
            servo.pulsewidth_us(10);
            wait(1.0);    
        }
        else{
            lcd.printf("Wrong\nPress B");    
        }
        while(1){
            key = kpad.ReadKey();                   //read the current key pressed
    
            if(key == '\0'){
                released = 1;   
            }                    //set the flag when all keys are released
               
            if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
                if(key=='B'){        
                    released = 0;                       //clear the flag to indicate that key is still pressed
                    lcd.cls();
                    lcd.printf("LOCKED\nEnter Pin");
                    servo.pulsewidth_us(1200);
                    break;
                }
            }
        }
    }
}*/


/*BACKUP 3

Progress = Hardware Ok, PIN Basic OK

#include <string>
#include "mbed.h"
#include "TextLCD.h"
#include "Keypad.h"
#define PWM_PERIOD_SERVO_MS  20

PwmOut servo(PA_13);
DigitalOut buzzer(D13);
TextLCD lcd(PC_3, PC_2, A2, A3, A4, A5); // rs, e, d4-d7


//COLUMNS 1 - 4, ROWS 1 - 4
Keypad kpad(D2, D3, D4, D5, D8, D9, D10, D11);

//COLUMNS REVERSED
//Keypad kpad(PC_3, PC_2, PF_1, PF_0, PB_7, PC_13, PC_14, PC_15);

//ROWS REVERSED
//Keypad kpad(PF_0, PF_1, PC_2, PC_3, PC_15, PC_14, PC_13, PB_7);

//FULL REVERSE
//Keypad kpad(PC_3, PC_2, PF_1, PF_0, PC_15, PC_14, PC_13, PB_7 );

string PIN = "1234";

Serial pc(USBTX,USBRX);

void beep(float time){
    buzzer = 1;
    wait(time);
    buzzer = 0;
}

int main() {
    servo.period_ms(PWM_PERIOD_SERVO_MS);
    servo.pulsewidth_us(1200);
    //buzzer.beep(2000,2.0);
    wait(1.0);
    
    char key;
    int released = 1;
    lcd.printf("LOCKED\nEnter Pin");
    wait(1.0);
    pc.printf("Serial OK");
    while(1){
        int chars = 0;
        string input = "";
        string encrypt = "";
        while(chars<4){
            key = kpad.ReadKey();                   //read the current key pressed
    
            if(key == '\0'){
                //led=0;
                released = 1;   
            }                    //set the flag when all keys are released
               
            if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
                lcd.cls();
                pc.printf("%c\n", key);
                input += key;
                encrypt+= '*';
                lcd.printf("%s", encrypt);
                chars++;
                beep(0.05);            
                released = 0;                       //clear the flag to indicate that key is still pressed
            }
        }
        wait(0.2);
        if(PIN.compare(input)==0){
            beep(0.1);
            wait(0.1);
            beep(0.1);
            lcd.cls();
            lcd.printf("Unlocked\nPress B to Lock",input);
            servo.pulsewidth_us(10);
            wait(1.0);    
        }
        else{
            beep(1.0);
            lcd.cls();
            lcd.printf("Wrong\nPress B");
                
        }
        while(1){
            key = kpad.ReadKey();                   //read the current key pressed
    
            if(key == '\0'){
                released = 1;   
            }                    //set the flag when all keys are released
               
            if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
                if(key=='B'){
                    beep(0.1);        
                    released = 0;                       //clear the flag to indicate that key is still pressed
                    lcd.cls();
                    lcd.printf("LOCKED\nEnter Pin");
                    servo.pulsewidth_us(1200);
                    break;
                }
            }
        }
    }
}
*/
