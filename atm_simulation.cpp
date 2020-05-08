#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

fstream file;
bool tryState = false;

struct Menu{
    void mainMenu();
    void insertData();
    void repeat();
    void cekSaldo();
    void transfer();
    void tarikTunai();
    void ubahPin();
}menu;

struct fileComponent{
    void readCard(string card);
    void readPin(string pin);
    void readSaldo();
    string getSaldo(int index);
    string card[2];
    string pin[2];
    string saldo[2];
    bool cardState;
    bool pinState;
    int i;
    int cIndex;
}f_comp;

struct Nasabah{
    void insertCard();
    void insertPIN();
    string card;
    string pin;
    long nominal;
    int saldo[2];
    short attempt, pinAttempt, indexRekTujuan;
    char pilihan;
}nasabah;

int main(){
    menu.insertData();
    menu.mainMenu();
}

void fileComponent::readCard(string card){
    string line;
    f_comp.cardState = false;
    f_comp.i = 0;
    file.open("card.txt");
    if(file.is_open()){
        while(getline(file,line)){
            f_comp.card[f_comp.i] = line;
            if(card == line){
                f_comp.cardState = true;
                f_comp.cIndex = i;
            }
            f_comp.i++;
        }
        file.close();
    }else{
        cout<<"unable to open"<<endl;
    }
}

void fileComponent::readPin(string pin){
    string line;
    f_comp.pinState = false;
    f_comp.i = 0;
    file.open("pin.txt");
    if(file.is_open()){
        while(getline(file,line)){
            f_comp.pin[f_comp.i] = line;
            f_comp.i++;
        }
        file.close();
    }else{
        cout<<"unable to open"<<endl;
    }
    if(pin == f_comp.pin[f_comp.cIndex]){
        f_comp.pinState = true;
    }
}

void fileComponent::readSaldo(){
    string line;
    file.open("saldo.txt");
    f_comp.i = 0;
    if(file.is_open()){
        while(getline(file,line)){
            f_comp.saldo[f_comp.i] = line;
            nasabah.saldo[f_comp.i] = atoi(line.c_str());
            f_comp.i++;
        }
    }else{
        cout<<"unable to open"<<endl;
    }
}

string fileComponent::getSaldo(int index){
    return f_comp.saldo[index];
}

void Nasabah::insertCard(){
    nasabah.card = "";
    while(nasabah.attempt < 3){
        cout<<"Card: ";
        cin>>nasabah.card;
        if(tryState){
            f_comp.readCard(nasabah.card);
        }
        f_comp.readCard(nasabah.card);
        if(f_comp.cardState){
            tryState = true;
            break;
        }else{
            nasabah.attempt++;
        }
    }
}

void Nasabah::insertPIN(){
    while(nasabah.pinAttempt < 3){
        cout<<"PIN: ";
        cin>>nasabah.pin;
        f_comp.readPin(nasabah.pin);
        if(f_comp.pinState){
            break;
        }else{
            nasabah.pinAttempt++;
        }
    }
}

void Menu::mainMenu(){
    system("cls");
    cout<<"======================================================================================================================="<<endl;
    cout<<"=====================================================Main Menu========================================================="<<endl;
    cout<<"======================================================================================================================="<<endl<<endl;
    cout<<"\t\t\t1. Transfer\t\t2. Penarikan Tunai"<<endl;
    cout<<"\t\t\t3. Informasi Saldo\t4. Ubah pin"<<endl;
    cout<<"\t\t\t5. Exit"<<endl<<endl;
    cout<<"Pilihan: ";
    cin>>nasabah.pilihan;
    if(nasabah.pilihan == '1'){
        menu.transfer();
    }else if(nasabah.pilihan == '2'){
        menu.tarikTunai();
    }else if(nasabah.pilihan == '3'){
        menu.cekSaldo();
    }else if(nasabah.pilihan == '4'){
        menu.ubahPin();
    }else if(nasabah.pilihan == '5'){
        exit(0);
    }
}

void Menu::insertData(){
    system("cls");
    cout<<"======================================================================================================================="<<endl;
    cout<<"==============================================Insert Your Card and PIN================================================="<<endl;
    cout<<"======================================================================================================================="<<endl<<endl;
    nasabah.attempt = 0;
    nasabah.pinAttempt = 0;

    nasabah.insertCard();
    if(nasabah.attempt == 3){
        cout<<"salah masuk 3x"<<endl;
        exit(0);
    }

    nasabah.insertPIN();
    if(nasabah.pinAttempt == 3){
        cout<<"salah masuk 3x"<<endl;
        exit(0);
    }
    f_comp.readSaldo();
}

