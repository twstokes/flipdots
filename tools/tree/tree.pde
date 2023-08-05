import hypermedia.net.*;

UDP udp;

PGraphics canvas;

int panels = 2;
int cols_per_panel = 28;
int dots_per_col = 7;

float theta;   

float r = 1;
float a = 0;

void setup() {
  size(840, 420);
  noSmooth();
  canvas = createGraphics(28, 14);
  frameRate(6);
  ellipseMode(RADIUS);
  udp = new UDP(this, 4210);
}
 
void draw() {
  canvas.beginDraw();
  canvas.ellipseMode(RADIUS);
  canvas.noFill();
  canvas.background(0);
  canvas.noSmooth();
  canvas.stroke(255);

  //canvas.circle(canvas.width / 2, canvas.height / 2, r);
  //float a = (mouseX / (float) width) * 90f;
    // Convert it to radians
  theta = radians(a);
  // Start the tree from the bottom of the screen
  canvas.translate(canvas.width/2, canvas.height);
  // Draw a line 120 pixels
  canvas.line(0,0,0,-2);
  // Move to the end of that line
  translate(0,-2);
  // Start the recursive branching!
  branch(10);
  
  
  canvas.loadPixels();
  canvas.updatePixels();
  canvas.endDraw();

  byte[] test = byteArrayFromPixels(canvas.pixels);
  image(canvas, 0, 0, width, height); // draw canvas streched to sketch dimensions
  //delay(1000);
  udp.send(test, "IP ADDRESS HERE", 4210);
  
  r += 1;
  
  if(r >= 10) {
    r = 0; 
  }
  
  //println(a);
  a++;
  
  if(a >50) {
    a = 0;
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

void branch(float h) {
  // Each branch will be 2/3rds the size of the previous one
  h *= 0.66;
  
  // All recursive functions must have an exit condition!!!!
  // Here, ours is when the length of the branch is 2 pixels or less
  if (h > 1) {
    canvas.pushMatrix();    // Save the current state of transformation (i.e. where are we now)
    canvas.rotate(theta);   // Rotate by theta
    canvas.line(0, 0, 0, -h);  // Draw the branch
    canvas.translate(0, -h); // Move to the end of the branch
    branch(h);       // Ok, now call myself to draw two new branches!!
    canvas.popMatrix();     // Whenever we get back here, we "pop" in order to restore the previous matrix state
    
    // Repeat the same thing, only branch off to the "left" this time!
    canvas.pushMatrix();
    canvas.rotate(-theta);
    canvas.line(0, 0, 0, -h);
    canvas.translate(0, -h);
    branch(h);
    canvas.popMatrix();
  }
}
