#include "PrjSubobjectsPrivate.hpp"

namespace openstudio {
namespace contam {

WeatherDataPrivate::WeatherDataPrivate(RX Tambt,RX barpres,RX windspd,RX winddir,RX relhum,int daytyp,int uTa,int ubP,int uws,int uwd):Tambt(Tambt),barpres(barpres),windspd(windspd),winddir(winddir),relhum(relhum),daytyp(daytyp),uTa(uTa),ubP(ubP),uws(uws),uwd(uwd){}

void WeatherDataPrivate::read(Reader &input)
{
    Tambt = input.readNumber<RX>(FILELINE);
    barpres = input.readNumber<RX>(FILELINE);
    windspd = input.readNumber<RX>(FILELINE);
    winddir = input.readNumber<RX>(FILELINE);
    relhum = input.readNumber<RX>(FILELINE);
    daytyp = input.read<int>(FILELINE);
    uTa = input.read<int>(FILELINE);
    ubP = input.read<int>(FILELINE);
    uws = input.read<int>(FILELINE);
    uwd = input.read<int>(FILELINE);
}

STRING WeatherDataPrivate::write()
{
    STRING string;
    string += TO_STRING(Tambt) + ' ' + TO_STRING(barpres) + ' ' + TO_STRING(windspd) + ' ' + TO_STRING(winddir) + ' ' + TO_STRING(relhum) + ' ' + TO_STRING(daytyp) + ' ' + TO_STRING(uTa) + ' ' + TO_STRING(ubP) + ' ' + TO_STRING(uws) + ' ' + TO_STRING(uwd) + '\n';
    return string;
}

IconPrivate::IconPrivate(int icon,int col,int row,int nr):icon(icon),col(col),row(row),nr(nr)
{}

void IconPrivate::read(Reader &input)
{
    icon = input.read<int>(FILELINE);
    col = input.read<int>(FILELINE);
    row = input.read<int>(FILELINE);
    nr = input.read<int>(FILELINE);
}

STRING IconPrivate::write()
{
    STRING string;
    string += TO_STRING(icon) + ' ' + TO_STRING(col) + ' ' + TO_STRING(row) + ' ' + TO_STRING(nr) + '\n';
    return string;
}

} // contam
} // openstudio
