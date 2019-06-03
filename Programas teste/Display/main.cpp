#include "mbed.h"
#include "Adafruit_SSD1306.h"
 

 
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
 
 
 
int main()
{   
    int contador = 0; 
 
    gOled1.clearDisplay();                      //Limpa a tela 
    gOled1.setTextCursor(0,0);                  //Posiciona o cursor (x,y)     
    gOled1.printf("Teste\r\n");                 //Escreve
    gOled1.printf("Exibir dados aqui");         //Escreve        
    gOled1.display();                           //Exibe o que está no buffer
    
    while(1)
    {
       
        
        gOled1.setTextCursor(0,0);
        gOled1.printf("\n\n\nContador: %d", contador);
        contador++;
        gOled1.display();
        
        wait(0.5);
        

    }
}


