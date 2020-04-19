#ifndef SIGNAL_H
#define SIGNAL_H

#include <signal.h>


typedef void Sigfunc(int);

Sigfunc *signal(int signo, Sigfunc *func);


void sig_chld(int signo);


#endif //SIGNAL_H
