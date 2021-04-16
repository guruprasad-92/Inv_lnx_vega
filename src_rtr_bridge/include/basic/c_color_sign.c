#include <stdio.h>


//------------------- colors --------------
#define   COLOR           "\033"
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

#define CC_COLOR_BRED  ( printf(Bold_Red) )
#define CC_COLOR_BGREEN  ( printf(Bold_Green) )
#define CC_COLOR_RST    ( printf(Reset) )
//-----------------------------------------

//------------------- Signs -------------------
//------ reference https://en.wikipedia.org/wiki/List_of_Unicode_characters
#define SGN_DEGREE          "\u00b0"
#define SGN_SML_ALPHA       "\u03b1"    //α
#define SGN_SML_BEETA       "\u03b2"    //β
#define SGN_SML_GAMMA       "\u03b3"    //γ
#define SGN_SML_DELTA       "\u03b4"    //δ
#define SGN_SML_EPSILON     "\u03b5"    //ε
#define SGN_SML_ZETA        "\u03b6"    //ζ
#define SGN_SML_ETA         "\u03b7"    //η
#define SGN_SML_THEETA      "\u03b8"    //θ
#define SGN_SML_LOTA        "\u03b9"    //ι
#define SGN_SML_KAPPA       "\u03ba"    //κ
#define SGN_SML_LAMBDA      "\u03bb"    //λ
#define SGN_SML_MU          "\u03bc"    //μ
#define SGN_SML_NU          "\u03bd"    //ν
#define SGN_SML_XI          "\u03be"    //ξ

#define SGN_SML_PI          "\u3c0"     //π
#define SGN_SML_RHO         "\u3c1"     //ρ
#define SGN_SML_FINAL_SIGMA "\u3c2"     //ς
#define SGN_SML_SIGMA       "\u3c3"     //σ
#define SGN_SML_TAU         "\U3C4"     //τ
#define SGN_SML_UPSILON     "\U3C5"
#define SGN_SML_PHI         "\U3C6"
#define SGN_SML_CHI         


//--------------------XXXX---------------------

// int main()
// {
//     char a[9] = {0};
//     printf("----------------colors--------------\n");
//     printf(Red"RED\n");
//     printf(Bold_Red"bold red\n");
//     printf(Green"green\n");
//     printf(Bold_Green"Bold green\n");
//     printf(Yellow"yellow\n");
//     printf(Bold_Yellow"Bold yellow\n");
//     printf(Blue"Blue\n");
//     printf(Bold_Blue"Bold blue\n");
//     printf(Magenta"magnenta\n");
//     printf(Bold_Magenta"bold magnenta\n");
//     printf(Cyan"cyan\n");
//     printf(Bold_Cyan"bold cyan\n");
//     printf(Reset"----------- end ------------------\n");

//     printf("----------------Signs--------------\n");
//     printf("degree celcious : %sC\n",SGN_DEGREE);
//     printf("Greek small alpha : %s\n",SGN_SML_ALPHA);
//     CC_COLOR_BRED;
//     printf("IN red\n\r");
//     CC_COLOR_RST;
//     CC_COLOR_BGREEN;
//     printf("IN B_green\n\r");
//     CC_COLOR_RST;

    
// }