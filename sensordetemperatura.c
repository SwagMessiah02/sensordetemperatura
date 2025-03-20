#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define BTA 5
#define DEBOUNCE 50000

// Cabeçalho para as funções
void setup();
void exibir_celsius(float temperatura);
char* gcvt(double x, int ndigit, char *buf);

// Variáveis globais 
volatile void (*ptr_exibir)(float temperatura);
volatile uint contador = -1;
absolute_time_t ultima_iterrupcao;
ssd1306_t display;

// Função principal
int main()
{
    setup(); // Função de configuração

    uint16_t leitura;
    const float conversor = 3.3f / (1 << 12); 
    float votagem;
    float temperatura; 

    ptr_exibir = &exibir_celsius;

    while (true) {
        leitura = adc_read(); // Realiza a leitura do sinal analógico
        votagem = leitura * conversor; // Obtem a voltagem apartir da leitura anterior 
        temperatura = 27 - (votagem - 0.706) / 0.001721; // Realiza a conversão para temperatura em graus Celsius 

        (*ptr_exibir)(temperatura); // Ponteiro para a função de exibir temperatura
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

    // Configura o botão A como entrada 
    gpio_init(BTA);
    gpio_set_dir(BTA, GPIO_IN);
    gpio_pull_up(BTA);

    // Habilita uma iterrupção para quando o botão for pressionado
    gpio_set_irq_enabled_with_callback(BTA, GPIO_IRQ_EDGE_RISE, true, &trocar_escala);

    // Inicializa o módulo ADC
    adc_init(); 
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Inicializa o display OLED
    display.external_vcc = false;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&display);
}

// Função que exibe a temperatura em graus Celcius no display OLED 
void exibir_celsius(float temperatura) {
    char buffer[4];

    gcvt(temperatura, 4, buffer);

    ssd1306_draw_string(&display, 24, 24, 2, buffer);
    ssd1306_draw_string(&display, 82, 24, 2, " C");
    ssd1306_show(&display);
    sleep_ms(48); 
    ssd1306_clear(&display);
    ssd1306_show(&display);
}

// Função que exibe a temperatura em graus Farenheit no display OLED 
void exibir_farenheit(float temperatura) {
    char buffer[4];

    temperatura = ((temperatura * 1.8) + 32);
    gcvt(temperatura, 4, buffer);

    ssd1306_draw_string(&display, 24, 24, 2, buffer);
    ssd1306_draw_string(&display, 82, 24, 2, " F");
    ssd1306_show(&display);
    sleep_ms(48); 
    ssd1306_clear(&display);
    ssd1306_show(&display);
}

// Função que exibe a temperatura em graus kelvin no display OLED 
void exibir_kelvin(float temperatura) {
    char buffer[4];

    temperatura = temperatura + 273.15;
    gcvt(temperatura, 4, buffer);

    ssd1306_draw_string(&display, 24, 24, 2, buffer);
    ssd1306_draw_string(&display, 82, 24, 2, " K");
    ssd1306_show(&display);
    sleep_ms(48); 
    ssd1306_clear(&display);
    ssd1306_show(&display);
}

// Função de callback que troca a escala de temperatura quando o botão A é pressionado
void trocar_escala(uint gpio, uint32_t events) {
    contador = (contador + 1) % 3;

    absolute_time_t tempo_atual = get_absolute_time();

    switch (contador)
    {
    case 0:
        ssd1306_clear(&display);
        ssd1306_show(&display);

        if(absolute_time_diff_us(ultima_iterrupcao, tempo_atual) > DEBOUNCE) {
            botao_pressionado = true;
            ultima_iterrupcao = tempo_atual;
            ptr_exibir = &exibir_celsius;
        }

        break;
    case 1:
        ssd1306_clear(&display);
        ssd1306_show(&display);

        if(absolute_time_diff_us(ultima_iterrupcao, tempo_atual) > DEBOUNCE) {
            botao_pressionado = true;
            ultima_iterrupcao = tempo_atual;
            ptr_exibir = &exibir_farenheit;
        }

        break;
    case 2:
        ssd1306_clear(&display);
        ssd1306_show(&display);

        if(absolute_time_diff_us(ultima_iterrupcao, tempo_atual) > DEBOUNCE) {
            botao_pressionado = true;
            ultima_iterrupcao = tempo_atual;
            ptr_exibir = &exibir_kelvin;
        }

        break;
    default:
        break;
    }
}
