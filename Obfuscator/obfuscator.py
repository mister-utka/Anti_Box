import os
import random
import string
import codecs
import re
import shutil
from time import sleep
from subprocess import check_output
import os
from pprint import pprint


path_ignore = [".git", ".exe", ".md"]                           # Директории и файлы, которые игнорируем и не обфуцируем
path_project = "../Box_Default"                                 # Папка и исходным кодом проекта
path_project_obfuscate = "../Box_Obfuscate"                     # Название папки, в которую скопируется проект для обфускации
path_payload_server = "../../payload"                           # Директория, в которой будут хранится итоговые исполняемые файлы для сервера
file_where_encryption_function = "encrypt_char.c"               # Файл, в который вставятся фунцкии по дешифровке
code_line = "_utka"                                             # Кодовая строка, которая идет в именах char (например funcName_utka[] = "")
script_dir = os.path.dirname(os.path.abspath(__file__)) + "\\"  # Путь до проекта

file_string_obfuscate = "string_obfuscate.txt"                  # Файл, из которого будут читаться названия функций и переменных для замеры


def clearing_blank_lines_and_comments(all_file_project):       # Удаление коментариев из файла

    for file_project in all_file_project:

        content = file_read(file_project)
        #filtered_lines = [line for line in content if line.strip() and not line.strip().startswith("//")]
        filtered_lines = [line.strip() for line in content if line.strip() and not line.strip().startswith("//")]
        file_write(file_project, filtered_lines)


def get_all_files_recursively(path: str) -> list:              # рекурсивное получение всех файлов по указанному пути
   
    all_files = []                                      # создание пустого списка для хранения всех найденных файлов

    path = script_dir + path
    print(path)
    
    for root, dirs, files in os.walk(path):             # использование os.walk() для рекурсивного обхода директорий
        for file in files:                              # создание полного пути к файлу с использованием os.path.join()
            full_path = os.path.join(root, file)        # игнорируем указанные директории
            
            if not any(ignore in full_path for ignore in path_ignore):
                all_files.append(full_path)
    return all_files


def get_string_obfuscate() -> list:                                     # получение строк из файла string_obfuscate.txt, которые будем обфуцировать 
    string_obfuscate = []
    with open(script_dir + file_string_obfuscate, 'r') as f:
        for line in f:
            if line.strip() and '#' not in line:                        # если строка не пропуск и не коментарий
                string_obfuscate.append(line.strip())
    string_obfuscate = sorted(string_obfuscate, key=len, reverse=True)  # сортируем массив, так как если этого не сделать, то маленькие переменные затрут часть больших
    string_obfuscate = list(dict.fromkeys(string_obfuscate))            # так же удаляем повторения
    return string_obfuscate
        

def generating_random_string():                                 # генерация рандомной строки
    # https://ru.stackoverflow.com/questions/1197413/%D0%93%D0%B5%D0%BD%D0%B5%D1%80%D0%B0%D1%86%D0%B8%D1%8F-%D1%80%D0%B0%D0%BD%D0%B4%D0%BE%D0%BC%D0%BD%D0%BE%D0%B9-%D1%81%D1%82%D1%80%D0%BE%D0%BA%D0%B8
    text = [random.choice(string.ascii_lowercase if i != 5 else string.ascii_uppercase) for i in range(random.randint(10, 20))]
    return (''.join(text))


def get_string_obfuscate_replace(string_obfuscate: list) -> dict:   # получаем словарь: {исходная строка: обфуцированная строка}

    string_obfuscate_replace = {}

    for s in string_obfuscate:
        random_s = generating_random_string()
        string_obfuscate_replace[s] = random_s
    
    return string_obfuscate_replace

    
def string_replace(all_file_project: list, string_obfuscate_replace: dict): # заменяем строки в файлах 

    for file_project in all_file_project:
        print(f"--> {file_project}")

        with open(file_project, 'r') as file:
            file_obj = codecs.open(file_project, "r", "utf_8_sig")                     # читаем данные из файла
            file_text = file_obj.read().split('\n')                                     # делим строку на спискок строк
    
        with open(file_project, 'w') as file:                                           # очищаем исходный файл
            for line in file_text:                                                      # перебираем строки
                for original_str, replace_str in string_obfuscate_replace.items():      
                    if original_str in line:                                            # если находим одну из строк которую надо заменить
                        line = line.replace(original_str, replace_str)                  # то заменяем ее
                file.write(line)                                                        # записываем в файл измененую/неизмененую строку


def rc4_init(key: bytes):
    """RC4 Key Scheduling Algorithm (KSA)"""
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]
    return S


def rc4_crypt(data: bytes, S: list) -> bytes:
    """RC4 Pseudo-Random Generation Algorithm (PRGA)"""
    i = j = 0
    output = bytearray()
    for byte in data:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        K = S[(S[i] + S[j]) % 256]
        output.append(byte ^ K)
    return bytes(output)


