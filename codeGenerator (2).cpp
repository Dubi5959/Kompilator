#include "symbolTable.hpp"
#include "codeGenerator.hpp"
using namespace std;

symbolTable sTable{};  //extern symbolTable sTable;
string currentScope{};


void error(string s, int line){
    cerr << "Blad w linii " << line << ": " << s << endl;
    exit(1);
}

string toString(vector<instruction>* a){
    string output{};
    for(auto &aaa : *a){
        output = output+ aaa.instructionName;
    }

    return output;
}

procedureHeader* proc_head_function(string* s, vector<pair<string*,bool>>* argumentsList, long long a){
    cout << "Debug proc_head_function\n";

    cout << *s << " | " << *argumentsList->at(0).first  << " | " << *argumentsList->at(1).first << " | \n";
    /*
    -1) dodać do tablicy symboli scope reprezentujący ciało danej procedury
    2) dodać do scope argumenty z listy
    */
    procedureHeader* pH = new procedureHeader();

    currentScope=*s;
    sTable.Scopes.push_back(pair<vector<variable>, string>(vector<variable>(),*s));

    pH->procedureName=currentScope;
    for(auto &sss : sTable.Scopes){
        if(sss.second == *s){
            for(auto &sssss : *argumentsList){
                pH->arguments.push_back(*(sssss.first));

                variable v = variable{};
                v.name=*sssss.first;
                v.address=sTable.addressNumber++;
                v.isArg=true;
                v.isArray=sssss.second;
                sss.first.push_back(v);
            }
        }
    }
    sTable.listOfProcNames.push_back(pH);
    return pH;

}

void varDeclaration(string* s,long long a){
    /*
    1) ta funkcja powinna dodać zmienną do aktualnego scope
    */
    cout << "Debug var declaration: \n";
    //cout << sTable.Scopes.size() << "\n";
    for(auto &sss : sTable.Scopes){
        //cout << "hmm\n";
        if(sss.second == currentScope){
            for(auto &sAuto: sss.first){
                if(sAuto.name == *s){
                    //error jeśli istnieje
                    cout << "ERROR PANIC VAR\n";
                }
            }
        }
    }

    variable v = variable{};
    v.name=*s;
    v.address=sTable.addressNumber;
    sTable.addressNumber++;
    v.isArg=false;
    v.isArray=false;
    vector<variable> vec;
    vec.push_back(v);
    pair<vector<variable>, string> myPair ;
    myPair.first=vec;
    myPair.second=currentScope;
    sTable.Scopes.push_back(myPair);




}

void arrayDeclaration(string* s,long long sizeArray, long long a){
    cout <<"Debug arrayDeclaration\n";
    /*
    1) ta funkcja powinna dodać zmienną tablicy do aktualnego scope i zarezerwować rozmiar tablicy 
    */
    for(auto &sss : sTable.Scopes){
        if(sss.second == currentScope){
            for(auto &sAuto: sss.first){
                if(sAuto.name == *s){
                    //error
                }
            }
        }
    }


    variable v = variable{};
    v.name=*s;
    v.address=sTable.addressNumber;
    sTable.addressNumber+=sizeArray;
    v.isArg=false;
    v.isArray=true;
    for(auto &sss : sTable.Scopes){
        if(sss.second == currentScope){
            sss.first.push_back(v);
                }
    }
}


vector<instruction>* read(vector<instruction>* a){
    cout << "Debug read\n";
    vector <instruction>* vec = new vector<instruction>();
    instruction ins;

    ins.instructionName = "READ\n";
    vec->push_back(ins);
    vec->insert(vec->begin(),a->begin(),a->end());
    ins.instructionName = "STORE d\n";
    //cout << "Done\n";
    vec->push_back(ins);
          
    //cout << toString(vec);

    delete a;
    return vec;
}



vector<instruction>* write (vector<instruction>* a){
    cout << "debug write\n";

    vector<instruction>* asd = new vector<instruction>();

    asd->insert(asd->end(),a->begin(),a->end());
    instruction ins{};
    ins.instructionName="WRITE\n";
    asd->push_back(ins);

    delete a;
    return asd;
}

