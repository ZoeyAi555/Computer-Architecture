#include <string>

using namespace std;
string s1;
string s2;
string s3 = s1 + s2;

string func(string s1,string s2){
    int index1 = 0;
    int index2 = 0;
    string s3 = "";
    int len = s1.length()+s2.length();
    for(int i = 0;i < len;i++){
        if(index1 < s1.length()){
            s3 += s1[index1];
            index1++;
        }
        if(index2 < s2.length()){
            s3 += s2[index2];
            index2++;
        }
    }//O(len)= O(n) time
    // O(len) = O(n) space
    return s3;
}


'&' '|'

uint32_t func2(uint32_t num,int length,int start){
    uint32_t fliped;
    uint32_t mask = ((1<<(length+1)) -1) << start;
    // 100000 -> 011111
    // 111111111111
    fliped = num^mask;
    return fliped;
}