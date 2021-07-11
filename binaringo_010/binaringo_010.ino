
/* Stochastic Binary Subdivision
    Version dure
    
    9/15 juin 2017
    010 : 26 juillet 2017, Orléans, Open Ateliers Version Longue
    arduino nano original / arduino 1.8.2 / urzhiata (windows XP SP2)
    
    + arduino midi library 4.3.1 / https://github.com/FortySevenEffects/arduino_midi_library
    + Rotary encoder handler for arduino. v1.1 / https://github.com/buxtronix/arduino/tree/master/libraries/Rotary
    + U8g2 2.14.7 / https://github.com/olikraus/u8g2
    + OneButton 1.2.0 / https://github.com/mathertel/OneButton

    VERSIONS
    004 : u8g2, interface fonctionnelle, MIDI non testé
    005 : passage à u8x8 
    007 : version fonctionnelle
    008 : version fonctionnelle, refonte avec nouvelles fonctions : swing, fixed on/off, plusieurs "views"
    009 : à chaque instrument est associé une banque! refonte du tableau instruments + sauvegardes EEPROM
    010 : version pour Yamaha QY70

    Notes :
    C'est l'affichage qui est vraiment long
    3 "familles" de réglages : param, glob, map

    Notes QY70 :
    sur cet expandeur, un canal de percussion peut-être assigné à n'importe quel canal
    il faut donc ABSOLUMENT envoyer une NOTE OFF pour chaque son déclenché

    TODO : 
      - 004 OK passer un maximum de valeurs en byte (ça pose un problème pour BPM : max 255 ?)
      - BOF ajouter un bouton de RESET (directement relié à arduino)
      - 004 OK, A TESTER! ajouter un évènement MIDI "ALL NOTES OFF" relié à un long press de bouton (bouton instrument ?)
      - 004 OK ajouter un évènement PLAY/STOP (bouton BPM ?)
      - 004 OK ajouter un bouton pour changer le nombre de répétitions
      - OUI! peut on méler changement de l'affichage et lancement de notes midi ?
      - 005 OK passer l'affichage à u8x8, c'est plus rapide!
      - se passer de la librairie MIDI
      - NON, ça ne change rien à la mémoire // enregistrer les patterns en boolean
      - NON, ça ne colle pas avec l'algo // calculer le prochain pattern au fur et à mesure de la progression
      - 006 OK prévoir un montage de test pour envoi MIDI (et réception sur polipus)
      - 008 OK et le swing, nom d'une pipe!
      - ??? souci quand le bpm est bas à cause du delay "time_between"
      - 004 OK ajouter un évènement pour afficher le rythme
      - 009 OK régler le delay entre les frames aux petits oignons
      - 009 OK régler le problème d'affichage quand il n'y a pas 3 chiffres...
      - ne réécrire que ce qui a changé
      - ce serait mieux de rajouter un inverseur dans le circuit cf. https://www.midi.org/articles/arduino-midi-output-basics
      - 008 OK, supprimée La durée des notes n'est pas utilisée...
      - 009 OK sauvegardes presets
      - 008 OK choisir les notes en fonction du MGS64 : ajouter bank
      - 008 OK ajouter bouton view, pour changer entre les 3 vues : rien, params, rythme
      - OK, il permet d'avoir des BPM + swing assez délirant //  TESTER LE DELAY MIN SANS VIEW
      - NON, pas nécessaire calculer le pattern à venir dans un buffer au fur et à mesure
      - ajouter un MIDI thru / alimentation autonome
      - faire un rafraichissement vertical de la vue "rythme"
      - 009 OK utiliser const
      - utiliser IFDEF pour le debug
      - réduire le tableau instruments[][][] à [][] (car il a causé tant de misères)
      - renommer "map" en "set"
      - on peut tout planter en changeant de preset trop vite, il passe alors à 255 et est enregistré en EEPROM...
      - 010 OK computeTimeBetween n'est recalculé qu'en changeant le BPM
      - le nombre de répétitions n'est pas enregistré correctement, ou il est mal chargé
      

    BOUTONS | CLIC                                         | LONG PRESS

    PARAM   | changer de paramètres                        | fixer le pattern
    SET     | changer de preset                            | enregistrer le preset
    GLOB    | changer REPEAT, SWING, BPM                   | play ON/OFF
    INST    | changer d'instrument                         | message MIDI "ALL NOTES OFF"
    VIEW    | changer de vue : PARAMS, RHYTHME, TOUT, DEBUG| vue ON/OFF

    004 :
    Le croquis utilise 15724 octets (51%) de l'espace de stockage de programmes. 
    Le maximum est de 30720 octets.
    Les variables globales utilisent 1481 octets (72%) de mémoire dynamique, 
    ce qui laisse 567 octets pour les variables locales. Le maximum est de 2048 octets.

    005 avec u8x8
    Le croquis utilise 13286 octets (43%) de l'espace de stockage de programmes. 
    Le maximum est de 30720 octets.
    Les variables globales utilisent 1309 octets (63%) de mémoire dynamique, 
    ce qui laisse 739 octets pour les variables locales. Le maximum est de 2048 octets.

    009 
    Le croquis utilise 16808 octets (54%) de l'espace de stockage de programmes. 
    Le maximum est de 30720 octets.
    Les variables globales utilisent 1351 octets (65%) de mémoire dynamique, 
    ce qui laisse 697 octets pour les variables locales. Le maximum est de 2048 octets.
    
    TESTS (003) :
    avec 8 instruments, 32 pas dans chaque pattern :
    une boucle sans affichage prend environ 1 à 2 ms
    un reset de patterns sans algo : 47 ms
    un affichage : 180 ms

    TESTS (003) :
    avec 8 instruments, 16 pas dans chaque pattern, un delay de 46 ms :
    un reset de patterns avec algo (comprenant delay) : 110 ms
    un affichage : pas mesuré

    TESTS (007) : 
    avec l'affichage, on arrive à un temps de boucle de 140 ms max, soit pas possible de dépasser les 107 BPM (sans swing!)

    TIMING :
    145 ms pour l'écriture de 42 bytes dans l'EEPROM
    100 ms pour la lecture de 42 bytes depuis l'EEPROM
    5 ms : pour un resetPattern() complet;
*/


