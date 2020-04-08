#include <iostream>
#include <vector>
#include <fstream>

class BigInteger {
    public: 

    BigInteger(long long system){
        base_number_system = system;
        long long i = 0;
        while (system > 0){
            system /= 10;
            i++;
        }
        i--;
        length_system = i;
    }

    BigInteger(){
        base_number_system = 1000000;
        length_system = 6;
    }

    void Input(){
        char c;
        long long check = 0, tmp = 0;
        std::string buff;
        std::cin >> buff;
        long long i = buff.size() - 1;
        for (; i >= length_system; i -= length_system){
            for (long long j = i - length_system + 1; j <= i; j++){
                tmp = tmp * 10 + (buff[j] - '0');
            }
            values.push_back(tmp);
            tmp = 0;
        }
        for (long long j = 0; j <= i; j++){
            tmp = tmp * 10 + (buff[i] - '0');
        }
        values.push_back(tmp);
    }

    void Int_to_Big(long long val){
        if (values.size() > 0){
            values.clear();
        }
        long long tmp = 0;
        while (val >= base_number_system){
            values.push_back(val % base_number_system);
            val /= base_number_system;
        }
        values.push_back(val);
    }

    void Print(){
        for (long long i = values.size() - 1; i >= 0; i--){
            std::cout << values[i];
        }
        std::cout << '\n';
    }

    private:
    
    long long base_number_system;
    long long length_system;
    std::vector<long long> values;
};

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    BigInteger *a = new BigInteger(1000);
    a->Input();
    a->Print();
}