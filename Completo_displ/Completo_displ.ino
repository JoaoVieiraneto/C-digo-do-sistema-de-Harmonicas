#include <arduinoFFT.h> //Biblioteca utilizada para aplicar a transformdade de Fourier
#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 

#define Sensor A2
#define Amostras 128 // Número de amostras
#define Freq_Amostragem 7750// Frequência de amostragem

#define col 20 // Serve para definir o numero de colunas do display utilizado
#define lin  4 // Serve para definir o numero de linhas do display utilizado
#define ende  0x3F // Serve para definir o endereço do display.

LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C

double Part_Real[Amostras];
double Part_Img[Amostras];

arduinoFFT FFT = arduinoFFT();

void setup() {
  pinMode(Sensor, INPUT); //Pino de entrada
  Serial.begin(9600);
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear(); // Serve para limpar a tela do display
}

void loop() {
  for (int i = 0; i < Amostras; i++) { //Processo de amostragem do sinal
    Part_Real[i] = analogRead(Sensor);
    Part_Img[i] = 0;
    delayMicroseconds(10); // Aumente o atraso para garantir uma leitura estável
  }

  FFT.Windowing(Part_Real, Amostras, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(Part_Real, Part_Img, Amostras, FFT_FORWARD);  //Realizando a aplicação da transformada de Fourier
  FFT.ComplexToMagnitude(Part_Real, Part_Img, Amostras);
  double Pico = FFT.MajorPeak(Part_Real, Amostras, Freq_Amostragem); //Encontrando a frequencia fundamental

  for (int i = 1; i <= 5; i++){
    double Freq_Harmonicas = Pico * i;  // Encontra as harmonicas
    int index_harmonica = floor((Freq_Harmonicas * Amostras) / Freq_Amostragem); //Encontra as amplitudes das harmonicas
    double Ampl_Harmonicas = Part_Real[index_harmonica]; 
    
    
    lcd.setCursor(0,0); // Coloca o cursor do display na coluna 1 e linha 1
    lcd.print("Harmonica: "); // Comando de saída com a mensagem que deve aparecer na coluna 2 e linha 1.
    lcd.print(i);
    lcd.setCursor(0, 1); //Coloca o cursor do display na coluna 1 e linha 2
    lcd.print("Freq: ");  // Comando de saida com a mensagem que deve aparecer na coluna 2 e linha 2
    lcd.print(Freq_Harmonicas);
    lcd.print("Hz");
    lcd.setCursor(0,2); //Coloca o cursor do display na coluna 1 e linha 1
    
    lcd.print("Amplitude: ");  // Comando de saida com a mensagem que deve aparecer na coluna 2 e linha 3
    lcd.print(Ampl_Harmonicas);
    lcd.setCursor(0 ,3); //Coloca o cursor do display na coluna 1 e linha 2
    


    Serial.print("Harmonica ");   //Imprime as harmonicas, com suas frequencias e amplitudes
    Serial.print(i);
    Serial.print("->");
    Serial.print(" Frequencia:");
    Serial.print(Freq_Harmonicas);
    Serial.print(" Hz Amplitude: ");
    Serial.println(Ampl_Harmonicas);
    delay(2000);
    lcd.clear();
  }
  
  Serial.println("_______________________________________________");
}
