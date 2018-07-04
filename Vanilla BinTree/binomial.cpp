#include <iostream>
#include <iomanip> /* std::setprecision */
#include <vector>
#include <fstream>
#include <cstdlib> /* atof & atol */
#include <cmath>
#include <cassert>

const std::string plikWejsciowy = "input.txt";
const std::string plikWyjsciowy = "output.txt";

/*
Sposób numeracji naszego drzewa:

        2
       /
      1
     / \
    0   1
     \ /
      0
       \
        0

t = 0 1 2...

tzn. wezly przechowujemy w kolekcji kolekcji, pozwala to na wykonywanie bardzo rozleglych sumulacji,
bez obawy, ze rozmiar kolekcji bedzie zbyt duzy.

Algorytm wyceny:

1) Wczytujemy wszystkie dane do specjalnie do tego przygotowanych klas
2) Dla kazdego instrumentu z portfela tworzy drzewo dwumianowe, wyceniamy kazdy wezel metoda delta-hedgingu
3) Sumujemy drzewa "po wspolrzednych", tzn. dodajemy ilosc akcji i payoffy portefli odpowiednich wezlach czasowych
4) Eksportujemy do pliku

*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Instrument
{
    private:
        std::string nazwa_;                    // name
        std::string charakterystyka_;          // exercise
        std::string pozycja_;                  // position
        std::string typ_;                      // type
        double cenaWykonania_ = 0.0;           // strike [price @ expiry]
        double dataWykonania_ = 0.0;           // expiry [in Yrs]

    public:
        Instrument(std::vector<std::string> WE = std::vector<std::string> (6, "0"))
        :
            nazwa_(WE[0]),
            charakterystyka_(WE[1]),
            pozycja_(WE[2]),
            typ_(WE[3]),
            cenaWykonania_(atof(WE[4].c_str())),
            dataWykonania_(atof(WE[5].c_str()))
        {
        }

        std::string nazwa() const;
        std::string charakterystyka() const;
        std::string pozycja() const;
        std::string typ() const;
        double cenaWykonania() const;
        double dataWykonania() const;
};

std::string Instrument::nazwa() const                   // Nie wszystkie metody sa potrzebne, lecz napisalismy ja na poczet
{                                                       // przyszlej edycji i rozszerzenia funkcjonalnosci programu.
    return nazwa_;
}

std::string Instrument::charakterystyka() const
{
    return charakterystyka_;
}

std::string Instrument::pozycja() const
{
    return pozycja_;
}

std::string Instrument::typ() const
{
    return typ_;
}

double Instrument::cenaWykonania() const
{
    return cenaWykonania_;
}

double Instrument::dataWykonania() const
{
    return dataWykonania_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParametryModelu
{
    private:
        int iloscKrokow_ = 0;                   // steps
        double horyzontCzasowy_ = 0.0;          // time horizon [in Yrs]
        double Spot_ = 0.0;                     // price @ T = 0 [spot price]
        double upMove_ = 0.0;                   // up-factor
        double downMove_ = 0.0;                 // down-factor
        double stopaWolnaOdRyzyka_ = 0.0;       // risk-free rate



    public:
        ParametryModelu(std::vector<std::string> WE = std::vector<std::string> (6, "0"))    // konstruktor domyslny, to model, w ktorym kazda wartosc jest rowna 0
        :
            iloscKrokow_(atoi(WE[0].c_str())),
            horyzontCzasowy_(atof(WE[1].c_str())),
            Spot_(atof(WE[2].c_str())),
            upMove_(atof(WE[3].c_str())),
            downMove_(atof(WE[4].c_str())),
            stopaWolnaOdRyzyka_(atof(WE[5].c_str()))
        {}

        int iloscKrokow() const;
        double horyzontCzasowy() const;
        double Spot() const;
        double upMove() const;
        double downMove() const;
        double stopaWolnaOdRyzyka() const;


};

int ParametryModelu::iloscKrokow() const
{
    return iloscKrokow_;
}

double ParametryModelu::horyzontCzasowy() const
{
    return horyzontCzasowy_;
}

double ParametryModelu::Spot() const
{
    return Spot_;
}

double ParametryModelu::upMove() const
{
    return upMove_;
}

double ParametryModelu::downMove() const
{
    return downMove_;
}

double ParametryModelu::stopaWolnaOdRyzyka() const
{
    return stopaWolnaOdRyzyka_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Wezel
{
    private:
        double czas_ = 0.0;
        double cenaAkcji_ = 0.0;
        double wartoscPortfela_ = 0.0;
        double delta_ = 0.0;


    public:
        Wezel(double czas = 0.0, double cenaAkcji = 0.0, double wartoscPortfela = 0.0, double delta = 0.0, double payoff = 0.0)
        :
            czas_(czas),
            cenaAkcji_(cenaAkcji),
            wartoscPortfela_(wartoscPortfela),
            delta_(delta)
        {}

        double czas() const;
        double cenaAkcji() const;
        double wartoscPortfela() const;
        double delta() const;

        void zmienCenaAkcji(double CA);
        void zmienWartoscPortfela(double WP);
        void zmienDelta(double D);
};

double Wezel::czas() const
{
    return czas_;
}

double Wezel::cenaAkcji() const
{
    return cenaAkcji_;
}

double Wezel::wartoscPortfela() const
{
    return wartoscPortfela_;
}

double Wezel::delta() const
{
    return delta_;
}

void Wezel::zmienCenaAkcji(double CA)
{
    cenaAkcji_ = CA;
}

void Wezel::zmienWartoscPortfela(double WP)
{
    wartoscPortfela_ = WP;
}

void Wezel::zmienDelta(double D)
{
    delta_ = D;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Drzewo
{
    private:
        std::vector<std::vector<Wezel> > d_;
        ParametryModelu pm_;
        double delta_t = 0.0;

    public:
        Drzewo(const ParametryModelu& PM);         // Ju¿ na etapie konstuowania drzewo bedzie zawiera³o podastawowe informacje dot danego modelu
                                                   // Jest to o tyle rozsadne, gdyz w naszej rzeczywistosci sytuacja rynkowa nie zmienia sie dla poszczegolnych instumentow

        void wycenIntrument(const Instrument& INSTR);
        void modyfikujWezel(int POZIOM, int WEZ, double DELTA, double WARTOSC);

        int size() const;
        std::vector<Wezel> operator[](int& WE) const;
        std::vector<Wezel> operator[](int WE);
        ParametryModelu pm() const;


};


Drzewo::Drzewo(const ParametryModelu& PM)
:
pm_(PM),
delta_t(PM.horyzontCzasowy() / PM.iloscKrokow())

{
    d_ = std::vector<std::vector<Wezel> > ();
    delta_t = PM.horyzontCzasowy() / PM.iloscKrokow();

    for(int i = 0; i <= PM.iloscKrokow(); i++)
    {
        std::vector<Wezel> TMPvec;
        double czasWchwili = delta_t * i;

        for(int j = 0; j <= i ; j++)                                // tzw. forward iteration
        {
            Wezel TMPwez;

            if(i == 0)
            {
                TMPwez = Wezel(czasWchwili, PM.Spot());
            }
            else if(j == 0)
            {
                TMPwez = Wezel(czasWchwili, (d_[i-1][j].cenaAkcji())*(PM.downMove()));
            }
            else
            {
                TMPwez = Wezel(czasWchwili, (d_[i-1][j-1].cenaAkcji())*(PM.upMove()));
            }

            TMPvec.push_back(TMPwez);
        }

    d_.push_back(TMPvec);
    }
}

void Drzewo::modyfikujWezel(int POZIOM, int WEZ, double DELTA, double WARTOSC)
{
    d_[POZIOM][WEZ].zmienDelta(DELTA);
    d_[POZIOM][WEZ].zmienWartoscPortfela(WARTOSC);
}


int Drzewo::size() const
{
    return d_.size();
}

void Drzewo::wycenIntrument(const Instrument& INSTR)
{
    double p = (exp(delta_t * pm_.stopaWolnaOdRyzyka()) - pm_.downMove())/(pm_.upMove() - pm_.downMove());   // p w p-podobienstwie neutralnym sluzacym do wyceny
    int ilePoziomow = d_.size()-1;                                      // zapobiega "wypluwaniu" warningow przez komplilator

    for(int i = ilePoziomow; i >= 0; i--)
    {
        for(int j = 0; j <= i; j++)
        {
            if(i == ilePoziomow)                                        // na ostatnim poziomie drzewa cena protfela to po prostu jego payoff
            {
                double tmp = 0.0;
                d_[i][j].zmienDelta(0.0);

                if(INSTR.typ() == "call")
                {
                    tmp = std::max((d_[i][j].cenaAkcji() - INSTR.cenaWykonania()), 0.0);
                }
                else if(INSTR.typ() == "put")
                {
                tmp = std::max((INSTR.cenaWykonania() - d_[i][j].cenaAkcji()), 0.0);
                }

                if(INSTR.pozycja() == "short")
                {
                    tmp = -tmp;
                }

                d_[i][j].zmienWartoscPortfela(tmp);
            }
            else                                                // na kolejnych poziomach uzywamy poprzednich wartosci do wstecznej iteracji
            {
                double deltaWchwili = ((d_[i+1][j+1].wartoscPortfela() - d_[i+1][j].wartoscPortfela())/(d_[i+1][j+1].cenaAkcji() - d_[i+1][j].cenaAkcji()));
                d_[i][j].zmienDelta(deltaWchwili);

                double wartoscWchwili = exp( - delta_t * pm_.stopaWolnaOdRyzyka()) * (p*(d_[i+1][j+1].wartoscPortfela()) + (1-p)*(d_[i+1][j].wartoscPortfela()));
                d_[i][j].zmienWartoscPortfela(wartoscWchwili);
            }
        }
    }
}

std::vector<Wezel> Drzewo::operator[](int& WE) const          //  Wszystkie pozniejsze funkcje ida przez  stala referencje,
{                                                             // dlatego tez niepotrzebny jest dostep przez kopie
    return d_[WE];
}

ParametryModelu Drzewo::pm() const
{
    return pm_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Drzewo zsumujDrzewa(const std::vector<Drzewo>& DRZvec)
{
    /* Zakladamy tutaj, ze obydwa drzewa sa takie same z dokladnoscia do modelu wyceny,
       tzn. ilosc krokow, horyzont czasowy, cena spot i reszta wspolczynnikow sa takie same.
       Zalozylismy to w naszym modelu, co znacznie to ulatwia wycene, gdyz mozemy dodac drzewa po wspolrzednych,
       tzn. w kazdym wezle dodamy do siebie wartosci delta i wartosc portfela, reszta pozostanie bez zmian.

       Jest to poprawne, gdyz najpierw rozbilismy nasz caly podstawowy, duzy portfel na pojedyncze, prostsze do wyceny portfele, stworzylismy ich odpowiedniki wolne od ryzyka,
       a nastepnie zsumujemy je, aby otrzymac portfel wolny od ryzyka, ktorego cena w kazdej chwili, bedzie rowna cenie
       naszego portfela podstawowego. */

    assert(!DRZvec.empty());                             // funkcja ma sens, gdy podany wekter jest niepusty

    Drzewo WY(DRZvec[0].pm());                           // Tworzymy drzewo pasujace do naszego modelu

    for(int i = 0; i < DRZvec[0].size(); i++)                       // w kazdym kroku,
    {
        for(int j = 0; j <= i; j++)                                 // w kazdym wezle
        {
            double TMPdelta = 0;
            double TMPwartosc = 0;

            for(unsigned int k = 0; k < DRZvec.size(); k++)         // sumujemy ze soba wszystkie delty i wartosci,
            {
                TMPdelta += DRZvec[k][i][j].delta();
                TMPwartosc += DRZvec[k][i][j].wartoscPortfela();
            }

            WY.modyfikujWezel(i, j, TMPdelta, TMPwartosc);          // a na koncu modyfikujemy drzewo wyjsciowe
        }
    }

    return WY;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Portfel
{
    private:
        std::vector<Instrument> p_;

    public:
        Portfel(std::vector<Instrument> P = std::vector<Instrument> ())
        {
            p_ = P;
        }

        void push_back(const Instrument& WE);
        Drzewo wycenPortfel(const ParametryModelu& PM) const;
};

