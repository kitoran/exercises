
#include <unordered_set>
#include <stack>
#include <map>
#include <iostream>
#include <algorithm>
class Solution {
public:
    std::map<char,char> ps = {{'(',')'}, {'[',']'},{'{','}'}};
    bool isValid(string s) {
        std::stack<char> stack;

        for(char c: s) {
            if(ps.contains(c)) stack.push(c);
            else if(!stack.empty() && c == ps[stack.top()]) stack.pop();
            else return false;
        }
        return stack.empty();
    }
};
int main()
{
    std::string j;
    std::string s;
    std::getline(std::cin, j);
    std::getline(std::cin, s);
    std::unordered_set jems(j.begin(), j.end());

    int res = 0;
    for(char stone : s) {

        if(jems.contains(stone)) {
            res++;
        }
    }
    std::cout << res;
}

//#include <vector>
//#include <queue>
//#include <iostream>

//int main()
//{
//    int count;
//    std::cin >> count;
//    struct Town {
//        int x;
//        int y;
//        std::vector<Town*> neighbours;
//        bool visited = false;
//        int distance = -1;
//    };
//    std::vector<Town> towns;
//    for(int i = 0; i < count; i++) {
//        Town t;
//        std::cin >> t.x >> t.y;
//        towns.push_back(t);
//    }
//    int maxDist;
//    std::cin >> maxDist;
//    int start, finish;
//    std::cin >> start >> finish;
//    start--; finish--;
//    for(int i = 0; i < count; i++) {
//        for(int j = i+1; j < count; j++) {
//            int dist = abs(towns[i].x-towns[j].x)+abs(towns[i].y-towns[j].y);
//            if(dist <= maxDist) {
//                towns[i].neighbours.push_back(&towns[j]);
//                towns[j].neighbours.push_back(&towns[i]);
//            }
//        }
//    }
////    bool found = false;
//    towns[start].distance = 0;
//    std::queue<Town*> nextTowns;
//    nextTowns.push(&towns[start]);
//    while(!nextTowns.empty()) {
//        Town* currentTown = nextTowns.front();
//        nextTowns.pop();
//        currentTown->visited = true;
//        for(Town* neighbour : currentTown->neighbours) {
//            if(neighbour->distance > currentTown->distance+1
//                    || neighbour->distance == -1) {
//                neighbour->distance = currentTown->distance+1;
//                if(!neighbour->visited) nextTowns.push(neighbour);
//                if(neighbour == &towns[finish]) {
////                    found = true;
//                    goto cplusplusdoesnthavenamedloops;
//                }
//            }
//        }
//    }
//    cplusplusdoesnthavenamedloops:
//    std::cout << towns[finish].distance;
//}
