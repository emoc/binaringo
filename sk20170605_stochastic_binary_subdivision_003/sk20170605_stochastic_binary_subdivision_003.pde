/* 
    Algorithme "Stochastic Binary Subdivision" pour la création de motifs rythmiques MIDI
    D'après Peter S. Langston "Six Techniques for Algorithmic Music Composition" (1988)
    
    Pierre Commenge, pierre <> les porteslogiques.net, 6 juin 2017, Quimper
    processing 3.2.1, urzhiata (win xp sp2)
    + themidibus v008 : http://www.smallbutdigital.com/projects/themidibus/
    + controlP5 v2.2.6
    
    v002 envoi d'un nouveau pattern toutes les {repeat_max} mesures
    Chaque temps des patterns est séparé par un delay de {time_between} millisecondes
    v003 : controles avec controlP5
    
    Note : la durée n'est pas utilisée
    Doute sur le calcul du BPM
    
    Clavier :
    s/S : sauvegarder les presets
    p/P : sauvegarder le pattern

*/

import themidibus.*; //Import the library
MidiBus myBus; // The MidiBus

import controlP5.*;
ControlP5 cp5;

import java.util.Map; // pour la listes des instruments

// fonctions de dates pour la sauvegarde de fichiers
import java.util.Date;
import java.text.SimpleDateFormat;
String SKETCH_NAME = getClass().getSimpleName();

int[][] instruments = {
    {36, 75,  2,  0, 127, 9}, // BD
    {38, 75,  4,  0,  96, 9}, // SD
    {42, 50,  8,  0, 120, 9}, // HHC
    {37, 50,  8,  0,  64, 9}, // RIM
    {45, 67,  8,  0,  80, 9}, // TOM3
    {39, 70,  8,  0,  64, 9}, // CLAP
    {56, 75,  8,  0,  64, 9}, // COWB
    {46, 50, 16,  0,  80, 9}, // HHO
    {48, 67, 16,  0,  72, 9}  // TOM1    
};

String[] instruments_labels = {"BD", "SD", "HHC", "RIM", "TOM3", "CLAP", "COWB", "HHO", "TOM1"};


String[] liste_instruments_midi = { "", "", "", "", "", "", "", "", "", "", "", "",
                                    "", "", "", "", "", "", "", "", "", "", "", "",
                                    "", "", "", "", "", "", "", "", "", "", "",
                                    "35 - Si2  * Grosse caisse acoustique ",
                                    "36 - Do3  * Grosse caisse 1",
                                    "37 - Do#3 * Side stick",
                                    "38 - Ré3  * Caisse claire Rock",
                                    "39 - Ré#3 * Clap de mains",
                                    "40 - Mi3  * Caisse claire",
                                    "41 - Fa3  * Tom basse - bas",
                                    "42 - Fa#3 * Charleston (amorti)",
                                    "43 - Sol3 * Tom basse - haut",
                                    "44 - Lab3 * Pédale Charleston",
                                    "45 - La3  * Tom basse",
                                    "46 - Sib3 * Charleston (ouvert)",
                                    "47 - Si3  * Tom médium - bas",
                                    "48 - Do4  * Tom médium - haut",
                                    "49 - Do#4 * Crash de cymbale - 1",
                                    "50 - Ré4  * Tom alto",
                                    "51 - Ré#4 * Cymbale ride - 1",
                                    "52 - Mi4  * Cymbale chinoise",
                                    "53 - Fa4  * Ride bell",
                                    "54 - Fa#4 * Tambourin",
                                    "55 - Sol4 * Cymbale splash",
                                    "56 - Lab4 * Cloche à vache",
                                    "57 - La4  * Crash de cymbale - 2",
                                    "58 - Sib4 * Vibraslap",
                                    "59 - Si4  * Cymbale Ride - 2",
                                    "60 - Do5  * Bongo alto",
                                    "61 - Do#5 * Bongo basse",
                                    "62 - Ré5  * Conga haut (amorti)",
                                    "63 - Ré#5 * Conga haut (ouvert)",
                                    "64 - Mi5  * Conga basse",
                                    "65 - Fa5  * Timbale (haute)",
                                    "66 - Fa#5 * Timbale (basse)",
                                    "67 - Sol5 * Agogo (haut)",
                                    "68 - Lab5 * Agogo (bas)",
                                    "69 - La5  * Cabasa",
                                    "70 - Sib5 * Maracas",
                                    "71 - Si5  * Petit sifflet",
                                    "72 - Do6  * Grand sifflet",
                                    "73 - Do#6 * Guiro (court)",
                                    "74 - Ré6  * Guiro (long)",
                                    "75 - Ré#6 * Claves",
                                    "76 - Mi6  * Woodblock (haut)",
                                    "77 - Fa6  * Woodblock (bas)",
                                    "78 - Fa#6 * Cuica (amorti)",
                                    "79 - Sol6 * Cuica (ouvert)",
                                    "80 - Lab6 * Triangle (amorti)",
                                    "81 - La6  * Triangle (ouvert)"
                                   };
                                  

