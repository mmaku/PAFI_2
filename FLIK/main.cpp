#include <iostream>
#include <math.h>

void FLIK ()
{
    double
        czasWlatach = 25,
        stan = 10000000,
        sekundyRok = 365 * 24 * 60 * 60,
        lok1 = 0.05/sekundyRok,
        lok2 = 0.06/sekundyRok,
        progLok = 20000000,
        EFS = 0.1/sekundyRok,
        Lik1 = (12 * 120000)/sekundyRok,
        Lik2 = (12 * 60000)/sekundyRok,
        czasUplynal = 0;

    while(stan >= 0 && czasUplynal <= czasWlatach*sekundyRok)
    {
        if(stan < progLok)
        {
            if(czasUplynal < sekundyRok)         stan = stan*(1.0 + lok1);
            else if(czasUplynal < 10*sekundyRok) stan = stan*(1.0 + lok1 + EFS);
            else if(czasUplynal < 15*sekundyRok) stan = stan*(1.0 + lok1 + EFS) - Lik1;
            else                                 stan = stan*(1.0 + lok1 + EFS) - Lik2;
        }
        else
        {
            if(czasUplynal < sekundyRok)         stan = stan*(1.0 + lok2);
            else if(czasUplynal < 10*sekundyRok) stan = stan*(1.0 + lok2 + EFS);
            else if(czasUplynal < 15*sekundyRok) stan = stan*(1.0 + lok2 + EFS) - Lik1;
            else                                 stan = stan*(1.0 + lok2 + EFS) - Lik2;
        }
        czasUplynal++;
    }

    czasUplynal = czasUplynal/sekundyRok;
    std::cout << "Mineło " << czasUplynal << " lat\n"
              << std::fixed << "Na koncie funduszu FLIK mamy " << stan << " złotych\n";

}

int main()
{
    FLIK();
    return(0);
}
