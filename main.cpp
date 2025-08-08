#include<iostream>
#include "helper.hpp"
#include<string>
class Produs{
    protected:
    std::string nume;
    float pret;
    int kcal;
    public:
    Produs(std::string n="nedefinit", float p=0, int k=0) : nume(n), pret(p),kcal(k){}
    Produs(const Produs& p) : nume(p.nume), pret(p.pret), kcal(p.kcal){}
    Produs& operator=(const Produs& p) {
        if (this != &p) {
            nume = p.nume;
            pret = p.pret;
            kcal = p.kcal;
        }
        return *this;
    }
    virtual ~Produs() = default;
    virtual void afisare(std::ostream &os) const=0;
    float getPret() const {
        return pret;
    }
    int getKcal() const {
        return kcal;
    }
    void setKcal(int k) {
        kcal = k;
    }
    std::string getNume() const {
        return nume;
    }
    void setNume(const std::string& n) {
        nume = n;
    }
    friend std::ostream & operator<<(std::ostream &os, const Produs *p);
};
std::ostream& operator<<(std::ostream& os, const Produs* p) {
    p->afisare(os);
    return os;
}
class Mancare: public Produs{
    std::string tip;
    public:
    Mancare(std::string n="nedefinit", float p=0, int k=0, std::string t="nedefinit") : Produs(n,p,k), tip(t){}
    Mancare(const Mancare& m) : Produs(m), tip(m.tip){}
    Mancare& operator=(const Mancare& m) {
        if (this != &m) {
            Produs::operator=(m);
            tip = m.tip;
        }
        return *this;
    }
    void afisare(std::ostream&os) const override {
        os<<"Produs Mancare: " << nume << "\n"
                 << "Pret: " << pret << "\n"
                 << "Kcal: " << kcal << "\n"
                 << "Tip: " << tip << "\n";
    }
    std::string getTip() const {
        return tip;
    }
    void setTip(const std::string& t) {
        tip = t;
    }
    friend std::istream& operator>>(std::istream &is, Mancare *&m);
    //friend std::ostream& operator<<(std::ostream &os, const Mancare *m);
};

std::istream& operator>>(std::istream &is, Mancare *&m) {
    std::string nume, tip;
    float pret;
    int kcal;
    std::cout << "Introduceti numele produsului:\n";
    is >> nume;
    std::cout << "Introduceti pretul produsului:\n";
    pret=MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti numarul de calorii:\n";
    kcal= MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti tipul produsului:\n";
    std::cout<<"1. Mic dejun\n";
    std::cout<<"2. Brunch\n";
    std::cout<<"3. Pranz\n";
    int nr;
    nr= MenuHelper::choose_number_between_1_n(3);
    if(nr==1)
        tip="Mic dejun";
    else if(nr==2)
        tip="Brunch";
    else if(nr==3)
        tip="Pranz";
    m = new Mancare(nume, pret, kcal, tip);
    return is;
}
class Desert: public Produs{
    std::string tip;
    public:
    Desert(std::string n="nedefinit", float p=0, int k=0, std::string t="nedefinit") : Produs(n,p,k), tip(t){}
    Desert(const Desert& d) : Produs(d), tip(d.tip){}
    Desert& operator=(const Desert& d) {
        if (this != &d) {
            Produs::operator=(d);
            tip = d.tip;
        }
        return *this;
    }
    void afisare(std::ostream&os) const override {
        os<<"Produs Desert: " << nume << "\n"
                 << "Pret: " << pret << "\n"
                 << "Kcal: " << kcal << "\n"
                 << "Tip: " << tip << "\n";
    }
    std::string getTip() const {
        return tip;
    }
    void setTip(const std::string& t) {
        tip = t;
    }
    friend std::istream& operator>>(std::istream &is, Desert *&d);
    //friend std::ostream& operator<<(std::ostream &os, const Desert *d);
};

std::istream& operator>>(std::istream &is, Desert *&d) {
    std::string nume, tip;
    float pret;
    int kcal;
    std::cout << "Introduceti numele produsului:\n";
    is >> nume;
    std::cout << "Introduceti pretul produsului:\n";
    pret=MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti numarul de calorii:\n";
    kcal= MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti tipul produsului:\n";
    std::cout<<"1. Tort\n";
    std::cout<<"2. Prajitura\n";
    std::cout<<"3. Inghetata\n";
    int nr;
    nr= MenuHelper::choose_number_between_1_n(3);
    if(nr==1)
        tip="Tort";
    else if(nr==2)
        tip="Prajitura";
    else if(nr==3)
        tip="Inghetata";
    d = new Desert(nume, pret, kcal, tip);
    return is;
}
class Bautura: public Produs{
    bool alcool;
    public:
    Bautura(std::string n="nedefinit", float p=0, int k=0, bool a=false) : Produs(n,p,k), alcool(a){}
    Bautura(const Bautura& b) : Produs(b), alcool(b.alcool){}
    Bautura& operator=(const Bautura& b) {
        if (this != &b) {
            Produs::operator=(b);
            alcool = b.alcool;  
        }
        return *this;
    }
    void afisare(std::ostream&os) const override {
        os<<"Produs Bautura: " << nume << "\n"
                 << "Pret: " << pret << "\n"
                 << "Kcal: " << kcal << "\n"
                 << "Alcool: " << (alcool ? "Da" : "Nu") << "\n";
    }
    bool isAlcool() const {
        return alcool;
    }
    void setAlcool(bool a) {
        alcool = a;
    }
    friend std::istream& operator>>(std::istream &is, Bautura *&b);
    //friend std::ostream& operator<<(std::ostream &os, const Bautura *&b);
};
std::istream& operator>>(std::istream &is, Bautura *&b) {
    std::string nume;
    float pret;
    int kcal;
    bool alcool;
    std::cout << "Introduceti numele produsului:\n";
    is >> nume;
    std::cout << "Introduceti pretul produsului:\n";
    pret=MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti numarul de calorii:\n";
    kcal= MenuHelper::read_non_negative_amount(is);
    std::cout<<"Contine alcool? Da/Nu\n";
    alcool=MenuHelper::DaorNu()=="da";
    b = new Bautura(nume, pret, kcal, alcool);
    return is;
}
int main(){
    int n;
    std::cin>>n;
    std::vector<Produs*> produse;
    for(int i=0; i<n; i++){
        std::cout<<"Introduceti tipul produsului (1. Mancare, 2. Desert, 3. Bautura): ";
        int tip;
        std::cin>>tip;
        //Produs* p = nullptr;
        if(tip == 1) {
            Mancare *p=nullptr;
            std::cin>> p;
            produse.push_back(p);
        } else if(tip == 2) {
            Desert *p=nullptr;
            std::cin>> p;
            produse.push_back(p);
        } else if(tip == 3) {
            Bautura *p=nullptr;
            std::cin>> p;
            produse.push_back(p);
        } else {
            std::cout << "Tip invalid! Incercati din nou.\n";
            i--; // repeat this iteration
            continue;
        }
        
    }
    for(const auto& produs : produse) {
        std::cout << produs << "\n";
    }
    // Clean up memory
    for(auto& produs : produse) {
        delete produs;
    }
    produse.clear();
    std::cout << "Toate produsele au fost afisate si memoria a fost eliberata.\n";
    return 0;
}
