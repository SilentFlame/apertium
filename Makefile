all : deshtml rehtml

deshtml : deformatter.cpp
	g++ -o deshtml deformatter.cpp -I/usr/include/libxml2 -lxml2 -I/usr/include/tidy -ltidy

rehtml : reformatter.cpp
	g++ -o rehtml reformatter.cpp

clean : 
	rm -f deshtml rehtml
