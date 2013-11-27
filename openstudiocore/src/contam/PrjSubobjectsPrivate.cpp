/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
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

std::string WeatherDataPrivate::write()
{
    std::string string;
    string += openstudio::toString(Tambt) + ' ' + openstudio::toString(barpres) + ' ' + openstudio::toString(windspd) + ' ' + openstudio::toString(winddir) + ' ' + openstudio::toString(relhum) + ' ' + openstudio::toString(daytyp) + ' ' + openstudio::toString(uTa) + ' ' + openstudio::toString(ubP) + ' ' + openstudio::toString(uws) + ' ' + openstudio::toString(uwd) + '\n';
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

std::string IconPrivate::write()
{
    std::string string;
    string += openstudio::toString(icon) + ' ' + openstudio::toString(col) + ' ' + openstudio::toString(row) + ' ' + openstudio::toString(nr) + '\n';
    return string;
}

} // contam
} // openstudio
