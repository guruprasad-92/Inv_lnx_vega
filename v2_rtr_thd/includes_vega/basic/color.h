#ifndef COLOR__H
#define COLOR__H

//------------------- colors --------------
#define   COLOR "\033"
#define   Red             COLOR"[0;31m"	
#define   Bold_Red        COLOR"[1;31m"	    
#define   Green           COLOR"[0;32m"	
#define   Bold_Green      COLOR"[1;32m"	  
#define   Yellow          COLOR"[0;33m"	
#define   Bold_Yellow     COLOR"[01;33m"	 
#define   Blue            COLOR"[0;34m"	
#define   Bold_Blue       COLOR"[1;34m"	   
#define   Magenta         COLOR"[0;35m"	
#define   Bold_Magenta    COLOR"[1;35m"	
#define   Cyan            COLOR"[0;36m"	
#define   Bold_Cyan       COLOR"[1;36m"	   
#define   Reset           COLOR"[0m"	    

#define CC_COLOR_RED   ( printf(Red) )
#define CC_COLOR_RST    ( printf(Reset))
//-----------------------------------------

#endif
