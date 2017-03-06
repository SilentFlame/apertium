#include <iostream>
#include <stack>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>
using namespace std;

stack<string> tag_stack;

void printOut(){
	
	cout << "[{";

	for(stack<string> temp_stack = tag_stack; !temp_stack.empty() ; temp_stack.pop()){
		cout << "<" << temp_stack.top() << ">";
	}

	cout << "}]";
}

void convertDeshtml(xmlNode *node){

	for(xmlNode *curr_node = node ; curr_node ; curr_node = curr_node->next){
		
		if(curr_node->type == XML_ELEMENT_NODE){
			tag_stack.push((char*)curr_node->name);
			convertDeshtml(curr_node->children);
			tag_stack.pop();
		}
		else{
			printOut();
			cout << (char*)curr_node->content;;
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