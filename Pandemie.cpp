//============================================================================
// Name        : Pandemie.cpp
// Author      : Thibaut DA SILVA
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

#define CDFT "\033[0m"     //default
#define CBLD "\033[1m"     //bold
#define CBLK "\033[30;40m" //black
#define CRED "\033[31;41m" //red
#define CGRN "\033[32;42m" //green
#define CYLW "\033[33;43m" //yellow
#define CBLU "\033[34;44m" //blue

int g_cmptImmunise = 0;
int g_cmptMort     = 0;
float g_probaMalade = 0.0;
float g_probaGuerison = 0.0;

enum eEtatSante
{
    E_NONE = 0,
    E_SAIN,
    E_CONTAMINE,
    E_MALADE,
    E_IMMUNISE,
    E_MORT
};

string fnEtatToString(eEtatSante etat)
{
    string strEtat;

    switch(etat)
    {
    case E_NONE:
        strEtat = "NONE";
        break;

    case E_SAIN:
        strEtat = "SAIN";
        break;

    case E_CONTAMINE:
        strEtat = "CONTAMINE";
        break;

    case E_MALADE:
        strEtat = "MALADE";
        break;

    case E_IMMUNISE:
        strEtat = "IMMUNISE";
        break;

    case E_MORT:
        strEtat = "MORT";
        break;
    }

    return strEtat;
}

eEtatSante** fnRemplissageInit(const unsigned int sizeX, const unsigned int sizeY)
{
    eEtatSante** table = new eEtatSante*[sizeX];
    for (unsigned int i = 0; i < sizeX; ++i)
    {
       table[i] = new eEtatSante[sizeY];
    }

    srand(time(0));
    unsigned int randX = rand()%sizeX;
    unsigned int randY = rand()%sizeY;
    bool bOk = false;

    for(unsigned int i = 0; i < sizeX; i++)
    {
        for(unsigned int j = 0; j < sizeY; j++)
        {
            if(not bOk and i == randX and j == randY)
            {
                //cout << "[" << i << "][" << j << "]: E_MALADE " << endl;
                table[i][j] = E_MALADE;
                bOk = true;
            }
            else
            {
                //cout << "[" << i << "][" << j << "]: E_SAIN   " << endl;
                table[i][j] = E_SAIN;
            }
        }
        cout << endl;
    }

    return table;
}

void fnFlush()
{
    int ch;
    while ((ch=getchar()) != EOF && ch != '\n'); //Flush de stdin
}

void fnPrintArray(eEtatSante** table, const unsigned int sizeX, const unsigned int sizeY)
{
    for(unsigned int i = 0; i < sizeX; i++)
    {
        cout << "    ";
        for(unsigned int j = 0; j < sizeY; j++)
        {
            switch(table[i][j])
            {
            case E_SAIN :
                cout << CBLU << "  " << CDFT;
                break;

            case E_MALADE :
                cout << CRED << CBLD << "  " << CDFT;
                break;

            case E_IMMUNISE :
                cout << CGRN << "  " << CDFT;
                break;

            case E_MORT :
                cout << CBLK << "  " << CDFT;
                break;

            default :
                break;
            }
        }
        cout << endl;
    }
    cout << endl;
}

eEtatSante fnTestContamination()
{
    eEtatSante etat;

    unsigned int randomInt = rand()%100;

    if(randomInt <= g_probaMalade*100)
    {
        etat = E_CONTAMINE;
    }
    else
    {
        etat = E_SAIN;
    }

    return etat;
}

eEtatSante fnTestGuerison()
{
    eEtatSante etat;

    unsigned int randomInt = rand()%100;

    if(randomInt <= g_probaGuerison*100)
    {
        etat = E_IMMUNISE;
        g_cmptImmunise++;
    }
    else
    {
        etat = E_MORT;
        g_cmptMort++;
    }

    return etat;
}

void fnDevientMalade(eEtatSante** table, unsigned int sizeX, unsigned int sizeY)
{
    for(unsigned int i = 0; i < sizeX; i++)
    {
        for(unsigned int j = 0; j < sizeY; j++)
        {
            if(table[i][j] == E_CONTAMINE)
            {
                table[i][j] = E_MALADE;
            }
        }
    }
}

