#include <string>
#include<iostream>
#include <vector>
#include <map>
#include <memory>

enum class Tip_Angajat {
    Bucatar,
    Ospatar
};

class IOStream {
protected:
    virtual void Print(std::ostream &) const = 0;

    friend std::ostream &operator<<(std::ostream &, const IOStream &);

    friend std::ostream &operator<<(std::ostream &, const IOStream *);

    virtual ~IOStream();
};

std::ostream &operator<<(std::ostream &os, const IOStream &ob) {
    ob.Print(os);
    return os;
}

std::ostream &operator<<(std::ostream &os, const IOStream *ob) {
    ob->Print(os);
    return os;
}

IOStream::~IOStream() = default;

class Angajat : public IOStream {
protected:
    std::string Name;
    std::string Prenume;
    int Age;
public:
    Angajat(std::string name, std::string prenume, int age);

    Angajat(const Angajat &rhs);

    Angajat &operator=(const Angajat &rhs);

    const std::string &GetName() const;

    void SetName(std::string name);

    void SetPrenume(std::string prenume);

    int GetAge() const;

    void SetAge(int age);

    virtual Tip_Angajat GetTip_Angajat() const = 0;

    virtual ~Angajat();
};

Angajat::Angajat(std::string name, std::string prenume, int age) : Name(std::move(name)), Prenume(std::move(prenume)),
                                                                   Age(age) {
}

Angajat::Angajat(const Angajat &rhs) : Angajat(rhs.Name, rhs.Prenume, rhs.Age) {
}

Angajat &Angajat::operator=(const Angajat &rhs) {
    if (this != &rhs) {
        Name = rhs.Name;
        Prenume = rhs.Prenume;
        Age = rhs.Age;
    }
    return *this;
}

const std::string &Angajat::GetName() const {
    return Name;
}

void Angajat::SetName(std::string name) {
    Angajat::Name = std::move(name);
}

void Angajat::SetPrenume(std::string prenume) {
    Angajat::Prenume = std::move(prenume);
}

int Angajat::GetAge() const {
    return Age;
}

void Angajat::SetAge(int age) {
    Angajat::Age = age;
}

Angajat::~Angajat() = default;

//CLASA BUCATAR

class Bucatar : public Angajat {
    std::string Specializare_Bucatar;
public:
    Bucatar(std::string name, std::string prenume, int age, std::string specializare_Bucatar);

    Bucatar(const Bucatar &rhs);

    Bucatar &operator=(Bucatar rhs);

    const std::string &GetSpecializare() const;

    void SetSpecializare(std::string specializare_Bucatar);

    void Print(std::ostream &) const override;

    Tip_Angajat GetTip_Angajat() const override { return Tip_Angajat::Bucatar; }
};

Bucatar::Bucatar(std::string name, std::string prenume, int age, std::string specializare_Bucatar)
        : Angajat(std::move(name), std::move(prenume), age), Specializare_Bucatar(std::move(specializare_Bucatar)) {
}

Bucatar::Bucatar(const Bucatar &rhs) : Angajat(rhs.Name, rhs.Prenume, rhs.Age),
                                       Specializare_Bucatar(rhs.Specializare_Bucatar) {
}

Bucatar &Bucatar::operator=(Bucatar rhs) {
    Angajat::operator=(rhs);
    std::swap(Specializare_Bucatar, rhs.Specializare_Bucatar);
    return *this;
}

const std::string &Bucatar::GetSpecializare() const {
    return Specializare_Bucatar;
}

void Bucatar::SetSpecializare(std::string specializare_Bucatar) {
    Bucatar::Specializare_Bucatar = std::move(specializare_Bucatar);
}

void Bucatar::Print(std::ostream &os) const {
    os << "Bucatar(" << Name << " " << Prenume << ", " << Age << ", " << Specializare_Bucatar << ")" << "\n";
}

//CLASA OSPATAR -- Ospatarii lucreaza fiecare pe sectiuni (sali) specifice ale restaurantului

class Ospatar : public Angajat {
    std::string Sala;
public:
    Ospatar(std::string name, std::string prenume, int age, std::string sala);

    const std::string &GetSala() const;

    void SetSala(std::string sala);

    void Print(std::ostream &os) const override;

    Tip_Angajat GetTip_Angajat() const override { return Tip_Angajat::Ospatar; }
};

Ospatar::Ospatar(std::string name, std::string prenume, int age, std::string sala)
        : Angajat(std::move(name), std::move(prenume), age), Sala(std::move(sala)) {
}

const std::string &Ospatar::GetSala() const {
    return Sala;
}

