#include "date.h"

//===некоторые данные и функции, необходимые для работы===

namespace _date_sys {

	bool _all_debug = false;

	std::unordered_map<int, std::string> _dweek = { //даю каждому номеру день недели
		{1, "Mon"},
		{2, "Tue"},
		{3, "Wed"},
		{4, "Thu"},
		{5, "Fri"}, 
		{6, "Sat"}, 
		{7, "Sun"}
	};

	std::unordered_map<std::string, int> _mes_int = { //даю каждому месяцу номер
		{"Jan", 1},
		{"Feb", 2},
		{"Mar", 3},
		{"Apr", 4},
		{"May", 5},
		{"Jun", 6},
		{"Jul", 7},
		{"Aug", 8},
		{"Sep", 9},
		{"Oct", 10},
		{"Nov", 11},
		{"Dec", 12}
	};

	std::multimap<int, int> _mes_day = { //опеределяю, сколько дней в каждом месяце
		{1, 31},
		{2, 28},
		{2, 29}, //если весокосный год
		{3, 31},
		{4, 30},
		{5, 31},
		{6, 30},
		{7, 31},
		{8, 31},
		{9, 30},
		{10, 31},
		{11, 30},
		{12, 31}
	};

	enum _enum_of_mes_int { //назначаю каждому номеру метку месяца
		JAN = 1,
		FEB,
		MAR,
		APR,
		MAY,
		JUN,
		JUL,
		AUG,
		SEP,
		OCT,
		NOV,
		DEC
	};

	//проверяю корректность даты
	bool dateIsCorrect (std::string &__date) {
		if (__date.size() != 10)  
			return false;

		if (__date[0] < '0' || __date[0] > '9') //проверяю день
			return false;
		if (__date[1] < '0' || __date[1] > '9')
			return false; 

		if (__date[3] < '0' || __date[3] > '9') //месяц
			return false;
		if (__date[4] < '0' || __date[4] > '9')
			return false;

		if (__date[6] < '0' || __date[6] > '9') //год
			return false;
		if (__date[7] < '0' || __date[7] > '9')
			return false;
		if (__date[8] < '0' || __date[8] > '9')
			return false;
		if (__date[9] < '0' || __date[9] > '9')
			return false;

		//проверяю, могло ли быть столько дней в месяце
		int __mes = (__date[3] - '0') * 10 + (__date[4] - '0');
		int __day = (__date[0] - '0') * 10 + (__date[1] - '0');
		int __god = (__date[6] - '0') * 1000 + (__date[7] - '0') * 100 +
			(__date[8] - '0') * 10 + (__date[9] - '0');

		if (__mes > 12 || __mes <= 0)
			return false;	

		if ((__day > _mes_day.upper_bound(__mes - 1)->second ||
			 	__day <= 0) && __mes != 2) //весокосный год и февраль 
			return false;			  //проверю отдельно

		if ((__god % 100 != 0 || __god % 400 == 0) && __god % 4 == 0) { //проверяю весокосный год и 
			if (__mes == 2 && (__day > 29 || __day <= 0))					//февраль
				return false;
		} else if (__mes == 2 && (__day > 28 || __day <= 0)) 
				return false;

		return true; //если дата корректна
	}

	bool dateIsCorrect (int __day, int __mes, int __god) {
		//сведу к уже определенной версии, после маленькой проверки
		if (__day > 99 || __day <= 0 || __mes > 99 || __mes <= 0 || __god > 9999 || __god <= 0)
			return false;
		std::string __d = std::string() + char(__day / 10 + '0') + char(__day % 10 + '0') + '.' +
			char(__mes / 10 + '0') + char(__mes % 10 + '0') + '.' + char(__god / 1000 + '0') +
			char(__god / 100 % 10 + '0') + char(__god / 10 % 10 + '0') + char(__god % 10 + '0');
		return dateIsCorrect(__d);	
	}

}

//======

//---конструкторы---

//определяю обычный конструктор
date::date () { }

//определяю конструктор, на  вход которого подается 3 числа
date::date (int __a, int __b, int __c) {
	this->setDate(__a, __b, __c);
}

//определяю конструктор, на вход которого подается указатель на строку
date::date (std::string *__a) {
	this->setDate(__a);
}

//определяю конструктор, на вход которого подается строка
date::date (std::string __a) {
	this->setDate(__a);
}

