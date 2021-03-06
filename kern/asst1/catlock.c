/*
 * catlock.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use LOCKS/CV'S to solve the cat syncronization problem in 
 * this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>


/*
 * 
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2

#define ITER 4

//All variables
static int bowl;//which bowl is currently being used
static int num_cats = 0; //number of cats that are eating
static int num_mice = 0; //number of mice that are eating
static struct lock *cats_lock; //lock for cats
static struct lock *mice_lock; //lock for mice
static struct cv *mice_ready; //cv for mice
static struct cv *cats_ready; //cv for cats
static struct lock *bowl1; //lock for bowl no.1
static struct lock *bowl2; //lock for bowl no.2




/*
 * 
 * Function Definitions
 * 
 */

/* who should be "cat" or "mouse" */
static void
lock_eat(const char *who, int num, int bowl, int iteration)
{
	
        kprintf("%s: %d starts eating: bowl %d, iteration %d\n", who, num, 
                bowl, iteration);
        clocksleep(1);
        kprintf("%s: %d ends eating: bowl %d, iteration %d\n", who, num, 
                bowl, iteration);
}

/*
 * catlock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS -
 *      1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
catlock(void * unusedpointer, 
        unsigned long catnumber)
{
        //unused variable
        (void) unusedpointer;
        //4 iterations
        int iter = 0;
        while (iter < ITER){            
            //if the first bowl is available
            if (bowl1->held == 0){
                bowl = 1;
                lock_acquire(bowl1);
                //when mice are eating, wait
                while(num_mice > 0)
                    //wait on cats_ready
                    cv_wait(cats_ready, bowl1);
                //when no mice are eating
                //increment on number of cats eating
                lock_acquire(cats_lock);
                num_cats++;
                lock_release(cats_lock);
                //print message
                lock_eat("cat", catnumber, bowl, iter);
                //decrement on number of cats eating
                lock_acquire(cats_lock);
                num_cats--;
                lock_release(cats_lock);
                lock_release(bowl1);
                iter++;
                if(num_cats == 0)
                   cv_signal(mice_ready, bowl1);
            }
            //if the second bowl is available
            else if (bowl2->held == 0){
                bowl = 2;
                lock_acquire(bowl2);
                //when mice are eating, wait
                while(num_mice > 0)
                    cv_wait(cats_ready, bowl2);
                //when no mice are eating
                //increment on number of cats eating
                lock_acquire(cats_lock);
                num_cats++;
                lock_release(cats_lock);
                //print message
                lock_eat("cat", catnumber, bowl, iter);
                //decrement on number of cats eating
                lock_acquire(cats_lock);
                num_cats--;
                lock_release(cats_lock);
                lock_release(bowl2);
                iter++;
                if(num_cats == 0)
                    cv_signal(mice_ready, bowl2);  
            }
        }
}
	

/*
 * mouselock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to 
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
mouselock(void * unusedpointer,
          unsigned long mousenumber)
{
        /*
         * Avoid unused variable warnings.
         */
        (void) unusedpointer;
        //4 iterations
        int iter = 0;
	while (iter < ITER){
            //if the first bowl is available
            if (bowl1->held == 0){
                bowl = 1;
                lock_acquire(bowl1);
                //when mice are eating, wait
                while(num_cats > 0)
                    cv_wait(mice_ready, bowl1);
                //when no mice are eating
                //increment on number of cats eating
                lock_acquire(mice_lock);
                num_mice++;
                lock_release(mice_lock);
                //print message
                lock_eat("mouse", mousenumber, bowl, iter);
                //decrement on number of cats eating
                lock_acquire(mice_lock);
                num_mice--;
                lock_release(mice_lock);
                lock_release(bowl1);
                iter++;
                if (num_mice == 0)
                    cv_signal(cats_ready, bowl1);
            }
            //if the second bowl is available
            else if (bowl2->held == 0){
                bowl = 2;
                lock_acquire(bowl2);
                //when mice are eating, wait
                while(num_cats > 0)
                    cv_wait(mice_ready, bowl2);
                //when no mice are eating
                //increment on number of cats eating
                lock_acquire(mice_lock);
                num_mice++;
                lock_release(mice_lock);
                //print message
                lock_eat("mouse", mousenumber, bowl, iter);
                //decrement on number of cats eating
                lock_acquire(mice_lock);
                num_mice--;
                lock_release(mice_lock);
                lock_release(bowl2);
                iter++;
                if (num_mice == 0)
                    cv_signal(cats_ready, bowl2);
                
            }
        }
}


/*
 * catmouselock()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catlock() and mouselock() threads.  Change
 *      this code as necessary for your solution.
 */

int
catmouselock(int nargs,
             char ** args)
{
        int index, error;
   
        bowl1 = lock_create("bowl1");
        bowl2 = lock_create("bowl2");
        mice_lock = lock_create("mice");
        cats_lock = lock_create("cats");
        mice_ready = cv_create("mice");
        cats_ready = cv_create("cats");
        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;
        
        
        
        
        /*
         * Start NCATS catlock() threads.
         */

        for (index = 0; index < NCATS; index++) {
           
                error = thread_fork("catlock thread", 
                                    NULL, 
                                    index, 
                                    catlock, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
                 
                        panic("catlock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        /*
         * Start NMICE mouselock() threads.
         */

        for (index = 0; index < NMICE; index++) {
   
                error = thread_fork("mouselock thread", 
                                    NULL, 
                                    index, 
                                    mouselock, 
                                    NULL
                                    );
      
                /*
                 * panic() on error.
                 */

                if (error) {
         
                        panic("mouselock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        return 0;
}

/*
 * End of catlock.c
 */
