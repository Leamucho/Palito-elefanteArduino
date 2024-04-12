void loop3(){
  serial.update();

  OSCBundle bndl;
  osctime_t timetag; 
///////////////////////////
/////ANALOG INS ////////////
//////////////////////////

for(byte pin = 0; pin < NUM_ANALOG_INPUTS; pin++){
  //actual_analog[pin]=analogRead(pin);///hacer la lectura
  //piezos_ref [pin] = actual_analog[pin] - last_analog[pin];//hacer la diferencia para abs    
    if ((pin == 0 )||(pin == 7 )||(pin == 2 )){
    
    
    total[pin] = total[pin] - potReadings [pin][readIndex[pin]];     // subtract the last reading:
    potReadings [pin][readIndex[pin]] = analogRead(pin);  // get from actual
    total[pin] = total[pin] + potReadings [pin][readIndex[pin]];      // add the reading to the total:
    readIndex[pin] = readIndex[pin] +1;                      // advance to the next position in the array
  
    if (readIndex[pin] >= numReadings) {          // if we're at the end of the array...
      readIndex[pin] = 0;                         //begin again
    }
    average[pin] = total[pin] / numReadings;          // calculate the average:
    if ( abs(average[pin]-last_analog[pin])>= pinJitter[pin]) {      // revise if the average has changed
      last_analog[pin] = average[pin];                        // save the average if to send
      char direccion[15];
      strcpy(direccion, "/palito/pot");
      strcat(direccion, numToOSCAddress(pin));
      bndl.add(direccion).add((int)(average[pin])); //form the OSC message
    }     
      
  } else if ((pin == 4 )||(pin == 5 )||(pin == 6 )){

  }
}

//SLIPSerial.println (bndl);
//SLIPSerial.beginPacket();
bndl.setTimetag(oscTime());
bndl.send(printer);
serial.send(b, printer.getOffset());
//serial.endPacket();
//bndl.send(SLIPSerial); // send the bytes to the SLIP stream

//SLIPSerial.endPacket(); // mark the end of the OSC Packet

bndl.empty(); // empty the bundle to free room for a new one
    
yield();
}