void Menu::repeat(){
    cout<<"Transaksi lagi? (y/n)"<<endl;
    cout<<"Pilihan: ";
    cin>>nasabah.pilihan;
    if(nasabah.pilihan == 'y'){
        main();
    }else{
        exit(0);
    }
}

void Menu::cekSaldo(){
    system("cls");
    cout<<"======================================================================================================================="<<endl;
    cout<<"=====================================================Cek Saldo========================================================="<<endl;
    cout<<"======================================================================================================================="<<endl<<endl;
    cout<<"Saldo Anda: "<<f_comp.getSaldo(f_comp.cIndex)<<endl;
    menu.repeat();
}

void Menu::transfer(){
    system("cls");
    cout<<"======================================================================================================================="<<endl;
    cout<<"=====================================================Transfer=========================================================="<<endl;
    cout<<"======================================================================================================================="<<endl<<endl;
    cout<<"Masukkan No. Rekening Tujuan(Card): ";
    cin>>nasabah.card;
    for(f_comp.i = 0; f_comp.i < 2; f_comp.i++){
        if(nasabah.card == f_comp.card[f_comp.i] && nasabah.card != f_comp.card[f_comp.cIndex]){
            cout<<"Masukkan Nominal: ";
            cin>>nasabah.nominal;
            if(nasabah.saldo[f_comp.cIndex] > nasabah.nominal){
                nasabah.saldo[f_comp.cIndex] -= nasabah.nominal;    // rekening anda
                nasabah.saldo[f_comp.i] += nasabah.nominal;         // rekening tujuan
                cout<<"saldo: "<<nasabah.saldo[f_comp.cIndex]<<endl;
                ofstream write("saldo.txt");
                if(write.is_open()){
                    for(int i = 0; i < 2; i++){
                        if(f_comp.cIndex == i){
                            write<<nasabah.saldo[f_comp.cIndex]<<"\n";
                        }else{
                            write<<nasabah.saldo[i]<<"\n";
                        }
                    }
                    write.close();
                    break;
                }
            }else{
                cout<<"Saldo tidak cukup"<<endl;
            }
        }else if(nasabah.card == f_comp.card[f_comp.cIndex] && f_comp.i == 1){
            cout<<"Tidak bisa transfer ke rekening sendiri!!"<<endl;
        }else if(nasabah.card != f_comp.card[f_comp.i] && f_comp.i == 1){
            cout<<"Nomer rekening tidak valid!!"<<endl;
        }
    }
    menu.repeat();
}

void Menu::tarikTunai(){
    system("cls");
    cout<<"======================================================================================================================="<<endl;
    cout<<"===================================================Tarik Tunai========================================================="<<endl;
    cout<<"======================================================================================================================="<<endl<<endl;
    cout<<"Masukkan Nominal: ";
    cin>>nasabah.nominal;
    if(nasabah.saldo[f_comp.cIndex] > nasabah.nominal){
        nasabah.saldo[f_comp.cIndex] -= nasabah.nominal;    // rekening anda
        cout<<"saldo: "<<nasabah.saldo[f_comp.cIndex]<<endl;
        ofstream write("saldo.txt");
        if(write.is_open()){
            for(int i = 0; i < 2; i++){
                if(f_comp.cIndex == i){
                    write<<nasabah.saldo[f_comp.cIndex]<<"\n";
                }else{
                    write<<nasabah.saldo[i]<<"\n";
                }
            }
            write.close();
        }
    }else{
        cout<<"Saldo tidak cukup"<<endl;
    }
    menu.repeat();
}

void Menu::ubahPin(){
    string pin[2];
    system("cls");
    cout<<"======================================================================================================================="<<endl;
    cout<<"===================================================Tarik Tunai========================================================="<<endl;
    cout<<"======================================================================================================================="<<endl<<endl;
    cout<<"Masukan PIN Lama: ";
    cin>>pin[0];
    if(pin[0] == f_comp.pin[f_comp.cIndex]){
        cout<<"Masukan PIN Baru: ";
        cin>>pin[1];
        if(pin[1] != pin[0]){
            cout<<"Konfirmasi PIN Baru: ";
            cin>>pin[0];
            if(pin[0] == pin[1]){
                f_comp.pin[f_comp.cIndex] = pin[0];
                ofstream write("pin.txt");
                for(short i = 0; i < 2; i++){
                    if(i == f_comp.cIndex){
                        write<<f_comp.pin[f_comp.cIndex]<<"\n";
                    }
                    write<<f_comp.pin[i]<<"\n";
                }
                write.close();
            }else{
                cout<<"PIN Konfirmasi Salah!!"<<endl;
            }
        }else{
            cout<<"PIN Baru Tidak Boleh Sama dengan PIN Lama!!"<<endl;
        }
    }else{
        cout<<"PIN Salah!!"<<endl;
    }
    menu.repeat();
}
