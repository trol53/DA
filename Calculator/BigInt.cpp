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
            tmp = tmp * 10 + (buff[j] - '0');
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

    BigInteger operator=(const BigInteger rhs){
        this->base_number_system = rhs.base_number_system;
        this->length_system = rhs.length_system;
        this->values = rhs.values;
        return *this;
    }

    BigInteger operator+(BigInteger rhs){
        BigInteger ans();
        long long tmp(0);
        long long sum;
        for (long long i = 0; i < this->values.size(); i++){
            sum = this->values[i] + rhs.values[i];
            ans.values.push_back((sum + tmp) % base_number_system);
            tmp = (sum + tmp) / base_number_system;
        }
        ans.values.push_back(tmp);
        return ans;
    }

    void Print(){
        std::cout << values[values.size() - 1] << " ";
        for (long long i = values.size() - 2; i >= 0; i--){
            long long j = 0;
            long long size = Int_Length(values[i]);
            while (j + size < length_system){
                std::cout << 0;
                j++;
            }
            std::cout << values[i] << " ";
        }
        std::cout << '\n';
    }

    private:

    long long Int_Length(long long val){
        long long i = 0;
        if (val == 0)
            return 1;
        while (val > 0){
            val /= 10;
            i++;
        }
        return i;
    }
    
    long long base_number_system;
    long long length_system;
    std::vector<long long> values;
};

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    BigInteger a(1000), b(1000);
    a.Input();
    b.Input();
    a.Print();
    b.Print();
    BigInteger ans = a + b;
    ans.Print();
}