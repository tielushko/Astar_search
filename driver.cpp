#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <limits>
#include <queue>
#include <list>
#include <algorithm> 
using namespace std;

using Graph = std::map<string, vector<string>>; 
using Location = std::map<string, tuple<int, int, bool>>;

bool byCity;                        //a bool for choice of cities or not
bool stepByStep;                    //a bool for choice if stepByStep;

Graph graph;                        //graph for the algorithm implementation
Location locationMap;               //map to keep track of each node

//the functions that will be used in our code
void AstarDistance(string start, string finish);
double heuristic(string start, string end);

int main() {
    
   //initializing variables
   ifstream in;
   string cityName;
   int xC, yC, numFollows;
   string connectionName;
   
   in.open("locations.txt");
   //file error check 
   if(!in.is_open()) 
       cerr << "couldn't open the file"; 
   
   //extraction from locations.txt
   while(in >> cityName >> xC >> yC) {
       locationMap.insert({cityName, make_tuple(xC, yC, false)}); 
   }
   
   //;)
   in.close();
    
   //extraction from connections.txt w/error check
   in.open("connections.txt");
   if(!in.is_open()) 
       cerr << "couldn't open the file"; 
   
   while (in >> cityName >> numFollows) {
       vector<string> myVec;
       for (int i = 0; i < numFollows; i++) {
           in >> connectionName;
           myVec.push_back(connectionName); 
       }
       graph.insert(pair<string, vector<string>>(cityName, myVec)); 
   }
   //;)
   in.close();
   //getting input from user
   string startLocation;
   string endLocation;
   string path;
   cout << "Please enter your starting location: " << endl;
   cin >> startLocation;
   
   //error check
   while(locationMap.count(startLocation) == 0) {
       cout << "The starting location does not exist! Please, enter the correct start!" << endl;
       cin.clear();
       cin.ignore(numeric_limits<streamsize>::max(), '\n');
       cin >> startLocation;
   }
   cout << "Please enter your ending location: " << endl;
   cin >> endLocation;
   //error check
   while(locationMap.count(endLocation) == 0) {
       cout << "The ending location does not exist! Please, enter the correct end!" << endl;
       cin.clear();
       cin.ignore(numeric_limits<streamsize>::max(), '\n');
       cin >> endLocation;
   }

   //need to include the cities which user does not want to include in the final path. -- works and tested with a map traversal
   //can select 1 or more cities
   string dontVisit;
   cout << "Please type the cities you do not want to include in your final path! Separate by space or enter. Type 'q' or 'done' to exit: \n";
   while(true) {
      cin >> dontVisit;
      if (dontVisit == "q" || dontVisit == "done") {
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         break;
      }
      if (locationMap.count(dontVisit) == 0) {
         cout << "Invalid city entered! Please, enter the correct city!" << endl;
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         continue;
      }
      get<2>(locationMap[dontVisit]) = true; 
   }

   //ask if the want a step by step option
   char c;
   cout << "Would you like a step-by-step option? Press Y for yes, N for no" << endl;
   cin >> c;
   while(c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
      cout << "Please, enter the correct choice! \n Y or y to choose step-by-step \n N or n to not." << endl;
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cin >> c;
   }
   if (c == 'y' || c == 'Y'){
      cout << "You chose step-by-step!" << endl;
      stepByStep = true;
   }else{ 
      cout << "You chose no step-by-step!" << endl;
      stepByStep = false;
   }

   //which heuristic does the user want to use? The straight line distance or fewest cities. --tested. works
   int heuristic;
   cout << "Please, enter a number of the heuristic you would like to use in the program:" << endl;
   cout << "1. Straight-line distance." << endl;
   cout << "2. Fewest cities visited" << endl;
   cin >> heuristic;
   while(heuristic != 1 && heuristic != 2) {
      cout << "Please, enter the correct heuristic choice! \n 1. Straight-line distance \n 2. Fewest cities visited." << endl;
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cin >> heuristic;
   }

    if(heuristic == 2) {
        cout << "You chose Fewest Cities Visited Heuristic!\n\n" << endl;
        byCity = true;
    } else {
        cout << "You chose Straigh-Line Distance Heuristic!\n\n" << endl;
        byCity = false;
    }

   //run A*
   AstarDistance(startLocation, endLocation);
   return 0;    
}

