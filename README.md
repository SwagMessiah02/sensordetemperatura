
## <p align="center"> Sensor de Temperatura </p> 

O seguinte projeto implementa um programa embarcado capaz de medir a temperatura interna da CPU da placa Raspberry pi pico W, exibindo essas medicações no display OLED da placa. 

O código do programa foi escrito em linguagem C, utilizando a SDK fornecida pela Raspberry. O programa se utiliza do módulo ADC do microcontrolador RP2040 para realizar a conversão de sinais analógicos para digitais; também se faz uso do módulo i2C para realizar a comunicação entre o microcontrolador e o display OLED. 

## <p align="center"> Melhorias </p> 

O programa foi atualizado para exibir a temperatura interna do microcontrolador RP2040 em outras escalas térmicas - como Farenheit e Kelvin. Agora a troca de escala pode ser realizada ao pressionar o botão A da placa BitDog Lab. Essa alteração permite a melhor compreenção do comportamento térmico do microcontrolador ao permitir a visualização da temperatura em outras escalas termométricas.

