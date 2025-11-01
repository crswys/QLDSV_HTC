#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>
#include "crs.h"
using namespace std;
const int MAX_LOPSV = 10000;

// MAMH | TENMH | STCLT | STCTH
struct MonHoc { // thong tin mon hoc
    string MAMH; string TENMH; 
    int STCLT ,STCTH; int height;
};

struct nodeMH { // node cua mon hoc
    MonHoc mh;                          
    nodeMH *left, *right;
};

typedef nodeMH* treeMH; // node mon hoc

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
};//thông tin sinh viên
 
struct nodeDK {
    DangKy dk;
    nodeDK *next;
};// danh sách sinh viên
typedef nodeDK* PTRDK; // MASV | HO | TEN | PHAI | SODT | EMAIL

struct LopTinChi { // MALOPTC | MAMH | NienKhoa | Hocky | Nhom | sosvmin | sosvmax 
    int MALOPTC ;
    string MAMH;
    string NienKhoa;  
    int Hocky, Nhom,sosvmin, sosvmax;
    bool huylop = false;
    PTRDK dssvdk=NULL; 
 }; //lớp tính chỉ

struct nodeLopTinChi {
    LopTinChi ltc;
    nodeLopTinChi *next;
}; //danh sách lớp tính chỉ

typedef nodeLopTinChi* PTRLTC; 

//------------------------------- search mon hoc--------------------------------------------
bool search_theo_ten_mon_hoc_BST(treeMH dsmh,string TENMH){
    if(!dsmh) return false;
    if(dsmh->mh.TENMH == TENMH) return true;
    if(TENMH < dsmh->mh.TENMH) return search_theo_ten_mon_hoc_BST(dsmh->left, TENMH);
    else return search_theo_ten_mon_hoc_BST(dsmh->right,TENMH);
}

bool search_theo_ma_mon_hoc_BST(treeMH dsmh, string maMonHoc){
    if(!dsmh) return false;
    if(maMonHoc == dsmh->mh.MAMH) return true;
    if(maMonHoc < dsmh->mh.MAMH)
        return search_theo_ma_mon_hoc_BST(dsmh->left, maMonHoc);
    else
        return search_theo_ma_mon_hoc_BST(dsmh->right, maMonHoc);
}

int tim_lop_theo_malop(const DS_LOPSV &dslopsv, const string &malop){
    for(int i = 0; i < dslopsv.n; i++)
        if(dslopsv.nodes[i]->MALOP == malop) return i;
    return -1;
}
//-------------------------------------------------------------------------------------------

void createFolder(const string& folder){
    if(!filesystem::exists(folder))
        filesystem::create_directory(folder);
}

void save_file_Mon_hoc(const treeMH &dsmh){
    if(!dsmh) return;
    string folder = "data";
    createFolder(folder);
    ofstream fout(folder + "/monhoc.txt", ios::app);
    if(!fout){ cerr << "Khong the mo file!\n"; return;}
    function<void(treeMH)> dfs = [&](treeMH t){
        if(!t) return;
        fout << t->mh.MAMH << "|" << t->mh.TENMH << "|" << t->mh.STCLT << "|" << t->mh.STCTH << "\n";
        dfs(t->left);
        dfs(t->right);
    };
    dfs(dsmh);
    fout.close();
    cout <<"Da luu mon hoc toi" << folder << "/monhoc.txt\n";
}

void save_file_lop_tc(PTRLTC dsltc){
    if(!dsltc) return;
    string folder = "data";
    createFolder(folder);
    ofstream fout(folder + "/dsltc.txt",ios::app);
    if(!fout){ cout << "Khong the mo file!\n";return;}
    while(dsltc){
        LopTinChi t = dsltc->ltc;
        fout << t.MALOPTC << "|" << t.MAMH << "|" << t.NienKhoa << "|" << t.Hocky << "|" << t.Nhom << "|" << t.sosvmin <<"|" <<t.sosvmax << "\n";
        dsltc = dsltc->next;
    }
    fout.close();
}