//определяю конструкьло, на вход которого подается объекст класса date
date::date (const date &__a) {
	this->_debug |= __a._debug; //для того, чтобы дебаг для элемента сохранился в любом случае

	this->_existDate = __a._existDate;

	if (this->_existDate) {
		this->_day = __a._day;
		this->_mes = __a._mes;
		this->_god = __a._god;
	}
}

date::date (std::vector<int> __a) {
	//проверяю, что числа 3
	if (__a.size() != 3) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: date(std::vector), incorrect digits\n";
		return;
	}

	this->setDate(__a[0], __a[1], __a[2]);
}

date::date (const char __a[11]) {
	this->setDate(std::string(__a));
}

date::date (int __a) {
	if (__a == 0) //оставляю дату несуществующей
		return;

	__a--;
	int __god = (__a / (365 + 365 + 365 + 366)) * 4 +        //по несложной формуле вычисляю год
					(__a % (365 + 365 + 365 + 366)) / 365 + 1;		

	bool __vs = ((__god % 100 != 0 || __god % 400 == 0) && __god % 4 == 0); //весокосный ли год				

	int __days = __a - ((__god - 1)  / 4 * (365 + 365 + 365 + 366) + 
					(__god - 1)  % 4 * 365); //сколько дней прошло в этом году

	int __mes = 1; //высчитываю месяц и день
	for (auto it = _date_sys::_mes_day.begin(); it != _date_sys::_mes_day.end(); ++it) {
		if (__vs && it->second == 28)
			continue;
		if (!__vs && it->second == 29)
			continue;

		if (__days >= it->second) {
			__days -= it->second;
			__mes++;
		} else 
			break;
	}

	int __day = 1 + __days;
 
	this->setDate(__day, __mes, __god);
}

//------


//===функции получения даты===

//записываю дату в строку 
void date::getDate (std::string &__a) {
	__a.clear(); // чищу строку перед ее заполнением

	if (!this->_existDate) { //проверяю, если и информация о дате
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: getDate(std::string&), date not exist\n";
		return;
	}

	__a += char(this->_day / 10 + '0'); //записываю день
	__a += char(this->_day % 10 + '0');

	__a += '.';

	__a += char(this->_mes / 10 + '0'); //месяц
	__a += char(this->_mes % 10 + '0');

	__a += '.';

	__a += char(this->_god / 1000 + '0'); //год
	__a += char(this->_god / 100 % 10 + '0');
	__a += char(this->_god / 10 % 10 + '0');
	__a += char(this->_god % 10 + '0');
}


//возвращаю строку
std::string date::getDate () {
	std::string __d; //тут будет строка для вывода
	this->getDate(__d); //вызываю уже определенную функцию
	return __d;
}

//======


//---функции установки даты---

//устанавливаю актуальную дату
void date::setDateLocal () {
	time_t __ct;
	time (&__ct);
	std::string __d = asctime(localtime(&__ct));

	//"вырезаю" информацию о месяце
	std::string __b = std::string() + __d.substr(4, 3);

	//узнаю месяц, записывая его в числовом формате
	switch (_date_sys::_mes_int[__b]) {

		case (_date_sys::JAN):
			this->_mes = _date_sys::JAN;
			break;

		case (_date_sys::FEB):
			this->_mes = _date_sys::FEB;
			break;

		case (_date_sys::MAR):
			this->_mes = _date_sys::MAR;
			break;

		case (_date_sys::APR):
			this->_mes = _date_sys::APR;
			break;

		case (_date_sys::MAY):
			this->_mes = _date_sys::MAY;
			break;

		case (_date_sys::JUN):
			this->_mes = _date_sys::JUN;
			break;

		case (_date_sys::JUL):
			this->_mes = _date_sys::JUL;
			break;

		case (_date_sys::AUG):
			this->_mes = _date_sys::AUG;
			break;

		case (_date_sys::SEP):
			this->_mes = _date_sys::SEP;
			break;

		case (_date_sys::OCT):
			this->_mes = _date_sys::OCT;
			break;

		case (_date_sys::NOV):
			this->_mes = _date_sys::NOV;
			break;

		case (_date_sys::DEC):
			this->_mes = _date_sys::DEC;
			break;											
	}

	//"вырезаю" информацию о дне
	__b = std::string() + __d.substr(8, 2);

	//узнаю день, записывая его в числовом формате
	this->_day = (__b[0] - '0') * 10 + (__b[1] - '0');

	//"вырезаю" информацию о том, какой сейчас год
	__b = std::string() + __d.substr(20, 4);

	//узнаю год, записывая его в числовом формате
	this->_god = (__b[0] - '0') * 1000 + (__b[1] - '0') * 100 + (__b[2] - '0') * 10 +
		 (__b[3] - '0');

	this->_existDate = true; //отмечаю, что дата существует	 
}

