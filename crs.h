#include <iostream>
#include <cctype>
using namespace std;

bool empty_check(const string &str){
    if(str.empty()) {
        cout << "Ban chua nhap gi het! Nhap lai: ";
        return false;
    }
    return true;
}

bool text_format(string &str){
    if(!empty_check(str)) return false;
    string result;
    bool newWord = true;
    for(char c : str){
        if(isalpha(c)) {
            result += newWord ? toupper(c) : tolower(c);
            newWord = false;
        } else if (isspace(c)){
            if(!result.empty() && result.back() != ' '){ 
            result += ' ';
            newWord = true;
            }
        }
    }
    if(str != result){ 
        cout << "Tu dong sua: ( " << result << " )\n";
        str = result;
    }
    return true;
}

bool num_check(string &num){
     if(!empty_check(num)) return false;
    string result;
    for(char c : num){
        if(isdigit(c)) result += c;
        else if(!isblank(c)){
        cout << "So khong hop le (loi ki tu)! Nhap lai: ";
        return false;}
    }
    if(result.length() != 10){
        cout << "So khong hop le (khong du 10 chu so)! Nhap lai: ";
        return false;
    }
    if(result[0] != '0'){
        cout << "So khong hop le (can bat dau la chu so 0)! Nhap lai: ";
        return false;
    }
    if(num != result){
        cout << "Tu dong sua: ( " << result << " )\n"; 
        num = result;
    }
    return true;
}

bool maso_check(string &maso){
    if(maso.empty()) return false;
    string result;
    for(char c: maso){
        if(isdigit(c)) result += c;
    }
    if(maso != result){
        cout << "Tu dong sua: ( " << result << " )\n";
        maso = result;
    }
    return true;
}

bool ten_lop_check(string &tenlop){
    if(!empty_check(tenlop)) return false;
    if(!isupper(tenlop[0])){
        tenlop[0] = toupper(tenlop[0]);
        cout << "Tu dong sua: ( " << tenlop << " )\n";
    }
    return true;
}