/*
Kevin Epp  CSE 20212
Lab7/8  blob.cpp
This program animates a simple dot against a Mortal Kombat background
that can move to the left and right very simply with SDL
*/

#include "SDL/SDL.h"
#include <string>

using namespace std;

//Screen dimensions and bits-per-pixel
const int screen_height = 566;
const int screen_width = 800;
const int screen_bpp = 32;

SDL_Surface *background; //Background image, a Mortal Kombat landscape
SDL_Surface *screen; //The blank screen

SDL_Event event; //Used to check what events happen at the keyboard

//Load the image inputted to the function so it can be displayed later
SDL_Surface *load_image(string filename){
	SDL_Surface *loaded; //Loaded image
	SDL_Surface *optimized; //Pixel-optimized image
	
	loaded = SDL_LoadBMP(filename.c_str()); //Load the image

	optimized = SDL_DisplayFormat(loaded); //Optimize the image
	SDL_FreeSurface(loaded); //Free the loaded image to avoid memory leak

	//Change the background color for the loaded image so it doesn't show up
	Uint32 colorkey = SDL_MapRGB( optimized->format, 255, 255, 255 );
	SDL_SetColorKey( optimized, SDL_SRCCOLORKEY, colorkey );

	//Return optimized image
	return optimized;
}

//Apply a loaded image to the screen at a certain place
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination){
	SDL_Rect offset;

	//Coordinates of where to place loaded image
	offset.x = x; 
	offset.y = y;
	
	//Display image to desired destination (most likely screen) at coordinates
	SDL_BlitSurface(source, NULL, destination, &offset);
}

//Dot's class
class Blob{
	public:
		Blob(); //Default Constructor
		void move(); //Move the dot according to key pressed
		void show(SDL_Surface*); //Show the dot on desired surface
		void free(); //Free the dot's space in memory

	private:
		int x; //x-coordinate
		int y; //y-coordinate
		SDL_Surface *image; //Image of the dot
};

//Initialize dot
Blob::Blob(){
	x = 400; //x-coordinate
	y = 450; //y-coordinate
	image = load_image("dot.bmp"); //Load image of the dot
}

//Move the dot
void Blob::move(){
	if(event.type == SDL_KEYDOWN){ //If key was pressed
		switch(event.key.keysym.sym){
			case SDLK_RIGHT: //If right arrow was presses
				x = x + 10; //move dot to right
				break;
			case SDLK_LEFT: //If left arrow was pressed
				x = x - 10; //move dot to left
				break;
		}
	}
}

//Display the dot
void Blob::show(SDL_Surface* destination){
	apply_surface(x,y,image, destination);
}

//Free the dot in memory
void Blob::free(){
	SDL_FreeSurface(image);
}

//Initialize the screen for SDL use
bool init(){
	//Initialize everything, return false if failed
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		return false;
	}

	//Open up the screen with desired dimensions
	screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_SWSURFACE);

	//Return if screen cannot open
	if(screen == NULL){
		return false;
	}

	SDL_WM_SetCaption("Blob", NULL); //Set screen's title

	return true;
}	

int main(int argc, char* args[]){
	bool quit = false; //Checks if user quit

	//If SDL couldn't intialize, quit
	if(init() == false){
		return 1;
	}

	Blob myDot; //Initialize dot
	
	background = load_image("images.bmp"); //Load background image
	//Mortal Kombat landscape	

	while(quit == false){ //While user has not exited window
		while(SDL_PollEvent(&event)){ //While there is an event happening
			myDot.move(); //Check if appropriate key was pressed and move dot accordingly
			if(event.type == SDL_QUIT){ //If user exited window
				quit = true; //Quit
			}
		}
		apply_surface(0,0,background,screen); //Display background on the screen
		myDot.show(screen); //Display dot on the screen over background

		//If screen could not be shown, quit
		if(SDL_Flip(screen) == -1){
			return 1;
		}		
	}
		
	myDot.free(); //Free dot
	SDL_FreeSurface(background); //Free background
	SDL_Quit(); //Free screen and quit SDL
	
	return 0;
}