def to_c_array(byte_data: bytes, var_name: str = "encrypted_data"):

    """Форматирует байты в C-массив для вставки в код"""
    hex_bytes = ", ".join(f"0x{b:02X}" for b in byte_data)
    # print(f"unsigned char {var_name}[] = {{{hex_bytes}, '\\0'}};")
    # print(f"size_t {var_name}_len = {len(byte_data)};")
    return hex_bytes


def file_read(file_project: str) -> str:
    with open(file_project, 'r') as file:
        # file_obj = codecs.open(file_project, "r", encoding="windows-1251")                # читаем данные из файла
        file_obj = codecs.open(file_project, "r", encoding="utf_8_sig")                     # читаем данные из файла
        file_text = file_obj.read().split('\n') 
        return file_text


def file_write(file_project: str, file_text_char_crypt: str):
    # with open(file_project, 'w', encoding='windows-1251') as file:
    with open(file_project, 'w', encoding='utf_8_sig') as file:
        for line in file_text_char_crypt:
            file.write(line + '\n')


def generate_random_c_code():
    var_names = [generating_random_string() for _ in range(7)]
    types = ["int", "float"]
    operations = ["+", "-", "*", "/"]

    var1 = random.choice(var_names)
    var2 = random.choice(var_names)
    while var1 == var2:
        var2 = random.choice(var_names)

    var_type = random.choice(types)
    op = random.choice(operations)

    val1 = random.randint(1,100)
    val2 = random.randint(1,100)

    result_var = generating_random_string()

    code = f"{var_type} {var1} = {val1};\n" + \
           f"{var_type} {var2} = {val2};\n" + \
           f"{var_type} {result_var} = {var1} {op} {var2};\n"
           # f'printf("Result: %{'d' if var_type == 'int' else 'f'}\\n", {result_var});\n'
    
    return code


def test_compiling_random_code(file, function_random_code_name) -> int:

    try:
        command = f'gcc {script_dir + file} -o {script_dir + file}.exe "-Wl,--dynamicbase,--subsystem,windows"'
        # print(f"command: {command}")
        DEVNULL = open(os.devnull, 'wb')
        check_output(command, shell=True, stderr=DEVNULL, stdin=DEVNULL).decode('utf-8')
    except:
        return 1
    return 0


def generate_random_functions():

    function_random_code_name = generating_random_string()

    while True:
        result_code = f"#include <stdio.h>\n" + \
                      f"int main()" + "{\n"
        for i in range(10):
            result_code += (generate_random_c_code())
        result_code += f"return 0;\n" + \
                    "}"

        file_write("test.c", result_code.split("\n"))

        if test_compiling_random_code("test.c", function_random_code_name) == 0:
            break

        sleep(3)
    
    result_code = result_code.replace("main",function_random_code_name)
    return function_random_code_name, result_code


def inserting_encryption_functions(file):                 # Функция добавляет в файл функции для дешифровки
    with open(file, 'a', encoding='utf-8') as f:

        function_random_code_name, random_code = generate_random_functions()
        
        string = random_code + "\n" + \
                  "#define SBOX_LEN 256\n" + \
                  "void arc4_init(unsigned char *s, const unsigned char *key, size_t key_len) {\n" + \
                  "for (int i = 0; i < SBOX_LEN; i++) {\n" + \
                  "s[i] = i;\n" + \
                  "}\n" +\
                  "int j = 0;\n" + \
                  "for (int i = 0; i < SBOX_LEN; i++) {\n" + \
                  "j = (j + s[i] + key[i % key_len]) % SBOX_LEN;\n" + \
                  "unsigned char temp = s[i];\n" + \
                  "s[i] = s[j];\n" + \
                  "s[j] = temp;\n" + \
                  "}\n" + \
                  "}\n" + \
                  "void arc4_crypt(unsigned char *data, size_t data_len, unsigned char *s) {\n" + \
                  "int i = 0, j = 0;\n" + \
                  "for (size_t n = 0; n < data_len; n++) {\n" + \
                  "i = (i + 1) % SBOX_LEN;\n" + \
                  "j = (j + s[i]) % SBOX_LEN;\n" + \
                  "unsigned char temp = s[i];\n" + \
                  "s[i] = s[j];\n" + \
                  "s[j] = temp;\n" + \
                  "unsigned char k = s[(s[i] + s[j]) % SBOX_LEN];\n" + \
                  "data[n] ^= k;\n" + \
                  "}\n" + \
                  f"{function_random_code_name}();\n" + \
                  "}\n"
        
        f.write(string)


