void displayParams() { 

  if (view_changed) {
    u8x8.clearDisplay();
  } else {

    u8x8.setFont(u8x8_font_chroma48medium8_r);



    // variables utilisées pour transformer les valeurs numériques en char arrays
    //String str;
    char valeur_a_afficher[4] = "    ";

    // première ligne
    u8x8.drawString(0, 0, "        SWG FIX");
    format(swing).toCharArray(valeur_a_afficher, 4); 
    u8x8.drawString(8, 1, valeur_a_afficher);
    u8x8.drawString(12, 1, fixed_state);
    
    // seconde ligne
    u8x8.drawString(0, 2, "PAS BPM PRE INS");
    //u8x8.drawString(0, 3, "               ");
    format(pas).toCharArray(valeur_a_afficher, 4); 
    u8x8.drawString(0, 3, valeur_a_afficher);
    format(bpm).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(4, 3, valeur_a_afficher);
    format(preset_selected).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(8, 3, valeur_a_afficher);
    format(inst_selected).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(12, 3, valeur_a_afficher);


  
    // troisième ligne
    u8x8.drawString(0, 4, "BNK NOT DEN SUB");
    format(instruments[0][inst_selected][0]).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(0, 5, valeur_a_afficher);
    format(instruments[0][inst_selected][1]).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(4, 5, valeur_a_afficher);
    format(instruments[0][inst_selected][2]).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(8, 5, valeur_a_afficher);
    format(instruments[0][inst_selected][3]).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(12, 5, valeur_a_afficher);
  
    // quatrième ligne
    u8x8.drawString(0, 6, "VOL     REP PLY");
    //u8x8.drawString(0, 7, "               ");
    format(instruments[0][inst_selected][4]).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(0, 7, valeur_a_afficher);
    format(repeat_max).toCharArray(valeur_a_afficher, 4);
    u8x8.drawString(8, 7, valeur_a_afficher);
    u8x8.drawString(12, 7, play_state);
  
    u8x8.setInverseFont(1);
    //if (glob_mode) u8x8.drawString(4, 2, "BPM");
    //if (_mode) u8x8.drawString(8, 6, "REP");
    if (param_mode) {
      switch(param_selected) {
        case 0:
          u8x8.drawString(0, 4, "BNK");
          break;
        case 1:
          u8x8.drawString(4, 4, "NOT");
          break;
        case 2:
          u8x8.drawString(8, 4, "DEN");
          break;
        case 3:
          u8x8.drawString(12, 4, "DUB");
          break;
        case 4:
          u8x8.drawString(0, 6, "VOL");
          break;
      }
    }
    if (map_mode) {
      switch(map_selected) {
        case 0:
          u8x8.drawString(8, 2, "PRE");
          break;
      }
    }
    if (glob_mode) {
      switch(glob_selected) {
        case 0:
          u8x8.drawString(4, 2, "BPM");
          break;
        case 1:
          u8x8.drawString(8, 6, "REP");
          break;
        case 2:
          u8x8.drawString(8, 0, "SWG");
          break;
      }
    }
    u8x8.setInverseFont(0);
    u8x8.refreshDisplay(); 
  }
  view_changed = false;
}


void displayRhythm() {

  if ((!view_changed) && (pas == 0)) {
    u8x8.setFont(u8x8_font_chroma48medium8_r);   
    for (byte i = 0; i < 8; i++) {
      for (byte j = 0; j < 16; j++) {
        if (patterns[i][j] == 1) u8x8.drawString(j, i, "O");
        else u8x8.drawString(j, i, ".");
      }
    }
  }

  if (view_changed) {
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_chroma48medium8_r);   
    for (byte i = 0; i < 8; i++) {
      for (byte j = 0; j < 16; j++) {
        if (patterns[i][j] == 1) u8x8.drawString(j, i, "O");
        else u8x8.drawString(j, i, ".");
      }
    }
  } else {
    u8x8.setFont(u8x8_font_chroma48medium8_r);  
    u8x8.setInverseFont(1); 
    for (byte i = 0; i < 8; i++) {
      if (patterns[i][pas] == 1) u8x8.drawString(pas, i, "O");
      else u8x8.drawString(pas, i, ".");
    }
    u8x8.setInverseFont(0);
    byte pas_prev = pas - 1;
    if (pas_prev < 0) pas_prev = 15;
    for (byte i = 0; i < 8; i++) {
      if (patterns[i][pas_prev] == 1) u8x8.drawString(pas_prev, i, "O");
      else u8x8.drawString(pas_prev, i, ".");
    }
  }
  view_changed = false;
}

