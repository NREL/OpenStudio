#ifndef PRJSUBOBJECTSPRIVATE_H
#define PRJSUBOBJECTSPRIVATE_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"

#include <QVector>
#include <QSharedData>

namespace openstudio {
namespace contam {

class WeatherDataPrivate : public QSharedData
{
public:
    WeatherDataPrivate(RX Tambt=RX_INIT(0),RX barpres=RX_INIT(0),RX windspd=RX_INIT(0),RX winddir=RX_INIT(0),
                       RX relhum=RX_INIT(0),int daytyp=0,int uTa=0,int ubP=0,int uws=0,int uwd=0);
    void read(Reader &reader);
    std::string write();
    RX Tambt;  // ambient temperature [K] (R4)
    RX barpres;  // barometric pressure [Pa] NOT corrected to sea level (R4)
    RX windspd;  // wind speed [m/s] (R4)
    RX winddir;  // wind direction: 0 = N, 90 = E, 180 = S, ...; (R4)
    RX relhum;  // relative humidity: 0.0 to 1.0 (R4)
    int daytyp;  // type of day (1-12) (I2)
    int uTa;  // units for Tambt (I2) {W}
    int ubP;  // units for barpres (I2) {W}
    int uws;  // units for windspd (I2) {W}
    int uwd;  // units for winddir (I2) {W}
};

class IconPrivate : public QSharedData
{
public:
    IconPrivate(int icon=0,int col=0,int row=0,int nr=0);
    void read(Reader &reader);
    STRING write();
    int icon;  // icon type - see 'special symbols' in contam.h (I2) {W}
    int col;  // column position on the SketchPad (I2) {W}
    int row;  // row position on the SketchPad (I2) {W}
    int nr;  // zone, path, duct, etc., number (I2) {W}
};

} // contam
} // openstudio

#endif
