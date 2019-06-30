#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
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
                              if (mudaop1 == 1) fs2 << "\t" << operando1;
                              if (mudaop2 == 1) fs2 << "\t" << operando2;
                              if(parametro > 3) {
                                for(j=4; j < parametro; j++) fs2 << "\t" << operando3[j-4];
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
int instrucao(string instrucao){
    if(instrucao=="add"){
      return 1;
    }
    else if(instrucao=="sub"){
      return 2;
    }
    else if(instrucao=="mult"){
      return 3;
    }
    else if(instrucao=="div"){
      return 4;
    }
    else if(instrucao=="jmp"){
      return 5;
    }
    else if(instrucao =="jmpn"){
      return 6;
    }
    else if(instrucao=="jmpp"){
      return 7;
    }
    else if(instrucao=="jmpz"){
      return 8;
    }
    else if(instrucao=="copy"){
      return 9;
    }
    else if(instrucao=="load"){
      return 10;
    }
    else if(instrucao=="store"){
      return 11;
    }
    else if(instrucao=="input"){
      return 12;
    }
    else if(instrucao=="output"){
      return 13;
    }
    else if(instrucao=="stop"){
      return 14;
    }
    else if (instrucao=="c_input") {
      return 15;
    }
    else if (instrucao=="c_output") {
      return 16;
    }
    else if (instrucao=="s_input") {
      return 19;
    }
    else if (instrucao=="s_output") {
      return 20;
    }
    else {
      return 0;
    }
}

int diretivas(string instrucao){
  if(instrucao == "space"){
    return 1;
  }
  else if(instrucao == "const"){
    return 2;
  }
  else if(instrucao == "section") {
    return 3;
  }
  else if(instrucao == "public") {
    return 4;
  }
  else if(instrucao == "extern") {
    return 5;
  }
  else if(instrucao == "begin") {
    return 6;
  }
  else if (instrucao == "end") {
    return 7;
  }
  else if (instrucao == "equ") {
    return 8;
  }
  else {
    return 0;
  }
}

void convertePseudoAssemblyIA32(int num_op, int num_dir, std::ofstream& fs,string rotulo, string operacao, string operando1, string operando2, int secao, string linha){
  if (linha.length() > 0) { // se a linha nao eh vazia
      if (rotulo.length() > 0) {
        if (secao == 0) {
          cout<<rotulo<<endl;
          fs << rotulo << endl;
        }
      }
      if (num_op == 1 || num_op == 2) { // add ou sub
        fs << "\t" << operacao << " eax" << ", " << "[" << operando1 << "]" << endl;
      }
      else if (num_op == 3) {
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "push  dword " << "[" << operando1 << "]";
        fs << "\t" << "call  multiplica"<<endl;
      }
      else if (num_op == 4) {
        fs << "\t" << "cdq" << endl;
        fs << "\t" << "mov  ebx," << "[" << operando1 << "]" << endl;
        fs << "\t" << "idiv  ebx" << endl;
      }
      else if (num_op == 5) { // jmp
        fs << "\t" << operacao << operando1 << endl;
      }
      else if (num_op == 6) { // jmpn
        fs << "\t" << "cmp" << "eax" <<  ", 0" << endl;
        fs << "\t" << "jl " << operando1 << endl;
      }
      else if (num_op == 7) { // jmpp
        fs << "\t" << "cmp" << "eax, " << "0" << endl;
        fs << "\t" << "jg " << operando1 << endl;
      }
      else if (num_op == 8) { // jmpz
        fs << "\t" << "cmp" << "eax, " << "0" << endl;
        fs << "\t" << "jz " << operando1 << endl;
      }
      else if (num_op == 9) { // copy
        fs << "\t" << "push  eax"<<endl;
        fs << "\t" << "mov  eax, [" <<  operando1 << "]" << endl;
        fs << "\t" << "mov [" << operando2 << "] ,eax" << endl;
        fs << "\t" << "pop  eax" << endl;
      }
      else if (num_op == 10) { // load
        fs << "\t" << "mov " << "eax, [" << operando1 << "]" << endl;
      }
      else if (num_op == 11) { // store
        fs << "\t" << "mov [" << operando1 << "], eax"<<endl;
      }
      else if (num_op == 12) { // input
        fs << "\t" << "push " << "eax" << endl;
        fs << "\t" << "mov " << "eax, " << "4" << endl;
        fs << "\t" << "mov " << "ebx, " << "1" << endl;
        fs << "\t" << "mov " << "ecx, " << "msg_input" << endl;
        fs << "\t" << "mov " << "edx, " << "tam_msgin" << endl;
        fs << "\t" << "int " << "80h" << endl;
        fs << "\t" << "mov " << "eax, " << "3" << endl;
        fs << "\t" << "mov " << "ebx, " << "0" << endl;
        fs << "\t" << "mov " << "ecx, " << "num_input" << endl;
        fs << "\t" << "mov " << "edx, " <<  "12" << endl;
        fs << "\t" << "int " <<  "80h" << endl;
        fs << "\t" << "push  eax" << endl;
        fs << "\t" << "push " << "num_input" << endl;
        fs << "\t" << "push " << "eax" << endl;
        fs << "\t" << "call " << "input" << endl;
        fs << "\t" << "mov [" << operando1 << "], eax" << endl;
        fs << "\t" << "pop " << "eax" << endl;
        fs << "\t" << "pop  eax" << endl;
      }
      else if (num_op == 13) { // output
        fs << "\t" << "push  eax" << endl;
        fs << "\t" << "push " << "eax" << endl;
        fs << "\t" << "push num_input" << endl;
        fs << "\t" << "push " << "dword, [" << operando1 << "]" << endl;
        fs << "\t" << "mov eax, 4" << endl;
        fs << "\t" << "mov ebx, 1" << endl;
        fs << "\t" << "mov ecx, msg_output" << endl;
        fs << "\t" << "mov edx, tam_msgout" << endl;
        fs << "\t" << "int 80h" << endl;
        fs << "\t" << "call output" << endl;
        fs << "\t" << "mov  eax,  4" << endl;
        fs << "\t" << "mov  ebx,  1" << endl;
        fs << "\t" << "mov  ecx,  endl" << endl;
        fs << "\t" << "mov  edx,  tam_endl"<<endl;
        fs << "\t" << "int  80h" << endl;
        fs << "\t" << "pop  eax" << endl;
        fs << "\t" << "pop eax" << endl;
      }
      else if (num_op == 14) { // stop ou return 0
        fs << "fim_prog:" << endl;
        fs << "\t" << "mov eax,  1" << endl;
        fs << "\t" << "mov ebx, 0" << endl;
        fs << "\t" << "int 80h" << endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "input:" << endl; // funcao de input
        fs << "\t" << "enter 0,0" << endl;
        fs << "\t" << "mov ecx, [ebp+8]"<< endl;
        fs << "\t" << "dec ecx" << endl;
        fs << "\t" << "xor esi, esi" << endl;
        fs << "\t" << "xor edi, edi"<< endl;
        fs << "\t" << "xor eax, eax" << endl;
        fs << "\t" << "mov ebx, [ebp+12]" << endl;
        fs << "\t" << "cmp byte [ebx], 0x2D" << endl;
        fs << "\t" << "jnz loopinput" << endl;
        fs << "\t" << "dec ecx" << endl;
        fs << "\t" << "inc esi" << endl;
        fs << "\t" << "inc edi" << endl;
        fs << "loopinput:" << endl;
        fs << "\t" << "cmp ecx, 1" << endl;
        fs << "\t" << "jl fim_linput" << endl;
        fs << "converte:" << endl;
        fs << "\t" << "xor edx, edx" << endl;
        fs << "\t" << "mov dl, [ebx+esi]" << endl;
        fs << "\t" << "sub dl, 0x30" << endl;
        fs << "\t" << "push ebx" << endl;
        fs << "\t" << "mov ebx, eax"<<endl;
        fs << "\t" << "add eax, ebx"<<endl;
        fs << "\t" << "add eax, ebx"<<endl;
        fs << "\t" << "add eax, ebx"<<endl;
        fs << "\t" << "add eax, ebx"<<endl;
        fs << "\t" << "sal eax, 1"<<endl;
        fs << "\t" << "mov ebx, edx"<<endl;
        fs << "\t" << "mov edx, ebx"<<endl;
        fs << "\t" << "add eax, edx"<<endl;
        fs << "\t" << "pop ebx"<<endl;
        fs << "\t" << "inc esi"<<endl;
        fs << "\t" << "dec ecx" << endl;
        fs << "\t" << "jmp loopinput"<<endl;
        fs << "fim_linput:"<<endl;
        fs << "\t" << "cmp edi, 1"<<endl;
        fs << "\t" << "jnz positivo_in"<<endl;
        fs << "negativo_in:"<<endl;
        fs << "\t" << "neg eax"<<endl;
        fs << "positivo_in:"<<endl;
        fs << "\t" << "mov [ebp+16], esi"<<endl;
        fs << "\t" << "leave"<<endl;
        fs << "\t" << "ret 8"<<endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "output:"<<endl; // funcao de output
        fs << "\t" << "enter 0,0" << endl;
        fs << "\t" << "mov  eax,  [ebp+8]" << endl;
        fs << "\t" << "mov  edi,  [ebp+12]" << endl;
        fs << "\t" << "xor  edx,  edx" << endl;
        fs << "\t" << "xor  ecx,  ecx" << endl;
        fs << "\t" << "xor  esi,  esi"<<endl;
        fs << "\t" << "mov  ebx,  10"<< endl;
        fs << "\t" << "cmp  eax,  0"<< endl;
        fs << "\t" << "jge  divide"<< endl;
        fs << "\t" << "neg  eax"<< endl;
        fs << "\t" << "add  esi,  1"<< endl;
        fs << "divide:"<< endl;
        fs << "\t" << "div  ebx"<< endl;
        fs << "\t" << "add  edx,  0x30"<< endl;
        fs << "\t" << "mov  byte  [edi+ecx],  dl"<< endl;
        fs << "\t" << "mov  edx,  0"<< endl;
        fs << "\t" << "cmp  eax,  0"<< endl;
        fs << "\t" << "je  fim_d"<< endl;
        fs << "\t" << "inc  ecx"<< endl;
        fs << "\t" << "jmp  divide"<< endl;
        fs << "fim_d:"<< endl;
        fs << "\t" << "mov  [ebp+16],  ecx"<< endl;
        fs << "\t" << "cmp  esi,  1"<< endl;
        fs << "\t" << "jne  positivo_out"<< endl;
        fs << "\t" << "inc  ecx"<< endl;
        fs << "\t" << "mov  byte  [edi+ecx],  0x2D"<< endl;
        fs << "positivo_out:"<< endl;
        fs << "\t" << "inc  dword  [ebp+16]"<< endl;
        fs << "\t" << "cmp  ecx,  0"<< endl;
        fs << "\t" << "jl  fimfunc"<< endl;
        fs << "\t" << "push  ecx"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  esi,  ecx"<< endl;
        fs << "\t" << "mov  ecx,  edi"<< endl;
        fs << "\t" << "add  ecx, esi"<< endl;
        fs << "\t" << "mov  edx,  1"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "pop  ecx"<< endl;
        fs << "\t" << "dec  ecx"<< endl;
        fs << "\t" << "jmp  positivo_out"<< endl;
        fs << "fimfunc:"<< endl;
        fs << "\t" <<"leave"<< endl;
        fs << "\t" << "ret  8"<< endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "s_input:"<< endl; // funcao de s_input
        fs << "\t" << "enter 0,0"<< endl;
        fs << "\t" << "mov  eax,  3"<< endl;
        fs << "\t" << "mov  ebx,  0"<< endl;
        fs << "\t" << "mov  ecx,  [ebp+16]"<< endl;
        fs << "\t" << "mov  edx,  100"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "mov  ebx,  [ebp+16]"<< endl;
        fs << "\t" << "mov  eax,  [ebp+12]"<< endl;
        fs << "\t" << "mov  ecx,  [ebp+8]"<< endl;
        fs << "\t" << "xor  esi,  esi"<< endl;
        fs << "transcreve:"<< endl;
        fs << "\t" << "cmp  ecx,  1"<< endl;
        fs << "\t" << "jl  fim_transcreve"<< endl;
        fs << "\t" << "mov  dl,  [ebx+esi]"<< endl;
        fs << "\t" << "mov  [eax+esi],  dl"<< endl;
        fs << "\t" << "inc  esi"<< endl;
        fs << "\t" << "dec  ecx"<< endl;
        fs << "\t" << "jmp  transcreve"<< endl;
        fs << "fim_transcreve:"<< endl;
        fs << "\t" << "mov  byte  [eax+esi],  0ah"<< endl;
        fs << "\t" << "mov  [ebp+20],  esi"<< endl;
        fs << "\t" << "leave"<< endl;
        fs << "\t" << "ret  12"<< endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "s_output:"<< endl; // funcao de s_output
        fs << "\t" << "enter  0,0"<< endl;
        fs << "\t" << "mov  ebx,  [ebp+12]"<< endl;
        fs << "\t" << "mov  esi,  [ebp+8]"<< endl;
        fs << "\t" << "xor  ecx,  ecx"<< endl;
        fs << "printa:"<< endl;
        fs << "\t" << "cmp  byte  [ebx+ecx],  0ah"<< endl;
        fs << "\t" << "jz  fim_printa"<< endl;
        fs << "\t" << "cmp  esi,  1"<< endl;
        fs << "\t" << "jl  fim_printa"<< endl;
        fs << "\t" << "push  ecx"<< endl;
        fs << "\t" << "push  ebx"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "add  ecx,  ebx"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  edx,  1"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "pop  ebx"<< endl;
        fs << "\t" << "pop  ecx"<< endl;
        fs << "\t" << "dec  esi"<< endl;
        fs << "\t" << "inc  ecx"<< endl;
        fs << "\t" << "jmp  printa"<< endl;
        fs << "fim_printa:"<< endl;
        fs << "\t" << "mov  [ebp+16],  ecx"<< endl;
        fs << "\t" << "leave"<< endl;
        fs << "\t" << "ret  8"<< endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "c_input:"<< endl; // funcao de c_input
        fs << "\t" << "enter  0,0"<< endl;
        fs << "\t" << "mov  eax,  3"<< endl;
        fs << "\t" << "mov  ebx,  0"<< endl;
        fs << "\t" << "mov  ecx,  [ebp+12]"<< endl;
        fs << "\t" << "mov  edx,  100"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "mov  ecx,  [ebp+12]"<< endl;
        fs << "\t" << "mov  al,  [ecx]"<< endl;
        fs << "\t" << "mov  ebx,  [ebp+8]"<< endl;
        fs << "\t" << "mov  [ebx],  al"<< endl;
        fs << "\t" << "mov  dword  [ebp+16],  1"<< endl;
        fs << "\t" << "leave"<< endl;
        fs << "\t" << "ret  8"<< endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "c_output:"<< endl; // funcao de c_output
        fs << "\t" << "enter  0,0"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  [ebp+8]"<< endl;
        fs << "\t" << "mov  edx,  1"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "mov  dword  [ebp+12],  1"<< endl;
        fs << "\t" << "leave"<< endl;
        fs << "\t" << "ret  4"<< endl;
        fs << endl;
        fs << endl;
        fs << endl;

        fs << "multiplica:"<< endl; // multiplicacao
        fs << "\t" << "enter  0,0"<< endl;
        fs << "\t" << "mov  eax,  [ebp+12]"<< endl;
        fs << "\t" << "mov  ebx,  [ebp+8]"<< endl;
        fs << "\t" << "imul  ebx"<< endl;
        fs << "\t" << "cmp  edx,  0"<< endl;
        fs << "\t" << "jnz  testa1"<< endl;
        fs << "\t" << "jmp  nao_overflow"<< endl;
        fs << "testa1:"<< endl;
        fs << "\t" << "cmp  edx,  -1"<< endl;
        fs << "\t" << "jz  nao_overflow"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  msgoverflow"<< endl;
        fs << "\t" << "mov  edx,  tam_msgover"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "jmp  fim_prog"<< endl;
        fs << "nao_overflow:"<< endl;
        fs << "\t" << "leave"<< endl;
        fs << "\t" << "ret  8"<< endl;
        fs << endl;
        fs << endl;
        fs << endl;
      }
      else if (num_op == 15) { // c_input
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl ;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  msg_cinput"<< endl;
        fs << "\t" << "mov  edx,  tam_msgcin"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "push  string_in"<< endl ;
        fs << "\t" << "push" << operando1 << endl ;
        fs << "\t" << "call  c_input"<< endl ;
        fs << "\t" << "pop  eax"<< endl;
        fs << "\t" << "pop  eax"<< endl;
      }
      else if (num_op == 16)  { // c_output
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  msg_coutput"<< endl;
        fs << "\t" << "mov  edx,  tam_msgcout"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "push" << "eax"<< endl;
        fs << "\t" << "push" << operando1<< endl;
        fs << "\t" << "call  c_output"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  endl"<< endl;
        fs << "\t" << "mov  edx,  tam_endl"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "pop  eax"<< endl;
        fs << "\t" << "pop  eax"<< endl;
      }
      else if (num_op == 19) { //s_input
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  msg_sinput"<< endl;
        fs << "\t" << "mov  edx,  tam_msgsin"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "push  string_in"<< endl;
        fs << "\t" << "push" <<  operando1<< endl;
        fs << "\t" << "mov  ecx,  eax"<< endl;
        fs << "\t" << "push  ecx"<< endl;
        fs << "\t" << "call  s_input"<< endl;
        fs << "\t" << "pop  eax"<< endl;
        fs << "\t" << "pop  eax"<< endl;
      }
      else if (num_op == 20) { // s_output
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  msg_soutput"<< endl;
        fs << "\t" << "mov  edx,  tam_msgsout"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "push  eax"<< endl;
        fs << "\t" << "push" <<  operando1<< endl;
        fs << "\t" << "mov  ecx," <<  operando2<< endl;
        fs << "\t" << "push  ecx"<< endl;
        fs << "\t" << "call  s_output"<< endl;
        fs << "\t" << "mov  eax,  4"<< endl;
        fs << "\t" << "mov  ebx,  1"<< endl;
        fs << "\t" << "mov  ecx,  endl"<< endl;
        fs << "\t" << "mov  edx,  tam_endl"<< endl;
        fs << "\t" << "int  80h"<< endl;
        fs << "\t" << "pop  eax"<< endl;
        fs << "\t" << "pop  eax"<< endl;
      }
      else if (num_op == 0 && num_dir !=0) {
            if (num_dir == 1) { // space
              if (operando1.length()==0) { // vetor
                fs << "\t" << rotulo << "\tresd\t" << "1"<< endl;
              }
              else {
                fs << "\t" << rotulo << "\tresd\t" << operacao << endl;
              }
            }
            else if (num_dir == 2) {
              fs << "\t" << rotulo << "\tdd\t" << operando1<< endl;
            }
            else if (num_dir == 3) { // section
              fs << operacao << "\t.\t" << operando1<< endl;
              if (operando1 == "data"){
                fs << "\t" << "msg_input\tdb\t'input: ' "<< endl;
                fs << "\t" << "tam_msgin\tEQU\t$-msg_input"<< endl;
                fs << "\t" << "msg_output\tdb\t'output: ' "<< endl;
                fs << "\t" << "tam_msgout\tEQU\t$-msg_output"<< endl;
                fs << "\t" << "msg_cinput\tdb\t'c_input: '"<< endl;
                fs << "\t" << "tam_msgcin\tEQU\t$-msg_cinput"<< endl;
                fs << "\t" << "msg_coutput\tdb\t'c_output: '"<< endl;
                fs << "\t" << "tam_msgcout\tEQU\t$-msg_coutput"<< endl;
                fs << "\t" << "msg_sinput\tdb\t's_input: '"<< endl;
                fs << "\t" << "tam_msgsin\tEQU\t$-msg_sinput"<< endl;
                fs << "\t" << "msg_soutput\tdb\t's_output: '"<< endl;
                fs << "\t" << "tam_msgsout\tEQU\t$-msg_soutput"<< endl;
                fs << "\t" << "endl\tdb\t0dh,0ah"<< endl;
                fs << "\t" << "tam_endl\tEQU\t$-endl"<< endl;
                fs << "\t" << "msgoverflow\tdb\t'OVERFLOW!'"<< endl;
                fs << "\t" << "tam_msgover\tEQU\t$-msgoverflow"<< endl;
              }
              if (operando1 == "bss") {
                fs << "\t" << "num_input" << " resb" << " 10"<< endl;
                fs << "\t" << "char_in\tresb\t1"<< endl;
                fs << "\t" << "string_in\tresb\t100"<< endl;
              }
              if (operando1 == "text") {
                fs << "global _start"<< endl;
                fs << "_start:"<< endl;
              }
            }
      }
  }
}

int main (int argc, char *argv[]){

  int i=0, parametro=1, mudaadic=0, mudarot=0, mudaop=0, mudaop1=0, mudaop2=0, num_op, num_dir, letrint, secao=0;
  int num_space=0, op = 0, dir = 0;
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
                  size_t found_dois_pontos = linha.find(":",0);// Acha se tem rotulo
                  if(found_dois_pontos!=std::string::npos)rotulo = linha.substr(0, found_dois_pontos);
                  std::size_t found_primeiro_espaco = linha.find("\t",0);
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
                  operando1 = linha.substr(found+1, linha.length());
              }
              if(operacao == operando1){
                  operando1 = operando2;
                  operando2.erase(operando2.begin(), operando2.end());
                  operacao.erase(std::remove(operacao.begin(), operacao.end(), '\t'), operacao.end());
              }
              cout<<i<<"\t"<<linha<<endl;
              i++;
              cout<<"op "<<operacao<<endl;
              cout<<"op1 "<<operando1<<endl;
              cout<<"op2 "<<operando2<<endl;
              op = instrucao(operacao);
              dir = diretivas(operacao);
              //cout<<op<<endl;
              //cout<<dir<<endl;
              //cout<<"------------"<<endl;
              if (dir == 3) {
                  if (operando1 == "text"){
                      secao = 0;
                  }
                  else if(operando1 == "data") {
                      secao = 1;
                  }
                  else if(operando1 == "bss") {
                    secao = 2;
                }
              }
              convertePseudoAssemblyIA32(op, dir, fs2, rotulo, operacao, operando1, operando2, secao, linha);
              op=0;
              dir=0;
              rotulo.erase(rotulo.begin(), rotulo.end());
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
