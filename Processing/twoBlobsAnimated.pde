import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;
//blob 1
int x1Value= height/2;
int y1Value= width/2;
int r1Value= 100;
int g1Value=100;
int b1Value=100;

//blob 2
int x2Value= height/2;
int y2Value= width/2;
int r2Value= 100;
int g2Value=100;
int b2Value=100;


void setup() {
size(400,400);
frameRate(25);
/* start oscP5, listening for incoming messages at port 12000 */
oscP5 = new OscP5(this,10000);

myRemoteLocation = new NetAddress("127.0.0.1",10000);
}

void draw() {
//background(0); 
}


void oscEvent(OscMessage theOscMessage) {

  //x positions
if(theOscMessage.checkAddrPattern("/x1pos")==true) {
if(theOscMessage.checkTypetag("i")) {

x1Value = theOscMessage.get(0).intValue(); 
println(" x1pos: "+x1Value);
return;
}
}

///y positions
if(theOscMessage.checkAddrPattern("/y1pos")==true) {
if(theOscMessage.checkTypetag("i")) {

y1Value = theOscMessage.get(0).intValue(); 
println(" y1pos: "+y1Value);
return;
}
}

////r values

if(theOscMessage.checkAddrPattern("/r1")==true) {
if(theOscMessage.checkTypetag("i")) {
r1Value = theOscMessage.get(0).intValue(); 
println(" r1color: "+r1Value);
return;
}
}
///g values

if(theOscMessage.checkAddrPattern("/g1")==true) {
if(theOscMessage.checkTypetag("i")) {

g1Value = theOscMessage.get(0).intValue(); 
println(" g1color: "+g1Value);
return;
}
}
////b values
if(theOscMessage.checkAddrPattern("/b1")==true) {
if(theOscMessage.checkTypetag("i")) {

b1Value = theOscMessage.get(0).intValue(); 
println(" b1color: "+b1Value);

//background(120);

return;
}
}


/*.......blob 2........*/
if(theOscMessage.checkAddrPattern("/x2pos")==true) {
if(theOscMessage.checkTypetag("i")) {

x2Value = theOscMessage.get(0).intValue(); 
println(" x2pos: "+x2Value);
return;
}
}


if(theOscMessage.checkAddrPattern("/y2pos")==true) {
if(theOscMessage.checkTypetag("i")) {

y2Value = theOscMessage.get(0).intValue(); 
println(" y2pos: "+y2Value);
return;
}
}


if(theOscMessage.checkAddrPattern("/r2")==true) {
if(theOscMessage.checkTypetag("i")) {

r2Value = theOscMessage.get(0).intValue(); 
println(" r2color: "+r2Value);
return;
}
}

if(theOscMessage.checkAddrPattern("/g2")==true) {
if(theOscMessage.checkTypetag("i")) {

g2Value = theOscMessage.get(0).intValue(); 
println(" g2color: "+g2Value);
return;
}
}

if(theOscMessage.checkAddrPattern("/b2")==true) {
if(theOscMessage.checkTypetag("i")) {

b2Value = theOscMessage.get(0).intValue(); 
println(" b2color: "+b2Value);

//draw ellipses and refresh background 


}
}

background(120);
fill(r1Value,g1Value,b1Value);
ellipse(x1Value, y1Value,50,50);
fill(r2Value,g2Value,b2Value);
ellipse(x2Value, y2Value,50,50);
line(x1Value,y1Value,x2Value,y2Value);
return;


}