vector<instruction>* loadVar(string* s){
    cout << "Debug LoadVar\n";
    vector <instruction>* vec;
    cout << "                   current scope is " << currentScope << "\n";
    for(auto &sss : sTable.Scopes){
            cout << "sss: " << sss.second << "\n";
            if(sss.second == currentScope){
                for(auto &sAuto: sss.first){
                    cout << "         " <<*s << "\n";
                    if(sAuto.name == *s){
                         vec=loadNumber(sAuto.address);
                                if(sAuto.isArg){
                                instruction ins{};
                                ins.instructionName="LOAD a\n";
                                vec->push_back(ins);
                            }
                    }
                   
                }
            }
        }


    //delete s;
    return vec;
}

vector<instruction>* makeEmptyVec(){

    instruction ins{};
    vector <instruction>* vec= new vector<instruction>();
    return vec;
}



void saveProgram(string outputFile, vector<instruction>* i1, vector<instruction>* i2){
    cout <<"Debug saveProgram\n";
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();
   
    i1->insert(i1->end(),i2->begin(),i2->end());
    
    for (int i=0; i<i1->size(); i++)  {
            vec->push_back(i1->at(i));  
            
            size_t pos = (*vec)[i].instructionName.find("JUMP");
            if (pos != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(pos + 4); 
                int jumpNumber = stoi(numberStr);
                jumpNumber+=i;
                (*vec)[i].instructionName = "JUMP " + to_string(jumpNumber) + "\n";
            }

            size_t posi = (*vec)[i].instructionName.find("JZERO");
            if (posi != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(posi + 5); 
                int jumpNumber = stoi(numberStr);
                jumpNumber+=i;
                (*vec)[i].instructionName = "JZERO " + to_string(jumpNumber) + "\n";
            }

            size_t positio = (*vec)[i].instructionName.find("JPOS");
            if (positio != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(positio + 4); 
                int jumpNumber = stoi(numberStr);
                jumpNumber+=i;
                (*vec)[i].instructionName = "JPOS " + to_string(jumpNumber) + "\n";
            }

            cout << i<< " " << (*vec)[i].instructionName;
    }

    string output = toString(vec);
    ofstream myfile;
  	myfile.open(outputFile);

	myfile << output;
    myfile.close();

};

vector<instruction>* procedures (vector<instruction>* a,procedureHeader* b, vector<instruction>* c){
    cout <<"Debug procedures\n"; 
    instruction ins{};

    //obliczyć tu nr linii z pH
    if(a->empty()==true){
        cout <<"             empty!\n";
        ins.instructionName="JUMP 2\n";
        a->push_back(ins);
    } 
    

    a->insert(a->end(),c->begin(),c->end());

    b->lineCode=a->size();

    //sTable.listOfProcNames

    //wczesniej sklejone procedury, to co dokleić, 
        //W b mam adres, liczę a, i bede znać linijki
    //skleja wszystkie procedury w jeden blok kodu, i 
    //jumpy dokleić




    cout << toString(a);
    return a;
};

static bool boolll = false;
void setMain(){
     cout << "Debug setMain \n";
     
	if(!boolll) {
    sTable.Scopes.push_back(pair<vector<variable>, string>(vector<variable>(),std::string("mainScope")));
	boolll=true;
	}
    currentScope="mainScope";
};


vector<instruction>* merge(vector<instruction>*  a, vector<instruction>*  b){
    cout << "Debug Merge\n";
    a->insert(a->end(),b->begin(),b->end());
    delete b;
    return a;
};

vector<instruction>* assign(vector<instruction>*  a, vector<instruction>*  b){
    cout << "Debug assign\n";
     instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

     //cout << "\n\n" << "AAAA";
     //cout << toString(a) << "\n\n" << toString(b) << "\n\n";

    vec->insert(vec->end(),b->begin(),b->end());



    ins.instructionName= "PUT b\n";
    vec->push_back(ins);

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="RST a\n";
    vec->push_back(ins);

    ins.instructionName="GET b\n";
    vec->push_back(ins);

    ins.instructionName="STORE d\n";
    vec->push_back(ins);

    ins.instructionName="RST b\n";
    vec->push_back(ins);

    ins.instructionName="RST a\n";
    vec->push_back(ins);
    
    ins.instructionName="RST d\n";
    vec->push_back(ins);


    
    delete a;
    delete b;
    return vec;
};

