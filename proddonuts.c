#include "donuts.h"

int		shmid, semid[3];
void		sig_handler (int);

int main(int argc, char *argv[])
{
	
	int	i,k, nsigs;
	struct donut_ring *shared_ring;
	struct timeval randtime;

    

    /*** only need to catch some signals ***/
   struct sigaction new_act;
   sigset_t   mask_sigs;
   int sigs [] = {SIGHUP, SIGINT, SIGQUIT, SIGBUS, 
                        SIGTERM, SIGSEGV, SIGFPE};

   nsigs = sizeof (sigs) / sizeof (int);
   sigemptyset (&mask_sigs);
   
   for(i=0; i < nsigs; i++)
       sigaddset (&mask_sigs, sigs [i]); 
   
   for(i=0; i < nsigs; i++)
   {
      new_act.sa_handler 	= sig_handler;
      new_act.sa_mask 	= mask_sigs;
      new_act.sa_flags 	= 0;
      if(sigaction (sigs [i], &new_act, NULL) == -1)
      {
         perror("can't set signals: ");
         exit(1);
      }
   }

    // /*** allocate a shared memory segment  ***/
	if((shmid = shmget(MEMKEY,sizeof(struct donut_ring),IPC_CREAT|0600)) == -1)
    {
		perror("shared get failed: ");
		exit(1);
	}

    /***  attach shared memory segment into the address space ***/
	if((shared_ring = (struct donut_ring *) shmat(shmid, NULL, 0)) == -1)
    {
		perror("shared attach failed: ");
		sig_handler(-1);
	}

    /***  allocate 3 semaphore IDs with 4 counters per ID ***/
	for(i=0; i<NUMSEMIDS; i++) 
    {
		if ((semid[i] = semget(SEMKEY+i, NUMFLAVORS, IPC_CREAT | 0600)) == -1)
        {
            perror("semaphore allocation failed: ");
            sig_handler(-1);
	    }
    }

    /***  begin syscall gettimeofday comment for time seed
     #include <sys/time.h>

     int gettimeofday (struct timeval *tp, struct timezone *tzp);

     struct timeval {
		long    tv_sec;         	/*** seconds 
        	long    tv_usec;        	/*** and microseconds 
     }

    /*** end syscall gettimeofday comment ***/

	gettimeofday (&randtime, (struct timezone *)0);

	if(semsetall (semid[PROD], NUMFLAVORS, NUMSLOTS) == -1)
    {
        perror("semsetsall failed: ");
        sig_handler(-1);
	}
    
	if(semsetall (semid[CONSUMER], NUMFLAVORS, 0) == -1)
    {
        perror("semsetsall failed: ");
        sig_handler(-1);
    }

	if(semsetall (semid[OUTPTR], NUMFLAVORS, 1) == -1)
    {
        perror("semsetsall failed: ");
        sig_handler(-1);
    }

    /*** seed the nrand48() random number generator ***/
	gettimeofday (&randtime, (struct timezone *)0);

    /*** use microsecond component for uniqueness ***/
	unsigned short xsub1[3];
    xsub1[0] = (ushort) randtime.tv_usec;
    xsub1[1] = (ushort)(randtime.tv_usec >> 16);
    xsub1[2] = (ushort)(getpid());


/*  the rest of the producer code is a simple endless loop	*/
/*  that forever flips a random number and attempts to 	*/
/*  produce a new item into the selected ring buffer        */
// THIS IS WHERE ALL THE IMPORTANT CODE WILL GO~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!
/*** producer initializes serial counters and in-pointers  ***/
	int	in_ptr [NUMFLAVORS];
    int serial [NUMFLAVORS];
    /*** producer initializes serial counters and in-pointers  ***/
	for(int i = 0; i < NUMFLAVORS; i++)
    {
		in_ptr [i] = 0;
		serial [i] = 1;
	}
	while(1){
        // flip a random number
		int flavor = nrand48(xsub1) & 3;
        // Take producer ID and decrement available slots for flavor j
        p(semid[PROD], flavor);
        // update shared buffer and increment
        shared_ring->flavor[flavor][in_ptr[flavor]] = serial[flavor];
        serial[flavor] += 1;
        // update in in_ptr to account for new entry. Use mod since ring buffer
        in_ptr[flavor] = (in_ptr[flavor] + 1) % NUMSLOTS;
        // tell the consumer another donut is ready
        v(semid[CONSUMER], flavor);
    }
}
	// 	// use the random number to select one of the ring buffers 
	// 	this wioll involve p and v operations

/*  produce forever, will block when a queue is full        */
/*  termination will be from an external signal             */

	//}


void sig_handler(int sig)
{
	int	i,j,k;

	printf("In signal handler with signal # %d\n",sig);

	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("handler failed shm RMID: ");
	}
	for(i = 0; i < NUMSEMIDS; i++){
	  if(semctl (semid[i], 0, 
 			    IPC_RMID) == -1){
		perror("handler failed sem RMID: ");
	  }
	}
        exit(5);
}
