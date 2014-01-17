#include "proto.hh"
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <fstream>




namespace recherche{
  using namespace std;
  using namespace netez;

  struct session_server;

  bool * test;              
  string mot;               
  session_server* tab[2];   
  string lettresUtilisees;        
  bool joueur_co = false;


  struct session_server:public session_base{
    string nom;            


    session_server(socket & io):session_base(io){

      proto.nom.sig_recv.connect(EZMETHOD(this,do_nom));
      proto.say.sig_recv.connect(EZMETHOD(this,do_say));


    }

    string chercher_mot(const char * fichier) {
      
      srand(time(NULL));
      int r = (rand() % (22217)+ 1);

      ifstream f(fichier,ios::in);
      int i=0;
      string mot;
      while(i!=r){
	getline(f,mot);
	i++;
       
      }

      f.close();
      return mot;
    }



    void gagne(int joueur,bool gagne,string lettresUtilisees){

      if(gagne == true){
        tab[joueur]->proto.perdu(mot,tab[joueur]->nom);
        tab[!joueur]->proto.gagner(mot,tab[!joueur]->nom);
        sleep(5);
	cout<<"Serveur connection terminée [OK]"<<endl;
        exit(1);
      }

      string encours ="";
      unsigned int i = 0;
      while(i<mot.length()){
        if (test[i] == true){
          encours += mot[i]; 
          encours += " ";        
        }
        else
          encours += "_ ";
        i++;
      }

      tab[joueur]->proto.afficher(lettresUtilisees,encours);
    }


    void do_nom(string n){

      if(!joueur_co){
        joueur_co=true;
        nom = n;
        tab[0]=this;
        tab[0]->proto.attendre(nom);
        cout<<"Joueur 1 connection [OK]"<<endl;
      }
      else{      
        cout<<"Joueur 2 connection [OK]"<<endl;
        
	cout<<"Mise en place nouveau mot [En cours]"<<endl;
        mot=chercher_mot("../motRecherche.txt");
	cout<<"Mise en place nouveau mot [OK]"<<endl;
        cout<<mot<<endl;
        test = new bool[mot.length()];
        unsigned int i = 0;
        while(i<mot.length()){
          test[i]=false;
          i++;
        }
	
        nom = n;
        tab[1]=this;
        tab[0]->proto.okint(0);
        tab[1]->proto.okint(1);
        gagne(0,false,lettresUtilisees);
        gagne(1,false,lettresUtilisees);
        tab[0]->proto.jouer(tab[0]->nom);
        tab[1]->proto.attendre(tab[1]->nom);

      }

    }

    void do_say(string essai,int joueur){
      cout<<"Joueur "<<joueur+1<<" joue [En cours]"<<endl;

      if(essai.length() == 1)  {

	char lettre = essai[0];
	unsigned int i = 0;
	while(i<mot.length()){
	  if(mot[i] == lettre)
	    test[i]=true;
	  i++;
	}
	lettresUtilisees += lettre;
	lettresUtilisees += " ";
	tab[!joueur]->proto.said(essai);          

	gagne(joueur,false,lettresUtilisees);
	gagne(!joueur,false,lettresUtilisees);

	
	bool ok=true;
	i = 0;
	while(i<mot.length()){
	  if(!test[i])
	    ok=false;
	  i++;
	}
	if(ok)
	  {

	    gagne(joueur,false,lettresUtilisees);
	    gagne(!joueur,true,lettresUtilisees);
	  }

      }
      else { 

        if(mot == essai){      
          cout<<"Joueur "<<joueur+1<<" a trouve le mot"<<endl;
          gagne(joueur,false,lettresUtilisees);
          gagne(!joueur,true,lettresUtilisees);  

        }
        else if(mot != essai)
	  {
	    cout<<"Joueur "<<joueur+1<<" n'a pas trouve le bon mot "<<endl;
	    tab[joueur]->proto.err("Ce n'est pas le bon mot !");
	    tab[!joueur]->proto.said(essai);

	    gagne(joueur,false,lettresUtilisees);
	    gagne(!joueur,false,lettresUtilisees);
	  }          

      }      

      tab[joueur]->proto.attendre(tab[joueur]->nom);
      tab[!joueur]->proto.jouer(tab[!joueur]->nom);
      cout<<"Joueur "<<joueur+1<<" joue [OK]"<<endl;
    }
  };
}


int main(int argc, char** argv){
  netez::server<recherche::session_server>server(argc,argv);
  cout<<"Serveur connection entrante [OK]"<<endl;
  server.join();

  
  return 0;
} 
