#include <sstream>
#include <string> 
#include <iostream> 
#include <fstream>
#include <stack>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>


using namespace std;

stack<string> tagStack;

string convertRehtml(string str){
	int len = str.length();
	int flag = 0, start = 0;
	string temp_str;
	string finalStr = "";
	int i = 0;
	while(i < len){
		if(i == 0 && str[i] == '[' && str[i+1] == ']'){
			i += 2;
		}
		else if( i != 0 && str[i] == '[' && str[i+1] == ']'){
			
			while(tagStack.size()){
				finalStr += "</";
				finalStr += tagStack.top();
				finalStr += ">";
				tagStack.pop();
			}
			i += 2;
		}
		else if(str[i]=='[' && str[i+1]=='<'){

			while(tagStack.size()){
				finalStr += "</" + tagStack.top() + ">";
				tagStack.pop();
			}

			i++;

			while(str[i]!='>' && str[i+1]!=']'){
				finalStr += str[i];
				i++;
			}
			finalStr += str[i];
			i += 2;;
		}
		else if(str[i]=='[' && str[i+1]=='{' && str[i+2] =='<'){
			
			while(tagStack.size()){
				finalStr += "</" + tagStack.top() + ">";
				tagStack.pop();
			}

			i+=2;
			while(str[i] != ']'){
				temp_str = "";
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
					tagStack.push(temp_str);
					finalStr += str[i];
					flag = 0;
					i++;
					if(str[i] == '}'){
						i += 2;
						break;
					}
				}
				else{
					finalStr += str[i];
						i++;
				}
			}
			
		}
		else if(str[i]=='\n'){
			finalStr += str[i];
			i++;
		}
		else{
			finalStr += str[i];
			i++;
		}
	}
	while(tagStack.size()){
		finalStr += "</" + tagStack.top() + ">";
		tagStack.pop();
	}
	return finalStr;
}


int main(int argc, char **argv) {
	
	string s((std::istreambuf_iterator<char>(cin)),
	  				std::istreambuf_iterator<char>());

	cout << convertRehtml(s);

	return 0;

}