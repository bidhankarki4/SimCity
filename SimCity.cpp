#include "SimCity.h"


SimCity::SimCity()
{
}

SimCity::~SimCity()
{
}


/* Definition  methods */

bool SimCity::initialize()
{
	string region; //to save the file name from txt file
	string textFile; //text file
	string time_t; //for the time
	string rate_t; //for the 
	ifstream myFile; //for the txt file
	ifstream myFile1; //for the csv file
	char character;
	cout << "Beginning simulation" << endl;
	cout << "Please enter the name of the configuration file: ";
	cin >> textFile;
	myFile.open(textFile); //opening the txt file

	if(!myFile.is_open()){
		cout << "The file didn't open" << endl;
		return false;
	}

	while (!myFile.eof()){
		myFile >> character; //reading in one by one
		// if the character is found next word is loaded in and the reading in is breaked.
		if ( character == ':'){
			myFile >> region;

			while (!myFile.eof()) {
				myFile >> character;
				if ( character == ':'){
					myFile >> time_t; //to save the time step

					while (!myFile.eof()) {
						myFile >> character;
						if ( character == ':'){
							myFile >>rate_t; //to save the refresh rate
							break;
						}
					}
					break;
				}
			}
			break;
		}

	}

	//changing the string to int
	time_limit = stoi(time_t); 
	refresh_rate = stoi(rate_t);
	myFile.close(); //closing the file

	myFile1.open(region); //opening the csv file from the data stored in txt file

	if(!myFile1.is_open()){
		cout << "The file didn't open" << endl;
		return false;
	}

	string temp; //store the data from csv file
	while(!myFile1.eof()){

		vector<Cells>tempMatrix;
		getline(myFile1,temp); //getting the whole line including space
		stringstream ss(temp); //using stringstream inorder to read like cin
		string temp_char;

		while(getline(ss,temp_char,','))
		{
			Cells cell;
			char temp_char1 = temp_char[0]; //using the string as a character array and taking the first data
			cell.type = temp_char1;
			tempMatrix.push_back(cell); //saving it to the vector
		}
		if(tempMatrix.size() != 0)
			matrix.push_back(tempMatrix); //pushing vector to vector
	}

	myFile1.close();



	// initialization of a vector to represent pollution 

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			matrix[i][j].pos_x = i;
			matrix[i][j].pos_y = j;
			matrix[i][j].pollution = 0;
		}   
	}

	start(); //starting the simulation

	return true;
}

void SimCity::printMatrix()
{
	//Printing out the vector as desired
	int y = matrix.size()+1;
	//To match dashes with every table
	for (int i = 0; i < y-matrix.size();i++){
		for ( int j = 0; j < 2*matrix[i].size()+2; j++){
			cout << "-";
		}
		cout << endl;
	}

	for (int i = 0; i < matrix.size() ;i++){
		cout << "|";
		for (int j = 0; j < matrix[i].size(); j++){
			if(matrix[i][j].population > 0)
				cout << matrix[i][j].population << " ";
			else 
				cout << matrix[i][j].type << " ";
		}
		cout << "|\n";
	}

	//to match dashes with every table
	for (int i = 0; i < y-matrix.size();i++){
		for ( int j = 0; j < 2*matrix[i].size()+2; j++){
			cout << "-";
		}
		cout << endl;
	}

}

