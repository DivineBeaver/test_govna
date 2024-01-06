#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Эта функция разбирает строку, работает как split() в питоне, принимает строку, список разделителей
// и возвращает вектор string
// Советую использовать для разбора ответов, для этого в отдельную функцию и выносил
vector<string> pars(string s, const char* sep)
{
    vector<string> vec;
    char* cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    char* token = strtok(cstr, sep);
    while (token != NULL)
    {
        vec.push_back(token);
        token = strtok(NULL, sep);
    }
    delete[] cstr;

    return vec;
}

pair<vector<vector<string>>, vector<vector<string> >> bulba(int zadanie, int nomer)
{
    vector<string> lines, ans, temp1, temp2;
    vector<vector<string>> output, input;
    string name, line;
    ifstream fin;
    int number;
    number = zadanie * 10 + nomer;
    name = to_string(zadanie) + "." + to_string(nomer) + ".txt";
    fin.open(name);
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    fin.close();
    for (string s : lines)
    {
        ans.clear();
        ans = pars(s, "|");
        switch (number)
        {
        case 12:
            temp1.clear();
            temp2.clear();
            temp1 = pars(ans[0], " ");
            temp2 = pars(ans[1], " ");
            input.push_back(temp1);
            output.push_back(temp2);
            break;
        default:
            break;
        }
    }
    return make_pair(input, output);
}


//узнает количество попаданий символа в строку
int podshitinator(char symbol, const std::string& str) {
    int count = 0;
    for (char ch : str) {
        if (ch == symbol) {
            count++;
        }
    }
    return count;
}

//Возвращает строку между двумя знаками ch
string betweenAtSymbols(const string& str, char ch) {
    size_t firstAt = str.find(ch);
    size_t secondAt = str.find(ch, firstAt + 1);
    string otv;
    if (firstAt != string::npos && secondAt != string::npos) {
        otv = str.substr(firstAt + 1, secondAt - firstAt - 1);
        otv = ' ' + otv + ' ';
        while (otv.find("  ") != string::npos) {
            otv.replace(otv.find("  "), 2, " ");
        }
        return otv;
    }else {
        return "'@' не найдены дважды"; 
    } 
}
    
//Собирает вывод для проверки в строку
string sobiratelVStr(vector<string>& m){
    string slovo=" ";
    for (int i = 0; i < size(m); i++) {
        slovo +=m[i]+" ";
    }
    return slovo;
}



int main() {
    // Получаем дескриптор стандартного вывода
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setlocale(0, "");
    const char* program_name = R"("D:\\Системные папки\\Рабочий стол\\C++\\1\\x64\\Debug\\1.exe")"; // чё такое R не ебу, но оно работает

    auto p = bulba(1, 2);//получаем два массива, входные и выходные данные
    vector<vector<string>> m1 = p.first;
    vector<vector<string>> m2 = p.second;
    
    boolean flag=true;//Флаг правильности вывода

    for (int i = 0; i<size(m1); i++) {  // оно работает через жопу, изменил файл с данными, добавив последнюю строку пустую, чтобы он проходил все строки

        // Формирование команды для запуска консольного приложения с вводом
        char command[1024];
        snprintf(command, sizeof(command), "\"%s\"", program_name);

        FILE* temp_file = tmpfile();// Создание временного файла для чтения
        if (!temp_file) {
            std::cerr << "Ошибка при создании временного файла." << std::endl;
            return 1;
        }

        for (int q = 0; q < size(m1[i]); q++) {// Запись каждого значения во временный файл
            fprintf(temp_file, "%s\n", m1[i][q].c_str());
        }
        fflush(temp_file);
        fseek(temp_file, 0, SEEK_SET);
        
        FILE* pipe = _popen(command, "r");// Запуск процесса с использованием _popen для чтения вывода
        if (!pipe) {
            cerr << "Ошибка при открытии процесса для чтения вывода." << endl;
            return 1;
        }

        // Перенаправление стандартного ввода процесса из временного файла
        _dup2(_fileno(temp_file), _fileno(stdin)); // что такое dup2 тебе знать не обязательно, как и мне   P.S. Справедливо

        cout << "--------------------------------------------------------------------"<<endl;
        // Чтение ответных данных из процесса
        char buffer[128] = {""};  // вот тебе буфер, что там, хуй его знает, но что-то есть, вроде как всё, что есть на экране - там   P.S. В теории мы считываем строку в буфер следующей строкой кода
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) { 
            cout << buffer;
            string strBuffer(buffer);
            if (i != 0 && podshitinator('@', strBuffer) == 2) {//если это строка с ответом
                string otv = betweenAtSymbols(strBuffer, '@');//ответ человека
                string prov_otv = sobiratelVStr(m2[i-1]);//реальный ответ

                if (otv != prov_otv) {//если ответ не сошелся
                    flag = false;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);// Устанавливаем красный цвет текста
                    cout <<endl<<"============================================================================"<<endl<< "ничтожество!!! как вообще такой безддарь мог появиться на свет в столь прекрасной стране???" << endl;
                    string prov_vvod = sobiratelVStr(m1[i-1]);//Ввод, который был подан
                    cout << "Было подано на ввод:" << endl << prov_vvod << endl;
                    //SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    cout << "Ожидалось:" << endl << prov_otv << endl << "Получено:" << endl;
                    //SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    cout<<otv;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);// Возвращаем стандартный цвет текста (белый)
                    break;
                }
            }
        }
        if (!flag) {break;}//коли всё идет по жёпе, мы всё закрываем

        int exit_code = _pclose(pipe);// Закрытие процесса с использованием _pclose
        if (exit_code != 0) {// Проверка кода завершения процесса
            std::cerr << "Произошла ошибка при выполнении программы. Код завершения: " << exit_code << std::endl;
            return 1;
        }
        fclose(temp_file);// Закрытие временного файла
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);// Устанавливаем зеленый цвет текста
    if (flag) {
        cout << endl << "============================================================================"<<endl << "Ну всё норм";
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);// Возвращаем стандартный цвет текста (белый)
    return 0;
}