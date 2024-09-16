#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

#define DATABASE_FILENAME "computers.dat"
#define SEARCH_RESULTS_FILENAME "search_results.dat"

const char PR_R[] = "rb";
const char PR_S[] = "r+b";
const char PR_W[] = "wb";
const char PR_A[] = "ab";

typedef struct {
    char processorType[20];
    double processorFrequency;
    int ramCapacity;
    int hddCapacity;
    char monitorType[20];
} Computer;

bool fileExists(const string&);
void createDatabase(const string&);
void addRecord(const string&);
void searchByProcessorType(const string&, const string&);
void sortByProcessorType(const string&, const string&);
void sortByProcessorFrequency(const string&, const string&);
void printFileContents(const string&);
void clearFile(const string&);
string getlineSafe();
bool compareProcessorType(const Computer&, const Computer&);
bool compareProcessorFrequency(const Computer&, const Computer&);


bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void createDatabase(const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка создания файла базы данных!" << endl;
        return;
    }

    cout << "Файл базы данных успешно создан." << endl;
}

void addRecord(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);

    if (!file.is_open()) {
        cout << "Ошибка открытия файла для добавления записи!" << endl;
        return;
    }

    Computer computer{};

    cout << "Введите данные о компьютере:" << endl;
    cout << "Тип процессора: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(computer.processorType, 20);

    cout << "Тактовая частота процессора: ";
    cin >> computer.processorFrequency;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Объем оперативной памяти (Гбайт): ";
    cin >> computer.ramCapacity;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Объем жесткого диска (Гбайт): ";
    cin >> computer.hddCapacity;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Тип монитора: ";
    cin.getline(computer.monitorType, 20);

    file.write((char*)&computer, sizeof(computer));

    cout << "Запись успешно добавлена." << endl;
}

void searchByProcessorType(const string& filename, const string& searchTerm) {
    ifstream infile(filename, ios::binary);
    ofstream outfile(SEARCH_RESULTS_FILENAME, ios::binary);

    if (!infile.is_open() || !outfile.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    bool found = false;
    Computer computer{};
    while (infile.read((char*)&computer, sizeof(computer))) {
        if (strstr(computer.processorType, searchTerm.c_str()) != nullptr) {
            outfile.write((char*)&computer, sizeof(computer));
            found = true;
        }
    }

    if (found) {
        cout << "Результаты поиска сохранены в файл " << SEARCH_RESULTS_FILENAME << endl;
    }
    else {
        cout << "Записи не найдены." << endl;
    }
}

bool compareProcessorType(const Computer& a, const Computer& b) {
    return strcmp(a.processorType, b.processorType) < 0;
}

bool compareProcessorFrequency(const Computer& a, const Computer& b) {
    return a.processorFrequency < b.processorFrequency;
}

void sortByProcessorType(const string& filename, const string& outputFilename) {
    ifstream file(filename, ios::binary);
    ofstream outfile(outputFilename, ios::binary);

    if (!file.is_open() || !outfile.is_open()) {
        cout << "Ошибка открытия файла для сортировки!" << endl;
        return;
    }

    file.seekg(0, ios::end);
    long fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int count = fileSize / sizeof(Computer);

    Computer* computers = new Computer[count];

    for (int i = 0; i < count; i++) {
        file.read((char*)&computers[i], sizeof(Computer));
    }

    sort(computers, computers + count, compareProcessorType);

    for (int i = 0; i < count; i++) {
        outfile.write((char*)&computers[i], sizeof(Computer));
    }

    delete[] computers;

    cout << "Сортировка по типу процессора выполнена." << endl;
}

void sortByProcessorFrequency(const string& filename, const string& outputFilename) {
    ifstream file(filename, ios::binary);
    ofstream outfile(outputFilename, ios::binary);

    if (!file.is_open() || !outfile.is_open()) {
        cout << "Ошибка открытия файла для сортировки!" << endl;
        return;
    }

    file.seekg(0, ios::end);
    long fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int count = fileSize / sizeof(Computer);

    Computer* computers = new Computer[count];

    for (int i = 0; i < count; i++) {
        file.read((char*)&computers[i], sizeof(Computer));
    }

    sort(computers, computers + count, compareProcessorFrequency);

    for (int i = 0; i < count; i++) {
        outfile.write((char*)&computers[i], sizeof(Computer));
    }

    delete[] computers;

    cout << "Сортировка по тактовой частоте процессора выполнена." << endl;
}

void printFileContents(const string& filename) {
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения!" << endl;
        return;
    }

    if (file.peek() == EOF) {
        cout << "База данных пуста." << endl;
        return;
    }

    cout << "База данных компьютеров" << endl;
    for (int i = 0; i <= 80; i++)
        printf("-");
    printf("\n|%-20s|%-20s|%-10s|%-10s|%-15s|\n",
        "Тип процессора", "Тактовая частота", "Объем ОЗУ", "Объем HDD", "Тип монитора");
    for (int i = 0; i <= 80; i++)
        printf("-");
    cout << endl;

    Computer computer{};
    while (file.read((char*)&computer, sizeof(computer))) {
        printf("|%-20s|%-20f|%-10d|%-10d|%-15s|\n",
            computer.processorType, computer.processorFrequency,
            computer.ramCapacity, computer.hddCapacity,
            computer.monitorType);
    }

    for (int i = 0; i <= 80; i++)
        printf("-");
    cout << endl;
}

void clearFile(const string& filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    if (file.is_open()) {
        cout << "Файл " << filename << " успешно очищен." << endl;
    }
    else {
        cout << "Ошибка очистки файла " << filename << "!" << endl;
    }
}

string getlineSafe() {
    cin.ignore();
    string input;
    getline(cin, input);
    return input;
}

int main() {
    setlocale(LC_ALL, "Russian");

    if (!fileExists(DATABASE_FILENAME)) {
        createDatabase(DATABASE_FILENAME);
    }

    if (!fileExists(SEARCH_RESULTS_FILENAME)) {
        createDatabase(SEARCH_RESULTS_FILENAME);
    }

    int choice;
    string searchTerm;

    do {
        cout << "\nМеню:" << endl;
        cout << "1. Добавить запись" << endl;
        cout << "2. Сортировать по тактовой частоте процессора" << endl;
        cout << "3. Сортировать по типу процессора" << endl;
        cout << "4. Поиск по типу процессора" << endl;
        cout << "5. Вывести содержимое файла базы данных" << endl;
        cout << "6. Вывести содержимое файла результатов поиска" << endl;
        cout << "7. Очистить файл базы данных" << endl;
        cout << "8. Очистить файл вывода" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addRecord(DATABASE_FILENAME);
            break;
        case 2:
            sortByProcessorFrequency(DATABASE_FILENAME, SEARCH_RESULTS_FILENAME);
            break;
        case 3:
            sortByProcessorType(DATABASE_FILENAME, SEARCH_RESULTS_FILENAME);
            break;
        case 4:
            cout << "Введите тип процессора для поиска: ";
            searchTerm = getlineSafe();
            searchByProcessorType(DATABASE_FILENAME, searchTerm);
            break;
        case 5:
            printFileContents(DATABASE_FILENAME);
            break;
        case 6:
            printFileContents(SEARCH_RESULTS_FILENAME);
            clearFile(SEARCH_RESULTS_FILENAME);
            break;
        case 7:
            clearFile(DATABASE_FILENAME);
            break;
        case 8:
            clearFile(SEARCH_RESULTS_FILENAME);
            break;
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор!" << endl;
        }
    } while (choice != 0);

    return 0;
}
