//  Hello World server

#include <czmq.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <stdio.h>

using namespace std;
typedef unordered_map <string,int> BankType;
BankType bank;

void dispatch(zmsg_t* incmsg, zmsg_t* outmsg) {
  zmsg_print(incmsg);
  char *operation = zmsg_popstr(incmsg);
  char *user = zmsg_popstr(incmsg);
//  cout << "Operation: " << operation << endl;
  
  if(strcmp(operation,"Crear") == 0) { 
    if (bank.count(user) > 0) {
      zmsg_addstr(outmsg,"Usuario ya existe!!!");
    } else {
      bank[user]=0;
      zmsg_addstr(outmsg,"Usuario creado!!!");
    }  
  } else if (strcmp(operation,"Depositar") == 0) {
    if (bank.count(user) > 0) {
      int temp=bank[user];
      char *c = zmsg_popstr(incmsg);
      int cantidad=atoi(c);
      
      bank[user]=temp+cantidad;
      zmsg_addstr(outmsg,"Deposito exitoso!");
      }else{
        zmsg_addstr(outmsg, "El usuario no existe!, Deposito fallido!!!");
      }
  } else if (strcmp(operation,"Retirar") == 0) {
    if (bank.count(user) > 0) {
      int temp=bank[user]; //saldo actual
      char *c = zmsg_popstr(incmsg);
      int cantidad=atoi(c);

      if(cantidad<=temp){
        bank[user]=temp-cantidad;
        zmsg_addstr(outmsg, "Retiro exitoso!");
      }else{
        zmsg_addstr(outmsg, "Retiro fallido, saldo insuficiente!");
      }
    }else{
        zmsg_addstr(outmsg, "El usuario no existe!, Deposito fallido!!!");
    }
  }else if (strcmp(operation,"Transferir") == 0) {
    char *user2= zmsg_popstr(incmsg);
    if (bank.count(user) > 0 && bank.count(user2)>0) {
      int temp=bank[user]; //saldo actual user1
      int temp2=bank[user2]; //saldo actual user2
      char *c = zmsg_popstr(incmsg);
      int cantidad=atoi(c);
      

      if(cantidad<=temp){
        bank[user]=temp-cantidad;
        bank[user2]=temp2+cantidad;
        zmsg_addstr(outmsg, "Transferencia exitosa!");
      }else{
        zmsg_addstr(outmsg, "Transferencia fallida, saldo insuficiente!");
      }
    }else{
        zmsg_addstr(outmsg, "Transferencia fallida, compruebe los usuarios!!!");
    }
  }else if (strcmp(operation,"Consultar") == 0) {

    if (bank.count(user) > 0) {
      int temp=bank[user];
      char cantidad[256];
      
      sprintf(cantidad,"%d",temp);
      
      zmsg_addstr(outmsg,cantidad);
      }else{
        zmsg_addstr(outmsg, "El usuario no existe!!");
      }
  }else {
    zmsg_addstr(outmsg, "Error!!");
  }
}


int main (void)
{
  zctx_t *context = zctx_new();
  void *responder = zsocket_new(context, ZMQ_REP);
  zsocket_bind(responder, "tcp://*:5555");
  
  while (1) {
    zmsg_t* request = zmsg_recv(responder);
    zmsg_t *response = zmsg_new();
    dispatch(request,response);
    zmsg_send(&response,responder);
    }
    
    zctx_destroy(&context);
    return 0;
}





