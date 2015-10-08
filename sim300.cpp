/********************************************************************************************************************************************************
 Title: sim300
 Author: Jose Antonio Uribe
 Created: November 21, 2014
 Purpose: The epic battle of Thermopylae is recounted, and uses a Binary Heap as our BATTLEFIELD. Each time a soldier is up, he picks
a target, and potentially removes it from the heap. The battle ends when the winner's team is vanquished and obliterated.

 Build with: g++ -std=c++11 -o programname  sim300 sim300.cpp
Execution commands: ./sim300 [simulations] [#Spartans] [#Persians]
********************************************************************************************************************************************************/


#include <iostream>
#include "soldierclass.h"
#include "BinaryHeap.h"
#include "QuadraticProbing.h"
#include <time.h>
#include <fstream>
#include <cmath>
using namespace std;

soldier generate_Soldier();

void generate_Soldiers(BinaryHeap<soldier>& heap, int number_of_Spartans, int number_of_Persians, vector<int>& spartans, vector<int>& persians);

void soldier_Info(soldier thisSoldier);

void kill_a_Soldier(vector<int>& soldier_Array, int& remaining_Soldiers, int dead_index);

void processTurn(BinaryHeap<soldier>& heap, vector<int>& soldiers, int& remaining_Soldiers);//Return a soldier to be attacked. Based off of who our min is.

void invigorate(BinaryHeap<soldier>& heap, vector<int>& soldiers, int remaining_Soldiers);//Will increase the soldier times by some random amount.

void printOutIDs(vector<int> soldiers, int numberOfSoldiers);//Prints our all soldiers in the array

double arithMean(double, int); //Function arithmean calculates the mean of a sum and the number of values

double standard_deviation(vector<double> values, int N, double mean);//Calculates the std. dev.

