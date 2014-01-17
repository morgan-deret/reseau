#include <iostream>
#include <cstring>
#include "proto.hh"

struct session_on_client;
namespace recherche
{
  using namespace std;
  using namespace netez;

  struct session_on_client: public session_base
  {
   
    int joueur;
    session_on_client(socket& io): session_base(io)
    {

      sig_begin.connect(EZMETHOD(this,on_begin));
      proto.err.sig_recv.connect(EZMETHOD(this,do_err));
      proto.okint.sig_recv.connect(EZMETHOD(this,do_okint));
      proto.gagner.sig_recv.connect(EZMETHOD(this,do_gagner));
      proto.perdu.sig_recv.connect(EZMETHOD(this,do_perdu));
      proto.said.sig_recv.connect(EZMETHOD(this,do_said));
      proto.afficher.sig_recv.connect(EZMETHOD(this,do_afficher));
      proto.attendre.sig_recv.connect(EZMETHOD(this,do_attendre));
      proto.jouer.sig_recv.connect(EZMETHOD(this,do_jouer));      
     
    }

    void do_err(string error){
      cerr<<error<<endl;
    }
   
    void do_okint(int j){

      cout<<"Votre adversaire est connecte!"<<endl;
      joueur=j;
       
    }
   
    void do_gagner(string mot,string nom){
      system ("clear");
      cout<<endl<<endl;
      cout<<nom << " vous avez gagne, le mot etait : "<<mot<<endl;
      cout<<endl<<endl;
      finish();
    }
   
    void do_perdu(string mot,string nom){
      system ("clear");
      cout<<endl<<endl;
      cout<<nom <<" vous avez perdu, le mot etait : "<<mot<<endl;
      cout<<endl<<endl;
      finish();
    }

    void do_said(string s){
     
      cout<<"\nVotre adversaire a joué : "<<s<<endl;
     
    }

    void do_afficher(string lettresUtilisees, string encours){
      system ("clear");
      cout<<endl<<endl;
      cout<<"Lettres utilisees : "<<lettresUtilisees<<endl;
      cout<<endl<<endl;
      cout<<encours<<endl;


    }
   
    void do_attendre(string nom){    
      cout<<endl<<endl;
      cout<<nom<<" veuillez attendre l'action de l'adversaire"<<endl;
    }

    void do_jouer(string nom){
      string mot;
      cout<<endl<<endl;
      cout<<nom<<" que voulez vous entrer ? ";
      cout.flush();
      cin>>mot;
      proto.say(mot,joueur);
    }

    void on_begin(){
      system ("clear");
      cout<<endl<<endl;
      string nom;
      cout<<"Quel est votre nom ? ";
      cout.flush();
      cin>>nom;
      proto.nom(nom);
    }

  };
}

int main(int argc, char* argv[]){

  netez::client<recherche::session_on_client> client(argc, argv);
  client.join();
  return 0;
}
