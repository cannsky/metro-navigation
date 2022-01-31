#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*
	Student Name, Surname: Can Gök
	Student ID: 150118014
	Program Definition: Program installs a metro system and calculates the best path for moving from a station to the another station,
	also program can be used to find the closest metro station with given x,y values.
*/

//Define size as 10
#define SIZE 10

//State
int beforeState = 0;
int afterState = 0;

//MetroStation structure which includes name, x and y values of the metro station
typedef struct MetroStation {
	char name[20];
	double x;
	double y;
} MetroStation;

//MetroLine structure which includes name and MetroStations of the metro line
typedef struct MetroLine {
	char color[20];
	MetroStation MetroStations[SIZE];
} MetroLine;

//MetroSystem structure which includes name and MetroLines of the MetroSystem
typedef struct MetroSystem {
	char name[20];
	MetroLine MetroLines[SIZE];
} MetroSystem;

//Pointer array for garbage collection
MetroLine *MetroLinePtr[100];
MetroSystem *MetroSystemPtr[100];

//Create a new metro system
MetroSystem istanbul = {"istanbul", "\0"};

//Method for collecting the garbage
/*
	Garbage collector will look if the pointer array included with the given arrays pointer.
	If it is not added to the pointer array then the garbage collector will collect the garbage of the array
	When the garbage is collected the array's pointer will be added to the pointer array
*/
int garbageCollectorMetroLine(MetroLine *metroLine){
	int i;
	for(i = 0; i < 100; i++) if(MetroLinePtr[i] == metroLine) return 0;
	for(i = 0; i < SIZE; i++) strcpy(metroLine->MetroStations[i].name, "");
	for(i = 0; i < 100; i++){
		if(MetroLinePtr[i] == 0){
			MetroLinePtr[i] = metroLine;
			return 0;
		}
	}
	return 0;
}

//Method for collecting the garbage
/*
	Garbage collector will look if the pointer array included with the given arrays pointer.
	If it is not added to the pointer array then the garbage collector will collect the garbage of the array
	When the garbage is collected the array's pointer will be added to the pointer array
*/
int garbageCollectorMetroSystem(MetroSystem *metroSystem){
	int i;
	for(i = 0; i < 100; i++) if(MetroSystemPtr[i] == metroSystem) return 0;
	for(i = 0; i < SIZE; i++) strcpy(metroSystem->MetroLines[i].color, "");
	for(i = 0; i < 100; i++){
		if(MetroSystemPtr[i] == 0){
			MetroSystemPtr[i] = metroSystem;
			return 0;
		}
	}
	return 0;
}

//Method for collecting the garbage of neighboring stations
void garbageCollectorNeighboringStations(MetroStation neighboringStations[]){
	int i;
	for(i = 0; i < SIZE; i++) strcpy(neighboringStations[i].name, "");
}

//Method looks if two stations are the same or not
int equals(MetroStation s1, MetroStation s2){
	//If same return 1, if not return 0
	if(strcmp(s1.name, s2.name) == 0) return 1;
	else return 0;
}

//Method adds station to the metro lines metro stations array
void addStation(MetroLine *metroLine, MetroStation metroStation){
	garbageCollectorMetroLine(metroLine);
	int i;
	for(i = 0; i < SIZE; i++)
		if(strcmp(metroLine->MetroStations[i].name, "") == 0){
			metroLine->MetroStations[i] = metroStation;
			break;
		}
}

//Method will look if the station is included in metro line or not
int hasStation(MetroLine metroLine, MetroStation metroStation){
	int i;
	for(i = 0; i < SIZE; i++)
		//If the metro station is included return 1
		if(strcmp(metroLine.MetroStations[i].name, metroStation.name) == 0) return 1;
	return 0;
}

//Get's the first stop of the metro line
MetroStation getFirstStop(MetroLine metroLine){
	MetroStation metroStation;
	if(strcmp(metroLine.MetroStations[0].name, "") == 0) return metroStation;
	return metroLine.MetroStations[0];
}

