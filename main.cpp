#include <iostream>
#include "helper.hpp"
#include <stdexcept>
#include <string>
#include </opt/homebrew/include/mysql/mysql.h>
#include </opt/homebrew/include/mysql/mysqld_error.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <functional>
#include "db_env.hpp"
class Review
{
protected:
    int idComanda;
    int nota;
    int idClient;

public:
    Review(int idComanda = 0, int nota = 0, int idClient = 0) : idComanda(idComanda), nota(nota), idClient(idClient) {}
    Review(const Review &r) : idComanda(r.idComanda), nota(r.nota), idClient(r.idClient) {}
    Review &operator=(const Review &r)
    {
        if (this != &r)
        {
            idComanda = r.idComanda;
            nota = r.nota;
            idClient = r.idClient;
        }
        return *this;
    }
    int getIdComanda() const
    {
        return idComanda;
    }
    int getNota() const
    {
        return nota;
    }
    int getIdClient() const
    {
        return idClient;
    }
    void setIdComanda(int id)
    {
        idComanda = id;
    }
    void setNota(int n)
    {
        nota = n;
    }
    void setIdClient(int id)
    {
        idClient = id;
    }
    virtual void afisare(std::ostream &os) const
    {
        os << "Review ID Comanda: " << idComanda << "\n"
           << "Nota: " << nota << "\n"
           << "ID Client: " << idClient << "\n";
    }
    friend std::ostream &operator<<(std::ostream &os, const Review &r);
    virtual ~Review() = 0;
};
Review::~Review() = default;
std::ostream &operator<<(std::ostream &os, const Review &r)
{
    r.afisare(os);
    return os;
}

class ReviewComanda : public Review
{
    std::string comentariu;

public:
    ReviewComanda(int idComanda = 0, int nota = 0, int idClient = 0, std::string comentariu = "nedefinit") : Review(idComanda, nota, idClient), comentariu(comentariu) {}
    ReviewComanda(const ReviewComanda &rc) : Review(rc), comentariu(rc.comentariu) {}
    ReviewComanda &operator=(const ReviewComanda &rc)
    {
        if (this != &rc)
        {
            Review::operator=(rc);
            comentariu = rc.comentariu;
        }
        return *this;
    }
    std::string getComentariu() const
    {
        return comentariu;
    }
    void setComentariu(const std::string &c)
    {
        comentariu = c;
    }
    void afisare(std::ostream &os) const override
    {
        Review::afisare(os);
        os << "Comentariu: " << comentariu << "\n";
    }
    friend std::ostream &operator<<(std::ostream &os, const ReviewComanda &rc);
    virtual ~ReviewComanda() = default;
};

class ReviewLivrator : public Review
{
    int notaLivrator;

public:
    ReviewLivrator(int idComanda = 0, int nota = 0, int idClient = 0, int notaLivrator = 0) : Review(idComanda, nota, idClient), notaLivrator(notaLivrator) {}
    ReviewLivrator(const ReviewLivrator &rl) : Review(rl), notaLivrator(rl.notaLivrator) {}
    ReviewLivrator &operator=(const ReviewLivrator &rl)
    {
        if (this != &rl)
        {
            Review::operator=(rl);
            notaLivrator = rl.notaLivrator;
        }
        return *this;
    }
    int getNotaLivrator() const
    {
        return notaLivrator;
    }
    void setNotaLivrator(int n)
    {
        notaLivrator = n;
    }
    void afisare(std::ostream &os) const override
    {
        Review::afisare(os);
        os << "Nota Livrator: " << notaLivrator << "\n";
    }
    friend std::ostream &operator<<(std::ostream &os, const ReviewLivrator &rl);
    virtual ~ReviewLivrator() = default;
};

class Client
{
    int id;
    inline static int idCounter = 0;
    std::string nume;
    std::string prenume;
    std::string password;
    std::vector<std::unique_ptr<Review>> reviews;
    std::string categorie = "Basic";

public:
    Client(std::string nume = "nedefinit", std::string prenume = "nedefinit", std::string password = "nedefinit") : id(++idCounter), nume(nume), prenume(prenume), password(password)
    {
        std::cout << "Clientul " << nume << " " << prenume << " a fost creat.\n";
    }
    Client(int id, std::string nume = "nedefinit", std::string prenume = "nedefinit", std::string password = "nedefinit") : id(id), nume(nume), prenume(prenume), password(password)
    {
        std::cout << "Clientul " << nume << " " << prenume << " a fost creat.\n";
    }
    Client(int id, std::string nume, std::string prenume, std::string password, std::string categorie) : id(id), nume(nume), prenume(prenume), password(password), categorie(categorie) {}

    Client &operator=(const Client &) = delete;
    void addReviewComanda(std::unique_ptr<ReviewComanda> rc)
    {
        reviews.push_back(std::move(rc));
    }
    void addReviewLivrator(std::unique_ptr<ReviewLivrator> rl)
    {
        reviews.push_back(std::move(rl));
    }

    int getId() const
    {
        return id;
    }
    friend std::istream &operator>>(std::istream &is, Client *&c);
    void afisare(std::ostream &os) const
    {
        os << "Client ID: " << id << "\n"
           << "Nume: " << nume << "\n"
           << "Prenume: " << prenume << "\n"
           << "Categorie: " << categorie << "\n"
           << "Reviews:\n";
        for (const auto &r : reviews)
        {
            os << *r << "\n";
        }
    }
    std::string getParola() const
    {
        return password;
    }
    std::string getNume() const
    {
        return nume;
    }
    std::string getPrenume() const
    {
        return prenume;
    }
    void setCategorie(const std::string &cat)
    {
        categorie = cat;
    }
    std::string getCategorie() const
    {
        return categorie;
    }
    friend std::ostream &operator<<(std::ostream &os, const Client &c);
    ~Client() = default;
};
std::ostream &operator<<(std::ostream &os, const Client &c)
{
    c.afisare(os);
    return os;
}
std::istream &operator>>(std::istream &is, Client *&c)
{
    std::string nume, prenume;

    std::cout << "Introduceti numele clientului:\n";
    std::getline(is, nume);
    std::cout << "Introduceti prenumele clientului:\n";
    std::getline(is, prenume);
    c = new Client(nume, prenume);
    return is;
}
class Produs
{
protected:
    int id;
    std::string nume;
    float pret;
    int kcal;
    int idRestaurant;

public:
    Produs(int id = 0, std::string n = "nedefinit", float p = 0, int k = 0, int idRestaurant = 0) : id(id), nume(n), pret(p), kcal(k), idRestaurant(idRestaurant) {}
    Produs(const Produs &p) : id(p.id), nume(p.nume), pret(p.pret), kcal(p.kcal), idRestaurant(p.idRestaurant) {}
    Produs &operator=(const Produs &p)
    {
        if (this != &p)
        {
            id = p.id;
            nume = p.nume;
            pret = p.pret;
            kcal = p.kcal;
            idRestaurant = p.idRestaurant;
        }
        return *this;
    }
    virtual ~Produs() = default;
    virtual void afisare(std::ostream &os) const = 0;
    float getPret() const
    {
        return pret;
    }
    int getKcal() const
    {
        return kcal;
    }
    void setKcal(int k)
    {
        kcal = k;
    }
    std::string getNume() const
    {
        return nume;
    }
    void setNume(const std::string &n)
    {
        nume = n;
    }
    int getIdRestaurant() const
    {
        return idRestaurant;
    }
    int getId() const
    {
        return id;
    }
    void setPret(float p) { pret = p; }
    friend std::ostream &operator<<(std::ostream &os, const Produs *p);
};
std::ostream &operator<<(std::ostream &os, const Produs *p)
{
    p->afisare(os);
    return os;
}
class Mancare : public Produs
{
    std::string tip;

public:
    Mancare(int id, std::string n = "nedefinit", float p = 0, int k = 0, std::string t = "nedefinit") : Produs(id, n, p, k), tip(t) {}
    Mancare(const Mancare &m) : Produs(m), tip(m.tip) {}
    Mancare &operator=(const Mancare &m)
    {
        if (this != &m)
        {
            Produs::operator=(m);
            tip = m.tip;
        }
        return *this;
    }
    void afisare(std::ostream &os) const override
    {
        os << "Produs Mancare: " << nume << "\n"
           << "Pret: " << pret << "\n"
           << "Kcal: " << kcal << "\n"
           << "Tip: " << tip << "\n";
    }
    std::string getTip() const
    {
        return tip;
    }

    void setTip(const std::string &t)
    {
        tip = t;
    }
    friend std::istream &operator>>(std::istream &is, Mancare *&m);
};

std::istream &operator>>(std::istream &is, Mancare *&m)
{
    std::string nume, tip;
    float pret;
    int kcal;
    int id;
    std::cout << "Introduceti ID-ul produsului:\n";
    is >> id;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Introduceti numele produsului:\n";
    std::getline(is, nume);
    std::cout << "Introduceti pretul produsului:\n";
    pret = MenuHelper::read_non_negative_float(is);
    std::cout << "Introduceti numarul de calorii:\n";
    kcal = MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti tipul produsului:\n";
    std::cout << "1. Mic dejun\n";
    std::cout << "2. Brunch\n";
    std::cout << "3. Pranz\n";
    int nr;
    nr = MenuHelper::choose_number_between_1_n(3);
    if (nr == 1)
        tip = "Mic dejun";
    else if (nr == 2)
        tip = "Brunch";
    else if (nr == 3)
        tip = "Pranz";
    m = new Mancare(id, nume, pret, kcal, tip);
    return is;
}
class Desert : public Produs
{
    std::string tip;

public:
    Desert(int id, std::string n = "nedefinit", float p = 0, int k = 0, std::string t = "nedefinit") : Produs(id, n, p, k), tip(t) {}
    Desert(const Desert &d) : Produs(d), tip(d.tip) {}
    Desert &operator=(const Desert &d)
    {
        if (this != &d)
        {
            Produs::operator=(d);
            tip = d.tip;
        }
        return *this;
    }
    void afisare(std::ostream &os) const override
    {
        os << "Produs Desert: " << nume << "\n"
           << "Pret: " << pret << "\n"
           << "Kcal: " << kcal << "\n"
           << "Tip: " << tip << "\n";
    }
    std::string getTip() const
    {
        return tip;
    }
    void setTip(const std::string &t)
    {
        tip = t;
    }
    friend std::istream &operator>>(std::istream &is, Desert *&d);
    // friend std::ostream& operator<<(std::ostream &os, const Desert *d);
};

std::istream &operator>>(std::istream &is, Desert *&d)
{
    std::string nume, tip;
    float pret;
    int kcal;
    std::cout << "Introduceti ID-ul produsului:\n";
    int id;
    is >> id;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Introduceti numele produsului:\n";
    std::getline(is, nume);
    std::cout << "Introduceti pretul produsului:\n";
    pret = MenuHelper::read_non_negative_float(is);
    std::cout << "Introduceti numarul de calorii:\n";
    kcal = MenuHelper::read_non_negative_amount(is);
    std::cout << "Introduceti tipul produsului:\n";
    std::cout << "1. Tort\n";
    std::cout << "2. Prajitura\n";
    std::cout << "3. Inghetata\n";
    int nr;
    nr = MenuHelper::choose_number_between_1_n(3);
    if (nr == 1)
        tip = "Tort";
    else if (nr == 2)
        tip = "Prajitura";
    else if (nr == 3)
        tip = "Inghetata";
    d = new Desert(id, nume, pret, kcal, tip);
    return is;
}
class Bautura : public Produs
{
    bool alcool;

public:
    Bautura(int id, std::string n = "nedefinit", float p = 0, int k = 0, bool a = false) : Produs(id, n, p, k), alcool(a) {}
    Bautura(const Bautura &b) : Produs(b), alcool(b.alcool) {}
    Bautura &operator=(const Bautura &b)
    {
        if (this != &b)
        {
            Produs::operator=(b);
            alcool = b.alcool;
        }
        return *this;
    }
    void afisare(std::ostream &os) const override
    {
        os << "Nume produs: " << nume << "\n"
           << "Pret: " << pret << "\n"
           << "Kcal: " << kcal << "\n"
           << "Alcool: " << (alcool ? "Da" : "Nu") << "\n";
    }
    bool isAlcool() const
    {
        return alcool;
    }
    void setAlcool(bool a)
    {
        alcool = a;
    }
    friend std::istream &operator>>(std::istream &is, Bautura *&b);
    // friend std::ostream& operator<<(std::ostream &os, const Bautura *&b);
};
std::istream &operator>>(std::istream &is, Bautura *&b)
{
    std::string nume;
    float pret;
    int kcal;
    bool alcool;
    int id;
    std::cout << "Introduceti ID-ul produsului:\n";
    is >> id;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Introduceti numele produsului:\n";
    std::getline(is, nume);
    std::cout << "Introduceti pretul produsului:\n";
    pret = MenuHelper::read_non_negative_float(is);
    std::cout << "Introduceti numarul de calorii:\n";
    kcal = MenuHelper::read_non_negative_amount(is);
    std::cout << "Contine alcool? Da/Nu\n";
    alcool = MenuHelper::DaorNu() == "da";
    b = new Bautura(id, nume, pret, kcal, alcool);
    return is;
}
class Staff; // am declarat forward ca sa pot face clasa Restaurant, ce contine un vector de pointeri Staff
std::ostream &operator<<(std::ostream &os, const Staff &s);
class Restaurant
{
    int id;
    std::string nume;
    std::vector<Produs *> produse;
    std::vector<Staff *> staff;
    float rating;
    bool areSupervisor = false;

public:
    Restaurant(int i = 0, std::string n = "nedefinit", float r = 0.0) : id(i), nume(n), rating(r) {}
    Restaurant(const Restaurant &r) : id(r.id), nume(r.nume), rating(r.rating)
    {
        for (const auto &produs : r.produse)
        {
            if (dynamic_cast<Mancare *>(produs))
            {
                produse.push_back(new Mancare(*dynamic_cast<Mancare *>(produs)));
            }
            else if (dynamic_cast<Desert *>(produs))
            {
                produse.push_back(new Desert(*dynamic_cast<Desert *>(produs)));
            }
            else if (dynamic_cast<Bautura *>(produs))
            {
                produse.push_back(new Bautura(*dynamic_cast<Bautura *>(produs)));
            }
        }
    }
    Restaurant &operator=(const Restaurant &r)
    {
        if (this != &r)
        {
            id = r.id;
            nume = r.nume;
            rating = r.rating;
            for (auto &produs : produse)
            {
                delete produs;
            }
            produse.clear();
            for (const auto &produs : r.produse)
            {
                if (dynamic_cast<Mancare *>(produs))
                {
                    produse.push_back(new Mancare(*dynamic_cast<Mancare *>(produs)));
                }
                else if (dynamic_cast<Desert *>(produs))
                {
                    produse.push_back(new Desert(*dynamic_cast<Desert *>(produs)));
                }
                else if (dynamic_cast<Bautura *>(produs))
                {
                    produse.push_back(new Bautura(*dynamic_cast<Bautura *>(produs)));
                }
            }
        }
        return *this;
    }
    /**
     * @brief Afiseaza meniul restaurantului intr-un format tabelar.
     */
    void meniu(std::ostream &os = std::cout) const;
    ~Restaurant()
    {
        for (auto &produs : produse)
        {
            delete produs;
        }
        for (auto &s : staff)
        {
            delete s;
        }
        staff.clear();
        produse.clear();
    }
    void addProdus(Produs *p)
    {
        if (p)
        {
            produse.push_back(p);
        }
        else
        {
            std::cout << "Produs invalid, nu a fost adaugat.\n";
        }
    }
    void afisareProduse(std::ostream &os) const
    {
        if (produse.empty())
        {
            os << "Nu exista produse in restaurant.\n";
            return;
        }
        os << "Produse in restaurant " << nume << ":\n";
        for (const auto &produs : produse)
        {
            os << produs << "\n";
        }
    }

    void afisareStaff(std::ostream &os) const
    {
        if (staff.empty())
        {
            os << "Nu exista personal in restaurant.\n";
            return;
        }
        os << "Personal in restaurant " << nume << ":\n";
        for (const auto *s : staff)
        {
            os << *s << "\n";
        }
    }
    void addStaff(Staff *s)
    {
        if (s)
        {
            staff.push_back(s);
        }
        else
        {
            std::cout << "Personal invalid, nu a fost adaugat.\n";
        }
    }
    int getId() const
    {
        return id;
    }
    std::string getNume() const
    {
        return nume;
    }
    std::vector<Produs *> &getProduse()
    {
        return produse;
    }
    std::vector<Staff *> &getStaff()
    {
        return staff;
    }
    void setNume(const std::string &n)
    {
        nume = n;
    }
    float getRating() const
    {
        return rating;
    }
    void setAreSupervisor(bool value)
    {
        areSupervisor = value;
    }
    bool getAreSupervisor() const
    {
        return areSupervisor;
    }
    void setRating(float r)
    {
        if (r >= 0.0 && r <= 5.0)
        {
            rating = r;
        }
        else
        {
            std::cout << "Rating invalid! Trebuie sa fie intre 0 si 5.\n";
        }
    }

