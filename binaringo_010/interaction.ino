// ****************************************************************

// La fonction rotate est appelée à chaque fois que l'encodeur rotatif change d'état
void rotate() {
  unsigned char result = rotary.process();
  if (result == DIR_CW) {
    counter++;
    //Serial.println(counter);
  } else if (result == DIR_CCW) {
    counter--;
    //Serial.println(counter);
  }
}


void mapClick() {                        // CHANGER DE ....

  if (glob_mode || param_mode) { // au premier clic on ne change pas, on sélectionne
    glob_mode = false;
    param_mode = false;
    map_mode = true;
  } else {
    map_selected++;
    if (map_selected >= map_max) map_selected = 0;
  }

  last_pressed = millis();
  memcpy(button_map_state, m_clic, 2);
} 

void mapLongpress() {                    // SAUVER LE PRESET
  last_pressed = millis();
  memcpy(button_map_state, m_long, 2);
  /*
  if (preset_selected > 24) {
    if ((preset_selected > 23) && ()) preset_selected
    if (preset_selected > 200) preset_selected = 0;
  }*/
  if ((preset_selected >= 0) && (preset_selected < 24)) saveEeprom();
}

void paramClick() {                         // CHOISIR LE PARAMETRE
  
  if (glob_mode || map_mode) { // au premier clic on ne change pas, on sélectionne
    glob_mode  = false;
    param_mode = true;
    map_mode   = false;
  } else {
    param_selected++;
    if (param_selected >= param_max) param_selected = 0;
  }

  last_pressed = millis();
  memcpy(button_param_state, m_clic, 2);
} 

void paramLongpress() {                       // BASCULER REPEAT ENDLESS
  last_pressed = millis();
  memcpy(button_param_state, m_long, 2);
  fixedOnOff();
}

void globClick() {                           // CHANGER LES REGLAGES GLOBAUX

  if (param_mode || map_mode) { // au premier clic on ne change pas, on sélectionne
    glob_mode  = true;
    param_mode = false;
    map_mode   = false;
  } else {
    glob_selected++;
    if (glob_selected >= glob_max) glob_selected = 0;
  }

  last_pressed = millis();
  memcpy(button_glob_state, m_clic, 2);  
} 

void globLongpress() {                       // BASCULER PLAY / STOP
  last_pressed = millis();
  memcpy(button_glob_state, m_long, 2);
  
  playStop();
}

void instClick() {                          // CHANGER D'INSTRUMENT
  /*
  glob_mode  = false;
  param_mode = false;
  map_mode   = false;
  */
  inst_selected ++;
  if (inst_selected >= inst_max) inst_selected = 0;

  last_pressed = millis();
  memcpy(button_inst_state, m_clic, 2);
} 

void instLongpress() {                       // BASCULER PLAY / STOP
  last_pressed = millis();
  memcpy(button_inst_state, m_long, 2);
  midiAllNotesOff();
}

void viewClick() {
  view++;
  if (view >= view_max) view = 0;
  view_changed = true;
  
  last_pressed = millis();
  memcpy(button_inst_state, m_long, 2);
}

void viewLongpress() {
  view_visible = !view_visible;
  u8x8.clearDisplay();
  
  last_pressed = millis();
  memcpy(button_view_state, m_long, 2);
  
}
/*
void repeatClick() {                        // CHANGER LE "REPEAT"
  bpm_mode = false;
  repeat_mode = true;
  screen_changed = true;
  last_pressed = millis();
  //Serial.println("bpm clic");
  memcpy(button_repeat_state, m_clic, 2);
  changeRepeat();
} 

void repeatLongpress() {                       // BASCULER PLAY / STOP
  
  //Serial.println("preset long");
  last_pressed = millis();
  memcpy(button_repeat_state, m_long, 2);
  display_params = !display_params;
  display_changed = true;
  
}*/