void AstarDistance(string start, string finish) {

    //two maps to keep of where the origin of the node and cost so far.
    map<string, string> comeFrom; 
    map<string, double> costSoFar;

    //priority queue to traverse the graph. Sorts the values by their lowest distances.
    std::priority_queue<pair<int, string>, std::vector<pair<int, string>>, std::greater<pair<int, string>>> PQ;

    pair<int,string> current;
    string parent;
    double cost;


    PQ.push(make_pair(heuristic(start, finish), start));
    comeFrom[start] = start;
    costSoFar[start] = 0;

    //in case the start is the finish
    if (start == finish) {
        cout << "You are already at a finish. \nTotal distance: 0" << endl;
        return;
    }


    //the loop to traverse the PQ
    while(!PQ.empty() && get<2>(locationMap[finish]) == false) {
        current = PQ.top();
        parent = current.second;
        get<2>(locationMap[parent]) = true;
        
        if(stepByStep == true) {
        //cout << "**********************************************\n\n";
         cout << "City Selected: " << current.second << "\n\n";
         cout << "Possible Cities to travel to: ";
        }

        PQ.pop();

        for (string child : graph[parent]) {
            //if child belongs to closed list, ignore it and move on
            if(get<2>(locationMap[child]) == true)
                continue;
            if(stepByStep == true)
                cout << child << " ";
            
            //distance total = distance travelled before child +  distance between child and parent
            cost = costSoFar[parent] + heuristic(child, parent);
         
            //if the child wasnt the part of the open list, we need to push it in and add the distance travelled for the child. 
            if (costSoFar.find(child) == costSoFar.end() || cost < costSoFar[child]) {
                costSoFar[child] = cost;
                double priority = cost + heuristic(child, finish); //calculate the distanceTOTAL as distance travelled and heuristic
                PQ.push(make_pair(priority,child));                //insert into the queue.
                comeFrom[child] = parent;                          //put the key for child into the map to remember which parent we accessed it.
            } 
        }
    
        if (stepByStep == true) {
            std::priority_queue<pair<int, string>, std::vector<pair<int, string>>, std::greater<pair<int, string>>> notPQ = PQ; //copy PQ to traverse the copy for step-by-step
            cout << "\n\nCities at the end of possible paths: ";
            
            //output the current contents of the PQ. 
            while(!notPQ.empty()){
                cout << notPQ.top().second << "(" << notPQ.top().first << ") ";
                notPQ.pop();
            }
            
            cout << endl;
            cout << "\n\n**********************************************\n\n";
      }
    }
   
    //if we were unable to reach the final destination
    if(PQ.empty() && get<2>(locationMap[finish]) == false) {
        cout << "There is no path to your Destination.\n\n";
        return;
    }

    //this section of code is responsible for deciphering the path that the A* algorithm found using deque (could use stack too)
    //and manipulating the comeFrom map
    
    deque<string> pathDeque;
    double distance = 0.0;
    string end = finish;
    
    //filling in the deque with the contents we read from the map comeFrom
    pathDeque.push_front(end);
    while (comeFrom[end] != start) {
        string origin = comeFrom[end];
        pathDeque.push_front(origin);
        end = origin;
    }
    pathDeque.push_front(start); 

    //output
    while (pathDeque.size() != 1) {
       string d1, d2;
       d1 = pathDeque.front();
       pathDeque.pop_front();
       d2 = pathDeque.front();
       distance += heuristic(d1, d2);
       cout << d1 << " to " << d2 << " length: " << heuristic(d1, d2) << endl;
    }
   cout << "Total path length: " << distance << endl;
   
   return;
}

double heuristic(string start, string end) {
    double distance;
    if(byCity == true) {
        distance = 1;
        if (start == end) { //not really needed.
            distance = 0;
        }
    } else {
        distance = sqrt(pow((get<0>(locationMap[end]) - get<0>(locationMap[start])), 2) + pow((get<1>(locationMap[end]) - get<1>(locationMap[start])), 2));
    }
   return distance;
}
