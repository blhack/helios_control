#include <DmxSimple.h>
#include <MIDI.h>

#define max_channels 512
#define space 61
#define max_keys 121
#define num_flashes 8

byte channels[max_channels];
int reds[] = {474};
int greens[] = {475};
int blues[] = {476};

bool keys[max_keys];
bool flashes[num_flashes];

bool flashOn = false;

byte displayed_r = 0;
byte displayed_g = 0;
byte displayed_b = 0;

byte measured_r = 0;
byte measured_g = 0;
byte measured_b = 0;

byte b_max = space;
byte g_max = (space - (space/3.0));
byte r_max = (space - ((space/3.0)*2));

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  DmxSimple.usePin(4);
  DmxSimple.maxChannel(max_channels);

  all_on();
  delay(500);
  all_off();

  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void tick() {
  if (measured_r > displayed_r) {
    displayed_r++;
  }
  if (measured_r < displayed_r) {
    displayed_r--;
  }

  if (measured_g > displayed_g) {
    displayed_g++;
  }
  if (measured_g < displayed_g) {
    displayed_g--;
  }

  if (measured_b > displayed_b) {
    displayed_r++;
  }
  if (measured_b < displayed_b) {
    displayed_b--;
  }

  for (int i=0; i<(sizeof(reds)/sizeof(int)); i++) {
    DmxSimple.write(reds[i], displayed_r);
  }
  for (int i=0; i<(sizeof(greens)/sizeof(int)); i++) {
    DmxSimple.write(greens[i], displayed_g);
  }
  for (int i=0; i<(sizeof(blues)/sizeof(int)); i++) {
    DmxSimple.write(blues[i], displayed_g);
  }
}

void rgb(int pitch) {
  measured_r = 0;
  measured_g = 0;
  measured_b = 0;


  if (pitch > 60) {
    int offset = (pitch % 60);
    pitch = 60 - offset;
  }

  if (pitch > space) {
    pitch = pitch - (pitch - space);
  }

  if (pitch < r_max) {
    measured_r = map(pitch, 0, r_max, 0, 255);
  }
  if ((pitch >= r_max) && (pitch <= g_max)) {
    measured_b = map(pitch - r_max, 0, r_max, 0, 255);
    measured_r = 255 - measured_b;
  }
  if (pitch >= g_max) {
    measured_r = map(pitch - g_max, 0, r_max, 0, 255);
    measured_g = 255 - measured_r;
  }
  if (pitch == space) {
    measured_b = 255;
  }
}

float getpitch() {
  float total = 0;
  float oncount = 0;
  for (int i=0; i<(sizeof(keys)/sizeof(bool)); i++) {
    if (keys[i] == true) {
      total = total + i;
      oncount++;
    }
  }
  return(total/oncount);
}

void flash() {
  if (flashes[0] == true) {
    DmxSimple.write(474, 255);
  }
  else {
    DmxSimple.write(474,0);
  }
  
  if (flashes[1] == true) {
    //
  }
  else {
    //
  }
  
  if (flashes[2] == true) {
    //  
  }
  else {
    //  
  }
  
  if (flashes[3] == true) {
    
  }
  else {
    
  }
  
  if (flashes[4] == true) {
    //  
  }
  else {
    //  
  }
  
  if (flashes[5] == true) {
    //  
  }
  else {
    //  
  }
  
  if (flashes[6] == true) {
    //  
  }
  else {
    //  
  }
  
  if (flashes[7] == true) {
    //  
  }
  else {
    //  
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (channel == 10) {
    flashes[pitch-36] = true;
    flash();
  }
  else {
    keys[pitch] = true;
    pitch = getpitch();
    rgb(pitch);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  if (channel == 10) {
    flashes[pitch-36] = false;
    flash();
  }
  else {
    keys[pitch] = false;
    pitch = getpitch();
    rgb(pitch);
  }
}

void all_on() {
  for (int i=0; i<(sizeof(reds)/sizeof(int)); i++) {
    DmxSimple.write(reds[i],255);
  }
  delay(500);
  for (int i=0; i<(sizeof(greens)/sizeof(int)); i++) {
    DmxSimple.write(greens[i],255);
  }
  delay(500);
  for (int i=0; i<(sizeof(blues)/sizeof(int)); i++) {
    DmxSimple.write(blues[i],255);
  }
}

void all_off() {
  for (int i=0; i<(sizeof(reds)/sizeof(int)); i++) {
    DmxSimple.write(reds[i],0);
  }
  delay(500);
  for (int i=0; i<(sizeof(greens)/sizeof(int)); i++) {
    DmxSimple.write(greens[i],0);
  }
  delay(500);
  for (int i=0; i<(sizeof(blues)/sizeof(int)); i++) {
    DmxSimple.write(blues[i],0);
  }
}

void loop() {
  if (((millis() % 5) == 0)) {
    int pitch = getpitch();
    rgb(pitch);
    tick();
  }
  MIDI.read();
}
