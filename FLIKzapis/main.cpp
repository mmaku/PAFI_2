#include <iostream>
#include <math.h>
#include <fstream>

void FLIPROK ()
{
    double
        stan = 10000000,
        sekundyRok = 365 * 24 * 60 * 60,
        lok = 0.05/sekundyRok,
        EFS = 0.025/sekundyRok,
        Lik = (12 * 100000)/sekundyRok,
        czasUplynal = 0;

    while(stan > 0)
    {
        stan = (stan*(1.0 + lok + EFS)) - Lik;
        czasUplynal++;
    }

    czasUplynal = czasUplynal/sekundyRok;

    std::cout << std::fixed << "Fundusz FLIPROK zbankrutował, mineło " << czasUplynal << " lat.\n\n";
}

void zapis (long long& wejscie1, double& wejscie2, double& wejscie3, double& wejscie4, double& wejscie5, double& wejscie6,
               double& wejscie7, double& wejscie8, double& wejscie9, double& wejscie10, double& wejscie11, double& wejscie12)
{
    std::ofstream log("FLIK.txt", std::ios_base::app | std::ios_base::out);
    log << std::fixed << wejscie1/604800 << ',' << wejscie2 << ',' << wejscie3 << ',' << wejscie4 << ','
        << wejscie5 << ',' << wejscie6 << ',' << wejscie7 << ',' << wejscie8 << ',' << wejscie9 << ','
        << wejscie10 << ',' << wejscie11 << ',' << wejscie12 << '\n';
}

void FLIK ()
{
    double
        czasWlatach = 25,
        stan = 10000000,
        sekundyRok = 365 * 24 * 60 * 60,
        lok1 = 0.05/sekundyRok,
        sumaLok1 = 0,
        lok2 = 0.06/sekundyRok,
        sumaLok2 = 0,
        progLok = 20000000,
        sumaLok = 0,
        EFS = 0.1/sekundyRok,
        sumaEFS = 0,
        sumaWplywow = 0,
        Lik1 = (12 * 100000)/sekundyRok,
        sumaLik1 = 0,
        Lik2 = (12 * 50000)/sekundyRok,
        sumaLik2 = 0,
        sumaLik = 0,
        wspolczynnik1 = 0,
        wspolczynnik2 = NAN,
        czasWdouble = 0;
    long long czasUplynal = 0;

	std::cout << czasWlatach << '\n' << stan << '\n' << sekundyRok << '\n' << lok1 << '\n' << sumaLok1 << '\n' << lok2 << '\n'
			  << sumaLok2 << '\n' << progLok << '\n' << sumaLok << '\n' << EFS << '\n' << sumaEFS << '\n' << sumaWplywow << '\n'
			  << Lik1 << '\n' << sumaLik1 << '\n' << Lik2 << '\n' << sumaLik2 << '\n' << sumaLik << '\n' << czasUplynal << '\n';
			  
    while(stan >= 0 && czasUplynal <= czasWlatach*sekundyRok)
    {
        if(stan < progLok)
        {
            if(czasUplynal < sekundyRok)
            {
            stan = (1.0 + lok1)*stan;
            sumaLok1 += stan*lok1;
            sumaLok += stan*lok1;
            sumaWplywow += stan*lok1;
            wspolczynnik1 = (stan*lok1);
            }
            else if(czasUplynal < 10*sekundyRok)
            {
            stan = (1.0 + lok1 + EFS)*stan;
            sumaLok1 += stan*lok1;
            sumaLok += stan*lok1;
            sumaEFS += stan*EFS;
            sumaWplywow += (stan*(lok1 + EFS));
            wspolczynnik1 = ((stan*(lok1 + EFS)));

            }
            else if(czasUplynal < 15*sekundyRok)
            {
            stan = (1.0 + lok1 + EFS)*stan - Lik1;
            sumaLok1 += stan*lok1;
            sumaLok += stan*lok1;
            sumaEFS += stan*EFS;
            sumaWplywow += (stan*(lok1 + EFS));
            sumaLik1 += Lik1;
            sumaLik += Lik1;
            wspolczynnik1 = ((stan*(lok1 + EFS)) - Lik1);
            wspolczynnik2 = ((stan*(lok1 + EFS)) / Lik1);
           }
            else
            {
            stan = (1.0 + lok1 + EFS)*stan - Lik2;
            sumaLok1 += stan*lok1;
            sumaLok += stan*lok1;
            sumaEFS += stan*EFS;
            sumaWplywow += (stan*(lok1 + EFS));
            sumaLik2 += Lik2;
            sumaLik += Lik2;
            wspolczynnik1 = ((stan*(lok1 + EFS)) - Lik2);
            wspolczynnik2 = ((stan*(lok1 + EFS)) / Lik2);
            }
        }
        else
        {
        if(czasUplynal < sekundyRok)
            {
            stan = (1.0 + lok2)*stan;
            sumaLok2 += stan*lok2;
            sumaLok += stan*lok2;
            sumaWplywow += stan*lok2;
            wspolczynnik1 = (stan*lok2);
            }
            else if(czasUplynal < 10*sekundyRok)
            {
            stan = (lok2 + EFS + 1.0)*stan;
            sumaLok2 += stan*lok2;
            sumaLok += stan*lok2;
            sumaEFS += stan*EFS;
            sumaWplywow += (stan*(lok2 + EFS));
            wspolczynnik1 = ((stan*(lok2 + EFS)));

            }
            else if(czasUplynal < 15*sekundyRok)
            {
            stan = (1.0 + lok2 + EFS)*stan - Lik1;
            sumaLok2 += stan*lok2;
            sumaLok += stan*lok2;
            sumaEFS += stan*EFS;
            sumaWplywow += (stan*(lok2 + EFS));
            sumaLik1 += Lik1;
            sumaLik += Lik1;
            wspolczynnik1 = ((stan*(lok2 + EFS)) - Lik1);
            wspolczynnik2 = ((stan*(lok2 + EFS))/Lik1);
            }
            else
            {
            stan = (1.0 + lok2 + EFS)*stan - Lik2;
            sumaLok2 += stan*lok2;
            sumaLok += stan*lok2;
            sumaEFS += stan*EFS;
            sumaWplywow += (stan*(lok2 + EFS));
            sumaLik2 += Lik2;
            sumaLik += Lik2;
            wspolczynnik1 = ((stan*(lok2 + EFS)) - Lik2);
            wspolczynnik2 = ((stan*(lok2 + EFS))/Lik2);
           }
        }
        if (czasUplynal%604800 == 0) zapis(czasUplynal, stan, sumaLok1, sumaLok2, sumaLok, sumaEFS, sumaWplywow,
                                           sumaLik1, sumaLik2, sumaLik, wspolczynnik1, wspolczynnik2);
        czasUplynal++;
    }

    zapis(czasUplynal, stan, sumaLok1, sumaLok2, sumaLok, sumaEFS, sumaWplywow,
         sumaLik1, sumaLik2, sumaLik, wspolczynnik1, wspolczynnik2);
    czasWdouble = (double)czasUplynal;
    czasWdouble = czasWdouble/sekundyRok;
    std::cout << std::fixed << "Minęło " << czasWdouble << " lat\n"
              << "Na koncie funduszu FLIK mamy " << stan << " złotych\n";

}

int main()
{
//    FLIPROK();
    FLIK();
    return(0);
}
