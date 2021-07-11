/* 42 octets sont nécessaires pour enregistrer un preset 
bpm         : 1 octet
swing       : 1 octet
instruments : 8x5 octets

On peut donc enregistrer 24 presets

Le premier octet sert à conserver le dernier preset utilisé (pour rallumer)

*/

void loadPresetAddr() {
  int addr = 0;
  byte value;
  while (!eeprom_is_ready());
  cli();
  value = EEPROM.read(addr);
  sei();
  preset_selected = value;
}

void savePresetAddr() {
  int addr = 0;
  while (!eeprom_is_ready());
  cli();
  EEPROM.update(addr, preset_selected);
  sei();
}


void createEeprom() {
  int addr = 0;
  while (!eeprom_is_ready());
  cli();
  EEPROM.write(addr, 0);
  sei();
  
  for (int i = 0; i < 24; i++) {
    int addr = (42 * i) + 1;
    while (!eeprom_is_ready());
    cli();
    EEPROM.write(addr, bpm);
    sei();
    int paddr = addr + 1;
    while (!eeprom_is_ready());
    cli();
    EEPROM.write(paddr, swing);
    sei();
    for (int instrument = 0; instrument < 8; instrument++) {
      for (int parametre = 0; parametre < 5; parametre++) {
        int paddr = (addr + 2) + (instrument * 5) + parametre;
        while (!eeprom_is_ready());
        cli();
        EEPROM.write(paddr, instruments[0][instrument][parametre]);
        sei();
      }
    }
  }
}

void loadEeprom() {
  int addr = 42 * preset_selected + 1;
  while (!eeprom_is_ready());
  cli();
  bpm   = EEPROM.read(addr);
  sei();
  while (!eeprom_is_ready());
  cli();
  swing = EEPROM.read(addr + 1);
  sei();
  byte value;
  for (int instrument = 0; instrument < 8; instrument++) {
    for (int parametre = 0; parametre < 5; parametre++) {
      int paddr = (addr + 2) + (instrument * 5) + parametre;
      while (!eeprom_is_ready());
      cli();
      value = EEPROM.read(paddr);
      sei();
      instruments[0][instrument][parametre] = value;
    }
  }
}

void saveEeprom() {
  int addr = 42 * preset_selected + 1;
  while (!eeprom_is_ready());
  cli();
  EEPROM.write(addr, bpm);
  sei();
  while (!eeprom_is_ready());
  cli();
  EEPROM.write(addr + 1, swing);
  sei();
  for (int instrument = 0; instrument < 8; instrument++) {
    for (int parametre = 0; parametre < 5; parametre++) {
      int paddr = (addr + 2) + (instrument * 5) + parametre;
      while (!eeprom_is_ready());
      cli();
      EEPROM.write(paddr, instruments[0][instrument][parametre]);
      sei();
    }
  }
}