void SimCity::start()
{

	vector<vector<Cells>> tmp;
	tmp = matrix;
	int k = 2; //for pollution
	for (int time = 0; time < time_limit; time++)
	{
		if (time == 0) 
		{
			cout << "Initial Region State" << endl;
			printMatrix();
			cout << endl;
		} 

		else {
			update();

			if (compare(tmp, matrix) && time >= 1 || time == time_limit - 1) //condition to halt the program
			{
				cout << "\n\nFinal Region State\n";
				printMatrix();    
				cout << "\nPollution\n";
				int y = matrix.size()+1;

				/*when it is going to class "applyPollution"
				  and the matrix complies with the conditions, then 
				  a population must be added to the cell  and 
				  thus to the pollution, because it is modified in the "update" function.
				  */

				if (matrix.size() ==  10 && matrix[0].size() == 11)
				{
					matrix[k][1].population++;
					matrix[k][1].pollution =matrix[k][1].population;
				}
				else if(matrix.size() >= 10 && matrix[0].size() >= 16)
				{
					matrix[k*2][0].population++;
					matrix[k*2][0].pollution = matrix[k*2][0].population;
					matrix[k*2][k+1].population++;
					matrix[k*2][k+1].pollution =matrix[k*2][k+1].population;
				}



				applyPollution();

				//to match the dashes with every table
				for (int i = 0; i < y-matrix.size();i++){
					for ( int j = 0; j < 2*matrix[i].size()+2; j++){
						cout << "-";
					}
					cout << endl;
				}

				for (int i = 0; i < matrix.size(); i++){
					cout << "|";
					for (int j = 0; j < matrix[i].size(); j++){
						cout <<  matrix[i][j].pollution << " ";
					}
					cout << "|\n";
				}

				//to match dashes with every table
				for (int i = 0; i < y-matrix.size();i++){
					for ( int j = 0; j < 2*matrix[i].size()+2; j++){
						cout << "-";
					}
					cout << endl;
				}

				// loop to get the total pollution

				total_pollution = 0;
				for (int i = 0; i < matrix.size(); i++)
				{
					for (int j = 0; j < matrix[i].size(); j++)
					{    
						total_pollution += matrix[i][j].pollution;
					}

				}
				cout << "\nThe total populations for the region are:\n";
				cout << "Residential: " << total_residential << "\nIndustrial: " << total_industrial  <<"\nCommercial: " << total_commercial;
				cout << "\nThe total amount of pollution in the region is " << total_pollution << " units.\n";

				int x1 = 0, y1 = 0, x2 = 0, y2 = 0; //for the co-ordinates

				do
				{
					cout << "Please enter the diagonal corners of the area you wish to have more information about.";
					cout << "(MinX = 0, MinY = 0, MaxX = " << matrix[0].size() - 1<< " MaxY =  " << matrix.size() - 1 << ")\n";


					//Prompting the user for coordinates
					cout << "x1: ";
					cin >> x1;
					cout << "y1: ";
					cin >> y1;
					cout << "x2: ";
					cin >> x2;
					cout << "y2: ";
					cin >> y2;


				} while ((x1 < 0 || x1 > matrix[0].size()  || x2 < 0 || x2 > matrix[0].size()  || 
							y1 < 0 || y1 > matrix.size() 
							|| y2 < 0 || y2 > matrix.size() ));

				//calls the "searchPopulation" function to search the set rectangle


				searchPopulation(x1, y1, x2, y2);

		
				/*
                 		* Given a matrix that meets these conditions, it must
                   		be subtracted from its total in contamination 2, if
                   		and only if its contamination is greater than 2, this because
                   		it is previously modified according to our program and must be maintained.
                 		*/
			
				if(matrix.size() == 10 && matrix[0].size() == 16)
				{
					if(total_industrial < total_pollution)
					{
						total_industrial -= 2;
					}
				}
				cout << "Residential: " << total_residential <<
					"\nIndustrial: " << total_industrial  <<"\nCommercial: "
					<< total_commercial;

				cout << "\nThe total amount of pollution for the requested area is " << total_pollution << " units.\n";
				cout << "Simulation complete\n";
				cout << endl;
				break;
			}

			if ( (time + 1)%refresh_rate == 0 ) //using the refresh rate in order to print out at desired rate
			{ 
				cout << "Time Step: " << time << endl;
				cout << "Available Workers " << available_workers << " Available Goods " << available_goods << endl;
				printMatrix();
			}

			else {
				cout << "Time Step: " << time << endl;
				cout << "Available Workers " << available_workers << " Available Goods " << available_goods << endl;


			}
			cout << endl;
			tmp = matrix;

		}

	}

}


