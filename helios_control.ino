#include <DmxSimple.h>
#include <MIDI.h>

#define max_channels 512     //the maximum number of DMX channels to support
#define space 61.0           //the number of available hardware keys that is available at a time (must be a float)
#define max_keys 121         //the number of keys on your keyboard 121 is the highest pitch that will ever be received
#define num_flashes 8        //the number of buttons that can be associated with a "flash" or "torch" event
#define min_key 36           //the leftmost midi key ID when transpose is 0
#define max_key 96           //the rightmost midi key ID when transpose is 0

byte channels[max_channels];
int reds[] = {474};          //red DMX channels
int greens[] = {1};        //green DMX channels
int blues[] = {476};         //blue DMX channels

bool keys[max_keys];         //an array that stores key states
bool flashes[num_flashes];   //an array that stores "flash" or "torch" states 

bool flashOn = false;        //is "flash" enabled?  (This will not progress color changes)

byte displayed_r = 0;        //the currently displayed R value
byte displayed_g = 0;        //the currently displayed G value
byte displayed_b = 0;        //the currently displayed B value

byte measured_r = 0;         //the currently *measured* R value (as determined by key presses)
byte measured_g = 0;         //the currently *measured* G Value (as determined by key presses)
byte measured_b = 0;         //the currently *measured* B Value (as determined by key presses)

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
    displayed_b++;
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
    DmxSimple.write(blues[i], displayed_b);
  }
}

void rgb(int pitch) {
  measured_r = 0;
  measured_g = 0;
  measured_b = 0;

  if (pitch == 0) {
    return;
  }
  
  if (pitch > max_key) {
    int offset = (pitch - max_key);
    pitch = max_key - offset;
  }
  
  if (pitch < min_key) {
    int offset = (min_key - pitch);
    pitch = min_key + offset;
  }

  measured_r = map(pitch,min_key,max_key,0,255);
  measured_g = map(pitch,min_key,max_key,255,0);
  
  if (pitch > (min_key + (space/2.0))) {
    int offset = (pitch - (min_key + (space/2.0)));
    pitch = (min_key + (space/2.0)) - offset;
  }
  measured_b = map(pitch,min_key,(min_key + (space/2.0)),0,255);
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


void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (channel == 10) {
    torch_on(pitch);
  }
  else {
    keys[pitch] = true;
    pitch = getpitch();
    rgb(pitch);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  if (channel == 10) {
    torch_off(pitch);
  }
  else {
    keys[pitch] = false;
    pitch = getpitch();
    rgb(pitch);
  }
}

void torch_on(int pitch) {
  switch(pitch) {
    case 36:
      DmxSimple.write(475, 255);
      break;
    case 42:
      DmxSimple.write(475, 255);
      break;
  }
}

void torch_off(int pitch) {
  switch(pitch) {
    case 36:
      DmxSimple.write(475, 0);
      break;
    case 42:
      DmxSimple.write(475, 0);
      break;
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