//Gets the previous stop of the metro station of the given metro line
/*
	method will find the metro station with the given name and will return the previous stop
	if this is the first stop method will return empty metro station
*/
MetroStation getPreviousStop(MetroLine metroLine, MetroStation metroStation){
	MetroStation newMetroStation;
	int i;
	for(i = 0; i < SIZE; i++)
		if(strcmp(metroLine.MetroStations[i].name, metroStation.name) == 0)
			if(i == 0) return newMetroStation;
			else return metroLine.MetroStations[i-1];
	return newMetroStation;
}

//Gets the next stop of the given metro line
/*
	previous method will find the metro station with the given name and will return the next stop
	if this is the last stop method will return empty metro station
*/
MetroStation getNextStop(MetroLine metroLine, MetroStation metroStation){
	MetroStation newMetroStation;
	int i;
	for(i = 0; i < SIZE; i++)
		if(strcmp(metroLine.MetroStations[i].name, metroStation.name) == 0)
			if(i == (SIZE - 1)) return newMetroStation;
			else return metroLine.MetroStations[i+1];
	return newMetroStation;
}

//Add line will add a new line to the metrosystem
void addLine(MetroSystem *metroSystem, MetroLine metroLine){
	garbageCollectorMetroSystem(metroSystem);
	int i;
	for(i = 0; i < SIZE; i++)
		if(strcmp(metroSystem->MetroLines[i].color, "") == 0){
			metroSystem->MetroLines[i] = metroLine;
			break;
		}
}

//Print line prints the metro line's every metro station
void printLine(MetroLine metroLine){
	printf("Metroline %s: ", metroLine.color);
	int i;
	for(i = 0; i < SIZE; i++){
		if(strcmp(metroLine.MetroStations[i].name, "") != 0){
			printf("%s", metroLine.MetroStations[i]);
			if(i != (SIZE-1)){
				if(strcmp(metroLine.MetroStations[i+1].name, "") == 0) printf(".\n");
				else printf(", ");
			}
			else printf(".\n");
		}
	}
}

//print path will print the given metro station array to the screen
void printPath(MetroStation metroStations[]){
	int i;
	for(i = 0; i < SIZE; i++){
		if(strcmp(metroStations[i].name, "") != 0) printf("%d. %s\n", i+1, metroStations[i].name);
	}
}

//Get distance travelled will calculate the 2D distance between every station and sum them to the single variable
//method will return the total distance travelled between stations
double getDistanceTravelled(MetroStation metroStations[]){
	double sum = 0;
	int i;
	for(i = 0; i < SIZE; i++){
		if(strcmp(metroStations[i+1].name, "") == 0) break;
		double xSquare = (metroStations[i+1].x - metroStations[i].x) * (metroStations[i+1].x - metroStations[i].x);
		double ySquare = (metroStations[i+1].y - metroStations[i].y) * (metroStations[i+1].y - metroStations[i].y);
		sum += sqrt(xSquare + ySquare);
	}
	return (i > 0) ? sum : 0.0;
}

//Calculate distance will calculate the distance between the x,y coordinates and the metro stations coordinates
//will return the distance calculated
double calculateDistance(double x, double y, double metroStationX, double metroStationY){
	double xSquare = (x - metroStationX) * (x - metroStationX);
	double ySquare = (y - metroStationY) * (y - metroStationY);
	return sqrt(xSquare + ySquare);
}

//Finds the nearest station in the metro system for the given x and y values.
/*
	Method will create a new temporary metro station to store the first metro station of the metro system
	If there are more than one station, method will calculate the differences of two stations to the given x,y values
	The closest distance will be stored in the metro station
	This will be continued until every metro station's distance has been calculated in the metro system.
*/
MetroStation findNearestStation(MetroSystem metroSystem, double x, double y){
	//Controls if there is any metro line in the metro system.
	MetroStation metroStation = {""};
	if(strcmp(metroSystem.MetroLines[0].color, "") == 0){
		printf("There is no metro line on the system!");
		return metroStation;
	}
	//Controls if there is any metro station on the system.
	if(strcmp(metroSystem.MetroLines[0].MetroStations[0].name, "") == 0){
		printf("There is no metro station on the system!");
		return metroStation;
	}
	int i, j;
	for(i = 0; i < SIZE; i++){
		if(strcmp(metroSystem.MetroLines[i].color, "") == 0) break;
		for(j = 0; j < SIZE; j++){
			if(strcmp(metroSystem.MetroLines[i].MetroStations[j].name, "") == 0) break;
			if(strcmp(metroStation.name, "") != 0) {
				if(calculateDistance(x, y, metroSystem.MetroLines[i].MetroStations[j].x, metroSystem.MetroLines[i].MetroStations[j].y) < calculateDistance(x, y, metroStation.x, metroStation.y)) metroStation = metroSystem.MetroLines[i].MetroStations[j];
			}
			else metroStation = metroSystem.MetroLines[i].MetroStations[j];
		}
	}
	return metroStation;
}