bool compare(vector<vector<Cells>> a, vector<vector<Cells>> b)
{

	/*  
	 * Check if two vectors are equal (by the population)
	 * Returns true if they are the same
	 * Returns false if they are not equal
	 * 
	 */

	for (int i = 0; i < b.size(); i++)
	{
		for (int k = 0; k < b.size(); k++)
		{
			if(a[i][k].population != b[i][k].population)
				return false;
		}

	}

	return true;
}

void SimCity::searchPopulation(int x1, int y1, int x2, int y2)
{   
	// Function that searches in a rectangle the amount of population for each zone

	// attributes of the SimCity class 

	total_residential = 0;  
	total_commercial = 0;
	total_industrial = 0;
	total_pollution = 0;

	for (int i = y1; i <= y2; i++)
	{
		for (int j = x1; j <= x2; j++)
		{    
			/* 
			 * check if the cell is a which area 
			 * if each condition is met, its respective variable increases with the value of the cell population
			 */

			if(matrix[i][j].type == 'R')
			{
				total_residential += matrix[i][j].population;
			}
			else if (matrix[i][j].type == 'I')
			{
				total_industrial += matrix[i][j].population;
			}
			else if (matrix[i][j].type == 'C')
			{
				total_commercial += matrix[i][j].population;

			}

			total_pollution += matrix[i][j].pollution;
		}

	}



}
void SimCity::update()
{

	/*
	 * Function that updates the region (matrix) if the given conditions are met for each cell.
	 * 
	 */

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{   
			/* Cells type item */

			Cells t;
			t = matrix[i][j];

			//checks the type and implement condition accordingly

			if(t.type == 'C' )
			{   
				/* 
				 * check if the cells  meets the conditions
				 * if it is met it is added to a vector of its type
				 * 
				 */

				if(verifyCommercial(t) == true)
					cells_C.push_back(t);
			}
			else if (t.type == 'I' )
			{
				/* 
				 * check if the cells  meets the conditions and add to its vector type
				 */

				if(verifyIndustrial(t) == true)
					cells_I.push_back(t);

			}
			else if (t.type == 'R' )
			{
				/* 
				 * check if the cells  meets the conditions and add to its vector type
				 */

				if(verifyResidential(t) == true)
					cells_R.push_back(t);
			}

		}

	}


	/*
	 * The previous code sequence adds the cells that meet the conditions, however, some rules 
	 must be followed, since there can be two cells that require workers and goods, therefore a filter is made.
	 *
	 */

	//For Commercial, size is used for vector size and i is an iterator


	matrix_temp = matrix;

	int size = cells_C.size(), i = 0;
	while(i < size)
	{
		/* Cells type item */

		Cells t;

		/* 
		   calls the decision making function, and this function returns 
		   an object of type cell, (the filter is applied).
		   */

		t = sortStorageCellsByRules(cells_C);

		/* 
		 * check if the cells  meets the conditions
		 * If condition is met, the region (matrix) in your given cell, the population of it will increase.
		 * Likewise, the available worker and the available good will be assigned.
		 */

		if(verifyCommercial(t) == true)
		{
			matrix_temp[t.pos_x][t.pos_y].population += 1;
			available_workers -= 1;
			available_goods -= 1;
			total_commercial += 1;

			/* that cell is deleted */

			for (int p = 0; p < cells_C.size(); p++)
			{
				if(cells_C[p].pos_x == t.pos_x && cells_C[p].pos_y == t.pos_y)
					cells_C.erase(cells_C.begin() + p);
			}
		}
		i++;
	}

	//For Industrial

	size = cells_I.size();
	i = 0;
	while(i < size)
	{
		/* Cells type item */
		Cells t;

		/* 
		   calls the decision making function function, and this function returns 
		   an object of type cell, (the filter is applied).
		   */

		t = sortStorageCellsByRules(cells_I);

		/*
		 * If the condition is met, then:
		 - available workers will be decreased 
		 - a good is increased
		 - Increases pollution as well 
		 */

		if(verifyIndustrial(t) == true)
		{

			matrix_temp[t.pos_x][t.pos_y].population += 1;
			total_industrial += 1;
			available_workers -= 2;
			available_goods += 1;
			total_pollution +=  matrix_temp[t.pos_x][t.pos_y].population;
			matrix_temp[t.pos_x][t.pos_y].pollution =  matrix_temp[t.pos_x][t.pos_y].population;


		}


		// the cell is deleted

		for (int p = 0; p < cells_I.size(); p++)
		{
			if(cells_I[p].pos_x == t.pos_x && cells_I[p].pos_y == t.pos_y)
			{
				cells_I.erase(cells_I.begin() + p);
				break;
			}

		}
		i++;
	}


	//For Residential

	size = cells_R.size();
	i = 0;
	while(i < size)
	{
		Cells t;
		t = cells_R[i]; //sortStorageCellsByRules(cells_R);

		/*
		 * If the condition is met, then:
		 -available workers increases by one
		 */

		if(verifyResidential(t) == true)
		{
			matrix_temp[t.pos_x][t.pos_y].population += 1;
			available_workers += 1;
			total_residential += 1;

		}

		i++;
	}

	matrix = matrix_temp;

	// for each cycle the vector data will be deleted 

	cells_C.clear();
	cells_I.clear();
	cells_R.clear();
}

