#include <iostream>
#include "crs.h"
using namespace std;
const int MAX_LOPSV = 10000;

struct MonHoc {
    string MAMH; string TENMH; 
    int STCLT ,STCTH; int height;
};

struct nodeMH {
    MonHoc mh;
    nodeMH *left, *right;
};

typedef nodeMH* treeMH;

struct SinhVien {
    string MASV, HO, TEN;
    string  PHAI, SODT, Email;
};

struct nodeSV {
    SinhVien sv;
    nodeSV *next;
};

typedef nodeSV* PTRSV;

struct LopSV  {
    string MALOP, TENLOP; 
    PTRSV FirstSV=NULL;
    PTRSV LastSV=NULL; //dễ thêm học sinh hơn (mới thêm)
};

struct DS_LOPSV {
 int n=0;
 LopSV *nodes[MAX_LOPSV];
};

struct DangKy {
    string MASV; float DIEM; 
    bool huydangky=false;
};
 
struct nodeDK {
    DangKy dk;
    nodeDK *next;
};
typedef nodeDK* PTRDK;

struct LopTinChi {
    int MALOPTC ;
    string MAMH;
    string NienKhoa;  
    int Hocky, Nhom,sosvmin, sosvmax;
    bool huylop = false;
    PTRDK dssvdk=NULL; 
 };
 
struct nodeLopTinChi {
    LopTinChi ltc;
    nodeLopTinChi *next;
};

typedef nodeDK* PTRLTC;

int tim_lop_theo_malop(const DS_LOPSV &dslopsv, const string &malop){
    for(int i = 0; i < dslopsv.n; i++)
        if(dslopsv.nodes[i]->MALOP == malop) return i;
    return -1;
}

bool tim_sv_theo_maso(const DS_LOPSV &dslopsv, string malop, const string maso){
    if(dslopsv.n < stoi(malop)) return false;
    int index = -1;
    for(int i = 0; i < dslopsv.n; i++){ if(dslopsv.nodes[i]->MALOP == malop){index = i; break;} }
    if(index == -1) return false;

    PTRSV p = dslopsv.nodes[index]->FirstSV;
    while(p){
        if(p->sv.MASV == maso) return true;
        p = p->next;
    }
    return false;
}

void nhap_lop_va_sinhvien(DS_LOPSV &dslopsv){
    cout << "Nhap ten lop: "; string tenlop; getline(cin,tenlop);
    while(!ten_lop_check(tenlop)) getline(cin,tenlop);
    dslopsv.nodes[dslopsv.n] = new LopSV();
    dslopsv.nodes[dslopsv.n]->MALOP = to_string(dslopsv.n);
    dslopsv.nodes[dslopsv.n]->TENLOP = tenlop;
    dslopsv.nodes[dslopsv.n]->FirstSV = nullptr;
    dslopsv.n++;
    while(true){
        string masv; cout <<"Nhap ma sinh vien (skip de out): "; getline(cin,masv);
        if(!maso_check(masv)) break; 
        
        while(tim_sv_theo_maso(dslopsv,dslopsv.nodes[dslopsv.n-1]->MALOP,masv)){
            cout <<"Ma sinh vien da ton tai! Nhap lai(skip de out): ";getline(cin, masv); 
            if(masv.empty()) break;
        } //kiểm tra mã sinh viên có bị trùng trong lớp không

        PTRSV node = new nodeSV();
        node->next = nullptr;

        node->sv.MASV = masv;
        string hosv; cout <<"Nhap ho sinh vien: "; getline(cin,hosv);
        while(!text_format(hosv)) getline(cin, hosv);
        node->sv.HO = hosv;
        string tensv; cout <<"Nhap ten sinh vien: "; getline(cin,tensv);
        while(!text_format(tensv)) getline(cin, tensv);
        node->sv.TEN = tensv;
        string phaisv; cout <<"Nhap phai sinh vien: "; getline(cin,phaisv);
        node->sv.PHAI = phaisv;
        while(phaisv.empty()){ cout <<"Ban chua nhap phai sinh vien! Nhap lai: "; getline(cin, phaisv);}
        string sodtsv; cout <<"Nhap so dien thoai sinh vien: "; getline(cin,sodtsv);
        while(!num_check(sodtsv)) getline(cin, sodtsv);
        node->sv.SODT = sodtsv;
        string emailsv; cout <<"Nhap email sinh vien: "; getline(cin,emailsv);
        while(emailsv.empty()){ cout <<"Ban chua nhap email sinh vien! Nhap lai: "; getline(cin, emailsv);} 
        node->sv.Email = emailsv;   
        
        LopSV* lop = dslopsv.nodes[dslopsv.n-1]; //da sua
        if(lop->FirstSV == nullptr){
            lop->FirstSV = lop->LastSV = node;
        } else{
            lop->LastSV->next = node;
            lop->LastSV = node;
        }
}
}

