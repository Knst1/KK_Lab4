#include "parser.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <stack>

using namespace std;

Node::Node(int num, string name, bool terminal) : num(num), name(name), terminal(terminal)
{ }

void Node::set_childcnt(int cnt)
{
	children = vector<Node*>(cnt);
}

void Node::add_child(Node *child, int num)
{
	children[num] = child;
}

int Node::get_num()
{
	return num;
}

string Node::get_name()
{
	return name;
}

bool Node::get_terminal()
{
	return terminal;
}

vector<Node*> Node::get_children()
{
	return children;
}

Node::~Node()
{
	children.clear();
}

pair<bool, string> Parser::next_terminal()
{
	if (text.size() == 0)
		return pair<bool, string>(false, "");
	string ret;
	for (string s : terminals)
	{
		if (s.size() > text.size())
			continue;
		else if (s == text.substr(0, s.size()))
		{
			text = text.substr(s.size());
			prnts = type_of_token[s] == RP;
			oper = (type_of_token[s] == LOGIC || type_of_token[s] == SUM || type_of_token[s] == MULT);
			return pair<bool, string>(true, s);
		}
	}
	ret.push_back(text[0]);
	text = text.substr(1);
	return pair<bool, string>(false, ret);
}

void Parser::gen_postf(string sym)
{
	if (type_of_token[sym] != LP && type_of_token[sym] != RP)
		postf.push_back(sym);
}

void Parser::print_sate(bool reduct)
{
	cout << endl << ((reduct) ? "r" : "s") << " [";
	for (string x : st)
		cout << x << " ";
	cout << ",\t" <<current.second << text << "] |-";
}

bool Parser::parse()
{
	string tmp = text;
	bool ans = true;
	st.push_back(marker);
	current = next_terminal();
	print_sate(false);
	while (current.second != "")
	{
		if (!current.first)
		{
			cout << endl << "ОШИБКА: Обнаружен недопустимый символ " << "\"" << current.second << "\"." << endl;
			ans = false;
			current = next_terminal();
			continue;
		}
		switch (matrix[type_of_token[st[st.size()-1]]][type_of_token[current.second]])
		{
		case '<':
		case '=': // Перенос
			if (prnts && type_of_token[st[st.size() - 1]] == LP)
			{
				cout << endl << "ОШИБКА: Между открывающей и закрывающей скобками отсутствует арифметическое выражение" << endl;
				ans = false;
			}
			if (oper && (type_of_token[st[st.size() - 1]] == LOGIC || type_of_token[st[st.size() - 1]] == SUM || type_of_token[st[st.size() - 1]] == MULT))
			{
				cout << endl << "ОШИБКА: Бинарные операторы записаны подряд" << endl;
				ans = false;
			}
			st.push_back(current.second);
			current = next_terminal();
			print_sate(false);
			break;
		case '>': // Свёртка
			prnts = false;
			if (oper && (type_of_token[st[st.size() - 1]] == LOGIC || type_of_token[st[st.size() - 1]] == SUM || type_of_token[st[st.size() - 1]] == MULT))
			{
				cout << endl << "ОШИБКА: Бинарные операторы записаны подряд" << endl;
				ans = false;
			}
			do {
				if (!(type_of_token[st[st.size() - 1]] == LOGIC || type_of_token[st[st.size() - 1]] == SUM || type_of_token[st[st.size() - 1]] == MULT))
					oper = false;
				postf.push_back(st[st.size() - 1]);
				st.pop_back();
				print_sate(true);
			} while (postf.size()>0 && matrix[type_of_token[st[st.size() - 1]]][type_of_token[postf[postf.size() - 1]]] != '<');
			break;
		case '0':
			if (tmp.size() == 1) {
				cout << endl << "ОШИБКА: Строка пуста" << endl;
				ans = false;
			}
			else
				cout << endl << "Допуск" << endl;
			current = next_terminal();
			
			break;
		case '1':
			cout << endl << "ОШИБКА: Между факторами отсутствует оператор" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '2':
			cout << endl << "ОШИБКА: В выражении больше одного логического оператора" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '3':
			cout << endl << "ОШИБКА: После логического оператора недостаёт открывающей скобки" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '4':
			cout << endl << "ОШИБКА: После арифметического выражения перед открывающей скобкой отсутствует бинарный оператор" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '5':
			cout << endl << "ОШИБКА: После закрывающей скобки перед арифметическим выражением отсутствует бинарный оператор" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '6':
			cout << endl << "ОШИБКА: Отсутствует закрывающая скобка" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '7':
			cout << endl << "ОШИБКА: Между закрывающей и открывающей скобками отсутствует бинарный оператор" << endl;
			current = next_terminal();
			ans = false;
			break;
		case '8':
			cout << endl << "ОШИБКА: Отсутствует открывающая" << endl;
			current = next_terminal();
			ans = false;
			break;
		default:
			cout << endl << "ОШИБКА: Тип ошибки неизвестен" << endl;
			current = next_terminal();
			ans = false;
			break;
		}
	}
	text = tmp;
	return ans;
}

