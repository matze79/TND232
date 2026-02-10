// matze79 - 01-01-2026

#define USE_PINSET_A 1
//#define USE_PINSET_A 0

#if USE_PINSET_A

// SMD Version
#define WE     12
#define LED    10
#define PIN_D0 2
#define PIN_D1 3
#define PIN_D2 4
#define PIN_D3 5
#define PIN_D4 6
#define PIN_D5 7
#define PIN_D6 8
#define PIN_D7 9

#else

// DHT Version
#define WE     13
#define LED    10
#define PIN_D0 14
#define PIN_D1 15
#define PIN_D2 16
#define PIN_D3 17
#define PIN_D4 18
#define PIN_D5 19
#define PIN_D6 2
#define PIN_D7 3

#endif
unsigned long lastSerialActivity = 0;
unsigned long ledOnUntil = 0;
bool muted = false;

const unsigned long IDLE_TIMEOUT_MS = 1000;
const unsigned long LED_ACTIVITY_MS = 120;

void SendByteToSoundChip(byte b)
{
  digitalWrite(PIN_D0, (b & 1)   ? HIGH : LOW);
  digitalWrite(PIN_D1, (b & 2)   ? HIGH : LOW);
  digitalWrite(PIN_D2, (b & 4)   ? HIGH : LOW);
  digitalWrite(PIN_D3, (b & 8)   ? HIGH : LOW);
  digitalWrite(PIN_D4, (b & 16)  ? HIGH : LOW);
  digitalWrite(PIN_D5, (b & 32)  ? HIGH : LOW);
  digitalWrite(PIN_D6, (b & 64)  ? HIGH : LOW);
  digitalWrite(PIN_D7, (b & 128) ? HIGH : LOW);

  digitalWrite(WE, LOW);
  delayMicroseconds(5);   // stabiler als delay(1)
  digitalWrite(WE, HIGH);
}

void SilenceAllChannels()
{
  // NUR EINMAL, nie im Datenstrom
  SendByteToSoundChip(0x9F);
  SendByteToSoundChip(0xBF);
  SendByteToSoundChip(0xDF);
  SendByteToSoundChip(0xFF);
}

void setup()
{
  pinMode(WE, OUTPUT);
  digitalWrite(WE, HIGH);
 
  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  pinMode(PIN_D2, OUTPUT);
  pinMode(PIN_D3, OUTPUT);
  pinMode(PIN_D4, OUTPUT);
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D6, OUTPUT);
  pinMode(PIN_D7, OUTPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  PlayJingle();

  Serial.begin(115200);
  lastSerialActivity = millis();
}

void SendTone(byte channel, int frequency, byte volume) {
  if (frequency == 0) { // Silence
    SendByteToSoundChip(0x90 | (channel << 5) | 0x0F); // Mute
    return;
  }

  int N = 3579545 / (32 * frequency); // SN76489 frequency formula
  byte lsb = N & 0x0F;
  byte msb = (N >> 4) & 0x3F;

  // Tone register
  SendByteToSoundChip(0x80 | (channel << 5) | lsb); // LSB
  SendByteToSoundChip(msb);                         // MSB

  // Volume register (0=full, 15=silent)
  SendByteToSoundChip(0x90 | (channel << 5) | (15 - volume));
}

void PlayJingle() {
  // C-Dur Tonleiter
  const int freq[] = {
    262,  // C4
    294,  // D4
    330,  // E4
    349,  // F4
    392,  // G4
    440,  // A4
    494,  // B4
    523   // C5
  };

  const int noteCount = sizeof(freq) / sizeof(freq[0]);

  SilenceAllChannels();

  for (int i = 0; i < noteCount; i++) {

    byte channel = i % 3;   // 0,1,2

    int N = 3579545 / (32 * freq[i]);

    byte lsb = N & 0x0F;
    byte msb = (N >> 4) & 0x3F;

    // --- Tone latch (LSB) ---
    SendByteToSoundChip(0x80 | (channel << 5) | lsb);

    // --- Tone MSB ---
    SendByteToSoundChip(msb);

    // --- Volume (0 = laut) ---
    SendByteToSoundChip(0x90 | (channel << 5) | 0x00);
    
    delay(300);
   }
  SilenceAllChannels();
}


void loop()
{
  // --- KRITISCHER PFAD ---
  while (Serial.available() > 0) {
    byte b = Serial.read();
    SendByteToSoundChip(b);

    lastSerialActivity = millis();
    ledOnUntil = lastSerialActivity + LED_ACTIVITY_MS;
    muted = false;
  }

  // --- UNKRITISCHER PFAD ---
  unsigned long now = millis();

  // LED nur zeitgesteuert, kein Einfluss auf RX
  if (now < ledOnUntil) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  // Idle-Mute nur wenn WIRKLICH ruhig
  if (!muted && (now - lastSerialActivity > IDLE_TIMEOUT_MS)) {
    SilenceAllChannels();
    muted = true;
  }
}
