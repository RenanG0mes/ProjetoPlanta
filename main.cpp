#include "mbed.h"
#include "C12832.h"
#include "Servo.h"
#include "Adafruit_SSD1306.h"
#include "BME280.h"
#include "PinDetect.h"

// set BME280 I2C pin
#if defined(TARGET_LPC1768)
BME280 sensorBME(p28, p27); // SDA, SCL
#else
BME280 sensorBME(I2C_SDA, I2C_SCL);
#endif

// set LCD pins
//C12832 lcd(p5, p7, p6, p8, p11);

// set humidity analog pin
AnalogIn reservatory_range(p20); //A0

// set luminosity analog pin
AnalogIn luminosity_range(p19); //Circuit (I)

// set pins for Bluetooth communication
Serial device(p9,p10); //tx, rx

// set motorpump digital pin
DigitalOut motorPumpIn1(p29); //In1
DigitalOut motorPumpIn2(p30); //In2

// set motion sensor digital pin
PinDetect motionSensor(p15);

// set the servo motor pin
Servo servoMotor(p21);

// create a SPI class to pre stablish the SPI pins properties
class SPIPreInit : public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk)
    {
        format(8,3);
        frequency(2000000);
    };
};

// set the LCD SPI communication pins ---------- Uncomment to use the LCD SPI
SPIPreInit gSpi(p5,NC,p7);                      //(MOSI,MISO,CLK)
Adafruit_SSD1306_Spi gOled1(gSpi,p11,p12,p13);  //(SPI,DC,RST,CS)

/*
Conexões:
MOSI: p5
CLK: p7
DC: p11
RST: p12
CS: p13
*/

//Out Control
DigitalOut myLed1(LED1);
DigitalOut myLed2(LED2);
DigitalOut myLed3(LED3);


void MotorPump(void){
    motorPumpIn1 = 1;
    motorPumpIn2 = 0;
    wait(1);
    motorPumpIn1 = 0;
    motorPumpIn2 = 0;
    wait(1);
    motorPumpIn1 = 1;
    motorPumpIn2 = 0;
    wait(1);
    motorPumpIn1 = 0;
    motorPumpIn2 = 0;
}

void MotorOn(void){
    motorPumpIn1 = 1;
    motorPumpIn2 = 0;  
}

void MotorOff(void){
    motorPumpIn1 = 0;
    motorPumpIn2 = 0;  
}

void Menu(void){
    if (device.writable()){
    // send menu
        device.printf("\n-------------------Menu--------------------\r\n");
        device.printf("Receber menu: (m)\r\n");
        device.printf("Para receber informacoes: (i)\r\n");
        device.printf("Para irrigar (PUMP): (p) \r\n");
        device.printf("Para ligar a bomba: (b) \r\n"); 
        device.printf("Para desligar a bomba: (o)\r\n");
        device.printf("Para intensidade de luz: (l) \r\n");
        device.printf("Para subir o boneco: (s) \r\n");
        device.printf("Para descer o boneco: (d) \r\n");
        device.printf("---------------------------------------------\r\n");   
    }    
}

double ReadRes(void){

int reservMAX = 91;
int reservMIN = 81;

double reservatory = reservatory_range.read()*100;

double percent = (reservatory - reservMIN)*100/(reservMAX - reservMIN); 

return percent;
}

double ReadLum(void){
    
double lumMAX = 95;
double lumMIN = 67;

double luminosity = luminosity_range.read()*100;

double percent = (lumMAX - luminosity)*100/(lumMAX - lumMIN); 

return percent;
}

void LCDMenu(void){
    gOled1.clearDisplay(); 
    gOled1.setTextCursor(0,0);         
    gOled1.printf("-Conecte a: BT05.\r\n");                 
    gOled1.printf("-Digite 'm'.\r\n");                 
    gOled1.display();
}

void GetInfo(void){
    double perLum = ReadLum();
    double perRes = ReadRes();
    
    if (device.writable()){
        LCDMenu();
        device.printf("Luminosidade: %.2f (%)\r\n",perLum);
        device.printf("Humidade(Solo): %.2f (%)\r\n",perRes);
        device.printf("Humidade(Ar): %.2f (%)\r\n",sensorBME.getHumidity());
        device.printf("Pressao: %.2f (Pa)\r\n",sensorBME.getPressure());
        device.printf("Temperatura: %.2f (°C)\r\n",sensorBME.getTemperature());
    }else{
        LCDMenu();                
        gOled1.printf("Conecte o Bluetooth!\r\n");                 
        gOled1.display();  
    }   
    
    
}

void Rx_interrupt(void){

    if (device.readable()){
        char cond = device.getc();
        char buff1 = device.getc();
        char buff2 = device.getc();
        
        switch(cond) {
                case 'm':
                LCDMenu();
                gOled1.printf("Menu. \r\n"); 
                gOled1.display();
                Menu();
            break;
                case 'i': 
                LCDMenu();
                device.printf("---------------Info----------------\r\n");
                gOled1.printf("Info. \r\n");
                gOled1.display();
                GetInfo();
                device.printf("-----------------------------------\r\n");
            break;
                case 'p':
                device.printf("Pump no motor! \r\n");
                LCDMenu();
                gOled1.printf("PUMP. \r\n");
                gOled1.display();
                MotorPump();
            break;
                case 'o':
                device.printf("Desligou o motor! \r\n");
                LCDMenu();
                gOled1.printf("MotorOff. \r\n");
                gOled1.display();
                MotorOff();
            break;
                case 'b':
                LCDMenu();
                device.printf("Ligou o motor! \r\n"); 
                gOled1.printf("MotorOn. \r\n");
                gOled1.display();
                MotorOn();
            break;
                case 'l':
                LCDMenu(); 
                gOled1.printf("Luz. \r\n");
                gOled1.display();
                device.printf("Intensidade: %.2f \r\n",ReadLum());
            break;
                case 's':
                LCDMenu();
                device.printf("Sobe boneco! \r\n");
                gOled1.printf("Sobe Boneco. \r\n");
                gOled1.display();
                servoMotor.write(1);
            break;
                case 'd':
                LCDMenu();
                device.printf("Desce bonceo! \r\n");
                gOled1.printf("Desce Boneco. \r\n");
                gOled1.display();
                servoMotor.write(0);
            break;
                default:
                myLed1 = 1;
                myLed2 = 0;
                myLed3 = 1;
            break;
        }
        
    }else{    
        myLed1 = 0;
        myLed2 = 1;
        myLed3 = 0;
    }    
}

void Motion(void){
    servoMotor.write(1);
    GetInfo(); 
    wait(2);
    servoMotor.write(0);        
}

int main() {
    
    // configure motion sensor pin
    motionSensor.mode(PullUp);
    motionSensor.setSampleFrequency();
    
    // servo calibration -- 180 degrees movment
    servoMotor.calibrate(0.001, 180.0); 
    
    // initialize the bluetooth communication baud rate
    device.baud(9600);
 
    // serial interrupt to receive data
    device.attach(&Rx_interrupt, Serial::RxIrq);
    
    // set motion interrupt
    motionSensor.attach_asserted(&Motion);
    // serial interrupt to transmit data
    //device.attach(&Tx_interrupt, Serial::TxIrq);
    
    
    // Initialize LCD
    LCDMenu();
    servoMotor.write(0);
    wait(5);
    
    while(1) {       
        
        if(ReadRes() < 0.3){
            MotorPump();        
        }
        
        wait(20);
        
    }
}