#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>

enum Terminal {
	ATOM, SUM, MULT, LOGIC, LP, RP, END
};

class Node {
	int num;
	std::string name;
	bool terminal;
	std::vector<Node*> children;
public:
	void set_childcnt(int cnt);
	Node(int num, std::string name, bool terminal);
	void add_child(Node *child, int num);
	int get_num();
	std::string get_name();
	bool get_terminal();
	std::vector<Node*> get_children();
	~Node();
};

class Parser {
	// Матрица отношений операторного предшествования
	const std::vector<std::vector<char>> matrix = std::vector<std::vector<char>>({
		//	   |					   atm  sum  mul  log  lp   rp   end
		// ----+----------------------------------------------------------
		/* atm | */ std::vector<char>({'1', '>', '>', '>', '4', '>', '>'}),
		/* sum | */ std::vector<char>({'<', '>', '<', '>', '<', '>', '>'}),
		/* mul | */ std::vector<char>({'<', '>', '>', '>', '<', '>', '>'}),
		/* log | */ std::vector<char>({'<', '<', '<', '2', '<', '3', '>'}),
		/* lp  | */ std::vector<char>({'<', '<', '<', '<', '<', '=', '6'}),
		/* rp  | */ std::vector<char>({'5', '>', '>', '>', '7', '>', '>'}),
		/* end | */ std::vector<char>({'<', '<', '<', '<', '<', '8', '0'}) });
	const std::string marker = "$";
	std::map<std::string, Terminal> type_of_token = std::map<std::string, Terminal>({
			std::pair<std::string, Terminal>("identifier", ATOM),
			std::pair<std::string, Terminal>("const", ATOM),
			std::pair<std::string, Terminal>("<=", LOGIC),
			std::pair<std::string, Terminal>("<>", LOGIC),
			std::pair<std::string, Terminal>("<", LOGIC),
			std::pair<std::string, Terminal>(">=", LOGIC),
			std::pair<std::string, Terminal>(">", LOGIC),
			std::pair<std::string, Terminal>("=", LOGIC),
			std::pair<std::string, Terminal>("+", SUM),
			std::pair<std::string, Terminal>("-", SUM),
			std::pair<std::string, Terminal>("*", MULT),
			std::pair<std::string, Terminal>("/", MULT),
			std::pair<std::string, Terminal>("(", LP),
			std::pair<std::string, Terminal>(")", RP),
			std::pair<std::string, Terminal>(marker, END) });
	const std::vector<std::string> terminals = std::vector<std::string>({
		"identifier", "const", "<=", "<>", "<", ">=", ">", "=", "+", "-", "*", "/", "(", ")", marker});
	// Входная строка
	std::string text;
	// Стек разбора
	std::vector<std::string> st;
	bool prnts;
	bool oper;
	std::vector<Node*> tree;
	std::vector<std::string> postf;
	std::pair<bool, std::string> current;
	std::vector<std::string> symbols;
	void make_graph();
	void print_sate(bool reduct);
	void gen_postf(std::string sym);
	std::pair<bool, std::string> next_terminal();
	void make_tree();
public:
	Parser(std::string txt);
	bool parse();
	void handle();
};
