# D-
Final project for Compilers course

To run

yacc -d syn.y && lex lex.l && g++ -g lex.yy.c y.tab.c -o d++
./ d++ "file_to_compile"

#Mac stuff

mv /usr/local/Cellar/bison /usr/local/bin

export PATH=/usr/local/opt/flex/bin:/usr/local/opt/bison/bin:$PATH
