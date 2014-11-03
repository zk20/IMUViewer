
#include "tableaudebord.h"


TableauDeBord::TableauDeBord()
{

    std::cout<<"Constructeur tdb"<<std::endl;
    // Lecture du fichier des données centrale
    CSV donneesCentrale;
    double **donneesBrutes   = donneesCentrale.readCSV(fichierCsv);
    // Nombre de lignes du fichier
    _nbEch = donneesCentrale.getNbLines();
    // Cree un vecteur de signaux avec toutes les données
    creeVecteurSignaux(donneesBrutes,freqFiltre,freqEch,true);

}


TableauDeBord::~TableauDeBord()
{}


void TableauDeBord::creeVecteurSignaux(double** donneesBrutes,  FrequencyType uneFreqFiltre, FrequencyType uneFreqEch,bool reEchantillone)
{

    // Données de l'accéléromètre
    for (int i=2;i<=4;i++)
    {
        Signal *signalBrut = new Signal(donneesBrutes,_nbEch,0,i);
        //signalBrut.passeBas(uneFreqFiltre,uneFreqEch,reEchantillone);
        signalBrut->doubleIntegre();
        _signaux.append(signalBrut);
    }

    // Données du gyroscope
    for (int i=6;i<=8;i++)
    {
        Signal *signalBrut = new Signal(donneesBrutes,_nbEch,0,i);
        //signalBrut.passeBas(uneFreqFiltre,uneFreqEch,reEchantillone);
        signalBrut->integre();
        _signaux.append(signalBrut);
    }
    // Données du magnéto
    for (int i=10;i<=12;i++)
    {
        Signal *signalBrut = new Signal(donneesBrutes,_nbEch,0,i);
        //signalBrut.passeBas(uneFreqFiltre,uneFreqEch,reEchantillone);
        _signaux.append(signalBrut);
    }

}


// Mise à jour des données du capteur de la centrale
void TableauDeBord::majCentrale()
{
    // Acc
    _IMU._acc[0] = _signaux[0]->getSignal(iCourant);
    _IMU._acc[1] = _signaux[1]->getSignal(iCourant);
    _IMU._acc[2] = _signaux[2]->getSignal(iCourant);
    // Gyro
    _IMU._gyro[0]= _signaux[3]->getSignal(iCourant);
    _IMU._gyro[1]= _signaux[4]->getSignal(iCourant);
    _IMU._gyro[2]= _signaux[5]->getSignal(iCourant);
    // Magnétomètre
    _IMU._magn[0]=_signaux[6]->getSignal(iCourant);
    _IMU._magn[1]=_signaux[7]->getSignal(iCourant);
    _IMU._magn[2]=_signaux[8]->getSignal(iCourant);

    //Position depuis l'accéléro
    _IMU._acc2I[0] = _signaux[0]->getSignalDoubleIntegre(iCourant);
    _IMU._acc2I[1] = _signaux[1]->getSignalDoubleIntegre(iCourant);
    _IMU._acc2I[2] = _signaux[2]->getSignalDoubleIntegre(iCourant);

    // Angle depuis le gyro
    _IMU._gyroI[0]= _signaux[3]->getSignalIntegre(iCourant);
    _IMU._gyroI[1]= _signaux[4]->getSignalIntegre(iCourant);
    _IMU._gyroI[2]= _signaux[5]->getSignalIntegre(iCourant);

    // Incrémentation de l'indice de parcours des données
    iCourant = (iCourant < (_signaux.at(0)->getTaille() -1)) ? iCourant+1:0;
    std::cout<< iCourant<< "acc = " << _IMU._acc[0]<< std::endl;
}
