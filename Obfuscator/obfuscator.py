import os
import random
import string
import codecs


path_ignore = [".git", ".exe", ".md"]
path_project = "../Box_Obfuscate"
file_string_obfuscate = "string_obfuscate.txt"


def get_all_files_recursively(path: str) -> list:                           # рекурсивное получение всех файлов по указанному пути
   
    all_files = []                                      # создание пустого списка для хранения всех найденных файлов
    
    for root, dirs, files in os.walk(path):             # использование os.walk() для рекурсивного обхода директорий
        for file in files:                              # создание полного пути к файлу с использованием os.path.join()
            full_path = os.path.join(root, file)        # игнорируем указанные директории
            
            if not any(ignore in full_path for ignore in path_ignore):
                all_files.append(full_path)
    return all_files


def get_string_obfuscate() -> list:                                         # получение строк из файла string_obfuscate.txt, которые будем обфуцировать 
    string_obfuscate = []
    with open(file_string_obfuscate, 'r') as f:
        for line in f:
            if line.strip() and '#' not in line:                # если строка не пропуск и не коментарий
                string_obfuscate.append(line.strip())
    string_obfuscate = sorted(string_obfuscate)                 # сортируем массив, так как если этого не сделать, то маленькие переменные затрут часть больших
    string_obfuscate = list(dict.fromkeys(string_obfuscate))    # так же удаляем повторения
    return string_obfuscate
        

def generating_random_string():                                             # генерация рандомной строки
    # https://ru.stackoverflow.com/questions/1197413/%D0%93%D0%B5%D0%BD%D0%B5%D1%80%D0%B0%D1%86%D0%B8%D1%8F-%D1%80%D0%B0%D0%BD%D0%B4%D0%BE%D0%BC%D0%BD%D0%BE%D0%B9-%D1%81%D1%82%D1%80%D0%BE%D0%BA%D0%B8
    text = [random.choice(string.ascii_lowercase if i != 5 else string.ascii_uppercase) for i in range(random.randint(10, 20))]
    return (''.join(text))


def get_string_obfuscate_replace(string_obfuscate: list) -> dict:           # получаем словарь: {исходная строка: обфуцированная строка}

    string_obfuscate_replace = {}

    for s in string_obfuscate:
        random_s = generating_random_string()
        string_obfuscate_replace[s] = random_s
    
    return string_obfuscate_replace

    
def string_replace(all_file_project: list, string_obfuscate_replace: dict): # заменяем строки в файлах 

    for file_project in all_file_project:
        print(f"--> {file_project}")

        with open(file_project, 'r') as file:
            file_obj = codecs.open(file_project, "r", "utf_8_sig" )                     # читаем данные из файла
            file_text = file_obj.read().split('\n')                                     # делим строку на спискок строк
    
        with open(file_project, 'w') as file:                                           # очищаем исходный файл
            for line in file_text:                                                      # перебираем строки
                for original_str, replace_str in string_obfuscate_replace.items():      
                    if original_str in line:                                            # если находим одну из строк которую надо заменить
                        line = line.replace(original_str, replace_str)                  # то заменяем ее
                file.write(line)                                                        # записываем в файл измененую/неизмененую строку
                   

def main():
    print("-"*60)

    print("1. Get all files project")
    all_file_project = get_all_files_recursively(path_project)                  # получаем все файлы проекта
    # print(all_file_project)

    print("-"*60)

    print("2. Read string obfuscate in file string_obfuscate.txt")
    string_obfuscate = get_string_obfuscate()                                   # получаем строки из файла, которые будем обфуцировать
    # print(string_obfuscate)

    print("-"*60)

    print("3. Generate name obfuscate")
    string_obfuscate_replace = get_string_obfuscate_replace(string_obfuscate)   # получаем словарь: {исходная строка: обфуцированная строка}
    # print(string_obfuscate_replace)

    print("-"*60)

    print("4. Replace in file:")
    string_replace(all_file_project, string_obfuscate_replace)                  # заменяем строк в файлах 


if __name__ == "__main__":
    main()