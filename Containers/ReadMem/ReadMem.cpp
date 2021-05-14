#include "ReadMem.h"
#include "../test_struct.h"
#include <fstream>
#include <iostream>

using namespace std;
ReadMem::ReadMem()///Construtor
{
    this->data = new PosixShMem("SH_MEM",sizeof(TIMESTAMPED_TEST_DATA));//é o que a thread vai ler
    this->startActivity();  /// Inicio a thread
}

ReadMem::~ReadMem() ///Destrutor
{
    this->stopActivity(); ///Finalizo a thread
    delete this->data;/// deleto o espaço alocado
}

void ReadMem::startActivity()
{
    ThreadBase::startActivity();
}

void ReadMem::stopActivity()
{
    ThreadBase::stopActivity();
    std::cout << "READ" << std::endl;
}

int ReadMem::run()
{
    this->is_running = 1;
    this->is_alive = 1;
    this->tim1.tv_sec = 0;
    this->tim1.tv_nsec = 100000000L;//10Hz
    
    TIMESTAMPED_TEST_DATA my_data;

    ofstream arquivo1;
    arquivo1.open("Velocidade_saida",ios::app);  

    while(this->is_alive)
    {
        //std::cout<<" teste"<<std::endl;
        this->data->read(&my_data, sizeof(TIMESTAMPED_TEST_DATA));
        std::cout<<" Velocidade: "<<my_data.velocidade<<std::endl;
        arquivo1 << my_data.velocidade<<endl;
        std::cout <<  "TEMPO: " << my_data.time1 << std::endl;

        //Realizará a leitura da memória e apresentará os dados no terminal
        //"Cont: " << my_data.data.contador << "
        nanosleep(&this->tim1, &this->tim2);
    }
    arquivo1.close();
    this->is_running = 0;
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}