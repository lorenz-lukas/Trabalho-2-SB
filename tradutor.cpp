#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

using namespace std;

typedef struct diretiva {
  string nome;
  int valor;
} diretiva;

void preprocessing(string in_file, string out_file){
  string linha, palavra, rotulo, operacao, operando1, operando2, adicionado,operando3;
  char letra, letra_anterior;
  int i, j,letrint, parametro, tam_rot, mudarot=0, mudaop=0, mudaop1=0, mudaop2=0, ignoralinha=0, mudaadic=0, contadorlinha=1;
  int temp = 0;
  bool ok = false;
  list<diretiva> tab_preproc;
  diretiva elemento_preproc;
  list<diretiva>::iterator iterador_p;

  ifstream fs1 (in_file);
  ofstream fs2 (out_file);
  if (fs1.is_open() and fs2.is_open())
  {
      parametro=1;
      i=0;
      while(!fs1.eof()){
        getline(fs1, linha);
        size_t found = linha.find(";");
        if(found != string::npos) linha = linha.substr(0, found);
        //palavra.erase(palavra.begin(), palavra.end());
        for(i = 0; i < linha.length(); i++){
            letra_anterior = letra;
            letra = linha[i];
            ok = false;
            if(letra == '+' && (letra_anterior == '\t'|| letra_anterior == ' ')){ // se o char for + e a letra anterior for espaco, eh porque a palavra anterior ja foi preenchida
              mudaadic = 1;
            }

            if(letra != ' ' && letra != '\t'  && letra != '+' && letra != ':'){ //verifica se o char faz parte de alguma palavra//
                if (letra > 64 && letra < 91) letra = letra + 32;
                palavra += letra;
                if(((i+1)==linha.length() || linha[i+1] == ' ' || linha[i+1] == ':' || linha[i+1] == '\t')){
                    ok=true;
                    letra = linha[i+1];
                }
            }
            if(ok){
                //cout<< palavra << endl;
                if ((letra == ' ' || letra == '\n' || (i+1)==linha.length() || letra == '\t' || letra == ':' || letra == '+')){ //verifica se eh termino de palavra//
                  if (letra == ':'){ //eh rotulo?//
                      mudarot = mudarot + 1;
                      rotulo = palavra;
                      cout<<"Rotulo: "<< rotulo << endl;
                  }else if (mudaadic == 1) {
                      adicionado =  palavra;
                      cout<< "Adicionado:\t" << adicionado << endl;
                  }else{
                      if (letra == '+') {
                        mudaadic = 1;
                      }
                      if (parametro == 1 && mudaop == 0){
                        mudaop = 1;
                        operacao = palavra;
                        cout<< "Operação:\t" << operacao << endl;
                      }
                      if (parametro == 2 && mudaop1 == 0){
                        mudaop1 = 1;
                        operando1 = palavra;
                        size_t found = operando1.find(",");
                        if(found != string::npos) operando1 = operando1.substr(0, found);
                        cout<< "Operando1:\t" << operando1 << endl;
                      }
                      if (parametro == 3 && mudaop2 == 0){
                        mudaop2 = 1;
                        operando2 = palavra;
                        cout << "Operando 2:\t" << operando2 << endl;
                      }
                      if (parametro > 3) {
                        operando3.append(palavra);
                      }
                      parametro = parametro + 1;
                    }
                    //i=0;
                }

                if ((i+1)==linha.length() && mudaop == 1){ //se a linha terminou//
                  if (mudaop == 1){
                    if (!operacao.compare("equ")){ // se for equ, armazena nome do rotulo e valor
                        cout << "Rotulo:\t" << rotulo << endl;
                        elemento_preproc.nome = rotulo;
                        stringstream geek(operando1); //corrigir o inteiro dado pela tabela ascii
                        geek >> elemento_preproc.valor;
                        cout << "Elemento_nome:\t" << elemento_preproc.nome << endl;
                        cout << "valor:\t" << elemento_preproc.valor << endl;
                        tab_preproc.push_back(elemento_preproc);
                    }
                    else if (!operacao.compare("if")){
                        iterador_p = tab_preproc.begin();
                        while(iterador_p->nome.compare(rotulo)!=0 && iterador_p != tab_preproc.end()){ //tenta achar rotulo na lista de simbolos//
                          cout<<"iterador->nome: "<< iterador_p->nome << "rotulo: " << rotulo << endl;
                          iterador_p++;
                        }
                        if (iterador_p == tab_preproc.end()){
                          cout<<"[ERRO] Rotulo nao encontrado!!!" << endl;
                        }
                        else {
                          if (iterador_p->valor != 1) {
                            ignoralinha = 1;
                          }
                        }
                    }
                    else{
                          if(ignoralinha == 0){
                              cout<< "mudarot: \t"<< mudarot << endl;
                              if (mudarot > 0) fs2 << rotulo + ":" + "\t";
                              fs2 << operacao;
                              if (mudaop1 == 1) fs2 << "\t" + operando1;
                              if (mudaop2 == 1) fs2 << "\t" + operando2;
                              if(parametro > 3) {
                                for(j=4; j < parametro; j++) fs2 << "\t" + operando3[j-4];
                              }
                              if (mudaadic == 1) fs2 << "+" + adicionado;
                              fs2 << endl;
                         }
                         ignoralinha = 0;
                    }
                  }
                  contadorlinha++;
                  parametro = 1;
                  cout<<endl;
                  mudarot = 0; mudaop = 0; mudaop1 = 0; mudaop2 = 0; mudaadic = 0;
                }
                palavra.erase(palavra.begin(), palavra.end());
            }
         }
      }
      fs1.close();
      fs2.close();
  }else{
     cout << "[ERROR] Unable to open files!" << endl;
     exit(0);
  }
  printf ("\nfim pre processamento!");
  printf("\n -------------------------------------------- \n");
}

