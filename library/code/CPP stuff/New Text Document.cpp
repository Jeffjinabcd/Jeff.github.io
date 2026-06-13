#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>	
#include <utility>
#include <cstring>
#include <vector>

#define RAND_HALF (RAND_MAX)/2

template<class Key, class Val>
class skiplist {
private:
	class _node {
	private:
		std::pair<const Key, Val> _data;
		_node **_forward;
		int _height;
		friend class skiplist;
	public:
		_node(int height, const Key &key = Key(), const Val &val = Val());
		~_node();
	};

private:
	int _level;
	int _height_limit;
	_node *_head;

public:
	skiplist(int height_limit);
	~skiplist();
	int insert(const Key &key, const Val &val);
	bool search(const Key &key);
	bool remove(const Key &key);
	
private:
	int randomLevel();

public:
	void display();
};

template<class Key, class Val>
skiplist<Key, Val>::_node::_node(int height, const Key &key, const Val &val):_data(std::make_pair(key,val)){
	this->_height = height;
	_forward = new _node*[height];
};

template<class Key, class Val>
skiplist<Key, Val>::_node::~_node(){
	delete []_forward;
};

template<class Key, class Val>
skiplist<Key, Val>::skiplist(int height_limit){
	
};

template<class Key, class Val>
skiplist<Key, Val>::~skiplist(){
	
};

template<class Key, class Val>
int skiplist<Key, Val>::insert(const Key &key, const Val &val) {
	
};

template<class Key, class Val>
bool skiplist<Key, Val>::search(const Key &key) {

};

template<class Key, class Val>
bool skiplist<Key, Val>::remove(const Key &key) {

};

template<class Key, class Val>
int skiplist<Key, Val>::randomLevel() {
	int new_level = 1;
	while (new_level < _height_limit && rand() < RAND_HALF) ++new_level;
	return new_level;
};

template<class Key, class Val>
void skiplist<Key, Val>::display() {
	std::cout << "********* Skiplist ********\n";

	for (int i = _level-1; i >= 0; --i) {
		_node *base = _head->_forward[0];
		_node *curr = _head->_forward[i];
		std::cout << "Level " << i+1 << ": ";
		while (curr != NULL) {
			while (curr != base && base != NULL) {
				std::cout << std::setw(3) << " " << std::setw(1) << " " << std::setw(2) << " " << std::setw(1) << " ";
				base = base->_forward[0];
			}
			if (curr == base) {
				std::cout << std::setw(3) << curr->_data.first << std::setw(1) << ":" << std::setw(2) << curr->_data.second << std::setw(1) << " ";
				base = base->_forward[0];
			}
			curr = curr->_forward[i];
		}
		std::cout << "\n" << '\n';
	}
};

void get_input(std::vector<std::string> *tokens, char *inputs) {
	std::string str = inputs;
	std::string token;
	size_t pos;

	while (str.size() != 0) {
		pos = str.find(" ");
		if (pos != std::string::npos) {
			token = str.substr(0, pos);
			str = str.substr(pos + 1);
		}
		else {
			token = str;
			str = "";
		}
		(*tokens).push_back(token);
	}
}

int main(void)
{
	srand(2);
	char c[20];
	std::cin.getline(c, 20, '\n');
	if (c[0] == '0') {
		skiplist<int, std::string> sl(10);

		while(std::cin.getline(c, 20, '\n')) {
			std::vector<std::string> tokens;
			get_input(&tokens, c);
			if (tokens[0] == "+") {
				sl.insert(atoi(tokens[1].c_str()), tokens[2]);
				std::cout << "insert: " << tokens[1] << ": " << tokens[2] << '\n';
			}
			else if (tokens[0] == "-") {
				sl.remove(atoi(tokens[1].c_str()));
				std::cout << "remove: " << tokens[1] << '\n';
			}
			else if (tokens[0] == "@") {
				std::cout << "search: " << tokens[1] << " " << std::boolalpha << sl.search(atoi(tokens[1].c_str())) << '\n';
			}
			else {
				std::cout << "display:" << '\n';
				sl.display();
			}
		}
	}
	else if (c[0] == '1') {
		skiplist<float, int> sl(10);

		while(std::cin.getline(c, 20, '\n')) {
			std::vector<std::string> tokens;
			get_input(&tokens, c);
			if (tokens[0] == "+") {
				sl.insert(atof(tokens[1].c_str()), atoi(tokens[2].c_str()));
				std::cout << "insert: " << tokens[1] << ": " << tokens[2] << '\n';
			}
			else if (tokens[0] == "-") {
				sl.remove(atof(tokens[1].c_str()));
				std::cout << "remove: " << tokens[1] << '\n';
			}
			else if (tokens[0] == "@") {
				std::cout << "search: " << tokens[1] << " " << std::boolalpha << sl.search(atof(tokens[1].c_str())) << '\n';
			}
			else {
				std::cout << "display:" << '\n';
				sl.display();
			}
		}
	}
	return 0;
}