    const Produs *findProdusById(int pid) const
    {
        for (auto *p : produse)
            if (p && p->getId() == pid)
                return p;
        return nullptr;
    }
    Produs *findProdusById(int pid)
    {
        for (auto *p : produse)
            if (p && p->getId() == pid)
                return p;
        return nullptr;
    }

    friend std::ostream &operator<<(std::ostream &os, const Restaurant &r);
};

std::ostream &operator<<(std::ostream &os, const Restaurant &r)
{
    os << "Restaurant ID: " << r.id << "\n"
       << "Nume: " << r.nume << "\n"
       << "Rating: " << r.rating << "\n";
    std::cout << "Produse:\n";
    r.afisareProduse(os);
    std::cout << "\nPersonal:\n";
    r.afisareStaff(os);
    return os;
}

void Restaurant::meniu(std::ostream &os) const
{
    using std::fixed;
    using std::left;
    using std::right;
    using std::setfill;
    using std::setprecision;
    using std::setw;

    std::vector<const Mancare *> md_micdejun, md_brunch, md_pranz;
    std::vector<const Desert *> deserts;
    std::vector<const Bautura *> bauturi_alcool, bauturi_fara;

    for (auto *p : produse)
    {
        if (auto *m = dynamic_cast<Mancare *>(p))
        {
            const std::string &tip = m->getTip();
            if (tip == "Mic dejun")
                md_micdejun.push_back(m);
            else if (tip == "Brunch")
                md_brunch.push_back(m);
            else if (tip == "Pranz")
                md_pranz.push_back(m);
        }
        else if (auto *d = dynamic_cast<Desert *>(p))
        {
            deserts.push_back(d);
        }
        else if (auto *b = dynamic_cast<Bautura *>(p))
        {
            (b->isAlcool() ? bauturi_alcool : bauturi_fara).push_back(b);
        }
    }

    auto linie = [&](int w = 64, char ch = '-')
    {
        os << setfill(ch) << setw(w) << "" << setfill(' ') << "\n";
    };
    auto cap = [&](const std::string &t)
    {
        os << "\n"
           << t << "\n";
        linie(64, '=');
    };

    auto print_header_basic = [&]()
    {
        os << left << setw(6) << "ID"
           << left << setw(32) << "Produs"
           << right << setw(12) << "Pret"
           << right << setw(10) << "Kcal" << "\n";
        linie();
    };
    auto print_row_basic = [&](const Produs *p)
    {
        os << left << setw(6) << p->getId()
           << left << setw(32) << p->getNume()
           << right << setw(12) << fixed << setprecision(2) << p->getPret()
           << right << setw(10) << p->getKcal() << "\n";
    };

    auto print_header_desert = [&]()
    {
        os << left << setw(6) << "ID"
           << left << setw(28) << "Desert"
           << left << setw(14) << "Tip"
           << right << setw(12) << "Pret"
           << right << setw(10) << "Kcal" << "\n";
        linie();
    };
    auto print_row_desert = [&](const Desert *d)
    {
        os << left << setw(6) << d->getId()
           << left << setw(28) << d->getNume()
           << left << setw(14) << d->getTip()
           << right << setw(12) << fixed << setprecision(2) << d->getPret()
           << right << setw(10) << d->getKcal() << "\n";
    };

    auto print_header_bautura = [&]()
    {
        os << left << setw(6) << "ID"
           << left << setw(28) << "Bautura"
           << left << setw(10) << "Alcool"
           << right << setw(12) << "Pret"
           << right << setw(10) << "Kcal" << "\n";
        linie();
    };
    auto print_row_bautura = [&](const Bautura *b)
    {
        os << left << setw(6) << b->getId()
           << left << setw(28) << b->getNume()
           << left << setw(10) << (b->isAlcool() ? "Da" : "Nu")
           << right << setw(12) << fixed << setprecision(2) << b->getPret()
           << right << setw(10) << b->getKcal() << "\n";
    };

    os << "\n=== MENIU — " << nume << " (rating " << fixed << setprecision(1) << rating << ") ===\n";

    bool are_mancare = !md_micdejun.empty() || !md_brunch.empty() || !md_pranz.empty();
    if (are_mancare)
    {
        cap("MANCARE");
        if (!md_micdejun.empty())
        {
            os << "\n-- Mic dejun --\n";
            print_header_basic();
            for (auto *m : md_micdejun)
                print_row_basic(m);
        }
        if (!md_brunch.empty())
        {
            os << "\n-- Brunch --\n";
            print_header_basic();
            for (auto *m : md_brunch)
                print_row_basic(m);
        }
        if (!md_pranz.empty())
        {
            os << "\n-- Pranz --\n";
            print_header_basic();
            for (auto *m : md_pranz)
                print_row_basic(m);
        }
    }

    if (!deserts.empty())
    {
        cap("DESERTURI");
        print_header_desert();
        for (auto *d : deserts)
            print_row_desert(d);
    }

    if (!bauturi_alcool.empty() || !bauturi_fara.empty())
    {
        cap("BAUTURI");
        if (!bauturi_alcool.empty())
        {
            os << "\n-- Cu alcool --\n";
            print_header_bautura();
            for (auto *b : bauturi_alcool)
                print_row_bautura(b);
        }
        if (!bauturi_fara.empty())
        {
            os << "\n-- Fără alcool --\n";
            print_header_bautura();
            for (auto *b : bauturi_fara)
                print_row_bautura(b);
        }
    }

    if (!are_mancare && deserts.empty() && bauturi_alcool.empty() && bauturi_fara.empty())
    {
        os << "Nu exista produse in meniu.\n";
    }

    os << std::flush;
}

class Comanda
{
    const int idComanda;
    const int idRestaurant;
    const int idClient;
    const int idLivrator;
    bool finalizata = false;
    bool livrata = false;
    std::vector<Produs *> produse;
    inline static int idComandaCounter = 0;
    float pretTotal = 0.0;

public:
    Comanda(int idRestaurant = 0, int idClient = 0, int idLivrator = 0)
        : idComanda(++idComandaCounter), idRestaurant(idRestaurant), idClient(idClient), idLivrator(idLivrator) {}
    Comanda(int idComanda, int idRestaurant = 0, int idClient = 0, int idLivrator = 0) : idComanda(idComanda), idRestaurant(idRestaurant), idClient(idClient), idLivrator(idLivrator) {}
    Comanda(const Comanda &c) : idComanda(c.idComanda), idRestaurant(c.idRestaurant), idClient(c.idClient), idLivrator(c.idLivrator), produse(c.produse) {}
    int getIdComanda() const
    {
        return idComanda;
    }
    int getIdRestaurant() const
    {
        return idRestaurant;
    }
    int getIdClient() const
    {
        return idClient;
    }
    int getIdLivrator() const
    {
        return idLivrator;
    }
    void addProdus(Produs *p)
    {
        if (p)
        {
            produse.push_back(p);
        }
        else
        {
            std::cout << "Produs invalid, nu a fost adaugat la comanda.\n";
        }
    }
    void setFinalizata(bool f)
    {
        finalizata = f;
    }
    std::vector<Produs *> &getProduse()
    {
        return produse;
    }
    void afisare(std::ostream &os) const
    {
        os << "Comanda ID: " << idComanda << "\n"
           << "ID Restaurant: " << idRestaurant << "\n"
           << "ID Client: " << idClient << "\n"
           << "ID Livrator: " << idLivrator << "\n";
        if (produse.empty())
        {
            os << "Nu exista produse in comanda.\n";
        }
        else
        {
            os << "Produse in comanda:\n";
            for (const auto &produs : produse)
            {
                os << produs << "\n";
            }
        }
    }
    void makeLivrata()
    {
        livrata = true;
    }
    void calculatePretTotal()
    {
        pretTotal = 0.0;
        for (const auto &produs : produse)
        {
            pretTotal += produs->getPret();
        }
    }
    float getPretTotal() const
    {
        return pretTotal;
    }
    bool isFinalizata() const
    {
        return finalizata;
    }
    bool isLivrata() const
    {
        return livrata;
    }
    bool isAnulata() const { return livrata && !finalizata; }
    friend std::ostream &operator<<(std::ostream &os, const Comanda &c);
};

std::ostream &operator<<(std::ostream &os, const Comanda &c)
{
    c.afisare(os);
    return os;
}

template <class T>
inline T *rawptr(T *p) { return p; }
template <class T>
inline T *rawptr(const std::unique_ptr<T> &p) { return p.get(); }
template <class Vec, class Predicat>
bool erase_first_if(Vec &v, Predicat pred)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        auto *p = rawptr(v[i]);
        if (p && pred(*p))
        {
            v.erase(v.begin() + i);
            return true;
        }
    }
    return false;
}
template <class Vec, class Predicat>
void erase_all_if(Vec &v, Predicat pred)
{
    const auto before = v.size();
    v.erase(std::remove_if(v.begin(), v.end(),
                           [&](auto &h)
                           {
                               auto *p = rawptr(h);
                               return p && pred(*p);
                           }),
            v.end());
}

class Staff
{
protected:
    const int idRestaurant;
    const std::string nume;
    const std::string prenume;
    const int id;

public:
    Staff(int idRestaurant = 0, std::string nume = "nedefinit", std::string prenume = "nedefinit", int id = 0) : idRestaurant(idRestaurant), nume(nume), prenume(prenume), id(id) {}
    Staff(const Staff &s) : idRestaurant(s.idRestaurant), nume(s.nume), prenume(s.prenume), id(s.id) {}
    std::string getNume() const
    {
        return nume;
    }
    std::string getPrenume() const
    {
        return prenume;
    }
    int getId() const
    {
        return id;
    }
    virtual void afisare(std::ostream &os) const
    {
        os << idRestaurant << "\n"
           << "Staff ID: " << id << "\n"
           << "Nume: " << nume << "\n"
           << "Prenume: " << prenume << "\n";
    }
    friend std::ostream &operator<<(std::ostream &os, const Staff &s);
    virtual ~Staff() = 0;
};

Staff::~Staff() = default;

class Bucatar : virtual public Staff
{
    std::vector<Comanda *> comenzi;

public:
    Bucatar(int idRestaurant, std::string nume = "nedefinit", std::string prenume = "nedefinit", int id = 0) : Staff(idRestaurant, nume, prenume, id) {}
    Bucatar(const Bucatar &b) : Staff(b) {}
    void afiseazaComenzi(std::ostream &os = std::cout)
    {
        if (comenzi.empty())
        {
            os << "Nu aveti comenzi in lucru.\n";
            return;
        }
        for (auto *com : comenzi)
        {
            if (!com)
                continue;
            if (!com || com->isAnulata())
                continue;
            os << "Comanda #" << com->getIdComanda() << " — produse:\n";
            auto &produse = com->getProduse();
            if (produse.empty())
            {
                os << "  (fara produse)\n";
            }
            else
            {
                for (auto *p : produse)
                {
                    if (!p)
                        continue;
                    os << "  - " << p->getNume()
                       << " (" << std::fixed << std::setprecision(2) << p->getPret()
                       << " lei, " << p->getKcal() << " kcal)\n";
                }
            }
        }
    }
    void afisare(std::ostream &os) const override
    {
        Staff::afisare(os);
    }
    int nrComenzi() const { return comenzi.size(); }
    void adaugaComanda(Comanda *c)
    {
        if (c)
            comenzi.push_back(c);
    }
    void preparaComanda(int idComanda)
    {
        for (auto &comanda : comenzi)
        {
            if (comanda->isAnulata())
            { // <<— nou
                std::cout << "Comanda " << idComanda << " este anulata.\n";
                return;
            }
            if (comanda->getIdComanda() == idComanda)
            {
                comanda->setFinalizata(true);
                std::cout << "Comanda " << idComanda << " a fost preparata de bucatarul " << nume << " " << prenume << ".\n";

                erase_first_if(comenzi, [&](const Comanda &c)
                               { return c.getIdComanda() == idComanda; });

                return;
            }
        }
        std::cout << "Comanda " << idComanda << " nu a fost gasita.\n";
    }
    bool areComanda(int idC) const
    {
        for (auto *c : comenzi)
            if (c && c->getIdComanda() == idC)
                return true;
        return false;
    }

    bool stergeComandaDinCoada(int idC)
    {
        return erase_first_if(comenzi, [&](const Comanda &c)
                              { return c.getIdComanda() == idC; });
    }

    ~Bucatar() override = default;
};

class Livrator : virtual public Staff
{
    std::vector<Comanda *> comenzi;

public:
    Livrator(int idRestaurant, std::string nume = "nedefinit", std::string prenume = "nedefinit", int id = 0) : Staff(idRestaurant, nume, prenume, id) {}
    Livrator(const Livrator &l) : Staff(l) {}
    void afisare(std::ostream &os) const override
    {
        Staff::afisare(os);
    }
    void adaugaComanda(Comanda *c)
    {
        if (c)
        {
            comenzi.push_back(c);
            std::cout << "Comanda " << c->getIdComanda() << " a fost adaugata la livratorul "
                      << nume << " " << prenume << ".\n";
        }
        else
        {
            std::cout << "Comanda invalida, nu a fost adaugata.\n";
        }
    }
    void livreazaComanda(int idComanda)
    {
        for (auto &comanda : comenzi)
        {
            if (comanda->getIdComanda() == idComanda)
            {
                comanda->makeLivrata();
                std::cout << "Comanda " << idComanda << " a fost livrata de livratorul " << nume << " " << prenume << ".\n";
                erase_first_if(comenzi, [&](const Comanda &c)
                               { return c.getIdComanda() == idComanda; });
                return;
            }
        }
        std::cout << "Comanda " << idComanda << " nu a fost gasita.\n";
    }
    int nrComenzi() const { return static_cast<int>(comenzi.size()); }
    void afiseazaComenzi(std::ostream &os = std::cout)
    {
        if (comenzi.empty())
        {
            os << "Nu aveti comenzi de livrat.\n";
            return;
        }
        for (auto *com : comenzi)
        {
            if (!com)
                continue;
            os << "Comanda " << com->getIdComanda() << " — produse:\n";
            auto &produse = com->getProduse();
            if (produse.empty())
            {
                os << "  (fara produse)\n";
            }
            else
            {
                for (auto *p : produse)
                {
                    if (!p)
                        continue;
                    os << "  - " << p->getNume()
                       << " (" << std::fixed << std::setprecision(2) << p->getPret()
                       << " lei, " << p->getKcal() << " kcal)\n";
                }
            }
        }
    }
    bool areComanda(int idC) const
    {
        for (auto *c : comenzi)
            if (c && c->getIdComanda() == idC)
                return true;
        return false;
    }
    // primește un int ca argument de intrare și returnează un std::string
    bool livreazaUrmatoareaComandaPrioritar(const std::function<std::string(int)> &categorieClient, int &idComandaOut, std::string &categorieOut);
};
bool Livrator::livreazaUrmatoareaComandaPrioritar(const std::function<std::string(int)> &categorieClient, int &idComandaOut, std::string &categorieOut)
{

    comenzi.erase(std::remove_if(comenzi.begin(), comenzi.end(),
                                 [](Comanda *c)
                                 { return !c || c->isLivrata() || c->isAnulata(); }),
                  comenzi.end());

    if (comenzi.empty())
        return false;

    auto rank = [](const std::string &c)
    {
        if (c == "Gold")
            return 4;
        if (c == "Silver")
            return 3;
        if (c == "Bronze")
            return 2;
        return 1;
    };

    /*
    Cu aceasta functie lambda aleg comanda cu cea mai mare prioritate in functie de categorie
    In caz de egalitate, folosesc principiul FIFO, id-ul cel mai mic e ales, fiind comanda mai veche
    */
    auto bestIt = std::max_element(comenzi.begin(), comenzi.end(),
                                   [&](Comanda *a, Comanda *b)
                                   {
                                       int ra = rank(categorieClient(a->getIdClient()));
                                       int rb = rank(categorieClient(b->getIdClient()));
                                       if (ra != rb)
                                           return ra < rb;
                                       return a->getIdComanda() > b->getIdComanda(); // FIFO
                                   });

    Comanda *pick = *bestIt;
    categorieOut = categorieClient(pick->getIdClient());
    idComandaOut = pick->getIdComanda();

    pick->makeLivrata();
    comenzi.erase(bestIt);

    return true;
}

