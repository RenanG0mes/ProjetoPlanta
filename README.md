# ProjetoPlanta

-Descritivo:
    O projeto consiste em um smart vaso com sensores para monitoramento de presença, localização, umidade e temperatura para que com esses dados seja possível criar um sistema de irrigação automático. Além disso outros módulos também possibilitam a visualização dos dados coletados em um display oled ou para outra interface serial via bluetooth. Por último o servo motor serve para a movimentação de personagem temático. Todos os módulos são controlados pela placa de desenvolvimento NXP LCP1768

-Placa de desenvolvimento:
      NXP LCP1768 + mbed Application Board 

-Módulos utilizados:
      Sensor de umidade
      Display Oled
      Servo Motor
      Modulo PIR
      Ponte H
      BME280
      Modulo Bluetooth
      Bomba
      
Data de entrega: 18/06/2018



Um sensor será utilizado para verificar a umidade do solo, caso a umidade esteja muito baixa, um sistema de irrigação será acionado, este sistema será composto por um reservatório de água, bomba e ponte H. Caso seja necessário, também será possível ativar o sistema de irrigação via bluetooth.
Ao detectar a presença de movimento próxima ao vaso, um personagem temático escondido será revelado ao som de uma música sendo tocada pelo buzzer da mbed Application Board, além disso será exibida a temperatura, umidade, pressão do ambiente e humidade do solo em um display oled, estes dados também serão transmitidos via bluetooth.
