void keyPressed() {
  
  // sauvegarder les presets
  if ((key == 's') || (key == 'S')) {
    Date now = new Date();
    SimpleDateFormat formater = new SimpleDateFormat("yyyyMMdd_HHmmss");
    //System.out.println(formater.format(now));
    //saveFrame(SKETCH_NAME + "_" + formater.format(now) + ".png");
    String[] preset = new String[patterns.length];
    for (int i = 0; i < instruments.length; i++) {
      preset[i] = "";
      for (int j = 0; j < instruments[i].length; j++) {
        preset[i] += instruments[i][j];
        if (j < instruments[i].length - 1) preset[i] += ";";
      }
    }
    saveStrings(SKETCH_NAME + "_" + formater.format(now) + "_presets.txt", preset);
  }
  
  // sauvegarder le pattern en cours
  if ((key == 'p') || (key == 'P')) {
    Date now = new Date();
    SimpleDateFormat formater = new SimpleDateFormat("yyyyMMdd_HHmmss");
    String[] pattern = new String[patterns.length];
    for (int i = 0; i < patterns.length; i++) {
      pattern[i] = "";
      for (int j = 0; j < patterns[i].length; j++) {
        if (patterns[i][j] == 0) pattern[i] += ".";
        if (patterns[i][j] == 1) pattern[i] += "!";
        if (patterns[i][j] == 2) pattern[i] += "#";
      }
    }
    saveStrings(SKETCH_NAME + "_" + formater.format(now) + "_pattern.txt", pattern);
  }
}