

#ifndef _SIMCITY
#define _SIMCITY


#include "Cells.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class SimCity
{
    private:

        /* attributes */
        
        vector<vector<Cells>> matrix;
        vector<vector<Cells>> matrix_temp;
    
        int time_limit = 0;
        int refresh_rate = 0;
        int available_goods = 0;
        int available_workers = 0;
        
        int total_residential = 0;
        int total_industrial = 0;
        int total_commercial = 0;
        int total_pollution = 0;

        vector<Cells> cells_R;
        vector<Cells> cells_I;
        vector<Cells> cells_C;


    public:
    
        /* Constructor */

        SimCity(/* args */);
        
        /* Destructor */

        ~SimCity();

        /* Methods */
        
        bool initialize();
        void printMatrix();
        void start();
        void update();


        int rule3(Cells); 


        bool verifyCommercial(Cells c);
        bool verifyIndustrial(Cells c);
        bool verifyResidential(Cells c);

        bool isAdjacentPowerLine(Cells c);
        int isAdjacentPopulation(Cells c);
        int isAdjacentPopulation(Cells c, int);

        Cells sortStorageCellsByRules(vector<Cells>);
        void applyPollution();
        void searchPopulation(int, int, int, int);
        void recursiveApplyPollution(Cells, int);

        
};


    /* Auxiliary functions definitionAuxiliary functions definition */

    bool compare_rule1(Cells, Cells );
    bool compare_rule2(Cells, Cells );
    bool compare_rule4_pos_x(Cells, Cells );
    bool compare_rule4_pos_y(Cells, Cells );
    bool compare(vector<vector<Cells>>, vector<vector<Cells>> );

#endif