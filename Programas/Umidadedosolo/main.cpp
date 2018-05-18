#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx

AnalogIn umidade_solo(p15); // Sensor de umidade: p15

int main() {
    while(1) {
        
        pc.printf("Umidade: %f\n", umidade_solo.read()); // Mergulhado na água = 0.39 ; Seco = 1.00
        wait(0.5);
        
    }
}