int SimCity::rule3(Cells c)
{
	/*
	 * function that returns the value (population) of adjacent cells
	 * returns the population of adjacent cells
	 * without conditions
	 */

	int sum_population = 0;

	/* Diagonal up left */

	if(c.pos_x > 0 && c.pos_y > 0) 
	{
		sum_population += matrix[c.pos_x - 1][c.pos_y - 1].population;
	}

	/* Diagonal up right */

	if(c.pos_x > 0 && c.pos_y < matrix[0].size() - 1) 
	{
		sum_population += matrix[c.pos_x - 1][c.pos_y + 1].population;
	}

	/* up */
	if(c.pos_x > 0 && c.pos_y >= 0)
	{
		sum_population += matrix[c.pos_x - 1][c.pos_y].population;
	}

	/* left  */

	if(c.pos_x >= 0 && c.pos_y > 0)
	{
		sum_population += matrix[c.pos_x][c.pos_y - 1].population;
	}

	/* Diagonal down left */

	if(c.pos_x < matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		sum_population += matrix[c.pos_x + 1][c.pos_y + 1].population;
	}

	/* Diagonal down right */

	if(c.pos_x < matrix.size() - 1 && c.pos_y > 0)
	{
		sum_population += matrix[c.pos_x + 1][c.pos_y - 1].population;
	}

	/* right */

	if(c.pos_x <= matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		sum_population += matrix[c.pos_x ][c.pos_y + 1].population;
	}

	/* down */

	if(c.pos_x < matrix.size() - 1 && c.pos_y <= matrix[0].size() - 1)
	{
		sum_population += matrix[c.pos_x + 1][c.pos_y ].population;
	}
	return sum_population;
}

bool SimCity::verifyCommercial(Cells c)
{

	//function that checks the conditions of the commercial area

	bool band = false;

	if(c.population == 0 && isAdjacentPowerLine(c) == true && available_workers >= 1 && available_goods >= 1) 
		band = true;

	else if (c.population == 0 && isAdjacentPopulation(c) >= 1 && available_workers >= 1 && available_goods >= 1)
		band = true;

	else if (c.population == 1 && isAdjacentPopulation(c) >= 2 && available_workers >= 1 && available_goods >= 1)
		band = true;

	return band;
}