// pour u8x8
#include <Arduino.h>
#include <U8x8lib.h>

#include <Rotary.h>
#include <MIDI.h>
#include <EEPROM.h>
#include <OneButton.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 

MIDI_CREATE_DEFAULT_INSTANCE();

/* 
   instruments[preset_max][inst_max][param_max]

   Banque           : instruments[preset_selected][inst_selected][0]
   Note MIDI        : instruments[preset_selected][inst_selected][1]
   Densité          : instruments[preset_selected][inst_selected][2]
   Subdivision min. : instruments[preset_selected][inst_selected][3]
   Volume           : instruments[preset_selected][inst_selected][4]

*/

boolean DEBUG = false;
boolean DEBUG_TIMING = false;
boolean CREATE_EEPROM = false; // à ne faire qu'une fois ou pour effacer la mémoire

// tous les instruments du preset en cours
byte instruments[1][8][5] = {
  {
    {0, 36, 75,  2,  127}, // BD
    {0, 38, 75,  4,   96}, // SD
    {0, 42, 50,  8,  120}, // HHC
    {0, 37, 50,  8,   64}, // RIM
    {0, 45, 67,  8,   80}, // TOM3
    {0, 39, 70,  8,   64}, // CLAP
    {0, 56, 75,  8,   64}, // COWB
    {0, 46, 50, 16,   80}, // HHO 
  }
};

// toutes les banques d'instrument
// pour le Roland M-GS64
/*
const byte banks[] = {0, 1, 8, 16, 24, 25, 26, 32, 40, 48, 49, 50, 56, 57};
const byte bank_max = 14;
*/
// pour le Yamaha QY70
const byte banks[] = {0, 1, 2, 3, 8,  9, 16, 17, 24, 25, 26, 27, 28, 29, 32, 33, 40, 48};
const byte bank_max = 18;

const byte MAXRES = 16;
byte patterns[8][16]; 
byte pas = 0;
byte repeat_max = 1;
byte repeat = 0;
boolean play = true;
//byte bank;
byte glob_selected = 0; // 0 : bpm, 1 : repeat, 2 : swing
const byte glob_max = 3;
byte preset_selected = 0;
const byte preset_max = 24;
byte param_selected = 0;
const byte param_max = 5;
byte inst_selected = 0;
const byte inst_max = 8;
byte map_selected = 0; // 0 : preset
const byte map_max = 1;
byte bpm = 140;
byte swing = 50;
byte view = 0;
const byte view_max = 4;


OneButton    button_map(4, true);
OneButton  button_param(5, true);
OneButton   button_glob(6, true);
OneButton   button_inst(7, true);
OneButton   button_view(8, true);

char button_map_state[]    = "--";
char button_param_state[]  = "--";
char button_glob_state[]   = "--";
char button_inst_state[]   = "--";
char button_view_state[]   = "--";
char play_state[]          = "ON ";
char fixed_state[]         = "OFF";

const char m_clic[] = "CC";
const char m_long[] = "LN";
const char m_vide[] = "--";
const char m_on[]   = "ON ";
const char m_off[]  = "OFF";

unsigned long last_pressed = 0;
unsigned long last_loop;
//unsigned long times[16];