/* TROP LENT
void displayRhythm() {

  boolean main;
  
  if ((!view_changed) && (pas == 0)) {
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    for (byte j = 0; j < 16; j++) {
      main = false;
      for (byte i = 0; i < 8; i++) {
        if (patterns[i][j] == 1) {
          if (!main) {
            u8x8.drawString(j, i, "#");
            main = true;
          } else u8x8.drawString(j, i, "!");
        }
        else u8x8.drawString(j, i, ".");
      }
    }
    
  }

  if (view_changed) {
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_chroma48medium8_r);   
    for (byte j = 0; j < 16; j++) {
      main = false;
      for (byte i = 0; i < 8; i++) {
        if (patterns[i][j] == 1) {
          if (!main) {
            u8x8.drawString(j, i, "#");
            main = true;
          } else u8x8.drawString(j, i, "!");
        }
        else u8x8.drawString(j, i, ".");
      }
    }
  } else {
    u8x8.setFont(u8x8_font_chroma48medium8_r);  
    u8x8.setInverseFont(1); 
    main = false;
    for (byte i = 0; i < 8; i++) {
      if (patterns[i][pas] == 1) {
        if (!main) {
          u8x8.drawString(pas, i, "#");
          main = true;
        } else u8x8.drawString(pas, i, "!");
      }  else u8x8.drawString(pas, i, ".");
    }
    u8x8.setInverseFont(0);
    byte pas_prev = pas - 1;
    if (pas_prev < 0) pas_prev = 15;
    main = false;
    for (byte i = 0; i < 8; i++) {
      if (patterns[i][pas_prev] == 1) {
        if (!main) {
          u8x8.drawString(pas_prev, i, "#");
          main = true;
        } else u8x8.drawString(pas_prev, i, "!");
      }  else u8x8.drawString(pas_prev, i, ".");
    }
  }
  view_changed = false;
}
*/


void displayAll() {
  if (view_changed) {
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    String str;
    char valeur_a_afficher[4] = "....";
    for (int i = 0; i < 8; i++) {
      String(instruments[0][i][0]).toCharArray(valeur_a_afficher, 3);
      u8x8.drawString(0, i, valeur_a_afficher);
      String(instruments[0][i][1]).toCharArray(valeur_a_afficher, 3);
      u8x8.drawString(3, i, valeur_a_afficher);
      String(instruments[0][i][2]).toCharArray(valeur_a_afficher, 4);
      u8x8.drawString(6, i, valeur_a_afficher);
      String(instruments[0][i][3]).toCharArray(valeur_a_afficher, 3);
      u8x8.drawString(10, i, valeur_a_afficher);
      String(instruments[0][i][4]).toCharArray(valeur_a_afficher, 4);
      u8x8.drawString(13, i, valeur_a_afficher);
    }
  }
  view_changed = false;
}

        /*
        Banque           : instruments[preset_selected][inst_selected][0]
   Note MIDI        : instruments[preset_selected][inst_selected][1]
   Densité          : instruments[preset_selected][inst_selected][2]
   Subdivision min. : instruments[preset_selected][inst_selected][3]
   Volume           : instruments[preset_selected][inst_selected][4]
    */



void displayDebug() {
  if (view_changed) {
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "display debug");
    
    // première ligne
    u8x8.drawString( 0, 0, "MP PA GL IN VW");
    u8x8.drawString( 0, 1, button_map_state);
    u8x8.drawString( 3, 1, button_param_state);
    u8x8.drawString( 6, 1, button_glob_state);
    u8x8.drawString( 9, 1, button_inst_state);
    u8x8.drawString(12, 1, button_view_state);
  }
  view_changed = false;
}