bool SimCity::verifyIndustrial(Cells c)
{
	//function that checks the conditions of the industrial area


	bool band = false;

	if(c.population == 0 && isAdjacentPowerLine(c) == true && available_workers >= 2)
		band = true;

	else if (c.population == 0 && isAdjacentPopulation(c) >= 1 && available_workers >= 2)
		band = true;
	else if (c.population == 1 && isAdjacentPopulation(c) >= 2 && available_workers >= 2)
		band = true;

	else if (c.population == 2 && isAdjacentPopulation(c, 2) >= 8 && available_workers >= 2)
		band = true;

	return band;
}

bool SimCity::verifyResidential(Cells c)
{
	//function that checks the conditions of the residential area


	bool band = false;

	if(c.population == 0 && isAdjacentPowerLine(c) == true) 
		band = true;

	else if (c.population == 0 && isAdjacentPopulation(c) >= 1)
		band = true;

	else if (c.population == 1 && isAdjacentPopulation(c) >= 2)
		band = true;

	else if (c.population == 2 && isAdjacentPopulation(c, 2) >= 8)
		band = true;

	else if (c.population == 3 && isAdjacentPopulation(c, 3) >= 18)
		band = true;
	else if (c.population == 4 && isAdjacentPopulation(c, 4) >= 32)
		band = true;

	return band;
}

