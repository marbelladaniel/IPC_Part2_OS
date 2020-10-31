#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

//Collaboration with Naja Crump

void  Dear_Old_Dad(int []);
void  Poor_Student(int []);
int Turn = 0;  

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
      

     
     srandom(getpid()); //seeding random function
     

     if (argc != 3) {
         printf("Use: %s 0 0\n", argv[0]); 
        
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
          Poor_Student(ShmPTR); //call function below 
          exit(0);
     }
    else{
      //else call parent proc here 
      Dear_Old_Dad(ShmPTR); }
  
     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  Dear_Old_Dad(int  account[])
{
    int i = 0;
    int BankAccount = account[0];
    int balance; 
    
   
    for(i=0; i<25;i++) // loop 25 times 
    {
      sleep(random() %5); //sleeps for 5-10 seconds 
      
      //child to withdraw 
      
        while(account[1]!=0)
        { 
          //do nothing here (Turn != 0)
        }
      
      
      BankAccount = account[0];
      
       if(BankAccount<=100)
       {
        balance = random() % 100; //deposit random amount
        
          if(balance % 2 == 0)
          { 
            BankAccount = BankAccount + balance; 
            printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, BankAccount);
          }
          else
          { 
            printf("Dear old Dad: Doesn't have any money to give\n");
          }
       }
      else
      {
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", BankAccount);
      }
      
      account[0]=BankAccount;  
      account[1]=1;  
    }

}




void  Poor_Student(int account[])
{
    int i = 0;
    int BankAccount = account[0];
    int balance; 
    
//Child Starts Here 
    for(i=0; i<25;i++)
    {
      sleep(random() %5); 
      
      //withdraw
      
        while(account[1]!=1)
        {
          //do nothing here
        }
      BankAccount = account[0];
      balance = random()%50;
      printf("Poor Student needs $%d\n", balance);
       
      if(balance <=BankAccount)
      {
        BankAccount = BankAccount - balance;//then withdraw balance from BA
        printf("Poor Student: Withdraws $%d / Bank Account Balance = $%d\n", balance, BankAccount);
      }
      else
      { //cannot withdraw, doesnt have enough in BA
        printf("Poor Student: Not Enough Cash ($%d)\n", BankAccount );
      }
      account[0]=BankAccount;  //we modif the copy , now put it back into account here 
      
      account[1]=0;  //Turn=0
    }
}