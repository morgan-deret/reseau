#ifndef _PROJET_
#define _PROJET_

#include "netez.hh"
#include <string>
#include <iostream>

using namespace netez;
using namespace std;

struct proto:public protocol<>
{
  message <0,void()>ok;
  message <1,void(string)>error;
  message <2,void(int,int)>deplace;
  message <3,void()>QUIT;
  message <4,void(string)>NICK;
  message <5,void(string)>JOINED;
  message <6,void(int,int)>changementpos;

  proto(socket & io):
  protocol<>(io),
  ok(this),
  error(this),
  deplace(this),
  QUIT(this),
  NICK(this),
  JOINED(this),
  changementpos(this)
  {}

};

typedef session<proto> session_base;

#endif
