#ifndef _CELLS
#define _CELLS

#include <iostream>

using namespace std;

class Cells
{
    public:
        
        /* attributes */

        int  population;
        int pollution;
        char type;
        bool state;
        bool state_pollution;

        int pos_x;
        int pos_y;


        /* constructor */
        Cells();

        /* Destructor */
        ~Cells();


};




#endif