int main (int argc, char *argv[]){

  int i=0, parametro=1, mudaadic=0, mudarot=0, mudaop=0, mudaop1=0, mudaop2=0, num_op, num_dir, letrint, secao=0;
  int num_space=0;
  char letra;
  bool ok = false;
  string arquivo_input, arquivo_output, arquivo_precomp, linha, token;
  string rotulo, operacao,operando1, operando2, adicionado;
  if(argc != 2) {
    cout<< "[ERROR] Input parameters must be 2.\nExample: ./tradutor entrada.asm" << endl;
    return 0;
  }

  std::size_t found = string(argv[1]).find(".");
  if(found!=std::string::npos){
      arquivo_input = "infile/" +string(argv[1]);
      arquivo_precomp = "outfile/" + string(argv[1]).substr(0,found) + ".pre";
      arquivo_output = "outfile/" + string(argv[1]).substr(0,found) + ".s";
  }else{
      cout<<"[ERROR] Wrong file input name." << endl;
  }

  preprocessing(arquivo_input, arquivo_precomp);

  ifstream fs1 (arquivo_precomp);//precomp_file
  ofstream fs2 (arquivo_output);

  if (fs1.is_open() and fs2.is_open())
  {
      while(!fs1.eof()){
          getline(fs1, linha);
          if(linha.length()){
              found = linha.find("section");
              if(found==std::string::npos){// Pula linhas com section
                //cout<<linha<<endl;
                size_t found_dois_pontos = linha.find(":",0);// Acha se tem rotulo
                if(found!=std::string::npos)rotulo = linha.substr(0, found_dois_pontos);
                std::size_t found_primeiro_espaco = linha.find("\t",0);// Acha se tem rotulo

                if(found_primeiro_espaco!=std::string::npos){
                  std::size_t found_segundo_espaco = linha.find("\t",found_primeiro_espaco+1);// Checa se tem mais de um argumento
                  if(found_dois_pontos!=std::string::npos)operacao = linha.substr(found_primeiro_espaco+1, found_segundo_espaco-5);
                  else operacao = linha.substr(0, found_primeiro_espaco);
                  if(found_segundo_espaco!=std::string::npos){
                    operando1 = linha.substr(found_primeiro_espaco+1, found_segundo_espaco-5);
                    operando2 = linha.substr(found_segundo_espaco+1, linha.length() );
                  }else{
                    operando1 = linha.substr(found_primeiro_espaco+1, linha.length() );
                  }
                }else operacao = linha.substr(0, linha.length() ); //stop
              }else{
                found = linha.find("\t");
                operacao = linha.substr(0, found);
              }
              if(operacao == operando1){
                operando1 = operando2;
                operando2.erase(operando2.begin(), operando2.end());
              }
              //cout<<i<<"\t"<<linha<<endl;
              //i++;
              //cout<<"op "<<operacao<<endl;
              //cout<<"op1 "<<operando1<<endl;
              //cout<<"op2 "<<operando2<<endl;
                      /*num_op = descobreinstrucao(operacao);
                      num_dir = descobrediretiva(operacao);
                      if (num_dir == 3) {
                      if (strcmp(operando1,"text") == 0) {
                      secao = 0;
                    }
                    else if (strcmp(operando1,"data") == 0) {
                    secao = 1;
                  }
                  else if (strcmp(operando1,"bss") == 0) {
                  secao = 2;
                }
              }
              converteia32(num_op, num_dir, entrada, saida, rotulo, operacao, operando1, operando2, adicionado, mudaop, mudaadic, mudarot, secao);
              parametro = 1;
              mudarot = 0; mudaop = 0; mudaop1 = 0; mudaop2 = 0; mudaadic = 0;
              cout << endl;*/
              operacao.erase(operacao.begin(), operacao.end());
              operando1.erase(operando1.begin(), operando1.end());
              operando2.erase(operando2.begin(), operando2.end());
          }
      }
      fs1.close();
      fs2.close();
  } else cout << "[ERROR] Unable to open files!" << endl;
  return 0;
}
