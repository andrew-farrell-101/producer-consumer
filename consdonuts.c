#include "donuts.h"
#define NUMDOZEN 12
int semid[3], out_ptr[NUMFLAVORS];
/*  some of the consumer code                               */
int main(int argc, char* argv[]) {
    int shmid; 
    struct donut_ring* shared_ring;
    if((shmid = shmget(MEMKEY,sizeof(struct donut_ring),IPC_CREAT|0600)) == -1)
    {
		perror("shared get failed: ");
		exit(1);
	}

    /***  attach shared memory segment into the address space ***/
	if((shared_ring = (struct donut_ring *) shmat(shmid, NULL, 0)) == -1)
    {
		perror("shared attach failed: ");
		 exit(1);
	}

    /***  allocate 3 semaphore IDs with 4 counters per ID ***/
	for(int i=0; i<NUMSEMIDS; i++) 
    {
		if ((semid[i] = semget(SEMKEY+i, NUMFLAVORS, IPC_CREAT | 0600)) == -1)
        {
            perror("semaphore allocation failed: ");
            exit(1);
	    }
    }
   
    struct timeval randtime;
    gettimeofday (&randtime, (struct timezone *) 0);
    unsigned short xsub[3] = { 
        (ushort) randtime.tv_usec,
        (ushort) (randtime.tv_usec >> 16),
        (ushort) (getpid()),
    };

    for( int i = 0; i < 10; i++ ) {
        // fprintf(stderr, "Dozen: %d from process %d\n", i, getpid());
        int consumed[NUMFLAVORS][NUMDOZEN];
        int serial [NUMFLAVORS];
        // init the arrays
        for(int i = 0; i < NUMFLAVORS; i++) {
            serial[i] = 0;
            for(int j = 0; j < NUMDOZEN; j++) {
                // making sential value for easier printing later
                consumed[i][j] = 0;
            }
        }

        for(int  m = 0; m < NUMDOZEN; m++ ) {
            // CURRENT ISSUE, GETTIGN SEG FAULT AFTER 1 ITERATION
            int flavor = nrand48(xsub) & 3;
            // Is there a donut for me to consume?
            p(semid[CONSUMER], flavor);
            // Is it my turn?
            p(semid[OUTPTR], flavor);

            // fprintf(stderr, "PID %d marker 1\n", getpid());
            // which donut do I want?
            int donut = shared_ring->flavor[flavor][shared_ring->outptr[flavor]];
            // consume that donut
            consumed[flavor][serial[flavor]] = donut;
            // update shared memory outptr
            shared_ring->outptr[flavor] = (shared_ring->outptr[flavor] + 1) % NUMSLOTS;
            
            // increment consumed buffer serial counter
            serial[flavor] += 1;
            // tell the producer they can continue to product
            v(semid[PROD], flavor);
            // unlock the outptr for the chosen flavor
            v(semid[OUTPTR], flavor);
            /*  get a donut and do a micro sleep for context switch  */ 
            usleep(100);
            // fprintf(stderr, "PID %d marker 3\n", getpid());
            
            
        }
        
        printf("%-10s %d", "Process ID: ", getpid());
        struct timeval tv;
        time_t t;
        gettimeofday(&tv, NULL);
        t = tv.tv_sec;

        struct tm tm = *localtime(&t);
        printf("%-10s %02d:%02d:%02d.%.3ld", "     Time: ",tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / 1000);  
        printf("    dozen #: %d\n\n", i + 1);      
        printf("plain          jelly          coconut        honey-dip\n");
        int out = 0;
        int i = 0;
        while (!out) {
            // assume we are out of donuts to read
            out = 1;
            for (int flavor = 0; flavor < NUMFLAVORS; flavor++) {
                // if there is a donut to read, we are not out and read it
                if (consumed[flavor][i]) {
                    out = 0;
                    printf("%-15d", consumed[flavor][i]);
                }
                else {
                    printf("%15s","");
                }
            }
            i++;
            printf("\n");
        }
        printf("\n\n");
    }  /*  end getting 10 dozen, now finish            */
    fprintf(stderr, "  CONSUMER %s DONE\n", argv[1]);
    return 0;
    /*  end consumer program                                 */
}