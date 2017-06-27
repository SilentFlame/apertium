#include <sstream>
#include <string> 
#include <iostream> 
#include <fstream>
#include <typeinfo>
#include <vector>
#include <stack>

using namespace std;

string convertRehtml(string str){
	int len = str.length();
	int flag =0;
	string temp_str;
	string finalStr = "";
	int i = 0;
	while(i < len){
		
		if(str[i]=='[' && str[i+1]=='<'){
			i++;
			while(str[i]!='>' && str[i+1]!=']'){
				finalStr += str[i];
				i++;
			}
			finalStr += str[i];
			i += 2;;
		}
		else if(str[i]=='[' && str[i+1]=='{' && str[i+2] =='<'){
			i+=2;
			// cout << "Before While " << endl;
			while(str[i] != ']'){
				// cout << "inside While-1 " << endl;
				temp_str = "";
				// cout << finalStr << endl;
				if(str[i] == '<' && flag == 0){
					finalStr += str[i];
					flag = 1;
					i++;
				}
				else if(str[i] == '>' && str[i+1] == '<'){
					finalStr += str[i];
					i++;
				}
				else if( str[i] == '>' && str[i+1] == '}'){
					finalStr += str[i];
					i+=2;
				}
				else if(flag == 1 ){
					while(str[i] != ' ' && str[i] != '>'){
						temp_str += str[i];
						finalStr += str[i];
						i++;
					}
					finalStr += str[i];
					flag = 0;
					i++;
					if(str[i] == '}'){
						i += 2;
					}
					// cout << temp_str << endl;
					// rehtmlTagStack.push(temp_str);
				}
				else{
					finalStr += str[i];
					// if(str[i+1]){
						i++;
					// }
				}
			}
			i++;
		}
		else if(str[i]=='\n'){
			finalStr += str[i];
			// finalStr += rehtmlTagStack.top();
			// rehtmlTagStack.pop();
			i++;
		}
		else{
			finalStr += str[i];
				i++;
		}
	}
return finalStr;
}


int main() {

 	ifstream myfile("test.txt");
 	string line;

	while(getline(myfile, line)){ 	
		cout << convertRehtml(line) << endl;// Process line
	}	

	return 0;
 }