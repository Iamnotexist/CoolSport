#include <windows.h>
#include <windowsx.h>
              
#include "lib/date.h" //добавляю класс date
#include "lib/provod2.h" //добавляю класс provod2

//уникальные цифровые обозначения для кнопок в окне

enum {
	BUTTON_OK = 1,
	BUTTON_LEFT,
	BUTTON_RIGTH,
	BUTTON_SELECT
};


//callback функция
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//функция, добавляющая все елементы в окно
void AddControls (HWND hWnd);

HINSTANCE hInst; //этот дескриптор нужен, чтобы работать другим функциям с окном
HWND hStatic; //статик, куда производится вывод даты
HWND hUpr; //edit, куда записывается название приложения
HWND hColvo; //edit, куда записывается кол-во выполненных приложений
HWND hOut; //static, куда будет производится вывод упражнений
HWND hDateEdit; //edit, для ввода даты
HWND hOutUpr; //static, куда будет производится вывод количества упражнений

//имя приложения, окна в данном случае
LPCSTR AppName = "Sport";

//главная функция
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//дескриптор консоли, вызываемой при открытии программы
	HWND hWnd_cmd = GetConsoleWindow();
	//скрываю консоль
	ShowWindow(hWnd_cmd, SW_HIDE);

	//дескриптор окна
	HWND hWnd; 
	MSG msg;

	//сохраняю для того, чтобы в будущем работать с этим знаячением
	hInst = hInstance; 


	WNDCLASS wc;

	//описываю то, каким будет создано окно

	ZeroMemory(&wc, sizeof(wc));
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInst; 
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
	wc.lpszClassName = AppName;

	RegisterClass(&wc); 


	//создаю окно 

	hWnd = CreateWindow(
		AppName,
		AppName,
		WS_OVERLAPPED | WS_SYSMENU, 
		CW_USEDEFAULT, 0, 
		400, 300, 
		NULL, 
		NULL, 
		hInst, 
		NULL); 


	//показываю окно
	ShowWindow(hWnd, nCmdShow); 
	//чтобы была реакция на действия пользователя
	UpdateWindow(hWnd); 

	//получаю информацию о действиях пользователя
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

 
//функция, которая перерисовывает окошко, в котором показаны упражнения и их количество
void re_draw_out () {
	//строка, в которой хранится дата
	TCHAR buf[11];
	//получаю дату
	Static_GetText(hStatic, buf, 11);
	//чтобы было удобнее работать, создаю std::string
	std::string st_str = buf;

	//map, в котором храню информацию об упражнении и о количестве выполнений
 	std::map<std::string, std::string> mp;
 	
 	provod2 reader("data/Sport.info"); //открываю файл Sport.info для чтения информации
 	std::vector<std::string> uprs = reader[st_str].getVertex(); //упражения, выполненные в данную дату

 	//записываю информацию об упражнениях и количестве в map
 	for (auto it : uprs) {
 		if (reader[st_str][it].getVertex().size() == 0) {
 			reader[st_str].deleteVertex(it);
 			continue;
 		}

 		mp[it] = reader[st_str][it].getVertex()[0];
 	}

	//я делаю вывод один раз. Все, что нужно будет вывести, будет содержаться в str_out str_upr_out;

	std::string str_out, str_upr_out;

	for (auto it = mp.begin(); it != mp.end(); ++it) {
		//здесь я вывожу каждому упражнению - колво раз, которое оно было выполнено
		str_out += it->first;
		str_out += "\n";
	 	
	 	str_upr_out += it->second;
	 	str_upr_out += "\n";
	}

	//вывожу текст 
	Static_SetText(hOut, str_out.c_str());  
	Static_SetText(hOutUpr, str_upr_out.c_str());              
}

