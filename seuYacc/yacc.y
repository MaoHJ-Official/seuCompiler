%token IDENTIFIER
%token STRING CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token BOOL LE_OP NUMBER GE_OP EQ_OP NE_OP
%token ';' '+' '-' '*' '/' '{' '}' '(' ')' '>' '<' '='

%start translation_unit
%%


type_specifier
	: VOID
	| STRING
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| BOOL
	;



translation_unit
	: type_specifier IDENTIFIER '(' ')' '{' statements '}'
	;

statements
	: statements statement
	| statement
	;

statement
	: if_s
	| int_s
	| while_s
	| for_s
	;

int_s
	: type_specifier assignment ';'
	| type_specifier IDENTIFIER ';'
	;

assignment
	: IDENTIFIER '=' expression
	;

expression
	: term 
	| term '+' term
	| term '-' term
	;

term
	: factor
	| factor '*' factor
	| factor '/' factor
	;

factor
	: IDENTIFIER
	| NUMBER
	| '(' expression ')'
	;

if_s
	: IF '(' expression relationcha expression ')' '{' statements '}'
	;
while_s
	: WHILE '(' expression relationcha expression ')' '{' statements '}'
	;

for_s
	: FOR '(' assignment ';' expression relationcha expression ';' assignment ')' '{' statements '}'
	;

relationcha
	: '>' 
	| '<' 
	| EQ_OP
	| LE_OP
	| GE_OP 
	| NE_OP
	;
%%
