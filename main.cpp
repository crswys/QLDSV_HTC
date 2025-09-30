#include <iostream>
#include <string>
#include <windows.h>
#include "utilities.h"

void clear() {system("cls");}

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

typedef nodeLopTinChi* PTRLTC;

void InDSSV_DK(PTRDK head)
{
    while (head)
    {
        if (!head->dk.huydangky)
            printf("%-20s %.1f\n", head->dk.MASV.c_str(), head->dk.DIEM);
        head= head->next;
    }
}

void printSofRL (PTRLTC List) //     b. Print student of registered list
{
    /*
    1 = NienKhoa
    2 = Hocky
    3 = Nhom
    4 = MAMH
    */
    int MAX_CONTENT = 4, i;
    string contents[MAX_CONTENT] = 
    {
        "Nien Khoa",
        "Hoc Ky",
        "Nhom",
        "Ma Mon Hoc",
    };
    for (i = 0; i < MAX_CONTENT; i++ ) cout << i + 1 << ". " << contents[i] << endl;
    cout << "Ban muon in ra theo kieu: ";
    size_t choice; cin >> choice;
    cout << "Nhap " << contents[choice-1] << " ma ban muon tim: ";
    string search; cin >> search;
    PTRDK DSDK;
    clear();
    cout << "Ma Sinh Vien        Diem"<< endl;
    while (List)
    {
        DSDK = List->ltc.dssvdk;
        switch(choice)
        {
            case 1:
                if (List->ltc.NienKhoa == search) InDSSV_DK(DSDK);
                break;
            case 2:
                if (to_string(List->ltc.Hocky) == search) InDSSV_DK(DSDK);
                break;
            case 3:
                if (to_string(List->ltc.Nhom) == search) InDSSV_DK(DSDK);
                break;
            case 4:
                if (List->ltc.MAMH == search) InDSSV_DK(DSDK);
                break;
            default:
                cout << "Yeu cau ban khong ton tai xin nhap lai: ";
                cin >> choice;
                cout << "Nhap " << contents[choice-1] << " ma ban muon tim: ";
                cin >> search;
                continue;
        } 
        List= List->next;
    }
}

int main(){
    PTRLTC dsltc=NULL;
    treeMH dsmh=NULL;
    DS_LOPSV dslopsv;
}