// ****************************************************************

void resetButtonDisplay() {
  memcpy(button_map_state, m_vide, 2);
  memcpy(button_param_state, m_vide, 2);
  memcpy(button_glob_state, m_vide, 2);
  memcpy(button_inst_state, m_vide, 2);
  memcpy(button_view_state, m_vide, 2);
 // screen_changed = true;
}
/*
void presetChange() {
  screen_changed = true;
  preset_selected ++;
  if (preset_selected >= preset_max) preset_selected = 0;
  counter = 0;
}*/

void presetSave() {
}
/*
void changeParam() {
  screen_changed = true;
  param_selected ++;
  if (param_selected >= param_max) param_selected = 0;
  counter = 0;
}*/

void changeBpm() {
 // screen_changed = true;
  counter = 0;
}

void playStop() {
 // screen_changed = true;
  play = !play;
  pas = 0;
  repeat = 0;
  if (play) memcpy(play_state, m_on, 3);
  else memcpy(play_state, m_off, 3);
}
/*
void play() {
 // screen_changed = true;
  play = true;
  memcpy(play_state, m_on, 3);
}*/

void restart() {
 // screen_changed = true;
  pas = 0;
  repeat = 0;
  resetPatterns();
}

void fixedOnOff() {
//  screen_changed = true;
  repeat_endless = !repeat_endless;

  if (repeat_endless) memcpy(fixed_state, m_on, 3);
  else memcpy(fixed_state, m_off, 3);
}

String format(byte b) {
  String ns = "";
  if (b < 100)  ns = String(b) + " ";
  else if (b < 10)  ns = String(b) + "  ";
  else ns = String(b);
  return ns;
}

/*
void changeRepeat() {
  screen_changed = true;
  counter = 0;
}
*/
/*
void changeInst() {
  screen_changed = true;
  inst_selected ++;
  if (inst_selected >= inst_max) inst_selected = 0;
}*/

void midiAllNotesOff() {
  for (byte i = 0; i < 16; i++) {
    MIDI.sendControlChange(123, 0, i); // envoyer le message sur chaque canal
  }
}
/*
void setBank() {
  //MIDI.sendControlChange(0, bank - 1, 10);
  //MIDI.sendControlChange(32, 0, 10);
  //MIDI.sendProgramChange(banks[bank-1], 10);
}
*/

byte modifyParam(byte val, byte val_min, byte val_max, int counter) {
  val += counter; 
  if (val < val_min) {
    val = val_min;
  }
  if (val > val_max) {
   val = val_max;
  }
  return val;
}

byte modifySubdivision(byte val, int counter) {
  if (counter > 0) val = val << 1;
  else val = val >> 1;
  if (val > MAXRES) val = MAXRES;
  if (val < 1) val = 1;
  return val;
}
/*
unsigned long moyenneTimes() {
  unsigned long m = 0;
  for (byte j = 0; j < 16; j++) {
    m += times[j];
  }
  m = m / 16; // 16
  return m;
}*/

void computeTimeBetween() {
  time_between = int((60000 / bpm) / (MAXRES / 4));
  time_between_even = int(time_between * 2 * ((float)swing / 100));
  time_between_odd  = int((time_between * 2) - time_between_even);
  if (DEBUG_TIMING) Serial.print("time_between ");
  if (DEBUG_TIMING) Serial.print(time_between);
  if (DEBUG_TIMING) Serial.print(" time_between_even ");
  if (DEBUG_TIMING) Serial.print(time_between_even);
  if (DEBUG_TIMING) Serial.print(" time_between_odd ");
  if (DEBUG_TIMING) Serial.println(time_between_odd);
}

