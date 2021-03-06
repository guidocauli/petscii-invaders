/*
	PETSCII Invaders by Guido Cauli 2022
	Copyright 2022 Guido Cauli <guido.cauli@gmail.com>

	Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione - Non commerciale - Non opere derivate 4.0 Internazionale. 
	Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-nc-nd/4.0/ o spedisci una lettera a
	Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

*/

const byte levelMap[10][54]={ 
	{1,9,4,	1,12,4,	1,15,4,	1,18,4,	1,21,4,	1,24,4,	
	1,7,7,	1,10,7,	1,13,7,	1,16,7,	1,19,7,	1,22,7,	1,25,7,	1,28,7, 1,6,4, 1,27,4, 1,30,4, 1,4,7,},
	
	{1,9,4,	1,12,4,	2,15,4,	2,18,4,	1,21,4,	1,24,4,	
	1,7,7,	2,10,7,	1,13,7,	1,16,7,	1,19,7,	1,22,7,	2,25,7,	1,28,7, 1,6,4, 1,27,4, 1,30,4, 1,4,7,},
	
	{1,3,4,	1,6,4,	2,9,4,	2,12,4,	3,15,4,	3,18,4,	2,21,4,	2,24,4,	1,27,4,	1,30,4,
	1,6,7,	2,9,7,	1,12,7,	1,15,7,	1,18,7,	2,21,7,	1,24,7,	1,27,7,},
	
	{2,8,4,  2,12,4, 2,16,4, 2,22,4, 2,26,4, 2,30,4,
	 1,9,7,  3,14,7, 3,19,7, 3,24,7, 1,29,7,
	1,7,10, 1,11,10, 2,15,10, 2,19,10, 2,23,10, 2,27,10, 1,30,10,},
	
	{3,6,4, 3,9,4, 2,12,4, 2,27,4, 3,30,4, 3,33,4,
	2,7,7, 2,10,7, 2,13,7, 2,26,7, 2,29,7, 2,32,7,
	1,5,10, 1,10,10, 1,15,10, 1,20,10, 1,25,10, 1,30,10,},
		
	{3,4,4, 3,7,4, 2,10,4, 2,24,4, 3,27,4, 3,30,4,
	2,7,14, 2,10,10, 2,13,10, 2,21,10, 2,24,10, 2,27,14,
	1,5,17, 1,10,14, 1,15,14, 1,20,14, 1,25,14, 1,30,17,}, 
	
	{2,5,4, 2,9,4,	2,12,4,	3,15,4,	3,18,4,	2,21,4,	2,24,4,	2,29,4,
	3,13,10,3,20,10,
	1,7,14,	2,10,14, 1,13,14, 1,16,14, 1,19,14, 1,22,14, 2,25,14, 1,28,14, },
	
	{2,15,4, 2,18,4, 2,21,4,
	4,9,6, 3,12,6, 3,24,6, 3,27,6,
	2,7,8, 2,30,8,
	2,7,10, 2,30,10,
	3,9,12, 3,12,12, 3,24,12, 4,27,12,
	2,15,14, 2,18,14, 2,21,14,
	},
	{2,3,11, 3,6,11, 4,9,11, 4,12,11, 4,15,11, 4,18,11, 4,21,11, 4,24,11, 3,27,11, 2,30,11,
	 2,4,14, 2,7,14, 2,10,14, 2,13,14, 2,18,14, 2,21,14, 2,24,14, 2,27,14,},
	
	{
		4,4,9, 2,7,9, 4,10,9, 3,13,9, 4,16,9, 3,19,9, 4,22,9, 2,25,9, 4,28,9,
		1,3,11, 4,6,11, 2,9,11, 2,12,11, 2,15,11, 2,18,11, 2,21,11, 4,24,11, 1,27,11,
	}
	
};