void date::setDate (std::string *__d) {
	//сведу к уже определенной функцие
	std::string __b = (*__d);
	this->setDate(__b);
}

//устанавливаю дату по строке формата "ДД.ММ.ГГГГ"
void date::setDate (std::string __d) {
	if (!_date_sys::dateIsCorrect(__d)) { //если введенная дата некорректна
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: setDate(std::string), incorrect date\n";

		return;
	}

	//если дата корректная, устанавливаю ее

	this->_day = (__d[0] - '0') * 10 + (__d[1] - '0');
	this->_mes = (__d[3] - '0') * 10 + (__d[4] - '0');
	this->_god = (__d[6] - '0') * 1000 + (__d[7] - '0') * 100 +
		(__d[8] - '0') * 10 + (__d[9] - '0');

	this->_existDate = true; //отмечаю, что дата существует	
}

void date::setDate (int __a, int __b, int __c) {
	if (!_date_sys::dateIsCorrect(__a, __b, __c)) { //проверяю, корректная ли дата
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: setDate(int, int, int), incorrect digits\n";
		
		return;
	}

	this->_day = __a;
	this->_mes = __b;
	this->_god = __c;

	this->_existDate = true;
}

//------

//===операторы изменения даты===

date& date::operator++ () {
	//проверяю, существует ли дата
	if (!this->_existDate) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: operator++(), date not exist\n";
		return *this;
	}

	//обрабатываю возможное "переполнение" даты
	if (this->_god == 9999 && this->_mes == 12 && this->_day == 31) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: operator++(), date so big\n";
		return *this;
	}


	this->_day++;

	//обрабатываю весокосный год
	if (this->_mes == 2 && (this->_god % 100 != 0 || this->_god % 400 == 0) && this->_god % 4 == 0) {
		//нужно ли менять месяц
		if (this->_day > 29) {
			this->_day = 1;
			this->_mes++;
		}
	}

	//нужно ли менять месяц, не обрабатываю весокосный год
	if (_date_sys::_mes_day.upper_bound(this->_mes - 1)->second < this->_day && 
		!(this->_mes == 2 && (this->_god % 100 != 0 || this->_god % 400 == 0) && this->_god % 4 == 0)) {

		this->_mes++;
		this->_day = 1;
		//нужно ли менять гоsд
		if (_mes > 12) {
			this->_god++;
			this->_mes = 1;
		}
	}

	return *this;
}

date& date::operator-- () {
	//проверяю, существует ли дата
	if (!this->_existDate) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: operator++(), date not exist\n";
		return *this;
	}

	//обрабатываю, может ли дата стать невозможной
	if (this->_god == 1 && this->_mes == 1 && this->_day == 1) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: operator--(), date so low\n";
		return *this;
	}

	this->_day--;

	//обрабатываю весокосный год
	if (this->_mes == 3 && (this->_god % 100 != 0 || this->_god % 400 == 0) && this->_god % 4 == 0) {
		//нужно ли менять месяц
		if (this->_day == 0) {
			this->_day = 29;
			this->_mes--;
		}
	}

	//нужно ли менять месяц, не обрабатываю весокосный год
	if (this->_day == 0 && 
		!(this->_mes == 2 && (this->_god % 100 != 0 || this->_god % 400 == 0) && this->_god % 4 == 0)) {

		this->_mes--;
		//нужно ли менять год
		if (this->_mes == 0) {
			this->_mes = 12;
			this->_god--;
		}

		this->_day = _date_sys::_mes_day.upper_bound(this->_mes - 1)->second; //устанавливаю нужный день
	}

	return *this;
}

date date::operator++ (int) {
	date __a = (*this);
	++(*this);
	return __a;
}

date date::operator-- (int) {
	date __a = (*this);
	--(*this);
	return __a;
}

//======

//---оператор присваивания

//приравниваю объекты
date date::operator= (date __a) {
	
	this->_debug |= __a._debug; //чтобы дебаг для элемента сохранился в любом случае
	this->_existDate = __a._existDate;

	if (this->_existDate || _date_sys::_all_debug) {
		this->_day = __a._day;
		this->_mes = __a._mes;
		this->_god = __a._god;
	}

	return *this;
}

//------

//===геттеры=== 

bool date::getDebugUse () {
	return _debug;
}

