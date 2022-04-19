#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

string fill(string line, string next_line, int row, int &link);
int nextAccessPoint(string line, int link);

int main() {
	// Create a text string, which is used to output the text file
	string line;
	vector <string> inputLabyrinth;
	vector <string> roadMap;
	string filename = "input.txt";
	bool check = true; // check = true means that the labyrinth has not a closed road (######)

	// Read from the text file
	ifstream MyReadFile(filename);

	// Use a while loop together with the getline() function to read the file line by line
	while (getline (MyReadFile, line)) {
		// Output the text from the file
		cout << line << endl;
		inputLabyrinth.push_back(line);
	}

	// Close the file and continue using the array
	MyReadFile.close();
	
	// check if the number of columns for all rows of the labyrinth are equal
	int col_number = inputLabyrinth[0].length();
	for(int i = 1; i < inputLabyrinth.size(); i++){
		if(inputLabyrinth[i].length() != col_number){
			cout << "The labyrinth shape is not consistant! (should be n*n)" << endl;
			return 1; // exit the program
		}
	}
	
	// Check if the labyrinth has an access and exit point and no closed road (#####)
	for(int i = 0; i < inputLabyrinth.size(); i++){
		for(int j = 0; j < inputLabyrinth[i].length(); j++){
			if(inputLabyrinth[i][j] != '.'){
				check = false;
			} else {
				check = true;
				break;
			}
			
			if(check == false && j == inputLabyrinth[i].length()-1){
				goto follow; // no need to continue looping if found closed road (#####)
			}
		}
	}
	
	follow:
	if(check){
		// count the access points tried
		int counter = 0;
		
		repeat:
		// road keeps the indexes of the roads like: "0,0; 1,0; 1,1; ..."
		string road = "";
		// index to link between rows (because +2 jump, taking rows one by one)
		int link = -1;
		// index to loop over line (row) characters
		int j = 0;
		for(int i = 0; i < inputLabyrinth.size(); i++){
			if(counter >= inputLabyrinth[0].length() && i == 0){
				goto follow2;
			}
			
			j = (link != -1) ? link : 0; // link != -1 means: we are inside laybirinth (start and end line excluded)
			
			if(i == 0){
				j = counter; // search the next access point
			}
			
			if(inputLabyrinth[i][j] == '.'){
				if(i == 0){
					link = j;
					road += fill(inputLabyrinth[i], inputLabyrinth[i+1], i, link);
					counter = nextAccessPoint(inputLabyrinth[i], j);
				} else if(i == inputLabyrinth.size()-1){
					road += fill(inputLabyrinth[i], inputLabyrinth[i], i, link);
				} else {
					road += fill(inputLabyrinth[i], inputLabyrinth[i+1], i, link);
				}
				
				if(link == -1){
					break;
				}
			} else {
				if(i == inputLabyrinth.size()-1){ // if we reach the exit line without an exit point
					goto special;
				} else {
					counter++;
					goto repeat;
				}
			}
		}
		
		if(road != ""){
			cout << road << endl;
			roadMap.push_back(road);
		}
		counter++;
		goto repeat;
		
		follow2:
		// extract the large road index from the roadMap
		int large_index = 0;
		for(int i = 0; i < roadMap.size()-1; i++){
			if(roadMap[i+1].length() > roadMap[large_index].length()){
				large_index = i+1;
			}
		}
		
		// write the road in the output file
		int road_size = 0;
		for(int i = 0; i < roadMap[large_index].length(); i++){
			if(roadMap[large_index][i] == ';'){
				int row = roadMap[large_index][i-3] - '0'; // - '0' is to convert to int
				int column = roadMap[large_index][i-1] - '0';
				inputLabyrinth[row][column] = road_size + '0'; // + '0' is to convert to char
				road_size++;
			}
		}
		
		// write output to file
		filename = "output.txt";
		ofstream outFile(filename);
		outFile << road_size++ << endl;
		for(int i = 0; i < inputLabyrinth.size(); i++){
			outFile << inputLabyrinth[i] << endl;
		}
	} else { // doesn't contain
		special:
		// write output to file
		filename = "output.txt";
		ofstream outFile(filename);
		outFile << -1 << endl;
		for(int i = 0; i < inputLabyrinth.size(); i++){
			outFile << inputLabyrinth[i] << endl;
		}
	}
	
	return 0;
}

string fill(string line, string next_line, int row, int &link){
	int left_edge = -1, right_edge = -1;
	string road = "";
	int next = -1;
	
	for(int i = link; i >= 0; i--){
		if(line[i] == '#'){
			left_edge = i+1;
			break;
		} else {
			if(next_line[i] == '.' && next == -1){
				next = i;
			}
		}
	}
	
	for(int i = link+1; i < line.length(); i++){
		if(line[i] == '#'){
			right_edge = i-1;
			break;
		} else {
			if(next_line[i] == '.' && next == -1){
				next = i;
			}
		}
	}
	
	link = next;
	
	if(left_edge != -1 && right_edge != -1){
		if(row % 2 == 0){
			for(int i = left_edge; i <= right_edge; i++){
				road += to_string(row) + "," + to_string(i) + ";";
			}
		} else {
			for(int i = right_edge; i >= left_edge; i--){
				road += to_string(row) + "," + to_string(i) + ";";
			}
		}
	}
	
	return road;
}

int nextAccessPoint(string line, int column){
	int next = -1;
	
	for(int i = column+1; i < line.length(); i++){
		if(line[i] == '#'){
			next = i+1;
			break;
		}
	}
	
	return next;
}
