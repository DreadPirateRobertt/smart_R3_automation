#include <Arduino.h>
#include <Wire.h>
#include <MicroLCD.h>

// Configuracao do Display LCD

LCD_SH1106 lcd;
//LCD_SSD1306 lcd;

#define pinVerde 2
#define pinAmarelo 3
#define pinVermelho 4
#define pinRelay 5 
#define rele_bomba 6
#define pinoSensorUmidade 8
#define rele_luz 7

//const int pinoSensorUmidade = A0; // Defina o pino do sensor de umidade


void semaforo(bool verde, bool amarelo, bool vermelho) {
    digitalWrite(pinVerde, verde ? HIGH : LOW);       // Liga ou desliga o LED verde
    digitalWrite(pinAmarelo, amarelo ? HIGH : LOW);   // Liga ou desliga o LED amarelo
    digitalWrite(pinVermelho, vermelho ? HIGH : LOW); // Liga ou desliga o LED vermelho
    delay(2000); // 2 segundos
}

void controle_luminosidade(){
    // sistema de luminosidade
    unsigned long tempoLigado = 18 * 60 * 60 * 1000; // 18 horas em milissegundos 
    unsigned long tempoDesligado = 6 * 60 * 60 * 1000; // 6 horas em milissegundos
    unsigned long tempoAtual = millis() % (tempoLigado + tempoDesligado);
    if (tempoAtual < tempoLigado) {
        digitalWrite(pinRelay, HIGH); // Liga o relé
    } else {
        digitalWrite(pinRelay, LOW); // Desliga o relé
    }
}

void sensor_umidade(){
    unsigned long horaAtual = controle_luminosidade(); // Obtém a hora atual usando a função controle_luminosidade

    int umidade = analogRead(pinoSensorUmidade); // Lê o valor da umidade do solo
    int porcentagem = map(umidade, 1023, 0, 0, 100); // valor analogico em porcentagem

    // Verifica os valores lidos e chama a função semaforo com os argumentos corretos
    if (porcentagem > 70) {
        semaforo(true, false, false); // Liga apenas o LED verde
        digitalWrite(rele_bomba, LOW);
    } else if (porcentagem >= 40 && umidade <= 70) {
        semaforo(false, true, false); // Liga apenas o LED amarelo
    } else {
        semaforo(false, false, true); // Liga apenas o LED vermelho
        //controle_irrigacao(true); // ligar sistema de irrigacao
        digitalWrite(rele_bomba, HIGH);
    }
    Serial.print(porcentagem);
    Serial.println("%");
    // Exibe a porcentagem da umidade do solo e o tempo atual no LCD
    display_ssd1306(porcentagem, horaAtual); // Passa a porcentagem e o tempo atual para a função display_ssd1306
    delay(1000); 

}

void display_ssd1306(int porcentagem, unsigned long tempoAtual){
    lcd.clear();
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.print("Umidade do Solo: ");
    lcd.print(porcentagem);
    lcd.print("Tempo Atual: ")
    lcd.print(tempoAtual);

}

void setup(){
    Serial.begin(9600);
    lcd.begin();
    pinMode(pinoSensorUmidade, INPUT);  // configura saída para o sensor de umidade
    pinMode(pinVerde, OUTPUT); // Define o pino do LED verde embutido como saída
    pinMode(pinAmarelo, OUTPUT); // Define o pino do LED amarelo embutido como saída
    pinMode(pinVermelho, OUTPUT); // Define o pino do LED vermelho embutido como saída
    pinMode(rele_bomba, OUTPUT); // configura saída para o sinal relé
    digitalWrite(rele_bomba, LOW);
    pinMode(rele_luz, OUTPUT); // configura saída para o sinal relé
    digitalWrite(rele_luz, LOW); 
}

//void real_time_temp_ds3231(){}

//void temp_umidade_do_ar_dht11(){}

void loop(){
    sensor_umidade()
    controle_luminosidade()
}
