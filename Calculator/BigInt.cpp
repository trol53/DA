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
        // while (c = getchar() != '\n'){
        //     tmp = tmp *
        //     if (tmp >= base_number_system){
        //         values.push_back(tmp % base_number_system);
        //         tmp /= base_number_system;
        //     }
        // }
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

    BigInteger operator=(const BigInteger &rhs){
        this->base_number_system = rhs.base_number_system;
        this->length_system = rhs.length_system;
        this->values = rhs.values;
        return *this;
    }

    bool operator==(BigInteger &rhs){
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

    bool operator>(BigInteger &rhs){
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

    bool operator<(BigInteger &rhs){
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

    bool operator<=(BigInteger &rhs){
        return *this < rhs || *this == rhs;
    }

    bool operator>=(BigInteger &rhs){
        return *this > rhs || *this == rhs;
    }    

    BigInteger operator+(BigInteger &rhs){
        BigInteger ans;
        ans = Sum(*this, rhs);
        return ans;
        
    }

    BigInteger operator-(BigInteger &rhs){
        BigInteger ans;
        if (*this >= rhs){
            ans = Subtruction(*this, rhs);
        } else{
            ans = Subtruction(rhs, *this);
            ans.check = '-';
        }
        return ans;
    }

    void operator*=(long long rhs){
        long long transfer = 0, mult;
        for (long long i = 0; i < this->values.size(); i++){
            mult = this->values[i] * rhs + transfer;
            this->values[i] = mult % base_number_system;
            transfer = mult / base_number_system;
        }
        if (transfer > 0){
            this->values.push_back(transfer % base_number_system);
        } 
    }

    BigInteger operator*(BigInteger &rhs){
        return Karatsuba(*this, rhs); 
    }

    BigInteger BinPow(BigInteger &a, BigInteger &x){
    if (x == 0)
        return 1;
    if (x % 2 != 0){
        return BinPow(a, x - 1) * a;
    } else {
        long long tmp = BinPow(a, x / 2);
        return tmp * tmp;
    }
}

    BigInteger operator*(long long rhs){
        BigInteger ans(base_number_system);
        ans.values.resize(this->values.size());
        long long transfer = 0, mult;
        for (long long i = 0; i < this->values.size(); i++){
            mult = this->values[i] * rhs + transfer;
            ans.values[i] = mult % base_number_system;
            transfer = mult / base_number_system;
        }
        if (transfer > 0){
            ans.values.push_back(transfer % base_number_system);
        } 
        return ans;
    }

    
    void Print(){
        if (values[0] == -1 || values.empty()){
            return;
        }
        if (this->check == '-')
            std::cout << '-';
        std::cout << values[values.size() - 1] << " ";
        for (long long i = values.size() - 2; i >= 0; i--){
            long long j = 0;
            long long size = IntLength(values[i]);
            while (j + size < length_system){
                std::cout << 0;
                j++;
            }
            std::cout << values[i] << " ";
        }
        std::cout << '\n';
    }

    BigInteger operator/(long long rhs){
        
    }

    private:

    BigInteger Sum(BigInteger &lhs, BigInteger &rhs){
        BigInteger ans(rhs.base_number_system);
        long long tmp(0);
        long long sum, i;

        for (i = 0; i < std::min(lhs.values.size(), rhs.values.size()); i++){
            sum = lhs.values[i] + rhs.values[i] + tmp;
            ans.values.push_back(sum % base_number_system);
            tmp = sum / base_number_system;
        }
        if (lhs.values.size() > i){
            for (; i < lhs.values.size(); i++){
                ans.values.push_back((lhs.values[i] + tmp) % base_number_system);
                tmp = (lhs.values[i] + tmp) / base_number_system;
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
        return ans;
    }

    BigInteger Subtruction(BigInteger &lhs, BigInteger &rhs){
        BigInteger ans(lhs.base_number_system);
        if (lhs < rhs){
            std::cout << "Error\n";
            ans.values.push_back(-1);
            return ans;
        }
        if (lhs == rhs){
            ans.values.push_back(0);
            return ans;
        }
        long long transfer = 0, diff, i;
        for (i = 0; i < rhs.values.size(); i++){        
            diff = lhs.values[i] - rhs.values[i] - transfer;
            if (diff < 0){
                diff += base_number_system;
                transfer = 1;
                ans.values.push_back(diff);
                continue;
            }
            ans.values.push_back(diff);
            transfer = 0;
        }

        for (; i < lhs.values.size(); i++){
            diff = lhs.values[i] - transfer;
            if (diff < 0){
                diff += base_number_system;
                transfer = 1;
                ans.values.push_back(diff);
                continue;
            }
            transfer = 0;
            ans.values.push_back(diff);

        }
        return ans;
    }

    void Normalize(BigInteger &val){
        long long i = val.values.size() - 1;
        while (val.values[i] == 0 && i > 0){
            i--;
            val.values.pop_back();
        }
    }

    BigInteger BaseMult(BigInteger &lhs ,BigInteger &rhs){
        BigInteger ans(lhs.base_number_system);
        ans.values.resize(lhs.values.size() + rhs.values.size(), 0);
        long long i, j, transfer = 0;
        for (i = 0; i < lhs.values.size(); i++){
            transfer = 0;
            for (j = 0; j < rhs.values.size(); j++){
                ans.values[i + j] += lhs.values[i] * rhs.values[j] + transfer;                
                transfer = ans.values[i + j] / ans.base_number_system;
                ans.values[i + j] %= ans.base_number_system;
            }
            if (transfer != 0){
                ans.values[i + rhs.values.size()] = transfer;
            }
        }
        if (transfer != 0){
            ans.values[i + rhs.values.size()] = transfer;
        }
        Normalize(ans);
        return ans;
    }

    BigInteger Karatsuba(BigInteger &lhs, BigInteger &rhs){
        long long size = std::max(lhs.values.size(), rhs.values.size());
        if (size == 1)
            return BaseMult(lhs, rhs);
        SetLength(lhs, rhs, size);
        if (size < min_length_karatsuba){
            return BaseMult(lhs, rhs);
        }
        BigInteger ans(lhs.base_number_system);
        ans.values.resize(size * 2);
        long long mid = size / 2;
        BigInteger xl(lhs.base_number_system);
        xl.values.assign(lhs.values.begin(), lhs.values.begin() + mid);
        BigInteger xr(lhs.base_number_system);
        xr.values.assign(lhs.values.begin() + mid, lhs.values.end());
        BigInteger yl(lhs.base_number_system);
        yl.values.assign(rhs.values.begin(), rhs.values.begin() + mid);
        BigInteger yr(lhs.base_number_system);
        yr.values.assign(rhs.values.begin() + mid, rhs.values.end());
        BigInteger x = xl + xr;
        BigInteger y = yl + yr;
        BigInteger p1 = Karatsuba(xl, yl);
        BigInteger p2 = Karatsuba(xr, yr);
        BigInteger p3 = Karatsuba(x, y);
        BigInteger tmp1 = p1 + p2;
        BigInteger tmp2 = p3 - tmp1;
        if (tmp2.check == '-')
            std::cout << "kek\n";
        tmp2 *= BinPow(base_number_system, size / 2);
        return p1 * BinPow(base_number_system, size) + tmp2 + p2;


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
    long long min_length_karatsuba = 4;
    char check = '+';
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
    BigInteger ans = a * b;

    ans.Print();
}