//Method will get the neighboring stations the given metro station in the metro system
/*
	Method will look for every metro station in the system to find the given metro station
	When the station is found the method will get the next and previous metro station of the metro station
	If there are more than one metro station as neighbour only one of them will be stored to the array
	Neighboring stations will be stored to the neighboring stations referenced array
*/
void getNeighboringStations(MetroSystem metroSystem, MetroStation metroStation, MetroStation neighboringStations[]){
	garbageCollectorNeighboringStations(neighboringStations);
	int i, j, k;
	for(i = 0; i < SIZE; i++){
		if(strcmp(metroSystem.MetroLines[i].color, "") == 0) break;
		for(j = 0; j < SIZE; j++){
			if(strcmp(metroSystem.MetroLines[i].MetroStations[j].name, "") == 0) break;
			if(strcmp(metroStation.name, metroSystem.MetroLines[i].MetroStations[j].name) == 0){
				beforeState = 1;
				afterState = 1;
				for(k = 0; k < SIZE; k++){
					if(strcmp(neighboringStations[k].name, "") == 0){
					if(strcmp(neighboringStations[k].name, metroSystem.MetroLines[i].MetroStations[j-1]) == 0) beforeState = 0;
					if(strcmp(neighboringStations[k].name, metroSystem.MetroLines[i].MetroStations[j+1]) == 0) afterState = 0;
						if(beforeState == 1) neighboringStations[k] = metroSystem.MetroLines[i].MetroStations[j-1];
						if(afterState == 1) neighboringStations[k+beforeState] = metroSystem.MetroLines[i].MetroStations[j+1];
						break;
					}
				}
			}
		}
	}
}

//Method will look if the given metro station is on the referenced array
int controlArray(MetroStation content, MetroStation partialPath[]){
	int i;
	for(i = 0; i < SIZE; i++){
		if(strcmp(partialPath[i].name, "") == 0) return 1;
		if(strcmp(partialPath[i].name, content.name) == 0) return 0;
	}
}

//Method will make two arrays equal each other
void equalArrays(MetroStation partialPath[], MetroStation duplicatePath[]){
	int i;
	for(i = 0; i < SIZE; i++) duplicatePath[i] = partialPath[i];
}

//Method adds given metro station to the and of the referenced array
void addArrayElement(MetroStation start, MetroStation duplicatePath[]){
	int i;
	for(i = 0; i < SIZE; i++){
		if(strcmp(duplicatePath[i].name, "") == 0){
			duplicatePath[i] = start;
			break;
		}
	}
}

//Find path will be called firstly in the main method, then will call the recursiveFindPath function
void findPath(MetroStation start, MetroStation finish, MetroStation path[]){
	MetroStation partialPath[SIZE];
	garbageCollectorNeighboringStations(partialPath);
	recursiveFindPath(start, finish, partialPath, path);
}

