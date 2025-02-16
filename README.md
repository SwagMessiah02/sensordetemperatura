
## <p align="center"> Sensor de Temperatura </p> 

O seguinte projeto implementa um programa embarcado capaz de medir a temperatura interna da CPU da placa Raspberry pi pico W, exibindo essas medicações no display OLED da placa. 

O código do programa foi escrito em linguagem C, utilizando a SDK oferecida pela Raspberry. O programa se utiliza do módulo ADC do microcontrolador RP2040 para realizar a conversão de sinais analógicos para digitais; também se faz uso do módulo i2C para realizar a comunicação entre o microcontrolador e o display OLED. 