int MAXRES = 64; // plus petite subdivision d'une mesure
int Lolim = 0;   // note MIDI la plus basse
int Hilim = 127; // note MIDI la plus haute

int[][] patterns = new int[instruments.length][MAXRES];
String string_patterns;

boolean DEBUG = false;

int beats_per_minute = 120;
int time_between = int(60000.0 / (((float)MAXRES / 4) * (float)beats_per_minute));
int pas = 0;
int repeat_max = 1;
int repeat = 0;

int next_beats_per_minute;
int next_repeat_max;

// pour les sliders
int inst0, inst1, inst2, inst3, inst4, inst5, inst6, inst7, inst8;
int inst0_vol, inst1_vol, inst2_vol, inst3_vol, inst4_vol, inst5_vol, inst6_vol, inst7_vol, inst8_vol;
int inst0_den, inst1_den, inst2_den, inst3_den, inst4_den, inst5_den, inst6_den, inst7_den, inst8_den;
int inst0_sub, inst1_sub, inst2_sub, inst3_sub, inst4_sub, inst5_sub, inst6_sub, inst7_sub, inst8_sub;

PFont info;
void setup() {
  
  size (900, 600);
  
  MidiBus.list();
  myBus = new MidiBus(this, -1, 13);
  
  cp5 = new ControlP5(this);
  createControls();
  
  info = loadFont("DejaVuSansMono-Bold-10.vlw");

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
  //myBus.sendMessage(0xC0, 10, instrument, 0); // Program Change, pour changer d'instrument (0-127)
  resetPatterns();
}

