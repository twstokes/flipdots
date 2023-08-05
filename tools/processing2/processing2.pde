import hypermedia.net.*;

UDP udp;

PGraphics canvas;

int panels = 2;
int cols_per_panel = 28;
int dots_per_col = 7;

float rotate = 0;

void setup() {
  size(840, 420, P3D);
  noSmooth();
  ((PGraphicsOpenGL)g).textureSampling(3);
  canvas = createGraphics(28, 14, P3D);
  frameRate(3);
  //noFill();
  ellipseMode(RADIUS);
  //canvas.ellipseMode(RADIUS);
  //canvas.noStroke();
  //canvas.noFill();
  udp = new UDP(this, 4210);
}
 
void draw() {
  canvas.beginDraw();
  canvas.ellipseMode(RADIUS);
  canvas.noFill();
  canvas.background(0);
  canvas.noSmooth();
  canvas.stroke(255);
  //canvas.line(0, yCoord, 27, 13);
  //canvas.set(0, 2, color(255));
   // Update the position of the shape

  canvas.translate(canvas.width / 2, canvas.height / 2, 0); 
  canvas.rotateZ(rotate);
  //canvas.rotateX(rotate);
  canvas.box(10);
  
  canvas.loadPixels();
  canvas.updatePixels();
  canvas.endDraw();
  byte[] test = byteArrayFromPixels(canvas.pixels);
  //println(test.length); // should be 56
  //println(test[0]); // should be 1
  //println(test[1]); // should be 1
  //println(test[2]); // should be 2
  //println(test[55]); // should be 64
  image(canvas, 0, 0, width, height); // draw canvas streched to sketch dimensions
  //delay(1000);
  //udp.send(test, "IP ADDRESS HERE", 4210);
  
  rotate += 0.3;
  
  if(rotate >= PI * 2) {
    rotate = 0; 
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
