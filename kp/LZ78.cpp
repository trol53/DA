#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

struct Node{
    int pose;
    char next;
};

Node MakeNode(int p, char n){
    Node ans;
    ans.pose = p;
    ans.next = n;
    return ans;
}

class LZ78{
    public:

    LZ78(){
        text = "";
    }

    LZ78(std::string &txt){
        text = txt;
    }

    LZ78(std::vector <std::vector<Node>> &decode){
        res = decode;
    }

    std::vector <std::vector<Node>> Compression(){
        std::string buffer = "";
        int level = 0;
        res.resize(1);
        for (int i = 0; i < text.size(); i++){
            if (dict.count(buffer + text[i]) != 0){
                buffer += text[i];
            } else {
                res[level].push_back(MakeNode(dict[buffer], text[i]));
                dict[buffer + text[i]] = dict.size();
                buffer = "";
                if (dict.size() >= max_dict_size){
                    dict.clear();
                    level++;
                    res.resize(level + 1);
                }
            }
        }
        if (buffer != ""){
            char last = buffer.back();
            buffer.pop_back();
            res[level].push_back(MakeNode(dict[buffer], last));
        }
        return res;
    }

    std::string Decompression(){
        std::vector <std::string> _dict;
        _dict.push_back("");
        text = "";
        for (int i = 0; i < res.size(); i++){
            for (auto node : res[i]){
                text += (_dict[node.pose] + node.next);
                _dict.push_back(_dict[node.pose] + node.next);
            }
            _dict.clear();
            _dict.push_back("");
        }
        return text;
    }

    void ReadFileText(std::string file_name){
        std::ifstream file(file_name);
        text.clear();
        std::stringstream ss;
        ss << file.rdbuf();
        text = ss.str();
        file.close();
    }

    void ReadFileCode(std::string file_name){
        std::ifstream file(file_name);
        std::vector<Node> tmp;
        std::string s;
        while (getline(file, s)){
            int count = 0;
            for (int i = 0; i < s.size(); i++){
                if (isdigit(s[i])){
                    count *= 10;
                    count += (s[i] - '0');
                    continue;
                }
                if (s[i] != ' '){
                    tmp.push_back(MakeNode(count, s[i]));
                    count = 0;
                    continue;
                }
            }
            res.push_back(tmp);
            tmp.clear();
        }
        file.close();
    }

    void WriteFileText(std::string file_name){
        std::ofstream file(file_name);
        file << text;
        file.close();
    }

    void WriteFileCode(std::string file_name){
        std::ofstream file(file_name);
        for (auto vec : res){
            for (int i = 0; i < vec.size(); i++){
                file << vec[i].pose << " " << vec[i].next << " ";
            }
            file << '\n';
        }
        file.close();
    }

    void PrintCode(){
        for (auto vec : res){
            for (int i = 0; i < vec.size(); i++){
                std::cout << vec[i].pose << " " << vec[i].next << '\n';
            }
        }
    }

    void PrintText(){
        for (int i = 0; i < text.size(); i++){
            std::cout << text[i];
        }
        std::cout << '\n';
    }

    int ZipMemory(){
        int sum = 0;
        for (auto vec : res){
            sum += vec.size();
        }
        return sum * 5;
    }

    private:

    std::map<std::string, int> dict;
    long long max_dict_size = 10000;
    std::string text;
    std::vector <std::vector<Node>> res;
};

int main(){
    // std::string text;
    // getline(std::cin, text);
    LZ78 lz;
    lz.ReadFileText("data4.txt");
    unsigned int start_comp = clock();
    std::vector <std::vector<Node>> res = lz.Compression();
    unsigned int finish_comp = clock();
    //lz.PrintCode();
    unsigned start_decomp = clock();
    std::string ans = lz.Decompression();
    unsigned finish_decomp = clock(); 
    lz.WriteFileText("output4.txt");
    //lz.WriteFileCode("output3code.txt");
    //lz.PrintText();
    std::cout << "zip memory:" << lz.ZipMemory() << " byte\n" << "text memory:" << ans.size() << " byte\n";
    std::cout << "zip time:" << finish_comp - start_comp << '\n' << "decompression time:" << finish_decomp - start_decomp << '\n';     
}