void recursiveFindPath(MetroStation start, MetroStation finish, MetroStation partialPath[], MetroStation bestPath[]){
	int i;
	//Create a new neighboring stations array
	MetroStation neighboringStations[SIZE];
	//Collect the garbage of the neighboring stations array
	garbageCollectorNeighboringStations(neighboringStations);
	//If start metro station included in partial path, return the function
	if(controlArray(start, partialPath) != 1) return;
	//If start and Finish are same, set bestPath as partialPath
	if(equals(start, finish) == 1){
		//If there is no best path, the partial path will be the best path
		if(strcmp(bestPath[0], "") == 0) equalArrays(partialPath, bestPath);
		//If the partial path distance is lower than the best path, new best path will be partial path
		else if(getDistanceTravelled(partialPath) < getDistanceTravelled(bestPath)){
			equalArrays(partialPath, bestPath);
			addArrayElement(start, bestPath);
		}
		//return void
		return;
	}
	//If partial path is empty, take neighboring stations of start metro station
	getNeighboringStations(istanbul, start, neighboringStations);
	//Look for every neighboring station
	for(i = 0; i < SIZE; i++){
		//Create a new metro station array duplicate path
		MetroStation duplicatePath[SIZE];
		//Collect the garbage of the duplicate path
		garbageCollectorNeighboringStations(duplicatePath);
		//Copy the elements of the partial path to the duplicate path
		equalArrays(partialPath, duplicatePath);
		//If there is no more neighboring stations exit for loop
		if(strcmp(neighboringStations[i].name, "") == 0) break;
		//Add start to the duplicate path array
		addArrayElement(start, duplicatePath);
		//Call recursive find path function for finding the path
		recursiveFindPath(neighboringStations[i], finish, duplicatePath, bestPath);
	}
}

int main(int argc, char *argv[]) {
	int i;
	double myX=1, myY=2; 
	double goalX=62, goalY=45; 
	
	// define 3 metro lines, 9 metro stations, and an empty myPath
	MetroLine red={'\0'}, blue={'\0'}, green={'\0'};
	MetroStation s1, s2, s3, s4, s5, s6, s7, s8, s9;
	MetroStation myPath[SIZE]={'\0'};
	
	strcpy(red.color, "red"); 
	strcpy(blue.color, "blue");
	strcpy(green.color, "green");
	
	strcpy(s1.name, "Haydarpasa"); 		s1.x=0; 	s1.y=0;
	strcpy(s2.name, "Sogutlucesme"); 	s2.x=10; 	s2.y=5;
	strcpy(s3.name, "Goztepe"); 		s3.x=20; 	s3.y=10;
	strcpy(s4.name, "Kozyatagi"); 		s4.x=30; 	s4.y=35;
	strcpy(s5.name, "Bostanci"); 		s5.x=45; 	s5.y=20;
	strcpy(s6.name, "Kartal"); 			s6.x=55; 	s6.y=20;
	strcpy(s7.name, "Samandira"); 		s7.x=60; 	s7.y=40;
	strcpy(s8.name, "Icmeler"); 		s8.x=70; 	s8.y=15;
	
	//Add several metro stations to the given metro lines.
	addStation(&red, s1); addStation(&red, s2); addStation(&red, s3); addStation(&red, s4); addStation(&red, s5); addStation(&red, s8);
	
	addStation(&blue, s2); addStation(&blue, s3); addStation(&blue, s4); addStation(&blue, s6); addStation(&blue, s7);
	
	addStation(&green, s2); addStation(&green, s3); addStation(&green, s5); addStation(&green, s6); addStation(&green, s8);
	
	addLine(&istanbul, red);
	addLine(&istanbul, blue);
	addLine(&istanbul, green);
	
	// print the content of the red, blue, green metro lines
	printLine(red);
	printLine(blue);
	printLine(green);
	
	// find the nearest stations to the current and target locations
	MetroStation nearMe = findNearestStation(istanbul, myX, myY);
	MetroStation nearGoal = findNearestStation(istanbul, goalX, goalY);
	
	printf("\n");
	
	printf("The best path from %s to %s is:\n", nearMe.name, nearGoal.name);
	
	// if the nearest current and target stations are the same, then print a message and exit.
	if(equals(nearMe, nearGoal)){
		printf("It is better to walk!\n");
		return 0;
	}
	
	// Calculate and print the myPath with the minimum distance travelled from start to target stations.
	findPath(nearMe, nearGoal, myPath);
	
	if(strlen(myPath[0].name) == 0)
		printf("There is no path on the metro!\n");
	else{
		printPath(myPath);
	}
	
	return 0;
}
