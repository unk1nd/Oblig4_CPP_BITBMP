class Pixel 
{

	public:

		// Konstrukt�r med flere argumenter
		Pixel(int r, int g, int b);

		// Funksjon for � endre fargene til en piksel
		void edit(int r, int g, int b);

		// Funksjoner for � lese ut R,G og B verdiene
		int getR();
		int getG();
		int getB();

	private: 

		// Medlemsvariabler
		int r;
		int g; 
		int b;
};