void draw() {
  
  background(0);
  
  textFont(info, 16);
  String tick = "";
  for (int i = 0; i < MAXRES; i++) {
    if (pas == i) tick += "*";
    else tick += " ";
  }
  fill(255);
  text(tick + "\n" + string_patterns, 200, 40);
  
  
  
  textFont(info, 10);
  text(liste_instruments_midi[inst0], 200, 310);
  text(liste_instruments_midi[inst1], 200, 340);
  text(liste_instruments_midi[inst2], 200, 370);
  text(liste_instruments_midi[inst3], 200, 400);
  text(liste_instruments_midi[inst4], 200, 430);
  text(liste_instruments_midi[inst5], 200, 460);
  text(liste_instruments_midi[inst6], 200, 490);
  text(liste_instruments_midi[inst7], 200, 520);
  text(liste_instruments_midi[inst8], 200, 550);
  
  //rect(40, 560, 820, 35);
  //fill(0);
  textFont(info, 10);
  text("Stochastic Binary Subdivision 003 / 6 juin 2017 / D'après Peter S. Langston", 40, 590);
  
  if (pas == MAXRES) {
    beats_per_minute = next_beats_per_minute;
    time_between = int(60000.0 / (((float)MAXRES / 4) * (float)beats_per_minute));
    pas = 0;
    
    instruments[0][0] = inst0; // récupérer la valeur du slider
    instruments[0][4] = inst0_vol; // récupérer la valeur du slider
    instruments[0][1] = inst0_den; // récupérer la valeur du slider
    instruments[0][2] = inst0_sub; // récupérer la valeur du slider
    instruments[0][2] = MAXRES / instruments[0][2];
    
    instruments[1][0] = inst1; // récupérer la valeur du slider
    instruments[1][4] = inst1_vol; // récupérer la valeur du slider
    instruments[1][1] = inst1_den; // récupérer la valeur du slider
    instruments[1][2] = inst1_sub; // récupérer la valeur du slider
    instruments[1][2] = MAXRES / instruments[1][2];
    
    instruments[2][0] = inst2; // récupérer la valeur du slider
    instruments[2][4] = inst2_vol; // récupérer la valeur du slider
    instruments[2][1] = inst2_den; // récupérer la valeur du slider
    instruments[2][2] = inst2_sub; // récupérer la valeur du slider
    instruments[2][2] = MAXRES / instruments[2][2];
    
    instruments[3][0] = inst3; // récupérer la valeur du slider
    instruments[3][4] = inst3_vol; // récupérer la valeur du slider
    instruments[3][1] = inst3_den; // récupérer la valeur du slider
    instruments[3][2] = inst3_sub; // récupérer la valeur du slider
    instruments[3][2] = MAXRES / instruments[3][2];
    
    instruments[4][0] = inst4; // récupérer la valeur du slider
    instruments[4][4] = inst4_vol; // récupérer la valeur du slider
    instruments[4][1] = inst4_den; // récupérer la valeur du slider
    instruments[4][2] = inst4_sub; // récupérer la valeur du slider
    instruments[4][2] = MAXRES / instruments[4][2];
    
    instruments[5][0] = inst5; // récupérer la valeur du slider
    instruments[5][4] = inst5_vol; // récupérer la valeur du slider
    instruments[5][1] = inst5_den; // récupérer la valeur du slider
    instruments[5][2] = inst5_sub; // récupérer la valeur du slider
    instruments[5][2] = MAXRES / instruments[5][2];
    
    instruments[6][0] = inst6; // récupérer la valeur du slider
    instruments[6][4] = inst6_vol; // récupérer la valeur du slider
    instruments[6][1] = inst6_den; // récupérer la valeur du slider
    instruments[6][2] = inst6_sub; // récupérer la valeur du slider
    instruments[6][2] = MAXRES / instruments[6][2];
    
    instruments[7][0] = inst7; // récupérer la valeur du slider
    instruments[7][4] = inst7_vol; // récupérer la valeur du slider
    instruments[7][1] = inst7_den; // récupérer la valeur du slider
    instruments[7][2] = inst7_sub; // récupérer la valeur du slider
    instruments[7][2] = MAXRES / instruments[7][2];
    
    instruments[8][0] = inst8; // récupérer la valeur du slider
    instruments[8][4] = inst8_vol; // récupérer la valeur du slider
    instruments[8][1] = inst8_den; // récupérer la valeur du slider
    instruments[8][2] = inst8_sub; // récupérer la valeur du slider
    instruments[8][2] = MAXRES / instruments[8][2];
    
    repeat ++;
    if (repeat == repeat_max) {
      repeat = 0;
      repeat_max = next_repeat_max;
      resetPatterns();
    }
  }
  
    
  for (int i = 0; i < instruments.length; i++) {
    if (patterns[i][pas] == 2) {
      myBus.sendNoteOn(instruments[i][5], instruments[i][0], instruments[i][4]); // Send a Midi noteOn
    }
  }
  
  delay(time_between);
  
  for (int i = 0; i < instruments.length; i++) {
    if (patterns[i][pas] == 2) {
      myBus.sendNoteOff(instruments[i][5], instruments[i][0], 0); // Send a Midi noteOff
    }
  }

  pas ++;
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
  if (DEBUG) {
    println();
    println("BPM : " + beats_per_minute + " / time_between : " + time_between);
    for (int i = 0; i < patterns.length; i++) {
      printPattern(i);
    }
  }
  
  string_patterns = buildStringPatterns();
  
  
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

String buildStringPatterns() {
  String sp = "";
  for (int i = 0; i < patterns.length; i++) {
    for (int j = 0; j < patterns[i].length; j++) {
      if (patterns[i][j] == 0) sp += ".";
      if (patterns[i][j] == 1) sp += "!";
      if (patterns[i][j] == 2) sp += "#";
    }
    sp += "\n";
  }
  return sp;
}

void delay(int time) {
  int current = millis();
  while (millis () < current+time) Thread.yield();
}