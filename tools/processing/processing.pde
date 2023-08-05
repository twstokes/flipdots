import hypermedia.net.*;

UDP udp;

PGraphics canvas;

int panels = 2;
int cols_per_panel = 28;
int dots_per_col = 7;

// ball
int rad = 2;        // Width of the shape
float xpos, ypos;    // Starting position of shape    

float xspeed = 1;  // Speed of the shape
float yspeed = 1;  // Speed of the shape

int xdirection = 1;  // Left or Right
int ydirection = 1;  // Top to Bottom
// ball
 
void setup() {
  size(840, 420);
  canvas = createGraphics(28, 14);
  noSmooth();
  frameRate(6);
  //noFill();
  ellipseMode(RADIUS);
  //canvas.ellipseMode(RADIUS);
  //canvas.noStroke();
  //canvas.noFill();
  udp = new UDP(this, 4210);
  //udp.log(true);
  xpos = canvas.width/2;
  ypos = canvas.height/2;
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
  xpos = xpos + ( xspeed * xdirection );
  ypos = ypos + ( yspeed * ydirection );
  
  // Test to see if the shape exceeds the boundaries of the screen
  // If it does, reverse its direction by multiplying by -1
  if (xpos > canvas.width-rad || xpos < rad) {
    xdirection *= -1;
  }
  if (ypos > canvas.height-rad || ypos < rad) {
    ydirection *= -1;
  }

  // Draw the shape
  //canvas.ellipse(xpos, ypos, rad, rad);
  canvas.circle(xpos, ypos, rad);
  
  
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
  udp.send(test, "IP address here", 4210);

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
