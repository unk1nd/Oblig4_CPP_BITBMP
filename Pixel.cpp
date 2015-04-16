#include "Pixel.h"

// GET og SET metoder for Pixel

Pixel::Pixel(int _r, int _g, int _b)
:r(_r), g(_g), b(_b) { }

int Pixel::getR(){
	return r;
}

int Pixel::getG(){
	return g;
}

int Pixel::getB(){
	return b;
}

void Pixel::edit(int _r, int _g, int _b){
	r = _r;
	g = _b;
	b = _b;
}