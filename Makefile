all : deshtml rehtml

deshtml : deformatter.cpp
	g++ -o deshtml deformatter.cpp -I/usr/include/libxml2 -lxml2

rehtml : reformatter.cpp
	g++ -o rehtml reformatter.cpp

clean : 
	rm -f deshtml rehtml
