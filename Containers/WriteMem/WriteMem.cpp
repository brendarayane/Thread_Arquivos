#include "WriteMem.h"
#include "../test_struct.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <sstream>
#include <utility>  
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;

int Vetor_velocidades[100];
int cont_aux=0;

int ReadFiles(const char* name) {
    int aux;
    fstream Arquivo(name, ios::in);
    if (!Arquivo) {
        cout << "Error to read the file " << name << endl;                //Caso não exista o arquivo retorna error
        return (1);
    }  
    while (Arquivo>>aux){
        cont_aux++;
    }
    Arquivo.clear();// Limpo para o proximo uso
    Arquivo.seekg(0,ios::beg);// posiciono o arquivo para o inicio
    
        for(int i=0;i<cont_aux;i++){
            Arquivo >> aux;
            Vetor_velocidades[i]=aux;
        }

    Arquivo.close();
}


WriteMem::WriteMem()
{
    
    
    this->data = new PosixShMem("SH_MEM",sizeof(TIMESTAMPED_TEST_DATA));
    this->startActivity();
}

WriteMem::~WriteMem()
{
    this->stopActivity();
}

void WriteMem::startActivity()
{
    ThreadBase::startActivity();
}

void WriteMem::stopActivity()
{
    ThreadBase::stopActivity();
    std::cout << "WRITE" << std::endl;
}

int WriteMem::run()
{

    ReadFiles("M");
    
    this->is_running = 1;
    this->is_alive = 1;
    this->tim1.tv_sec = 0;
    this->tim1.tv_nsec = 100000000L;//10Hz

    TIMESTAMPED_TEST_DATA my_data;
    int my_cont = -1;
    int cont=0;
   // char sentido;
    
    while(this->is_alive)
    {   
            
        
        if(cont<cont_aux){
                my_data.velocidade = Vetor_velocidades[cont]; 
                }else{
             my_data.velocidade =0;
          } 
        cont++;
        
        //if(sentido=='a'){
            //cont++;
        //}
            my_data.data.contador = my_cont++;
            my_data.time = road_time();
            this->data->write(&my_data, sizeof(TIMESTAMPED_TEST_DATA));
            //std:: cout<< "Escrita"<< std:: endl;
            nanosleep(&this->tim1, &this->tim2);
       
       
    }
    this->is_running = 0;
    pthread_exit(NULL);
    
    return EXIT_SUCCESS;
}
