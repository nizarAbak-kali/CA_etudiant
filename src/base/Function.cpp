#include <Function.h>

Function::Function(){
  _head = NULL;
  _end = NULL;
  BB_computed = false;
  BB_pred_succ = false;
  dom_computed = false;
}

Function::~Function(){}

void Function::set_head(Line *head){
  _head = head;
}

void Function::set_end(Line *end){
  _end = end;
}

Line* Function::get_head(){
  return _head;
}

Basic_block* Function::get_firstBB(){
   return _myBB.front();
}

Line* Function::get_end(){
  return _end;
}
void Function::display(){
  cout<<"Begin Function"<<endl;
  Line* element = _head;

  if(element == _end)	
    cout << _head->get_content() <<endl;

  while(element != _end){
    cout << element->get_content() <<endl;
		
    if(element->get_next()==_end){
      cout << element->get_next()->get_content() <<endl;
      break;
    }
    else element = element->get_next();

    }
  cout<<"End Function\n\n"<<endl;
	
}

int Function::size(){
  Line* element = _head;
  int lenght=0;
  while(element != _end)
    {
      lenght++;		
      if (element->get_next()==_end)
	break;
      else
	element = element->get_next();
    }
  return lenght;
}	


void Function::restitution(string const filename){
	
  Line* element = _head;
  ofstream monflux(filename.c_str(), ios::app);

  if(monflux){
    monflux<<"Begin"<<endl;
    if(element == _end)	
      monflux << _head->get_content() <<endl;
    while(element != _end)
      {
	if(element->isInst() || 
	   element->isDirective()) 
	  monflux<<"\t";
	
	monflux << element->get_content() ;
	
	if(element->get_content().compare("nop")) 
	  monflux<<endl;
		
	if(element->get_next()==_end){
	  if(element->get_next()->isInst() || 
	     element->get_next()->isDirective())
	    monflux<<"\t";
	  monflux << element->get_next()->get_content()<<endl;
	  break;
	}
	else element = element->get_next();

      }
    monflux<<"End\n\n"<<endl;
		
  }

  else {
    cout<<"Error cannot open the file"<<endl;
  }

  monflux.close();
}

void Function::comput_label(){
  Line* element = _head;

  if(element == _end && element->isLabel())	
    _list_lab.push_back(getLabel(element));
  while(element != _end)
    {

      if(element->isLabel())	
	_list_lab.push_back(getLabel(element));

      if(element->get_next()==_end){
	if(element->isLabel())	
	  _list_lab.push_back(getLabel(element));
	break;
      }
      else element = element->get_next();

    }

}

int Function::nbr_label(){
  return _list_lab.size();

}

Label* Function::get_label(int index){

  list<Label*>::iterator it;
  it=_list_lab.begin();

  int size=(int) _list_lab.size();
  if(index< size){
    for (int i=0; i<index;i++ ) it++;
    return *it;	
  }
  else cout<<"Error get_label : index is bigger than the size of the list"<<endl; 
	
  return _list_lab.back();
}

Basic_block *Function::find_label_BB(OPLabel* label){
  //Basic_block *BB = new Basic_block();
   int size=(int)_myBB.size();
   string str;
   for(int i=0; i<size; i++){		
      if(get_BB(i)->is_labeled()){
	 
	 str=get_BB(i)->get_head()->get_content();
	 if(!str.compare(0, (str.size()-1),label->get_op())){
	    return get_BB(i);
	 }
      }
  }
  return NULL;
}


/* ajoute nouveau BB � la liste de BB de la fonction en le creant */

void Function::add_BB(Line *debut, Line* fin, Line *br, int index){
   Basic_block *b=new Basic_block();
   b->set_head(debut);
   b->set_end(fin);
   b->set_index(index);
   b->set_branch(br);
   _myBB.push_back(b);
}


//Calcule la liste des blocs de base : il faut d�limiter les BB, en parcourant la liste des lignes/instructions � partir de la premiere, il faut s'arreter � chaque branchement (et prendre en compte le delayed slot qui appartient au meme BB, c'est l'instruction qui suit tout branchement) ou � chaque label (on estime que tout label est utilis� par un saut et donc correspond bien � une entete de BB).


void Function::comput_basic_block(){
  Line *debut, *current, *prev;
   current=_head;
   debut=_head;
   prev = NULL;
   int ind=0;
   Line *l=NULL;
   Instruction *i=NULL;
   Line * b;

   cout<< "comput BB" <<endl;
   cout<<"head :"<<_head->get_content()<<endl;
   cout<<"tail :"<<_end->get_content()<<endl;

   //ne pas enlever la ligne ci-dessous 
   if (BB_computed) return;

  
   /**** A COMPLETER ****/


   cout<<"end comput Basic Block"<<endl;
   BB_computed = true;
   return;
}


int Function::nbr_BB(){
   return _myBB.size();
}

Basic_block *Function::get_BB(int index){

  list<Basic_block*>::iterator it;
  it=_myBB.begin();
  int size=(int)_myBB.size();

  if(index< size){
    for (int i=0; i<index;i++ ) it++;
    return *it;	
  }
  else 
    return NULL;
}

list<Basic_block*>::iterator Function::bb_list_begin(){
   return _myBB.begin();
}

list<Basic_block*>::iterator Function::bb_list_end(){
   return _myBB.end();
}

/* comput_pred_succ calcule les successeurs et pr�d�cesseur des BB, pour cela il faut commencer par les successeurs */
/* et it�rer sur tous les BB d'une fonction */
/* il faut determiner si un BB a un ou deux successeurs : d�pend de la pr�sence d'un saut pr�sent ou non � la fin */
/* pas de saut ou saut incontionnel ou appel de fonction : 1 successeur (lequel ?)*/
/* branchement conditionnel : 2 successeurs */ 
/* le dernier bloc n'a pas de successeurs - celui qui se termine par jr R31 */
/* les sauts indirects n'ont pas de successeur */


void Function::comput_succ_pred_BB(){
  
   list<Basic_block*>::iterator it, it2;
   Basic_block *current;
   Instruction *instr;
   Basic_block *succ=NULL;
   // IMPORTANT ne pas enlever la ligne ci-dessous 
   if (BB_pred_succ) return;
   int size= (int) _myBB.size();
   it=_myBB.begin();
   
   for (int i=0; i<size; i++){
     current=*it;
    
     /** A COMPLETER **/
     /*** boucle qui permet d'it�rer sur les blocs de la fonction ***/

     it++;
   }
   
   // ne pas enlever la ligne ci-dessous
   BB_pred_succ = true;
   return;
}

void Function::compute_dom(){
 list<Basic_block*>::iterator it, it2;
  list<Basic_block*> workinglist;
  Basic_block *current, *bb, *pred;
  Instruction *instr;
  bool change = true;
  
  // NE pas enlever les 2 ligne ci-dessous
  if (dom_computed) return;
  comput_succ_pred_BB();
 

  // A COMPLETER 

  // ne pas enlever 
  dom_computed = true;
  return;
}

void Function::compute_live_var(){
  
  
  /*** A COMPLETER */

  return;
}
     





/* en implementant la fonction test de la classe BB, permet de tester des choses sur tous les blocs de base d'une fonction par exemple l'affichage de tous les BB d'une fonction ou l'affichage des succ/pred des BBs comme c'est le cas -- voir la classe Basic_block et la m�thode test */

void Function::test(){
  int size=(int)_myBB.size();
   for(int i=0;i<size; i++){
    get_BB(i)->test();
  }
   return;
}
