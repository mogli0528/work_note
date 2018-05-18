#include<map>  
#include<string>  
#include<iostream>  
using namespace std;  
  
typedef pair<string, int> PAIR;  
  
ostream& operator<<(ostream& out, const PAIR& p) {  
  return out << p.first << "\t" << p.second;  
}  
  
int main() {  
  map<string, int, greater<string> > name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }  
  return 0;  
}  