#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;
struct Data{
    string Name;
    int PID;
    string sessionname;
    string session;
    int memoryUsage;
};
class process{
    vector<Data> processes;
public:
    process(){processes.clear();}
    void getprocesses(){
        system("tasklist /FO CSV /NH > tasklist_output.txt"); // Put the data in a file
        ifstream file("tasklist_output.txt");
        if (!file) {
            cout << "Failed to get process list." << endl;
            exit(0);
        }
        string line;
        while(getline(file,line)){ // Separate the data
            istringstream ss(line);
            string name,pid,sessionname,setion,memoryuse;
            getline(ss,name,',');
            getline(ss,pid,',');
            getline(ss,sessionname,',');
            getline(ss,setion,',');
            getline(ss,memoryuse);
            string Name = name.substr(1, name.size() - 2); // Remove quotes
            string PID = pid.substr(1, pid.size() - 2);
            string SessionName = sessionname.substr(1, sessionname.size() - 2);
            string Session = setion.substr(1, setion.size() - 2);
            string memory = "";
            for(int i = 0 ; i < memoryuse.size() ; i++){
                if(memoryuse[i] != ','){
                    memory += memoryuse[i];
                }
            }
            string memoryUsage = memory.substr(1, memory.size() - 4);
            processes.push_back({Name,stoi(PID),SessionName,Session,stoi(memoryUsage)});
        }
        file.close();
    }
    void display(){
        for(auto & processe : processes){
            cout << processe.Name <<
                 setw(45-processe.Name.size()) << setprecision(1) << processe.PID <<
                 "   " << processe.sessionname << setw(30 - processe.sessionname.size()) << setprecision(1) <<
                 processe.session << setw(20) << setprecision(1);
            if(processe.memoryUsage > 999999){ // separate the large numbers with comma(,)
                cout << to_string(processe.memoryUsage).
                        insert(to_string(processe.memoryUsage).length()-3,",").
                        insert(to_string(processe.memoryUsage).length()-6,",") << " K" << endl;
            }else {
                (processe.memoryUsage > 999 ?
                 cout << to_string(processe.memoryUsage).insert(to_string(processe.memoryUsage).length() - 3, ",") << " K" << endl :
                 cout << processe.memoryUsage << " K" << endl);
            }
        }
    }
    void sortByPID(){
        for(int i = 0 ; i < processes.size()-1 ; i++){
            if(processes[i].PID > processes[i+1].PID){
                swap(processes[i],processes[i+1]);
                sortByPID();
            }
        }
        display();
        exit(0);
    }
    void sortByName(){
        for(int i = 0 ; i < processes.size()-1 ; i++){
            if(processes[i].Name[0] > processes[i+1].Name[0]){
                swap(processes[i],processes[i+1]);
                sortByName();
            }
        }
        display();
        exit(0);
    }
    void sortByMemory(){
        for(int i = 0 ; i < processes.size()-1 ; i++){
            if(processes[i].memoryUsage > processes[i+1].memoryUsage){
                swap(processes[i],processes[i+1]);
                sortByMemory();
            }
        }
        display();
        exit(0);
    }
};
int main(){
    process process;
    process.getprocesses();
    cout << "How do you want to sort the Task Manager?" << endl;
    cout << "1) By name\n"
            "2) By PID\n"
            "3) By memory usage\n"
            "4) Don't sort\n";
    int choice;
    cin >> choice;
    cout << "Name" << setw(55) << setprecision(1) << "PID  Session name" << setw(20) << setprecision(1) <<
         "Session#" << setw(22) << setprecision(1) << "Memory Usage (K)" << endl;
    cout << "==============================   ============  =====================   ============  ==================\n";
    if(choice == 1){
        process.sortByName();
    }else if(choice == 2){
        process.sortByPID();
    }else if(choice == 3){
        process.sortByMemory();
    }else if(choice == 4){
        process.display();
    }
}