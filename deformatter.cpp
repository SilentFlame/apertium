#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <tidybuffio.h>
#include <tidy.h>
#include <errno.h>
	
#include <cstdio>
#include <cstdlib>
#include <string>
#include <getopt.h>


#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

stack<xmlNode *> tagStack;

// a vector of all the inline tags
char const *str[] = {"a", "b", "u", "big", "i", "small", "tt", "abbr", "acronym", "cite", "code", "dfn","em", "kbd", "strong", "samp", "time", "var", "bdo", "br", "img", "map", "object", "q", "script", "span", "sub", "sup", "button", "input", "label", "select", "textarea"};
vector<string> tagVector(str, str + sizeof(str)/sizeof(str[0]));

int isInlineTags(string tag){
	
	//if it's an Inline tag
	if( find(tagVector.begin(), tagVector.end(), tag) != tagVector.end() ){
	 	return 1;
	}	
	else{
		return 0;
	}
}

string validateXml(const std::string &html)
{
    // init a tidy document
    TidyDoc tidy_doc=tidyCreate();
    TidyBuffer output_buffer= {0};
 
    // configure tidy
    // the flags tell tidy to output xml and disable warnings
    bool config_success=tidyOptSetBool(tidy_doc,TidyXmlOut,yes)
                        && tidyOptSetBool(tidy_doc,TidyQuiet,yes)
                        && tidyOptSetBool(tidy_doc,TidyNumEntities,yes)
                        && tidyOptSetBool(tidy_doc,TidyShowWarnings,no);
 
    int tidy_rescode=-1;
 
    // parse input
    if(config_success)
        tidy_rescode=tidyParseString(tidy_doc,html.c_str());
 
    // process html
    if(tidy_rescode>=0)
        tidy_rescode=tidySaveBuffer(tidy_doc,&output_buffer);
 
    if(tidy_rescode<0)
        throw("tidy has a error: "+tidy_rescode);
 
    std::string result=(char *)output_buffer.bp;
    tidyBufFree(&output_buffer);
    tidyRelease(tidy_doc);
 
    return result;
}

void usage(char *progname)
{
  wcerr << L"USAGE: " << basename(progname) << L" [input_file [output_file]]" << endl;
  exit(EXIT_FAILURE);
}


void printOut(ostream& outfile, bool noNewline){
	
	if(noNewline){
		// only when we have elements in the stack;	
		if(tagStack.size() > 0){
			outfile << "[{";
		}
		xmlChar *attr_val;

		// this always to maintain the syntax in the output too.
		for(stack<xmlNode *> temp_stack = tagStack; !temp_stack.empty() ; temp_stack.pop()){
				xmlNode *curr_node = temp_stack.top();
				outfile << "<" << curr_node->name;
			
			for(xmlAttr *curr_attr = curr_node->properties; curr_attr; curr_attr = curr_attr->next){
				outfile << " " << curr_attr->name << " = ";
				attr_val = xmlNodeGetContent((xmlNode*)curr_attr);
				outfile << "\"" << attr_val << "\"";
			}
			
			outfile << ">";
		}
		
		if(tagStack.size() > 0){
			outfile << "}]";
		}
	}	
}

void convertDeshtml(xmlNode *node, ostream& outfile){
	// to have a look that we don't print the stack again after a closing tag.
	bool noNewline = true;

	for(xmlNode *curr_node = node ; curr_node ; curr_node = curr_node->next){
		
		if(curr_node->type == XML_ELEMENT_NODE){
			noNewline = true;
			if(isInlineTags((char*)curr_node->name)){
				tagStack.push(curr_node);
			}
			else{
				xmlChar *attr_val;
				
				outfile << "[<" << curr_node->name;

				//to print the things inside the tags. i.e id' clas and all
				for(xmlAttr *curr_attr = curr_node->properties; curr_attr; curr_attr = curr_attr->next){
					outfile << " " << curr_attr->name << " = ";
					attr_val = xmlNodeGetContent((xmlNode*)curr_attr);
					outfile << "\"" << attr_val << "\"";
				}
				outfile << ">]";
			}

			convertDeshtml(curr_node->children, outfile);
			// to pop when we get the closing brackets of inline tags
			if(isInlineTags((char*)curr_node->name)){
				tagStack.pop();
				outfile << "[]";
			}
			else{
				outfile << "[]";
				outfile << "[</" << curr_node->name << ">]";
			}
		}
		else{
			// checking cases of nested inline tags.
			string str = (char*)curr_node->content;
			if(str[0]=='\n'){
				noNewline=false;
			}
			printOut(outfile, noNewline);  
			outfile << (char*)curr_node->content;
		}
	}
}

int main(int argc, char **argv){
	ofstream outfile ("temp.txt");
	
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	LIBXML_TEST_VERSION

  if((argc-optind+1) > 3)
  {
    usage(argv[0]);
  }

  FILE *input, *output;
  
  if((argc-optind+1) == 1)
  {
    input = stdin;
    output = stdout;
    
    char mystring [100];
    FILE *fp;
    fp = fopen("input.xml","w+b");
    while(fgets(mystring,100,input) != NULL)
    {
    	fputs(mystring,fp);
    }
    fclose(fp);
 
 	ifstream in("input.xml");
	string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    s = validateXml(s);
	
    fp = fopen("final_input.xml","w+b");
    fputs(s.c_str(),fp);
	fclose(fp);
    
    doc = xmlReadFile("final_input.xml",NULL,0);
	if (doc == NULL) {
		printf("error: could not parse file %s\n", argv[1]);
	}
  }
  else if ((argc-optind+1) == 2)
  {
    input = fopen(argv[argc-1], "r");
    if(!input)
    {
      usage(argv[0]);
    }
    output = stdout;

    char mystring [100];
    FILE *fp;
    fp = fopen("input.xml","w+b");
    while(fgets(mystring,100,input) != NULL)
    {
    	fputs(mystring,fp);
    }
    fclose(fp);
    

    ifstream in("input.xml");
	string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    s = validateXml(s);
	
    fp = fopen("final_input.xml","w+b");
    fputs(s.c_str(),fp);
	fclose(fp);
    
    doc = xmlReadFile("final_input.xml",NULL,0);
	if (doc == NULL) {
		printf("error: could not parse file %s\n", argv[1]);
	}
  }
  else
  {
    input = fopen(argv[argc-2], "r");
    output = fopen(argv[argc-1], "w");

    if(!input || !output)
    {
      usage(argv[0]);
    }
        char mystring [100];
    FILE *fp;
    fp = fopen("input.xml","w+b");
    while(fgets(mystring,100,input) != NULL)
    {
    	fputs(mystring,fp);
    }
    fclose(fp);
    
    ifstream in("input.xml");
	string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    s = validateXml(s);
	
    fp = fopen("final_input.xml","w+b");
    fputs(s.c_str(),fp);
	fclose(fp);

    doc = xmlReadFile("final_input.xml",NULL,0);
	if (doc == NULL) {
		printf("error: could not parse file %s\n", argv[1]);
	}
  }

#ifdef _MSC_VER
    _setmode(_fileno(input), _O_U8TEXT);
    _setmode(_fileno(output), _O_U8TEXT);
#endif

	LIBXML_TEST_VERSION
	root_element = xmlDocGetRootElement(doc);
	convertDeshtml(root_element,outfile);
	outfile<<endl;

	xmlFreeDoc(doc);

	xmlCleanupParser();

	ifstream in("temp.txt");
	string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	fputs(s.c_str(),output);
	fclose(output);
	
	return 0;

}
