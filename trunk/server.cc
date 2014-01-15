#include "proto.hh"
#include <map>
#include <string>
#include <iostream>

using namespace std;
using namespace netez;
ezmutex mutex;


struct session_on_server:public session_base{
  static map<string,session_on_server*> m ;
  string nick;
  session_on_server(socket & io): session_base(io){
    
    proto.ok.sig_recv.connect(EZMETHOD(this,do_ok));
    proto.error.sig_recv.connect(EZMETHOD(this,do_error));
    proto.deplace.sig_recv.connect(EZMETHOD(this,do_deplace));
    proto.NICK.sig_recv.connect(EZMETHOD(this,do_NICK));
    proto.QUIT.sig_recv.connect(EZMETHOD(this,do_QUIT));
    
    
  }

  void do_ok(){
    cout<<"Ok"<<endl;
  }

  void do_error(string s){
    cout<<s<<endl;
  }

  void do_deplace(int x,int y){
    if(x<=0 || x>=300 || y<=0 || y>= 300)
      proto.error("Error");
    auto it = m.begin();
    while (it -> first != nick){
      ++it;
    }
    it->second->proto.changementpos(x,y);
  }

  
  void do_NICK(string s){
    if (m.find(s) == m.end()){
      nick = s;
      m[s] = this;
      proto.ok();
      auto beg = m.begin();
      auto end = m.end();
      while (beg != end){
	if(beg->second->nick != s)
	  beg->second->proto.JOINED(s);
	++beg;
      }
    }
    else{
      proto.error("le Nick est déja pris veuillez en choisir un autre");

    }
  }
    
  void do_QUIT(){
    cout<<"fin de la session"<<endl;
    m.erase(nick);
    proto.QUIT();
    finish();
  }

};

map<string, session_on_server*> session_on_server::m;


int main(int argc, char** argv){
  server<session_on_server>server(argc,argv);
  server.join();
  return 0;
}