// Pins de sortie de l'encodeur rotatif
const byte encoderA = 2;
const byte encoderB = 3;
Rotary rotary = Rotary(encoderA, encoderB);
// compteur de l'encodeur rotatif
int counter = 0;

/*
boolean refresh_play = false;
boolean refresh_repeat = false;
boolean refresh_swing = false;
boolean refresh_fixed = false;
*/

// cette variable contient la valeur en cours, ******* est ce vraiement utile ?
//byte changed_value; 

// modifie t'on les réglages du morceaux ?
boolean glob_mode = true;

// modifie t'on les sons utilisés ?
boolean map_mode = false;

// modifie t'on les paramètres d'un instrument ?
boolean param_mode = false;

// affiche t'on quelque chose ?
boolean view_visible = true;
// a t'on changé de vue ?
boolean view_changed = true;

// répète t'on for ever ?
boolean repeat_endless = false;

// Pour le QY70, faut-il arrêter les notes?
int last_sent_note = 0;
int last_sent_volume;

/*
// Est ce que l'affichage a changé ?
boolean screen_changed = true;

// Est ce que le mode d'affichage a changé (params, rhythm) ?
boolean display_changed = false;

// Affiche t'on le rythme ou les paramètres ?
boolean display_params = true;

uint32_t last_screen = 0;
*/

// avec le QY70, basculer le mode local en ON/OFF
boolean local_mode = true;

// A quelle vitesse joue t'on les notes ?
int time_between;      // intervalle moyen
int time_between_even; // delay pour les intervalles pairs
int time_between_odd;  // delay pour les intervalles impairs

void setup(void) {
  if (DEBUG || DEBUG_TIMING) Serial.begin(57600);
  else MIDI.begin();

  // pas pour chaque note avec le BPM choisi
  computeTimeBetween();

  if (CREATE_EEPROM) {
    createEeprom();
  }

  // encodeur rotatif
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  attachInterrupt(0, rotate, CHANGE);
  attachInterrupt(1, rotate, CHANGE);

  // fixer la durée en ms d'un clic
  button_map.setClickTicks(100);
  button_param.setClickTicks(100);
  button_glob.setClickTicks(100);
  button_inst.setClickTicks(100);
  button_view.setClickTicks(100);
  
  // fixer la durée en ms d'une pression longue
  button_map.setPressTicks(700);
  button_param.setPressTicks(700);
  button_glob.setPressTicks(700);
  button_inst.setPressTicks(700);
  button_view.setPressTicks(700);

  // attacher les évènements aux boutons
  button_map.attachClick(mapClick);
  button_map.attachLongPressStop(mapLongpress);
  button_param.attachClick(paramClick);
  button_param.attachLongPressStop(paramLongpress);
  button_glob.attachClick(globClick);
  button_glob.attachLongPressStop(globLongpress);
  button_inst.attachClick(instClick);
  button_inst.attachLongPressStop(instLongpress);
  button_view.attachClick(viewClick);
  button_view.attachLongPressStop(viewLongpress);

  // démarrer l'affichage sur écran OLED
  u8x8.begin();
  u8x8.setPowerSave(0); 

  if (DEBUG) Serial.print("time_between : ");
  if (DEBUG) Serial.println(time_between);

  //bank = banks[0];
  //setBank();

  loadPresetAddr();
  if ((preset_selected < 0) || (preset_selected > 23)) preset_selected = 0;
  if (DEBUG) Serial.print("preset_selected : ");
  if (DEBUG) Serial.println(preset_selected);
  loadEeprom();

  if (DEBUG) {
    for (int instrument = 0; instrument < 8; instrument++) {
      for (int parametre = 0; parametre < 5; parametre++) {
        Serial.print(instruments[0][instrument][parametre]);
        Serial.print(", ");
      }
      Serial.println();
    }
  }

  delay(50);

  resetPatterns();
  
  if (DEBUG) printPattern();
}