void xoa_sv(DS_LOPSV& dslopsv){
    cout << "Nhap ma lop(skip de out): "; string malop; getline(cin,malop);
    if(malop.empty()) return;
    int i = tim_lop_theo_malop(dslopsv,malop);
    while(i == -1 || dslopsv.nodes[i]->FirstSV == nullptr){
        if(i == -1) cout << "Khong tim thay ma lop! Nhap lai(skip de out): ";
        else cout << "Lop khong co hoc sinh de xoa! Nhap lai(skip de out): ";
        getline(cin, malop);
        if(malop.empty()) return;
        i = tim_lop_theo_malop(dslopsv, malop);
    }

    LopSV* lop = dslopsv.nodes[i];
    
    while(true){
    cout << "Nhap ma so sinh vien de xoa(skip de out): "; string masv; getline(cin,masv);
    if(masv.empty()) return;
    while(!tim_sv_theo_maso(dslopsv,lop->MALOP,masv)){
        cout <<"Khong tim thay sinh vien! Nhap lai(skip de out): ";
        getline(cin,masv);
        if(masv.empty()) return;
    }

    PTRSV p = lop->FirstSV;
    
    if(p->sv.MASV == masv){ //xóa node đầu
        lop->FirstSV = p->next;
        if(lop->FirstSV == nullptr) lop->LastSV = nullptr; //cập nhật lastSV (trường hợp sau khi xóa thì lớp rỗng)
        delete p;
    } else{
    while (p->next != nullptr && p->next->sv.MASV != masv) p = p->next;
    PTRSV temp = p->next;
    p->next = temp->next;
    if(p->next == nullptr) lop->LastSV = p; //cập nhật last (nếu xóa hs ở cuối )
    delete temp;
    }
    cout << "Da xoa sinh vien "<< masv << " khoi lop " << malop <<endl;
}
}

