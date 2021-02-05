void loop(){
  serial.update();

  //declare the bundle
  OSCBundle bndl1;
  osctime_t timetag; 
     

////////////////////////
/// INICIAR ROTARY ////
///////////////////////
//  temp=my_encoder1.getKey(); // Use phi_keypads object to access the keypad
  temp_rota= int(dial1->getKey()); // Use the phi_interfaces to access the same keypad
 // temp_rota= int(temp_rota_char);
  //t_ang_rota = rotary.get_angle();




/////////////////////
///MODOS DE BOTON//////
/////////////////////
// Update button state
  but.Update(); 
  if (but.clicks != 0) butState = but.clicks; // Save click codes in butstate, as click codes are reset at next Update()
  unsigned long currentMillis = millis(); //guardar millis para varios 
  
//////MODO COLOR DEEP 2 CLICKS///////
  if (butState == 2) modo_but = col_deep;
//////MODO TRASTES 3 CLICKS//////
  if (butState == 3) modo_but = afterM;
//////MODO VAL_SUBAJ 2CLICKSHOLD//////
  if (butState == -2) modo_but = val_subaj;
//////MODO PICK 3CLICKSHOLD///////
  if (butState == -3) modo_but = pick;

  
  //if (deep_st== true) subaj
  switch (modo_but){
    //////////////////////////////
    //// MODO COLORES 1 CLICK ////
    //////////////////////////////
    case col_rotador :  
      deep_st= false;
      if (but.clicks == 1) {//cada vez click rota var color y marca que cambio' con blanco
        color++;
        modo_but = col_rotador;
        if (color > 2) {
           color = 0;
        }
      }
      if (temp_rota!=NO_KEY) {
        if (temp_rota == 2){ //PA ABAJO          
          //pre_Mil_baj = currentMillis;// save the last time you bajar
          if (est_col[color]<= 0) est_col[color]= 0; // limite por abajo 0
          else  est_col[color]= est_col[color] - subaj[color]; // resta con subaj
        }else { //PA ARRIBA
          //pre_Mil_sub = currentMillis;// save the last time you subir
          if (est_col[color]>= 1024) est_col[color]= 1024;// limite por arriba 1024
          else  est_col[color]= est_col[color] + subaj[color]; // suma con subaj
        }
        
        strcpy(direccionp, "/palito/rotador");
        strcat(direccionp, numToOSCAddress(color));
        bndl1.add(direccionp).add((int)est_col[color]).add((int)subaj[color]);
        //numero de rotador (color) y valor (est_col[color]) para irse a OSC
        
      }
      
    break;

    /////////////////////////////////
    //// MODO COLOR DEEP 2 CLICKS ////
    /////////////////////////////////
    case col_deep :
      deep_st= true;
      if (butState == -1){ //devolverse a colors con hold
        
        modo_but = 1;
        deep_st = false;
        break;
      }
      //if (butState == -3) modo_but = val_subaj;  
      if (but.clicks == 1) {//cada vez click rota color_deep y se devuelve en 5
        color_deep++;
        modo_but = col_deep;
        
        if (color_deep > 5) {
           color_deep = 0;
        }
      }
      if (temp_rota!=NO_KEY) {
        if (temp_rota == 2){ //PA ABAJO          
          //pre_Mil_baj = currentMillis;// save the last time you bajar
          if (est_col_deep[color_deep]<= 0) est_col_deep[color_deep]= 0; // limite por abajo 0
          else  est_col_deep[color_deep]= est_col_deep[color_deep] - subaj[color_deep+3]; // resta con subaj
        }else { //PA ARRIBA
          //pre_Mil_sub = currentMillis;// save the last time you subir
          if (est_col_deep[color_deep]>= 1024) est_col_deep[color_deep]= 1024;// limite por arriba 1024
          else  est_col_deep[color_deep]= est_col_deep[color_deep] + subaj[color_deep+3]; // suma con subaj
        }
        
        strcpy(direccionp, "/palito/rotaD");
        strcat(direccionp, numToOSCAddress(color_deep));
        bndl1.add(direccionp).add((int)est_col_deep[color_deep]).add((int)subaj[color_deep+3])/*.add(timetag)*/;
        //numero de rotadordeep + valor de su estado + valor de su subaj para irse a OSC
        
      }
      
    break;
    //////////////////////////////////
    //////MODO AFTERHOLD 3 CLICKS///////moverse entre los trastes y seleccionar
    /////////////////////////////////// 
    case afterM :   
      if (butState == -1){ //devolverse a colors con hold
        modo_but = 1;
        break;
      }
      zul_actual = ver_actual = 255; // color cuando no, POR DEFECTO ESTO ESTAA
      roj_actual = 128;
      //last_rota = temp_rota; // guarda el 'ultimo de rota 
      if (temp_rota!=NO_KEY) {
        
        if (temp_rota == 2){ //PA ABAJO          
          if (afterHold <= 50) afterHold= 50; // limite por abajo 25
          else  afterHold = afterHold - 50; // resta con 25
        }else { //PA ARRIBA
          if (afterHold >= 10000) afterHold = 10000;// limite por arriba 10000
          else  afterHold = afterHold + 50; // suma con 25
        }
        bndl1.add("/palito/afterHold").add(afterHold);//enviar valor de hold
      } 
      if (but.clicks == 1){
        pre_Mil_but = currentMillis;// save the last time you pressed
        after_ON = !after_ON;
        //trasterio.padOn = !trasterio.padOn;
        bndl1.add("/palito/panic").add(after_ON);//que env'ie el mensaje OSC solo si es 1 
                                                    //porque no queremos estar todo el tiempo 
                                                    //enviando mensajes babosos
        //prueba 
      } 
      //print colors in red...
      roj_actual = map ( afterHold ,50,10000,50,200);
      zul_actual = map ( afterHold ,50,10000,100,200);
      ver_actual = map ( afterHold ,50,10000,150,200);
      if (currentMillis - pre_Mil_but <= interval) {  // si todavia intervalo desde que se
        if (after_ON){
          
          roj_actual = 0;
          ver_actual = 150;                      // BLANCO rojo ON
          zul_actual = 36; 
        }else {
          
          
          zul_actual = ver_actual = 255;                   // rojo OSCURO OFF
          roj_actual = 200; 
        }
      }
      
    break;

    //////////////////////////////////////////
    //////MODO VAL_SUBAJ 2CLICKSHOLD//////
    ///////////////////////////////////////////
    case val_subaj ://subaj es variable de + o - para los color-rotadores (qué tan rápido cambia)
      if (butState == -1){ //devolverse a colors con hold
        //color=0;
        if (deep_st == true) modo_but = 2;
        else modo_but = 1;
        break;
      }

      if (deep_st == true) co_parse = color_deep+3; //articular subaj para colors o colordeep
      else co_parse= color;

      if (temp_rota!=NO_KEY) {
        if (temp_rota == 2){ //PA ABAJO
          
          if (subaj[co_parse] <= 16) {
            subaj[co_parse] = 16; // limite 0.015625 16
            
          }
          else {
            subaj[co_parse]= subaj[co_parse] /2; // multiplos de 2 con respecto a 64 comienzo
            
          }
        }else { //PA ARRIBA
          
          if (subaj[co_parse] >= 1024) {
            subaj[co_parse] = 1024; // limite 1 1024//1024 1 512 0.5 256 0.25 128 0.125 64 0.0625 32 0.03125 16 0.015625
            
          }
          else {
            subaj[co_parse]= subaj[co_parse] *2; // multiplos de 2 con respecto a 0.0625 comienzo
            
          }
        }
        //est_col[color]=[co_parse]=
      }   
    break;

    //////////////////////////////////
    //////MODO PICK 3CLICKSHOLD ////// boton de pick prendido o apagado
    //////////////////////////////////  
    case pick :  
      if (butState == -1){ //devolverse a colors con hold
        //color=0;
        modo_but = 1;
        break;
      } 
      if (temp_rota!=NO_KEY) {
        if (temp_rota == 2){ //PA ABAJO
          pick_ON = false;
          
          bndl1.add("/palito/pick").add(0); //guardar que pick apagado a OSC
        }else { //PA ARRIBA
          pick_ON = true;
          
          bndl1.add("/palito/pick").add(1); //guardar que pick prendido a OSC
        }
      }
      if (pick_ON) {
        roj_actual = 140; //BLANCO
        ver_actual = 0;
        zul_actual = 0;}
      else {
        roj_actual =200; // GRIS
        ver_actual =127;
        zul_actual =127;
      }
    break;       
  }

  ////PARA LOS COLORES////
  
  if (deep_st && (modo_but == col_deep || modo_but == val_subaj)) {
    switch (color_deep) {
    //1024 1 512 0.5 256 0.25 128 0.125 64 0.0625 32 0.03125 16 0.015625 
      case 0: //violeta
        
        if (subaj [color_deep+3] == 1024) {
          roj_actual = 253; 
          ver_actual = 255;
          zul_actual = 240;
          
        } else if (subaj [color_deep+3] == 512) {
          roj_actual = 247; 
          ver_actual = 255;
          zul_actual = 200;
          
        } else if (subaj [color_deep+3] == 256) {
          roj_actual = 240; 
          ver_actual = 255;
          zul_actual = 151;
          
        } else if (subaj [color_deep+3] == 128) {
          roj_actual = 230; 
          ver_actual = 255;
          zul_actual = 128;
          
        } else if (subaj [color_deep+3] == 64) {
          roj_actual = 215; 
          ver_actual = 249;
          zul_actual = 50;
          
        } else if (subaj [color_deep+3] == 32) {
          roj_actual = 215; 
          ver_actual = 230;
          zul_actual = 50;
          
        } else if (subaj [color_deep+3] == 16) {
          roj_actual = 205; 
          ver_actual = 220;
          zul_actual = 0;
          
        }
      break;
      case 1: // azul violeta
        
        if (subaj [color_deep+3] == 1024) {
          roj_actual = 253; 
          ver_actual = 255;
          zul_actual = 186;
          
        } else if (subaj [color_deep+3] == 512) {
          roj_actual = 252; 
          ver_actual = 255;
          zul_actual = 150;
          
        } else if (subaj [color_deep+3] == 256) {
          roj_actual = 251; 
          ver_actual = 255;
          zul_actual = 100;
          
        } else if (subaj [color_deep+3] == 128) {
          roj_actual = 247; 
          ver_actual = 255;
          zul_actual = 30;
          
        } else if (subaj [color_deep+3] == 64) {
          roj_actual = 245; 
          ver_actual = 240;
          zul_actual = 30;
          
        } else if (subaj [color_deep+3] == 32) {
          roj_actual = 242; 
          ver_actual = 230;
          zul_actual = 10;
          
        } else if (subaj [color_deep+3] == 16) {
          roj_actual = 235; 
          ver_actual = 210;
          zul_actual = 0;
          
        }
      break;
      case 2: // azul
        
        if (subaj [color_deep+3] == 1024) {
          roj_actual = 255; 
          ver_actual = 255;
          zul_actual = 210;
          
        } else if (subaj [color_deep+3] == 512) {
          roj_actual = 255; 
          ver_actual = 250;
          zul_actual = 150;
          
        } else if (subaj [color_deep+3] == 256) {
          roj_actual = 255; 
          ver_actual = 255;
          zul_actual = 50;
          
        } else if (subaj [color_deep+3] == 128) {
          roj_actual = 255; 
          ver_actual = 255;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 64) {
          roj_actual = 253; 
          ver_actual = 240;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 32) {
          roj_actual = 250; 
          ver_actual = 230;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 16) {
          roj_actual = 246; 
          ver_actual = 225;
          zul_actual = 0;
          
        }
      break;
      case 3: //casi cyan
        
        if (subaj [color_deep+3] == 1024) {
          roj_actual = 255; 
          ver_actual = 225;
          zul_actual = 100;
          
        } else if (subaj [color_deep+3] == 512) {
          roj_actual = 255; 
          ver_actual = 200;
          zul_actual = 35;
          
        } else if (subaj [color_deep+3] == 256) {
          roj_actual = 255; 
          ver_actual = 175;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 128) {
          roj_actual = 255; 
          ver_actual = 150;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 64) {
          roj_actual = 248; 
          ver_actual = 135;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 32) {
          roj_actual = 240; 
          ver_actual = 125;
          zul_actual = 0;
          
        } else if (subaj [color_deep+3] == 16) {
          roj_actual = 232; 
          ver_actual = 130;
          zul_actual = 0;
          
        }
      break;
      case 4: // verde
        
        if (subaj [color_deep+3] == 1024) {
          roj_actual = 255; 
          ver_actual = 200;
          zul_actual = 212;
          
        } else if (subaj [color_deep+3] == 512) {
          roj_actual = 255; 
          ver_actual = 128;
          zul_actual = 200;
          
        } else if (subaj [color_deep+3] == 256) {
          roj_actual = 255; 
          ver_actual = 52;
          zul_actual = 200;
          
        } else if (subaj [color_deep+3] == 128) {
          roj_actual = 255; 
          ver_actual = 0;
          zul_actual = 152;
          
        } else if (subaj [color_deep+3] == 64) {
          roj_actual = 235; 
          ver_actual = 0;
          zul_actual = 102;
          
        } else if (subaj [color_deep+3] == 32) {
          roj_actual = 220; 
          ver_actual = 0;
          zul_actual = 52;
          
        } else if (subaj [color_deep+3] == 16) {
          roj_actual = 200; 
          ver_actual = 0;
          zul_actual = 0;
          
        }
      break;
      case 5: // verde limon
        
        if (subaj [color_deep+3] == 1024) {
          roj_actual = 250; 
          ver_actual = 200;
          zul_actual = 255;
          
        } else if (subaj [color_deep+3] == 512) {
          roj_actual = 245; 
          ver_actual = 161;
          zul_actual = 255;
          
        } else if (subaj [color_deep+3] == 256) {
          roj_actual = 225; 
          ver_actual = 50;
          zul_actual = 255;
          
        } else if (subaj [color_deep+3] == 128) {
          roj_actual = 215; 
          ver_actual = 0;
          zul_actual = 255;
          
        } else if (subaj [color_deep+3] == 64) {
          roj_actual = 200; 
          ver_actual = 0;
          zul_actual = 200;
          
        } else if (subaj [color_deep+3] == 32) {
          roj_actual = 200; 
          ver_actual = 0;
          zul_actual = 152;
          
        } else if (subaj [color_deep+3] == 16) {
          roj_actual = 190; 
          ver_actual = 0;
          zul_actual = 100;
          
        }
      break;       
    }

  } else if (modo_but == col_rotador || modo_but == val_subaj) {
    switch (color) {
    //1024 1 512 0.5 256 0.25 128 0.125 64 0.0625 32 0.03125 16 0.015625 
      case 0: //vermellon
        if (subaj [color] == 1024){
          
          roj_actual = 200; 
          ver_actual = 200;
          zul_actual = 255;
        } else if (subaj [color] == 512) {
          roj_actual = 170; 
          ver_actual = 185;
          zul_actual = 255;
          
        } else if (subaj [color] == 256) {
          roj_actual = 100; 
          ver_actual = 150;
          zul_actual = 255;
          
        } else if (subaj [color] == 128) {
          roj_actual = 0; 
          ver_actual = 130;
          zul_actual = 255;
          
        } else if (subaj [color] == 64) {
          roj_actual = 0; 
          ver_actual = 80;
          zul_actual = 200;
          
        } else if (subaj [color] == 32) {
          roj_actual = 0; 
          ver_actual = 50;
          zul_actual = 120;
          
        } else if (subaj [color] == 16) {
          roj_actual = 0; 
          ver_actual = 0;
          zul_actual = 25;
          
        }
      break;
      case 1: // naranja
        
        if (subaj [color] == 1024) {
          roj_actual = 200; 
          ver_actual = 177;
          zul_actual = 255;
          
        } else if (subaj [color] == 512) {
          roj_actual = 150; 
          ver_actual = 100;
          zul_actual = 255;
          
        } else if (subaj [color] == 256) {
          roj_actual = 100; 
          ver_actual = 50;
          zul_actual = 255;
          
        } else if (subaj [color] == 128) {
          roj_actual = 70; 
          ver_actual = 0;
          zul_actual = 255;
          
        } else if (subaj [color] == 64) {
          roj_actual = 70; 
          ver_actual = 0;
          zul_actual = 170;
          
        } else if (subaj [color] == 32) {
          roj_actual = 70; 
          ver_actual = 0;
          zul_actual = 100;
          
        } else if (subaj [color] == 16) {
          roj_actual = 82; 
          ver_actual = 0;
          zul_actual = 0;
          
        }
      break;
      case 2: // amarillo
        
        if (subaj [color] == 1024) {
          roj_actual = 220; 
          ver_actual = 165;
          zul_actual = 255;
          
        } else if (subaj [color] == 512) {
          roj_actual = 200; 
          ver_actual = 130;
          zul_actual = 255;
          
        } else if (subaj [color] == 256) {
          roj_actual = 170; 
          ver_actual = 35;
          zul_actual = 255;
          
        } else if (subaj [color] == 128) {
          roj_actual = 150; 
          ver_actual = 0;
          zul_actual = 255;
          
        } else if (subaj [color] == 64) {
          roj_actual = 150; 
          ver_actual = 0;
          zul_actual = 200;
          
        } else if (subaj [color] == 32) {
          roj_actual = 150; 
          ver_actual = 0;
          zul_actual = 150;
          
        } else if (subaj [color] == 16) {
          roj_actual = 150; 
          ver_actual = 0;
          zul_actual = 100;
          
        }
      break;  
    }
  }
  



  analogWrite(redPin, roj_actual); //actualizar los leds pa todo
  analogWrite(greenPin, ver_actual);
  analogWrite(bluePin, zul_actual);
  
  
  //SLIPSerial.beginPacket();
  bndl1.setTimetag(oscTime());
  //bndl1.send(serial);
  bndl1.send(printer);
  serial.send(b, printer.getOffset());
  //serial.endPacket();
  //bndl1.send(SLIPSerial); // send the bytes to the SLIP stream
  //SLIPSerial.endPacket(); // mark the end of the OSC Packet
  bndl1.empty(); // empty the bundle to free room for a new one
    
  yield();
} 