bool fnPropagation(eEtatSante** table, unsigned int sizeX, unsigned int sizeY)
{
    bool bMaladeExiste = false;

    for(unsigned int i = 0; i < sizeX; i++)
    {
        for(unsigned int j = 0; j < sizeY; j++)
        {
            if(table[i][j] == E_MALADE)
            {
                //cout << CRED << i << ":" << j << " malade !" << CDFT << endl;
                bMaladeExiste = true;

                if(i > 0)
                {
                    if(j > 0 and table[i-1][j-1] == E_SAIN)
                    {
                        table[i-1][j-1] = fnTestContamination();
                        //cout << "\t" << CRED << "i-1:j-1" << " etat:" << fnEtatToString(table[i-1][j-1]).c_str() << CDFT << endl;
                    }
                    if(table[i-1][j] == E_SAIN)
                    {
                        table[i-1][j] = fnTestContamination();
                        //cout << "\t" << CRED << "i-1:j  " << " etat:" << fnEtatToString(table[i-1][j]).c_str() << CDFT << endl;
                    }
                    if(j < sizeY-1 and table[i-1][j+1] == E_SAIN)
                    {
                        table[i-1][j+1] = fnTestContamination();
                        //cout << "\t" << CRED << "i-1:j+1" << " etat:" << fnEtatToString(table[i-1][j+1]).c_str() << CDFT << endl;
                    }
                }

                if(i < sizeX-1)
                {
                    if(j > 0 and table[i+1][j-1] == E_SAIN)
                    {
                        table[i+1][j-1] = fnTestContamination();
                        //cout << "\t" << CRED << "i+1:j-1" << " etat:" << fnEtatToString(table[i+1][j-1]).c_str() << CDFT << endl;
                    }
                    if(table[i+1][j] == E_SAIN)
                    {
                        table[i+1][j] = fnTestContamination();
                        //cout << "\t" << CRED << "i+1:j  " << " etat:" << fnEtatToString(table[i+1][j]).c_str() << CDFT << endl;
                    }
                    if(j < sizeY-1 and table[i+1][j+1] == E_SAIN)
                    {
                        table[i+1][j+1] = fnTestContamination();
                        //cout << "\t" << CRED << "i+1:j-1" << " etat:" << fnEtatToString(table[i+1][j+1]).c_str() << CDFT << endl;
                    }
                }

                if(j > 0 and table[i][j-1] == E_SAIN)
                {
                    table[i][j-1] = fnTestContamination();
                    //cout << "\t" << CRED << "i  :j-1" << " etat:" << fnEtatToString(table[i][j-1]).c_str() << CDFT << endl;
                }

                if(j < sizeY-1 and table[i][j+1] == E_SAIN)
                {
                    table[i][j+1] = fnTestContamination();
                    //cout << "\t" << CRED << "i  :j+1" << " etat:" << fnEtatToString(table[i][j+1]).c_str() << CDFT << endl;
                }

                table[i][j] = fnTestGuerison();
            }
        }
    }

    fnDevientMalade(table, sizeX, sizeY);

    return bMaladeExiste;
}

int main(int argc, char *argv[])
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	int SIZE_X;
	int SIZE_Y;

	if(argc == 1)
	{
        SIZE_X          = 50;
        SIZE_Y          = 50;
	    g_probaMalade   = 0.3;
	    g_probaGuerison = 0.5;
	}
	else if(argc == 5)
	{
        SIZE_X          = atoi(argv[1]);
        SIZE_Y          = atoi(argv[2]);
        g_probaMalade   = atof(argv[3]);
        g_probaGuerison = atof(argv[4]);

	}
	else
	{
	    cout << argv[0] << " <int sizeX> <int sizeY> <float probaMaladie> <float probaGuerison>"<< endl;
	    exit(-1);
	}



	eEtatSante** array;

	array = fnRemplissageInit(SIZE_X, SIZE_Y);

	bool bMaladeExistant = true;

	while(bMaladeExistant)
	{
	    system("clear");

	    fnPrintArray(array, SIZE_X, SIZE_Y);
	    bMaladeExistant = fnPropagation(array, SIZE_X, SIZE_Y);

	    usleep(1000000);
	    //getchar();
	    //fnFlush();
	}

	int cmptSain = ( (SIZE_X*SIZE_Y) - g_cmptMort - g_cmptImmunise );

	cout << "Bilan de l'epidemie:" << endl;

	cout << "\tL'epidemie a fait " << g_cmptMort << " morts, soit " << ( ( (float)g_cmptMort/float(SIZE_X*SIZE_Y) ) * 100.0 ) << "% de la pop" << endl;
	cout << "\t" << g_cmptImmunise << " personnes y ont survecu, soit " << ( ( (float)g_cmptImmunise/float(SIZE_X*SIZE_Y) ) * 100.0 ) << "% de la pop" << endl;
	cout << "\tLes autres (" << cmptSain << " personnes) n'ont pas ete infectees, soit " << ( ( (float)cmptSain/float(SIZE_X*SIZE_Y) ) * 100.0 ) << "% de la pop" << endl;

	return 0;
}