void loop(void) {

  
  button_map.tick();
  button_param.tick();
  button_glob.tick();
  button_inst.tick();
  button_view.tick();


  // ou en est on avec cette mesure ? *************************************
  if (play) {
    if (pas == MAXRES) {
      pas = 0;
      repeat ++;
      if (repeat == repeat_max) {
        repeat = 0;
        if (!repeat_endless) resetPatterns();
        if (DEBUG) printPattern(); 
      }
    }
  }
  

  // appliquer les modifications en fonction de l'encodeur rotatif *********
  if (counter != 0) {
   // screen_changed = true;
    
    if (glob_mode) {
      if (glob_selected == 0) { // changer le BPM
        bpm += counter;
        if (bpm < 20) bpm = 20;
        if (bpm > 255) bpm = 255;
        computeTimeBetween();
        counter = 0;
      }
      if (glob_selected == 1) { // changer le repeat
        repeat_max += counter;
        if (repeat_max < 1)   repeat_max = 1;
        if (repeat_max > 255) repeat_max = 255;
        counter = 0;
      }
      if (glob_selected == 2) { // changer le swing
        swing += counter;
        if (swing < 10) swing = 10;
        if (swing > 90) swing = 90;
        computeTimeBetween();
        counter = 0;
      }
    } 

    if (map_mode) {
      /*
      if (map_selected == 0) { // changer la bank 
        if (counter > 0) {
          bank++;
          if (bank >= bank_max) bank = 0;
        } else {
          bank--;
          if (bank < 0) bank = bank_max - 1;
        }
        setBank();
        counter = 0;
      }*/
      if (map_selected == 0) { // changer le preset
        if (counter > 0) {
          preset_selected++;
          if (preset_selected >= preset_max) preset_selected = 0;
          savePresetAddr();
          loadEeprom();
          view_changed = true;
          restart();
        } else {
          preset_selected--;
          if (preset_selected < 0) preset_selected = preset_max - 1;
          savePresetAddr();
          loadEeprom();
          view_changed = true;
          restart();
        }
        counter = 0;
      }
    }

 
    if (param_mode) {
      byte val = instruments[0][inst_selected][param_selected];
      switch(param_selected) {
        case 0:                  // banque
          instruments[0][inst_selected][param_selected] = modifyParam(val, 0, bank_max, counter);
          break;
        case 1:                  // note MIDI
          instruments[0][inst_selected][param_selected] = modifyParam(val, 0, 127, counter);
          break;
        case 2:                  // durée (pas utilisée)
          instruments[0][inst_selected][param_selected] = modifyParam(val, 0, 100, counter);
          break; 
        case 3:                  // subdivision min
          instruments[0][inst_selected][param_selected] = modifySubdivision(val, counter);
          break;        
        case 4:                  // volume = vélocité
          instruments[0][inst_selected][param_selected] = modifyParam(val, 0, 127, counter);
          break;    
      }
      counter = 0;
    }
  }

  // x secondes se sont écoulées depuis qu'on a pressé un bouton ************************
  if ((millis() - last_pressed > 2000) && (last_pressed != 0)) {
    resetButtonDisplay();
    last_pressed = 0;
  }

  //screen_changed = false;
  if (view_visible) {
    if (view == 0) displayParams();
    if (view == 1) displayRhythm();
    if (view == 2) displayAll();
    if (view == 3) displayDebug();
    //last_screen = millis();
  }

  if (DEBUG) Serial.print("avant envoi des notes : ");
  if (DEBUG) Serial.println(millis());

  // si le temps écoulé est inférieur à un changement de note on rajoute un petit delay
  long now = millis();

  if (pas%2 == 0) {
    if (now - last_loop < time_between_even) {
      delay(time_between_even - (now - last_loop) );
    }
    //times[pas] = now - last_loop;
  } else {
    if (now - last_loop < time_between_odd) {
      delay(time_between_odd - (now - last_loop) );
    }
    //times[pas] = now - last_loop;
  }
  
  //if (DEBUG) Serial.println(moyenneTimes());
  last_loop = millis();
  
  
  if (play) { // jouer les notes *************************************
    for (byte i = 0; i < 8; i++) {
      if (patterns[i][pas] == 1) {
        if (DEBUG) {        
          Serial.print("instrument ");
          Serial.print(i);
          Serial.print(" / jouer note ");
          Serial.print(instruments[0][i][1]); 
          Serial.print(" ");
          Serial.print(instruments[0][i][4]);
          Serial.println();
        } 
        

     
        if (!DEBUG) {
          byte b = instruments[0][i][0];
          MIDI.sendProgramChange(banks[b], 10);
          
          //second petit hack pour le QY70
          if (last_sent_note != 0) {
            MIDI.sendNoteOff(last_sent_note, 0, 10);
          }
          MIDI.sendNoteOn(instruments[0][i][1], instruments[0][i][4], 10);
          last_sent_note = instruments[0][i][1];

          // petit hack spécial pour le Yamaha QY70 : ça ne marche pas, ce code CC n'est pas pris en charge...
          /*
          if (local_mode) {
            MIDI.sendControlChange(122, 127, 10);
          } else {
            MIDI.sendControlChange(122, 0, 10);
          }
          local_mode = !local_mode;
          */
        }

        break;
      }
    }
  }

  if (DEBUG_TIMING) Serial.print(millis());
  if (DEBUG_TIMING) Serial.print(" / pas ");
  if (DEBUG_TIMING) Serial.print(pas);
  if (DEBUG_TIMING) Serial.println(" fin de boucle");
  

  if (play) pas ++;

}