void save_sinh_vien_theo_ltc(DS_LOPSV dslopsv,PTRLTC dsloptc){
    if(!dsloptc) return;
    string folder = "data/dsloptc";
    createFolder(folder);
    while (dsloptc)
    {
        string lop = to_string(dsloptc->ltc.MALOPTC) + "_" + dsloptc->ltc.NienKhoa + ".txt";
        ofstream fout(folder + "/" + lop,ios::app);
        if(!fout) {cout << "Khong the mo file " << lop << "\n"; dsloptc = dsloptc->next; continue;}
        PTRDK dssvdk = dsloptc->ltc.dssvdk;
        while(dssvdk){
            SinhVien sv = tim_sinh_vien_chi_voi_maso(dslopsv,dssvdk->dk.MASV);
            fout << sv.MASV << "|" << sv.HO << "|" << sv.TEN << "|" << sv.PHAI << "|" << sv.SODT << "|" << sv.Email <<"\n";
            dssvdk = dssvdk->next;  
        }
        fout.close();
        dsloptc = dsloptc->next;
    }
}

void save_sinh_vien_theo_lop(DS_LOPSV dslopsv){
    if(dslopsv.n == 0) return;
    string folder = "data/dslop";
    createFolder(folder);
    for(int i=0;i<dslopsv.n;i++){
        string lop = dslopsv.nodes[i]->TENLOP + "_" + dslopsv.nodes[i]->MALOP+".txt";
        ofstream fout(folder + "/" + lop,ios::app);
        if(!fout) {cout << "Khong the mo file " << lop << "\n"; continue;}
        PTRSV p = dslopsv.nodes[i]->FirstSV;
        while (p){
            SinhVien sv = p->sv;
            fout << sv.MASV << "|" << sv.HO << "|" << sv.TEN << "|" << sv.PHAI << "|" << sv.SODT << "|" << sv.Email <<"\n";
            p=p->next;
        }
        fout.close();
    }
}

bool tim_sv_theo_maso_va_lop(const DS_LOPSV &dslopsv, string malop, const string maso){
    long long maso_ll = stoll(maso);
    if(dslopsv.n <= stoi(malop)) return false;
    int index = -1;
    for(int i = 0; i < dslopsv.n; i++){ if(dslopsv.nodes[i]->MALOP == malop){index = i; break;} }
    if(index == -1) return false;

    PTRSV p = dslopsv.nodes[index]->FirstSV;
    while(p && stoll(p->sv.MASV) <= maso_ll){
        if(p->sv.MASV == maso) return true;
        p = p->next;
    }
    return false;
}

