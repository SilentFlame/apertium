#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <libxml/parser.h>
#include <libxml/tree.h>
using namespace std;

stack<string> tagStack;

// a vector of all the inline tags
char const *str[] = {"a", "b", "big", "i", "small", "tt", "abbr", "acronym", "cite", "code", "dfn","em", "kbd", "strong", "samp", "time", "var", "bdo", "br", "img", "map", "object", "q", "script", "span", "sub", "sup", "button", "input", "label", "select", "textarea"};
vector<string> tagVector(str, str + sizeof(str)/sizeof(str[0]));

int isInlineTags(string tag){
	
	// //if it's an Inline tag
	if( find(tagVector.begin(), tagVector.end(), tag) != tagVector.end() ){
	 	return 1;
	}	
	else{
		return 0;
	}
}

void printOut(){
	
	//only when we have elements in the stack;	
	if(tagStack.size() > 0){
		cout << "[{";
	}
	// this always to maintain the syntax in the output too.
	for(stack<string> temp_stack = tagStack; !temp_stack.empty() ; temp_stack.pop()){
		cout << "<" << temp_stack.top() << ">";
	}
	if(tagStack.size() > 0){
		cout << "}]";
	}
}

void convertDeshtml(xmlNode *node){

	for(xmlNode *curr_node = node ; curr_node ; curr_node = curr_node->next){
		
		if(curr_node->type == XML_ELEMENT_NODE){

			if(isInlineTags((char*)curr_node->name)){
				tagStack.push((char*)curr_node->name);
			}
			else{
				xmlChar *attr_val;
				
				cout << "[<" << curr_node->name;

				//to print the things inside the tags. i.e id' clas and all
				for(xmlAttr *curr_attr = curr_node->properties; curr_attr; curr_attr = curr_attr->next){
					cout << " " << curr_attr->name << " = ";
					attr_val = xmlNodeGetContent((xmlNode*)curr_attr);
					cout << "\"" << attr_val << "\"";
				}
				cout << ">]";
			}

			convertDeshtml(curr_node->children);
			// to pop when we git the closing brackets of inline tags
			if(isInlineTags((char*)curr_node->name)){
				tagStack.pop();
			}
			else{
				cout << "[</" << curr_node->name << ">]";
			}
		}
		else{
			printOut();
			cout << (char*)curr_node->content;
		}
	}
}

int main(int argc, char **argv){

	//only two arguments during compiling
	if(argc!=2){
		return(1);
	}
	/*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
	LIBXML_TEST_VERSION

	// parse the file and get the DOM of it.
	xmlDoc *file = xmlReadFile(argv[1], NULL, 0);

	if(file==NULL){
		cout << "error: could not parse file: " << argv[1] << endl; 
	}

	
	// get the root of the DOM
	xmlNode *root = xmlDocGetRootElement(file);
	convertDeshtml(root);
	
	cout << endl; 

	//freeing the file
	xmlFreeDoc(file);

	// freeing the global variables allocated by the parser.
	xmlCleanupParser();
	return 0;
}