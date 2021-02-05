    
void loop2(){
  serial.update();
 //declare the bundle
  OSCBundle bndl3;
  osctime_t timetag;    
  ///Frets all///
  // Fills trasterio.key[ ] array with up-to 14 active keys.
    // Returns true if there are ANY active keys.
    if (trasterio.getKeys()){
      //int base = 
      for (int i=0; i<LIST_MAX; i++) {  // Scan the whole key list for one fret   
        
        for (int j=0; j<LIST_MAX; j++) {  // Scan the whole key list for the other fret
          if (trasterio.key[i].kchar != 98 || trasterio.key[i].kchar != 0 || 
              trasterio.key[j].kchar != 98 || trasterio.key[j].kchar != 0){ //sean validos
            if ( trasterio.key[i].stateChanged || trasterio.key[j].stateChanged ) {  // Only find keys that have changed state.
             //como hace la lista en que orden, necesito evaluaar i con respecto a i+1 o i-1 talvez 
             //pero depende de como es la lista... debe ser lineal, entonces se necesita algo tipo x + y*ancho , de pixels 
                for (int k=11; k<17 ;k++){       //goes from fret 21 till 26, doing +10 on m for
                  for(int m=k ; m< 247 ;m+=10){  // so it goes from fret 2x till 24x
                             // it is to test based on the +10 if two adjacent frets are pressed
                    if ( (trasterio.key[i].kchar == m ) && (trasterio.key[j].kchar == m+10 )){
                      if(trasterio.key[i].kstate == trasterio.key[j].kstate){ // both on same state
                        //then based on state send the osc message from the lower fret
                        switch (trasterio.key[j].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                            case PRESSED:
                            msg = 1;
                            after = 0;
                        break;
                            case HOLD:
                            msg = 5;
                            after = Entropy.random(1024);
                        break;
                            case RELEASED:
                            msg = 0;
                            after = 0;
                        break;
                            case IDLE:
                            msg = 0;
                            after = 0;
                        }
                        String dir_trastes = "/palito/trastes/" ;
                        int tecla = (trasterio.key[j].kchar); //**no eswta pasando el traste 21, lo deja 11
                        dir_trastes.concat (tecla); 
                        char dirtrastes [20];
                        dir_trastes.toCharArray(dirtrastes,20);
                        bndl3.add(dirtrastes).add(msg).add(after);
                      }
                      else if ((trasterio.key[i].kstate == RELEASED)||(trasterio.key[i].kstate == IDLE)) {
                        msg = 0;
                        after = 0;
                        String dir_trastes = "/palito/trastes/" ;
                        int tecla = (trasterio.key[i].kchar); //**no eswta pasando el traste 21, lo deja 11
                        dir_trastes.concat (tecla); 
                        char dirtrastes [20];
                        dir_trastes.toCharArray(dirtrastes,20);
                        bndl3.add(dirtrastes).add(msg).add(after);
                    
                      }
                      else if((trasterio.key[j].kstate == RELEASED)||(trasterio.key[j].kstate == IDLE)){
                        msg = 0;
                        after = 0;
                        String dir_trastes = "/palito/trastes/" ;
                        int tecla = (trasterio.key[j].kchar); //**no eswta pasando el traste 21, lo deja 11
                        dir_trastes.concat (tecla); 
                        char dirtrastes [20];
                        dir_trastes.toCharArray(dirtrastes,20);
                        bndl3.add(dirtrastes).add(msg).add(after);
                      }
                    }
                    ///solucion crear un estado adicional en la libreria de ONLYONE y TWO y desarrollarles el paso, tal
                    /// y que cada uno su released y los otros estados... se resuelve es con la logica del oprimir y soltar...
                    ///la teoria actual del over rojo es por exceso de mensajes midi... 
                  }
                }
             
            }
          }
        }
      }
    }
  //SLIPSerial.beginPacket();
  bndl3.setTimetag(oscTime());
  //bndl3.send(serial);
  bndl3.send(printer);
  serial.send(b, printer.getOffset());
  //serial.endPacket();
  //bndl3.send(SLIPSerial); // send the bytes to the SLIP stream
  //SLIPSerial.endPacket(); // mark the end of the OSC Packet
  bndl3.empty(); // empty the bundle to free room for a new one

  yield();
}
