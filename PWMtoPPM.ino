
////////////////////////CONFIGURAÇÃO///////////////////////////////
#define NUMERO_DE_CANAIS 8  //define o número de canais
#define VALOR_INICIAL_PWM 1500  //define o valor padrão do PWM
#define DURACAO_DO_QUADRO_PPM 22500  //define o comprimento do quadro PPM em microssegundos (1ms = 1000µs)
#define DURACAO_DO_PULSO_PPM 300  //define o comprimento do pulso
#define POLARIDADE_DO_PULSO 1  //define a polaridade dos pulsos: 1 é positivo, 0 é negativo
#define PINO_DO_SINAL 10  //define o pino de saída do sinal PPM no arduino


int sinalPPM[NUMERO_DE_CANAIS];

ISR(TIMER1_COMPA_vect){  
  static boolean estado = true;
  
  TCNT1 = 0;
  
  if(estado) { 
    digitalWrite(PINO_DO_SINAL, POLARIDADE_DO_PULSO);
    OCR1A = DURACAO_DO_PULSO_PPM * 2;
    estado = false;
  }
  else{  
    static byte numeroDoCanalAtual;
    static unsigned int restoCalculado;
  
    digitalWrite(PINO_DO_SINAL, !POLARIDADE_DO_PULSO);
    estado = true;

    if(numeroDoCanalAtual >= NUMERO_DE_CANAIS){
      numeroDoCanalAtual = 0;
      restoCalculado = restoCalculado + DURACAO_DO_PULSO_PPM;
      OCR1A = (DURACAO_DO_QUADRO_PPM - restoCalculado) * 2;
      restoCalculado = 0;
    }
    else{
      OCR1A = (sinalPPM[numeroDoCanalAtual] - DURACAO_DO_PULSO_PPM) * 2;
      restoCalculado = restoCalculado + sinalPPM[numeroDoCanalAtual];
      numeroDoCanalAtual++;
    }     
  }
}

void setup(){  
  //inicializa os valores padrão do ppm para todos os canais
  for(int i=0; i<NUMERO_DE_CANAIS; i++){
    sinalPPM[i]= VALOR_INICIAL_PWM;
  }

  pinMode(PINO_DO_SINAL, OUTPUT);
  digitalWrite(PINO_DO_SINAL, !POLARIDADE_DO_PULSO);  //define o pino do sinal PPM para o estado padrão (desligado)
  
  cli();
  TCCR1A = 0; // define todo o registro TCCR1 para 0
  TCCR1B = 0;
  
  OCR1A = 100;  // registro de correspondência de comparação, altere isso
  TCCR1B |= (1 << WGM12);  // liga o modo CTC
  TCCR1B |= (1 << CS11);  // prescaler 8: 0,5 microssegundos a 16mhz
  TIMSK1 |= (1 << OCIE1A); // habilita a interrupção de comparação do temporizador
  sei();
}

void loop(){

  /*
  sinalPPM[Canal referente ao "Receptor"] = Valor PWM;

  Canal 1 = 0 | Roll
  Canal 2 = 1 | Pitch
  Canal 3 = 2 | Throttle
  Canal 4 = 3 | Yaw
  Canal 5 = 4 | Canal Livre
  Canal 6 = 5 | Canal Livre
  Canal 7 = 6 | Canal Livre
  Canal 8 = 7 | Canal Livre
  */
  sinalPPM[0] = 1800; // Sinal PWM
  delay(10); // Não tire o delay!!!
}


