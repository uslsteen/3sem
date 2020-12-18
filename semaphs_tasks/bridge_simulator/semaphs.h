#ifndef __SEMAPHS_H__
#define __SEMAPHS_H__

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


#include <fcntl.h>


const int num_of_semaphors = 4;

const int MAX_PERMISSION  = 0777;
const int BUFFER_SIZE     = 4096;


struct Bridge_organization
{
  //! Vars for working with shared memory and semaphors

  int num_of_waiting_cars;
  int num_of_waiting_ships;

  //! Just nums of waiting ships and cars

  //! if bridge_cond  == 0 <=> bridge close
  //! if bridge_cond  == 1 <=> bridge open
  //! Default condition - bridge is closed
  int bridge_cond;
};


enum Semaphors
{
  SHIP,
  CAR,
  BRIDGE,
  SHRD_VAR
};


int Err_proc(char* err_string)
{
  perror(err_string);
  return errno;
}


//! Semaphors operation: p(c, 15) <=> minus 15 to semaphor c,
//!                      v(c, 15) <=> plus 15 to semaphor c,
//!                      z waiting for NULL


void P_oper(int sem_id, enum Semaphors sem_name, short int num)
{
    struct sembuf sem_struct[] = {{sem_name, (short int)-num, 0}};

    if (semop(sem_id, sem_struct, 1) < 0)
    {
        perror("sem op return not zero in P_oper!\n");
        exit(0);
    }
}

void V_oper(int sem_id, enum Semaphors sem_name, short int num)
{
    struct sembuf sem_struct = {sem_name, (short int)num, 0};

    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in V_oper!\n");
        exit(0);
    }
}

void Z_oper(int sem_id, enum Semaphors sem_name)
{
    struct sembuf sem_struct = {sem_name, 0, 0};
    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in Z_oper!\n");
        exit(0);
    }
}

#endif // __SEMAPHS_H__
