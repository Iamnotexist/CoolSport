#ifndef PROVOD2_H
#define PROVOD2_H

#include <map>
#include <string>
#include <ostream>
#include <fstream>
#include <vector>

namespace _provod2_sys {

	struct node {
		std::string _name = ""; //имя вершины дерева
		node* _par = nullptr; //указатель на родителя
		std::map<std::string, node*> _vp; //vertex pointers, указатели на вершины дерева

		node (std::string, node*); //конструктор, имя вершины, родитель

		node& operator[] (const std::string); //переопределяю оператор доступа и записи

		std::vector<std::string> getVertex (); //возвращает вектор из всех детей данной вершины 
		deleteVertex (std::string); //удаляет сына вершины 
	};


	void write (std::ofstream&, node*); //переопределяю ф-цию для записи

}

class provod2 {
private:
	_provod2_sys::node *_tree = nullptr; //указатель на мнимый корень
public:

	std::ifstream _in; //для чтения информации
	std::ofstream _out; //для записи информации
	std::string _file; //файл для вывода/записи

	_provod2_sys::node& operator[] (const std::string); //переопределяю оператор доступа и записи

	std::vector<std::string> getVertex (); //возвращает вектор из всех детей мнимого корня
	deleteVertex (std::string); //удаляет сына мнимого корня

	//конструктор по пути до файла или его названию
	provod2 (std::string);

	//записываю в файл новые данные
	write ();
};

#endif