int main(int argc, char *argv[] )
{	

	srand (time(NULL));

	clock_t start;

	double seconds_since_start;

        BinaryHeap<soldier> heap;
	
	soldier nextUp;

	vector<int> alive_Spartans, alive_Persians;

	

	int total_Spartans, total_Persians, increase_Amount, time_Elapsed = 0, spartan_Count = 0, persian_Count = 0;
	int numberOfSpartanVictories = 0, numberOfPersianVictories = 0;

	double sum_Spartans = 0, sum_Persians = 0, sum_Time = 0, spartan_Squared= 0, persian_Squared = 0, time_Squared = 0; 
	double spartan_Final_Average = 0, persian_Final_Average = 0, time_Final_Average = 0, omega_Spartan = 0, omega_Time = 0;
	long double omega_Persian = 0;

	vector<double> soldier_count_Spartans, soldier_count_Persians, time_Count;

	total_Spartans = atoi(argv[2]);
	total_Persians = atoi(argv[3]);
	
	spartan_Count = total_Spartans;
	persian_Count = total_Persians;

	int numberOfTimes;
	numberOfTimes = atoi(argv[1]);

	cout << "You have set the simulations to run for: " << numberOfTimes << " times." << endl;
	cout << "You have selected the number of spartans to be: " << spartan_Count << endl;
	cout << "You have selected the number of persians to be: " << persian_Count << endl;

	if(total_Spartans == 0 || total_Persians == 0){
		cout << "what are you doing" << endl;
		exit(1);
	}

	string outputFile;
	cout << "Enter the name of the file to output results to: " << endl;
	cin >> outputFile;

	ofstream file(outputFile);
	
	
	//cout << "How many times would you like to simulate the battle?" << endl;
	

	//char begin;

	cout << "You have set the simulations to run for: " << numberOfTimes << " times." << endl;
	cout << "You have selected the number of spartans to be: " << spartan_Count << endl;
	cout << "You have selected the number of persians to be: " << persian_Count << endl;
	//cout << "Enter any character to proceed..." << endl;
	//cin >> numberOfTimes;
	
	for(int i = 0; i < numberOfTimes; i++)
	{
		start = clock();
		generate_Soldiers(heap, total_Spartans, total_Persians, alive_Spartans, alive_Persians);

		//invigorate(heap, alive_Spartans, total_Spartans);


		//cout << "The ID's of all our Spartans: " << endl;
		//printOutIDs(alive_Spartans, total_Spartans);
		//cout << "The ID's of all our Persians: " << endl;
		//printOutIDs(alive_Persians, total_Persians);
	
		cout << "Our heap reports these soldiers: " << endl;
		//heap.printAllSoldiers();
		cout << "------------------------Let the battle begin!-------------------------" << endl;
		while( spartan_Count > 0 && persian_Count > 0)
		{
			//cout << "Spartans left: " << spartan_Count << endl;
			//cout << "Persians left: " << persian_Count << endl;

			nextUp = heap.findMin();//Peek at the first soldier. This is the soldier whose turn is next

			time_Elapsed = nextUp.return_actionTime();
			//cout << "Next up: " << "soldier number: " << nextUp.return_iD() << endl;
			//cout << "This soldier's turn is at: " << nextUp.return_actionTime() << endl;
			//if(nextUp.return_Faction() == true) //cout << "A Spartan! " << endl;
			//else //cout << "A Persian!" << endl;
			if(nextUp.return_Faction()  == true) processTurn(heap, alive_Persians, persian_Count);
			else processTurn(heap, alive_Spartans, spartan_Count);

			//The soldier's key is edited; his next turn is determined by adding a random value to his turn time atm.
			if(nextUp.return_Faction() == true) heap.increase_by_iD(nextUp.return_iD(), rand() % 7 + 1);
			else if(nextUp.return_Faction() == false) heap.increase_by_iD(nextUp.return_iD(), rand() % 51 + 10);

			//cout << "After the event, our heap looks like: " << endl;
			//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
			//heap.printAllSoldiers();
			//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
 
		}
	
		

		//cout << "our heap reports these soldiers: " << endl;
		//heap.printAllSoldiers();


		cout << "Spartans remaining: " << spartan_Count << endl;

		sum_Spartans = sum_Spartans + spartan_Count;
		soldier_count_Spartans.push_back(spartan_Count);

		cout << "Persians remaining: " << persian_Count << endl;

		sum_Persians = sum_Persians + persian_Count;
		soldier_count_Persians.push_back(persian_Count);

		cout << "The ID's of all our Spartans: " << endl;

		file << "Output for simulation: " << i << endl;
		file << "-----------------------" << endl;
		file << "Remaining soldiers: " << endl;
		file << "Spartans: " << spartan_Count << " Persians: " << persian_Count << endl;

		//printOutIDs(alive_Spartans, total_Spartans);
		cout << "The ID's of all our Persians: " << endl;
		//printOutIDs(alive_Persians, total_Persians);

		cout << "*****************************************************" << endl;
		cout << "******************THE WINNER IS..********************" << endl;
		if(spartan_Count == 0){
			 cout << "The Persians are victorious!" << endl;
			 numberOfPersianVictories++;

			 file << "The Persians have won simulation: " << i << endl;

		}
		else{
			 cout << "The Spartans are victorious!" << endl;
			 numberOfSpartanVictories++;
			
			 file << "The Spartans have won simulation: " << i << endl;
		}

		heap.makeEmpty();	
		spartan_Count = total_Spartans;
		persian_Count = total_Persians;
		alive_Spartans.clear();
		alive_Persians.clear();


		seconds_since_start = (double)(clock() - start)/CLOCKS_PER_SEC;
		cout << "This battle took: " << seconds_since_start << " seconds."  << endl;
		file << "Battle " << i << " took a total of: " << seconds_since_start << " seconds." << endl;
		time_Count.push_back(seconds_since_start);
		sum_Time += seconds_since_start;
		
		file << "------------------" << endl;
		
	}
	
	file << "~~~~~~~~~~~~~~~~~~~~~SIMULATIONS HAVE ENDED~~~~~~~~~~~~~~~~~~" << endl;

	cout << "The Spartans have won: " << numberOfSpartanVictories << " time(s)" << endl;
	cout << "The Persians have won: " << numberOfPersianVictories << " time(s)" << endl;
	
	spartan_Final_Average = arithMean(sum_Spartans, numberOfTimes);
	persian_Final_Average = arithMean(sum_Persians, numberOfTimes);
	time_Final_Average = arithMean(sum_Time, numberOfTimes);

	cout << "Average number of living Spartans: " << spartan_Final_Average << endl;
	cout << "Average number of living Persians: " << persian_Final_Average << endl;
	cout << "Average battle length: " << time_Final_Average << endl;

	omega_Spartan = standard_deviation(soldier_count_Spartans, numberOfTimes, spartan_Final_Average);
	omega_Persian = standard_deviation(soldier_count_Persians, numberOfTimes, persian_Final_Average);
	omega_Time = standard_deviation(time_Count, numberOfTimes, time_Final_Average);

	cout << "Std. dev of living Spartans: " << omega_Spartan << endl;
	cout << "Std. dev of living Persians: " << omega_Persian << endl;
	cout << "Std. dev of battle length: " << omega_Time << endl;

	file << "TOTAL BATTLES WON BY EACH TEAM: " << endl;
	file << "Spartan victories: " << numberOfSpartanVictories << endl;
	file << "Persian victories: " << numberOfPersianVictories << endl;
	file << "STATISTICS:" << endl;
	file << "Average number of living Spartans: " << spartan_Final_Average << endl;
	file << "Average number of living Persians: " << persian_Final_Average << endl;
	file << "Average battle length: " << time_Final_Average << endl;
	file << "Std. Dev of living Spartans: " << omega_Spartan << endl;
	file << "Std. Dev of living Persians: " << omega_Persian << endl;
	file << "Std. Dev of battle length: " << omega_Time << endl;

	//cout << "I will show you where each soldier is in the heap: " << endl;
	
	//heap.determine_hash_positions(10);


	//heap.remove(5, removed);

	//cout << removed.return_iD() << endl;
	//cout << removed.return_actionTime() << endl;
	//Test min remove snippet

	/*soldier min;
	while(!heap.isEmpty()){
	cout << "1 if not empty, 0 if empty: " << !heap.isEmpty() << endl;
	min = heap.findMin();
	cout << "This soldier's ID is: " << new_Mook.return_iD() << endl;
	if(new_Mook.return_Faction() == true) cout << "Tis a Spartan!" << endl;
	else cout << "tis a persian" << endl;
	heap.deleteMin(min);

	}*/

	cout << "Done!" << endl;
	return 0;
}



