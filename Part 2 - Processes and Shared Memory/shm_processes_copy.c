#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ChildProcess(int []);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;

     if (argc != 5) {
          printf("Use: %s #1 #2 #3 #4\n", argv[0]);
          exit(1);
     }

     ShmID = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     //printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     //printf("Server has attached the shared memory...\n");

     //ShmPTR[0] = atoi(argv[1]);
     //ShmPTR[1] = atoi(argv[2]);
     //ShmPTR[2] = atoi(argv[3]);
     //ShmPTR[3] = atoi(argv[4]);
     //printf("Server has filled %d %d %d %d in shared memory...\n",
            // ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

     // starting my code here?
     // create bank account and turn variables
     int BankAccount = 0;
     int Turn = 0;
     int Deposit = 0;
     ShmPTR[0] = BankAccount;
     ShmPTR[1] = Turn;
     printf("Program is about to create a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ChildProcess(ShmPTR);
          wait(&status);
          exit(0);
     }
    else {
      // parent process
      for(int i = 0; i < 25; i++) {
        sleep(rand() % 5); // sleep between 0-5 seconds
        if(ShmPTR[1]==0){
          if(ShmPTR[0]<=100) {
            Deposit = rand() % 100;
            if(Deposit % 2 == 0) {
              // even
              ShmPTR[0] = ShmPTR[0] + Deposit;
              printf("Dear old Dad: Deposits $%d / Balance = $%d\n", Deposit, ShmPTR[0]);
            }
            else
              printf("Dear old Dad: Doesn't have any money to give\n");
          }
          ShmPTR[1] = 1;
        }
        //printf("Parent is on turn: %d\n", i);
      }
      
    }
  
      
     // completion of child
     //printf("Pprogram is done and will wait for child");
     wait(&status); // not needed
     //printf("Program has detected the completion of its child and finishes\n");
     //shmdt((void *) ShmPTR);
     //printf("Server has detached its shared memory...\n");
     //shmctl(ShmID, IPC_RMID, NULL);
     //printf("Server has removed its shared memory...\n");
     //printf("Server exits...\n");
     exit(0);
}

void  ChildProcess(int  SharedMem[])
{
     //printf("   Client process started\n");
     //printf("   Client found %d %d %d %d in shared memory\n",
     //           SharedMem[0], SharedMem[1], SharedMem[2], SharedMem[3]);
     //printf("   Client is about to exit\n");
     int balance;
     for(int i = 0; i < 25; i++) {
        sleep(rand() % 5); // sleep between 0-5 seconds
        if(SharedMem[1]==1) {
          balance = rand() % 50;
          printf("Poor Student needs $%d\n", balance);
          if(balance <= SharedMem[0]) {
            SharedMem[0] = SharedMem[0] - balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, SharedMem[0]);
          }
          else
            printf("Poor Student: Not Enough Cash ($%d)\n", SharedMem[0] );
        SharedMem[1] = 0;
        }
          //printf("Child is on turn: %d\n", i);
        }
      
      //sleep(rand() % 18); // virtually ensures completion of parent first
      exit(0);
     
}