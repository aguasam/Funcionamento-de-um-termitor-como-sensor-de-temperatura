#include <LiquidCrystal.h>

// Configuração dos pinos do display LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Número de pontos para a interpolação
const int N = 101;

// Vetores contendo os valores para interpolação: x_vals (temperaturas) e y_vals (tensões correspondentes)
float x_vals[N] = {0, 1, 2, 3, 4, 5, /* ... valores omitidos para brevidade ... */, 98, 99, 100};
float y_vals[N] = {4.7768, 4.7661, 4.7549, /* ... valores omitidos para brevidade ... */, 1.5862, 1.5533, 1.5229};

// Pino do LED para indicação (não usado atualmente, mas configurado)
const int ledPin = 9;

void setup() {
  // Inicializa a comunicação serial para depuração
  Serial.begin(9600);

  // Inicializa o display LCD com dimensões 16x2
  lcd.begin(16, 2);

  // Configura o pino do LED como saída (não utilizado no código principal)
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Lê o valor analógico do pino A0 (sensor conectado, range 0-1023)
  int sensorValue = analogRead(A0);

  // Converte a leitura para tensão real (0-5V)
  float Vout = sensorValue * (5.0 / 1023.0);

  // Calcula a temperatura com base na interpolação usando a tensão medida
  float Temp = interpolar(Vout);

  // Limpa o display LCD antes de atualizar os valores
  lcd.clear();
  
  // Exibe o texto "Temperatura: " na primeira linha do LCD
  lcd.setCursor(1, 0);
  lcd.print("Temperatura: ");

  // Exibe a temperatura calculada na segunda linha do LCD
  lcd.setCursor(1, 1);
  lcd.print(Temp);
  
  // Adiciona o símbolo de graus Celsius após a temperatura
  lcd.setCursor(7, 1);
  lcd.print("C");

  // Envia a temperatura calculada para o Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(Temp);
  Serial.println(" C");

  // Pausa de 100 ms antes da próxima leitura
  delay(100);
}

/**
 * Função de interpolação linear para estimar a temperatura com base em uma tensão fornecida.
 * @param x Tensão medida (Vout)
 * @return Temperatura interpolada correspondente
 */
float interpolar(float x) {
  // Percorre os pontos de interpolação
  for (int i = 0; i < N - 1; i++) {
    // Verifica se a tensão está no intervalo entre y_vals[i+1] e y_vals[i]
    if (x >= y_vals[i+1] && x <= y_vals[i]) {
      // Realiza interpolação linear para estimar a temperatura
      return x_vals[i] + (x_vals[i + 1] - x_vals[i]) * ((x - y_vals[i+1]) / (y_vals[i] - y_vals[i+1]));
    }
  }
  // Retorna 0 se a tensão estiver fora do intervalo definido
  return 0;
}
