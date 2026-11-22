// Garagentor: Offline-Logik. I9 muss fuer Ausgaenge freigegeben sein.
// I1/I2 Endlagen, I3 Motorspannung. I4 AUF, I5 ZU, I6 STOP.
// I7 Referenzfahrt, I8 Teilziel-Taste, I9 Freigabe, I10 Wunschposition.
// I11/I12 gespeicherte AUF-/ZU-Zeit in Sekunden; O7/O8 neue Messwerte.
// O1/O2 Impuls AUF/ZU, O3 Zustand, O4 Position (-1 unbekannt).
// O5 Referenzschritt, O6 Fehler, O9 externe Fahrt, O10 Teilziel erlaubt.
int tick=0, rise=0, started=0, off=0, pending=0, waiting=0;
int dir=0, origin=0, previousOrigin=0, external=0, ref=0, fault=0;
int expected=0, pulse=0, pulseUntil=0, next=0, lastMotor=0;
int oldUp=0, oldDown=0, oldStop=0, oldRef=0, oldTarget=0;
int stopStarted=0, needsReference=0;
int phase=0, homeFirst=0, traceable=0;
float pos=-1, target=-1, nextTarget=-1, upTime=0, downTime=0;
int limitUp,limitDown,motor,up,down,stop,learn,preset,enable;
int upEdge,downEdge,stopEdge,learnEdge,presetEdge,selected;
float wanted,elapsed,travel;
int timing, impulseTicks, stillTicks, startTicks, stopTicks, marginTicks;
while(1) {
  limitUp=getinput(0)>0.5; limitDown=getinput(1)>0.5; motor=getinput(2)>0.5;
  up=getinput(3)>0.5; down=getinput(4)>0.5; stop=getinput(5)>0.5;
  learn=getinput(6)>0.5; preset=getinput(7)>0.5; enable=getinput(8)>0.5;
  wanted=getinput(9);
  timing=(int)getinput(12);
  impulseTicks=(timing%32)*10;
  stillTicks=((timing/32)%32)*10;
  startTicks=((timing/1024)%32)*10;
  stopTicks=((timing/32768)%32)*10;
  marginTicks=((timing/1048576)%16)*40;
  if(!impulseTicks || !stillTicks || !startTicks || !stopTicks) enable=0;
  if(getinput(10)>0 && upTime<=0) upTime=getinput(10);
  if(getinput(11)>0 && downTime<=0) downTime=getinput(11);
  upEdge=up&&!oldUp; downEdge=down&&!oldDown; stopEdge=stop&&!oldStop;
  learnEdge=learn&&!oldRef; presetEdge=preset&&!oldTarget;
  oldUp=up; oldDown=down; oldStop=stop; oldRef=learn; oldTarget=preset;
  next=0; selected=0;
  if(tick>=pulseUntil) pulse=0;
  if(!motor && off<stillTicks) off++;
  if(motor) off=0;
  origin=0;
  if(limitUp&&!limitDown) origin=1;
  if(limitDown&&!limitUp) origin=-1;
  if(origin==1) pos=100;
  if(origin==-1) pos=0;
  if(motor&&!lastMotor&&!phase) {
    rise=tick; traceable=previousOrigin;
    external=0;
    if(expected) {dir=expected; expected=0;}
    else {dir=-previousOrigin; external=2; if(dir) external=dir;}
  }
  if(motor && lastMotor && !phase && !origin && dir && pos>=0) {
    travel=dir==1?upTime:downTime;
    if(travel>0) pos=pos+dir*5.0/travel;
    if(pos>=100) pos=99.9;
    if(pos<=0) pos=0.1;
  }
  if(lastMotor&&!motor) {started=tick; phase=1;}
  if(phase && motor && !lastMotor) {
    phase=0; dir=0; pos=-1; traceable=0; external=2;
    if(!fault) fault=2;
    ref=0; pending=0; target=-1; homeFirst=0;
  }
  if(limitUp&&limitDown) {
    fault=5; pos=-1; ref=0; pending=0; target=-1; homeFirst=0;
  }
  if(fault==5 && !(limitUp&&limitDown) && off>=stillTicks) fault=0;
  if(stopEdge) {
    if(ref) fault=1;
    ref=0; pending=0; target=-1; homeFirst=0;
    if(motor && !waiting) { next=1; waiting=1; stopStarted=tick; }
    expected=0;
  }
  if(phase&&off>=stillTicks) {
    phase=0;
    if(origin && traceable==-origin && dir==origin) {
      elapsed=(started-rise)*0.05;
      if(origin==1) upTime=elapsed; else downTime=elapsed;
    }
    traceable=0;
    if(origin && !needsReference) fault=0;
    if(!waiting && !origin) {
      if(!fault) fault=2;
      ref=0; pending=0; target=-1; homeFirst=0;
    }
    if(!waiting && origin && ref) {
      if(ref==1) {ref=2; pending=-origin;}
      else {ref=0; fault=0; needsReference=0;}
    }
    if(!waiting && origin==1 && homeFirst) {
      homeFirst=0; pending=-1;
    }
    dir=0;
  }
  if(waiting && off>=stillTicks) waiting=0;
  if(waiting && tick-stopStarted>=stopTicks) {pending=0; target=-1; homeFirst=0; ref=0; expected=0; traceable=0; pos=-1; dir=0; fault=6;}
  if(expected && tick-rise>=startTicks) {
    expected=0; ref=0; pending=0; target=-1; homeFirst=0; fault=3;
  }
  if(motor && !waiting && dir) {
    travel=dir==1?upTime:downTime;
    if(travel>0 && tick-rise>travel*20+marginTicks) {
      next=1; waiting=1; stopStarted=tick; fault=4;
      ref=0; pending=0; target=-1; homeFirst=0;
    }
    if(!waiting && !homeFirst && target>=1 && pos>=0 &&
       ((dir==1 && pos>=target)||(dir==-1 && pos<=target))) {
      next=1; waiting=1; stopStarted=tick; target=-1;
    }
  }
  if(!stopEdge && !waiting && !phase && !expected && !ref && !(limitUp&&limitDown)) {
    if(upEdge&&!downEdge) selected=1;
    if(downEdge&&!upEdge) selected=-1;
    if(motor && selected && selected!=dir) {
      next=selected; pending=selected; target=-1; waiting=1; stopStarted=tick;
    }
    if(!motor && off>=stillTicks) {
      if(learnEdge && origin) {ref=1; pending=-origin; target=-1; fault=0;}
      else if(presetEdge && upTime>0 && downTime>0 && wanted>=1 && wanted<=99) {
        target=wanted;
        if(pos<0) {homeFirst=1; pending=1;}
        else if(pos<target) pending=1;
        else if(pos>target) pending=-1;
      }
      else if(selected && selected!=origin) {pending=selected; target=-1;}
    }
  }
  if(!stopEdge && !waiting && !motor && off>=stillTicks && pending && !expected &&
     !(limitUp&&limitDown) && !pulse) {
    expected=pending; next=pending; pending=0; rise=tick;
  }
  if(enable && next) {pulse=next; pulseUntil=tick+impulseTicks;}
  if(!enable) {pulse=0; expected=0; pending=0; ref=0; homeFirst=0; target=-1;}
  if(fault && fault!=5 && (upTime<=0 || downTime<=0)) needsReference=1;
  setoutput(0,pulse==1); setoutput(1,pulse==-1);
  setoutput(2,motor?dir*2:origin); setoutput(3,pos);
  setoutput(4,ref); setoutput(5,fault); setoutput(6,upTime); setoutput(7,downTime);
  setoutput(8,external); setoutput(9,upTime>0&&downTime>0&&!motor&&!waiting&&!phase&&!expected&&!ref&&!(limitUp&&limitDown));
  lastMotor=motor; previousOrigin=origin;
  tick++; sleep(50);
}
