#include "provod2.h"

//определяю функцию для мнимого корня
provod2::deleteVertex (std::string __a) {
	delete this->_tree->_vp[__a]; //удаляю вершину
	this->_tree->_vp[__a] = nullptr; //ставлю нулевой указатель на место
}

//определяю функцию для вершины
_provod2_sys::node::deleteVertex (std::string __a) {
	delete this->_vp[__a]; //удаляю вершину
	this->_vp[__a] = nullptr; //ставлю нулевой казатель на место
}

//определяю функцию для мнимого корня
std::vector<std::string> provod2::getVertex () {
	std::vector<std::string> __out;

	for (auto it = this->_tree->_vp.begin(); it != this->_tree->_vp.end(); ++it)
		if (it->second)
			__out.push_back(it->second->_name);

	return __out;
}

//определяю функцию для вершины
std::vector<std::string> _provod2_sys::node::getVertex () {
	std::vector<std::string> __out;

	for (auto it = this->_vp.begin(); it != this->_vp.end(); ++it)
		if (it->second)
			__out.push_back(it->second->_name);

	return __out;
}


//даю имя вершине дерева и укащываю родителя
_provod2_sys::node::node (std::string __a, node* __b) {
	this->_name = __a;
	this->_par = __b;
}

//здесь обращение идет к классу
_provod2_sys::node& provod2::operator[] (const std::string __a) {
	if (!this->_tree->_vp[__a])
		this->_tree->_vp[__a] = new _provod2_sys::node(__a, this->_tree);

	return *(this->_tree->_vp[__a]);
}

//здесь обращение идет к указателю на структуру 
_provod2_sys::node& _provod2_sys::node::operator[] (const std::string __a) {
	if (!this->_vp[__a])
		this->_vp[__a] = new _provod2_sys::node(__a, this);

	return *(this->_vp[__a]);
}

//определяю конструктор
provod2::provod2 (std::string __a) {
	//запоминаю файл 
	this->_file = __a;

	//открываю файл, в котором содержится информация для класса
	this->_in.open(__a.c_str(), std::ios_base::in);

	this->_tree = new _provod2_sys::node("", nullptr); //создаю дерево

	//указываю на вершину, которую я рассматриваю в данный момент
	_provod2_sys::node *cur = this->_tree; 

	std::string __d, // 
	__zn; //информация, которую нужно записать

	while (this->_in >> __d) { //читаю файл

		if (__d == "(") { //если нужно спуститься по дереву
			(*cur)[__zn]; //создаю новою вершину и спускаюсь в нее
			cur = cur->_vp[__zn];
			__zn.clear(); //чищу значение
			continue;
		}

		if (__d == ")") { //если нужно подняться по дереву
			cur = cur->_par;
			continue;
		}

		__zn += __d; //дополняю значение информацией из буффера
	}
	
	this->_in.close(); //закрываю файл

}

//определяю ф-ции для записи
void _provod2_sys::write(std::ofstream &__out, _provod2_sys::node *__a) {
	__out << __a->_name << " ( "; //записываю спуск

	for (auto it = __a->_vp.begin(); it != __a->_vp.end(); ++it)
		if (!it->second) //проверяю, сущетсвует ли вершина
			continue;
		else
			_provod2_sys::write(__out, it->second); //рекурсивно спускаюсь по дереву

	__out << " ) "; //записываю подъем

}

//запись из мнимого корня

provod2::write() {
	this->_out.open(this->_file, std::ios_base::out | 
			std::ios_base::trunc); //открываю файл для записи

	for (auto it = this->_tree->_vp.begin(); it != this->_tree->_vp.end(); ++it)
		if (!it->second) //существует ли вершина
			continue;
		else
			_provod2_sys::write(this->_out, it->second); //спускаюсь в вершину
	
	this->_out.close(); //закрываю файл

}