#include <iostream>
#include <Program.h>
#include <Basic_block.h>
#include <Function.h>

/*
  ##################Programme � appeler avec le nom d'un fichier assembleur en argument###############

  -Affichage du nombre de lignes du programme, de son contenu
  - D�coupage en fonctions et affichage du nombre de fonction
  - D�coupage en nombre de bloc de base
  -Pour chaque fonction, calcul du decoupage en blocs de base, des successeurs et pr�d�cesseurs des BB, construction et restitution en fichier .dot du CFG
  -Affichage du BB0 de la fonction0

 */

int main(int argc, char * argv[]) {

    remove("tmp/test1.txt");
    remove("tmp/test2.txt");
    remove("./tmp/graph.dot");
    if (argc < 2) {
        cout << "erreur : pas de fichier assembleur en entr�e" << endl;
    }
    Program prog(argv[1]);
    Function* functmp;

    list <Basic_block*> myBB;

    cout << "Le programme a " << prog.size() << " lignes\n" << endl;

    cout << "Contenu du programme:" << endl;
   // prog.display();

    //prog.in_file("tmp/restit.txt");

    cout << "\n Calcul des fonctions des block de base et restitution\n" << endl;
    prog.comput_function();

    cout << "nombre de fonction: " << prog.nbr_func() << endl;

    Cfg *graph;
    for (int i = 0; i < prog.nbr_func(); i++) {

        functmp = prog.get_function(i);

        if (functmp == NULL) {
            cout << "null" << endl;
            break;
        }
        cout << "FONCTION " << i << endl;
        functmp -> display();
        
        cout << "Determination des block de base " << endl;
        //determine les BB de la fonction
        functmp->comput_basic_block();

        
        cout << "Affichage des blocs de base " << endl; 
        for(int j=0; j<functmp->nbr_BB(); j++){
           functmp->get_BB(j)->display();
        }
         

        // calcul les �tiquettes de la fonction
        functmp->comput_label();
        // calcul les liens entre BB 
        functmp->comput_succ_pred_BB();
        functmp->test();
        functmp->compute_dom();
        // cr�e un nouveau CFG pour la fonction
        // permet de le visualiser avec dotty, cf. plus bas
        cout << "CREATION DU CFG ----------------------------------------------------" << endl;
        graph = new Cfg(functmp->get_BB(0),functmp->nbr_BB());
        cout << "fin DU CFG ----------------------------------------------------" <<endl ;

        cout << "------------Function " << (i + 1) << "/" << prog.nbr_func() << " DISPLAY----------\n" << endl;
        // affichage des BB et des predecesseurs/successeurs
        // functmp->test();
        // pour afficher le CFG mais l'affichage ci-dessus donne les infos aussi
        // graph->display(NULL);
    }
    // met dans une fichier une repr�sentation du dernier CFG construit

    graph->restitution(NULL, "./tmp/graph.dot");
}
