#include <iostream>
#include <string>
#include <fstream>

int PATTSIZE = 50000;
int TEXTSIZE = 550000;

int main(){
    std::ofstream f;
    f.open("input.txt");
    for (int i = 0; i < PATTSIZE; i++){
        f << (char)(rand() % 26 + 'a');
    }
    f << '\n';
    for (int i = 0; i < TEXTSIZE; i++){
        f << (char)(rand() % 26 + 'a');
    }
    f.close();
}