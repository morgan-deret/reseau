#include "proto.hh"
#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include "ecran.hpp"

using namespace std;
using namespace netez;


struct session_on_client:public session_base{
  enum state_t
    {
      state_connected,
      state_logged
    };
  
  state_t state=state_connected;
  session_on_client(socket & io):session_base(io), state(state_connected)
  {
    sig_begin.connect(EZMETHOD(this,on_begin));
    proto.QUIT.sig_recv.connect(EZMETHOD(this,do_QUIT));
    proto.JOINED.sig_recv.connect(EZMETHOD(this, do_JOINED));
    proto.NICK.sig_recv.connect(EZMETHOD(this, do_NICK));
  }

  void do_QUIT(){
    finish();
  }

  void on_begin(){}

  void do_NICK(string s){
    if(state == state_connected){
      proto.NICK(s);
      state=state_logged;
    }
    else cout<<"vous etes deja connecter"<<endl;
  }

  void do_quit(){
    proto.QUIT();
  }

  void do_JOINED(string s){   
    if(state==state_logged)

      cout<<"server : "<<s<<" joined the chat"<<endl; 
  }

};

string strip( string & s){
  size_t i = s.find_first_not_of(" \t");
  if (i == string :: npos) 
    return "";
  size_t j = s.find_last_not_of(" \t");
  return s.substr (i,j-i+1);
}

int main(int argc, char** argv)
{
  client<session_on_client> cli(argc, argv);
  string line;
  while ( getline (cin , line ))
    {
      istringstream is(line );
      string cmd;
      is >> cmd;
      string rest, nick;
      getline (is , rest);
      if(cmd == "NICK"){

	rest = strip(rest);
	cli.session.do_NICK(rest);
	
      }
      if(cmd=="QUIT")
	break;
    
    }

  SDL_Event event; // evenements (pour la gestion des touches clavier) 
  bool continuer = true;

  ecran zone2(800, 500), zone_affichage(640,480); 

  // permet de simuler un timer 
  int tempsPrecedent = 0, tempsActuel = 0; 
  //espace de temps entre deux passages de boucles 
  int intervalle = 10; 

  while (continuer)
    {
      
      SDL_PollEvent(&event); 

      switch(event.type)
	{
	case SDL_QUIT: return false;	
	case SDL_KEYDOWN: 
	  switch(event.key.keysym.sym) {
	  case SDLK_RIGHT: 
	    break;
	  case SDLK_LEFT:
	    

	    break;
	  }
	  break;

	}
      zone_affichage.vider();      
      zone_affichage.peindre(0,0,5,640);
      zone_affichage.peindre(0,0,480,5);
      zone_affichage.peindre(475,0,5,640);
      zone_affichage.flip(); 



      tempsActuel = SDL_GetTicks(); 

      if (tempsActuel - tempsPrecedent < intervalle)
	SDL_Delay(intervalle - (tempsActuel - tempsPrecedent));
  
      tempsPrecedent = tempsActuel;
    }

  cli.session.do_quit();
  cli.join();

  return 0;
}