Parser::Parser(string txt)
{
	text = txt + marker;
	prnts = false;
	oper = false;
}

void Parser::make_tree()
{
	const string E = "E";
	int num = 0;
	stack<Node*> roots;
	tree.push_back(new Node(num++, E, false));
	roots.push(tree[0]);
	for (int i = postf.size() - 1; i > -1; i--)
	{
		if (type_of_token[postf[i]] == LP) // (E)
		{
			roots.top()->set_childcnt(3);
			tree.push_back(new Node(num++, "(", true));
			roots.top()->add_child(tree[tree.size() - 1], 0);
			tree.push_back(new Node(num++, E, false));
			roots.top()->add_child(tree[tree.size() - 1], 1);
			tree.push_back(new Node(num++, ")", true));
			roots.top()->add_child(tree[tree.size() - 1], 2);
			i--;
			roots.pop();
			roots.push(tree[tree.size() - 2]);
		}
		else if (type_of_token[postf[i]] == LOGIC || type_of_token[postf[i]] == SUM || type_of_token[postf[i]] == MULT) // E<operator>E
		{
			roots.top()->set_childcnt(3);
			tree.push_back(new Node(num++, E, false));
			roots.top()->add_child(tree[tree.size() - 1], 0);
			tree.push_back(new Node(num++, postf[i], true));
			roots.top()->add_child(tree[tree.size() - 1], 1);
			tree.push_back(new Node(num++, E, false));
			roots.top()->add_child(tree[tree.size() - 1], 2);
			roots.pop();
			roots.push(tree[tree.size() - 3]);
			roots.push(tree[tree.size() - 1]);
		}
		else if (type_of_token[postf[i]] == ATOM) // <atom>
		{
			roots.top()->set_childcnt(1);
			tree.push_back(new Node(num++, postf[i], true));
			roots.top()->add_child(tree[tree.size() - 1], 0);
			roots.pop();
		}
	}
}

void Parser::make_graph()
{
	ofstream tree_dot("tree.dot");
	if (!tree_dot.is_open())
		throw runtime_error("Не удалось открыть файл tree.dot для записи\n");
	tree_dot << "digraph G{" << endl << "node[shape=rectangle style=filled fillcolor=white fontsize=12];" << endl;
	for (Node* node : tree)
	{
		tree_dot << node->get_num() << "[label=\"" << node->get_name() << "\"";
		if (node->get_terminal())
			tree_dot << " fillcolor=lightgrey shape=ellipse";
		tree_dot << "];" << endl;
		for (Node* child : node->get_children())
			tree_dot << node->get_num() << "->" << child->get_num() << ";" << endl;
	}
	tree_dot << "}" << endl;
	tree_dot.close();
	system("dot -Tsvg tree.dot -o tree.svg");
}

void Parser::handle()
{
	bool ok = parse();
	cout << endl;
	if (ok)
	{
		cout << "Постфиксная запись:" << endl;
		for (string x : postf)
			if (type_of_token[x] != LP && type_of_token[x] != RP)
			cout << x << " ";
		cout << endl;
		make_tree();
		make_graph();
	}
}