void generate_Soldiers(BinaryHeap<soldier>& heap, int number_of_Spartans, int number_of_Persians, vector<int>& spartans, vector<int>& persians)
{
	soldier newSoldier, minimum;
	int faction_Selection, current_Spartans, current_Persians;
	current_Spartans = 0, current_Persians = number_of_Spartans;
	int timeValue = 0;

	while((number_of_Spartans > 0 || number_of_Persians > 0) )
	{
		faction_Selection = rand() % 2;//Select from 1 to 0 for the faction

		if(faction_Selection == 1 && (number_of_Spartans > 0)){

 			newSoldier.set_Faction(true);//Set the faction of our new soldier.
			timeValue = rand() % 51 + 1;
			newSoldier.set_actionTime(timeValue);

			newSoldier.set_iD(current_Spartans);//Sets the iD to the current value of i. We will step through this i times, creating i soldiers.

			spartans.push_back(current_Spartans);//The ID is pushed into the vector of soldiers.
			current_Spartans++;
			number_of_Spartans--;

			heap.insert(newSoldier);//Insert our newly created soldier

		}
		else if(faction_Selection == 0 && (number_of_Persians > 0)){

			newSoldier.set_Faction(false);


			timeValue = rand() % 900 + 51;
			newSoldier.set_actionTime( timeValue );
			newSoldier.set_iD(current_Persians);//Sets the iD to the current value of i. We will step through this i times, creating i soldiers.
			persians.push_back(current_Persians);
			current_Persians++;
			number_of_Persians--;

			heap.insert(newSoldier);//Insert our newly created soldier

		}
	}
}


void soldier_Info(soldier thisSoldier)
{
	//cout << "This soldier's ID is: " << thisSoldier.return_iD() << endl;
	//cout << "His action time is: " << thisSoldier.return_actionTime() << endl;
	if(thisSoldier.return_Faction() == true) cout << "Tis a Spartan!" << endl;
	else cout << "tis a persian" << endl;

}

