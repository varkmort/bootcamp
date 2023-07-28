/*
    Цель работы в лагере создать свою игру в жанре текстовый квест либо dungeon crawler
    за первые три дня мы с ребятами освоили работу с циклами и условиями,
    создание структур и функций.
    К сожалению у некоторых ступор на тему провести аналогию между примером на экране
    и тем что они хотят видеть в своей программе
    ниэе примеры кода которые мы делали для демонстрационных шагов
*/

#include <iostream>     // ввод вывод пользователя
#include <string>       // работа со строками
#include <vector>       // динамические массивы
#include <fstream>      // работа с файлами
#include <Windows.h>    // функции операционной системы


/*
 демонстрационная структура описывающая некое место через
 name - навание
 peopleCount - количество постоянно размещаемых людей
 seats - количество мест для сидения в помещении(временное размещение)
 volume - объём помещения
*/
struct Place {
    std::string name;
    int peopleCount;
    int seats;
    double volume;   
};

/*
перегруженный оператор для ввода данных о помещении из любого потока ввода
*/
std::istream& operator >>(std::istream& in, Place& obj)
{
    std::string tmp;//прокси переменная через которую мы считываем данные из потока
                    //строковый тип нужен для того чтобы чтение из потока не генерировало ошибок
    
    // считывание строковых данных в поле структуры
    std::getline(in, tmp, ':');// считываем название поля до символа разделителя
    //if(tmp == "name") // лениво выключаем проверку на то что нашли правильное поле
                        // 1. кидать исключения долго
                        // 2. априори считаем что файл заполнен верно
                        // 3. в тексте оставили чтобы помнить а что нашли то
    std::getline(in, obj.name, '\n');// записывем значение поля в строковую переменную - поле структуры

    //считывание целочисленного поля структуры из потока
    std::getline(in, tmp, ':');
    //if (tmp == "peopleCount") 
    std::getline(in, tmp, '\n');// считываем значение поля в промежуточную переменную
    obj.peopleCount = std::stoi(tmp);// преобразуем строку в значение соотвествующего типа
                                     // и записываем в поле структуры

    std::getline(in, tmp, ':');
    //if(tmp == "seats")
    std::getline(in, tmp, '\n');
    obj.seats = std::stoi(tmp);//stoi - string to integer

    //считывание числа с плавающей точкой в поле структуры
    std::getline(in, tmp, ':');
    //if(tmp == "volume")
    std::getline(in, tmp, '\n');
    obj.volume = std::stod(tmp);//stod - string to double
    
    return in;//завершаем работу с потоком передачей потока следующему пользователю
}

// перегруженный оператор вывода данных типа в нужном нам формате
std::ostream& operator <<(std::ostream& out, const Place& obj) {
    // формирование записи в приемлимом для нас формате
    out << "name: " << obj.name << '\n' <<
        "peopleCount: " << obj.peopleCount << '\n' <<
        "seats: " << obj.seats << '\n' <<
        "volume: " << obj.volume;
    return out;//завершаем работу с потоком передачей потока следующему пользователю

}

// демонстрационная структура в составе которой есть коллекция других структур
struct Flat {
    std::string name;
    std::vector<Place> rooms;
};

//пример формирования вывода структуры содержащей поле в виде коллекции других струтур
std::ostream& operator <<(std::ostream& out, const Flat& obj)
{
    // формируем постоянную  часть вывода информации о полях структуры
    out << "name: " << obj.name << '\n';
    out << "roomsCount: " << obj.rooms.size() << '\n';// выводим  количество элементов в коллекции
    for (int i = 0; i < obj.rooms.size(); i++) { // цикл просто не будет работать 
                                                 // если хранилось 0 элементов
        out << obj.rooms[i];// выводим элемент коллекции оператор вывода должен быть описан заранее
        //если ещё есть элементы в коллекции то добавляем перевод строки чтобы 
        //элемены структур не сливались а были в разных строках
        if (i != obj.rooms.size() - 1) {
            out << '\n';
        }
    }
    return out;
}

//пример считывания структуры содержащей поле в виде коллекции других струтур
std::istream& operator >>(std::istream& in, Flat& obj) {
    std::string tmp;

    std::getline(in, tmp, ':');
    //if(tmp == "name:")
    std::getline(in, obj.name, '\n');

    int rooms = 0;//промежуточная переменная для работы с коллекцией
                  // нужна так как нельзя напрямую в вектор из потока засовывать 
                  // размеры и прочие служепные данные
    std::getline(in, tmp, ':');
    //if(tmp == "roomsCount:")
    std::getline(in, tmp, '\n');
    rooms = std::stoi(tmp);
    obj.rooms.clear();// очищаем вектор на случай если у нас заполняется уже заполненная структура
    //obj.rooms.reserve(rooms); так с детьми не делали но для коллекции с большим количеством вложений
    // позволит ускорить работу
    for (int i = 0; i < rooms; i++) {
        Place place;// без промежуточной переменной сложно разместить новую в векторе 
        in >> place;// для этого манёвра нужно чтобы оператор был перегружен заранее для структуры
        obj.rooms.push_back(place);// запихиапем копию того что считали в коллекцию
        //obj.rooms.push_back(std::move(place)); так не делали но очень ускоряет процесс 
        //при работе с громоздкими 
    }    
    return in;
}

int main() {
    using namespace std;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<Place> location;

    std::ifstream inf;
    inf.open("data.txt");
    if (inf.is_open()) {
        while (!inf.eof()) {
            Place tmp;
            inf >> tmp;
            location.push_back(tmp);
        }
    }
    inf.close();
    cout << "Всего комнат - " << location.size() << '\n';
    Flat flat;
    flat.name = "Резеденция Уейна";
    for (int i = 0; i < location.size(); i += 1) {
        cout << "комната №" << i + 1 << ":\n";
        cout << location[i] << '\n';
        flat.rooms.push_back(location[i]);
    }
    cout << '\n' << flat;

    std::vector<Flat> realEstate;
    inf.open("flats.txt");
    if (inf.is_open()) {
        while (!inf.eof()) {
            Flat tmp;
            inf >> tmp;
            realEstate.push_back(tmp);
        }
    }
    inf.close();
    for(const auto&house: realEstate){
        cout << '\n' << house;
    }
    return 0;
}