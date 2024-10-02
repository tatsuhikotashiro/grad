#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

// グラフを表すデータ構造
using Graph = vector<vector<int>>;

class seat
    {
    public:
        bool isTaken(){
            return taken;
        };
        bool takeSeat(){
            this->taken = true;
        };
        bool untakeSeat(){
            this->taken = false;
        };
    private:
        bool taken = false; // if the seat has already be taken, this variable is true.
        set<seat> adjacent;
        bool searched = false;
    };

bool canSitOn(int PeopleNum, std::vector<seat> seats){
    while(seats(i)->isTaken()){
        
    }
}


int main() {
	std::vector<seat> customerSeats(12);

	
}