void hieu_chinh_thong_tin_sinhvien(DS_LOPSV &dslopsv){
    cout << "Nhap ma lop(skip de out): "; string malop; getline(cin,malop);
    if(!maso_check(malop)) return;
    int i = tim_lop_theo_malop(dslopsv,malop);
    while(i == -1 || dslopsv.nodes[i]->FirstSV == nullptr){
        if(i == -1) cout << "Khong tim thay ma lop! Nhap lai(skip de out): ";
        else cout << "Lop khong co hoc sinh de chinh sua! Nhap lai(skip de out): ";
        getline(cin, malop);
        if(malop.empty()) return;
        i = tim_lop_theo_malop(dslopsv, malop);
    }
    
    LopSV* lop = dslopsv.nodes[i];
    
    while(true){
    cout << "Nhap ma so sinh vien de hieu chinh(skip de out): "; string masv; getline(cin,masv);
    if(masv.empty()) return;
    while(!tim_sv_theo_maso(dslopsv,lop->MALOP,masv)){
        cout <<"Khong tim thay sinh vien! Nhap lai(skip de out): ";
        getline(cin,masv);
        if(masv.empty()) return;
    }
    PTRSV p = lop->FirstSV;
    while( p != nullptr && p->sv.MASV != masv) p = p->next;
    string option;
    cout << "Nhap thong tin muon hieu chinh (Ma/Ho/Ten/Phai/SDT/Email/All/* de out): " ;getline(cin,option);
        if(option == "Ma"){
            cout << "Ma sinh vien cu: " << p->sv.MASV <<"\nNhap ma sinh vien moi(skip de out): "; getline(cin,masv);
            if(masv.empty()) return;
            
            while(tim_sv_theo_maso(dslopsv,lop->MALOP,masv)){
                cout <<"Ma sinh vien da ton tai! Nhap lai(skip de out): ";getline(cin, masv); 
                if(masv.empty()) return;
            } 
            p->sv.MASV = masv;
        }    
        else if(option == "Ho"){
            string hosv; cout <<"Ho cu cua sinh vien : "<< p->sv.HO <<"\nNhap ho sinh vien moi: "; getline(cin,hosv);
            while(!text_format(hosv)) getline(cin, hosv);
            p->sv.HO = hosv;
        }
        else if(option == "Ten"){
            string tensv; cout <<"Ten cu cua sinh vien: "<<p->sv.TEN<<"\nNhap ten sinh vien: "; getline(cin,tensv);
            while(!text_format(tensv)) getline(cin, tensv);
            p->sv.TEN = tensv;
        }    
        else if(option == "Phai"){
            string phaisv; cout <<"Phai cu cua sinh vien: " << p->sv.PHAI <<"\nNhap phai sinh vien: "; getline(cin,phaisv);
            while(phaisv.empty()){ cout <<"Ban chua nhap phai sinh vien moi! Nhap lai: "; getline(cin, phaisv);}
            p->sv.PHAI = phaisv;
        }else if(option == "SDT"){
            string sodtsv; cout <<"So dien thoai cu cua sinh vien: "<<p->sv.SODT<<"\nNhap so dien thoai sinh vien: "; getline(cin,sodtsv);
            while(!num_check(sodtsv)) getline(cin, sodtsv);
            p->sv.SODT = sodtsv;
        }else if(option == "Email"){
            string emailsv; cout <<"Email cu cua sinh vien: "<< p->sv.Email<<"\nNhap email sinh vien: "; getline(cin,emailsv);
            while(emailsv.empty()){ cout <<"Ban chua nhap email sinh vien moi! Nhap lai: "; getline(cin, emailsv);} 
            p->sv.Email = emailsv;
        }else if(option == "All"){
            cout <<"Ma sinh vien cu: " << p->sv.MASV <<"\nNhap ma sinh vien moi(skip de out): "; getline(cin,masv);
            if(masv.empty()) return;
            
            while(tim_sv_theo_maso(dslopsv,lop->MALOP,masv)){
                cout <<"Ma sinh vien da ton tai! Nhap lai(skip de out): ";getline(cin, masv); 
                if(!maso_check(masv)) return;
            } 
            p->sv.MASV = masv;

            string hosv; cout <<"Ho cu cua sinh vien : "<< p->sv.HO <<"\nNhap ho sinh vien moi: "; getline(cin,hosv);
            while(!text_format(hosv)) getline(cin, hosv);
            p->sv.HO = hosv;
            string tensv; cout <<"Ten cu cua sinh vien: "<<p->sv.TEN<<"\nNhap ten sinh vien: "; getline(cin,tensv);
            while(!text_format(tensv)) getline(cin, tensv);
            p->sv.TEN = text_format(tensv);
            string phaisv; cout <<"Phai cu cua sinh vien: " << p->sv.PHAI <<"\nNhap phai sinh vien: "; getline(cin,phaisv);
            while(phaisv.empty()){ cout <<"Ban chua nhap phai sinh vien moi! Nhap lai: "; getline(cin, phaisv);}
            p->sv.PHAI = phaisv;
            string sodtsv; cout <<"So dien thoai cu cua sinh vien: "<<p->sv.SODT<<"\nNhap so dien thoai sinh vien: "; getline(cin,sodtsv);
            while(!num_check(sodtsv)) getline(cin, sodtsv);
            p->sv.SODT = sodtsv;
            string emailsv; cout <<"Email cu cua sinh vien: "<< p->sv.Email<<"\nNhap email sinh vien: "; getline(cin,emailsv);
            while(emailsv.empty()){ cout <<"Ban chua nhap email sinh vien moi! Nhap lai: "; getline(cin, emailsv);} 
            p->sv.Email = emailsv;
        }else if(option == "*") return;
        cout <<"Da hieu chinh thong tin cua sinh vien co ma so " << p->sv.MASV <<" thanh cong" <<endl;
    }
}

int main(){
    PTRLTC dsltc=NULL;
    treeMH dsmh=NULL;
    DS_LOPSV dslopsv;
    nhap_lop_va_sinhvien(dslopsv);
    //xoa_sv(dslopsv);
    hieu_chinh_thong_tin_sinhvien(dslopsv);
}