#include <stdio.h>
#include <time.h>
#include "E101.h"



/*__________________________________ FUNCTIONS _______________________ */

int measure_line()
{
	/* Take picture, find the maximum and minimum value of white pixels, find the threshold
    * for determining whether to assign a pixel binary value of 0 - black or 1 - white   */
	
		take_picture();
	
		
				
	 int max = 0;
         int min = 255;
         int scan_row = 120;
   	     for (int i = 0; i <320; i++)
	     {
		   int pix = get_pixel(scan_row,i,3);
           if ( pix > max) 
           {
			  max = pix;
	  	   }
		   if (pix < min)
		   {
			  min = pix;
		   }
        }
        
       int thr = (max+min)/2;
       printf(" min=%d max=%d threshold=%d\n", min, max, thr);
       
     if (max < 60) 
     {
		//printf("I aam seeing all black \n");
		return -10000;
	 }
	 
      /* Removing noise - assigns binary values to each pixel, */
      
      int whi[320];  // white pixels (array)
      int nwp = 0; // number of white pixels
      for (int i = 0; i <320; i++)
	  {
		 whi[i]= 0 ;
		 
		 int pix = get_pixel(scan_row,i,3);
		
		 if ( pix > thr)
		 {
			whi[i] = 1;
			nwp++;
		 }
		
      }
    
		
	
	/* Computes the error signal - if positive, white line is on right side
	 * of image, if negative white line is on left side, if zero in middle */	
		
	double error = 0;
		
	for (int i = 0; i < 320; i++)
	{
	   error = error +(i-160)*whi[i];	
   	}
	
	if ( nwp > 0)
	{
	   error = error/nwp;
    }
		
    return error;
	
}



/* ____________________________________ MAIN _________________________________ */


int main (){
	
	init();
	
	int quad = 0;
	
	
	/* Code for opening Gate 1 */
	
	char ip[15]  = "130.195.6.196";
	
	char please[24]  = "Please";
	
	char message[24];
	
	connect_to_server(ip, 1024);
	send_to_server (please);
	receive_from_server(message);
	send_to_server(message);
	sleep1(0.5,0);
	
	
	
	while(1)
	{
	
		if (quad == 0)
		{
				
		
			int error = measure_line();
			printf("error=%d\n",error); // prints error for debug

			if (error == -10000)
			{
				// go back
				printf("go back\n");
				set_motor(1, -40);
				set_motor(2, -105);
				sleep1(0,200000);
			}
	
	/* Controller - ( note: default speeds different for motor 1 & 2 as we had found that 
	 * motor 2 was much slower than motor 1 */ 	
	
			double vgo_m1 =40; // motor 1 (left) speed
			double vgo_m2 = 105; // motor 2 (right) speed
			double kp =0.20; // proportionality - how much motor reacts to error
			double dv = (double)error * kp; // difference in speed of left and right motors
	
			set_motor(1, vgo_m1 - dv ); 
			set_motor(2, vgo_m2 + dv); 
		

		}
	
    }	

}	
	
	


