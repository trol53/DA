#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

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

    BigInteger(int n, int system){
        base_number_system = system;
        long long i = 0;
        while (system > 0){
            system /= 10;
            i++;
        }
        i--;
        length_system = i;
        if (n < base_number_system)
            values.push_back(n);
        else {
            for(; n; n /= base_number_system)
                values.push_back(n % base_number_system);
        }
    }

    BigInteger(){
        base_number_system = 1000000;
        length_system = 6;
    }

    BigInteger(long long system, std::string &val){
        this->base_number_system = system;
        long long i = 0;
        while (system > 0){
            system /= 10;
            i++;
        }
        i--;
        length_system = i;
        this->Input(val);
    }

    void Input(std::string &buff){
        values.clear();
        long long tmp = 0;
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
        this->Normalize();
    }


    BigInteger Int_to_Big(long long val, long long system){
        BigInteger ans(system);
        while (val >= system){
            ans.values.push_back(val % system);
            val /= system;
        }
        ans.values.push_back(val);
        return ans;
    }


    bool operator==(const BigInteger &rhs)const{
        if (this->values.size() != rhs.values.size()){
            return false;
        } else {
            for (long long i = this->values.size() - 1; i >= 0; i--){
                if (this->values[i] != rhs.values[i])
                    return false;
            }
            return true;
        }
    }

    bool operator==(long long rhs){
        if (this->values.size() > 1){
            return false;
        } else if (this->values[0] == rhs){
            return true;
        } else {
            return false;
        }
    } 

    bool operator!=(long long rhs){
        return !((*this) == rhs);
    }

    bool operator>(const BigInteger &rhs)const{
        if (this->values.size() > rhs.values.size()){
            return true;
        } else if (this->values.size() < rhs.values.size()){
            return false;
        } else {
            for (long long i = this->values.size() - 1; i >= 0; i--){
                if (this->values[i] < rhs.values[i])
                    return false;
                if (this->values[i] > rhs.values[i])
                    return true;
            }
            return false;
        }
    }

    bool operator<(const BigInteger &rhs)const{
        if (this->values.size() < rhs.values.size()){
            return true;
        } else if (this->values.size() > rhs.values.size()){
            return false;
        } else {
            for (long long i = this->values.size() - 1; i >= 0; i--){
                if (this->values[i] > rhs.values[i])
                    return false;
                if (this->values[i] < rhs.values[i])
                    return true;
            }
            return false;
        }
    }


    bool operator<=(const BigInteger &rhs) const{
        return *this < rhs || *this == rhs;
    }

    bool operator>=(const BigInteger &rhs) const{
        return *this > rhs || *this == rhs;
    }   

    BigInteger operator+(const BigInteger &rhs){
        BigInteger ans(rhs.base_number_system);
        long long tmp(0);
        long long sum, i;

        for (i = 0; i < std::min(this->values.size(), rhs.values.size()); i++){
            sum = this->values[i] + rhs.values[i] + tmp;
            ans.values.push_back(sum % base_number_system);
            tmp = sum / base_number_system;
        }
        if (this->values.size() > i){
            for (; i < this->values.size(); i++){
                ans.values.push_back((this->values[i] + tmp) % base_number_system);
                tmp = (this->values[i] + tmp) / base_number_system;
            }
            if (tmp != 0)
                ans.values.push_back(tmp);
        } else if (rhs.values.size() > i){
            for (; i < rhs.values.size(); i++){
                ans.values.push_back((rhs.values[i] + tmp) % base_number_system);
                tmp = (rhs.values[i] + tmp) / base_number_system;
            }
            if (tmp != 0)
                ans.values.push_back(tmp);
        } else {
            if (tmp != 0)
                ans.values.push_back(tmp);
        }
        ans.Normalize();
        return ans;
    }

    BigInteger operator-(const BigInteger &rhs){
        BigInteger ans(this->base_number_system);
        if (*this < rhs){
            std::cout << "Error\n";
            ans.values.push_back(-1);
            return ans;
        }
        int carry = 0;
        for (int i = 0; i < values.size() || carry; ++i) {
            int aa = i < values.size() ? values[i] : 0;
            int bb = i < rhs.values.size() ? rhs.values[i] : 0;
            ans.values.push_back(aa - carry - bb);
            carry = ans.values.back() < 0;
            if (carry)
                ans.values.back() += base_number_system;
        }
        ans.Normalize();
        return ans;
    }


    BigInteger operator*(const BigInteger &rhs)const{
        BigInteger ans(this->base_number_system);
        ans.values.resize(this->values.size() + rhs.values.size());
        for (int i = 0; i < values.size(); ++i) {
            int carry = 0;
            for (int j = 0; j < rhs.values.size() || carry; ++j) {
                int bb = j < rhs.values.size() ? rhs.values[j] : 0;
               ans.values[i + j] += values[i] * bb + carry;
               carry = ans.values[i + j] / base_number_system;
               ans.values[i + j] -= carry * base_number_system;
            }
        }
        ans.Normalize();
        return ans; 
    }
    
    void Print(){
        if (values[0] == -1 || values.empty()){
            return;
        }
        if (this->check == '-')
            std::cout << '-';
        std::cout << values[values.size() - 1];
        for (long long i = values.size() - 2; i >= 0; i--){
            long long j = 0;
            long long size = IntLength(values[i]);
            while (j + size < length_system){
                std::cout << 0;
                j++;
            }
            std::cout << values[i];
        }
        std::cout << '\n';
    }


    BigInteger Bin_Pow(BigInteger r){
    BigInteger res(1, base_number_system);
    while (r > 0) {
        if (r.values[0] % 2)
            res = res * (*this);
        (*this) = (*this) * (*this);
        r = r / BigInteger(2, base_number_system);
    }
    return res;
}

    BigInteger operator/(const BigInteger &obj)
{
    BigInteger cv = BigInteger(0, this->base_number_system);
    BigInteger res(base_number_system);
    res.values.resize(values.size());
    for (int i = (int) values.size() - 1; i >= 0; --i) {
        cv.values.insert(cv.values.begin(), values[i]);
        if (!cv.values.back())
            cv.values.pop_back();
        int x = 0, l = 0, r = base_number_system;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger cur = obj * BigInteger(m, base_number_system);
            if (cur <= cv) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        res.values[i] = x;
        cv = cv - obj * BigInteger(x, base_number_system);
    }
    res.Normalize();
    return res;
}

    private:


    void Normalize(){
        while (values.size() > 1 && !values.back())
            values.pop_back();
    }


    void SetLength(BigInteger &lhs, BigInteger &rhs, long long len){
        if (len % 2 != 0)
            len++;
        lhs.values.resize(len, 0);
        rhs.values.resize(len, 0);
        return;
    }


    long long IntLength(long long val){
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
    char check = '+';
    std::vector<long long> values;
};

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    std::string left, right;
    char oper;
    while (std::cin >> left >> right >> oper){
        BigInteger a(100000, left);
        BigInteger b(100000, right);
        if (oper == '+'){
            BigInteger ans = a + b;
            ans.Print();
        } else if (oper == '-'){
            BigInteger ans = a - b;
            if (ans != -1){
                ans.Print();
            }
        } else if (oper == '*'){
            BigInteger ans = a * b;
            ans.Print();
        } else if (oper == '/'){
            if (b == 0){
                std::cout << "Error\n";
            } else {
                BigInteger ans = a / b;
                ans.Print();
            }
        } else if (oper == '^'){
            if (b == 0 && a == 0){
                std::cout << "Error\n";
            } else {
                BigInteger ans = a.Bin_Pow(b);
                ans.Print();
            }
        } else if (oper == '>'){
            if (a > b){
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (oper == '<'){
            if (a < b){
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (oper == '='){
            if (a == b){
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        }
    }
    
}