class Casier : virtual public Staff
{

public:
    Casier(int idRestaurant, std::string nume = "nedefinit", std::string prenume = "nedefinit", int id = 0) : Staff(idRestaurant, nume, prenume, id) {}
    Casier(const Casier &c) : Staff(c) {}
    void afisare(std::ostream &os) const override
    {
        Staff::afisare(os);
    }
    bool distribuieComanda(int idComanda, std::vector<Comanda *> &comenziDisponibile, std::vector<Bucatar *> &bucatari) const
    {
        auto it = std::find_if(comenziDisponibile.begin(), comenziDisponibile.end(), [&](Comanda *c)
                               { return c && c->getIdComanda() == idComanda; });
        if (it == comenziDisponibile.end())
        {
            std::cout << "Comanda " << idComanda << " nu exista in lista disponibila.\n";
            return false;
        }

        if ((*it)->isAnulata())
        {
            std::cout << "Comanda " << idComanda << " este anulata.\n";
            return false;
        }

        for (auto *b : bucatari)
        {
            if (b && b->areComanda(idComanda))
            {
                std::cout << "Comanda " << idComanda << " este deja asignata unui bucatar.\n";
                return false;
            }
        }

        Comanda *com = *it;

        /*
        Cu acest for aleg primul bucatar cu coada de comenzi <= 2
        Daca nu exista niciun bucatar cu coada <= 2, comanda ramane in vectorul de comenzi disponibile
        */
        for (auto *b : bucatari)
        {
            if (b && b->nrComenzi() <= 2)
            {
                b->adaugaComanda(com);
                comenziDisponibile.erase(it);
                std::cout << "Comanda " << idComanda
                          << " a fost asignata bucatarului "
                          << b->getNume() << " " << b->getPrenume() << ".\n";
                return true;
            }
        }

        std::cout << "Nu exista bucatar cu coada <= 2 pentru comanda " << idComanda << ".\n";
        return false;
    }
    bool distribuieComandaLivrator(int idComanda, std::vector<Comanda *> &comenziDisponibile, std::vector<Livrator *> &livratori) const
    {

        /**
         * @brief Caută comanda cu ID-ul specificat în lista de comenzi disponibile.
         * Dacă comanda nu este găsită, afișează un mesaj de eroare și returnează false.
         * Dacă comanda este deja asignată unui livrator, afișează un mesaj de eroare și returnează false.
         * Dacă există un livrator cu o coadă de comenzi mai mică sau egală cu 2, asignă comanda acestuia,
         * elimină comanda din lista de comenzi disponibile și returnează true.
         * Dacă nu există niciun livrator disponibil, afișează un mesaj de eroare și returnează false.
         */
        auto it = std::find_if(comenziDisponibile.begin(), comenziDisponibile.end(),
                               [&](Comanda *c)
                               { return c && c->getIdComanda() == idComanda; });
        if (it == comenziDisponibile.end())
        {
            std::cout << "Comanda " << idComanda << " nu exista in lista disponibila pentru livrare.\n";
            return false;
        }

        for (auto *l : livratori)
        {
            if (l && l->areComanda(idComanda))
            {
                std::cout << "Comanda " << idComanda << " este deja asignata unui livrator.\n";
                return false;
            }
        }

        Comanda *com = *it;

        for (auto *l : livratori)
        {
            if (l && l->nrComenzi() <= 2)
            {
                l->adaugaComanda(com);
                comenziDisponibile.erase(it);
                return true;
            }
        }

        std::cout << "Nu exista livrator cu coada <= 2 pentru comanda " << idComanda << ".\n";
        return false;
    }
};

class Supervisor : public Bucatar, public Casier
{
public:
    Supervisor(int idRestaurant, std::string nume = "nedefinit", std::string prenume = "nedefinit", int id = 0)
        : Staff(idRestaurant, nume, prenume, id), Bucatar(idRestaurant, nume, prenume, id), Casier(idRestaurant, nume, prenume, id) {}
    Supervisor(const Supervisor &s) : Staff(s), Bucatar(s), Casier(s) {}
    void afisare(std::ostream &os) const override
    {
        Staff::afisare(os);
    }
    Bucatar *adaugaBucatar(std::string nume, std::string prenume, int id)
    {
        auto *bucatar = new Bucatar(idRestaurant, nume, prenume, id);
        return bucatar;
    }
    Casier *adaugaCasier(std::string nume, std::string prenume, int id)
    {
        auto *casier = new Casier(idRestaurant, nume, prenume, id);
        return casier;
    }

    Mancare *adaugaMancare()
    {
        Mancare *m;
        std::cin >> m;
        return m;
    }
    Desert *adaugaDesert()
    {
        Desert *d;
        std::cin >> d;
        return d;
    }
    Bautura *adaugaBautura()
    {
        Bautura *b;
        std::cin >> b;
        return b;
    }
    Mancare *adaugaMancareCuId(int idNou);
    Desert *adaugaDesertCuId(int idNou);
    Bautura *adaugaBauturaCuId(int idNou);
    Livrator *adaugaLivrator(std::string nume, std::string prenume, int id)
    {
        auto *livrator = new Livrator(idRestaurant, nume, prenume, id);
        return livrator;
    }
};

Mancare *Supervisor::adaugaMancareCuId(int idNou)
{
    std::string nume, tip;
    float pret;
    int kcal;

    std::cout << "Introduceti numele produsului:\n";
    std::getline(std::cin >> std::ws, nume);

    std::cout << "Introduceti pretul produsului:\n";
    pret = MenuHelper::read_non_negative_float(std::cin);

    std::cout << "Introduceti numarul de calorii:\n";
    kcal = MenuHelper::read_non_negative_amount(std::cin);

    std::cout << "Introduceti tipul produsului:\n"
                 "1. Mic dejun\n"
                 "2. Brunch\n"
                 "3. Pranz\n";
    int nr = MenuHelper::choose_number_between_1_n(3);
    if (nr == 1)
        tip = "Mic dejun";
    else if (nr == 2)
        tip = "Brunch";
    else
        tip = "Pranz";

    return new Mancare(idNou, nume, pret, kcal, tip);
}

Desert *Supervisor::adaugaDesertCuId(int idNou)
{
    std::string nume, tip;
    float pret;
    int kcal;

    std::cout << "Introduceti numele produsului:\n";
    std::getline(std::cin >> std::ws, nume);

    std::cout << "Introduceti pretul produsului:\n";
    pret = MenuHelper::read_non_negative_float(std::cin);

    std::cout << "Introduceti numarul de calorii:\n";
    kcal = MenuHelper::read_non_negative_amount(std::cin);

    std::cout << "Introduceti tipul produsului:\n"
                 "1. Tort\n"
                 "2. Prajitura\n"
                 "3. Inghetata\n";
    int nr = MenuHelper::choose_number_between_1_n(3);
    if (nr == 1)
        tip = "Tort";
    else if (nr == 2)
        tip = "Prajitura";
    else
        tip = "Inghetata";

    return new Desert(idNou, nume, pret, kcal, tip);
}

Bautura *Supervisor::adaugaBauturaCuId(int idNou)
{
    std::string nume;
    float pret;
    int kcal;
    bool alcool;

    std::cout << "Introduceti numele produsului:\n";
    std::getline(std::cin >> std::ws, nume);

    std::cout << "Introduceti pretul produsului:\n";
    pret = MenuHelper::read_non_negative_float(std::cin);

    std::cout << "Introduceti numarul de calorii:\n";
    kcal = MenuHelper::read_non_negative_amount(std::cin);

    std::cout << "Contine alcool? Da/Nu\n";
    alcool = (MenuHelper::DaorNu() == "da");

    return new Bautura(idNou, nume, pret, kcal, alcool);
}

std::ostream &operator<<(std::ostream &os, const Staff &s)
{
    if (auto p = dynamic_cast<const Supervisor *>(&s))
    {
        os << "Supervisor:\n";
    }
    else if (auto p = dynamic_cast<const Casier *>(&s))
    {
        std::cout << "Casier:\n";
    }
    else if (auto p = dynamic_cast<const Bucatar *>(&s))
    {
        std::cout << "Bucatar:\n";
    }
    else if (auto p = dynamic_cast<const Livrator *>(&s))
    {
        std::cout << "Livrator:\n";
    }
    else
    {
        os << "Unknown staff type.\n";
    }
    s.afisare(os);
    return os;
}

/**
 * @brief Clasa pentru excepția aruncată atunci când se încearcă adăugarea unui produs de tip necunoscut.
 */
class UnsupportedAdd : std::logic_error
{
public:
    explicit UnsupportedAdd(const std::string &msg) : std::logic_error(msg) {}
    ~UnsupportedAdd() override = default;
};

class OrderItemSnap
{
    int idItem = 0;
    int idComanda = 0;
    int idProdus = 0;
    std::string tipProdus;
    int cantitate = 0;
    std::string nume_snapshot;
    double pret_unit_snapshot = 0.0;
    int kcal_unit_snapshot = 0;

public:
    int getIdItem() const { return idItem; }
    int getIdComanda() const { return idComanda; }
    int getIdProdus() const { return idProdus; }
    std::string getTipProdus() const { return tipProdus; }
    int getCantitate() const { return cantitate; }
    std::string getNumeSnapshot() const { return nume_snapshot; }
    double getPretUnitSnapshot() const { return pret_unit_snapshot; }
    int getKcalUnitSnapshot() const { return kcal_unit_snapshot; }
    void setIdItem(int id) { idItem = id; }
    void setIdComanda(int id) { idComanda = id; }
    void setIdProdus(int id) { idProdus = id; }
    void setTipProdus(const std::string &tip) { tipProdus = tip; }
    void setCantitate(int cant) { cantitate = cant; }
    void setNumeSnapshot(const std::string &nume) { nume_snapshot = nume; }
    void setPretUnitSnapshot(double pret) { pret_unit_snapshot = pret; }
    void setKcalUnitSnapshot(int kcal) { kcal_unit_snapshot = kcal; }
};
class OrderItemDB
{
    int idProdus = 0;
    std::string tipProdus;
    int cantitate = 0;
    std::string nume_snapshot;
    double pret_unit_snapshot = 0.0;
    int kcal_unit_snapshot = 0;

public:
    int getIdProdus() const { return idProdus; }
    std::string getTipProdus() const { return tipProdus; }
    int getCantitate() const { return cantitate; }
    std::string getNumeSnapshot() const { return nume_snapshot; }
    double getPretUnitSnapshot() const { return pret_unit_snapshot; }
    int getKcalUnitSnapshot() const { return kcal_unit_snapshot; }
    void setIdProdus(int id) { idProdus = id; }
    void setTipProdus(const std::string &tip) { tipProdus = tip; }
    void setCantitate(int cant) { cantitate = cant; }
    void setNumeSnapshot(const std::string &nume) { nume_snapshot = nume; }
    void setPretUnitSnapshot(double pret) { pret_unit_snapshot = pret; }
    void setKcalUnitSnapshot(int kcal) { kcal_unit_snapshot = kcal; }
};
class OrderRow
{
    int idComanda = 0;
    int idRestaurant = 0;
    int idLivrator = 0;
    bool finalizata = false;
    bool livrata = false;
    double pretTotal = 0.0;
    std::vector<OrderItemSnap> items;

public:
    int getIdComanda() const { return idComanda; }
    int getIdRestaurant() const { return idRestaurant; }
    int getIdLivrator() const { return idLivrator; }
    bool isFinalizata() const { return finalizata; }
    bool isLivrata() const { return livrata; }
    double getPretTotal() const { return pretTotal; }
    const std::vector<OrderItemSnap> &getItems() const { return items; }
    std::vector<OrderItemSnap> &getItems() { return items; }
    void addItem(const OrderItemSnap &it) { items.push_back(it); }
    void addItem(OrderItemSnap &&it) { items.push_back(std::move(it)); }
    void setItems(std::vector<OrderItemSnap> v) { items = std::move(v); }
    void setIdComanda(int id) { idComanda = id; }
    void setIdRestaurant(int id) { idRestaurant = id; }
    void setIdLivrator(int id) { idLivrator = id; }
    void setFinalizata(bool f) { finalizata = f; }
    void setLivrata(bool l) { livrata = l; }
    void setPretTotal(double p) { pretTotal = p; }
};

class ClientOrdersView
{
    int clientId = 0;
    std::string nume;
    std::string prenume;
    std::vector<OrderRow> comenzi;

public:
    ClientOrdersView() = default;
    ClientOrdersView(int id, std::string n, std::string p, std::vector<OrderRow> c = {})
        : clientId(id), nume(std::move(n)), prenume(std::move(p)), comenzi(std::move(c)) {}
    int getClientId() const { return clientId; }
    std::string getNume() const { return nume; }
    std::string getPrenume() const { return prenume; }
    const std::vector<OrderRow> &getComenzi() const { return comenzi; }
    std::vector<OrderRow> &getComenzi() { return comenzi; }
    void addComanda(const OrderRow &r) { comenzi.push_back(r); }
    void addComanda(OrderRow &&r) { comenzi.push_back(std::move(r)); }
    void setComenzi(std::vector<OrderRow> v) { comenzi = std::move(v); }
    void setClientId(int id) { clientId = id; }
    void setNume(const std::string &n) { nume = n; }
    void setPrenume(const std::string &p) { prenume = p; }
};

class Aplicatie
{
    std::vector<std::unique_ptr<Restaurant>> restaurante;
    std::vector<std::unique_ptr<Client>> clienti;
    std::vector<std::unique_ptr<Comanda>> comenzi;
    inline static int idCounter = 0;

public:
    static Aplicatie &getInstance()
    {
        if (!instance)
        {
            instance = new Aplicatie();
        }
        return *instance;
    }
    Aplicatie(const Aplicatie &) = delete;
    Aplicatie &operator=(const Aplicatie &) = delete;
    void push(std::unique_ptr<Restaurant> p)
    {
        if (p)
            restaurante.emplace_back(std::move(p));
    }
    void push(std::unique_ptr<Client> p)
    {
        if (p)
            clienti.emplace_back(std::move(p));
    }
    void push(std::unique_ptr<Comanda> p)
    {
        if (p)
            comenzi.emplace_back(std::move(p));
    }

    template <class T>
    void push(std::unique_ptr<T> p)
    {
        throw UnsupportedAdd(std::string("Aplicatie::operator+= nu suporta acest tip: ") + typeid(T).name());
    }
    void afiseazaRestaurante(std::ostream &os = std::cout) const;
    void printClientsOrders(const std::vector<ClientOrdersView> &data, std::ostream &os = std::cout);

private:
    Aplicatie() = default;
    inline static Aplicatie *instance = nullptr;

public:
    ~Aplicatie() { std::cout << "Aplicatie a fost inchisa\n"; }

    template <class T>
    Aplicatie &operator+=(std::unique_ptr<T> p)
    {
        push(std::move(p));
        return *this;
    }

    std::vector<std::unique_ptr<Restaurant>> &getRestaurante()
    {
        return restaurante;
    }
    std::vector<std::unique_ptr<Client>> &getClienti()
    {
        return clienti;
    }
    std::vector<std::unique_ptr<Comanda>> &getComenzi() { return comenzi; }
    static int getNextId()
    {
        return ++idCounter;
    }
};

void Aplicatie::printClientsOrders(const std::vector<ClientOrdersView> &data, std::ostream &os)
{
    using std::fixed;
    using std::left;
    using std::right;
    using std::setfill;
    using std::setprecision;
    using std::setw;

    std::unordered_map<int, std::string> restNameById;
    for (const auto &r : restaurante)
    {
        if (r)
            restNameById[r->getId()] = r->getNume();
    }
    auto restName = [&](int id)
    {
        auto it = restNameById.find(id);
        return (it != restNameById.end()) ? it->second : std::string("(necunoscut)");
    };

    auto line = [&](int n = 80, char ch = '-')
    { os << setfill(ch) << setw(n) << "" << setfill(' ') << "\n"; };

    for (const auto &c : data)
    {
        os << "\nCLIENT #" << c.getClientId() << " — " << c.getNume() << " " << c.getPrenume() << "\n";
        line(80, '=');

        if (c.getComenzi().empty())
        {
            os << "(fără comenzi)\n";
            continue;
        }

        for (const auto &ord : c.getComenzi())
        {
            os << "Comanda #" << ord.getIdComanda()
               << " | Restaurant: " << restName(ord.getIdRestaurant())
               << " | Livrator: " << ord.getIdLivrator()
               << " | Finalizata: " << (ord.isFinalizata() ? "da" : "nu")
               << " | Livrata: " << (ord.isLivrata() ? "da" : "nu")
               << " | Total salvat: " << fixed << setprecision(2) << ord.getPretTotal()
               << "\n";

            os << left << setw(8) << "idItem"
               << setw(10) << "tip"
               << setw(30) << "nume"
               << right << setw(8) << "cant"
               << setw(12) << "pretUnit"
               << setw(8) << "kcal"
               << "\n";
            line();

            double totalRecalculat = 0.0;
            for (const auto &it : ord.getItems())
            {
                os << left << setw(8) << it.getIdItem()
                   << setw(10) << it.getTipProdus()
                   << setw(30) << it.getNumeSnapshot()
                   << right << setw(8) << it.getCantitate()
                   << setw(12) << fixed << setprecision(2) << it.getPretUnitSnapshot()
                   << setw(8) << it.getKcalUnitSnapshot()
                   << "\n";
                totalRecalculat += it.getPretUnitSnapshot() * it.getCantitate();
            }
            if (!ord.getItems().empty())
                os << "Total (recalculat din items): " << fixed << setprecision(2) << totalRecalculat << "\n";

            line();
        }
    }
}

