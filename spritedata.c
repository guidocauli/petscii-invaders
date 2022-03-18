/*
	PETSCII Invaders by Guido Cauli 2022
	Copyright 2022 Guido Cauli <guido.cauli@gmail.com>

	Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione - Non commerciale - Non opere derivate 4.0 Internazionale. 
	Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-nc-nd/4.0/ o spedisci una lettera a
	Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

*/

const byte udg[] = { 
	1,2,1,2,1,2,1,2,		// beam
	14,31,21,31,14,10,17,0,		//alien pod
	0,66,24,32,4,24,66,0,		// explosion
	6,9,0,6,9,0,6,9,		// power beam
	8,20,34,20,8,20,8,20,		//hyper beam
};


const byte udg64[] = {

	0,0,67,100,89,35,39,45, 0,0,194,38,154,196,228,180, 
	47,36,41,34,20,20,18,12, 244,36,148,68,40,40,72,48, //alien 1.1 
	0,0,0,227,92,35,39,77, 0,0,0,199,58,196,228,178, 
	79,68,41,38,72,144,136,112, 242,34,148,100,18,9,17,14, // alien 1.2
	0,1,2,37,86,105,86,75, 0,128,64,164,106,150,106,210, 
	38,21,42,69,10,17,8,0, 100,168,84,162,80,136,16,0, //alien 2.1
	0,1,2,133,230,153,150,139, 0,128,64,161,103,153,105,209, 
	102,21,42,37,10,17,32,0, 102,168,84,164,80,136,4,0, //alien 2.2
	14,57,100,78,159,179,243,251, 112,156,38,114,249,205,207,223, 
	79,69,32,43,52,24,0,0, 242,82,4,212,44,24,0,0, //alien 3.1
	0,15,20,46,95,115,115,123, 0,240,40,116,250,206,206,222, 
	79,69,32,42,55,24,0,0, 242,82,4,84,236,24,0,0,		//alien 3.2
	66,164,146,73,74,37,35,154, 66,37,73,146,82,164,68,89, 
	100,26,34,37,74,145,224,1, 38,88,196,164,82,145,143,0, // alien 4.1
	98,148,138,73,106,21,19,26, 70,41,81,146,82,164,72,88,
	100,154,18,21,42,81,81,96, 38,89,200,164,82,146,10,140, //alien 4.2
	
	};

const byte udgB[] = {
	
	0,5,15,14,4,20,61,123, 0,160,240,112,32,40,188,222, 
	62,9,6,96,42,14,14,4, 124,144,96,6,84,112,112,32, //ship 1
	0,5,15,14,4,20,61,123, 0,160,240,112,32,40,188,222, 
	62,9,6,32,89,10,6,0, 124,144,96,4,154,80,96,0, //ship 2
	7,25,45,78,68,148,189,233, 224,152,180,114,34,41,189,151,
	239,190,137,70,81,42,29,6, 247,125,145,98,138,84,184,96, // shield 1
};


