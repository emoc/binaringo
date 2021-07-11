// ALGORITHME DE DIVISION BINAIRE STOCHASTIQUE ***************************************



void divvy(byte i, byte lo, byte hi) {
  byte mid;
  mid = (lo + hi) >> 1;
  
  patterns[i][lo] = 1;

  byte min_sub = MAXRES / instruments[0][i][3];
  
  if (   (byte(random(101)) < instruments[0][i][2]) 
      && ((hi - lo) > min_sub) ) {
    divvy(i, lo, mid);
    divvy(i, mid, hi);
  }
}

void resetPatterns() {
  if (DEBUG_TIMING) Serial.print(millis());
  if (DEBUG_TIMING) Serial.println(" resetPatterns start");
  for (byte i = 0; i < 8; i++) { // 8 = nombre d'instruments
    for (byte j = 0; j < 16; j++) { // 16 = nombre de pas
      patterns[i][j] = 0;
    }
  }
  
  // chercher les notes potentiellement jouées
  for (byte i = 0; i < 8; i++) { // 8 = nombre d'instruments
    divvy(i, 0, MAXRES);
  }

  if (DEBUG_TIMING) Serial.print(millis());
  if (DEBUG_TIMING) Serial.println(" resetPatterns stop");
  
  /*   
  // CETTE FONCTION FAIT TOUT PLANTER !
  // chercher les notes réellement jouées
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 16; j++) { 
      if (patterns[i][j] > 0) {
        if (i == 0) patterns[i][j] = 2; // première ligne toujours jouée
        else {
          byte k = i - 1;
          boolean note_au_dessus = false;
          while (k >= 0) {
            if (patterns[k][j] > 0) note_au_dessus = true;
            k--;
          }
          if (!note_au_dessus) patterns[i][j] = 2;
        }
      }
    }
  } */
}
/*
void preparePatternDisplay() {
  for (byte i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      if (patterns[i][j] == 0) patterns_display[i][j] = '.';
      if (patterns[i][j] == 1) patterns_display[i][j] = 'o';
      //if (patterns[i][j] == 2) patterns_display[i][j] = '#';
    }
  }
}*/

void printPattern() {
  for (byte i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      if (patterns[i][j] == 0) Serial.print(".");
      if (patterns[i][j] == 1) Serial.print("!");
      if (patterns[i][j] == 2) Serial.print("#");
    }
    Serial.println();
  }
  Serial.println();
}

