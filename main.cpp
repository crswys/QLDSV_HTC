#include <iostream>
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

typedef nodeDK* PTRLTC;
  
 int main(){
    PTRLTC dsltc=NULL;
    treeMH dsmh=NULL;
    DS_LOPSV dslopsv;
}