void Aplicatie::afiseazaRestaurante(std::ostream &os) const
{
    using std::fixed;
    using std::left;
    using std::right;
    using std::setfill;
    using std::setprecision;
    using std::setw;

    auto linie = [&](int w = 52, char ch = '-')
    {
        os << setfill(ch) << setw(w) << "" << setfill(' ') << "\n";
    };

    os << "\n=== Restaurante ===\n";
    if (restaurante.empty())
    {
        os << "(nu există restaurante)\n";
        return;
    }

    os << left << setw(6) << "ID"
       << left << setw(36) << "Restaurant"
       << right << setw(10) << "Rating" << "\n";
    linie();

    for (const auto &r : restaurante)
    {
        if (!r)
            continue;
        os << left << setw(6) << r->getId()
           << left << setw(36) << r->getNume()
           << right << setw(10) << fixed << setprecision(1) << r->getRating()
           << "\n";
    }
    linie();
    os << std::flush;
}
enum class ProdusType
{
    Mancare,
    Desert,
    Bautura
};

class ProdusData
{
    int id;
    int idRestaurant;
    std::string nume;
    float pret;
    int kcal;
    std::string tip;
    bool alcool = false;

public:
    int getId() const { return id; }
    int getIdRestaurant() const { return idRestaurant; }
    std::string getNume() const { return nume; }
    float getPret() const { return pret; }
    int getKcal() const { return kcal; }
    std::string getTip() const { return tip; }
    bool isAlcool() const { return alcool; }
    void setId(int i) { id = i; }
    void setIdRestaurant(int i) { idRestaurant = i; }
    void setNume(const std::string &n) { nume = n; }
    void setPret(float p) { pret = p; }
    void setKcal(int k) { kcal = k; }
    void setTip(const std::string &t) { tip = t; }
    void setAlcool(bool a) { alcool = a; }
};

class ProdusFactory
{
public:
    static std::unique_ptr<Produs> create(ProdusType t, const ProdusData &d)
    {
        switch (t)
        {
        case ProdusType::Mancare:
            return std::make_unique<Mancare>(d.getId(), d.getNume(), d.getPret(), d.getKcal(), d.getTip());
        case ProdusType::Desert:
            return std::make_unique<Desert>(d.getId(), d.getNume(), d.getPret(), d.getKcal(), d.getTip());
        case ProdusType::Bautura:
            return std::make_unique<Bautura>(d.getId(), d.getNume(), d.getPret(), d.getKcal(), d.isAlcool());
        }
        throw std::runtime_error("Unknown ProdusType");
    }
};

inline int asInt(const char *s) { return s ? std::stoi(s) : 0; }
inline float asFloat(const char *s) { return s ? std::stof(s) : 0.0f; }
inline std::string asStr(const char *s) { return s ? std::string(s) : ""; }

template <typename Out>
class RowAdapter
{
public:
    virtual ~RowAdapter() = default;
    virtual Out from(MYSQL_ROW row) const = 0;
};

class ClientOrderItemJoinRow
{
    int clientId = 0;
    std::string nume;
    std::string prenume;

    int idComanda = 0;
    int idRestaurant = 0;
    int idLivrator = 0;
    bool finalizata = false;
    bool livrata = false;
    double pretTotal = 0.0;

    int idItem = 0;
    int idProdus = 0;
    std::string tipProdus;
    int cantitate = 0;
    std::string nume_snapshot;
    double pret_unit_snapshot = 0.0;
    int kcal_unit_snapshot = 0;

public:
    int getClientId() const { return clientId; }
    std::string getNume() const { return nume; }
    std::string getPrenume() const { return prenume; }
    int getIdComanda() const { return idComanda; }
    int getIdRestaurant() const { return idRestaurant; }
    int getIdLivrator() const { return idLivrator; }
    bool isFinalizata() const { return finalizata; }
    bool isLivrata() const { return livrata; }
    double getPretTotal() const { return pretTotal; }
    int getIdItem() const { return idItem; }
    int getIdProdus() const { return idProdus; }
    std::string getTipProdus() const { return tipProdus; }
    int getCantitate() const { return cantitate; }
    std::string getNumeSnapshot() const { return nume_snapshot; }
    double getPretUnitSnapshot() const { return pret_unit_snapshot; }
    int getKcalUnitSnapshot() const { return kcal_unit_snapshot; }
    void setClientId(int id) { clientId = id; }
    void setNume(const std::string &n) { nume = n; }
    void setPrenume(const std::string &p) { prenume = p; }
    void setIdComanda(int id) { idComanda = id; }
    void setIdRestaurant(int id) { idRestaurant = id; }
    void setIdLivrator(int id) { idLivrator = id; }
    void setFinalizata(bool f) { finalizata = f; }
    void setLivrata(bool l) { livrata = l; }
    void setPretTotal(double p) { pretTotal = p; }
    void setIdItem(int id) { idItem = id; }
    void setIdProdus(int id) { idProdus = id; }
    void setTipProdus(const std::string &tip) { tipProdus = tip; }
    void setCantitate(int cant) { cantitate = cant; }
    void setNumeSnapshot(const std::string &nume) { nume_snapshot = nume; }
    void setPretUnitSnapshot(double pret) { pret_unit_snapshot = pret; }
    void setKcalUnitSnapshot(int kcal) { kcal_unit_snapshot = kcal; }
};

class ClientOrderItemJoinAdapter : RowAdapter<ClientOrderItemJoinRow>
{
public:
    ClientOrderItemJoinRow from(MYSQL_ROW row) const override
    {
        ClientOrderItemJoinRow r;
        r.setClientId(asInt(row[0]));
        r.setNume(asStr(row[1]));
        r.setPrenume(asStr(row[2]));
        r.setIdComanda(asInt(row[3]));
        r.setIdRestaurant(asInt(row[4]));
        r.setIdLivrator(asInt(row[5]));
        r.setFinalizata(asInt(row[6]) != 0);
        r.setLivrata(asInt(row[7]) != 0);
        {
            std::string s = asStr(row[8]);
            r.setPretTotal(s.empty() ? 0.0 : std::stod(s));
        }
        r.setIdItem(asInt(row[9]));
        r.setIdProdus(asInt(row[10]));
        r.setTipProdus(asStr(row[11]));
        r.setCantitate(asInt(row[12]));
        r.setNumeSnapshot(asStr(row[13]));
        {
            std::string s = asStr(row[14]);
            r.setPretUnitSnapshot(s.empty() ? 0.0 : std::stod(s));
        }
        r.setKcalUnitSnapshot(asInt(row[15]));
        return r;
    }
};

class ClientOrdersAccumulator
{
    std::vector<ClientOrdersView> out;
    int lastClientId = -1;
    int lastOrderId = -1;
    ClientOrdersView *curClient = nullptr;
    OrderRow *curOrder = nullptr;

public:
    void accept(const ClientOrderItemJoinRow &r)
    {
        if (!curClient || r.getClientId() != lastClientId)
        {
            out.emplace_back(r.getClientId(), r.getNume(), r.getPrenume());
            curClient = &out.back();
            lastClientId = r.getClientId();
            curOrder = nullptr;
            lastOrderId = -1;
        }

        if (r.getIdComanda() > 0)
        {
            if (!curOrder || r.getIdComanda() != lastOrderId)
            {
                OrderRow ord;
                ord.setIdComanda(r.getIdComanda());
                ord.setIdRestaurant(r.getIdRestaurant());
                ord.setIdLivrator(r.getIdLivrator());
                ord.setFinalizata(r.isFinalizata());
                ord.setLivrata(r.isLivrata());
                ord.setPretTotal(r.getPretTotal());
                curClient->addComanda(std::move(ord));
                curOrder = &curClient->getComenzi().back();
                lastOrderId = r.getIdComanda();
            }

            if (r.getIdItem() > 0)
            {
                OrderItemSnap it;
                it.setIdItem(r.getIdItem());
                it.setIdComanda(r.getIdComanda());
                it.setIdProdus(r.getIdProdus());
                it.setTipProdus(r.getTipProdus());
                it.setCantitate(r.getCantitate());
                it.setNumeSnapshot(r.getNumeSnapshot());
                it.setPretUnitSnapshot(r.getPretUnitSnapshot());
                it.setKcalUnitSnapshot(r.getKcalUnitSnapshot());
                curOrder->addItem(std::move(it));
            }
        }
    }

    std::vector<ClientOrdersView> take() { return std::move(out); }
};

class LivratorRatingRow
{
    int id = 0;
    std::string nume, prenume;
    double rating = 0.0;
    int nrReviewuri = 0;

public:
    int getId() const { return id; }
    std::string getNume() const { return nume; }
    std::string getPrenume() const { return prenume; }
    double getRating() const { return rating; }
    int getNrReviewuri() const { return nrReviewuri; }
    void setId(int i) { id = i; }
    void setNume(const std::string &n) { nume = n; }
    void setPrenume(const std::string &p) { prenume = p; }
    void setRating(double r) { rating = r; }
    void setNrReviewuri(int n) { nrReviewuri = n; }
};

class BauturaRowAdapter : public RowAdapter<std::unique_ptr<Produs>>
{
public:
    std::unique_ptr<Produs> from(MYSQL_ROW row) const override
    {
        ProdusData d;
        d.setId(asInt(row[0]));
        d.setNume(asStr(row[1]));
        d.setPret(asFloat(row[2]));
        d.setKcal(asInt(row[3]));
        d.setAlcool(row[4] && std::string(row[4]) == "1");
        return ProdusFactory::create(ProdusType::Bautura, d);
    }
};

class MancareRowAdapter : public RowAdapter<std::unique_ptr<Produs>>
{
public:
    std::unique_ptr<Produs> from(MYSQL_ROW row) const override
    {
        ProdusData d;
        d.setId(asInt(row[0]));
        d.setNume(asStr(row[1]));
        d.setPret(asFloat(row[2]));
        d.setKcal(asInt(row[3]));
        d.setTip(asStr(row[4]));
        return ProdusFactory::create(ProdusType::Mancare, d);
    }
};

class DesertRowAdapter : public RowAdapter<std::unique_ptr<Produs>>
{
public:
    std::unique_ptr<Produs> from(MYSQL_ROW row) const override
    {
        ProdusData d;
        d.setId(asInt(row[0]));
        d.setNume(asStr(row[1]));
        d.setPret(asFloat(row[2]));
        d.setKcal(asInt(row[3]));
        d.setTip(asStr(row[4]));
        return ProdusFactory::create(ProdusType::Desert, d);
    }
};

class DesertRowAdapter5 : public RowAdapter<std::unique_ptr<Produs>>
{
public:
    std::unique_ptr<Produs> from(MYSQL_ROW row) const override
    {
        ProdusData d;
        d.setId(asInt(row[0]));
        d.setNume(asStr(row[1]));
        d.setPret(asFloat(row[2]));
        d.setKcal(asInt(row[3]));
        d.setTip(asStr(row[4]));
        return ProdusFactory::create(ProdusType::Desert, d);
    }
};

class ClientRowAdapter : public RowAdapter<std::unique_ptr<Client>>
{
public:
    std::unique_ptr<Client> from(MYSQL_ROW row) const override
    {
        int id = asInt(row[0]);
        std::string nume = asStr(row[1]);
        std::string prenume = asStr(row[2]);
        std::string password = asStr(row[3]);
        std::string categorie = asStr(row[4]);
        return std::make_unique<Client>(id, nume, prenume, password, categorie);
    }
};

class StaffRowAdapter : public RowAdapter<std::unique_ptr<Staff>>
{
public:
    std::unique_ptr<Staff> from(MYSQL_ROW row) const override
    {
        int idRestaurant = asInt(row[0]);
        int id = asInt(row[1]);
        std::string nume = asStr(row[2]);
        std::string prenume = asStr(row[3]);
        std::string tip = asStr(row[4]);

        if (tip == "Bucatar")
            return std::make_unique<Bucatar>(idRestaurant, nume, prenume, id);
        else if (tip == "Livrator")
            return std::make_unique<Livrator>(idRestaurant, nume, prenume, id);
        else if (tip == "Casier")
            return std::make_unique<Casier>(idRestaurant, nume, prenume, id);
        else if (tip == "Supervisor")
            return std::make_unique<Supervisor>(idRestaurant, nume, prenume, id);

        throw std::runtime_error("Unknown staff type: " + tip);
    }
};

class LivratorRatingRowAdapter : public RowAdapter<LivratorRatingRow>
{
public:
    LivratorRatingRow from(MYSQL_ROW row) const override
    {
        LivratorRatingRow r;
        r.setId(asInt(row[0]));
        r.setNume(asStr(row[1]));
        r.setPrenume(asStr(row[2]));
        std::string s = asStr(row[3]);
        r.setRating(s.empty() ? 0.0 : std::stod(s));
        r.setNrReviewuri(asInt(row[4]));
        return r;
    }
};

class reviewComandaRowAdapter : public RowAdapter<std::unique_ptr<ReviewComanda>>
{
public:
    std::unique_ptr<ReviewComanda> from(MYSQL_ROW row) const override
    {
        int idComanda = asInt(row[0]);
        int rating = asInt(row[1]);
        int idClient = asInt(row[2]);
        std::string comentariu = asStr(row[3]);
        return std::make_unique<ReviewComanda>(idComanda, rating, idClient, comentariu);
    }
};

class reviewLivratorRowAdapter : public RowAdapter<std::unique_ptr<ReviewLivrator>>
{
public:
    std::unique_ptr<ReviewLivrator> from(MYSQL_ROW row) const override
    {
        int idComanda = asInt(row[0]);
        int rating = asInt(row[1]);
        int idClient = asInt(row[2]);
        int notaLivrator = asInt(row[3]);
        return std::make_unique<ReviewLivrator>(idComanda, rating, idClient, notaLivrator);
    }
};

class DatabaseFacade
{
    MYSQL *conn = nullptr;
    static void die(const std::string &where, MYSQL *c)
    {
        throw std::runtime_error(where + " failed: " + (c ? mysql_error(c) : "no connection"));
    }

public:
    DatabaseFacade(const char *host, const char *user, const char *pass, const char *db, unsigned port = 3306)
    {
        conn = mysql_init(nullptr);
        if (!conn)
            throw std::runtime_error("mysql_init failed");
        if (!mysql_real_connect(conn, host, user, pass, db, port, nullptr, 0))
            die("connect", conn);
    }
    ~DatabaseFacade()
    {
        if (conn)
            mysql_close(conn);
    }