void Ospatar::SetSala(std::string sala) {
    Ospatar::Sala = std::move(Sala);
}

void Ospatar::Print(std::ostream &os) const {
    os << "Ospatar(" << Name << " " << Prenume << ", " << Age << ", " << Sala << ")" << "\n";
}

class Nu_exista : public std::exception, public IOStream {
public:
    Nu_exista() = default;

    const char *what() const noexcept override;

    void Print(std::ostream &) const;
};

const char *Nu_exista::what() const noexcept {
    return "\nAcest angajat nu exista.\n";
}

void Nu_exista::Print(std::ostream &os) const {
    os << this->what();
}

class Restaurant {
    static std::vector<std::shared_ptr<Angajat>> Angajati;
public:
    Restaurant() = delete;

    static void Adauga_angajat(std::shared_ptr<Angajat> angajat);

    static std::vector<std::shared_ptr<Angajat>> &GetAngajati();

    static std::shared_ptr<Angajat> Cauta_angajat(const std::string &name);

    static std::vector<std::shared_ptr<Ospatar>> Cauta_toti_ospatarii();
};

std::vector<std::shared_ptr<Angajat>> Restaurant::Angajati;

void Restaurant::Adauga_angajat(std::shared_ptr<Angajat> angajat) {
    Angajati.push_back(angajat);
}

std::vector<std::shared_ptr<Angajat>> &Restaurant::GetAngajati() {
    return Restaurant::Angajati;
}

std::shared_ptr<Angajat> Restaurant::Cauta_angajat(const std::string &name) {
    int ok=0;
    for (auto &angajat: Restaurant::Angajati)
        if (angajat->GetName() == name)
            {ok++;
             if(ok!=1)
                std::cout << *angajat;
            }
    for (auto &angajat: Restaurant::Angajati)
        if (angajat->GetName() == name)
            return angajat;
    throw Nu_exista();
}

std::vector<std::shared_ptr<Ospatar>> Restaurant::Cauta_toti_ospatarii() {
    std::vector<std::shared_ptr<Ospatar>> ospatari;
    for (auto &angajat: Restaurant::Angajati) {
        if (typeid(*angajat) == typeid(Ospatar)) {
            auto ospatar = std::dynamic_pointer_cast<Ospatar>(angajat);
            ospatari.push_back(ospatar);
        }
    }

    return ospatari;
}


int main() {
    /*
3
Ionescu Marian 25 Sala_3
Popescu Georgian 29 Sala_2
Avram Ion 33 Sala_1
2
Ionescu Alexandru 39 Mancare_asiatica
Nitu Adrian 45 Mancare_traditionala
     */
    int n;
    std::cout << "Citeste numarul de ospatari:\n";
    std::cin >> n;
    std::string nume;
    std::string prenume;
    int varsta;
    std::string sala;
    for (int i = 0; i < n; i++) {
        std::cin >> nume;
        std::cin >> prenume;
        std::cin >> varsta;
        std::cin >> sala;
        auto add_ospatar = std::make_shared<Ospatar>(nume, prenume, varsta, sala);
        Restaurant::Adauga_angajat(add_ospatar);
    }

    std::cout << "Citeste numarul de bucatari:\n";
    std::cin >> n;
    std::string specializare;
    for (int i = 0; i < n; i++) {
        std::cin >> nume;
        std::cin >> prenume;
        std::cin >> varsta;
        std::cin >> specializare;
        auto add_bucatar = std::make_shared<Bucatar>(nume, prenume, varsta, specializare);
        Restaurant::Adauga_angajat(add_bucatar);
        std::cout << *add_bucatar;
    }

    std::cout<< "Meniu interactiv:\n Tasta 0 -> Exit \n Tasta 1 -> Cauta angajati dupa numele de familie \n Tasta 2 -> Afiseaza toti ospatarii\n";
    int alegere;
    std::string nume_familie;
    do {
        std::cin >> alegere;
        switch (alegere) {
            case 0:
                std::cout << "Final";
                break;
            case 1:
                std::cout << "Citeste numele de familie pe care doresti sa il cauti:\n";
                std::cin >> nume_familie;
                try {
                    std::cout << *Restaurant::Cauta_angajat(nume_familie);
                } catch (const Nu_exista &err) {
                    std::cout << err;
                }
                break;
            case 2:
                std::cout << "Afisarea tuturor ospatarilor:\n";
                for (auto &angajat: Restaurant::Cauta_toti_ospatarii()) {
                    std::cout << *angajat << '\n';
                }
                break;
        }
    } while (alegere != 0);
    return 0;
}
