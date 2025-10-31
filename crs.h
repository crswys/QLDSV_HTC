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
        if(!isalpha(c) && !isblank(c)){cout << "Loi khong duoc nhap ki tu dac biet! Nhap lai: "; return false;}
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
    if(result.empty()){cout <<"Ki tu khong hop le! Nhap lai: "; return false;}
    if(result.back() == ' ') result.pop_back();
    str = result;
    cout << "Tu dong sua: ( "<< result <<" )\n";
    return true;
}

bool num_check(string &num){
    if(!empty_check(num)) return false;
    string result;
    for(char c : num){
        if(isdigit(c)) result += c;
        else if(!isblank(c)){
            cout << "So khong hop le (loi ki tu)! Nhap lai: ";
            return false;
        }
    }
    if(result.empty()){
        cout << "So khong hop le (khong co chu so nao)! Nhap lai: ";
        return false;
    }
    num = result;
    if(num.length() != 10){
        cout << "So khong hop le (phai dung 10 chu so)! Nhap lai: ";
        return false;
    }
    if(num == "0000000000"){
        cout << "So khong hop le (khong the tat ca la 0)! Nhap lai: ";
        return false;
    }
    if(num[0] != '0'){
        cout << "So khong hop le (can bat dau la chu so 0)! Nhap lai: ";
        return false;
    }
    return true;
}

bool maso_check(string &maso){
    if(maso.empty()) return false;
    string result;
    for(char c: maso){
        if(isdigit(c)) result += c;
        else if(!isblank(c)){
            cout << "Ma so sinh vien khong hop le! Nhap lai: ";
            return false;
        }
    }
    if(result.empty()){
        cout << "Ma so sinh vien khong hop le! Nhap lai: ";
        return false;
    }
    maso = result;
    return true;
}


int int_check(const string &str){
    if(!empty_check(str)) return -1;
    for(char c : str){
        if(!isdigit(c)) return -1;
    }

    return stoi(str);
}

float float_check(const string &str){
    if(!empty_check(str)) return -1;
    bool dot=false;
    for(char c:str){
        if(isdigit(c)) continue;
        if(c=='.'&&!dot){
            dot=true;
            continue;
        }
        cout<<"Diem khong hop le! Nhap lai: ";
        return -1;
    }
    float diem;
    try{
        diem=stof(str);
    }catch(...){
        cout << "Diem khong hop le! Nhap lai: ";
        return -1;
    }
    if(diem < 0.0 || diem > 10.0){
        cout <<"Diem phai tu 0 den 10! Nhap lai: ";
        return -1;
    }
    return diem;
}