/*
soldier generate_Soldier()
//A simple soldier generate function.
{
	soldier newSoldier;

	int faction = rand() % 2;//Generates a value from 0 to 1.

	if(faction == 1) newSoldier.set_Faction(true);//creates a new soldier with the number obtained above.
	else newSoldier.set_Faction(false);

	newSoldier.set_actionTime(rand() % 20);
	
	if(faction == true) newSoldier.set_iD(rand() % 300);
	else newSoldier.set_iD(rand() % 700 + 300);

	return newSoldier;

}*/

void kill_a_Soldier(vector<int>& soldier_Array, int& remaining_Soldiers, int dead_index)
//The way this works is that dead soldiers are kept from index remaining_Soldiers to the end of the array.
//So if a soldier is killed, we switch them out with the last index of the soldiers that are still considered alive.
{
	if(remaining_Soldiers - 1 == -1) return;
	//cout << "Kill a soldier will now begin" << endl;

	int tempID;
	
	tempID = soldier_Array[remaining_Soldiers - 1];
	soldier_Array[remaining_Soldiers - 1]  = soldier_Array[dead_index];
	soldier_Array[dead_index] = tempID;

	remaining_Soldiers--;

	//cout << "Kill a soldier has finished" << endl;
}

int obtain_iD(vector<int>& soldiers, int remaining_Soldiers)
//Simply grab a soldier ID from our list of available soldiers.
{
	int randomValue = rand() % remaining_Soldiers;
	return soldiers[randomValue];
}


//Will pick a soldier from our remaining soldiers alive.
//
void processTurn(BinaryHeap<soldier>& heap, vector<int>& soldiers, int& remaining_Soldiers)
{
	int position_of_Soldier;
	if(remaining_Soldiers == 1) position_of_Soldier = 0;//A condition to ensure kill a soldier is not called when only one soldier is left
	else position_of_Soldier = rand() % remaining_Soldiers;

	bool target_Condition;
	
	soldier nextUp = heap.findMin(); //Return the smallest value; our next up.

	target_Condition = heap.interact_with_Soldier(nextUp.return_Faction(), soldiers[position_of_Soldier]);//False if alive, true if killed
	if(target_Condition && nextUp.return_Faction() ) kill_a_Soldier(soldiers, remaining_Soldiers, position_of_Soldier);//Will removed the soldier if dead.
	else if(target_Condition && !(nextUp.return_Faction()) ) {//Will invigorate if a soldier is killed, AND the soldier target was a Persian

		kill_a_Soldier(soldiers, remaining_Soldiers, position_of_Soldier);//Remove the soldier as per usual
		invigorate(heap, soldiers, remaining_Soldiers);//function invigorate will go through the entire array to invigorate soldiers.
	}

}

void invigorate(BinaryHeap<soldier>& heap, vector<int>& soldiers, int remaining_Soldiers)
//Stepping through all remaining soldiers, we randomly subtract a small value to allow them to act faster.
{
	int currentSoldier, delta;

	if(remaining_Soldiers == 0){//We cannot invigorate an array of dead soldiers. 
		 return;
	}
	
	for(int i = 0; i < remaining_Soldiers; i++)
	{
		soldier current = heap.find_by_ID(i);

		currentSoldier = soldiers[i];
		delta = (rand() % 2 + 1);//determine a random amount to decrease by
		heap.decrease_by_iD(currentSoldier, delta);//Manipulate our current heap so that spartans act a little quicker.
		
	}

}

void printOutIDs(vector<int> soldiers, int numberOfSoldiers)
{
	for(int i = 0; i < numberOfSoldiers; i++)
	{
		cout << soldiers[i] << endl;
	}

}

double arithMean(double sum, int N)
{
	cout << "The sum is: " << sum << endl;
	return static_cast<double>(( sum / static_cast<double>(N)));
}

double standard_deviation(vector<double> values, int N, double mean)
{
	double currentSum = 0, distance = 0;

	for(int i = 0; i < values.size(); i++)
	{
		distance = pow((mean - values[i]), 2);
		currentSum += distance;
	}

	return sqrt(( currentSum / N ));
}

