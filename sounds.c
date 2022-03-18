

void laserSound(void){
	unsigned int c;
	SID.v2.ad = 0x18;
	SID.v2.sr = 0x25;
	SID.amp   = 0x1F; 
	SID.v2.ctrl  = 0x11; 
	SID.v2.freq  = 0x4B45; //B5 
	for(c=0;c<2500;++c){}
	SID.amp      = 0x00; 
	SID.v2.ctrl  = 0x10;
};


void pingSound(void){
	unsigned int c;
	SID.v2.ad = 0x14;
	SID.v2.sr = 0x25;
	SID.amp   = 0x1F; 
	SID.v2.ctrl  = 0x11; 
	SID.v2.freq  = 0x3A3B; //A5 
	for(c=0;c<4500;++c){}
	SID.amp      = 0x00; 
	SID.v2.ctrl  = 0x10;
}


void pingSoundHigh(void){
	unsigned int c;
	SID.v2.ad = 0x14;
	SID.v2.sr = 0x25;
	SID.amp   = 0x1F; 
	SID.v2.ctrl  = 0x11; 
	SID.v2.freq  = 0x6E97; //B6? 
	for(c=0;c<2500;++c){}
	SID.amp      = 0x00; 
	SID.v2.ctrl  = 0x10;
}


void bangSound(void) 
{ 
	unsigned int c;
	unsigned int d;
	SID.v3.freq  = 0x6800; 
	SID.v3.ad    = 0x88; 
	SID.v3.sr    = 0xC8; 
	SID.flt_freq = 0x5000; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x21; 
	
	for(c=0;c<253;++c) 
	{ 
		SID.v3.freq+=16;
		for(d=0;d<10;++d){};
	}
	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x10; 
}


void zraySound(void){
	unsigned int c,d;
	SID.amp = 0x1F;
	SID.v1.ad  = 0x00;
	SID.v1.sr  = 0xF0;
	SID.v1.ctrl = 0x11;
	for (c=0; c<7; c++){
		for (d=0; d<255; d+=5){
			SID.v1.freq = d*256;
		}
	}
	SID.v1.ctrl = 0x10;
	
}

void explosionSound(void){
	unsigned int c; 
	SID.v3.sr    = 0x6A; 
	SID.v3.ad    = 0x37;
	SID.v3.freq  = 0x4FB; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x21;
	SID.flt_freq = 0x5000; 	
	for(c=0;c<5000;++c) {} 
	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x10; 
}

void bonusSound(void){
	unsigned int c;
	SID.v1.ad    = 0x15; 
	SID.v1.sr    = 0x26; 
	SID.amp      = 0x1F; 
	SID.v1.ctrl  = 0x11; 
	SID.v1.freq  = 0x430F; 	
	for(c=0;c<2000;++c) {} 
	SID.v1.ad    = 0x18;
	SID.v1.freq  = 0x6479; 
	for(c=0;c<3500;++c) {} 
	
	SID.amp      = 0x00; 
	SID.v1.ctrl  = 0x10; 
}

void endLevelSound(void){
	unsigned int c;
	SID.v1.ad    = 0x15; 
	SID.v1.sr    = 0x64; 
	SID.amp      = 0x1F; 
	SID.v1.ctrl  = 0x21; 
	SID.v1.freq  = 0x25A2; 	
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x11;
	SID.v1.freq  = 0x3863; 
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x31;
	SID.v1.freq  = 0x3F4B; 
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x31;
	SID.v1.freq  = 0x470C; 
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x39;
	SID.v1.sr    = 0x6A; 
	SID.v1.freq  = 0x4B45; 
	for(c=0;c<9500;++c) {} 
	SID.amp      = 0x00; 
	SID.v1.ctrl  = 0x10; 
}

void failLevelSound(void){
	unsigned int c;
	SID.v1.ad    = 0x15; 
	SID.v1.sr    = 0x64; 
	SID.amp      = 0x1F; 
	SID.v1.ctrl  = 0x21; 
	SID.v1.freq  = 0x647; 	
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x11;
	SID.v1.freq  = 0x598; 
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x31;
	SID.v1.freq  = 0x547; 
	for(c=0;c<6500;++c) {} 
	SID.v1.sr    = 0x6A; 
	SID.v1.ad    = 0x37;
	SID.v1.freq  = 0x4FB; 
	for(c=0;c<6500;++c) {} 
	SID.amp      = 0x00; 
	SID.v1.ctrl  = 0x10; 
}


void gameOverSound(void){
	unsigned int c;
	for(c=0;c<6500;++c) {}
	SID.v1.ad    = 0x15; 
	SID.v1.sr    = 0x64; 
	SID.amp      = 0x1F; 
	SID.v1.ctrl  = 0x21; 
	SID.v1.freq  = 0x4F6; 	
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x11;
	SID.v1.freq  = 0x4A5; 
	for(c=0;c<6500;++c) {} 
	SID.v1.ad    = 0x31;
	SID.v1.freq  = 0x454; 
	for(c=0;c<6500;++c) {} 
	SID.amp      = 0x00; 
	SID.v1.ctrl  = 0x10;
}

void bossBeamSound(void){
	SID.amp      = 0x15;
	SID.v1.sr    = 0xAA; 
	SID.v1.ad    = 0x97;
	SID.v1.freq  = 0x4FB;
	SID.v1.ctrl  = 0x21; 
}

void playNote(int frequency, int attdec, int susrel, int amplif ){
	//unsigned int c;
	//	
	SID.v2.ad    = attdec; 
	SID.v2.sr    = susrel; 
	SID.amp      = amplif; 
	SID.v2.ctrl  = 0x21; 
	SID.v2.freq  = frequency; 	
	//for (c=0; c<2500;++c){}
}

void silence(int voice){
	switch (voice){
		case 1:
		SID.v1.ctrl  = 0x10;
		break;
		case 2:
		SID.v2.ctrl  = 0x10;
		break;
		case 3:
		SID.v2.ctrl  = 0x10;
		break;
		default:
		break;
	}
}