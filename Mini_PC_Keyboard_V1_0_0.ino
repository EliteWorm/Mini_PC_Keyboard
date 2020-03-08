//Mini PC Keyboard by Elite Worm (YouTube.com/c/EliteWorm)
//Version 1.0.0

const byte north = 16;
const byte south = 7;
const byte west = 8;
const byte east = 6;
const byte northwest = 14;
const byte northeast = 9;
const byte southwest = 5;
const byte southeast = 4;
const byte shift = 3;
const byte control = 2;

const byte leds = 10;

const byte keys[10] = {north, south, west, east, northwest, northeast, southwest, southeast, shift, control};

byte sender_step = 0;
byte activekey = 0;
bool currentkeys[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
bool previouskeys[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
bool allowsending = false;

bool shifthold = false;
bool ctrlhold = false;

byte t_key_t1 = 150;
byte t_key_t2 = 40;
long t_key_printed = 0;

byte leds_step = 1;
bool ramp = 1;
byte brightness = 0;
long t_leds_previous = 0;
byte t_leds_delay = 10;

long t_timemout_previous = 0;
bool timeout = 0;
int t_timeout = 10000;

void setup() {
  
  pinMode(north, INPUT_PULLUP);
  pinMode(south, INPUT_PULLUP);
  pinMode(west, INPUT_PULLUP);
  pinMode(east, INPUT_PULLUP);
  pinMode(northwest, INPUT_PULLUP);
  pinMode(northeast, INPUT_PULLUP);
  pinMode(southwest, INPUT_PULLUP);
  pinMode(southeast, INPUT_PULLUP);
  pinMode(shift, INPUT_PULLUP);
  pinMode(control, INPUT_PULLUP);  
  
  pinMode(leds, OUTPUT);

  Serial.begin(9600);
  Keyboard.begin();
  
}

void loop() {

  scankeys();
  processkeys();  
  
  if(allowsending) {
	  
    sender();
    t_timemout_previous = millis();
    timeout = 0;
    leds_step = 2;
	
  }
  
  light();
 
}

void scankeys() {
  
  for (byte k = 0;k <= 9;k++) {
    
    currentkeys[k] = digitalRead(keys[k]);    
   
  }
    
}

void processkeys() {
  
  for (byte k = 0;k <= 9;k++) {
    
    if((currentkeys[k]) < (previouskeys[k])) {
      
      activekey = k;
      allowsending = true;
      sender_step = 0;
      
    }
  
  }
  
  if(currentkeys[activekey]) {
    
    allowsending = false;
    sender_step = 0;
    
  }
  
  if((currentkeys[8]) && (shifthold)) {
    
    Keyboard.release(0x85);
    shifthold = false;
    
  }
  
  if((currentkeys[9]) && (ctrlhold)) {
    
    Keyboard.release(0x84);
    ctrlhold = false;
    
  }
  
  for (byte k = 0;k <= 9;k++) (previouskeys[k] = currentkeys[k]);
  
}

void printkey() {

  switch(activekey){
    
    case 0:

      Keyboard.write(0xDA);
      
    break;
    
    case 1:
      
      Keyboard.write(0xD9);
      
    break;
    
    case 2:
    
      Keyboard.write(0xD8);
    
    break;
    
    case 3:
    
      Keyboard.write(0xD7);
    
    break;
    
    case 4:
    
      Keyboard.press(0x85);
      Keyboard.press(0xE7);
      Keyboard.release(0xE7);
      Keyboard.release(0x85);
    
    break;
    
    case 5:
    
      Keyboard.press(0x85);
      Keyboard.press(0xE9);
      Keyboard.release(0xE9);
      Keyboard.release(0x85);
      
    break;
    
    case 6:
    
      Keyboard.press(0x85);
      Keyboard.press(0xE1);
      Keyboard.release(0xE1);
      Keyboard.release(0x85);
    
    break;
    
    case 7:
    
      Keyboard.press(0x85);
      Keyboard.press(0xE3);
      Keyboard.release(0xE3);
      Keyboard.release(0x85); 
    
    break;
    
    case 8:
      
      if(shifthold == 0) {

        Keyboard.press(0x85);
        shifthold = 1;
        
      }
      
    break;
    
    case 9:
      
      if(ctrlhold == 0) {
        
        Keyboard.press(0x84);
        ctrlhold = 1;
        
      }
    
    break;
    
  }

}

void sender() {
	
  switch(sender_step) {
		
    case 0:
		
      printkey();
      t_key_printed = millis();
      sender_step++;
			
    break;
		
    case 1:
		
      if(millis() - t_key_printed >= t_key_t1) {
				
        printkey();
        t_key_printed = millis();
        sender_step++;
				
      }
		
    break;
		
    case 2:
		
      if(millis() - t_key_printed >= t_key_t2) {
      
        printkey();
	t_key_printed = millis();
				
      }		
		
    break;
		
  }
	
}

void light() {
	  
  if(millis() - t_timemout_previous >= t_timeout) {
    
	if(!timeout) {
		
	  timeout = true;
	  leds_step = 1;
          ramp = 0;
          	
	}
		
  }

  switch(leds_step) {
		
    case 0:
		
      t_leds_previous = millis();
      ramp = 0;
      leds_step++;
			
    break;
		
    case 1:
		
      if(millis() - t_leds_previous >= t_leds_delay) {
		
        if(ramp == 1) {
			
	  brightness++;       
		
        } else {
		
	  brightness--;
		   
          }
                
        if(brightness == 255) ramp = 0;
        if(brightness == 5) ramp = 1;        
        t_leds_previous = millis();
				
      }
		
    break;
    
    case 2:
      
      brightness = 255;
      leds_step++;
    
    break;
	
  }
  
  analogWrite(leds, brightness);
  
}
	

