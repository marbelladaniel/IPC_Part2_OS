#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  StudentProcess(int []);
void  DearOldDad(int []);
int Turn=0;  //shared,global var 

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
      

     
     srandom(getpid()); //to seed random function
     

     if (argc != 3) {
         printf("Use: %s 0 0\n", argv[0]);//takes in the name of the prgm 
        
     }
  
     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
      //error checkers
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
  
      //else
     printf("Server has received a shared memory of two integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
  
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
      //else
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = atoi(argv[1]); //BankAcoount (atoi= converts string to int )
     ShmPTR[1] = atoi(argv[2]);//Turn var 
    
     printf("Server has filled %d %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1]); //puts 4 ints into shared mem ptr arr 

     printf("Server is about to fork a child process...\n");
  
     pid = fork(); //creates a proc 
     
  //error checker 
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) { //child proc -student 
          StudentProcess(ShmPTR); //call function below 
          exit(0);
     }
    else{
      //else call parent proc here 
      DearOldDad(ShmPTR); }
  
     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  DearOldDad(int  SharedMem[])
{
    int i = 0;
    int BankAccount = SharedMem[0];
    int balance; 
    
   
    for(i=0; i<25;i++)
    {
      sleep(random() %5); //student goes to sleep for random time 0-5s
      
      //withdraws money from BA
      
        while(SharedMem[1]!=0)
        { //Turn !=0
          //do nothing 
        }
      BankAccount = SharedMem[0];
      
       if(BankAccount<=100)
       {
        balance=random() % 100; //get random amt of money 
        
          if(balance % 2 == 0)
          {//even 
            BankAccount = BankAccount + balance; //deposit balance to BA
            printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, BankAccount);
          }
          else
          {//odd 
            printf("Dear old Dad: Doesn't have any money to give\n");
          }
       }
      else
      {
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", BankAccount);
      }
      
      SharedMem[0]=BankAccount;  //we modif the copy , now put it back into shared mem here 
      SharedMem[1]=1;  //Turn=1
    }

}




void  StudentProcess(int  SharedMem[])
{
    int i = 0;
    int BankAccount = SharedMem[0];
    int balanceNeed; 
    
   //child proc created here 
    for(i=0; i<25;i++)
    {
      sleep(random() %5); //student goes to sleep for random time 0-5s
      
      //withdraws money from BA
      
        while(SharedMem[1]!=1)
        {
          //do nothing 
        }
      BankAccount = SharedMem[0];
      balanceNeed=random()%50;
      printf("Poor Student needs $%d\n", balanceNeed);
       
      if(balanceNeed<=BankAccount)
      {
        BankAccount=BankAccount-balanceNeed;//then withdraw balanceNeed from BA
        printf("Poor Student: Withdraws $%d / Bank Account Balance = $%d\n", balanceNeed, BankAccount);
      }
      else
      { //cannot withdraw, doesnt have enough in BA
        printf("Poor Student: Not Enough Cash ($%d)\n", BankAccount );
      }
      SharedMem[0]=BankAccount;  //we modif the copy , now put it back into shared mem here 
      
      SharedMem[1]=0;  //Turn=0
    }
}