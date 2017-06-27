## Coding challenge scripts ##

## Prereuisites ##
- download/install 'libxml' library.

### Challenge-1 ###

##### To Compile #####

- g++ -o challenge1 `xml2-config --cflags` challenge-1.cpp `xml2-config --libs`
- ./challenge1 input.html

Output would be:
- `[{<i>}]foo [{<b><i>}]bar`

### Challenge-2 ###

##### To Compile and run #####
- g++ -o challenge2 `xml2-config --cflags` challenge-2.cpp `xml2-config --libs`
- ./challenge2 input.html

Input1:
- `<p>foo <b>bar fie <i>baz</i> fum</b> fiz</p>` <!--taken from https://github.com/unhammer/apertium/blob/3f80f35503656f70db2386c0b5316d502282a689/blank_notes.org -->

Output1:
- `[<p>]foo [{<b>}]bar fie [{<i><b>}]baz[{<b>}] fum fiz[</p>]`

Input2:
- ```<div id="id1">
  <p class="class1" id="id2">
    <i>Hello Sir,</i>
    <u style="italic">would you 
    <b>like to play</b>
    <em>football?</em></u>
  </p>
</div>```

Updated Output2:
- ```[<div id = "id1">]
  [<p class = "class1" id = "id2">]
    [{<i>}]Hello Sir,
    [{<u style = "italic">}]would you 
    [{<b><u style = "italic">}]like to play[{<u style = "italic">}]
    [{<em><u style = "italic">}]football?
  [</p>]
[</div>]```

- have some problem with dealing `</b>` in the input. 

### Deformatter ###

##### Compile and run with #####
- $> g++ deformatter.cpp -I/usr/include/libxml2 -lxml2
- $> ./a.out input.html

#### Input ####
```
<div id="id1">
  <p class="class1" id="id2">
    <i>Hello Sir,</i>
    <u style="italic">would you
    <b>like to play</b>
    <em>football?</em></u>
  </p>
</div>
```

#### Output ####
```
<div id = "id1">]
  [<p class = "class1" id = "id2">]
    [{<i>}]Hello Sir,[]
    [{<u style = "italic">}]would you
    [{<b><u style = "italic">}]like to play[]
    [{<em><u style = "italic">}]football?[][]
  [][</p>]
[][</div>]

```

------

### Reformatter ###

##### Compile and run with(inside the reformatter-trying repo) #####
- $> g++ --std=c++11 reform.cpp
- $> ./a.out test.txt

#### Input-1 ####
```
[<div id = "id1">]
  [<p class = "class1" id = "id2">]
    [{<i>}]Hello Sir,[]
    [{<u style = "italic">}]would you
    [{<b><u style = "italic">}]like to play[]
    [{<em><u style = "italic">}]football?[][]
  [][</p>]
[][</div>]

```

#### Output-2 ####
```
<div id = "id1">
  <p class = "class1" id = "id2">
    <i>Hello Sir,</i>
    <u style = "italic">]would you</u>
    <b><u style = "italic">]like to play</u></b>
    <em><u style = "italic">]football?</u></em>
  </p>
</div>
```

#### Input-2 ####
```
[{<i>}]foo [{<b><i>}]bar[][]
```

#### Output-2 ####
```
<i>foo </i><b><i>bar</i></b>
```

-------