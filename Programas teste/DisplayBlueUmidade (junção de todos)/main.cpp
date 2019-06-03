#include "mbed.h"
#include "Adafruit_SSD1306.h"
 
Serial blu(p9, p10); // tx, rx

AnalogIn umidade_solo(p15); // Sensor de umidade: p15



 
// an SPI sub-class that provides a constructed default
class SPIPreInit : public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk)
    {
        format(8,3);
        frequency(2000000);
    };
};
 
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
 
 
int main()
{   
     
 
    gOled1.clearDisplay();                      //Limpa a tela 
    gOled1.setTextCursor(0,0);                  //Posiciona o cursor (x,y)     
    gOled1.printf("Teste\r\n");                 //Escreve
    gOled1.printf("Exibir dados aqui");         //Escreve        
    gOled1.display();                           //Exibe o que está no buffer
    
    while(1)
    {
       
        blu.printf("Umidade: %f\n", umidade_solo.read()); // Mergulhado na água = 0.39 ; Seco = 1.00
        
        gOled1.setTextCursor(0,0);
        gOled1.printf("\n\n\nUmidade: %f", umidade_solo.read());
        gOled1.display();
        
        wait(0.5);
        

    }
}


