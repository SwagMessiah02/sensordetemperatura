#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

void setup();
void exibir(char *buffer);
char* gcvt(double x, int ndigit, char *buf);

ssd1306_t display;

int main()
{
    setup(); // Função de configuração

    char buffer[4];

    uint16_t leitura;
    const float conversor = 3.3f / (1 << 12); 
    float votagem;
    float temperatura; 

    while (true) {
        leitura = adc_read(); // Realiza a leitura do sinal analógico
        votagem = leitura * conversor; // Obtem a voltagem apartir da leitura anterior 
        temperatura = 27 - (votagem - 0.706) / 0.001721; // Realiza a conversão para temperatura em graus Celsius 

        gcvt(temperatura, 4, buffer); // Função para a conversão de float para string
        exibir(buffer); // Exibe o conteúdo do buffer no display OLED
    }
}

void setup() {
    stdio_init_all();

    // Atribui a frequência de funcionamento para 400 KHz
    i2c_init(I2C_PORT, 400 * 1000);

    // Inicializa os pinos GPIO 14 e GPIO 15 como, respectivamente, SDA e SCL
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o módulo ADC
    adc_init(); 
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Inicializa o display OLED
    display.external_vcc = false;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&display);
}

void exibir(char *buffer) {
    ssd1306_draw_string(&display, 24, 24, 2, buffer);
    ssd1306_draw_string(&display, 82, 24, 2, " C");
    ssd1306_show(&display);
    sleep_ms(48); 
    ssd1306_clear(&display);
    ssd1306_show(&display);
}