vector<instruction>* ifElseFunction(vector<instruction>*  a, vector<instruction>*  b, vector<instruction>*  c){
    cout <<"Debug ifElse\n";
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    for (int i=0; i<a->size(); i++)  {
            vec->push_back(a->at(i));  
            
            size_t pos = (*vec)[i].instructionName.find("JZERO");
            if (pos != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(pos + 5);
                (*vec)[i].instructionName = "JZERO " + to_string(b->size()+2) + "\n";
            }

            size_t posit = (*vec)[i].instructionName.find("JPOS");
            if (posit != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(posit + 4);
                (*vec)[i].instructionName = "JPOS " + to_string(b->size()+2) + "\n";
            }
    }


    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="JUMP " + to_string(c->size()+1) + "\n";
    vec->push_back(ins);

    vec->insert(vec->end(),c->begin(),c->end());


    //cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";
    delete a;
    delete b;
    delete c;
    return vec;
};
vector<instruction>* ifFunction(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug if\n";                                                //DODAĆ ZAGNIEZDZONE IFY!!!!!!!!!!
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    for (int i=0; i<a->size(); i++)  {
            vec->push_back(a->at(i));  
            
            size_t pos = (*vec)[i].instructionName.find("JZERO");
            if (pos != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(pos + 5);
                (*vec)[i].instructionName = "JZERO " + to_string(b->size()+1) + "\n";
            }

            size_t posit = (*vec)[i].instructionName.find("JPOS");
            if (posit != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(posit + 4);
                (*vec)[i].instructionName = "JPOS " + to_string(b->size()+1) + "\n";
            }
    }


    vec->insert(vec->end(),b->begin(),b->end());


    //cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";
    delete a;
    delete b;
    return vec;
};
vector<instruction>* whileDoFunction(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug while do\n";

    instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

    for (int i=0; i<a->size(); i++)  {
            vec->push_back(a->at(i));  
            
            size_t pos = (*vec)[i].instructionName.find("JZERO");
            if (pos != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(pos + 5);
                (*vec)[i].instructionName = "JZERO " + to_string(b->size()+2) + "\n";
            }

            size_t posit = (*vec)[i].instructionName.find("JPOS");
            if (posit != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(posit + 4);
                (*vec)[i].instructionName = "JPOS " + to_string(b->size()+2) + "\n";
            }
    }


     vec->insert(vec->end(),b->begin(),b->end());

     ins.instructionName="JUMP -" + to_string(b->size()+a->size()) + "\n";
     vec->push_back(ins);

     //cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";

    delete a;
    delete b;
    return vec;
};

