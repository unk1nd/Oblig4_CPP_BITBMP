#include "main.h"
#include <iostream>

void settMerke(std::vector<Pixel> &pixelTabell, int _row, int _col, int _width, int _rowwidth) 
{
	// _row og _col er antatt å være 1-basert!
	// _width er bredden på bildet i antall piksler.
	// idx representerer indeks i tabell:
	int idx;

	//sr=start row, sc=start column
	//er=end row, ec=end column
	int sr = _row, sc=_col, er=sr+_width, ec=sc+_width;
	for (int i=sr; i < er; i++) 
	{
		for (int j=sc; j < ec; j++) 
		{
			//Finner indeks i tabell som svarer til
			//gitt rad (i) og kolonne (j):
			idx = (i - 1) * _rowwidth + (j - 1);  pixelTabell[idx].edit(0,0,0);
		}
	}
}

//component: 0=R, 1=G, 2=B
void changeColor(std::vector<Pixel> &pixelTabell, int component, int color)
{
	for (int i=0; i < pixelTabell.size(); i++) 
	{

		int r = pixelTabell[i].getR();
		int g = pixelTabell[i].getG();
		int b = pixelTabell[i].getB();

		switch (component)
		{
			case 0:
				r = color;
				break;
			case 1:
				g = color;
				break;
			case 2:
				b = color;
				break;
			default:
				break;
		}

		pixelTabell[i].edit(r,g,b);
	}
}

void greyScale(std::vector<Pixel> &pixelTabell){
	for (int i=0; i < pixelTabell.size(); i++) {

		int r = pixelTabell[i].getR();
		int g = pixelTabell[i].getG();
		int b = pixelTabell[i].getB();

		int color = (r+g+b)/3;

		pixelTabell[i].edit(color,color,color);
	}
}

// for å holde console running
void Ferdig()
{
	std::cout << "" << std::endl;
	std::cout << "Ferdig lagret. Trykk Enter for " << (char)134 << " avslutte...";
	std::cout.flush();
	std::cin.ignore(10000, '\n');
	return;
}


// MAIN metoden
int main() 
{
	std::cout << "BMP manipulering`s oppgave i C++ - Oblig 4" << std::endl; 
	std::cout << "--------------------------------------------" << std::endl; 

	std::fstream infile; 
	std::string innfilnavn = "mikael.bmp";		// innfil
	std::string utfilnavn = "mikael-new.bmp";	// utfil
	unsigned int offset;						// Angir starten på pikslene 
	unsigned int width;							// Bildets bredde 
	unsigned int height;						// Bildets høyde
	unsigned char header[54];					// Kan holde på hele headeren
	unsigned char enPixel[3];					// Bestående av 3 byte

	//Pixeltabel med filens pixler
	std::vector<Pixel> pixelTabell;

	// Åpning av BMP 
	// feiler åpning printes Feilmelding.
	// åpnes filen kjøres resten av koden.
	infile.open(innfilnavn.c_str(), std::ios::binary | std::ios::in); 
	if (infile.fail()) 
	{ 
		std::cout << "Feil ved " << (char)134 << "pning av fil" << std::endl;
	} 
	else 
	{
		std::cout << (char)143 << "pner bilde " << innfilnavn << " ..." << std::endl;
		std::cout << "Henter header..." << std::endl;

		// Leser hele headeren slik at den kan brukes i ny fil 
		infile.read((char *)header,54); 

		// Søker frem til 10. byte (fra start)
		infile.seekg(10, std::ios::beg); 
		infile.read((char*)&offset,4);
		infile.seekg(18, std::ios::beg);

		std::cout << "Leser pixler..." << std::endl;
		
		infile.read(reinterpret_cast<char *>(&width), sizeof(unsigned int));	//i antall piksler
		infile.read(reinterpret_cast<char *>(&height), sizeof(unsigned int));	//i antall piksler
		std::cout << " - H" << (char)155 << "yde:  " << width << " pixler" << std::endl;
		std::cout << " - Bredde: " << height << " pixler" << std::endl;
		std::cout << " - Total: " << height*width << " pixler" << std::endl;

		// Plasserer filpeker til starten av pikseldata:
		infile.seekg(offset);

		// NB!Bredden (i antall piksler) på bildet skal være delelig på 4.
		// Dersom bredden på bildet er delelig på 4 vil stuffing være lik 0,
		// ellers vil den bli 1,2 eller 3.
		int stuffing = width % 4;

		// pix holder rede på antall leste piksler per linje
		int pix=0;

		// Leser pikslene (3 byte per piksel)
		for(int i=0; i < (int)(height*width); i++)
		{
			// Leser tre bytes (en piksel) i slengen
			infile.read((char *)enPixel,3);

			// Øker antall piksler for denne linja med 1
			pix++;

			// Når antall piksler er lik bredden sjekker vi for evt. stuffing
			if((pix == width))
			{
				// p settes lik filpekeren
				int p = infile.tellp();

				// Søker forbi evt. stuffbytes
				infile.seekg(p + stuffing);
				pix=0;
			}

			// Opprett et nytt pikselobjekt og legg i vektor
			Pixel nyPixel((int)enPixel[2], (int)enPixel[1], (int)enPixel[0]);
			pixelTabell.push_back(nyPixel);
		}

		// Kaller fra main
		settMerke(pixelTabell, 10, 10, 50, width);
		std::cout << "Setter merke..." << std::endl;
		
		changeColor(pixelTabell, 1, 30);
		std::cout << "Bytter farge..." << std::endl;

		greyScale(pixelTabell);
		std::cout << "GreyScaler bilde..." << std::endl;


		// Skriver manipulert innhold til ny fil:
		std::fstream outfile;
		outfile.open(utfilnavn.c_str(), std::ios::binary | std::ios::out);

		std::cout << "Setter inn gammel header..." << std::endl;
		// Skriver headeren
		outfile.write((char *)header, 54);

		// "Hjelpebytes"
		unsigned char utdata[3];
		unsigned char stuffBytes[3] = {0, 0, 0};

		//Plasserer skrivepeker (byte 54):
		outfile.seekp(offset, std::ios::beg);

		//pix holder rede på antall skrevne piksler per linje
		pix=0;

		//Skriver alle pikslene fra tabellen til den nye fila:
		std::cout << "Lagrer bilde som "<< utfilnavn << " ..." << std::endl;
		for(int j=0; j < (int)pixelTabell.size(); j++)
		{
			utdata[0] = pixelTabell[j].getB();
			utdata[1] = pixelTabell[j].getG();
			utdata[2] = pixelTabell[j].getR();
			outfile.write((char *)utdata, 3);
			pix++;

			//Legger til evt. stuffing:
			if((pix == width))
			{
				outfile.write((char *)stuffBytes, stuffing);
				pix=0;
			}
			
		}
		infile.close();
		outfile.close();
		Pause();
	}
}