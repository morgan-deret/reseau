#ifndef ECRANHPP
#define ECRANHPP



#include <SDL/SDL.h>

 


class ecran{

private:

  SDL_Surface *ecr ;

public:

  ecran(int h, int l);
  void peindre(int x, int y, int h, int l);
  void vider();
  void flip();
  ~ecran();
};

#endif