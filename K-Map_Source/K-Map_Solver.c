#include "Self_Define_Functions.h"
#define MAX_MIN 16

int main(int argc, char const *Min_in_Char[])
{
  if (argc <= 1)
  {
    printf("No input has been received.( use this --> ./K-Map_Solver 1001 )");
    return 1;
  }

  int Min = strlen(Min_in_Char[1]); // Number of Minterms
  int Var;                          // Number of Variables
  float temp = log2f((float)Min);

  if ((int)temp == temp)
    Var = (int)temp;
  else
  {
    printf("Entered input is incomplete or Not correct\n");
    return 1;
  }

  Min = _1sC(Min_in_Char[1], Min); // !important! Assigning the number of minterms based on the number of 1s.
  if (!Min)
  {
    FILE *file = fopen("Output.txt", "w");
    fprintf(file, "%c", '0');
    fclose(file);
    return 0;
  }

  int LPP = (Var * pow(2, Var - 1)); // Largest Possible Pair (When all Minterms is set to '1' or The Worst case of this program)

  int *PP = (int *)malloc((LPP * 2) * sizeof(int)); // Allocating Memory for Possible-Pairs

  int PI_CLine, PP_ELine; // Prime Implicant Current Line, Possible Pair End Line

  char Result[500] = {'\0'}; // Result wiil be stored here in Boolean Algebraic form

  char Tick[LPP + 1]; // Ticked and UnTicked Minterms Track stored here
  Tick[LPP] = '\0';

  char PI[MAX_MIN][Var]; // Prime Implicants Container!!

  memset(Tick, '$', sizeof(char) * LPP); // Setting Whole Array with Doller Character

  char **Min_in_Bin = _2DPointer(Min, Var); // Binary Minterms Clone for Further Operations Execution

  Dec_to_Bin(Min_in_Char[1], Min_in_Bin, MAX_MIN, Var); // Converting Input Data to its Corresponding Binary Form

  int C0Min = Min; // Clone-0 Minterm-Count
  int C1Min = Min; // Clone-1 Minterm-Count

  while (1) // Infinite Loop
  {
    memset(Tick, '0', sizeof(char) * C1Min); // Assuming all the minterms are prime implicants!!

    PP_ELine = PT_Generator(Min_in_Bin, C1Min, Var, PP, Tick); // Generating Possible Pairs And Ticking PI!!

    PI_CLine = Prime_Implicant(Min_in_Bin, &PI[0][0], Var, Tick, LPP); // Storing Generated PI in the PI-Container!!  <------ problem

    if (PP_ELine == 0)
      break; // Exit the loop when thier is no Possible Pair is Available!!

    else if (PP_ELine < LPP)
      PP = (int *)realloc(PP, (PP_ELine * 2) * sizeof(int)); // Freeing the Unuseble memory space!!

    C0Min = PP_ELine;

    char **Temp = _2DPointer(C0Min, Var); // Temporary Container for Minterms Comparision!!

    //------------------------Comparision Stage Between Paired Minterms--------------------------//
    {
      int Line = 0;
      for (int i = 0; i < C0Min; i++)
      {
        for (int j = 0; j < Var; j++)
        {
          if ((Min_in_Bin[PP[(2 * i)]][j] ^ Min_in_Bin[PP[(2 * i) + 1]][j]) == 1)
          {
            Temp[Line][j] = '_';
          }
          else
          {
            Temp[Line][j] = Min_in_Bin[PP[(2 * i)]][j];
          }
        }
        Line++;
      }
    }
    //-------------------------------------Comparision End--------------------------------------//

    //------------Data Exchanging Process------------//
    {
      _2DFreeArray(Min_in_Bin, C1Min);     // Freeing the Binary Array Data Memory Space!!
      Min_in_Bin = _2DPointer(C0Min, Var); // Creating Again 2D Pointer Array with Changed Minterm Sized!!

      {
        for (int i = 0; i < C0Min; i++)
        {
          for (int j = 0; j < Var; j++)
          {
            Min_in_Bin[i][j] = Temp[i][j]; // Copying Data Temp to Min_in_Bin Array!!
          }
        }
      }

      _2DFreeArray(Temp, C0Min); // Freeing Temp Array
    }
    //--------------End of Data Exchange------------//

    memset(Tick, '$', sizeof(char) * C1Min); // Resetting Tick Data with dollar characters!!

    C1Min = C0Min; // Copying Changed Minterms-Count to Other Minterm Count!!
  }

  _2DFreeArray(Min_in_Bin, C1Min); // Freeing Min_in_Bin-Clone After Loop End!!

  free(PP); // Freeing Possible Pair Memory After Loop End!!

  FILE *file = fopen("Output.txt", "w");
  Boolean_Output(&PI[0][0], PI_CLine, Var, &Result[0]);
  fprintf(file, "%s", &Result[0]);
  fclose(file);

  return 0;
}
/*
->This program generates output in MathJax syntax and writes it to an output file.
->This program is currently incomplete and can only print the prime implicant,
  which does not represent the final or accurate result of Boolean simplification.
  The generation of essential prime implicants for the final result is a feature that will be updated in the near future.
  Therefore, I recommend using the previous version of this program,
  which can be downloaded from the development history file containing the download link.*/