import hypermedia.net.*;

UDP udp;

PGraphics canvas;

int panels = 2;
int cols_per_panel = 28;
int dots_per_col = 7;

int lastSend = 0;

float diameter; 
float angle = 0;

void setup() {
  size(840, 420);
  noSmooth();
  canvas = createGraphics(28, 14);
  //frameRate(20);
  fill(255, 0, 0);
  //ellipseMode(RADIUS);
  udp = new UDP(this, 4210);
  canvas.beginDraw();
  canvas.background(0);
  canvas.endDraw();
  diameter = 8;
}
 
void draw() {
  
  canvas.beginDraw();
  //canvas.ellipseMode(RADIUS);
  //canvas.noFill();
  canvas.fill(255, 0, 0);
  canvas.noStroke();
  canvas.background(0);
  canvas.noSmooth();
  
  float d1 = 0 + (sin(angle) * diameter/2) + diameter/2;
  float d2 = 0 + (sin(angle + PI/2) * diameter/2) + diameter/2;
  float d3 = 0 + (sin(angle + PI) * diameter/2) + diameter/2;
  
  canvas.ellipse(4, canvas.height/2, d1, d1);
  canvas.ellipse(canvas.width/2, canvas.height/2, d2, d2);
  canvas.ellipse(canvas.width - 4, canvas.height/2, d3, d3);
  
  angle += 0.04;
  
  
  canvas.loadPixels();
  canvas.updatePixels();
  canvas.endDraw();

  
  sendUdpData(byteArrayFromPixels(canvas.pixels));
  image(canvas, 0, 0, width, height); // draw canvas streched to sketch dimensions  
}

// debouncing
// todo - only send if we have new data, currently it's sending the buffer even if nothing changed
void sendUdpData(byte[] data) {
  int debounce = 50; // don't send data faster than this value in milliseconds
  
  int currentTime = millis();
  if (currentTime > lastSend + debounce) {
    udp.send(data, "IP ADDRESS HERE", 4210);
    lastSend = currentTime;
  }
}

// returns a byte with the LSB equalling 0 or 1
byte colorToByte(color c) {
  // colors are 24 bit - we need to dumb it down to a single bit
  // since we only care about all on or all off, the first bit will suffice
  return byte(c >> 23 & 1);
}


// pixels go from left to right, top to bottom
// we need to convert them to top to bottom columns (7), left to right
byte[] byteArrayFromPixels(color[] pixels) {
  // the number of pixels should be panels * cols_per_panel * dots_per_col
  // we're taking those pixels and setting bits on panels * cols_per_panel bytes
  // the MSB bit on each array is ignored, per the docs, because we only have 7 dots (which are represented by 7 bits)
  
  //int p = 0; // index of current pixel
  byte[] array = new byte[cols_per_panel * panels];
  
  for(int p = 0; p < panels; p++) {
    for(int c = 0; c < cols_per_panel; c++) {
      byte colByte = 0;
      
      for(int r = dots_per_col - 1; r >= 0; r--) {
        int index = r * cols_per_panel + c + (p*cols_per_panel*dots_per_col);
        byte pixel = colorToByte(pixels[index]);
        colByte = byte(colByte << 1);
        colByte = byte(colByte | pixel);     
      }
      
      array[p*cols_per_panel + c] = colByte;
    }
  }
  
  return array;
}
