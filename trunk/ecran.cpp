#include "ecran.hpp"

// ****************************************************************************** //
// ******************************* E C R A N ************************************ //
// ****************************************************************************** //

ecran::ecran(int h, int l){
	SDL_Init(SDL_INIT_VIDEO);  
	ecr = SDL_SetVideoMode(l, h, 32, SDL_HWSURFACE); 
	SDL_WM_SetCaption("Casse Briques", NULL); 
}


void ecran::peindre(int x, int y, int h, int l) {
	SDL_Rect rectangle = {x, y, h, l};  
	SDL_FillRect(ecr, &rectangle, SDL_MapRGB(ecr->format, 0, 0, 0));
}

void ecran::vider() {
	SDL_FillRect(ecr, NULL, SDL_MapRGB(ecr->format, 255, 255, 255));
}

void ecran::flip() {
	SDL_Flip(ecr);
}


ecran::~ecran(){
	SDL_Quit();
}

