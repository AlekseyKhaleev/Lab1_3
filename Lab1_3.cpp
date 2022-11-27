// Lab1_3.cpp: Листинг программы для 1-й лабораторной работы 3-го семестра
//*********************************************************************************
// Имя файла: Lab1_3.cpp
// Резюме: Пример программы написанной на языке программирования С++
// Описание:
// Программа позволяет выполнять шифрование/дешифрование текста из файла
// Шифрование/дешифрование реализовано с помощью линейного конгруэнтного генератора случайных чисел,
// для каждого блока из 64 байт генерируется новый ключ, с помощью которого производится посимвольное
// XoR преобразование текста.
// - для корректного вызова программы требуется указать серию аргументов:
// --help или -h      : режим справки
// [N] [file_name] : режим шифрования/дешифрования
// где:
//    N – стартовый ключ генератора случайных чисел;
//    file_name – абсолютный или относительный путь к файлу;
// Дата создания: 27 / 11 / 2022
// Автор : Студент НГТУ ИРИТ, Халеев А.А., гр. 21-ИВТз
// Платформа : OS Ubuntu 22.04, Jet Brains Clion IDE (Study)
// *********************************************************************************

#include <iostream>
#include <fstream>
#include "SyntaxException.h"

const std::size_t step = 64; // шаг генерации случайных чисел

int next_rand(int start_key = 0) {
    static int rand_num{0};
    if (start_key) {
        rand_num = (421 * start_key + 1663) % 7875;
    } else
        rand_num = (421 * rand_num + 1663) % 7875;
    return rand_num;
}

int main(int argc, char **argv) {
    try {
        switch (argc) {
            case 2: {   // получен 1 пользовательский аргумент + argc (помним, что argc всегда получаем первым)
                /* Если полученный аргумент соответствует запуску в режиме справки*/
                if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
                    std::string help_screen[] = {
                            "\nLab1_3 is text encoding/decoding app\n",
                            "Use the same start key to decode your encoded text\n",
                            "Be care of your path - it's need to be correct\n\n",
                            "Correct arguments for run this app:\n\n",
                            "--help or -h    : help mode\n",
                            "[N] [file_name] : encryption/decryption\n",
                            "    N           – start LKG key;\n",
                            "file_name       – file name or full path to save/read the text file;\n\n"
                    };
                    for (auto &i: help_screen) {
                        std::cout << i;
                    }
                } else
                    throw SyntaxException("First argument is wrong. Among other things, check your keyboard layout", 1,
                                          argc);
                break;
            } break;
            case 3: { // получено 2 пользовательских аргумента + argc (помним, что argc всегда получаем первым)
                int key;
                try {
                    key = std::stoi(argv[1]); // попытка привести к целому типу полученного аргумента
                } catch (std::exception &stoi_err) {
                    throw SyntaxException("The specified number of lines must be an integer", 2, argc);
                }
                if (argv[1] != std::to_string(key)) {
                    throw SyntaxException(
                            "It is not possible to use a floating point number to specify the start LKG number\n"
                            "You should also use only digits for an integer", 3, argc);
                } else {
                    int next_key = next_rand(key); // инициализируем стартовый ключ для XoR преобразования
                    char str_buf;           // буфер для считанных символов
                    std::string main_buf;   // буфер-накопитель
                    std::size_t chr_cnt{0}; // счетчик считанных символов

                    /* Открытие файла */
                    std::fstream data(argv[2], std::fstream::in | std::fstream::out);
                    if (data.is_open()) {
                        while (!data.eof()) {                          // Пока не достигнут конец файла
                            data.read(&str_buf, sizeof(char));   // Считать очередной символ в str_buf
                            main_buf += str_buf ^ next_key;           // XoR преобразование символа с добавлением в буфер-накопитель
                            chr_cnt++;
                            if (chr_cnt == step) {
                                next_key = next_rand();             // генерируем следующий ключ
                                chr_cnt = 0;                        // обнуляем счетчик
                            }
                        }
                        data.clear();
                        data.seekg(0);
                        for (auto &elem: main_buf) {
                            data.write(&elem, sizeof(char)); // запись преобразованного текста
                        }
                    } else {
                        throw SyntaxException(
                                "Unable to open or create specified file. Check that the specified file exists",
                                4, argc);                      // Аварийное завершение программы с выводом справки
                    }
                    /* Закрытие файла */
                    data.close();
                    if (data.is_open() != 0) {
                        throw SyntaxException("Unable to close specified file", 8, argc);
                    }
                }
            } break;
            default: /* Если полученное количество пользовательских аргументов больше 3 или равно 0 */
            {
                throw SyntaxException(
                        "Wrong number of arguments", 6, argc);
            }
        }
    } catch (SyntaxException &ex)
    /* Если поймали собственное исключение (переданы некорректные аргументы командной строки) */
    {
        ex.description(); // Вызываем сообщение, соответствующее ошибке с помощью метода .description()
        return 1; // Завершение программы с кодом 1
    }
    return 0;
}
