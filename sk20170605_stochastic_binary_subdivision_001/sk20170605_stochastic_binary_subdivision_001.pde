/* 
    Algorithme "Stochastic Binary Subdivision" pour la création de motifs rythmiques MIDI
    D'après Peter S. Langston "Six Techniques for Algorithmic Music Composition" (1988)
    
    Pierre Commenge, pierre <> les porteslogiques.net, 6 juin 2017, Quimper
    processing 3.2.1, urzhiata (win xp sp2)
    
    Cette version (001) se contente de calculer les patterns, sans envoyer de notes    
    En pressant une touche, on recalcule les patterns de chaque instrument
*/

int[][] instruments = {
    {45, 75,  2,  0,  96, 1}, // BD
    {52, 75,  4,  0,  96, 1}, // SD
    {57, 50,  8,  0, 120, 1}, // HHC
    {51, 50,  8,  0,  64, 1}, // RIM
    {48, 67,  8,  0,  80, 1}, // TOM3
    {54, 70,  8,  0,  64, 1}, // CLAP
    {55, 75,  8,  0,  64, 1}, // COWB
    {59, 50, 16,  0,  80, 1}, // HHO
    {53, 67, 16,  0,  72, 1}  // TOM1    
};

String[] instruments_labels = {"BD", "SD", "HHC", "RIM", "TOM3", "CLAP", "COWB", "HHO", "TOM1"};

int MAXRES = 64; // plus petite subdivision d'une mesure
int Lolim = 0;   // note MIDI la plus basse
int Hilim = 127; // note MIDI la plus haute

int[][] patterns = new int[instruments.length][MAXRES];

boolean DEBUG = false;

void setup() {
  for (int i = 0; i < instruments.length; i++) {
    if (DEBUG) {
      println("Note MIDI :                         " + instruments[i][0]);
      println("Densité :                           " + instruments[i][1]);
      println("Plus petite subdivision autorisée : " + instruments[i][2]);
      println("Durée (en 1/64e de note)          : " + instruments[i][3]);
      println("Volume                            : " + instruments[i][4]);
      println("Canal MIDI                        : " + instruments[i][5]);
      println();
    }
    instruments[i][2] = MAXRES / instruments[i][2];
  }
}

void draw() {
  resetPatterns();
  
  // chercher les notes potentiellement jouées
  for (int i = 0; i < instruments.length; i++) {
    divvy(i, 0, MAXRES);
  }
    
  // chercher les notes réellement jouées
  for (int i = 0; i < patterns.length; i++) {
    for (int j = 0; j < patterns[i].length; j++) { 
      if (patterns[i][j] > 0) {
        if (i == 0) patterns[i][j] = 2; // première ligne toujours jouée
        else {
          int k = i - 1;
          boolean note_au_dessus = false;
          while (k >= 0) {
            if (patterns[k][j] > 0) note_au_dessus = true;
            k--;
          }
          if (!note_au_dessus) patterns[i][j] = 2;
        }
      }
    }
  }
  
  // afficher le pattern de chaque instrument
  println();
  for (int i = 0; i < patterns.length; i++) {
    printPattern(i);
  }
  noLoop();
}

void divvy(int i, int lo, int hi) {
  int mid;
  mid = (lo + hi) >> 1;

  if (DEBUG) {
    println("instrument : " + i + " / lo : " + lo + " / hi : " + hi + " / mid : " + mid + " / res : " + instruments[i][2] );
    printPattern(i);
  }
  
  patterns[i][lo] = 1;
    
  if (DEBUG) {
    printPattern(i);
    println();
  }
  if ( (int(random(101)) < instruments[i][1]) && ((hi - lo) > instruments[i][2]) ) {
    divvy(i, lo, mid);
    divvy(i, mid, hi);
  }
}

void resetPatterns() {
  for (int i = 0; i < patterns.length; i++) {
    for (int j = 0; j < patterns[i].length; j++) {
      patterns[i][j] = 0;
    }
  }
}

void printPattern(int i) {
  for (int j = 0; j < patterns[i].length; j++) {
    if (patterns[i][j] == 0) print(".");
    if (patterns[i][j] == 1) print("!");
    if (patterns[i][j] == 2) print("#");
  }
  print("  " + instruments_labels[i]);
  println();
}

void keyPressed() {
  loop();
}