void Portfel::push_back(const Instrument& WE)
{
    p_.push_back(WE);
}

Drzewo Portfel::wycenPortfel(const ParametryModelu& PM) const
{
    std::vector<Drzewo> DRZvec;

    for(unsigned int i = 0; i < p_.size(); i++)
    {
        Drzewo TMPdrz(PM);
        TMPdrz.wycenIntrument(p_[i]);
        DRZvec.push_back(TMPdrz);
    }

    return(zsumujDrzewa(DRZvec));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void testujStrumien(std::ifstream& WE, int& ILE)
{
    if(WE.eof())
    {
        ++ILE;
        std::cerr << "Koniec danych!\nIlosc Wczytanych linijek: " << ILE << "\n";
    }
    else if(WE)
    {
        ++ILE;
    }
    else
    {
        std::cerr << "Blad wczytywania danych!\nIlosc Wczytanych linijek: " << ILE << "\n";
    }
}

std::string wyciaganieZlinijki(std::ifstream& WE, int& ILE)
{
    std::string tmp = "-1";                              // w przypadku problemow zwracamy -1, co bêdzie latwe do wykrycia
    getline(WE,tmp);
    testujStrumien(WE, ILE);
        if(WE)
        {
            tmp = (tmp.substr(tmp.find('=') + 1));
            tmp.shrink_to_fit();
        }
    return tmp;

}

void Wczytywanie(ParametryModelu& PM, Portfel& P)               // zalozylismy, ze nasz plik wejsciowy jest dobry, tzn. wszystkie dane sa w odpowiedniej kolejnosc, a takze sa dobrze reprezentowane
{
    std::ifstream we(plikWejsciowy.c_str());
    std::string tmpstr;


    if(we)
    {
        int ile = 0;
        std::vector<std::string> tmp1;

        int i = 1;
        while(i <= 6 && we && !we.eof())
        {
            tmp1.push_back(wyciaganieZlinijki(we, ile));
            ++i;
        }
        if(we)
        {
            PM = ParametryModelu(tmp1);
        }

        getline(we, tmpstr);
        testujStrumien(we, ile);

        int iloscWportfelu = atol(wyciaganieZlinijki(we, ile).c_str());

        int j = 1;
        int ileInstrumentow = 0;

        while(j <= iloscWportfelu && we)
        {
            getline(we, tmpstr);                                                // usuwamy linie odstepu
            testujStrumien(we, ile);

            std::vector<std::string> tmp2;
            int i = 1;
            while(i <= 6 && we)
            {

                tmp2.push_back(wyciaganieZlinijki(we, ile));
                ++i;
            }

            if(we)
            {
                P.push_back(Instrument(tmp2));
                ileInstrumentow++;
            }

            ++j;
        }

        we.close();


        std::string formaINST = " instrumentow.\n";
        if(ileInstrumentow == 1) formaINST = " intstrument.\n";
        else if(2 <= ileInstrumentow && ileInstrumentow <= 4) formaINST = " instrumenty.\n";

        std::cerr << "Portfel zawiera " << ileInstrumentow << formaINST;
    }
    else
    {
        std::cerr << "Blad wczytywania pliku!\n";
    }
}

void Zapis(const Drzewo& D)
{
    std::ofstream wy(plikWyjsciowy.c_str());

    if(wy)
    {
        wy << std::fixed << std::setprecision(2);

        for(int i = 0; i <= D.size()-1; i++)
        {
            for(int j = i; j >= 0 ; j--)
            {
                wy << "time=" << D[i][j].czas() << ", stock price=" << D[i][j].cenaAkcji()
                   << "\nportfolio value=" << D[i][j].wartoscPortfela()
                   << "\nDelta=" << D[i][j].delta();

                if(i != D.size()-1 || j != 0)
                {
                    wy << "\n\n";
                }

            }

            if(i != D.size()-1)
            {
                wy << "\n\n";
            }
        }

        std::cerr << "\nZapisano plik wyjsciowy!\nWcisnij dowolny klawisz.\n";
    }
    else
    {
        std::cerr << "\nBlad zapisu pliku!\nWcisnij dowolny klawisz.\n";
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Portfel p = Portfel();
    ParametryModelu pm = ParametryModelu();
    Wczytywanie(pm, p);

    Zapis(p.wycenPortfel(pm));

    getchar();
    return 0;
}
