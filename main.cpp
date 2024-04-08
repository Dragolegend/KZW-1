#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

struct Task{
    int R;
    int P;
    int Q;
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
                    tmp.R = stoi(parts.at(0));
                    tmp.P = stoi(parts.at(1));
                    tmp.Q = stoi(parts.at(2));
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
            std::cout << "R: " << e.R << " P: " << e.P << " Q: " << e.Q <<  " n: " << e.n << "\n";
        }

        std::cout << "\nOrder: ";

        for(Task e : tasks){
            std::cout << e.n << ' ';
        }
        std::cout << "\n\n\n";
    }

    std::vector <Task> resolve_tasks_sortR() {
        std::vector <Task> tmp = tasks;

        std::sort( tmp.begin( ), tmp.end( ), [ ]( const auto& lhs, const auto& rhs )
        {
            return lhs.R < rhs.R;
        });

        return tmp;
    }
    
    std::vector <Task> resolve2() {
        std::vector <Task> tmp = resolve_tasks_sortR();
        std::vector <Task> best = tmp;
            
        std::random_device rd; 
        std::mt19937 g(rd());
        
        
        for(int i = 0; i < 100000; ++i){
            tmp = best;
             
  
            shuffle(tmp.begin(), tmp.end(), g); 
            
            if(Cmax(best) > Cmax(tmp)){
                best = tmp;
                std::cout << Cmax(tmp) << "\n";
            }
        }
        
        return best;
    }

    std::vector <Task> search(std::vector <Task> tmp, std::vector <Task> best, int k_max){
        for(int i = 0; i < tmp.size(); ++i){
                for(int j = 1; j < tmp.size(); ++j){
                    tmp = best;

                    Task temp;
                    for(int k = 0; k <= k_max; ++k){
                        if(j >= k){                
                            temp = tmp.at(i);
                            tmp.at(i) = tmp.at(j - k);
                            tmp.at(j - k) = temp;

                            if(Cmax(best) > Cmax(tmp)){
                                best = tmp;
                                std::cout << Cmax(tmp) << "\n";
                            }
                        }
                    }
                }
            }

            return best;
    }
    
    std::vector <Task> resolve(int index) {
        std::vector <Task> tmp = resolve_tasks_sortR();
        std::vector <Task> best = tmp;
        
        
        if(index == 0){
            best = search(tmp, best, 1);
        }
        
        if(index == 3){
            best = search(tmp, best, 3);
        }
        
        if(index == 2){
            best = search(tmp, best, 5);
            
            best = search(tmp, best, 3);
        
            best = search(tmp, best, 2);
        
            best = search(tmp, best, 0);
        }
        
        std::cout << Cmax(best);
        return best;
    }
    
    std::vector <Task> resolve_tasks_sortRQ() {
        std::vector <Task> tmp = tasks;

        std::sort( tmp.begin( ), tmp.end( ), [ ]( const auto& lhs, const auto& rhs )
        {
            return (lhs.R * lhs.Q) < (rhs.R * rhs.Q);
        });

        return tmp;
    }
    
    int Cmax(std::vector<Task> result){
        int t=0,
            u=0;
        for (int i = 0; i < result.size(); i++){
            t=std::max(t,result.at(i).R)+result.at(i).P;
            u=std::max(u,t+result.at(i).Q);
        }
        return u;
    }

    Resolver(){
        files = {"./data1.txt", "./data2.txt", "./data3.txt", "./data4.txt"};
    }

};



int main(){
    int CmaxSum = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    Resolver r1;
    std::cout << "\n\n\n";
    int index = 0;

    for(std::string path : r1.files){
        r1.load_data(path);
        std::vector<Task> tasks;
        if(path == "./data2.txt"){
            tasks = r1.resolve2();
        }else{
            tasks = r1.resolve(index);
        }
        r1.print_tasks(tasks);
        CmaxSum += r1.Cmax(tasks);
        
        ++index;
    }
    
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "Execution time(microseconds): " << duration.count() << "\n";
    std::cout << "Total Cmax: " << CmaxSum;
}