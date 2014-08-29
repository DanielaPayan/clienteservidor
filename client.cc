//  Hello World client
#include <czmq.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
 
  zctx_t *context = zctx_new();
  void *requester = zsocket_new(context, ZMQ_REQ);
  zsocket_connect(requester, "tcp://localhost:5555");

  // Creates an empty message
  zmsg_t* request = zmsg_new();
  
  if (strcmp(argv[1],"crear") == 0) {
    zmsg_addstr(request,"Crear");
    zmsg_addstr(request,argv[2]);
  } else if (strcmp(argv[1],"depositar") == 0) {
    zmsg_addstr(request,"Depositar");
    zmsg_addstr(request,argv[2]);
    zmsg_addstr(request,argv[3]);
  }else if (strcmp(argv[1],"retirar") == 0) {
    zmsg_addstr(request,"Retirar");
    zmsg_addstr(request,argv[2]);
    zmsg_addstr(request,argv[3]);
  }else if (strcmp(argv[1],"transferir") == 0) {
    zmsg_addstr(request,"Transferir");
    zmsg_addstr(request,argv[2]); //Us1
    zmsg_addstr(request,argv[3]); //Us2
    zmsg_addstr(request,argv[4]); //Cantidad
  }else if (strcmp(argv[1],"consultar") == 0) {
    zmsg_addstr(request,"Consultar");
    zmsg_addstr(request,argv[2]); //Us1 
  }else {
    cout << "error!\n";
  } 
  
  // Sends message request through socket requester
  zmsg_send(&request,requester);
  
  zmsg_t* resp = zmsg_recv(requester);
  
  zmsg_print(resp);      
         
  zctx_destroy(&context);
 
  return 0;
}