vector<instruction>* repeatUntilFunction(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug repeat until\n";
    instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

     vec->insert(vec->end(),a->begin(),a->end());
     
     for (int i=0; i<b->size(); i++)  {
            vec->push_back(b->at(i));  
            cout << "element: " << vec->at(i).instructionName <<"\n";
            size_t pos = (*vec)[i+a->size()].instructionName.find("JZERO");
            if (pos != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(pos + 5);
                (*vec)[i+a->size()].instructionName = "JZERO -" + to_string(a->size()+b->size()-1) + "\n";
            }

            size_t posit = (*vec)[i+a->size()].instructionName.find("JPOS");
            if (posit != string::npos) {
                string numberStr = (*vec)[i].instructionName.substr(posit + 4);
                (*vec)[i+a->size()].instructionName = "JPOS -" + to_string(a->size()+b->size()-1) + "\n";
                cout << "a.size, b.size, jpos:  " << a->size() << "|" << b->size() << "|" << (*vec)[i].instructionName <<"\n";
            }
    }



    cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";
    return vec;
};
vector<instruction>* proc_call_function(string* s,vector<string>* a, long long b){
    cout <<"Debug proc_call_func\n";

    vector<instruction>* c= new vector<instruction>();
    instruction ins{};

    cout << "s: " << *s << " vector: ";
    for(auto &sss: *a){
        cout  << sss << " | ";
    }
    cout << "long long : " << b << "\n";

    //do x a, do d b, w petli wszystkie ogarnac
    //jump i jump powrotu

    // for(auto &aa: a){
    //     for(auto &sss : sTable.Scopes){
    //         //cout << "hmm\n";
    //         if(sss.second == currentScope){
    //             for(auto &sAuto: sss.first){
    //                 if(sAuto.name == a){
    //                     //error jeśli istnieje
    //                     cout << "ERROR PANIC VAR\n";
    //                 }
    //             }
    //         }
    //     }
    // }
    procedureHeader* pH{};
    for(auto &listProc: sTable.listOfProcNames){
        if(listProc->procedureName==*s){
            pH=listProc;
        }
    }

    for(int i=0;i<a->size();i++){
        for(auto &sss : sTable.Scopes){
            if(sss.second == currentScope){
                for(auto &sAuto: sss.first){
                    if(sAuto.name==a->at(i)){  //ustawiamy current scope na procedury, szukamy adres, wpisujemy adres a i zapisuje do pamieci do adresu x
                        for(auto &autoAuto: sTable.Scopes){
                            if(autoAuto.second==*s){
                                for(auto &autoMoto: sss.first){
                                    cout << autoMoto.name << "   |  "<< autoMoto.address << " i: " << i << " \n";
                                    // cout << pH->arguments.at(i) << " | " << "\n";
                                    // cout << a->at(i) << "\n";

                                    if(autoMoto.name == pH->arguments.at(i)){
                                        
                                    }



                                    //c=loadNumber()
                                    //ins.




                                    


                                    // c=loadVar(autoMoto.name);
                                    // ins.instructionName="PUT h";
                                    // d=loadVar()
                                        //cout << autoMoto.name << "   |  "<< autoMoto.address << " i: " << i << " \n";
                                        //adres automoto - x, adres a - sAuto
                                        //autoMoto.address //to do rejestru h, ładuje adres a do g, musze zapisać wartosc g do pamieci w h  //+adres powrotu

                            }
                        }
                        

                    }
                }
            }
        }
    }
    }




    

    return c;
};
vector<pair<string*,bool>>* args_decl_function(vector<pair<string*,bool>>* a,string* s, bool b){
    cout <<"Debug args_decl_function2\n";
    pair<string*, bool> myPair;
     myPair.first =s;
     myPair.second=b;
     cout << "           a vector: " << *myPair.first << " | " << myPair.second << "\n";
    a->push_back(myPair);
    return a;
};
vector<pair<string*,bool>>* args_decl_function(string* s, bool b){
    cout <<"Debug args_decl_function1\n";
     pair<string*, bool> myPair;
     myPair.first =s;
     myPair.second=b;
    vector<pair<string*,bool>>* a = new vector<pair<string*,bool>>;
    cout << "           a vector: " << *myPair.first << " | " << myPair.second << "\n";
    a->push_back(myPair);
    
    return a;
};
vector<string>*  args_calling(vector<string>* a,string* s){
    cout <<"Debug args_calling1\n";
    a->push_back(*s);
    return a;
};
vector<string>*  args_calling(string* s){
    cout <<"Debug args_calling2\n";
    vector<string>* a = new vector<string>();
    a->push_back(*s);
    return a;
};
vector<instruction>* add(vector<instruction>*  a, vector<instruction>*  b){
    cout << "Debug add\n";
     instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="PUT b\n";
    vec->push_back(ins);


     vec->insert(vec->end(),b->begin(),b->end());

     ins.instructionName="ADD b\n";
    vec->push_back(ins);

    ins.instructionName="PUT b\n";
    vec->push_back(ins);


    //cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";
    delete a;
    delete b;
    return vec;
};
vector<instruction>* sub(vector<instruction>*  a, vector<instruction>*  b){

    cout << "Debug sub\n";
     instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

     vec->insert(vec->end(),b->begin(),b->end());
    

    ins.instructionName="PUT b\n";
    vec->push_back(ins);

    vec->insert(vec->end(),a->begin(),a->end());
     

     ins.instructionName="SUB b\n";
    vec->push_back(ins); 

    ins.instructionName="PUT b\n";
    vec->push_back(ins);

    //cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";
    delete a;
    delete b;
    return vec;
};
vector<instruction>* multi(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug multi\n";

    instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="PUT f\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="PUT g\n";
    vec->push_back(ins);

    ins.instructionName="RST e\n";
    vec->push_back(ins);

    ins.instructionName="JZERO 16\n";
    vec->push_back(ins);

    ins.instructionName="GET g\n";
    vec->push_back(ins);

    ins.instructionName="PUT b\n";
    vec->push_back(ins);

    ins.instructionName="SHR a\n";
    vec->push_back(ins);

    ins.instructionName="SHL a\n";
    vec->push_back(ins);

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    ins.instructionName="GET b\n";
    vec->push_back(ins);

    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="JZERO 4\n";
    vec->push_back(ins);

    ins.instructionName="GET e\n";
    vec->push_back(ins);

    ins.instructionName="ADD f\n";
    vec->push_back(ins);

    ins.instructionName="PUT e\n";
    vec->push_back(ins);

    ins.instructionName="SHL f\n";
    vec->push_back(ins);

    ins.instructionName="SHR g\n";
    vec->push_back(ins);

    ins.instructionName="GET g\n";
    vec->push_back(ins);

    ins.instructionName="JUMP -15\n";
    vec->push_back(ins);

    ins.instructionName="GET e\n";
    vec->push_back(ins);

    ins.instructionName="PUT b\n";
    vec->push_back(ins);

    ins.instructionName="RST e\n";
    vec->push_back(ins);

    ins.instructionName="RST f\n";
    vec->push_back(ins);


    //cout << "\n\n" << toString(a) << "\n\n" << toString(b) << "\n\n";
        delete a;
    delete b;
    return vec;
};
vector<instruction>* division(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug div\n";

    instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="PUT g\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="PUT h\n";
    vec->push_back(ins);

    ins.instructionName="PUT d\n";
    vec->push_back(ins);

    ins.instructionName="RST f\n";
    vec->push_back(ins);

    ins.instructionName="RST e\n";
    vec->push_back(ins);

    ins.instructionName="GET h\n";
    vec->push_back(ins);

    ins.instructionName="SUB g\n";
    vec->push_back(ins);

    ins.instructionName="JPOS 20\n";
    vec->push_back(ins);

    ins.instructionName="RST e\n";
    vec->push_back(ins);

    ins.instructionName="INC e\n";
    vec->push_back(ins);

    ins.instructionName="GET d\n";
    vec->push_back(ins);

    ins.instructionName="SUB g\n";
    vec->push_back(ins);

    ins.instructionName="JPOS 4\n";
    vec->push_back(ins);

    ins.instructionName="SHL d\n";
    vec->push_back(ins);

    ins.instructionName="SHL e\n";
    vec->push_back(ins);

    ins.instructionName="JUMP -5\n";
    vec->push_back(ins);

    ins.instructionName="SHR d\n";
    vec->push_back(ins);

    ins.instructionName="SHR e\n";
    vec->push_back(ins);

    ins.instructionName="GET f\n";
    vec->push_back(ins);

    ins.instructionName="ADD e\n";
    vec->push_back(ins);

    ins.instructionName="PUT f\n";
    vec->push_back(ins);

    ins.instructionName="GET g\n";
    vec->push_back(ins);

    ins.instructionName="SUB d\n";
    vec->push_back(ins);

    ins.instructionName="PUT g\n";
    vec->push_back(ins);

    ins.instructionName="GET h\n";
    vec->push_back(ins);

    ins.instructionName="PUT d\n";
    vec->push_back(ins);

    ins.instructionName="JUMP -21\n";
    vec->push_back(ins);

    ins.instructionName="GET f\n";
    vec->push_back(ins);

    ins.instructionName="PUT b\n";
    vec->push_back(ins);

    delete a;
    delete b;
    return vec;
};
vector<instruction>* mod(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug mod\n";
    instruction ins{};
     vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="PUT g\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="PUT h\n";
    vec->push_back(ins);

    ins.instructionName="PUT d\n";
    vec->push_back(ins);

    ins.instructionName="RST f\n";
    vec->push_back(ins);

    ins.instructionName="RST e\n";
    vec->push_back(ins);

    ins.instructionName="GET h\n";
    vec->push_back(ins);

    ins.instructionName="SUB g\n";
    vec->push_back(ins);

    ins.instructionName="JPOS 20\n";
    vec->push_back(ins);

    ins.instructionName="RST e\n";
    vec->push_back(ins);

    ins.instructionName="INC e\n";
    vec->push_back(ins);

    ins.instructionName="GET d\n";
    vec->push_back(ins);

    ins.instructionName="SUB g\n";
    vec->push_back(ins);

    ins.instructionName="JPOS 4\n";
    vec->push_back(ins);

    ins.instructionName="SHL d\n";
    vec->push_back(ins);

    ins.instructionName="SHL e\n";
    vec->push_back(ins);

    ins.instructionName="JUMP -5\n";
    vec->push_back(ins);

    ins.instructionName="SHR d\n";
    vec->push_back(ins);

    ins.instructionName="SHR e\n";
    vec->push_back(ins);

    ins.instructionName="GET f\n";
    vec->push_back(ins);

    ins.instructionName="ADD e\n";
    vec->push_back(ins);

    ins.instructionName="PUT f\n";
    vec->push_back(ins);

    ins.instructionName="GET g\n";
    vec->push_back(ins);

    ins.instructionName="SUB d\n";
    vec->push_back(ins);

    ins.instructionName="PUT g\n";
    vec->push_back(ins);

    ins.instructionName="GET h\n";
    vec->push_back(ins);

    ins.instructionName="PUT d\n";
    vec->push_back(ins);

    ins.instructionName="JUMP -21\n";
    vec->push_back(ins);

    ins.instructionName="GET g\n";
    vec->push_back(ins);

    ins.instructionName="PUT b\n";
    vec->push_back(ins);

    delete a;
    delete b;
    return vec;
};
vector<instruction>* isEqual(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug isEqual\n";

    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),a->begin(),a->end());
    

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="PUT d\n";
    vec->push_back(ins);


    ins.instructionName="RST a\n";
    vec->push_back(ins);

    ins.instructionName="GET c\n";
    vec->push_back(ins);

    ins.instructionName="SUB d\n";
    vec->push_back(ins);

    ins.instructionName="PUT e\n";
    vec->push_back(ins);



    ins.instructionName="GET d\n";
    vec->push_back(ins);

    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="ADD e\n";
    vec->push_back(ins);

    ins.instructionName="JPOS \n";
    vec->push_back(ins);

    delete a;
    delete b;
    return vec;
};
vector<instruction>* isNotEqual(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug isNotEqual\n";
    
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),a->begin(),a->end());
    

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="PUT d\n";
    vec->push_back(ins);


    ins.instructionName="RST a\n";
    vec->push_back(ins);

    ins.instructionName="GET c\n";
    vec->push_back(ins);

    ins.instructionName="SUB d\n";
    vec->push_back(ins);

    ins.instructionName="PUT e\n";
    vec->push_back(ins);



    ins.instructionName="GET d\n";
    vec->push_back(ins);

    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="ADD e\n";
    vec->push_back(ins);

    ins.instructionName="JZERO \n";
    vec->push_back(ins);

    delete a;
    delete b;
    return vec;
};
vector<instruction>* isMore(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug isMore\n";
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),b->begin(),b->end());
    

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="JZERO \n";
    vec->push_back(ins);



    delete a;
    delete b;
    return vec;
};
vector<instruction>* isLess(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug isLess\n";
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    
    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());
                                                          
    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="JZERO \n";
    vec->push_back(ins);



    delete a;
    delete b;
    return vec;
};
vector<instruction>* isMoreEqual(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug isMoreEqual\n";
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    
    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    vec->insert(vec->end(),b->begin(),b->end());

    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="JPOS \n";
    vec->push_back(ins);



    delete a;
    delete b;
    return vec;
};
vector<instruction>* isLessEqual(vector<instruction>*  a, vector<instruction>*  b){
    cout <<"Debug isLessEqual\n";


    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();

    vec->insert(vec->end(),b->begin(),b->end());
    

    ins.instructionName="PUT c\n";
    vec->push_back(ins);

    vec->insert(vec->end(),a->begin(),a->end());

    ins.instructionName="SUB c\n";
    vec->push_back(ins);

    ins.instructionName="JPOS \n";
    vec->push_back(ins);



    delete a;
    delete b;
    return vec;
};
vector<instruction>* loadNumber(long long num){
    long long number=0;
    cout << "Debug loadNumber\n";
    instruction ins{};
    vector <instruction>* vec = new vector<instruction>();
    ins.instructionName="RST d\n";
    vec->push_back(ins);
    while(number<num){
        ins.instructionName="INC d\n";
         vec->push_back(ins);
         number++;

         while(number*2<=num){               
            ins.instructionName="SHL d\n";
            vec->push_back(ins);
            number*=2;
            
         }
    }


    ins.instructionName="GET d\n";
    vec->push_back(ins);

    //cout << "\n" << toString(vec) << "\n";

    return vec;
};
void checkVar(string* s, bool b, long long a){

    cout <<"Debug checkVar\n";

    bool isIt =false;
    for(auto &sss : sTable.Scopes){
        if(sss.second == currentScope){
            for(auto &sAuto: sss.first){
                if(sAuto.name == *s && sAuto.isArray==b){
                    isIt=true;
                    
                }
            }
        }
    }
    if(isIt==false){
        //error jeśli nie istnieje
        cout << "ERROR PANIC VAR\n";
    }

};
vector<instruction>* loadArray(string *s, long long a){
    cout <<"Debug LoadArray1\n";
    
    instruction ins{};
    ins.instructionName=*s;
    vector <instruction>* vec;
    for(auto &sss : sTable.Scopes){
            if(sss.second == currentScope){
                for(auto &sAuto: sss.first){
                    if(sAuto.name == *s){
                         vec=loadNumber(sAuto.address+a);
                    }
                }
            }
        }
    delete s;
    return vec;
};
vector<instruction>* loadArray(string *s, string *r){
    cout <<"Debug loadArray2\n";
    
     instruction ins{};
     ins.instructionName=*s;
    vector <instruction>* vec;
     vector <instruction>* vec2;

         vec=loadVar(r);

        cout << "|||| " << toString(vec) << "\n";

    ins.instructionName="LOAD d\n";
    vec->push_back(ins);

    ins.instructionName="PUT c\n";
    vec->push_back(ins);
    
    vec2=loadVar(s);



    // for(auto &sss : sTable.Scopes){
    //         if(sss.second == currentScope){
    //             for(auto &sAuto: sss.first){
    //                 if(sAuto.name == *s){
    //                      vec2=loadNumber(sAuto.address);
    //                 }
    //             }
    //         }
    // }

    ins.instructionName="ADD c\n";
    vec2->push_back(ins);
    ins.instructionName="PUT d\n";
    vec2->push_back(ins);
    ins.instructionName="GET d\n";
    vec2->push_back(ins);


    vec->insert(vec->end(),vec2->begin(),vec2->end());


    //delete s;
    //delete r;
    return vec;
};

vector<instruction>* addHalt(vector<instruction>* a){
    cout << "Debug AddHalt\n";
    instruction ins{};
    ins.instructionName="HALT\n";
    a->push_back(ins);
    // ins.instructionName="JUMP 2\n";
    // a->insert(a->begin(), ins);
    ins.instructionName="RST b\n";
    a->insert(a->begin(), ins);
    ins.instructionName="RST a\n";
    a->insert(a->begin(), ins);
    return a;
};