def cryptor_char(all_file_project):                 # Функция по шифрования char массивов

    for file in all_file_project:

        print(f"--> {file}")

        if os.path.basename(file) == file_where_encryption_function:                    # Если имя файла совпало с именем куда мы должны поместить функции для дешифровки
            inserting_encryption_functions(file)

        file_text = file_read(file)

        number_line = 0                                                                 # Индекс строки в словаре с которой работаем

        for line in file_text:

            if ("char" in line or "CHAR" in line) and code_line in line and "[]" in line:                   # Если в строке содержится char и кодовое слово
                # print(f"line: {line}")

                buf = line.split("=")                                                   # Отделяем объявление переменной от ее значения (char от "text")
                code = buf[0].strip()
                string = buf[1]
                # print(f"code: {code}")
                # print(f"string: {string}")

                string = re.search(r'".*"', string).group().replace('"','').encode()    # Получаем только строковое значение переменой    
                # print(f"string: {string}")

                key_krypt_char = generating_random_string().encode()                    # Генерируем для каждого масива свой уникальный ключ
                
                while True:
                    S = rc4_init(key_krypt_char)                                        # Шифруем строку
                    ciphertext = rc4_crypt(string, S)
                    # print(ciphertext)

                    S = rc4_init(key_krypt_char)                                        # Проверяем, дешифруется ли строка нормально
                    string_check = rc4_crypt(ciphertext, S)

                    if string == string_check:
                        if not "x00" in str(ciphertext):                                # Так же в строке не должно быть нулевого байта (\x00), иначе строка обрежится
                            break
                        else:
                            key_krypt_char = generating_random_string().encode()        #  Если проверка не прошла, генерируем новый ключ и шифруем заново
                
                ciphertext_format = to_c_array(ciphertext)                              # Получаем как массив, чтобы можно было вставить в исходник
                # print(ciphertext_format)

                sbox_len = generating_random_string()                                   # Генерируем радномные значения переменных, чтобы они не пересекались и не было ошибок инициализации
                key_s = generating_random_string()
                s = generating_random_string()
                data_len = generating_random_string()
                key_len = generating_random_string()
                encrypted_data = generating_random_string()

                key_krypt_char_result = "{"
                for i in key_krypt_char.decode():
                    key_krypt_char_result += f"'{i}',"
                key_krypt_char_result += "'\\0'}"

                result_string = f"unsigned char {encrypted_data}[]" + " = {" + ciphertext_format + ",'\\0'};\n" + \
                                f'int {sbox_len} = 256;\n' + \
                                f'unsigned char {key_s}[] = {key_krypt_char_result};\n' + \
                                f'unsigned char {s}[{sbox_len}];\n' + \
                                f'size_t {data_len} = strlen((char *){encrypted_data});\n' + \
                                f'size_t {key_len} = strlen((char *){key_s});\n' + \
                                f'arc4_init({s}, {key_s}, {key_len});\n' + \
                                f'arc4_crypt({encrypted_data}, {data_len}, {s});\n' + \
                                f'{code.replace('[','').replace(']','')}[sizeof({encrypted_data})];\n' + \
                                f'memcpy({code.split()[-1].replace('[','').replace(']','')}, {encrypted_data}, {data_len} + 1);\n'
                
                # print(result_string)

                file_text[number_line] = result_string                                  # Подменяем строку на зашифрованную

            number_line += 1

        #print(file_text)

        file_write(file, file_text)                                                     # Записываем обфуцированный код


def сompilation_final_executable_file():

    path_final_executable_file_c = path_project_obfuscate + "/main.c"

    try:
        command = f"gcc {script_dir}{path_final_executable_file_c} -o {script_dir}{path_payload_server}/main.exe"
        print(f"command: {command}")
        DEVNULL = open(os.devnull, 'wb')
        check_output(command, shell=True, stderr=DEVNULL, stdin=DEVNULL).decode('utf-8')
    except:
        return 1
    return 0


def obfuscator():

    print("-"*60)

    print(f"1. Copy the project to the directory {path_project_obfuscate}")
    try:
        shutil.rmtree(script_dir + path_project_obfuscate)                              # Удаляем копию проекта
    except FileNotFoundError:
        pass
    shutil.copytree(script_dir + path_project, script_dir + path_project_obfuscate)     # Снова копируем для новой обфускации

    print("-"*60)

    print("2. Get all files project")
    all_file_project = get_all_files_recursively(path_project_obfuscate)                # получаем все файлы проекта

    print("-"*60)

    print("3. Deleting comments")
    clearing_blank_lines_and_comments(all_file_project)                                 # Удаляем строки коментарии

    print("-"*60)

    print("4. String encryption:")
    cryptor_char(all_file_project)                                                      # Шифруем все char с с указывающим на шифрование постфиксом

    print("-"*60)

    print("5. Read string obfuscate in file string_obfuscate.txt")
    string_obfuscate = get_string_obfuscate()                                           # получаем строки из файла, которые будем обфуцировать
    # print(string_obfuscate)

    print("-"*60)

    print("6. Generate name obfuscate")
    string_obfuscate_replace = get_string_obfuscate_replace(string_obfuscate)           # получаем словарь: {исходная строка: обфуцированная строка}
    # print(string_obfuscate_replace)

    print("-"*60)

    print("7. Replace in file:")
    string_replace(all_file_project, string_obfuscate_replace)                          # заменяем строк в файлах 

    print("-"*60)

    print("7. Compiling execute file:")
    if сompilation_final_executable_file() == 1:
        print("Error сompilation final executable file. Exit...")
        exit(1)


def main():
    obfuscator()    

if __name__ == "__main__":
    main()