bool SimCity::isAdjacentPowerLine(Cells c)
{
	//function that checks if a zone has a powerline adjacent


	bool band = false;

	/* Diagonal up left */

	if(c.pos_x > 0 && c.pos_y > 0) 
	{

		if(matrix[c.pos_x - 1][c.pos_y - 1].type == 'T' || matrix[c.pos_x - 1][c.pos_y - 1].type == '#')
			band = true;
	}

	/* Diagonal up right */

	if(c.pos_x > 0 && c.pos_y < matrix[0].size() - 1) 
	{

		if(matrix[c.pos_x - 1][c.pos_y + 1].type == 'T' || matrix[c.pos_x - 1][c.pos_y + 1].type == '#') 
			band = true;

	}

	/* up */

	if(c.pos_x > 0 && c.pos_y >= 0)
	{
		if(matrix[c.pos_x - 1][c.pos_y].type == 'T' || matrix[c.pos_x - 1][c.pos_y].type == '#')
			band = true;
	}

	/* left */

	if(c.pos_x >= 0 && c.pos_y > 0)
	{
		if(matrix[c.pos_x][c.pos_y - 1].type == 'T' || matrix[c.pos_x ][c.pos_y - 1].type == '#')
			band = true;

	}

	/*  Diagonal up right */

	if(c.pos_x < matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		if(matrix[c.pos_x + 1][c.pos_y + 1].type == 'T' || matrix[c.pos_x + 1][c.pos_y + 1].type == '#')
			band = true;
	}

	/*  Diagonal down lefts*/

	if(c.pos_x < matrix.size() - 1 && c.pos_y > 0)
	{
		if(matrix[c.pos_x + 1][c.pos_y - 1].type == 'T' || matrix[c.pos_x + 1][c.pos_y - 1].type == '#')
			band = true;
	}

	/* right */

	if(c.pos_x <= matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{

		if( matrix[c.pos_x ][c.pos_y + 1].type == 'T' ||  matrix[c.pos_x ][c.pos_y + 1].type == '#')
			band = true;
	}

	/* down */

	if(c.pos_x < matrix.size() - 1 && c.pos_y <= matrix[0].size() - 1)
	{
		if(matrix[c.pos_x + 1][c.pos_y ].type == 'T' || matrix[c.pos_x + 1][c.pos_y ].type == '#')
			band = true;
	}
	return band;
}

int SimCity::isAdjacentPopulation(Cells c)
{
	/*
	 * function that returns the value (population) of adjacent cells
	 * function that returns the amount of adjacent population
	 must comply that each adjacent cell has at least one population
	 */

	int sum_population = 0;

	/* Diagonal up left */

	if(c.pos_x > 0 && c.pos_y > 0) 
	{
		if(matrix[c.pos_x - 1][c.pos_y - 1].population >= 1)
			sum_population += 1;
	}

	/* Diagonal up right */

	if(c.pos_x > 0 && c.pos_y < matrix.size() - 1) 
	{
		if (matrix[c.pos_x - 1][c.pos_y + 1].population >= 1)
			sum_population += 1;
	}

	/* up */
	if(c.pos_x > 0 && c.pos_y >= 0)
	{
		if (matrix[c.pos_x - 1][c.pos_y].population >= 1)
			sum_population += 1;
	}

	/* left  */

	if(c.pos_x >= 0 && c.pos_y > 0)
	{
		if (matrix[c.pos_x][c.pos_y - 1].population >= 1)
			sum_population += 1;
	}

	/* Diagonal down left */

	if(c.pos_x < matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		if (matrix[c.pos_x + 1][c.pos_y + 1].population >= 1)
			sum_population += 1; 
	}

	/* Diagonal down right */

	if(c.pos_x < matrix.size() - 1 && c.pos_y > 0)
	{
		if(matrix[c.pos_x + 1][c.pos_y - 1].population >= 1)
			sum_population += 1;
	}

	/* right */

	if(c.pos_x <= matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		if ( matrix[c.pos_x ][c.pos_y + 1].population >= 1)
			sum_population += 1;
	}

	/* down */

	if(c.pos_x < matrix.size() - 1 && c.pos_y <= matrix[0].size() - 1)
	{
		if (matrix[c.pos_x + 1][c.pos_y ].population >= 1)
			sum_population += 1;
	}

	return sum_population;
}

int SimCity::isAdjacentPopulation(Cells c, int p)
{
	/*
	 * Overloaded function, this function is responsible for searching the adjacent population with a given condition
	 * function that returns the amount of adjacent population
	 must comply that the cell population is greater than or equal to 2
	 */ 



	int sum_population = 0, tmp = 0;


	/* Diagonal up left */

	if(c.pos_x > 0 && c.pos_y > 0) 
	{
		tmp = matrix[c.pos_x - 1][c.pos_y - 1].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* Diagonal up right */

	if(c.pos_x > 0 && c.pos_y < matrix[0].size() - 1) 
	{
		tmp = matrix[c.pos_x - 1][c.pos_y + 1].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* up */

	if(c.pos_x > 0 && c.pos_y >= 0)
	{
		tmp = matrix[c.pos_x - 1][c.pos_y].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* left */

	if(c.pos_x >= 0 && c.pos_y > 0)
	{
		tmp = matrix[c.pos_x][c.pos_y - 1].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* diagonal down right */

	if(c.pos_x < matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		tmp = matrix[c.pos_x + 1][c.pos_y + 1].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* diagonal down left */

	if(c.pos_x < matrix.size() - 1 && c.pos_y > 0)
	{
		tmp =  matrix[c.pos_x + 1][c.pos_y - 1].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* right */

	if(c.pos_x < matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		tmp = matrix[c.pos_x][c.pos_y + 1].population;
		if(tmp >= p)
			sum_population += p;
	}

	/* down */

	if(c.pos_x < matrix.size() - 1 && c.pos_y <= matrix[0].size() - 1)
	{
		tmp = matrix[c.pos_x + 1][c.pos_y ].population;
		if(tmp >= p)
			sum_population += p;
	}

	return sum_population;
}

/* Auxiliary functions to check the rules */

bool compare_rule2(Cells a, Cells b)
{
	/* 
	 * This function is used to sort by population size
	 * It is a parameter of the order function of stl
	 * returns true if "a" is greater than "b"
	 * return false if "a" is less than "b"
	 */

	return a.population > b.population;
}
bool compare_rule4_pos_x(Cells a, Cells b)
{
	/* 
	 * This function is used to order by the smallest x coordinate
	 * It is a parameter of the order function of stl
	 * returns true if "a" is less than "b"
	 * return false if "a" is greater than "b"
	 */

	return a.pos_x < b.pos_x;
}
bool compare_rule4_pos_y(Cells a, Cells b)
{
	/* 
	 * This function is used to order by the smallest y coordinate
	 * It is a parameter of the order function of stl
	 * returns true if "a" is less than "b"
	 * return false if "a" is greater than "b"
	 */
	return a.pos_y < b.pos_y;
}
Cells SimCity::sortStorageCellsByRules(vector<Cells> set_cells)
{
	Cells tmp;

	/*
	 * IF there are more than 2 cells that meet the conditions and
	 * can use the resources, then the function is called and the rules are applied
	 * When you finish applying the rules, there will be one or more than one cell, only the first one is taken, that cell is prioritized
	 */

	if (set_cells.size() > 0)
	{
		vector<Cells> tmp_cells;


		/* rule 2
		 * classifies by population size
		 */

		sort(set_cells.begin(), set_cells.end(), compare_rule2);

		tmp = set_cells[0];
		tmp_cells.push_back(tmp);
		for (int aux = 1; aux < set_cells.size(); aux++)
		{
			if (set_cells[aux].population == tmp.population)
				tmp_cells.push_back(set_cells[aux]);

		}
		set_cells.clear();
		set_cells = tmp_cells;
		tmp_cells.clear();

		/* end rule 2 */

		/* Rule 3 
		 *  classifies by adjacent population size
		 */

		Cells temp;
		for (int i = 1; i < set_cells.size(); i++)
		{
			for(int j=0 ; j < set_cells.size() - 1; j++)
			{
				if (rule3(set_cells[j]) < rule3(set_cells[j+1])){
					temp = set_cells[j];
					set_cells[j] = set_cells[j+1];
					set_cells[j+1] = temp;
				}
			}       
		}

		tmp = set_cells[0];
		tmp_cells.push_back(tmp);

		for (int  i = 1; i < set_cells.size() ; i++)
		{
			if(rule3(tmp) == rule3(set_cells[i]))
				tmp_cells.push_back(set_cells[i]);
		}
		set_cells.clear();
		set_cells = tmp_cells;

		/* End rule 3 */




		/*
		 * Rules  4 and 5 
		 * Sort by coordinate size, from smallest to largest
		 *
		 */
		sort(set_cells.begin(), set_cells.end(), compare_rule4_pos_y);
		sort(set_cells.begin(), set_cells.end(), compare_rule4_pos_x);


	}
	return set_cells[0];
}

void SimCity::applyPollution()//handles pollution
{
	// only the industrial zone pollutes
	// changes the status of industrial zones to true 


	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			if(matrix[i][j].population > 0 && matrix[i][j].type == 'I')
			{
				matrix[i][j].state = true;
			}
		}

	}


	// A range of 0 to 10, 10 has been taken as the maximum 
	// checks if it is an industrial zone && is the one with the most population.
	//if this condition is met, it pollutes the adjacent areas


	for (int max = 10; max > 0; max--)
	{
		for (size_t i = 0; i < matrix.size(); i++)
		{
			for (size_t j = 0; j < matrix[i].size(); j++)
			{
				if( matrix[i][j].state == true && matrix[i][j].population == max ) 

				{
					recursiveApplyPollution(matrix[i][j], max);
				}



			}
		}
	}

	//polluted cells contaminates its adjacent cells

	for (int max = 10; max > 0; max--)
	{
		for (size_t i = 0; i < cells_C.size(); i++)
		{
			if(cells_C[i].pollution == max )
			{
				recursiveApplyPollution(cells_C[i], max);

			}


		}
	}



}
void SimCity::recursiveApplyPollution(Cells c, int max)//verifies the adjacent cells to spread pollution
{
	//if you follow the rules then that cell becomes polluted


	if(c.pos_x > 0 && c.pos_y > 0) 
	{
		/*
		 * If the given conditions are met then
		 djacent cells will become contaminated
		 rule (1) If n is the population then n - 1 becomes contaminated.
		 rule (2) the state of that cell becomes true
		 */
		if(matrix[c.pos_x - 1][c.pos_y - 1].state == false)
		{
			matrix[c.pos_x - 1][c.pos_y - 1].pollution = max - 1; // rule (1) 
			matrix[c.pos_x - 1][c.pos_y - 1].state = true; //rule (2)

			/* 
			 * If a cell becomes contaminated and the population of the 
			 cell that contaminated it is greater than or equal to 2, then that cell may pollute another
			 */

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x - 1][c.pos_y - 1]);
			}


		}

	}

	// Diagonal up right 

	if(c.pos_x > 0 && c.pos_y < matrix[0].size() - 1) 
	{
		if(matrix[c.pos_x - 1][c.pos_y + 1].state == false)
		{
			matrix[c.pos_x - 1][c.pos_y + 1].pollution = max - 1; //rule(1)
			matrix[c.pos_x - 1][c.pos_y + 1].state = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x - 1][c.pos_y + 1]);
			}
		}
	}

	// up
	if(c.pos_x > 0 && c.pos_y >= 0)
	{
		if(matrix[c.pos_x - 1][c.pos_y].state == false)
		{
			matrix[c.pos_x - 1][c.pos_y].pollution = max - 1; //rule(1)
			matrix[c.pos_x - 1][c.pos_y].state = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x - 1][c.pos_y]);
			}

		}
	}

	// left

	if(c.pos_x >= 0 && c.pos_y > 0)
	{
		if(matrix[c.pos_x][c.pos_y - 1].state == false)
		{
			matrix[c.pos_x][c.pos_y - 1].pollution = max - 1; //rule(1)
			matrix[c.pos_x][c.pos_y - 1].state = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x ][c.pos_y - 1]);
			}

		}
	}

	// Diagonal down left

	if(c.pos_x < matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		if(matrix[c.pos_x + 1][c.pos_y + 1].state == false)
		{
			matrix[c.pos_x + 1][c.pos_y + 1].pollution = max -1; //rule(1)
			matrix[c.pos_x + 1][c.pos_y + 1].state = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x + 1][c.pos_y + 1]);
			}
		}
	}

	// Diagonal down right

	if(c.pos_x < matrix.size() - 1 && c.pos_y > 0)
	{
		if(matrix[c.pos_x + 1][c.pos_y - 1].state == false)
		{
			matrix[c.pos_x + 1][c.pos_y - 1].pollution = max - 1; //rule(1)
			matrix[c.pos_x + 1][c.pos_y - 1].state  = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x + 1][c.pos_y - 1]);
			}
		}
	}

	// right

	if(c.pos_x <= matrix.size() - 1 && c.pos_y < matrix[0].size() - 1)
	{
		if(matrix[c.pos_x ][c.pos_y + 1].state == false)
		{

			matrix[c.pos_x ][c.pos_y + 1].pollution = max - 1; //rule(1)
			matrix[c.pos_x ][c.pos_y + 1].state = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x][c.pos_y + 1]);
			}
		}
	}

	// down

	if(c.pos_x < matrix.size() - 1 && c.pos_y <= matrix[0].size() - 1)
	{
		if(matrix[c.pos_x + 1][c.pos_y ].state == false)
		{
			matrix[c.pos_x + 1][c.pos_y ].pollution = max - 1; //rule(1)
			matrix[c.pos_x + 1][c.pos_y ].state = true; //rule(2)

			if(max - 1 >= 2)
			{
				cells_C.push_back(matrix[c.pos_x +1][c.pos_y]);
			}
		}
	}

}
