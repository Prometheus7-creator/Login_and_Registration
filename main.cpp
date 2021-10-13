#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <openssl/sha.h>

#define SHA512_DIGEST_LENGTH 64


class User {
    private:
        std::string name, password;
    public:
        User () {
            std::string username, password;
                
            std::cout << "Username: ";
            std::cin >> username;
            std::cin.ignore();
            password = getpass("Password : ");                       //Turn off echo (terminal POSIX) and take input
            this->setNameAndPassword(username, sha512(password));
        }
        void setNameAndPassword(std::string username, std::string password) {
            this->name = username;
            this->password = password;
        }
        std::string getName() {
            return name;
        }
        std::string getPassword() {
            return password;
        }

        //Encrypt password string with sha-512 hash
        std::string sha512 (const std::string password) {
            unsigned char encryptedPassword[SHA512_DIGEST_LENGTH] = {0};
            SHA512 ((const unsigned char*)password.c_str(), password.size(), encryptedPassword);
            std::stringstream ss;
            for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)encryptedPassword[i];

            }
            return ss.str();
        }
};


void handleLogin (const std::unordered_map<std::string, std::string>& dataTable) {

    std::cout << "Login with your credentials" << std::endl;
    User current;
    
    if (dataTable.find(current.getName()) != dataTable.end() && dataTable.at(current.getName()) == current.getPassword()) {
        std::cout << "Success" << std::endl;
    }
    else {
        std::cout << "Invalid User" << std::endl;
    }
}

void handleRegister (std::unordered_map<std::string, std::string>& dataTable) {
    User current;

    while ((dataTable.find(current.getName()) != dataTable.end())) {
        system("clear || cls");
        std::cout << "Username already exists!\nTry again." << std::endl;
        current = *(new User);
    }
    std::ofstream data;
    data.open("data.txt", std::ios::app);
    data << current.getName() << std::endl;
    data << current.getPassword() << std::endl;
    data.close();

    dataTable[current.getName()] = current.getPassword();
}

int main() {
    system("clear || cls");   
    int choice = 0;

    std::cout << "Welcome!" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "Enter your choice: ";
    
    try {
        std::cin >> choice;

        if (choice != 1 && choice != 2) {
            throw "Invalid choice!";
        }
    }
    catch (const char* e) {
        std::cerr << e << std::endl;
        exit (EXIT_FAILURE);
    }

    //Load credentials in map
    std::ifstream data("data.txt");
    std::unordered_map<std::string, std::string> dataTable;
    
    if (data.is_open()) {

        while (data) {
            std::string tmpName, tmpPassword;
            getline(data, tmpName);
            getline(data, tmpPassword);
            dataTable[tmpName] = tmpPassword;
        }

        data.close();
    }
    else {
        std::cout << "Can't open database!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (choice == 2) {
        system("clear || cls");
        handleRegister(dataTable);
    }

    system("clear || cls");
    handleLogin(dataTable);
    std::cout << "Press \"Enter\" to exit: ";
    getchar();
    system("clear || cls");
    return 0;
}
