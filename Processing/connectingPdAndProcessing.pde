import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

void setup() {
size(400,400);
frameRate(25);
/* start oscP5, listening for incoming messages at port 9000 */
oscP5 = new OscP5(this,9000);

myRemoteLocation = new NetAddress("127.0.0.1",12000);
}

void draw() {
background(0);
}


void oscEvent(OscMessage theOscMessage) {

if(theOscMessage.checkAddrPattern("/first")==true) {
if(theOscMessage.checkTypetag("i")) {

int firstValue = theOscMessage.get(0).intValue();
println(" values: "+firstValue);
return;
}
}
}
