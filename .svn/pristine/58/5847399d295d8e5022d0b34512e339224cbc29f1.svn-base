/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
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


/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */

static const char *directions[] = { "N", "E", "S", "W" };

static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

/*
 *Declaration for semaphores
 */
 
 static struct semaphore *NorthWest;
 static struct semaphore *NorthEast;
 static struct semaphore *SouthWest;
 static struct semaphore *SouthEast;
 static struct semaphore *Limit3; 
 /**************************************************************
 Limit 3 semaphore is created to limit the traffic in the intersection below 3. 
 Although it will prevent the situation where four cars turning right at the 
 same time, it significantly prevents the deadlock from happening, which will 
 time out the system. 
 ***************************************************************/

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}
 
/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * cardirection and destination are set randomly.
         * 0 = North
         * 1 = East
         * 2 = South
         * 3 = West
         */
         
		switch(cardirection){
			case 0: //Going straight from North to South.
				message(0 , carnumber, cardirection, 2);
				P(Limit3);
				P(NorthWest);
				message(1 , carnumber, cardirection, 2);
				P(SouthWest);				
				message(2 , carnumber, cardirection, 2);
				V(NorthWest);
				message(4, carnumber, cardirection, 2);
				V(SouthWest);
				V(Limit3);
				break;
			case 1: //Going straight from East to West.
				message(0 , carnumber, cardirection, 3);
				P(Limit3);
				P(NorthEast);
				message(1 , carnumber, cardirection, 3);
				P(NorthWest);			
				message(2 , carnumber, cardirection, 3);
				V(NorthEast);
				message(4, carnumber, cardirection, 3);
				V(NorthWest);
				V(Limit3);
				break;
			case 2: //Going straight from South to North. 
				message(0 , carnumber, cardirection, 0);
				P(Limit3);
				P(SouthEast);
				message(1 , carnumber, cardirection, 0);
				P(NorthEast);				
				message(2 , carnumber, cardirection, 0);
				V(SouthEast);
				message(4, carnumber, cardirection, 0);
				V(NorthEast);
				V(Limit3);
				break;
			case 3: //Going straight from West to East.
				message(0 , carnumber, cardirection, 1);
				P(Limit3);
				P(SouthWest);
				message(1 , carnumber, cardirection, 1);
				P(SouthEast);			
				message(2 , carnumber, cardirection, 1);
				V(SouthWest);
				message(4, carnumber, cardirection, 1);
				V(SouthEast);
				V(Limit3);
				break;
		}
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
		switch(cardirection){
			case 0: //Turning left from North to East.
				message(0 , carnumber, cardirection, 1);
				P(Limit3); 
				P(NorthWest);
				message(1 , carnumber, cardirection, 1);
				P(SouthWest);
				message(2 , carnumber, cardirection, 1);
				V(NorthWest);
				P(SouthEast);								
				message(3 , carnumber, cardirection, 1);
				V(SouthWest);
				message(4, carnumber, cardirection, 1);
				V(SouthEast);
				V(Limit3);
				break;
			case 1: //Turning left from East to South.
				message(0 , carnumber, cardirection, 2);
				P(Limit3);
				P(NorthEast);
				message(1 , carnumber, cardirection, 2);
				P(NorthWest);
				message(2 , carnumber, cardirection, 2);
				V(NorthEast);
				P(SouthWest);				
				message(3 , carnumber, cardirection, 2);
				V(NorthWest);
				message(4, carnumber, cardirection, 2);
				V(SouthWest);
				V(Limit3);
				break;
			case 2: //Turning left from South to West. 
				message(0 , carnumber, cardirection, 3);
				P(Limit3);
				P(SouthEast);
				message(1 , carnumber, cardirection, 3);
				P(NorthEast);
				message(2 , carnumber, cardirection, 3);
				V(SouthEast);
				P(NorthWest);
				message(3 , carnumber, cardirection, 3);
				V(NorthEast);
				message(4, carnumber, cardirection, 3);
				V(NorthWest);
				V(Limit3);
				break;
			case 3: //Turning left from West to North.
				message(0 , carnumber, cardirection, 0);
				P(Limit3);
				P(SouthWest);
				message(1 , carnumber, cardirection, 0);
				P(SouthEast);
				message(2 , carnumber, cardirection, 0);
				V(SouthWest);
				P(NorthEast);				
				message(3, carnumber, cardirection, 0);
				V(SouthEast);
				message(4, carnumber, cardirection, 0);
				V(NorthEast);
				V(Limit3);
				break;
		}
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
		switch(cardirection){
			case 0: //Turning right from North to West.
				message(0 , carnumber, cardirection, 3);
				P(Limit3);
				P(NorthWest);
				message(1 , carnumber, cardirection, 3);
				message(4, carnumber, cardirection, 3);
				V(NorthWest);
				V(Limit3);
				break;
			case 1: //Turning right from East to Morth.
				message(0 , carnumber, cardirection, 0);
				P(Limit3);
				P(NorthEast);
				message(1 , carnumber, cardirection, 0);
				message(4, carnumber, cardirection, 0);
				V(NorthEast);
				V(Limit3);
				break;
			case 2: //Turning right from South to East. 
				message(0 , carnumber, cardirection, 1);
				P(Limit3);
				P(SouthEast);
				message(1 , carnumber, cardirection, 1);
				message(4, carnumber, cardirection, 1);
				V(SouthEast);
				V(Limit3);
				break;
			case 3: //Turning right from West to South.
				message(0 , carnumber, cardirection, 2);
				P(Limit3);
				P(SouthWest);
				message(1 , carnumber, cardirection, 2);
				message(4, carnumber, cardirection, 2);
				V(SouthWest);
				V(Limit3);
				break;
		}
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection, destination;


        /*
         * cardirection and destination are set randomly.
         * 0 = North
         * 1 = East
         * 2 = South
         * 3 = West
         */

        cardirection = random() % 4;
        while(1){
        	destination = random() % 4;
        	if(cardirection != destination) break; 
        }
        
        
        
        switch(cardirection){
        	//Car coming from North.
        	case 0: 
        		switch(destination){
        			//Car going East.
        			case 1:
        				turnleft(cardirection, carnumber);
        				break;
        			//Car going South.
        			case 2:
        				gostraight(cardirection, carnumber);
        				break;
        			//Car going West.
        			case 3:
        				turnright(cardirection, carnumber);
        				break;
        		}
        		break;	
        		
        	//Car coming from East.
        	case 1:
        		switch(destination){
        			//Car going North.
        			case 0:
        				turnright(cardirection, carnumber);
        				break;
        			//Car turning South.
        			case 2:
        				turnleft(cardirection, carnumber);
        				break;
        			//Car Going West.
        			case 3:
        				gostraight(cardirection, carnumber);
        				break;	
        		}  
        		break;	 
        		    	
        	//Car coming from South.
        	case 2:
        		switch(destination){
        			//Car going North.
        			case 0:
        				gostraight(cardirection, carnumber);
        				break;
        			//Car turning East.
        			case 1:
        				turnright(cardirection, carnumber);
        				break;
        			//Car turning West.
        			case 3:
        				turnleft(cardirection, carnumber);
        				break;
        		}    
        		break;	    
        			
        	//Car coming from West.
        	case 3:
        		switch(destination){
        			//Car going North.
        			case 0:
        				turnleft(cardirection, carnumber);
        				break;
        			//Car turning East.
        			case 1:
        				gostraight(cardirection, carnumber);
        				break;
        			//Car turning South.
        			case 2:
        				turnright(cardirection, carnumber);
        				break;	
        		} 
        		break;	      	     		
        }
        
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;


		//Create semaphores for four regions of the intersection.
		NorthWest = sem_create("NorthWest", 1);
		NorthEast = sem_create("NorthEast", 1);
		SouthWest = sem_create("SouthWest", 1);
		SouthEast = sem_create("SouthEast", 1);
		Limit3 = sem_create("Limit3", 3);
		
        /*
         * Start NCARS approachintersection() threads.
         */

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        return 0;
}