bool date::getExist () {
	return _existDate;
}

int date::getDay () {
	//проверяю, существует ли день
	if (!this->_existDate) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: getDay(), date not exist\n";
		return _day;
	}

	return _day;
}

int date::getMonth () {
	//проверяю, существует ли месяц
	if (!this->_existDate) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: getMonth(), date not exist\n";
		return _mes;
	}

	return _mes;
}

int date::getYear () {
	//проверяю, существует ли год
	if (!this->_existDate) {
		if (this->_debug || _date_sys::_all_debug)
			std::cout << "error_date: getYear(), date not exist\n";
		return _god;
	}

	return _god;
}

//получаю день недели, знаю информацию о дне недели одного дня

std::string date::getDayOfWeek () {
	//737655 - понедельник
	if (int(*this) >= 737655)
		return _date_sys::_dweek[(*this - 737655) % 7 + 1];
	return _date_sys::_dweek[(7 - abs(*this - 737655) % 7) % 7 + 1];
}

//======

//---дебаг---

//нужно ли выводить информацию об ошибках
void date::useDebug (bool __a) {
	this->_debug = __a;
}

//------

//===чистка===

void date::clear () {
	this->_existDate = false;
	this->_day = 0;
	this->_mes = 0;
	this->_god = 0;
}

//======

//---оператора преобразования---

date::operator int () {

	if (!this->_existDate) //если дата не существует, считаю ее равной 0
		return 0;

	int __out = (this->_god - 1)  / 4 * (365 + 365 + 365 + 366) + 
					(this->_god - 1)  % 4 * 365; //по несложной формуле, вычисляю,
												 //сколько полных лет прошло (в днях)
	bool __vs = ((this->_god - 1)  / 4 + 1) * (365 + 365 + 365 + 366) - //весокосный ли год
					__out == 366; 

	for (auto it = _date_sys::_mes_day.begin();     //считаю сколько дней прошло (по месяцам)
			it != _date_sys::_mes_day.end(); ++it) {

		if (it->first == this->_mes)
			break;

		if (it->second == 28 && __vs)
			continue;

		if (it->second == 29 && !__vs)
			continue;

		__out += it->second;
	}

	__out += this->_day;    //прибавляю оставшиеся дни

	return __out;
}

date::operator std::string () {
	//свожу к определенной функцие
	std::string __out = this->getDate();
	return __out;
}

date::operator std::vector<int> () {
	//проверяю, существует ли дата
	if (!this->_existDate) {
		if (this->_debug)
			std::cout << "error_date: operator std::vector<int>(), date not exist\n";
		return std::vector<int>{};
	}

	return std::vector<int>{this->_day, this->_mes, this->_god};
}

//------


//===операторы сравнения===

bool operator== (date __a, date __b) {
	if (__a._existDate != __b._existDate) //если у кого-то дата существует, они точно не равны
		return false;

	if (!__a._existDate) //из условия выше понятно, что их переменная-сущетвования совпадает
		return true;	//если у обоих нет даты, я считаю их равными

	return (__a._day == __b._day &&
			 __a._mes == __b._mes && 
			 __a._god == __b._god); //из условия выше следует, что у обоих объектов существуют даты
			 						//сравниваю их даты		
}

//свожу к уже определенной функцие
bool operator!= (date __a, date __b) {
	return !(__a == __b);
}

bool operator> (date __a, date __b) {
	if (__a._existDate && !__b._existDate) //я считаю существующую дату больше несуществующей
		return true;
	if (!__a._existDate && __b._existDate)
		return false;

	if (!__a._existDate) //из условия выше следует, что у обоих объектов переменная-существования совпадает
		return false;	 //я считаю несуществующие даты равными	

	if (__a._god > __b._god) //из условий выше видно, что обе даты существуют
		return true;
	if (__a._god < __b._god)
		return false;

	if (__a._mes > __b._mes)
		return true;
	if (__a._mes < __b._mes)
		return false;

	if (__a._day > __b._day)
		return true;

	return false;
}

//свожу к уже определенной функцие
bool operator< (date __a, date __b) {
	return (__b > __a);
}

//оставшиеся операторы сравнения легко сводятся к определенным операторам
bool operator>= (date __a, date __b) {
	return ((__a == __b) || (__a > __b));
}

bool operator<= (date __a, date __b) {
	return ((__a == __b) || (__a < __b));
}

//======


//---сеттер для _all_debug---

void _date_set_all_debug (bool __a) {
	_date_sys::_all_debug = __a;
}

//------