    std::vector<std::unique_ptr<Client>> getAllClients()
    {
        const char *q = "SELECT id, nume, prenume, password, categorie FROM client";
        if (mysql_query(conn, q))
            die("query client", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result client", conn);

        std::vector<std::unique_ptr<Client>> v;
        MYSQL_ROW row;
        ClientRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Produs>> getAllBauturi()
    {
        const char *q = "SELECT id,name,pret,kcal,alcool FROM bautura";
        if (mysql_query(conn, q))
            die("query bautura", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result bautura", conn);

        std::vector<std::unique_ptr<Produs>> v;
        MYSQL_ROW row;
        BauturaRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Produs>> getAllMancaruri()
    {
        const char *q = "SELECT id,name,pret,kcal,tip FROM mancare";
        if (mysql_query(conn, q))
            die("query mancare", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result mancare", conn);

        std::vector<std::unique_ptr<Produs>> v;
        MYSQL_ROW row;
        MancareRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Produs>> getAllDeserturi()
    {
        const char *q = "SELECT id, idRestaurant, name,pret,kcal,tip FROM desert";
        if (mysql_query(conn, q))
            die("query desert", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result desert", conn);

        std::vector<std::unique_ptr<Produs>> v;
        MYSQL_ROW row;
        DesertRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<ReviewComanda>> getAllReviewComandaByIDClient(int idClient)
    {
        const std::string q =
            "SELECT idComanda, nota, idClient, comentariu FROM reviewComanda WHERE idClient = " + std::to_string(idClient);
        if (mysql_query(conn, q.c_str()))
            die("query reviewComanda by client", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result reviewComanda by client", conn);

        std::vector<std::unique_ptr<ReviewComanda>> v;
        MYSQL_ROW row;
        reviewComandaRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<ReviewLivrator>> getAllReviewLivratorByIDClient(int idClient)
    {
        const std::string q =
            "SELECT idComanda, nota, idClient, notaLivrator FROM reviewLivrator WHERE idClient = " + std::to_string(idClient);
        if (mysql_query(conn, q.c_str()))
            die("query reviewLivrator by client", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result reviewLivrator by client", conn);

        std::vector<std::unique_ptr<ReviewLivrator>> v;
        MYSQL_ROW row;
        reviewLivratorRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Restaurant>> getAllRestaurants()
    {
        const char *q = "SELECT id,name,rating FROM restaurant";
        if (mysql_query(conn, q))
            die("query restaurant", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result restaurant", conn);

        std::vector<std::unique_ptr<Restaurant>> v;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            int id = asInt(row[0]);
            std::string name = asStr(row[1]);
            float rating = asFloat(row[2]);
            v.emplace_back(std::make_unique<Restaurant>(id, name, rating));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Produs>> getAllDesertsbyRestaurant(int idRestaurant)
    {

        const std::string q = "SELECT id, name, pret, kcal, tip FROM desert WHERE idRestaurant = " + std::to_string(idRestaurant);
        if (mysql_query(conn, q.c_str()))
            die("query desert by restaurant", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result desert by restaurant", conn);

        MYSQL_ROW row;
        DesertRowAdapter5 adapter;
        std::vector<std::unique_ptr<Produs>> v;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Produs>> getAllMancaribyRestaurant(int idRestaurant)
    {
        const std::string q = "SELECT id, name, pret, kcal, tip FROM mancare WHERE idRestaurant = " + std::to_string(idRestaurant);
        if (mysql_query(conn, q.c_str()))
            die("query mancare by restaurant", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result mancare by restaurant", conn);
        std::vector<std::unique_ptr<Produs>> v;
        MYSQL_ROW row;
        MancareRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Produs>> getAllBauturibyRestaurant(int idRestaurant)
    {
        const std::string q = "SELECT id, name, pret, kcal, alcool FROM bautura WHERE idRestaurant = " + std::to_string(idRestaurant);
        if (mysql_query(conn, q.c_str()))
            die("query bautura by restaurant", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result bautura by restaurant", conn);

        std::vector<std::unique_ptr<Produs>> v;
        MYSQL_ROW row;
        BauturaRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Staff>> getAllStaff()
    {
        const char *q = "SELECT idRestaurant, id, nume, prenume, tip FROM staff";
        if (mysql_query(conn, q))
            die("query staff", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result staff", conn);

        std::vector<std::unique_ptr<Staff>> v;
        MYSQL_ROW row;
        StaffRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Staff>> getStaffByRestaurant(int idRestaurant)
    {
        const std::string q = "SELECT idRestaurant, id, nume, prenume, tip FROM staff WHERE idRestaurant = " + std::to_string(idRestaurant);
        if (mysql_query(conn, q.c_str()))
            die("query staff by restaurant", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result staff by restaurant", conn);

        std::vector<std::unique_ptr<Staff>> v;
        MYSQL_ROW row;
        StaffRowAdapter adapter;
        while ((row = mysql_fetch_row(res)))
        {
            v.emplace_back(adapter.from(row));
        }
        mysql_free_result(res);
        return v;
    }

    void addBautura(int id, int idRestaurant, const std::string &name, float pret, int kcal, bool alcool)
    {
        std::string q = "INSERT INTO bautura(id, idRestaurant, name,pret,kcal,alcool) VALUES(" +
                        std::to_string(id) + "," + std::to_string(idRestaurant) + ",'" + name + "'," + std::to_string(pret) + "," +
                        std::to_string(kcal) + "," + (alcool ? "1" : "0") + ")";
        if (mysql_query(conn, q.c_str()))
            die("insert bautura", conn);
    }
    void addMancare(int id, int idRestaurant, const std::string &name, float pret, int kcal, const std::string &tip)
    {
        std::string q = "INSERT INTO mancare(id, idRestaurant, name,pret,kcal,tip) VALUES(" +
                        std::to_string(id) + "," + std::to_string(idRestaurant) + ",'" + name + "'," + std::to_string(pret) + "," +
                        std::to_string(kcal) + ",'" + tip + "')";
        if (mysql_query(conn, q.c_str()))
            die("insert mancare", conn);
    }
    void addDesert(int id, int idRestaurant, const std::string &name, float pret, int kcal, const std::string &tip)
    {
        std::string q = "INSERT INTO desert(id, idRestaurant, name,pret,kcal,tip) VALUES(" +
                        std::to_string(id) + "," + std::to_string(idRestaurant) + ",'" + name + "'," + std::to_string(pret) + "," +
                        std::to_string(kcal) + ",'" + tip + "')";
        if (mysql_query(conn, q.c_str()))
            die("insert desert", conn);
    }
    void addRestaurant(int id, const std::string &name, float rating = 0.0f)
    {
        std::string q = "INSERT INTO restaurant(id,name,rating) VALUES(" +
                        std::to_string(id) + ",'" + name + "'," + std::to_string(rating) + ")";
        if (mysql_query(conn, q.c_str()))
            die("insert restaurant", conn);
    }
    unsigned long long addClient(const std::string &nume, const std::string &prenume, const std::string &parola)
    {
        std::string q = "INSERT INTO client(nume,prenume,`password`) VALUES(" + esc(nume) + "," + esc(prenume) + "," + esc(parola) + ")";
        if (mysql_query(conn, q.c_str()))
            die("insert client", conn);
        return mysql_insert_id(conn);
    }

    void addStaff(int idRestaurant, const std::string &nume, const std::string &prenume, const std::string &tip, int id)
    {
        std::string q = "INSERT INTO staff(idRestaurant,id,nume,prenume,tip) VALUES(" +
                        std::to_string(idRestaurant) + "," + std::to_string(id) + ",'" + nume + "','" +
                        prenume + "','" + tip + "')";
        if (mysql_query(conn, q.c_str()))
            die("insert staff", conn);
    }

    int addComanda(int idRestaurant, int idClient, int idLivrator = 0, bool finalizata = false, bool livrata = false);

    void addComandaItem(int idComanda, const Produs *p, int cantitate = 1);

    std::string esc(const std::string &s)
    {
        std::string out;
        out.resize(s.size() * 2 + 1);
        unsigned long n = mysql_real_escape_string(conn, &out[0], s.c_str(), (unsigned long)s.size());
        out.resize(n);
        return "'" + out + "'";
    }

    void updateComandaFinalizata(int idComanda, bool fin)
    {
        std::string q = "UPDATE comanda SET finalizata=" + std::to_string(fin ? 1 : 0) +
                        " WHERE idComanda=" + std::to_string(idComanda);
        if (mysql_query(conn, q.c_str()))
            die("update comanda.finalizata", conn);
    }

    unsigned long long addComanda(int idRestaurant, int idClient, double total)
    {
        std::ostringstream tot;
        tot << std::fixed << std::setprecision(2) << total;

        std::string q = "INSERT INTO comanda(idRestaurant, idClient, pretTotal) VALUES(" + std::to_string(idRestaurant) + "," + std::to_string(idClient) + "," + tot.str() + ")";

        if (mysql_query(conn, q.c_str()))
            die("insert comanda", conn);

        return mysql_insert_id(conn);
    }
    /**
     * @brief Adaugă un item în comanda_item cu snapshot al produsului
     */
    void addComandaItemSnapshot(unsigned long long idComanda, int idProdus, const std::string &tipProdus, int cantitate, const std::string &nume_snapshot, double pret_unit_snapshot, int kcal_unit_snapshot)
    {
        std::ostringstream pret;
        pret << std::fixed << std::setprecision(2) << pret_unit_snapshot;

        std::string q =
            "INSERT INTO comanda_item("
            "idComanda, idProdus, tipProdus, cantitate, "
            "nume_snapshot, pret_unit_snapshot, kcal_unit_snapshot) VALUES(" +
            std::to_string(idComanda) + "," + std::to_string(idProdus) + "," + esc(tipProdus) + "," + std::to_string(cantitate) + "," + esc(nume_snapshot) + "," + pret.str() + "," + std::to_string(kcal_unit_snapshot) + ")";

        if (mysql_query(conn, q.c_str()))
            die("insert comanda_item", conn);
    }

    std::vector<ClientOrdersView> getAllClientsWithOrdersAndItems();
    void updateClientPassword(int idClient, const std::string &newPassword);
    std::vector<OrderItemDB> getOrderItems(int idComanda)
    {
        std::string q =
            "SELECT idProdus, tipProdus, cantitate, "
            "       nume_snapshot, pret_unit_snapshot, kcal_unit_snapshot "
            "FROM comanda_item WHERE idComanda = " +
            std::to_string(idComanda) +
            " ORDER BY idItem";
        if (mysql_query(conn, q.c_str()))
            die("query comanda_item", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result comanda_item", conn);

        std::vector<OrderItemDB> v;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            OrderItemDB it;
            it.setIdProdus(asInt(row[0]));
            it.setTipProdus(asStr(row[1]));
            it.setCantitate(asInt(row[2]));
            it.setNumeSnapshot(asStr(row[3]));
            {
                std::string s = asStr(row[4]);
                it.setPretUnitSnapshot(s.empty() ? 0.0 : std::stod(s));
            }
            it.setKcalUnitSnapshot(asInt(row[5]));
            v.push_back(std::move(it));
        }
        mysql_free_result(res);
        return v;
    }

    std::vector<std::unique_ptr<Comanda>> getAllOrders()
    {
        const char *q =
            "SELECT idComanda, idRestaurant, idClient, COALESCE(idLivrator,0), "
            "       COALESCE(finalizata,0), COALESCE(livrata,0) "
            "FROM comanda";

        if (mysql_query(conn, q))
            die("query comanda", conn);
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            die("store_result comanda", conn);

        std::vector<std::unique_ptr<Comanda>> v;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            int idCom = asInt(row[0]);
            int idRest = asInt(row[1]);
            int idCli = asInt(row[2]);
            int idLiv = asInt(row[3]);
            bool fin = asInt(row[4]) != 0;
            bool liv = asInt(row[5]) != 0;

            auto c = std::make_unique<Comanda>(idCom, idRest, idCli, idLiv);
            c->setFinalizata(fin);
            if (liv)
                c->makeLivrata();
            v.emplace_back(std::move(c));
        }
        mysql_free_result(res);
        return v;
    }
    void updateComandaLivrata(int idComanda, bool livrata);
    void updateClientCategory(int idClient, const std::string &categorie);
    void insertReviewComanda(int idComanda, int idClient, int nota, const std::string &comentariu);
    void insertReviewLivrator(int idComanda, int idClient, int nota, int notaLivrator);
    void updateRestaurantRating(int idRestaurant, double rating);
    double recomputeAndUpdateRestaurantRating(int idRestaurant);
    bool wasOrderRatedByClient(int idComanda, int idClient);
    std::vector<LivratorRatingRow> getLivratoriCuRatingMediu(int idRestaurant);
    std::vector<int> concediazaLivratoriSubPrag(int idRestaurant, double prag);
    void updateComandaLivrator(int idComanda, int idLivrator);
    void discountProductsForLowRatedRestaurants(double ratingThreshold = 3.0, double factor = 0.8);
};
void DatabaseFacade::discountProductsForLowRatedRestaurants(double maxRating, double factor)
{
    std::ostringstream th;
    th << std::fixed << std::setprecision(1) << maxRating;
    std::ostringstream f;
    f << std::fixed << std::setprecision(3) << factor;

    auto run = [&](const std::string &table)
    {
        std::string q =
            "UPDATE " + table + " t "
                                "JOIN restaurant r ON t.idRestaurant = r.id "
                                "SET t.pret = ROUND(t.pret * " +
            f.str() + ", 2) "
                      "WHERE r.rating IS NOT NULL "
                      "AND r.rating >= 1.0 "
                      "AND r.rating <= " +
            th.str();

        if (mysql_query(conn, q.c_str()))
            die("discount " + table, conn);
    };

    run("mancare");
    run("desert");
    run("bautura");
}

void DatabaseFacade::updateComandaLivrator(int idComanda, int idLivrator)
{
    std::string q =
        "UPDATE comanda SET idLivrator=" + std::to_string(idLivrator) +
        " WHERE idComanda=" + std::to_string(idComanda);
    if (mysql_query(conn, q.c_str()))
        die("update comanda.idLivrator", conn);
}

/**
 * @brief Returnează toți livratorii dintr-un restaurant, împreună cu ratingul mediu și numărul de reviewuri
 */
std::vector<LivratorRatingRow> DatabaseFacade::getLivratoriCuRatingMediu(int idRestaurant)
{
    std::string q =
        "SELECT s.id, s.nume, s.prenume, "
        "       AVG(rl.notaLivrator) AS rating_mediu, "
        "       COUNT(rl.notaLivrator) AS nr_reviewuri "
        "FROM staff s "
        "LEFT JOIN comanda c "
        "       ON c.idLivrator = s.id AND c.idRestaurant = s.idRestaurant "
        "LEFT JOIN reviewLivrator rl "
        "       ON rl.idComanda = c.idComanda "
        "WHERE s.idRestaurant = " +
        std::to_string(idRestaurant) + " AND s.tip = 'Livrator' "
                                       "GROUP BY s.id, s.nume, s.prenume "
                                       "ORDER BY s.nume, s.prenume";

    if (mysql_query(conn, q.c_str()))
        die("query livratori + rating mediu", conn);

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
        die("store_result livratori + rating mediu", conn);

    std::vector<LivratorRatingRow> v;
    MYSQL_ROW row;
    LivratorRatingRowAdapter adapter;
    while ((row = mysql_fetch_row(res)))
    {
        v.emplace_back(adapter.from(row));
    }
    mysql_free_result(res);
    return v;
}
/**
 * @brief Concediază toți livratorii dintr-un restaurant care au rating mediu sub un prag dat
 * @return Lista ID-urilor livratorilor concediați
 */
std::vector<int> DatabaseFacade::concediazaLivratoriSubPrag(int idRestaurant, double prag)
{
    std::string q_sel =
        "SELECT s.id "
        "FROM staff s "
        "LEFT JOIN comanda c "
        "       ON c.idLivrator = s.id AND c.idRestaurant = s.idRestaurant "
        "LEFT JOIN reviewLivrator rl "
        "       ON rl.idComanda = c.idComanda "
        "WHERE s.idRestaurant = " +
        std::to_string(idRestaurant) + " AND s.tip = 'Livrator' "
                                       "GROUP BY s.id "
                                       "HAVING COUNT(rl.notaLivrator) > 0 AND AVG(rl.notaLivrator) <= " +
        std::to_string(prag);

    if (mysql_query(conn, q_sel.c_str()))
        die("select livratori sub prag", conn);

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
        die("store_result livratori sub prag", conn);

    std::vector<int> deConcediat;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        deConcediat.push_back(asInt(row[0]));
    }
    mysql_free_result(res);

    if (deConcediat.empty())
        return deConcediat;

    std::ostringstream inlist;
    for (size_t i = 0; i < deConcediat.size(); ++i)
    {
        if (i)
            inlist << ',';
        inlist << deConcediat[i];
    }

    std::string q_del =
        "DELETE FROM staff "
        "WHERE idRestaurant = " +
        std::to_string(idRestaurant) +
        "  AND tip = 'Livrator' "
        "  AND id IN (" +
        inlist.str() + ")";

    if (mysql_query(conn, q_del.c_str()))
        die("delete livratori sub prag", conn);

    return deConcediat;
}

void DatabaseFacade::updateClientCategory(int idClient, const std::string &categorie)
{
    std::string q =
        "UPDATE client SET categorie = " + esc(categorie) +
        " WHERE id = " + std::to_string(idClient);
    if (mysql_query(conn, q.c_str()))
        die("update client.categorie", conn);
}

void DatabaseFacade::updateComandaLivrata(int idComanda, bool livr)
{
    std::string q = "UPDATE comanda SET livrata=" + std::to_string(livr ? 1 : 0) +
                    " WHERE idComanda=" + std::to_string(idComanda);
    if (mysql_query(conn, q.c_str()))
        die("update comanda.livrata", conn);
}

void DatabaseFacade::updateClientPassword(int idClient, const std::string &newPassword)
{
    std::string q =
        "UPDATE client SET password = " + esc(newPassword) +
        " WHERE id = " + std::to_string(idClient);

    if (mysql_query(conn, q.c_str()))
    {
        unsigned err = mysql_errno(conn);
        if (err == ER_BAD_FIELD_ERROR)
        {
            q = "UPDATE client SET parola = " + esc(newPassword) +
                " WHERE id = " + std::to_string(idClient);
            if (mysql_query(conn, q.c_str()))
                die("update client password (parola)", conn);
        }
        else
        {
            die("update client password", conn);
        }
    }

    auto affected = mysql_affected_rows(conn);
    if (affected == 0)
    {
        throw std::runtime_error(
            "Nu s-a actualizat nicio înregistrare (id inexistent sau parola identică).");
    }
}

std::vector<ClientOrdersView> DatabaseFacade::getAllClientsWithOrdersAndItems()
{

    const char *q =
        "SELECT cl.id AS clientId, cl.nume, cl.prenume, "
        "       c.idComanda, c.idRestaurant, c.idLivrator, c.finalizata, c.livrata, "
        "       COALESCE(c.pretTotal, 0) AS pretTotal, "
        "       ci.idItem, ci.idProdus, ci.tipProdus, ci.cantitate, "
        "       ci.nume_snapshot, ci.pret_unit_snapshot, ci.kcal_unit_snapshot "
        "FROM client cl "
        "LEFT JOIN comanda c       ON c.idClient = cl.id "
        "LEFT JOIN comanda_item ci ON ci.idComanda = c.idComanda "
        "ORDER BY cl.id, c.idComanda, ci.idItem";

    if (mysql_query(conn, q))
        die("query clients+orders+items", conn);
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
        die("store_result clients+orders+items", conn);

    ClientOrderItemJoinAdapter adapter;
    ClientOrdersAccumulator acc;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        acc.accept(adapter.from(row));
    }
    mysql_free_result(res);
    return acc.take();
}

int DatabaseFacade::addComanda(int idRestaurant, int idClient, int idLivrator,
                               bool finalizata, bool livrata)
{
    std::string q = "INSERT INTO comanda(idRestaurant,idClient,idLivrator,finalizata,livrata) VALUES(" + std::to_string(idRestaurant) + "," + std::to_string(idClient) + "," + std::to_string(idLivrator) + "," + std::to_string(finalizata ? 1 : 0) + "," + std::to_string(livrata ? 1 : 0) + ")";
    if (mysql_query(conn, q.c_str()))
        die("insert comanda", conn);

    return (int)mysql_insert_id(conn);
}

void DatabaseFacade::addComandaItem(int idComanda, const Produs *p, int cantitate)
{
    if (!p)
        throw std::runtime_error("addComandaItem: produs null");

    std::string tip;
    if (dynamic_cast<const Mancare *>(p))
        tip = "Mancare";
    else if (dynamic_cast<const Desert *>(p))
        tip = "Desert";
    else if (dynamic_cast<const Bautura *>(p))
        tip = "Bautura";
    else
        throw std::runtime_error("addComandaItem: tip necunoscut");

    std::string q = "INSERT INTO comanda_item"
                    "(idComanda,idProdus,tipProdus,cantitate,nume_snapshot,pret_unit_snapshot,kcal_unit_snapshot) VALUES(" +
                    std::to_string(idComanda) + "," + std::to_string(p->getId()) + "," + esc(tip) + "," + std::to_string(cantitate) + "," + esc(p->getNume()) + "," + std::to_string(p->getPret()) + "," + std::to_string(p->getKcal()) + ")";

    if (mysql_query(conn, q.c_str()))
        die("insert comanda_item", conn);
}

void DatabaseFacade::insertReviewComanda(int idComanda, int idClient, int nota, const std::string &comentariu)
{
    if (wasOrderRatedByClient(idComanda, idClient))
        throw std::runtime_error("Comanda deja evaluata - nu poti adauga alt review.");

    std::string q =
        "INSERT INTO reviewComanda(idComanda, nota, idClient, comentariu) VALUES(" +
        std::to_string(idComanda) + "," + std::to_string(nota) + "," +
        std::to_string(idClient) + "," + esc(comentariu) + ")";
    if (mysql_query(conn, q.c_str()))
        die("insert reviewComanda", conn);
}

void DatabaseFacade::insertReviewLivrator(int idComanda, int idClient, int nota, int notaLivrator)
{

    std::string q =
        "INSERT INTO reviewLivrator(idComanda, nota, idClient, notaLivrator) VALUES(" +
        std::to_string(idComanda) + "," + std::to_string(nota) + "," +
        std::to_string(idClient) + "," + std::to_string(notaLivrator) + ")";
    if (mysql_query(conn, q.c_str()))
        die("insert reviewLivrator", conn);
}

void DatabaseFacade::updateRestaurantRating(int idRestaurant, double rating)
{
    std::ostringstream r;
    r << std::fixed << std::setprecision(1) << rating; // păstrăm o zecimală
    std::string q = "UPDATE restaurant SET rating=" + r.str() +
                    " WHERE id=" + std::to_string(idRestaurant);
    if (mysql_query(conn, q.c_str()))
        die("update restaurant.rating", conn);
}

/**
 * @brief Recalculează ratingul unui restaurant pe baza review-urilor din reviewComanda și actualizează câmpul în DB
 * @return Noul rating (0.0 dacă nu există review-uri)
 */
double DatabaseFacade::recomputeAndUpdateRestaurantRating(int idRestaurant)
{
    std::string q =
        "SELECT AVG(rc.nota) "
        "FROM reviewComanda rc "
        "JOIN comanda c ON c.idComanda = rc.idComanda "
        "WHERE c.idRestaurant = " +
        std::to_string(idRestaurant);

    if (mysql_query(conn, q.c_str()))
        die("avg reviewComanda per restaurant", conn);
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
        die("store_result avg reviewComanda", conn);

    MYSQL_ROW row = mysql_fetch_row(res);
    double avg = 0.0;
    if (row && row[0])
    {
        std::string s = row[0];
        if (!s.empty())
            avg = std::stod(s);
    }
    mysql_free_result(res);

    updateRestaurantRating(idRestaurant, avg);
    return avg;
}

bool DatabaseFacade::wasOrderRatedByClient(int idComanda, int idClient)
{
    std::string q =
        "SELECT "
        " (EXISTS(SELECT 1 FROM reviewComanda  "
        "          WHERE idComanda=" +
        std::to_string(idComanda) +
        "            AND idClient=" + std::to_string(idClient) +
        "            AND COALESCE(nota,0)<>0) "
        "  OR "
        "  EXISTS(SELECT 1 FROM reviewLivrator "
        "          WHERE idComanda=" +
        std::to_string(idComanda) +
        "            AND idClient=" + std::to_string(idClient) +
        "            AND COALESCE(nota,0)<>0)) AS rated";

    if (mysql_query(conn, q.c_str()))
        die("exists review for order+client", conn);

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
        die("store_result exists review", conn);

    MYSQL_ROW row = mysql_fetch_row(res);
    bool rated = false;
    if (row && row[0])
        rated = (std::string(row[0]) == "1");

    mysql_free_result(res);
    return rated;
}

class BadCredentials : public std::runtime_error
{
public:
    explicit BadCredentials(const std::string &msg)
        : std::runtime_error(msg) {}

    explicit BadCredentials(const char *msg)
        : std::runtime_error(msg) {}

    ~BadCredentials() override = default;
};

class CategoryStrategy
{
public:
    virtual ~CategoryStrategy() = default;
    virtual std::string assign(double stotal) const = 0;
};

/**
 * @brief Strategie de atribuire a categoriilor cu 3 praguri
 * - Gold: 600+
 * - Silver: 400+
 * - Bronze: 200+
 * - Basic: sub 200
 */
class ThreeCategoryStrategy : public CategoryStrategy
{
public:
    std::string assign(double stotal) const override
    {
        if (stotal >= 600.0)
            return "Gold";
        if (stotal >= 400.0)
            return "Silver";
        if (stotal >= 200.0)
            return "Bronze";
        return "Basic";
    }
};

class CategoryContext
{
    const CategoryStrategy &strat;

public:
    explicit CategoryContext(const CategoryStrategy &s) : strat(s) {}
    std::string choose(double stotal) const { return strat.assign(stotal); }
};

class Admin
{
    const std::string username = "admin12!";
    const std::string password = "hello12!";

public:
    static Admin &getInstance()
    {
        if (!instance)
        {
            instance = new Admin();
        }
        return *instance;
    }
    Admin(const Admin &) = delete;
    Admin &operator=(const Admin &) = delete;
    void authenticate(const std::string &u, const std::string &p) const
    {
        if (u != username || p != password)
        {
            throw BadCredentials("Autentificare esuata: username sau parola incorecta.");
        }
    }

private:
    Admin() = default;
    inline static Admin *instance = nullptr;

public:
    ~Admin() = default;
};

int main()
{
    Admin *admin = &Admin::getInstance();
    Aplicatie &app = Aplicatie::getInstance();
    try
    {
        DBConfig cfg = load_db_config(); 
        DatabaseFacade db(cfg.getServer().c_str(), cfg.getUser().c_str(), cfg.getPass().c_str(), cfg.getName().c_str());

        auto restaurants = db.getAllRestaurants();
        for (auto &r : restaurants)
        {
            int idRestaurant = r->getId();
            auto deserturi = db.getAllDesertsbyRestaurant(idRestaurant);
            auto mancaruri = db.getAllMancaribyRestaurant(idRestaurant);
            auto bauturi = db.getAllBauturibyRestaurant(idRestaurant);
            for (auto &d : deserturi)
            {
                r->addProdus(d.release());
            }
            for (auto &m : mancaruri)
            {
                r->addProdus(m.release());
            }
            for (auto &b : bauturi)
            {
                r->addProdus(b.release());
            }
            auto staff = db.getStaffByRestaurant(idRestaurant);
            for (auto &s : staff)
            {
                r->addStaff(s.release());
            }
            app += std::move(r);
        }

        auto clienti = db.getAllClients();
        for (auto &c : clienti)
        {
            auto reviewsComanda = db.getAllReviewComandaByIDClient(c->getId());
            for (auto &rc : reviewsComanda)
            {
                c->addReviewComanda(std::move(rc));
            }
            auto reviewsLivrator = db.getAllReviewLivratorByIDClient(c->getId());
            for (auto &rl : reviewsLivrator)
            {
                c->addReviewLivrator(std::move(rl));
            }
            app += std::move(c);
        }

        auto comenziDB = db.getAllOrders();
        for (auto &c : comenziDB)
        {
            app += std::move(c);
        }

        for (auto &cup : app.getComenzi())
        {
            Comanda *c = cup.get();
            if (!c)
                continue;

            Restaurant *restPtr = nullptr;
            for (auto &r : app.getRestaurante())
            {
                if (r && r->getId() == c->getIdRestaurant())
                {
                    restPtr = r.get();
                    break;
                }
            }

            auto items = db.getOrderItems(c->getIdComanda());

            for (const auto &it : items)
            {
                Produs *p = restPtr ? restPtr->findProdusById(it.getIdProdus()) : nullptr;

                if (!p)
                    continue;

                for (int k = 0; k < it.getCantitate(); ++k)
                {
                    c->addProdus(p);
                }
            }
        }

        bool sesiune = true;
        while (sesiune)
        {
            std::cout << "1. Client\n";
            std::cout << "2. Staff\n";
            std::cout << "3. Administrator\n";
            std::cout << "4. Logout\n";
            int optiune = MenuHelper::choose_number_between_1_n(4);
            if (optiune == 1)
            {
                bool inClient = true;
                while (inClient)
                {
                    std::cout << "1. Login client\n";
                    std::cout << "2. Creare cont nou\n";
                    std::cout << "3. Parola uitata?\n";
                    std::cout << "4. Inapoi\n";
                    int optiuneClient = MenuHelper::choose_number_between_1_n(4);
                    if (optiuneClient == 1)
                    {
                        std::cout << "Introduceti numele: ";
                        std::string nume;
                        std::getline(std::cin >> std::ws, nume);
                        std::cout << "Introduceti prenumele: ";
                        std::string prenume;
                        std::getline(std::cin >> std::ws, prenume);
                        std::cout << "Introduceti parola: ";
                        std::string parola;
                        std::getline(std::cin >> std::ws, parola);
                        auto &clienti = app.getClienti();
                        auto it = std::find_if(clienti.begin(), clienti.end(),
                                               [nume, prenume, parola](const std::unique_ptr<Client> &c)
                                               {
                                                   return c && c->getNume() == nume && c->getPrenume() == prenume && c->getParola() == parola;
                                               });
                        if (it != clienti.end())
                        {
                            Client *client = it->get();
                            bool inClientLogged = true;
                            while (inClientLogged)
                            {
                                std::cout << "Cont de client gasit!\n";
                                std::cout << "Ce doriti sa faceti?\n";
                                std::cout << "1. Vizualizare restaurante\n";
                                std::cout << "2. Vizualizare comenzi livrate\n";
                                std::cout << "3. Adauga rating la comanda\n";
                                std::cout << "4. Logout\n";
                                int optiuneClientLogged = MenuHelper::choose_number_between_1_n(4);
                                if (optiuneClientLogged == 1)
                                {
                                    app.afiseazaRestaurante();

                                    int idRestaurant;
                                    std::cout << "Introduceti ID-ul restaurantului pentru a vedea meniul: ";
                                    std::cin >> idRestaurant;
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                                    auto &restaurants = app.getRestaurante();
                                    auto itR = std::find_if(restaurants.begin(), restaurants.end(),
                                                            [idRestaurant](const std::unique_ptr<Restaurant> &r)
                                                            {
                                                                return r && r->getId() == idRestaurant;
                                                            });
                                    if (itR == restaurants.end())
                                    {
                                        std::cout << "Restaurant inexistent.\n";
                                        continue;
                                    }

                                    Restaurant *restaurant = itR->get();

                                    restaurant->meniu();

                                    std::vector<Produs *> selectie;
                                    std::cout << "\nAdaugati produse in comanda (introduceti ID produs)."
                                              << "\n0 = finalizeaza selectia, -1 = renunta.\n";
                                    while (true)
                                    {
                                        std::cout << "ID produs: ";
                                        int pid;
                                        std::cin >> pid;
                                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                                        if (pid == -1)
                                        {
                                            selectie.clear();
                                            break;
                                        }
                                        if (pid == 0)
                                        {
                                            break;
                                        }

                                        Produs *p = restaurant->findProdusById(pid);
                                        if (!p)
                                        {
                                            std::cout << "Produs inexistent.\n";
                                            continue;
                                        }

                                        selectie.push_back(p);
                                        std::cout << "   Adaugat: " << p->getNume()
                                                  << " (" << std::fixed << std::setprecision(2) << p->getPret() << ")\n";
                                    }

                                    if (selectie.empty())
                                    {
                                        std::cout << "Nu ati selectat produse. Reveniti la meniu.\n";
                                        continue;
                                    }

                                    auto comUP = std::make_unique<Comanda>(0, restaurant->getId(), client->getId(), 0);
                                    for (auto *p : selectie)
                                        comUP->addProdus(p);
                                    comUP->calculatePretTotal();
                                    double total = comUP->getPretTotal();

                                    std::cout << "\nTotal comanda: " << std::fixed << std::setprecision(2) << total << "\n";
                                    std::cout << "Finalizati comanda? (da/nu): ";
                                    if (MenuHelper::DaorNu() != "da")
                                    {
                                        std::cout << "Comanda anulata.\n";
                                        continue;
                                    }

                                    unsigned long long newId = db.addComanda(restaurant->getId(), client->getId(), total);

                                    auto comUP2 = std::make_unique<Comanda>((int)newId, restaurant->getId(), client->getId(), 0);
                                    for (auto *p : selectie)
                                    {
                                        comUP2->addProdus(p);

                                        std::string tip = dynamic_cast<Mancare *>(p)  ? "Mancare"
                                                          : dynamic_cast<Desert *>(p) ? "Desert"
                                                                                      : "Bautura";

                                        db.addComandaItemSnapshot(
                                            newId,
                                            p->getId(),
                                            tip,
                                            1,
                                            p->getNume(),
                                            p->getPret(),
                                            p->getKcal());
                                    }
                                    comUP2->calculatePretTotal();

                                    Comanda *cRaw = comUP2.get();
                                    app += std::move(comUP2);

                                    std::cout << "Comanda #" << newId << " a fost plasata. Total: "
                                              << std::fixed << std::setprecision(2) << total << ".\n";
                                }
                                else if (optiuneClientLogged == 2)
                                {
                                    std::unordered_map<int, std::string> restNameById;
                                    for (const auto &r : app.getRestaurante())
                                        if (r)
                                            restNameById[r->getId()] = r->getNume();

                                    auto restName = [&](int id)
                                    {
                                        auto it = restNameById.find(id);
                                        return (it != restNameById.end()) ? it->second : std::string("(necunoscut)");
                                    };

                                    std::vector<Comanda *> livrate;
                                    for (auto &cup : app.getComenzi())
                                        if (cup && cup->getIdClient() == client->getId() && cup->isFinalizata() && cup->isLivrata()) // exclude anulate
                                            livrate.push_back(cup.get());

                                    using std::fixed;
                                    using std::left;
                                    using std::right;
                                    using std::setfill;
                                    using std::setprecision;
                                    using std::setw;

                                    auto linie = [&](int w = 80, char ch = '-')
                                    {
                                        std::cout << setfill(ch) << setw(w) << "" << setfill(' ') << "\n";
                                    };

                                    std::cout << "\n=== Comenzile tale livrate ===\n";
                                    if (livrate.empty())
                                    {
                                        std::cout << "(niciuna)\n";
                                    }
                                    else
                                    {
                                        for (auto *c : livrate)
                                        {
                                            double total = 0.0;
                                            for (auto *p : c->getProduse())
                                                if (p)
                                                    total += p->getPret();

                                            std::cout << "Comanda #" << c->getIdComanda()
                                                      << " | Restaurant: " << restName(c->getIdRestaurant())
                                                      << " | Total: " << fixed << setprecision(2) << total << "\n";

                                            std::cout << left << setw(6) << "ID"
                                                      << left << setw(32) << "Produs"
                                                      << right << setw(12) << "Pret"
                                                      << right << setw(10) << "Kcal" << "\n";
                                            linie();

                                            for (auto *p : c->getProduse())
                                            {
                                                if (!p)
                                                    continue;
                                                std::cout << left << setw(6) << p->getId()
                                                          << left << setw(32) << p->getNume()
                                                          << right << setw(12) << fixed << setprecision(2) << p->getPret()
                                                          << right << setw(10) << p->getKcal() << "\n";
                                            }
                                            linie();
                                        }
                                    }
                                }
                                else if (optiuneClientLogged == 3)
                                {
                                    std::vector<Comanda *> elig;
                                    for (auto &cup : app.getComenzi())
                                        if (cup && cup->getIdClient() == client->getId() && cup->isFinalizata() && cup->isLivrata())
                                            elig.push_back(cup.get());

                                    if (elig.empty())
                                    {
                                        std::cout << "Nu aveti comenzi livrate pe care sa le evaluati.\n";
                                    }
                                    else
                                    {
                                        std::cout << "Alege ID-ul comenzii pe care vrei s-o evaluezi din lista:\n";
                                        for (auto *c : elig)
                                            std::cout << " - " << c->getIdComanda() << "\n";

                                        int idc;
                                        std::cout << "ID comanda: ";
                                        std::cin >> idc;
                                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                                        Comanda *com = nullptr;
                                        for (auto *c : elig)
                                            if (c->getIdComanda() == idc)
                                            {
                                                com = c;
                                                break;
                                            }
                                        if (!com)
                                        {
                                            std::cout << "Comanda invalida.\n";
                                        }
                                        else
                                        {

                                            try
                                            {
                                                if (db.wasOrderRatedByClient(com->getIdComanda(), client->getId()))
                                                {
                                                    std::cout << "Comanda #" << com->getIdComanda()
                                                              << " a fost deja evaluata. Nu poti adauga un al doilea review.\n";
                                                    continue;
                                                }
                                            }
                                            catch (const std::exception &e)
                                            {
                                                std::cerr << "Eroare la verificarea review-ului existent: " << e.what() << "\n";
                                                continue;
                                            }

                                            std::cout << "Alege nota pentru comanda (1..5):\n";
                                            int notaComanda = MenuHelper::choose_number_between_1_n(5);

                                            std::cout << "Scrieti un comentariu (optional, ENTER pentru a sari):\n";
                                            std::string comentariu;
                                            std::getline(std::cin, comentariu);
                                            if (comentariu.empty())
                                                comentariu = "Clientul nu a lasat vreun comentariu";

                                            std::cout << "Alege nota pentru livrator (1..5):\n";
                                            int notaLivrator = MenuHelper::choose_number_between_1_n(5);

                                            try
                                            {
                                                db.insertReviewComanda(com->getIdComanda(), client->getId(), notaComanda, comentariu);
                                                db.insertReviewLivrator(com->getIdComanda(), client->getId(), notaComanda, notaLivrator);
                                            }
                                            catch (const std::exception &e)
                                            {
                                                std::cerr << "Eroare la salvarea review-ului: " << e.what() << "\n";
                                            }

                                            {
                                                auto rc = std::make_unique<ReviewComanda>(com->getIdComanda(), notaComanda, client->getId(), comentariu);
                                                client->addReviewComanda(std::move(rc));

                                                auto rl = std::make_unique<ReviewLivrator>(com->getIdComanda(), notaLivrator, client->getId(), notaLivrator);
                                                client->addReviewLivrator(std::move(rl));
                                            }

                                            int idRest = com->getIdRestaurant();
                                            double newAvg = 0.0;
                                            try
                                            {
                                                newAvg = db.recomputeAndUpdateRestaurantRating(idRest);
                                            }
                                            catch (const std::exception &e)
                                            {
                                                std::cerr << "Eroare la recalcularea ratingului restaurantului: " << e.what() << "\n";
                                            }

                                            for (auto &r : app.getRestaurante())
                                                if (r && r->getId() == idRest)
                                                {
                                                    r->setRating(static_cast<float>(newAvg));
                                                    break;
                                                }

                                            std::cout << "Multumim! Review-ul a fost inregistrat. Ratingul restaurantului a fost actualizat la "
                                                      << std::fixed << std::setprecision(1) << newAvg << ".\n";
                                        }
                                    }
                                }
                                else if (optiuneClientLogged == 4)
                                {
                                    inClientLogged = false;
                                }
                            }
                        }
                    }
                    else if (optiuneClient == 2)
                    {
                        std::string nume, prenume, parola;
                        std::cout << "Introduceti numele: ";
                        std::getline(std::cin >> std::ws, nume);
                        std::cout << "Introduceti prenumele: ";
                        std::getline(std::cin >> std::ws, prenume);
                        std::cout << "Introduceti parola: ";
                        std::getline(std::cin >> std::ws, parola);
                        auto dbId = db.addClient(nume, prenume, parola);
                        auto client = std::make_unique<Client>(static_cast<int>(dbId), nume, prenume, parola);
                        app += std::move(client);
                    }
                    else if (optiuneClient == 3)
                    {
                        int id;
                        std::string newPass;
                        std::cout << "Introduceti ID-ul clientului: ";
                        std::cin >> id;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Introduceti noua parola: ";
                        std::getline(std::cin >> std::ws, newPass);

                        try
                        {
                            db.updateClientPassword(id, newPass);
                            std::cout << "Parola a fost actualizata cu succes.\n";
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Eroare la schimbarea parolei: " << e.what() << "\n";
                        }
                    }
                    else if (optiuneClient == 4)
                    {
                        inClient = false;
                    }
                }
            }
            else if (optiune == 2)
            {
                std::cout << "Introduceti ID-ul restaurantului: ";
                int idRestaurant;
                std::cin >> idRestaurant;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto &restaurants = app.getRestaurante();
                auto it = std::find_if(restaurants.begin(), restaurants.end(),
                                       [idRestaurant](const std::unique_ptr<Restaurant> &r)
                                       {
                                           return r && r->getId() == idRestaurant;
                                       });
                if (it != restaurants.end())
                {
                    Restaurant *restaurant = it->get();
                    std::vector<Staff *> staff = restaurant->getStaff();
                    std::cout << "Introduceti numele dumneavoastra: ";
                    std::string nume;
                    std::getline(std::cin >> std::ws, nume);
                    std::cout << "Introduceti prenumele dumneavoastra: ";
                    std::string prenume;
                    std::getline(std::cin >> std::ws, prenume);
                    std::cout << "Introduceti ID-ul dumneavoastra: ";
                    int id;
                    std::cin >> id;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    for (auto &s : staff)
                    {
                        if (s->getNume() == nume && s->getPrenume() == prenume && s->getId() == id)
                        {
                            if (auto ang = dynamic_cast<Supervisor *>(s))
                            {
                                bool inSupervisor = true;
                                while (inSupervisor)
                                {
                                    std::cout << "Cont de supervisor gasit!\n";
                                    std::cout << "Ce doriti sa faceti?\n";
                                    std::cout << "1. Adauga bucatar\n";
                                    std::cout << "2. Adauga casier\n";
                                    std::cout << "3. Adauga livrator\n";
                                    std::cout << "4. Adauga produs\n";
                                    std::cout << "5. Distribuie comezi\n";
                                    std::cout << "6. Prepara comanda\n";
                                    std::cout << "7. Vezi livratorii + rating mediu\n";
                                    std::cout << "8. Concediaza livratorii cu rating <= 3\n";
                                    std::cout << "9. Logout\n";
                                    int optiuneSupervisor = MenuHelper::choose_number_between_1_n(9);
                                    auto nextStaffId = [&]()
                                    {
                                        int mx = 0;
                                        for (auto *st : restaurant->getStaff())
                                            if (st)
                                                mx = std::max(mx, st->getId());
                                        return mx + 1;
                                    };
                                    if (optiuneSupervisor == 1)
                                    {
                                        std::string numeBucatar, prenumeBucatar;
                                        std::cout << "Introduceti numele bucatarului: ";
                                        std::getline(std::cin >> std::ws, numeBucatar);
                                        std::cout << "Introduceti prenumele bucatarului: ";
                                        std::getline(std::cin >> std::ws, prenumeBucatar);
                                        int idBucatar = nextStaffId();
                                        std::cout << "ID-ul bucatarului va fi: " << idBucatar << "\n";
                                        auto bucatar = ang->adaugaBucatar(numeBucatar, prenumeBucatar, idBucatar);
                                        restaurant->addStaff(bucatar);
                                        db.addStaff(idRestaurant, numeBucatar, prenumeBucatar, "Bucatar", idBucatar);
                                        std::cout << "Bucatar adaugat cu succes!\n";
                                    }
                                    else if (optiuneSupervisor == 2)
                                    {
                                        std::string numeCasier, prenumeCasier;
                                        std::cout << "Introduceti numele casierului: ";
                                        std::getline(std::cin >> std::ws, numeCasier);
                                        std::cout << "Introduceti prenumele casierului: ";
                                        std::getline(std::cin >> std::ws, prenumeCasier);
                                        int idCasier = nextStaffId();
                                        std::cout << "ID-ul casierului va fi: " << idCasier << "\n";
                                        auto casier = ang->adaugaCasier(numeCasier, prenumeCasier, idCasier);
                                        restaurant->addStaff(casier);
                                        db.addStaff(idRestaurant, numeCasier, prenumeCasier, "Casier", idCasier);
                                        std::cout << "Casier adaugat cu succes!\n";
                                    }
                                    else if (optiuneSupervisor == 3)
                                    {
                                        std::string numeLivrator, prenumeLivrator;
                                        std::cout << "Introduceti numele livratorului: ";
                                        std::getline(std::cin >> std::ws, numeLivrator);
                                        std::cout << "Introduceti prenumele livratorului: ";
                                        std::getline(std::cin >> std::ws, prenumeLivrator);
                                        int idLivrator = nextStaffId();
                                        std::cout << "ID-ul livratorului va fi: " << idLivrator << "\n";
                                        auto livrator = ang->adaugaLivrator(numeLivrator, prenumeLivrator, idLivrator);
                                        restaurant->addStaff(livrator);
                                        db.addStaff(idRestaurant, numeLivrator, prenumeLivrator, "Livrator", idLivrator);
                                        std::cout << "Livrator adaugat cu succes!\n";
                                    }
                                    else if (optiuneSupervisor == 4)
                                    {
                                        std::cout << "Ce tip de produs doriti sa adaugati?\n";
                                        std::cout << "1. Mancare\n";
                                        std::cout << "2. Desert\n";
                                        std::cout << "3. Bautura\n";
                                        int tipProdus = MenuHelper::choose_number_between_1_n(3);
                                        auto calcNextId = [&](Restaurant *r)
                                        {
                                            int mx = 0;
                                            for (auto *p : r->getProduse())
                                                if (p)
                                                    mx = std::max(mx, p->getId());
                                            return mx + 1;
                                        };
                                        int nextId = calcNextId(restaurant);
                                        if (tipProdus == 1)
                                        {
                                            auto mancare = ang->adaugaMancareCuId(nextId);
                                            restaurant->addProdus(mancare);
                                            db.addMancare(nextId, idRestaurant, mancare->getNume(), mancare->getPret(), mancare->getKcal(), mancare->getTip());
                                            std::cout << "Mancare adaugata cu succes! (ID=" << nextId << ")\n";
                                        }
                                        else if (tipProdus == 2)
                                        {
                                            auto desert = ang->adaugaDesertCuId(nextId);
                                            restaurant->addProdus(desert);
                                            db.addDesert(nextId, idRestaurant, desert->getNume(), desert->getPret(), desert->getKcal(), desert->getTip());
                                            std::cout << "Desert adaugat cu succes! (ID=" << nextId << ")\n";
                                        }
                                        else if (tipProdus == 3)
                                        {
                                            auto bautura = ang->adaugaBauturaCuId(nextId);
                                            restaurant->addProdus(bautura);
                                            db.addBautura(nextId, idRestaurant, bautura->getNume(), bautura->getPret(), bautura->getKcal(), bautura->isAlcool());
                                            std::cout << "Bautura adaugata cu succes! (ID=" << nextId << ")\n";
                                        }
                                    }
                                    else if (optiuneSupervisor == 5)
                                    {
                                        std::vector<Bucatar *> bucatari;
                                        for (auto *st : restaurant->getStaff())
                                            if (auto *b = dynamic_cast<Bucatar *>(st))
                                                if (!dynamic_cast<Supervisor *>(st))
                                                    bucatari.push_back(b);

                                        auto eAsignataLaBucatar = [&](Comanda *c)
                                        {
                                            for (auto *b : bucatari)
                                                if (b && b->areComanda(c->getIdComanda()))
                                                    return true;
                                            return false;
                                        };

                                        std::vector<Comanda *> poolBuc;
                                        for (auto &cptr : app.getComenzi())
                                            if (cptr && cptr->getIdRestaurant() == restaurant->getId() && !cptr->isFinalizata() && !cptr->isAnulata() && !eAsignataLaBucatar(cptr.get()))
                                                poolBuc.push_back(cptr.get());

                                        if (poolBuc.empty())
                                            std::cout << "Nu exista comenzi noi de distribuit catre bucatari.\n";
                                        else
                                        {
                                            std::size_t i = 0;
                                            while (i < poolBuc.size())
                                            {
                                                int idC = poolBuc[i]->getIdComanda();
                                                bool ok = ang->distribuieComanda(idC, poolBuc, bucatari);
                                                if (!ok)
                                                    ++i;
                                            }
                                            std::cout << "Distribuire catre bucatari finalizata.\n";
                                        }

                                        std::vector<Livrator *> livratori;
                                        for (auto *st : restaurant->getStaff())
                                            if (auto *l = dynamic_cast<Livrator *>(st))
                                                livratori.push_back(l);

                                        auto eAsignataLaLivrator = [&](Comanda *c)
                                        {
                                            for (auto *l : livratori)
                                                if (l && l->areComanda(c->getIdComanda()))
                                                    return true;
                                            return false;
                                        };

                                        std::vector<Comanda *> poolLiv;
                                        for (auto &cptr : app.getComenzi())
                                            if (cptr && cptr->getIdRestaurant() == restaurant->getId() && cptr->isFinalizata() && !cptr->isLivrata() && !eAsignataLaLivrator(cptr.get()))
                                                poolLiv.push_back(cptr.get());

                                        if (livratori.empty())
                                            std::cout << "Nu exista livratori la acest restaurant.\n";
                                        else if (poolLiv.empty())
                                            std::cout << "Nu exista comenzi preparate de distribuit catre livratori.\n";
                                        else
                                        {
                                            std::size_t i = 0;
                                            while (i < poolLiv.size())
                                            {
                                                int idC = poolLiv[i]->getIdComanda();
                                                bool ok = ang->distribuieComandaLivrator(idC, poolLiv, livratori);
                                                if (!ok)
                                                    ++i;
                                                else
                                                {
                                                    int idLivAles = 0;
                                                    for (auto *l : livratori)
                                                        if (l && l->areComanda(idC))
                                                        {
                                                            idLivAles = l->getId();
                                                            break;
                                                        }
                                                    if (idLivAles > 0)
                                                    {
                                                        try
                                                        {
                                                            db.updateComandaLivrator(idC, idLivAles);
                                                        }
                                                        catch (const std::exception &e)
                                                        {
                                                            std::cerr << "Eroare setare idLivrator: " << e.what() << "\n";
                                                        }
                                                    }
                                                }
                                            }
                                            std::cout << "Distribuire catre livratori finalizata.\n";
                                        }
                                    }
                                    else if (optiuneSupervisor == 6)
                                    {
                                        std::vector<Bucatar *> bucatari;
                                        for (auto *st : restaurant->getStaff())
                                            if (auto *b = dynamic_cast<Bucatar *>(st))
                                                if (!dynamic_cast<Supervisor *>(st))
                                                    bucatari.push_back(b);

                                        auto totiOcupati = [&]()
                                        {
                                            if (bucatari.empty())
                                                return true;
                                            for (auto *b : bucatari)
                                                if (b && b->nrComenzi() <= 2)
                                                    return false;
                                            return true;
                                        }();

                                        if (!totiOcupati)
                                        {
                                            std::cout << "Exista bucatari disponibili; folositi mai intai 'Distribuie comenzi'.\n";
                                        }
                                        else
                                        {
                                            auto eAsignataLaBucatar = [&](Comanda *c)
                                            {
                                                for (auto *b : bucatari)
                                                    if (b && b->areComanda(c->getIdComanda()))
                                                        return true;
                                                return false;
                                            };

                                            Comanda *target = nullptr;

                                            for (auto &cptr : app.getComenzi())
                                                if (cptr && cptr->getIdRestaurant() == restaurant->getId() && !cptr->isFinalizata() && !cptr->isAnulata() && !eAsignataLaBucatar(cptr.get()))
                                                {
                                                    target = cptr.get();
                                                    break;
                                                }

                                            if (!target)
                                                for (auto &cptr : app.getComenzi())
                                                    if (cptr && cptr->getIdRestaurant() == restaurant->getId() && !cptr->isFinalizata() && !cptr->isAnulata())
                                                    {
                                                        target = cptr.get();
                                                        break;
                                                    }

                                            if (!target)
                                            {
                                                std::cout << "Nu exista comenzi de preparat.\n";
                                            }
                                            else
                                            {
                                                int idc = target->getIdComanda();
                                                target->setFinalizata(true);

                                                for (auto *b : bucatari)
                                                    if (b)
                                                        b->stergeComandaDinCoada(idc);

                                                try
                                                {
                                                    db.updateComandaFinalizata(idc, true);
                                                }
                                                catch (const std::exception &e)
                                                {
                                                    std::cerr << "Eroare la update finalizata: " << e.what() << "\n";
                                                }

                                                std::cout << "Comanda " << idc << " a fost preparata de supervisorul "
                                                          << ang->getNume() << " " << ang->getPrenume() << ".\n";
                                            }
                                        }
                                    }
                                    else if (optiuneSupervisor == 7)
                                    {
                                        try
                                        {
                                            auto rows = db.getLivratoriCuRatingMediu(idRestaurant);

                                            using std::fixed;
                                            using std::left;
                                            using std::right;
                                            using std::setfill;
                                            using std::setprecision;
                                            using std::setw;

                                            auto linie = [&](int w = 64, char ch = '-')
                                            {
                                                std::cout << setfill(ch) << setw(w) << "" << setfill(' ') << "\n";
                                            };

                                            std::cout << "\n=== Livratori (restaurant " << restaurant->getNume() << ") ===\n";
                                            std::cout << left << setw(6) << "ID"
                                                      << left << setw(20) << "Nume"
                                                      << left << setw(20) << "Prenume"
                                                      << right << setw(8) << "Medie"
                                                      << right << setw(8) << "#Rev" << "\n";
                                            linie();

                                            for (const auto &r : rows)
                                            {
                                                std::cout << left << setw(6) << r.getId()
                                                          << left << setw(20) << r.getNume()
                                                          << left << setw(20) << r.getPrenume()
                                                          << right << setw(8) << fixed << setprecision(2) << r.getRating()
                                                          << right << setw(8) << r.getNrReviewuri()
                                                          << "\n";
                                            }
                                            linie();
                                        }
                                        catch (const std::exception &e)
                                        {
                                            std::cerr << "Eroare la listarea livratorilor: " << e.what() << "\n";
                                        }
                                    }
                                    else if (optiuneSupervisor == 8)
                                    {
                                        try
                                        {
                                            auto concediati = db.concediazaLivratoriSubPrag(idRestaurant, 3.0);
                                            if (concediati.empty())
                                            {
                                                std::cout << "Niciun livrator nu se afla sub sau la pragul de 3.0.\n";
                                            }
                                            else
                                            {
                                                auto &sv = restaurant->getStaff();
                                                sv.erase(std::remove_if(sv.begin(), sv.end(),
                                                                        [&](Staff *s)
                                                                        {
                                                                            auto *l = dynamic_cast<Livrator *>(s);
                                                                            if (!l)
                                                                                return false;
                                                                            if (std::find(concediati.begin(), concediati.end(), l->getId()) != concediati.end())
                                                                            {
                                                                                delete s;
                                                                                return true;
                                                                            }
                                                                            return false;
                                                                        }),
                                                         sv.end());

                                                std::cout << "Concediati " << concediati.size() << " livratori cu rating <= 3.0.\n";
                                            }
                                        }
                                        catch (const std::exception &e)
                                        {
                                            std::cerr << "Eroare la concediere: " << e.what() << "\n";
                                        }
                                    }
                                    else if (optiuneSupervisor == 9)
                                    {
                                        inSupervisor = false;
                                        std::cout << "Ati iesit din contul de supervisor.\n";
                                    }
                                }
                            }
                            else if (auto cas = dynamic_cast<Casier *>(s))
                            {
                                std::cout << "Cont de casier gasit!\n";

                                std::vector<Bucatar *> bucatari;
                                for (auto *st : restaurant->getStaff())
                                    if (auto *b = dynamic_cast<Bucatar *>(st))
                                        if (!dynamic_cast<Supervisor *>(st))
                                            bucatari.push_back(b);

                                if (bucatari.empty())
                                {
                                    std::cout << "Nu exista bucatari la acest restaurant. Nu pot distribui comenzi.\n";
                                }
                                else
                                {
                                    auto eAsignataLaBucatar = [&](Comanda *c)
                                    {
                                        for (auto *b : bucatari)
                                            if (b && b->areComanda(c->getIdComanda()))
                                                return true;
                                        return false;
                                    };

                                    std::vector<Comanda *> pool;
                                    for (auto &cptr : app.getComenzi())
                                    {
                                        if (cptr && cptr->getIdRestaurant() == restaurant->getId() && !cptr->isFinalizata() && !cptr->isAnulata() && !eAsignataLaBucatar(cptr.get()))
                                        {
                                            pool.push_back(cptr.get());
                                        }
                                    }

                                    if (pool.empty())
                                    {
                                        std::cout << "Nu exista comenzi noi de distribuit catre bucatari.\n";
                                    }
                                    else
                                    {
                                        std::size_t i = 0;
                                        while (i < pool.size())
                                        {
                                            int idC = pool[i]->getIdComanda();
                                            bool asignata = cas->distribuieComanda(idC, pool, bucatari);
                                            if (!asignata)
                                                ++i;
                                        }
                                        std::cout << "Distribuire catre bucatari finalizata.\n";
                                    }
                                }

                                std::vector<Livrator *> livratori;
                                for (auto *st : restaurant->getStaff())
                                    if (auto *l = dynamic_cast<Livrator *>(st))
                                        livratori.push_back(l);

                                if (livratori.empty())
                                {
                                    std::cout << "Nu exista livratori la acest restaurant. Nu pot distribui la livrare.\n";
                                }
                                else
                                {
                                    auto eAsignataLaLivrator = [&](Comanda *c)
                                    {
                                        for (auto *l : livratori)
                                            if (l && l->areComanda(c->getIdComanda()))
                                                return true;
                                        return false;
                                    };

                                    std::vector<Comanda *> poolLivrare;
                                    for (auto &cptr : app.getComenzi())
                                    {
                                        if (cptr && cptr->getIdRestaurant() == restaurant->getId() && cptr->isFinalizata() && !cptr->isLivrata() && !eAsignataLaLivrator(cptr.get()))
                                        {
                                            poolLivrare.push_back(cptr.get());
                                        }
                                    }

                                    if (poolLivrare.empty())
                                    {
                                        std::cout << "Nu exista comenzi preparate de distribuit catre livratori.\n";
                                    }
                                    else
                                    {
                                        std::size_t i = 0;
                                        while (i < poolLivrare.size())
                                        {
                                            int idC = poolLivrare[i]->getIdComanda();
                                            bool asignata = cas->distribuieComandaLivrator(idC, poolLivrare, livratori);
                                            if (!asignata)
                                                ++i;
                                            else
                                            {
                                                int idLivAles = 0;
                                                for (auto *l : livratori)
                                                    if (l && l->areComanda(idC))
                                                    {
                                                        idLivAles = l->getId();
                                                        break;
                                                    }
                                                if (idLivAles > 0)
                                                {
                                                    try
                                                    {
                                                        db.updateComandaLivrator(idC, idLivAles);
                                                    }
                                                    catch (const std::exception &e)
                                                    {
                                                        std::cerr << "Eroare setare idLivrator: " << e.what() << "\n";
                                                    }
                                                }
                                            }
                                        }
                                        std::cout << "Distribuire catre livratori finalizata.\n";
                                    }
                                }

                                std::cout << "Distribuire terminata.\n";
                            }
                            else if (auto buc = dynamic_cast<Bucatar *>(s))
                            {
                                bool inBucatar = true;
                                while (inBucatar)
                                {
                                    std::cout << "Cont de bucatar gasit!\n";
                                    std::cout << "Ce doriti sa faceti?\n";
                                    std::cout << "1. Afiseaza comenzile de preparat\n";
                                    std::cout << "2. Marcheaza o comanda ca preparata\n";
                                    std::cout << "3. Logout\n";

                                    int opt = MenuHelper::choose_number_between_1_n(3);
                                    if (opt == 1)
                                    {
                                        buc->afiseazaComenzi();
                                    }
                                    else if (opt == 2)
                                    {
                                        std::cout << "Introduceti ID-ul comenzii de preparat: ";
                                        int idc;
                                        std::cin >> idc;
                                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                                        buc->preparaComanda(idc);
                                        try
                                        {
                                            db.updateComandaFinalizata(idc, true);
                                        }
                                        catch (const std::exception &e)
                                        {
                                            std::cerr << "Eroare la update finalizata: " << e.what() << "\n";
                                        }
                                    }
                                    else if (opt == 3)
                                    {
                                        inBucatar = false;
                                        std::cout << "Ati iesit din contul de bucatar.\n";
                                    }
                                }
                            }
                            else if (auto liv = dynamic_cast<Livrator *>(s))
                            {
                                bool inLivrator = true;
                                while (inLivrator)
                                {
                                    std::cout << "Cont de livrator gasit!\n";
                                    std::cout << "Ce doriti sa faceti?\n";
                                    std::cout << "1. Afiseaza comenzile de livrat\n";
                                    std::cout << "2. Livreaza comanda\n";
                                    std::cout << "3. Logout\n";

                                    int opt = MenuHelper::choose_number_between_1_n(3);
                                    if (opt == 1)
                                    {
                                        liv->afiseazaComenzi();
                                    }
                                    else if (opt == 2)
                                    {
                                        int idLivrat = 0;
                                        std::string cat;

                                        auto categorieClient = [&](int idCli) -> std::string
                                        {
                                            for (auto &cup : app.getClienti())
                                                if (cup && cup->getId() == idCli)
                                                    return cup->getCategorie();
                                            return "Basic";
                                        };

                                        if (liv->livreazaUrmatoareaComandaPrioritar(categorieClient, idLivrat, cat))
                                        {
                                            std::cout << "Comanda " << idLivrat
                                                      << " a fost livrata (categoria clientului: " << cat << ").\n";
                                            try
                                            {
                                                db.updateComandaLivrata(idLivrat, true);
                                            }
                                            catch (const std::exception &e)
                                            {
                                                std::cerr << "Eroare la update livrata: " << e.what() << "\n";
                                            }
                                        }
                                        else
                                        {
                                            std::cout << "Nu aveti comenzi de livrat.\n";
                                        }
                                    }
                                    else if (opt == 3)
                                    {
                                        inLivrator = false;
                                        std::cout << "Ati iesit din contul de livrator.\n";
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    std::cout << "Restaurantul cu ID-ul " << idRestaurant << " nu a fost gasit.\n";
                }
            }
            else if (optiune == 3)
            {
                bool sesiuneAdmin = true;
                while (sesiuneAdmin)
                {
                    std::string u, p;

                    std::cout << "Username: ";
                    std::getline(std::cin >> std::ws, u);

                    std::cout << "Parola: ";
                    std::getline(std::cin, p);

                    try
                    {
                        admin->authenticate(u, p);
                        bool inAdmin = true;
                        while (inAdmin)
                        {
                            std::cout << "Autentificare reusita!\n";
                            std::cout << "Bine ati venit in aplicatia de administrare!\n";
                            std::cout << "Ce doriti sa faceti?\n";
                            std::cout << "1. Adaugare restaurant\n";
                            std::cout << "2. Vizualizare restaurante (nume + rating mediu)\n";
                            std::cout << "3. Aplica reducere 20% pentru restaurante cu rating <= 3\n";
                            std::cout << "4. Logout\n";
                            int adminOptiune = MenuHelper::choose_number_between_1_n(4);
                            if (adminOptiune == 1)
                            {
                                std::string numeRestaurant;
                                std::cout << "Introduceti numele restaurantului: ";
                                std::getline(std::cin >> std::ws, numeRestaurant);
                                auto nextRestaurantId = [&]()
                                {
                                    int mx = 0;
                                    for (const auto &rup : app.getRestaurante())
                                        if (rup)
                                            mx = std::max(mx, rup->getId());
                                    return mx + 1; 
                                };

                                int idRestaurant = nextRestaurantId();
                                db.addRestaurant(idRestaurant, numeRestaurant);
                                std::cout << "Restaurantul a fost adaugat cu ID-ul: " << idRestaurant << "\n";
                                auto r = std::make_unique<Restaurant>(idRestaurant, numeRestaurant);
                                std::cout << "Adaugati un supervisor pentru restaurantul " << numeRestaurant << "\n";
                                std::string numeSupervisor, prenumeSupervisor;
                                std::cout << "Nume: ";
                                std::getline(std::cin >> std::ws, numeSupervisor);
                                std::cout << "Prenume: ";
                                std::getline(std::cin >> std::ws, prenumeSupervisor);
                                auto nextStaffIdFor = [&](Restaurant *rr)
                                {
                                    int mx = 0;
                                    for (auto *st : rr->getStaff())
                                        if (st)
                                            mx = std::max(mx, st->getId());
                                    return mx + 1;
                                };
                                int idSupervisor = nextStaffIdFor(r.get());
                                std::cout << "ID-ul supervisorului va fi: " << idSupervisor << "\n";
                                auto supervisor = std::make_unique<Supervisor>(idRestaurant, numeSupervisor, prenumeSupervisor, idSupervisor);
                                r->addStaff(supervisor.release());
                                db.addStaff(idRestaurant, numeSupervisor, prenumeSupervisor, "Supervisor", idSupervisor);
                                std::cout << "Supervisorul a fost adaugat cu succes!\n";
                                r->setAreSupervisor(true);
                                app += std::move(r);
                            }
                            else if (adminOptiune == 2)
                            {
                                app.afiseazaRestaurante();
                            }
                            else if (adminOptiune == 3)
                            {
                                int restAfectate = 0, produseAfectate = 0;
                                for (auto &rup : app.getRestaurante())
                                {
                                    if (!rup)
                                        continue;
                                    if (rup->getRating() <= 3.0f and rup->getRating() > 0.0f)
                                    {
                                        ++restAfectate;
                                        for (auto *p : rup->getProduse())
                                        {
                                            if (!p)
                                                continue;
                                            p->setPret(p->getPret() * 0.8f);
                                            ++produseAfectate;
                                        }
                                    }
                                }

                                try
                                {
                                    db.discountProductsForLowRatedRestaurants(3.0, 0.8);
                                    std::cout << "Reducerea a fost aplicata cu succes la "
                                              << produseAfectate << " produse, in " << restAfectate
                                              << " restaurante (rating <= 3).\n";
                                }
                                catch (const std::exception &e)
                                {
                                    std::cerr << "Eroare la aplicarea reducerii in DB: " << e.what() << "\n";
                                }
                            }
                            else if (adminOptiune == 4)
                            {
                                sesiuneAdmin = false;
                                inAdmin = false;
                                std::cout << "Ati iesit din sesiunea de administrator.\n";
                            }
                        }
                    }
                    catch (const BadCredentials &e)
                    {
                        std::cerr << e.what() << "\n";
                        std::cout << "Doriti sa incercati din nou? (da/nu): ";
                        std::string raspuns = MenuHelper::DaorNu();
                        if (raspuns == "nu")
                            sesiuneAdmin = false;
                    }
                }
            }
            else if (optiune == 4)
            {
                sesiune = false;
                std::cout << "Ati iesit din aplicatie.\n";
            }
        }
        const auto &cl = app.getClienti();
        const auto &rest = app.getRestaurante();

        auto clientiCuComenzi = db.getAllClientsWithOrdersAndItems();
        // app.printClientsOrders(clientiCuComenzi);

        try
        {
            auto clientsWithOrders = db.getAllClientsWithOrdersAndItems();

            std::unordered_map<int, double> stotalByClient;

            for (const auto &cv : clientsWithOrders)
            {
                double stotalLocal = 0.0;

                for (const auto &ord : cv.getComenzi())
                {
                    double totalOrder = 0.0;
                    for (const auto &it : ord.getItems())
                        totalOrder += it.getPretUnitSnapshot() * it.getCantitate();
                    if (totalOrder == 0.0)
                        totalOrder = ord.getPretTotal();

                    const bool eGata = (ord.isFinalizata() && ord.isLivrata());

                    if (!eGata)
                    {

                        try
                        {
                            db.updateComandaFinalizata(ord.getIdComanda(), false);
                            db.updateComandaLivrata(ord.getIdComanda(), true);
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Eroare anulare comanda #" << ord.getIdComanda() << ": " << e.what() << "\n";
                        }

                        for (auto &cptr : app.getComenzi())
                        {
                            if (cptr && cptr->getIdComanda() == ord.getIdComanda())
                            {
                                cptr->setFinalizata(false);
                                cptr->makeLivrata();
                                break;
                            }
                        }

                        stotalLocal += 0.70 * totalOrder;
                    }
                    else
                    {
                        stotalLocal += totalOrder;
                    }
                }

                stotalByClient[cv.getClientId()] = stotalLocal;
            }

            ThreeCategoryStrategy strat;
            CategoryContext ctx(strat);

            for (auto &cup : app.getClienti())
            {
                if (!cup)
                    continue;
                const int idCl = cup->getId();
                const double stotal = stotalByClient.count(idCl) ? stotalByClient[idCl] : 0.0;

                const std::string cat = ctx.choose(stotal);

                cup->setCategorie(cat);

                try
                {
                    db.updateClientCategory(idCl, cat);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Eroare update categorie pentru client #" << idCl << ": " << e.what() << "\n";
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Eroare la reconcilierea finala: " << e.what() << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Eroare: " << e.what() << "\n";
        return 1;
    }

    delete admin;
    return 0;
}