//обработка событий
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {

		case WM_CREATE: {
			//добавляю все элементы в окно
			AddControls(hWnd); 

			date cur_date; //здесь хранится дата сегодняшнего дня
			cur_date.setDateLocal(); //устанавливаю сегодняшнуюю дату

			Static_SetText(hStatic, std::string(cur_date).c_str()); //вывожу дату

			provod2 reader("data/Sport.info"); //открываю файл для чтения информации
			std::vector<std::string> uprs = reader[std::string(cur_date)].getVertex(); //получаю информацию об упражнениях

			std::string st_str = std::string(cur_date); //перевожу дату в строку

			//удаляю лишиние упражнения
 			for (auto it : uprs) {
 				if (reader[st_str][it].getVertex().size() == 0) {
 					reader[st_str].deleteVertex(it);
 					continue;
 				} 

 			}

 			reader.write(); //записываю измененную информацию

			re_draw_out();

			break;
		}

		//обрабатываю события

		case WM_COMMAND:
		                                                               
			switch(wParam) {

				//обрабатываю событие, при котором нажимается кнопка OK
			                             
				case BUTTON_OK: {
					std::string upr_str, colvo_str, n_date; //название упражнения, колво выполнений, дата выполнения упражнения

					//далее я просто получаю информацию из различных элементов окна
					TCHAR buf[40];
					Edit_GetText(hUpr, buf, 40);
					upr_str = buf;

					Edit_GetText(hColvo, buf, 40);
					colvo_str = buf;

					if (upr_str.size() == 0 || colvo_str.size() == 0) {
						MessageBox(hWnd, "Incorrect Input", NULL, MB_OK);
						break;
					}



					bool _minus = false; //отрицательное ли число 

					//если оно отрицательное, запоминаю это и удаляю '-' из строки-числа

					if (colvo_str[0] == '-') {
						_minus = true;
						colvo_str.erase(colvo_str.begin()); 	
					}

					//проверяю, число ли вводится
					bool correct_digit = true;
					for (auto it : colvo_str) {
						if (it < '0' || it > '9') {
							MessageBox(hWnd, "Incorrect Input", NULL, MB_OK);
							correct_digit = false;
							break;
						}
					}
					if (!correct_digit)
						break;


					Static_GetText(hStatic, buf, 11);
					n_date = buf;

					bool _corupr = true; //корректное ли упражнение
					for (auto it : upr_str) {
						if (it == ')' || it == '(' || (it >= '0' && it <= '9')) 
							_corupr = false;

					}

					if (!_corupr) {
						MessageBox(hWnd, "Ex. name can not have '(' or ')' or digits '0 - 9'", NULL, MB_OK);
						break;
					}

					//записываю информацию об упражнении

					provod2 prov("data/Sport.info");


					if (prov[n_date][upr_str].getVertex().size() == 0) { //если информации о таком упражнении еще нет
						if (prov[n_date].getVertex().size() == 13) { //если упражнений уже слишком много
							MessageBox(hWnd, "Ex. count 12 maximum", NULL, MB_OK);
							break;
						}


						if (colvo_str == "0") //чтобы не записывать 0
							break;


						if (_minus) { //если пытаюсь записать отрицательное число
							MessageBox(hWnd, "Incorrect Input", NULL, MB_OK);
							break;
						}
						prov[n_date][upr_str][colvo_str];
					}
					else { 
						//получаю число упражнений, которое записано в данный момент
						std::string cur_colvo = prov[n_date][upr_str].getVertex()[0];

						int int_colvo = atoi(cur_colvo.c_str()); // перевожу в целочисленный тип

						if (_minus && atoi(colvo_str.c_str()) > int_colvo) { // если пытаюсь записать отрицательное число
							MessageBox(hWnd, "Incorrect Input", NULL, MB_OK);
							break;
						}

						if (atoi(colvo_str.c_str()) + int_colvo > 5000 && !_minus) { // если пытаюсь записать слишком большое число
							MessageBox(hWnd, "Digit so big (5000 maximum)", NULL, MB_OK);
							break;
						}

						//удаляю предыдущее значение
						prov[n_date][upr_str].deleteVertex(cur_colvo); 

						//вычисляю количество
						if (_minus) 
							int_colvo -= atoi(colvo_str.c_str());
						else
							int_colvo += atoi(colvo_str.c_str());

						//если упражнение записывать не нужно
						if (int_colvo != 0) {

							char colvo_end[10] = "";
							itoa(int_colvo, colvo_end, 10); //перевожу конечное число выполнений в строку

							std::string col_end = colvo_end; //перевожу в string
							prov[n_date][upr_str][col_end]; //записываю информацию
						}
					}	 

					prov.write(); //записываю информацию в файл

					re_draw_out();
					
					break;
				}

				//обрабатываю событие, при нажатии на кноку, которая увеличит дату на 1 день 

				case BUTTON_RIGTH: {

					date r_date;
					TCHAR buf[11];
					Static_GetText(hStatic, buf, 11);
				
					r_date = std::string(buf); //получаю информацию о дате
					r_date++; //увеличиваю дату на один день 
					Static_SetText(hStatic, std::string(r_date).c_str()); //устанавливаю дату

					re_draw_out(); //вывожу информацию об упражнениях
					break;
				}

				//обрабатываю событие, при нажатии на кноку, которая уменьшит дату на 1 день
				//аналогично коду выше

			 	case BUTTON_LEFT: {
			 		
			 		date l_date;
			 		TCHAR buf[11];
			 		Static_GetText(hStatic, buf, 11);
			 		
			 		l_date = std::string(buf); 
			 		l_date--;
			 		Static_SetText(hStatic, std::string(l_date).c_str());

			 		re_draw_out();
			 		break;
			 	}

			 	//обрабатываю события нажатия на кнопку установки даты

			 	case BUTTON_SELECT: {
			 		date s_date;
			 		TCHAR buf[40];
			 		Edit_GetText(hDateEdit, buf, 40); //получаю дату, которую нужно установить

			 		s_date = std::string(buf); 

			 		if (!s_date.getExist()) { //проверяю корректность введенной даты
			 			MessageBox(hWnd, "Incorrect date, need 'DD.MM.YYYY'", NULL, MB_OK);
			 			break;
			 		}

			 		Static_SetText(hStatic, std::string(s_date).c_str()); //устанавливаю новую дату 

			 		re_draw_out(); //выписываю вывод

			 		break;
			 	}

			}
			break;

		//если окно закрывается

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}