void nhap_lop_va_sinhvien(DS_LOPSV &dslopsv){
    cout << "Nhap ten lop: "; string tenlop; getline(cin,tenlop);
    while(!text_format(tenlop)) getline(cin,tenlop);
    dslopsv.nodes[dslopsv.n] = new LopSV();
    dslopsv.nodes[dslopsv.n]->MALOP = to_string(dslopsv.n);
    dslopsv.nodes[dslopsv.n]->TENLOP = tenlop;
    dslopsv.nodes[dslopsv.n]->FirstSV = nullptr;
    dslopsv.n++;
    while(true){
        string masv; cout <<"Nhap ma sinh vien (skip de out): "; getline(cin,masv);
        if(masv.empty()) break;
        while(!maso_check(masv)) getline(cin,masv); 
        
        while(tim_sv_theo_maso_va_lop(dslopsv,dslopsv.nodes[dslopsv.n-1]->MALOP,masv)){
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
        PTRSV p = lop->FirstSV,prev = nullptr;
        while(p&&stoi(p->sv.MASV)<stoi(node->sv.MASV)){ //tim vi tri p nam truoc node moi
            prev=p;
            p=p->next;
        }if(!prev){ //chen dau
            node->next = lop->FirstSV;
            lop->FirstSV = node;
            if(!lop->LastSV) lop->LastSV = node;
        }else if(!p){ //chen cuoi
            prev->next = node;
            lop->LastSV = node;
        }else{ //chen giua
            prev->next = node;
            node->next = p;
        }
}
}

void xoa_sv(DS_LOPSV& dslopsv){
    cout << "Nhap ma lop(skip de out): "; string malop; getline(cin,malop);
    if(malop.empty()) return;
    int i = tim_lop_theo_malop(dslopsv,malop);
    while(i == -1 || !dslopsv.nodes[i]->FirstSV){
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
    while(!tim_sv_theo_maso_va_lop(dslopsv,lop->MALOP,masv)){
        cout <<"Khong tim thay sinh vien! Nhap lai(skip de out): ";
        getline(cin,masv);
        if(masv.empty()) return;
    }
    // sort san
    PTRSV p = lop->FirstSV;
    
    if(p->sv.MASV == masv){ //xóa node đầu
        lop->FirstSV = p->next;
        if(!lop->FirstSV) lop->LastSV = nullptr; //cập nhật lastSV (trường hợp sau khi xóa thì lớp rỗng)
        delete p;
    } else{
    while (p->next && p->next->sv.MASV != masv) p = p->next;
    PTRSV temp = p->next;
    p->next = temp->next;
    if(!p->next) lop->LastSV = p; //cập nhật last (nếu xóa hs ở cuối )
    delete temp;
    }
    cout << "Da xoa sinh vien "<< masv << " khoi lop " << malop <<endl;
}
}

void hieu_chinh_thong_tin_sinhvien(DS_LOPSV &dslopsv){
    cout << "Nhap ma lop(skip de out): "; string malop; getline(cin,malop);
    if(!maso_check(malop)) return;
    int i = tim_lop_theo_malop(dslopsv,malop);
    while(i == -1 || !dslopsv.nodes[i]->FirstSV){
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
    while(!tim_sv_theo_maso_va_lop(dslopsv,lop->MALOP,masv)){
        cout <<"Khong tim thay sinh vien! Nhap lai(skip de out): ";
        getline(cin,masv);
        if(masv.empty()) return;
    }
    PTRSV p = lop->FirstSV;
    while(p && p->sv.MASV != masv) p = p->next;
    string option;
    cout << "Nhap thong tin muon hieu chinh (Ma/Ho/Ten/Phai/SDT/Email/All/* de out): " ;getline(cin,option);
        if(option == "Ma"){
            cout << "Ma sinh vien cu: " << p->sv.MASV <<"\nNhap ma sinh vien moi(skip de out): "; getline(cin,masv);
            if(masv.empty()) return;
            
            while(tim_sv_theo_maso_va_lop(dslopsv,lop->MALOP,masv)){
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
            
            while(tim_sv_theo_maso_va_lop(dslopsv,lop->MALOP,masv)){
                cout <<"Ma sinh vien da ton tai! Nhap lai(skip de out): ";getline(cin, masv); 
                if(!maso_check(masv)) return;
            } 
            p->sv.MASV = masv;

            string hosv; cout <<"Ho cu cua sinh vien : "<< p->sv.HO <<"\nNhap ho sinh vien moi: "; getline(cin,hosv);
            while(!text_format(hosv)) getline(cin, hosv);
            p->sv.HO = hosv;
            string tensv; cout <<"Ten cu cua sinh vien: "<<p->sv.TEN<<"\nNhap ten sinh vien: "; getline(cin,tensv);
            while(!text_format(tensv)) getline(cin, tensv);
            p->sv.TEN = tensv;
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

void in_ds_mh_tang_dan(const nodeMH* dsmh){
    if(!dsmh){
        cout << "Danh sach mon hoc rong!";
        return;
    }
    cout <<"Danh sach mon hoc: ";
    auto back_track = [&](const nodeMH* node, auto&& self) -> void{
        if(!node) return;
        self(node->left,self);
        cout << node->mh.TENMH << " ";
        self(node->right,self);
        return;
    };
    back_track(dsmh,back_track);
}

PTRLTC tim_lop_tin_chi(PTRLTC ltc ,string nienkhoa, int hocky, int nhom, string monhoc){
    while(ltc){
        if(ltc->ltc.NienKhoa == nienkhoa && ltc->ltc.Hocky == hocky && ltc->ltc.Nhom == nhom && ltc->ltc.MAMH == monhoc){
            if(ltc->ltc.huylop){ cout << "Lop tin chi da bi huy!\n"; return nullptr; }
            if(!ltc->ltc.dssvdk){ cout <<"Lop chua co sinh vien dang ky!\n"; return nullptr; }
            return ltc;
        }
        ltc = ltc->next;
    }
    cout <<"Khong tim thay lop tinh chi!\n";
    return nullptr;
}

SinhVien tim_sinh_vien_chi_voi_maso(const DS_LOPSV &dslopsv,const string masv){
    long long masv_ll = stoll(masv);
    for(int i = 0; i < dslopsv.n; i++){
        PTRSV p = dslopsv.nodes[i]->FirstSV;
        while(p && stoll(p->sv.MASV) <= masv_ll){
            if(stoll(p->sv.MASV) == masv_ll) return p->sv;
            p = p->next;
        }   
    }
    SinhVien sv;
    sv.MASV = "-1";
    return sv;
}

void in_diem_sv(DS_LOPSV &dslopsv,PTRLTC ltc,string nienkhoa, int hocky, int nhom, string monhoc){
    if(!ltc){
        cout <<"Chua co lop tinh chi nao!\n";
        return;
    }
    ltc = tim_lop_tin_chi(ltc,nienkhoa,hocky,nhom,monhoc);
    if(!ltc) return;
    PTRDK dsdk = ltc->ltc.dssvdk;
    int i = 0;
    while (dsdk)
    {
        if(dsdk->dk.huydangky == false){
        SinhVien sv = tim_sinh_vien_chi_voi_maso(dslopsv,dsdk->dk.MASV);
        if(sv.MASV != "-1"){
        cout << "STT: " << i++ << " Mssv: " << sv .MASV << " Ho: " << sv.HO << " Ten: " << sv.TEN <<" Diem: " << dsdk->dk.DIEM <<endl;
            }
        }
        dsdk = dsdk->next;
    }
}

PTRDK tim_sinh_vien_theo_maso_voi_lop_tinh_chi(const PTRLTC ltc, const string maso){
    PTRDK dssv = ltc->ltc.dssvdk;
    long long maso_ll=stoll(maso);
    while(dssv&& stoll(dssv->dk.MASV) <= maso_ll){
        if(dssv->dk.MASV == maso) return dssv;
        dssv = dssv->next;
        }
    return nullptr;
}

void in_hieu_chinh_diem_sv(DS_LOPSV &dslopsv,PTRLTC ltc){
    if(!ltc){
        cout << "Danh sach lop tinh chi trong!";
        return;
    }
    string nienkhoa; cout <<"Nhap nien khoa cua lop can tim: "; getline(cin,nienkhoa);
    string hk; cout <<"Nhap hoc ky cua lop can tim: "; getline(cin,hk);
    while(int_check(hk) == -1){ cout << "Loi chi duoc hoc ky la chu so! Nhap lai: "; getline(cin,hk); }
    int hocky = int_check(hk);
    string nhoms; cout <<"Nhap nhom cua lop can tim: "; getline(cin,nhoms);
    while(int_check(nhoms) == -1){cout <<"Loi chi duoc nhap nhom la chu so! Nhap lai: "; getline(cin,nhoms);}
    int nhom = int_check(nhoms);
    string monhoc; cout <<"Nhap mon hoc can tim: "; getline(cin,monhoc);
    ltc = tim_lop_tin_chi(ltc,nienkhoa,hocky,nhom,monhoc);  
    if(!ltc) return;
    PTRDK dsdk = ltc->ltc.dssvdk;
    in_diem_sv(dslopsv, ltc, nienkhoa, hocky, nhom, monhoc);
    while(true){
        cout << "Nhap ma so sinh vien muon hieu chinh diem (skip de xem bang): "; string masv; getline(cin,masv);
        if(!maso_check(masv)) break;
        PTRDK dssv = tim_sinh_vien_theo_maso_voi_lop_tinh_chi(ltc,masv);
        while(!dssv){cout << "Khong tim thay sinh vien! Nhap lai: "; getline(cin,masv);dssv = tim_sinh_vien_theo_maso_voi_lop_tinh_chi(ltc,masv);}
        cout << "Nhap diem muon hieu chinh cho sinh vien co ma so "<<masv<< " :"; string diems; getline(cin,diems);
        while(float_check(diems)==-1){cout <<"Chi duoc nhap diem la chu so! Nhap lai: "; getline(cin,diems);}
        dssv->dk.DIEM = float_check(diems);
    }
    in_diem_sv(dslopsv, ltc, nienkhoa, hocky, nhom, monhoc);
}


int main(){
    PTRLTC dsltc=NULL;
    treeMH dsmh=NULL;
    DS_LOPSV dslopsv;
    PTRLTC ltc = nullptr;
    nhap_lop_va_sinhvien(dslopsv);
    xoa_sv(dslopsv);
    hieu_chinh_thong_tin_sinhvien(dslopsv);
    in_hieu_chinh_diem_sv(dslopsv, ltc);
}