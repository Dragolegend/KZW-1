#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

struct Task{
    int time;
    int weight;
    int max_time;
    int n;
};

class Resolver{
    public:
    std::vector<Task> tasks;

    std::vector<std::string> files;

    std::vector <std::string> custom_split(std::string str, char separator) {
        std::vector <std::string> strings;
        int startIndex = 0, endIndex = 0;
        for (int i = 0; i <= str.size(); i++) {
            
            if (str[i] == separator || i == str.size()) {
                endIndex = i;
                std::string temp;
                temp.append(str, startIndex, endIndex - startIndex);
                strings.push_back(temp);
                startIndex = endIndex + 1;
            }
        
        }
        return strings;
    }

    void load_data(std::string path){
        std::ifstream inputFile(path);
        
        tasks.erase(tasks.begin(), tasks.end());
        
        if (inputFile.is_open()) {
            std::string line;
            int count = 1;
            while (std::getline(inputFile, line)) {

                std::vector<std::string> parts = custom_split(line, ' ');
                
                if(parts.size() == 3){
                    Task tmp;
                    tmp.time = stoi(parts.at(0));
                    tmp.weight = stoi(parts.at(1));
                    tmp.max_time = stoi(parts.at(2));
                    tmp.n = count++;

                    tasks.push_back(tmp);
                }
            }

            inputFile.close();
        }
    }

    void print_tasks(std::vector <Task> tasks) {
        std::cout << "\n\n\n";
        for(Task e : tasks){
            std::cout << "Time: " << e.time << " Weight: " << e.weight << " Max time: " << e.max_time <<  " n: " << e.n << "\n";
        }

        std::cout << "\nOrder: ";

        for(Task e : tasks){
            std::cout << e.n << ' ';
        }
        std::cout << "\n\n\n";
    }

    int penalty(std::vector <Task> tasks){
        int time = 0;
        int penalty = 0;

        for(int i = 0; i < tasks.size(); ++i){
            time += tasks.at(i).time;
            if(tasks.at(i).max_time < time){
                penalty += tasks.at(i).weight * (time - tasks.at(i).max_time);
            }
        }

        return penalty;
    }

    int penalty2(std::vector <Task> tasks, std::string mask){
        int time = 0;
        int penalty = 0;

        for(int i = 0; i < tasks.size(); ++i){
            if(mask.at(i) == '1'){           
                time += tasks.at(i).time;
                if(tasks.at(i).max_time < time){
                    penalty += tasks.at(i).weight * (time - tasks.at(i).max_time);
                }
            }
        }

        return penalty;
    }

    std::vector<Task> random(std::vector<Task> tasks){
        std::vector <Task> tmp = tasks;
        std::vector <Task> best = tmp;
            
        std::random_device rd; 
        std::mt19937 g(rd());
        
        
        for(int i = 0; i < 100000; ++i){
            tmp = best;
             
  
            shuffle(tmp.begin(), tmp.end(), g); 
            
            if(penalty(best) > penalty(tmp)){
                best = tmp;
                std::cout << penalty(tmp) << "\n";
            }
        }
        
        return best;
    }

    Resolver(){
        files = {"./data.10", "./data.11", "./data.12", "./data.13", "./data.14", "./data.15", "./data.16", "./data.17", "./data.18", "./data.19" , "./data.20"};
    }

};



int main(){
    
    auto start = std::chrono::high_resolution_clock::now();
    
    Resolver r1;

    for(std::string x : r1.files){
        r1.load_data(x);
        std::vector<Task> tasks = r1.random(r1.tasks);

        r1.print_tasks(tasks);

        std::cout << "Penalty: " << r1.penalty(tasks) << "\n\n\n";
    }

    r1.load_data("./data.10");
    std::cout << "Penalty: " << r1.penalty2(r1.tasks, "1100111001") << "\n\n\n";

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "Execution time(microseconds): " << duration.count() << "\n";
}