//здесь описаны все элементы окна

void AddControls (HWND hWnd) {
	//кнопка уменьшения даты на 1
    CreateWindow("button", "<-", WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE, 0, 1, 50, 20, hWnd, (HMENU)BUTTON_LEFT, NULL, NULL);

    //кнопка увеличения даты на 1
    CreateWindow("button", "->", WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE, 157, 1, 50, 20, hWnd, (HMENU)BUTTON_RIGTH, NULL, NULL);

    //кнопка записи информации об упражнении
	CreateWindow("button", "OK", WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE, 150, 25, 50, 20, hWnd, (HMENU)BUTTON_OK, NULL, NULL);

	//edit для записи названия упражненияs
	hUpr = CreateWindow("edit", "", WS_CHILD|WS_VISIBLE, 0, 27, 100, 16, hWnd, NULL, NULL, NULL);

	//edit для записи количества выполненных упражнений
	hColvo = CreateWindow("edit", "", WS_CHILD|WS_VISIBLE, 105, 27, 30, 16, hWnd, NULL, NULL, NULL);

	//static для вывода информации об упражнениях
	hOut = CreateWindow("static", "", WS_VISIBLE|WS_CHILD, 10, 50, 150, 200, hWnd, NULL, NULL, NULL);

	//static для вывода информации о количестве упражнений
	hOutUpr = CreateWindow("static", "", WS_VISIBLE|WS_CHILD, 150, 50, 50, 200, hWnd, NULL, NULL, NULL); 

	//static для вывода даты
	hStatic = CreateWindow("static", "09.06.2020", WS_VISIBLE|WS_CHILD|SS_CENTER, 53, 3, 100, 16, hWnd, NULL, NULL, NULL);

	//edit для ввода даты
	hDateEdit = CreateWindow("edit", "", WS_CHILD|WS_VISIBLE|SS_CENTER, 250, 3, 100, 16, hWnd, NULL, NULL, NULL);

	//кнопка для установки новой даты
	CreateWindow("button", "Select New Date", WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE, 240, 25, 120, 25, hWnd, (HMENU)BUTTON_SELECT, NULL, NULL);

}




