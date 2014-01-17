#ifndef _PENDUEZ_HH_
#define _PENDUEZ_HH_

#include "netez.hh"
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
namespace recherche
{
  using namespace netez;

  struct proto: public protocol<>  {
    message<0,void(string)>nom;
    message<1,void(string,int)> say;
    message<2,void(string)>err;
    message<3,void(int)>okint;
    message<4,void(string,string)>gagner;
    message<5,void(string,string)>perdu;
    message<6,void(string)>said;
    message<7,void(string,string)>afficher;
    message<8,void(string)>attendre;
    message<9,void(string)>jouer;
    proto(socket & io):protocol<>(io),nom(this),say(this),err(this),okint(this),gagner(this),perdu(this),said(this),afficher(this),attendre(this),jouer(this){}   
  }; 
  typedef session<proto> session_base;
}

#endif
