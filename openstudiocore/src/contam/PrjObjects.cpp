/**********************************************************************
 *  Copyright (c) 2013, The Pennsylvania State University.
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
#include "PrjObjects.hpp"
#include "PrjDefs.hpp"

#include <utilities/core/Logger.hpp>

CONTAMNAMESPACESTART
namespace prj
{

static void error(QString mesg DECCFILELINE)
{
	//LOG_FREE(Error, mesg.toStdString());
//#ifdef NOFILELINE
//    std::cout << mesg.toStdString() << std::endl;
//#else
//    std::cout << mesg.toStdString() << '(' << file << ',' << line << ")\n";
//#endif
    exit(EXIT_FAILURE);
}

// Section 1: Project, Weather, Simulation, and Output Controls

Weather::Weather()
{
    Tambt=RX_I(0);
    barpres=RX_I(0);
    windspd=RX_I(0);
    winddir=RX_I(0);
    relhum=RX_I(0);
    daytyp=0;
    uTa=0;
    ubP=0;
    uws=0;
    uwd=0;
}

void Weather::read(Reader *input)
{
    Tambt = input->readNumber<RX>(FILELINE);
    barpres = input->readNumber<RX>(FILELINE);
    windspd = input->readNumber<RX>(FILELINE);
    winddir = input->readNumber<RX>(FILELINE);
    relhum = input->readNumber<RX>(FILELINE);
    daytyp = input->readInt(FILELINE);
    uTa = input->readInt(FILELINE);
    ubP = input->readInt(FILELINE);
    uws = input->readInt(FILELINE);
    uwd = input->readInt(FILELINE);
}

QString Weather::write()
{
    return QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10").arg(RX_C(Tambt)).arg(RX_C(barpres)).arg(RX_C(windspd))
            .arg(RX_C(winddir)).arg(RX_C(relhum)).arg(daytyp).arg(uTa).arg(ubP).arg(uws).arg(uwd);
}

RunControl::RunControl()
{
    echo=0;
    skheight=0;
    skwidth=0;
    def_units=0;
    def_flows=0;
    def_T=RX_I(0);
    udefT=0;
    rel_N=RX_I(0);
    wind_H=RX_I(0);
    uwH=0;
    wind_Ao=RX_I(0);
    wind_a=RX_I(0);
    scale=RX_I(0);
    uScale=0;
    orgRow=0;
    orgCol=0;
    invYaxis=0;
    showGeom=0;
    X0=RX_I(0);
    Y0=RX_I(0);
    Z0=RX_I(0);
    angle=RX_I(0);
    u_XYZ=0;
    epsPath=RX_I(0);
    epsSpcs=RX_I(0);
    tShift=QString();
    useWPCwp=0;
    useWPCmf=0;
    wpctrig=0;
    latd=RX_I(0);
    lgtd=RX_I(0);
    Tznr=RX_I(0);
    altd=RX_I(0);
    Tgrnd=RX_I(0);
    utg=0;
    u_a=0;
    sim_af=0;
    afcalc=0;
    afmaxi=0;
    afrcnvg=RX_I(0);
    afacnvg=RX_I(0);
    afrelax=RX_I(0);
    uac2=0;
    Pres=RX_I(0);
    uPres=0;
    afslae=0;
    afrseq=0;
    aflmaxi=0;
    aflcnvg=RX_I(0);
    aflinit=0;
    Tadj=0;
    sim_mf=0;
    ccmaxi=0;
    ccrcnvg=RX_I(0);
    ccacnvg=RX_I(0);
    ccrelax=RX_I(0);
    uccc=0;
    mfnmthd=0;
    mfnrseq=0;
    mfnmaxi=0;
    mfnrcnvg=RX_I(0);
    mfnacnvg=RX_I(0);
    mfnrelax=RX_I(0);
    mfngamma=RX_I(0);
    uccn=0;
    mftmthd=0;
    mftrseq=0;
    mftmaxi=0;
    mftrcnvg=RX_I(0);
    mftacnvg=RX_I(0);
    mftrelax=RX_I(0);
    mftgamma=RX_I(0);
    ucct=0;
    mfvmthd=0;
    mfvrseq=0;
    mfvmaxi=0;
    mfvrcnvg=RX_I(0);
    mfvacnvg=RX_I(0);
    mfvrelax=RX_I(0);
    uccv=0;
    mf_solver=0;
    sim_1dz=0;
    sim_1dd=0;
    celldx=RX_I(0);
    sim_vjt=0;
    udx=0;
    cvode_mth=0;
    cvode_rcnvg=RX_I(0);
    cvode_acnvg=RX_I(0);
    cvode_dtmax=RX_I(0);
    tsdens=0;
    tsrelax=RX_I(0);
    tsmaxi=0;
    cnvgSS=0;
    densZP=0;
    stackD=0;
    dodMdt=0;
    restart=0;
    list=0;
    doDlg=0;
    pfsave=0;
    zfsave=0;
    zcsave=0;
    achvol=0;
    achsave=0;
    abwsave=0;
    cbwsave=0;
    expsave=0;
    ebwsave=0;
    zaasave=0;
    zbwsave=0;
    rzfsave=0;
    rzmsave=0;
    rz1save=0;
    csmsave=0;
    srfsave=0;
    logsave=0;
    BldgFlowZ=0;
    BldgFlowD=0;
    BldgFlowC=0;
    cfd_ctype=0;
    cfd_convcpl=RX_I(0);
    cfd_var=0;
    cfd_zref=0;
    cfd_imax=0;
    cfd_dtcmo=0;
}

void RunControl::read(Reader *input)
{
    name = input->readString(FILELINE);
    version = input->readString(FILELINE);
    echo = input->readInt(FILELINE);
    prjdesc = input->readLine(FILELINE);
    skheight = input->readInt(FILELINE);
    skwidth = input->readInt(FILELINE);
    def_units = input->readInt(FILELINE);
    def_flows = input->readInt(FILELINE);
    def_T = input->readNumber<RX>(FILELINE);
    udefT = input->readInt(FILELINE);
    rel_N = input->readNumber<RX>(FILELINE);
    wind_H = input->readNumber<RX>(FILELINE);
    uwH = input->readInt(FILELINE);
    wind_Ao = input->readNumber<RX>(FILELINE);
    wind_a = input->readNumber<RX>(FILELINE);
    scale = input->readNumber<RX>(FILELINE);
    uScale = input->readInt(FILELINE);
    orgRow = input->readInt(FILELINE);
    orgCol = input->readInt(FILELINE);
    invYaxis = input->readInt(FILELINE);
    showGeom = input->readInt(FILELINE);
    ssWeather.read(input);
    wptWeather.read(input);
    WTHpath = input->readString(FILELINE);
    CTMpath = input->readString(FILELINE);
    CVFpath = input->readString(FILELINE);
    DVFpath = input->readString(FILELINE);
    WPCfile = input->readString(FILELINE);
    EWCfile = input->readString(FILELINE);
    WPCdesc = input->readLine(FILELINE);
    X0 = input->readNumber<RX>(FILELINE);
    Y0 = input->readNumber<RX>(FILELINE);
    Z0 = input->readNumber<RX>(FILELINE);
    angle = input->readNumber<RX>(FILELINE);
    u_XYZ = input->readInt(FILELINE);
    epsPath = input->readNumber<RX>(FILELINE);
    epsSpcs = input->readNumber<RX>(FILELINE);
    tShift = input->readString(FILELINE);
    dStart = input->readString(FILELINE);
    dEnd = input->readString(FILELINE);
    useWPCwp = input->readInt(FILELINE);
    useWPCmf = input->readInt(FILELINE);
    wpctrig = input->readInt(FILELINE);
    latd = input->readNumber<RX>(FILELINE);
    lgtd = input->readNumber<RX>(FILELINE);
    Tznr = input->readNumber<RX>(FILELINE);
    altd = input->readNumber<RX>(FILELINE);
    Tgrnd = input->readNumber<RX>(FILELINE);
    utg = input->readInt(FILELINE);
    u_a = input->readInt(FILELINE);
    sim_af = input->readInt(FILELINE);
    afcalc = input->readInt(FILELINE);
    afmaxi = input->readInt(FILELINE);
    afrcnvg = input->readNumber<RX>(FILELINE);
    afacnvg = input->readNumber<RX>(FILELINE);
    afrelax = input->readNumber<RX>(FILELINE);
    uac2 = input->readInt(FILELINE);
    Pres = input->readNumber<RX>(FILELINE);
    uPres = input->readInt(FILELINE);
    afslae = input->readInt(FILELINE);
    afrseq = input->readInt(FILELINE);
    aflmaxi = input->readInt(FILELINE);
    aflcnvg = input->readNumber<RX>(FILELINE);
    aflinit = input->readInt(FILELINE);
    Tadj = input->readInt(FILELINE);
    sim_mf = input->readInt(FILELINE);
    ccmaxi = input->readInt(FILELINE);
    ccrcnvg = input->readNumber<RX>(FILELINE);
    ccacnvg = input->readNumber<RX>(FILELINE);
    ccrelax = input->readNumber<RX>(FILELINE);
    uccc = input->readInt(FILELINE);
    mfnmthd = input->readInt(FILELINE);
    mfnrseq = input->readInt(FILELINE);
    mfnmaxi = input->readInt(FILELINE);
    mfnrcnvg = input->readNumber<RX>(FILELINE);
    mfnacnvg = input->readNumber<RX>(FILELINE);
    mfnrelax = input->readNumber<RX>(FILELINE);
    mfngamma = input->readNumber<RX>(FILELINE);
    uccn = input->readInt(FILELINE);
    mftmthd = input->readInt(FILELINE);
    mftrseq = input->readInt(FILELINE);
    mftmaxi = input->readInt(FILELINE);
    mftrcnvg = input->readNumber<RX>(FILELINE);
    mftacnvg = input->readNumber<RX>(FILELINE);
    mftrelax = input->readNumber<RX>(FILELINE);
    mftgamma = input->readNumber<RX>(FILELINE);
    ucct = input->readInt(FILELINE);
    mfvmthd = input->readInt(FILELINE);
    mfvrseq = input->readInt(FILELINE);
    mfvmaxi = input->readInt(FILELINE);
    mfvrcnvg = input->readNumber<RX>(FILELINE);
    mfvacnvg = input->readNumber<RX>(FILELINE);
    mfvrelax = input->readNumber<RX>(FILELINE);
    uccv = input->readInt(FILELINE);
    mf_solver = input->readInt(FILELINE);
    sim_1dz = input->readInt(FILELINE);
    sim_1dd = input->readInt(FILELINE);
    celldx = input->readNumber<RX>(FILELINE);
    sim_vjt = input->readInt(FILELINE);
    udx = input->readInt(FILELINE);
    cvode_mth = input->readInt(FILELINE);
    cvode_rcnvg = input->readNumber<RX>(FILELINE);
    cvode_acnvg = input->readNumber<RX>(FILELINE);
    cvode_dtmax = input->readNumber<RX>(FILELINE);
    tsdens = input->readInt(FILELINE);
    tsrelax = input->readNumber<RX>(FILELINE);
    tsmaxi = input->readInt(FILELINE);
    cnvgSS = input->readInt(FILELINE);
    densZP = input->readInt(FILELINE);
    stackD = input->readInt(FILELINE);
    dodMdt = input->readInt(FILELINE);
    date_st = input->readString(FILELINE);
    time_st = input->readString(FILELINE);
    date_0 = input->readString(FILELINE);
    time_0 = input->readString(FILELINE);
    date_1 = input->readString(FILELINE);
    time_1 = input->readString(FILELINE);
    time_step = input->readString(FILELINE);
    time_list = input->readString(FILELINE);
    time_scrn = input->readString(FILELINE);
    restart = input->readInt(FILELINE);
    rstdate = input->readString(FILELINE);
    rsttime = input->readString(FILELINE);
    list = input->readInt(FILELINE);
    doDlg = input->readInt(FILELINE);
    pfsave = input->readInt(FILELINE);
    zfsave = input->readInt(FILELINE);
    zcsave = input->readInt(FILELINE);
    achvol = input->readInt(FILELINE);
    achsave = input->readInt(FILELINE);
    abwsave = input->readInt(FILELINE);
    cbwsave = input->readInt(FILELINE);
    expsave = input->readInt(FILELINE);
    ebwsave = input->readInt(FILELINE);
    zaasave = input->readInt(FILELINE);
    zbwsave = input->readInt(FILELINE);
    rzfsave = input->readInt(FILELINE);
    rzmsave = input->readInt(FILELINE);
    rz1save = input->readInt(FILELINE);
    csmsave = input->readInt(FILELINE);
    srfsave = input->readInt(FILELINE);
    logsave = input->readInt(FILELINE);
    for(int i=0;i<16;i++)
        save[i] = input->readInt(FILELINE);
    int nrvals = input->readInt(FILELINE);
    rvals = QVector<RX>(nrvals);
    for(int i=0;i<nrvals;i++)
        rvals[i] = input->readNumber<RX>(FILELINE);
    BldgFlowZ = input->readInt(FILELINE);
    BldgFlowD = input->readInt(FILELINE);
    BldgFlowC = input->readInt(FILELINE);
    cfd_ctype = input->readInt(FILELINE);
    cfd_convcpl = input->readNumber<RX>(FILELINE);
    cfd_var = input->readInt(FILELINE);
    cfd_zref = input->readInt(FILELINE);
    cfd_imax = input->readInt(FILELINE);
    cfd_dtcmo = input->readInt(FILELINE);
}

QString RunControl::write()
{
    QString string;
    string += QString("%1 %2 %3\n").arg(name).arg(version).arg(echo);
    string += prjdesc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").arg(skheight).arg(skwidth).arg(def_units)
            .arg(def_flows).arg(RX_C(def_T)).arg(udefT).arg(RX_C(rel_N)).arg(RX_C(wind_H)).arg(uwH)
            .arg(RX_C(wind_Ao)).arg(RX_C(wind_a));
    string += QString("\n%1 %2 %3 %4 %5 %6\n").arg(RX_C(scale)).arg(uScale).arg(orgRow).arg(orgCol).arg(invYaxis).arg(showGeom);
    string += ssWeather.write() + '\n' + wptWeather.write();
    string += '\n' + WTHpath;
    string += '\n' + CTMpath;
    string += '\n' + CVFpath;
    string += '\n' + DVFpath;
    string += '\n' + WPCfile;
    string += '\n' + EWCfile;
    string += '\n' + WPCdesc;
    string += QString("\n%1 %2 %3 %4 %5").arg(RX_C(X0)).arg(RX_C(Y0)).arg(RX_C(Z0)).arg(RX_C(angle)).arg(u_XYZ);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8").arg(RX_C(epsPath)).arg(RX_C(epsSpcs)).arg(tShift).arg(dStart)
            .arg(dEnd).arg(useWPCwp).arg(useWPCmf).arg(wpctrig);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(RX_C(latd)).arg(RX_C(lgtd)).arg(RX_C(Tznr)).arg(RX_C(altd))
            .arg(RX_C(Tgrnd)).arg(utg).arg(u_a);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9").arg(sim_af).arg(afcalc).arg(afmaxi).arg(RX_C(afrcnvg))
            .arg(RX_C(afacnvg)).arg(RX_C(afrelax)).arg(uac2).arg(RX_C(Pres)).arg(uPres);
    string += QString("\n%1 %2 %3 %4 %5 %6").arg(afslae).arg(afrseq).arg(aflmaxi).arg(RX_C(aflcnvg)).arg(aflinit).arg(Tadj);
    string += QString("\n%1 %2 %3 %4 %5 %6").arg(sim_mf).arg(ccmaxi).arg(RX_C(ccrcnvg)).arg(RX_C(ccacnvg))
            .arg(RX_C(ccrelax)).arg(uccc);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8").arg(mfnmthd).arg(mfnrseq).arg(mfnmaxi).arg(RX_C(mfnrcnvg))
            .arg(RX_C(mfnacnvg)).arg(RX_C(mfnrelax)).arg(RX_C(mfngamma)).arg(uccn);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8").arg(mftmthd).arg(mftrseq).arg(mftmaxi).arg(RX_C(mftrcnvg))
            .arg(RX_C(mftacnvg)).arg(RX_C(mftrelax)).arg(RX_C(mftgamma)).arg(ucct);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(mfvmthd).arg(mfvrseq).arg(mfvmaxi).arg(RX_C(mfvrcnvg))
            .arg(RX_C(mfvacnvg)).arg(RX_C(mfvrelax)).arg(uccv);
    string += QString("\n%1 %2 %3 %4 %5 %6").arg(mf_solver).arg(sim_1dz).arg(sim_1dd).arg(RX_C(celldx)).arg(sim_vjt).arg(udx);
    string += QString("\n%1 %2 %3 %4").arg(cvode_mth).arg(RX_C(cvode_rcnvg)).arg(RX_C(cvode_acnvg)).arg(RX_C(cvode_dtmax));
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(tsdens).arg(RX_C(tsrelax)).arg(tsmaxi).arg(cnvgSS).arg(densZP)
            .arg(stackD).arg(dodMdt);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9").arg(date_st).arg(time_st).arg(date_0).arg(time_0).arg(date_1)
            .arg(time_1).arg(time_step).arg(time_list).arg(time_scrn);
    string += QString("\n%1 %2 %3").arg(restart).arg(rstdate).arg(rsttime);
    string += QString("\n%1 %2 %3 %4 %5").arg(list).arg(doDlg).arg(pfsave).arg(zfsave)
            .arg(zcsave);
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8").arg(achvol).arg(achsave).arg(abwsave).arg(cbwsave)
            .arg(expsave).arg(ebwsave).arg(zaasave).arg(zbwsave);
    string += QString("\n%1 %2 %3 %4 %5 %6\n").arg(rzfsave).arg(rzmsave).arg(rz1save).arg(csmsave).arg(srfsave).arg(logsave);
    for(int i=0;i<16;i++)
        string += QString(" %1").arg(save[i]);
    string += QString("\n%1\n").arg(rvals.size());
    for(int i=0;i<rvals.size();i++)
        string += QString(" %1").arg(rvals[i]);
    string += QString("\n%1 %2 %3").arg(BldgFlowZ).arg(BldgFlowD).arg(BldgFlowC);
    string += QString("\n%1 %2 %3 %4 %5 %6").arg(cfd_ctype).arg(RX_C(cfd_convcpl)).arg(cfd_var).arg(cfd_zref)
            .arg(cfd_imax).arg(cfd_dtcmo);
    return string;
}

// Section 2: Species and Contaminants

Species::Species()
{
    nr=0;
    sflag=0;
    ntflag=0;
    molwt=RX_I(0);
    mdiam=RX_I(0);
    edens=RX_I(0);
    decay=RX_I(0);
    Dm=RX_I(0);
    ccdef=RX_I(0);
    Cp=RX_I(0);
    ucc=0;
    umd=0;
    ued=0;
    udm=0;
    ucp=0;
}

void Species::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    sflag = input->readInt(FILELINE);
    ntflag = input->readInt(FILELINE);
    molwt = input->readNumber<RX>(FILELINE);
    mdiam = input->readNumber<RX>(FILELINE);
    edens = input->readNumber<RX>(FILELINE);
    decay = input->readNumber<RX>(FILELINE);
    Dm = input->readNumber<RX>(FILELINE);
    ccdef = input->readNumber<RX>(FILELINE);
    Cp = input->readNumber<RX>(FILELINE);
    ucc = input->readInt(FILELINE);
    umd = input->readInt(FILELINE);
    ued = input->readInt(FILELINE);
    udm = input->readInt(FILELINE);
    ucp = input->readInt(FILELINE);
    name = input->readString(FILELINE);
    desc = input->readLine(FILELINE);
}

QString Species::write()
{
    return QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16\n").arg(nr).arg(sflag).arg(ntflag)
            .arg(RX_C(molwt)).arg(RX_C(mdiam)).arg(RX_C(edens)).arg(RX_C(decay)).arg(RX_C(Dm)).arg(RX_C(ccdef))
            .arg(RX_C(Cp)).arg(ucc).arg(umd).arg(ued).arg(udm).arg(ucp).arg(name) + desc;
}

// Section 3: Level and Icon Data

Icon::Icon()
{
    icon=0;
    col=0;
    row=0;
    nr=0;
}

void Icon::read(Reader *input)
{
    this->icon = input->readInt(FILELINE);
    this->col = input->readInt(FILELINE);
    this->row = input->readInt(FILELINE);
    this->nr = input->readInt(FILELINE);
}

QString Icon::write()
{
    return QString("%1 %2 %3 %4").arg(icon).arg(col).arg(row).arg(nr);
}

bool Icon::isWall()
{
    return (this->icon >= WL_EW) && (this->icon <= WL_NESW);
}

uint Icon::bits()
{
    if(isWall())
        switch(icon)
        {
        case WL_EW:
            return BIT_EW;
        case WL_NS:
            return BIT_NS;
        case WL_ES:
            return BIT_ES;
        case WL_SW:
            return BIT_SW;
        case WL_NW:
            return BIT_NW;
        case WL_NE:
            return BIT_NE;
        case WL_NES:
            return BIT_NES;
        case WL_ESW:
            return BIT_ESW;
        case WL_NSW:
            return BIT_NSW;
        case WL_NEW:
            return BIT_NEW;
        case WL_NESW:
            return BIT_NESW;
        }
    return 0;
}

Level::Level()
{
    nr=0;
    refht=RX_I(0);
    delht=RX_I(0);
    u_rfht=0;
    u_dlht=0;
}

void Level::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    refht = input->readNumber<RX>(FILELINE);
    delht = input->readNumber<RX>(FILELINE);
    int nicon = input->readInt(FILELINE);
    u_rfht = input->readInt(FILELINE);
    u_dlht = input->readInt(FILELINE);
    name = input->readString(FILELINE);
    icons.resize(nicon);
    for(int i=0;i<nicon;i++)
        icons[i].read(input);
}

QString Level::write()
{
    QString string;
    string += QString("%1 %2 %3 %4 %5 %6 %7").arg(nr).arg(RX_C(refht)).arg(RX_C(delht)).arg(icons.size())
            .arg(u_rfht).arg(u_dlht).arg(name);
    for(int i=0;i<icons.size();i++)
        string += '\n' + icons[i].write();
    return string;
}

// Section 4: Day Schedules

DaySchedule::DaySchedule()
{
    nr=0;
    shape=0;
    utyp=0;
    ucnv=0;
}

void DaySchedule::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    int npts = input->readInt(FILELINE);
    shape = input->readInt(FILELINE);
    utyp = input->readInt(FILELINE);
    ucnv = input->readInt(FILELINE);
    name = input->readString(FILELINE);
    desc = input->readLine(FILELINE);
    time.resize(npts);
    ctrl.resize(npts);
    for(int i=0;i<npts;i++)
    {
        time[i] = input->readString(FILELINE);
        ctrl[i] = input->readNumber<RX>(FILELINE);
    }
}

QString DaySchedule::write()
{
    QString string;
    int npts = qMin(time.size(),ctrl.size());
    string += QString("%1 %2 %3 %4 %5 %6\n").arg(nr).arg(npts).arg(shape).arg(utyp).arg(ucnv).arg(name);
    string += desc;
    for(int i=0;i<npts;i++)
        string += QString("\n%1 %2").arg(time[i]).arg(RX_C(ctrl[i]));
    return string;
}

// Section 5: Week Schedules

WeekSchedule::WeekSchedule()
{
    nr=0;
    utyp=0;
    ucnv=0;
}

void WeekSchedule::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    utyp = input->readInt(FILELINE);
    ucnv = input->readInt(FILELINE);
    name = input->readString(FILELINE);
    desc = input->readLine(FILELINE);
    for(int i=0;i<12;i++)
        j[i] = input->readInt(FILELINE);
}

QString WeekSchedule::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(utyp).arg(ucnv).arg(name);
    string += desc + '\n';
    for(int i=0;i<12;i++)
        string += QString(" %1").arg(j[i]);
    return string;
}

// Section 6: Wind Pressure Profiles

WindPressureProfile::WindPressureProfile()
{
    nr=0;
    type=0;
}

void WindPressureProfile::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    int npts = input->readInt(FILELINE);
    type = input->readInt(FILELINE);
    name = input->readString(FILELINE);
    desc = input->readLine(FILELINE);
    azm.resize(npts);
    coef.resize(npts);
    for(int i=0;i<npts;i++)
    {
        azm[i] = input->readNumber<RX>(FILELINE);
        coef[i] = input->readNumber<RX>(FILELINE);
    }
}

QString WindPressureProfile::write()
{
    QString string;
    int npts = qMin(azm.size(),coef.size());
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(npts).arg(type).arg(name);
    string += desc;
    for(int i=0;i<npts;i++)
        string += QString("\n%1 %2").arg(RX_C(azm[i])).arg(RX_C(coef[i]));
    return string;
}

// Section 8a: Filter Elements
/*
FilterElement* FilterElement::readElement(Reader *input)
{
    FilterElement *out=NULL;
    int nr = input->readInt(FILELINE);
    QString ftype = input->readString(FILELINE);
    RX area = input->readNumber<RX>(FILELINE);
    RX depth = input->readNumber<RX>(FILELINE);
    RX dens = input->readNumber<RX>(FILELINE);
    int ual = input->readInt(FILELINE);
    int ud = input->readInt(FILELINE);
    QString name = input->readString(FILELINE);
    QString desc = input->readLine(FILELINE);
    int itype = -1;

    QString tags[4] = {QString("cef"),QString("pf0"),QString("gf0"),QString("spf")};
    FilterElement::Type types[4] = {FilterElement::FLE_CEF,FilterElement::FLE_PF0,FilterElement::FLE_GF0,FilterElement::FLE_SPF};

    for(int i=0;i<4;i++)
    {
        if(ftype==tags[i])
        {
            itype = i;
            break;
        }
    }
    if(itype == -1)
        error(QString("Unknown FilterElement type '%1' at line %2").arg(ftype).arg(input->lineNumber()) CFILELINE);

    switch(types[itype])
    {
    case FilterElement::FLE_CEF:
    {
        Fle_Cef *obj = new Fle_Cef(nr,ftype,area,depth,dens,ual,ud,name,desc);
        obj->readDetails(input);
        out = (FilterElement*)obj;
        break;
    }
    case FilterElement::FLE_PF0:
    {
        Fle_Pf0 *obj = new Fle_Pf0(nr,ftype,area,depth,dens,ual,ud,name,desc);
        obj->readDetails(input);
        out = (FilterElement*)obj;
        break;
    }
    case FilterElement::FLE_GF0:
    {
        Fle_Gf0 *obj = new Fle_Gf0(nr,ftype,area,depth,dens,ual,ud,name,desc);
        obj->readDetails(input);
        out = (FilterElement*)obj;
        break;
    }
    case FilterElement::FLE_SPF:
    {
        Fle_Spf *obj = new Fle_Spf(nr,ftype,area,depth,dens,ual,ud,name,desc);
        obj->readDetails(input);
        out = (FilterElement*)obj;
        break;
    }
    }
    return out;
}

Fle_Cef::Fle_Cef(int nr,QString ftype,RX area,RX depth,RX dens,int ual,int ud,QString name,QString desc)
{

    this->nr = nr;
    this->ftype = ftype;
    this->area = area;
    this->depth = depth;
    this->dens = dens;
    this->ual = ual;
    this->ud = ud;
    this->name = name;
    this->desc = desc;
    type = FilterElement::FLE_CEF;
}

void Fle_Cef::readDetails(Reader *input)
{
    int nspcs = input->readInt(FILELINE);
    spcs.resize(nspcs);
    eff.resize(nspcs);
    for(int i=0;i<nspcs;i++)
    {
        spcs[i] = input->readString(FILELINE);
        eff[i] = input->readNumber<RX>(FILELINE);
    }
}

QString Fle_Cef::write()
{
    QString string;
    int nspcs = qMin(spcs.size(),eff.size());
    string += QString("%1 %2 %3 %4 %5 %6 %7 %8\n").arg(nr).arg(ftype).arg(RX_C(area)).arg(RX_C(depth)).arg(RX_C(dens)).arg(ual).arg(ud).arg(name);
    string += desc;
    string += QString("\n%1").arg(nspcs);
    for(int i=0;i<nspcs;i++)
        string += QString("\n%1 %2").arg(spcs[i]).arg(RX_C(eff[i]));
    return string;
}

Fle_Pf0::Fle_Pf0(int nr,QString ftype,RX area,RX depth,RX dens,int ual,int ud,QString name,QString desc)
{

    this->nr = nr;
    this->ftype = ftype;
    this->area = area;
    this->depth = depth;
    this->dens = dens;
    this->ual = ual;
    this->ud = ud;
    this->name = name;
    this->desc = desc;
    usz=0;
    type = FilterElement::FLE_PF0;
}

void Fle_Pf0::readDetails(Reader *input)
{
    int npts = input->readInt(FILELINE);
    usz = input->readInt(FILELINE);
    size.resize(npts);
    eff.resize(npts);
    for(int i=0;i<npts;i++)
    {
        size[i] = input->readNumber<RX>(FILELINE);
        eff[i] = input->readNumber<RX>(FILELINE);
    }
}

QString Fle_Pf0::write()
{
    QString string;
    int npts = qMin(size.size(),eff.size());
    string += QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(nr).arg(ftype).arg(RX_C(area)).arg(RX_C(depth)).arg(RX_C(dens)).arg(ual).arg(ud).arg(name);
    string += desc;
    string += QString("\n%1 %2").arg(npts).arg(usz);
    for(int i=0;i<npts;i++)
        string += QString("\n%1 %2").arg(RX_C(size[i])).arg(RX_C(eff[i]));
    return string;
}

Fle_Gf0::Fle_Gf0(int nr,QString ftype,RX area,RX depth,RX dens,int ual,int ud,QString name,QString desc)
{

    this->nr = nr;
    this->ftype = ftype;
    this->area = area;
    this->depth = depth;
    this->dens = dens;
    this->ual = ual;
    this->ud = ud;
    this->name = name;
    this->desc = desc;
    type = FilterElement::FLE_GF0;
}

void Fle_Gf0::readDetails(Reader *input)
{
    int nspcs = input->readInt(FILELINE);
    spcs.resize(nspcs);
    bthru.resize(nspcs);
    load.resize(nspcs);
    eff.resize(nspcs);
    for(int i=0;i<nspcs;i++)
    {
        spcs[i] = input->readString(FILELINE);
        bthru[i] = input->readNumber<RX>(FILELINE);
        int npts = input->readInt(FILELINE);
        load[i].resize(npts);
        eff[i].resize(npts);
        for(int j=0;j<npts;j++)
        {
            load[i][j] = input->readNumber<RX>(FILELINE);
            eff[i][j] = input->readNumber<RX>(FILELINE);
        }
    }
}

QString Fle_Gf0::write()
{
    QString string;
    int nspcs = qMin(spcs.size(),
                     qMin(bthru.size(),
                          qMin(load.size(),eff.size())));
    string += QString("%1 %2 %3 %4 %5 %6 %7 %8\n").arg(nr).arg(ftype).arg(RX_C(area)).arg(RX_C(depth)).arg(RX_C(dens)).arg(ual).arg(ud).arg(name);
    string += desc;
    string += QString("\n%1").arg(nspcs);
    for(int i=0;i<nspcs;i++)
    {
        int npts = qMin(load[i].size(),eff[i].size());
        string += QString("\n%1 %2 %3").arg(spcs[i]).arg(RX_C(bthru[i])).arg(npts);
        for(int j=0;j<npts;j++)
            string += QString("\n%1 %2").arg(RX_C(load[i][j])).arg(RX_C(eff[i][j]));
    }
    return string;
}

Fle_Spf::Fle_Spf(int nr,QString ftype,RX area,RX depth,RX dens,int ual,int ud,QString name,QString desc)
{

    this->nr = nr;
    this->ftype = ftype;
    this->area = area;
    this->depth = depth;
    this->dens = dens;
    this->ual = ual;
    this->ud = ud;
    this->name = name;
    this->desc = desc;
    type = FilterElement::FLE_SPF;
}

void Fle_Spf::readDetails(Reader *input)
{
    int nselmt = input->readInt(FILELINE);
    elmt.resize(nselmt);
    for(int i=0;i<nselmt;i++)
        elmt[i] = input->readInt(FILELINE);
}

QString Fle_Spf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(nr).arg(ftype).arg(RX_C(area)).arg(RX_C(depth)).arg(RX_C(dens)).arg(ual).arg(ud).arg(name);
    string += QString("\n%1\n%2\n").arg(desc).arg(elmt.size());
    for(int i=0;i<elmt.size();i++)
        string += QString("%1 ").arg(elmt[i]);
    return string;
}
*/
// Section 8b: Filters

Filter::Filter()
{
    nr=0;
    fe=0;
    //nsub=0;
    //load=RX_I(0);
    //nsload=0;
    //spcs=0;
    //load=RX_I(0);
}

void Filter::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    fe = input->readInt(FILELINE);
    int nsub = input->readInt(FILELINE);
    load.resize(nsub);
    for(int i=0;i<nsub;i++)
    {
        load[i] = input->readNumber<RX>(FILELINE);
        input->readInt(FILELINE);  // dump nsload
    }
    //spcs = new int[];
    //for(int i=0;i<;i++)
    //    spcs[i] = input->readInt(__FILE__,__LINE__);
    //load = input->readNumber<RX>(__FILE__,__LINE__);
}

QString Filter::write()
{
    int nsload = 0;
    QString string;
    string += QString("%1 %2 %3").arg(nr).arg(fe).arg(load.size());
    for(int i=0;i<load.size();i++)
        string += QString("\n%1 %2").arg(RX_C(load[i])).arg(nsload);
    //string += QString("\n%1 %2").arg(spcs).arg(RX_C(load));
    return string;
}

// Section 9: Source/Sink Elements

SourceSinkElement* SourceSinkElement::readElement(Reader *input)
{
    SourceSinkElement *out=NULL;
    int nr = input->readInt(FILELINE);
    QString spcs = input->readString(FILELINE);
    QString ctype = input->readString(FILELINE);
    QString name = input->readString(FILELINE);
    QString desc = input->readLine(FILELINE);
    SourceSinkElement::Type type = convertType(ctype);
    switch(type)
    {
    case SourceSinkElement::CSE_CCF:
    {
        Cse_Ccf *obj = new Cse_Ccf(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_PRS:
    {
        Cse_Prs *obj = new Cse_Prs(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_CUT:
    {
        Cse_Cut *obj = new Cse_Cut(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_EDS:
    {
        Cse_Eds *obj = new Cse_Eds(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_BLS:
    {
        Cse_Bls *obj = new Cse_Bls(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_BRS:
    {
        Cse_Brs *obj = new Cse_Brs(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_DVS:
    {
        Cse_Dvs *obj = new Cse_Dvs(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_DRS:
    {
        Cse_Drs *obj = new Cse_Drs(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_DVR:
    {
        Cse_Dvr *obj = new Cse_Dvr(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_PLM:
    {
        Cse_Plm *obj = new Cse_Plm(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_PKM:
    {
        Cse_Pkm *obj = new Cse_Pkm(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::CSE_SUP:
    {
        Cse_Sup *obj = new Cse_Sup(nr,spcs,ctype,name,desc);
        obj->readDetails(input);
        out = (SourceSinkElement*)obj;
        break;
    }
    case SourceSinkElement::UNKNOWN:
    default:
        error(QString("Unknown SourceSinkElement type '%1' at line %2").arg(ctype).arg(input->lineNumber()) CFILELINE);
    }
    return out;
}

SourceSinkElement::Type SourceSinkElement::convertType(QString string)
{
    QString tags[12] = {QString("ccf"),QString("prs"),QString("cut"),QString("eds"),
                        QString("bls"),QString("brs"),QString("dvs"),QString("drs"),
                        QString("dvr"),QString("plm"),QString("pkm"),QString("sup")};
    SourceSinkElement::Type type[12] = {SourceSinkElement::CSE_CCF,SourceSinkElement::CSE_PRS,
                                        SourceSinkElement::CSE_CUT,SourceSinkElement::CSE_EDS,
                                        SourceSinkElement::CSE_BLS,SourceSinkElement::CSE_BRS,
                                        SourceSinkElement::CSE_DVS,SourceSinkElement::CSE_DRS,
                                        SourceSinkElement::CSE_DVR,SourceSinkElement::CSE_PLM,
                                        SourceSinkElement::CSE_PKM,SourceSinkElement::CSE_SUP};
    for(int i=0;i<12;i++)
        if(string==tags[i])
            return type[i];
    return SourceSinkElement::UNKNOWN;
}

Cse_Ccf::Cse_Ccf(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    G=RX_I(0);
    D=RX_I(0);
    u_G=0;
    u_D=0;
    type = SourceSinkElement::CSE_CCF;
}

void Cse_Ccf::readDetails(Reader *input)
{
    G = input->readNumber<RX>(FILELINE);
    D = input->readNumber<RX>(FILELINE);
    u_G = input->readInt(FILELINE);
    u_D = input->readInt(FILELINE);
}

QString Cse_Ccf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4").arg(RX_C(G)).arg(RX_C(D)).arg(u_G).arg(u_D);
    return string;
}

Cse_Prs::Cse_Prs(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    G=RX_I(0);
    x=RX_I(0);
    u_G=0;
    type = SourceSinkElement::CSE_PRS;
}

void Cse_Prs::readDetails(Reader *input)
{
    G = input->readNumber<RX>(FILELINE);
    x = input->readNumber<RX>(FILELINE);
    u_G = input->readInt(FILELINE);
}

QString Cse_Prs::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3").arg(RX_C(G)).arg(RX_C(x)).arg(u_G);
    return string;
}

Cse_Cut::Cse_Cut(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    G=RX_I(0);
    Co=RX_I(0);
    u_G=0;
    u_C=0;
    type = SourceSinkElement::CSE_CUT;
}

void Cse_Cut::readDetails(Reader *input)
{
    G = input->readNumber<RX>(FILELINE);
    Co = input->readNumber<RX>(FILELINE);
    u_G = input->readInt(FILELINE);
    u_C = input->readInt(FILELINE);
}

QString Cse_Cut::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4").arg(RX_C(G)).arg(RX_C(Co)).arg(u_G).arg(u_C);
    return string;
}

Cse_Eds::Cse_Eds(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    G0=RX_I(0);
    k=RX_I(0);
    u_G=0;
    u_k=0;
    type = SourceSinkElement::CSE_EDS;
}

void Cse_Eds::readDetails(Reader *input)
{
    G0 = input->readNumber<RX>(FILELINE);
    k = input->readNumber<RX>(FILELINE);
    u_G = input->readInt(FILELINE);
    u_k = input->readInt(FILELINE);
}

QString Cse_Eds::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4").arg(RX_C(G0)).arg(RX_C(k)).arg(u_G).arg(u_k);
    return string;
}

Cse_Bls::Cse_Bls(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    hm=RX_I(0);
    rho=RX_I(0);
    Kp=RX_I(0);
    M=RX_I(0);
    u_h=0;
    u_r=0;
    u_M=0;
    type = SourceSinkElement::CSE_BLS;
}

void Cse_Bls::readDetails(Reader *input)
{
    hm = input->readNumber<RX>(FILELINE);
    rho = input->readNumber<RX>(FILELINE);
    Kp = input->readNumber<RX>(FILELINE);
    M = input->readNumber<RX>(FILELINE);
    u_h = input->readInt(FILELINE);
    u_r = input->readInt(FILELINE);
    u_M = input->readInt(FILELINE);
}

QString Cse_Bls::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(RX_C(hm)).arg(RX_C(rho)).arg(RX_C(Kp)).arg(RX_C(M)).arg(u_h).arg(u_r).arg(u_M);
    return string;
}

Cse_Brs::Cse_Brs(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    M=RX_I(0);
    u_M=0;
    type = SourceSinkElement::CSE_BRS;
}

void Cse_Brs::readDetails(Reader *input)
{
    M = input->readNumber<RX>(FILELINE);
    u_M = input->readInt(FILELINE);
}

QString Cse_Brs::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2").arg(RX_C(M)).arg(u_M);
    return string;
}

Cse_Dvs::Cse_Dvs(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    dV=RX_I(0);
    dA=RX_I(0);
    u_V=0;
    u_A=0;
    type = SourceSinkElement::CSE_DVS;
}

void Cse_Dvs::readDetails(Reader *input)
{
    dV = input->readNumber<RX>(FILELINE);
    dA = input->readNumber<RX>(FILELINE);
    u_V = input->readInt(FILELINE);
    u_A = input->readInt(FILELINE);
}

QString Cse_Dvs::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4").arg(RX_C(dV)).arg(RX_C(dA)).arg(u_V).arg(u_A);
    return string;
}

Cse_Drs::Cse_Drs(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    kd=RX_I(0);
    u_k=0;
    type = SourceSinkElement::CSE_DRS;
}

void Cse_Drs::readDetails(Reader *input)
{
    kd = input->readNumber<RX>(FILELINE);
    u_k = input->readInt(FILELINE);
}

QString Cse_Drs::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2").arg(RX_C(kd)).arg(u_k);
    return string;
}

Cse_Dvr::Cse_Dvr(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    dV=RX_I(0);
    R=RX_I(0);
    dA=RX_I(0);
    rA=RX_I(0);
    u_V=0;
    u_R=0;
    u_dA=0;
    u_dR=0;
    type = SourceSinkElement::CSE_DVR;
}

void Cse_Dvr::readDetails(Reader *input)
{
    dV = input->readNumber<RX>(FILELINE);
    R = input->readNumber<RX>(FILELINE);
    dA = input->readNumber<RX>(FILELINE);
    rA = input->readNumber<RX>(FILELINE);
    u_V = input->readInt(FILELINE);
    u_R = input->readInt(FILELINE);
    u_dA = input->readInt(FILELINE);
    u_dR = input->readInt(FILELINE);
}

QString Cse_Dvr::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8").arg(RX_C(dV)).arg(RX_C(R)).arg(RX_C(dA)).arg(RX_C(rA)).arg(u_V).arg(u_R).arg(u_dA).arg(u_dR);
    return string;
}

Cse_Plm::Cse_Plm(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    a=RX_I(0);
    b=RX_I(0);
    tp=RX_I(0);
    u_a=0;
    u_tp=0;
    type = SourceSinkElement::CSE_PLM;
}

void Cse_Plm::readDetails(Reader *input)
{
    a = input->readNumber<RX>(FILELINE);
    b = input->readNumber<RX>(FILELINE);
    tp = input->readNumber<RX>(FILELINE);
    u_a = input->readInt(FILELINE);
    u_tp = input->readInt(FILELINE);
}

QString Cse_Plm::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4 %5").arg(RX_C(a)).arg(RX_C(b)).arg(RX_C(tp)).arg(u_a).arg(u_tp);
    return string;
}

Cse_Pkm::Cse_Pkm(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    a=RX_I(0);
    b=RX_I(0);
    tp=RX_I(0);
    u_a=0;
    u_tp=0;
    type = SourceSinkElement::CSE_PKM;
}

void Cse_Pkm::readDetails(Reader *input)
{
    a = input->readNumber<RX>(FILELINE);
    b = input->readNumber<RX>(FILELINE);
    tp = input->readNumber<RX>(FILELINE);
    u_a = input->readInt(FILELINE);
    u_tp = input->readInt(FILELINE);
}

QString Cse_Pkm::write()
{
    QString string;
    string += QString("%1 %2 %3 %4").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += '\n' + desc;
    string += QString("\n%1 %2 %3 %4 %5").arg(RX_C(a)).arg(RX_C(b)).arg(RX_C(tp)).arg(u_a).arg(u_tp);
    return string;
}

Cse_Sup::Cse_Sup(int nr,QString spcs,QString ctype,QString name,QString desc)
{

    this->nr = nr;
    this->spcs = spcs;
    this->ctype = ctype;
    this->name = name;
    this->desc = desc;
    type = SourceSinkElement::CSE_SUP;
}

void Cse_Sup::readDetails(Reader *input)
{
    int n = input->readInt(FILELINE);
    elmt.resize(n);
    for(int i=0;i<n;i++)
        elmt[i] = input->readInt(FILELINE);
}

QString Cse_Sup::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(spcs).arg(ctype).arg(name);
    string += desc;
    string += QString("\n%1\n").arg(elmt.size());
    for(int i=0;i<elmt.size();i++)
        string += QString("%1 ").arg(elmt[i]);
    return string;
}

// Section 10: Airflow Elements

AirflowElement *AirflowElement::readElement(Reader *input)
{
    int nr = input->readInt(FILELINE);
    int icon = input->readInt(FILELINE);
    QString dtype = input->readString(FILELINE);
    QString name = input->readString(FILELINE);
    QString desc = input->readLine(FILELINE);
    AirflowElement::Type type = AirflowElement::convertType(dtype);
    AirflowElement *out = NULL;
    switch(type)
    {
    case AirflowElement::PLR_ORF:
    {
        Plr_Orf *obj = new Plr_Orf(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_LEAK:
    {
        Plr_Leak *obj = new Plr_Leak(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_CONN:
    {
        Plr_Conn *obj = new Plr_Conn(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_QCN:
    {
        Plr_Qcn *obj = new Plr_Qcn(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_FCN:
    {
        Plr_Fcn *obj = new Plr_Fcn(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_TEST1:
    {
        Plr_Test1 *obj = new Plr_Test1(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_TEST2:
    {
        Plr_Test2 *obj = new Plr_Test2(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_CRACK:
    {
        Plr_Crack *obj = new Plr_Crack(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_STAIR:
    {
        Plr_Stair *obj = new Plr_Stair(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_SHAFT:
    {
        Plr_Shaft *obj = new Plr_Shaft(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_BDQ:
    {
        Plr_Bdq *obj = new Plr_Bdq(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::PLR_BDF:
    {
        Plr_Bdf *obj = new Plr_Bdf(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::QFR_QAB:
    {
        Qfr_Qab *obj = new Qfr_Qab(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::QFR_FAB:
    {
        Qfr_Fab *obj = new Qfr_Fab(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::QFR_CRACK:
    {
        Qfr_Crack *obj = new Qfr_Crack(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::QFR_TEST2:
    {
        Qfr_Test2 *obj = new Qfr_Test2(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::AFE_DOR:
    {
        Afe_Dor *obj = new Afe_Dor(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::DR_PL2:
    {
        Dr_Pl2 *obj = new Dr_Pl2(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::AFE_CMF:
    {
        Afe_Cmf *obj = new Afe_Cmf(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::AFE_CVF:
    {
        Afe_Cvf *obj = new Afe_Cvf(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::AFE_FAN:
    {
        Afe_Fan *obj = new Afe_Fan(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::AFE_CSF:
    {
        Afe_Csf *obj = new Afe_Csf(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::AFE_SUP:
    {
        Afe_Sup *obj = new Afe_Sup(nr,icon,dtype,name,desc);
        obj->readDetails(input);
        out = (AirflowElement*)obj;
        break;
    }
    case AirflowElement::UNKNOWN:
    default:
        error(QString("Unknown AirflowElement type '%1' at line %2").arg(dtype)
              .arg(input->lineNumber()) CFILELINE);
    }
    return out;
}

AirflowElement::Type AirflowElement::convertType(QString string)
{
    QString tags[28] = {QString("plr_orfc"),QString("plr_leak1"),QString("plr_leak2"),QString("plr_leak3"),
                        QString("plr_conn"),QString("plr_qcn"),QString("plr_fcn"),QString("plr_test1"),
                        QString("plr_test2"),QString("plr_crack"),QString("plr_stair"),QString("plr_shaft"),
                        QString("plr_bdq"),QString("plr_bdf"),QString("qfr_qab"),QString("qfr_fab"),
                        QString("qfr_crack"),QString("qfr_test2"),QString("dor_door"),QString("dor_pl2"),
                        QString("fan_cmf"),QString("fan_cvf"),QString("fan_fan"),QString("csf_fsp"),
                        QString("csf_qsp"),QString("csf_psf"),QString("csf_psq"),QString("sup_afe")};
    AirflowElement::Type type[28] = {AirflowElement::PLR_ORF,AirflowElement::PLR_LEAK,AirflowElement::PLR_LEAK,
                                     AirflowElement::PLR_LEAK,AirflowElement::PLR_CONN,AirflowElement::PLR_QCN,
                                     AirflowElement::PLR_FCN,AirflowElement::PLR_TEST1,AirflowElement::PLR_TEST2,
                                     AirflowElement::PLR_CRACK,AirflowElement::PLR_STAIR,AirflowElement::PLR_SHAFT,
                                     AirflowElement::PLR_BDQ,AirflowElement::PLR_BDF,AirflowElement::QFR_QAB,
                                     AirflowElement::QFR_FAB,AirflowElement::QFR_CRACK,AirflowElement::QFR_TEST2,
                                     AirflowElement::AFE_DOR,AirflowElement::DR_PL2,AirflowElement::AFE_CMF,
                                     AirflowElement::AFE_CVF,AirflowElement::AFE_FAN,AirflowElement::AFE_CSF,
                                     AirflowElement::AFE_CSF,AirflowElement::AFE_CSF,AirflowElement::AFE_CSF,
                                     AirflowElement::AFE_SUP};
    for(int i=0;i<28;i++)
        if(string==tags[i])
            return type[i];
    return AirflowElement::UNKNOWN;
}

Plr_Conn::Plr_Conn(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    area=RX_I(0);
    coef=RX_I(0);
    u_A=0;
    type = AirflowElement::PLR_CONN;
}

void Plr_Conn::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    area = input->readNumber<RX>(FILELINE);
    coef = input->readNumber<RX>(FILELINE);
    u_A = input->readInt(FILELINE);
}

QString Plr_Conn::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6\n").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(area)).arg(RX_C(coef)).arg(u_A);
    return string;
}

Qfr_Qab::Qfr_Qab(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    a=RX_I(0);
    b=RX_I(0);
    type = AirflowElement::QFR_QAB;
}

void Qfr_Qab::readDetails(Reader *input)
{
    a = input->readNumber<RX>(FILELINE);
    b = input->readNumber<RX>(FILELINE);
}

QString Qfr_Qab::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2").arg(RX_C(a)).arg(RX_C(b));
    return string;
}

Plr_Bdf::Plr_Bdf(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    Cp=RX_I(0);
    xp=RX_I(0);
    Cn=RX_I(0);
    xn=RX_I(0);
    type = AirflowElement::PLR_BDF;
}

void Plr_Bdf::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    Cp = input->readNumber<RX>(FILELINE);
    xp = input->readNumber<RX>(FILELINE);
    Cn = input->readNumber<RX>(FILELINE);
    xn = input->readNumber<RX>(FILELINE);
}

QString Plr_Bdf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5").arg(RX_C(lam)).arg(RX_C(Cp)).arg(RX_C(xp)).arg(RX_C(Cn)).arg(RX_C(xn));
    return string;
}

Plr_Leak::Plr_Leak(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    coef=RX_I(0);
    pres=RX_I(0);
    area1=RX_I(0);
    area2=RX_I(0);
    area3=RX_I(0);
    u_A1=0;
    u_A2=0;
    u_A3=0;
    u_dP=0;
    type = AirflowElement::PLR_LEAK;
}

void Plr_Leak::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    coef = input->readNumber<RX>(FILELINE);
    pres = input->readNumber<RX>(FILELINE);
    area1 = input->readNumber<RX>(FILELINE);
    area2 = input->readNumber<RX>(FILELINE);
    area3 = input->readNumber<RX>(FILELINE);
    u_A1 = input->readInt(FILELINE);
    u_A2 = input->readInt(FILELINE);
    u_A3 = input->readInt(FILELINE);
    u_dP = input->readInt(FILELINE);
}

QString Plr_Leak::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(coef)).arg(RX_C(pres)).arg(RX_C(area1)).arg(RX_C(area2)).arg(RX_C(area3)).arg(u_A1).arg(u_A2).arg(u_A3).arg(u_dP);
    return string;
}

Afe_Dor::Afe_Dor(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    dTmin=RX_I(0);
    ht=RX_I(0);
    wd=RX_I(0);
    cd=RX_I(0);
    u_T=0;
    u_H=0;
    u_W=0;
    type = AirflowElement::AFE_DOR;
}

void Afe_Dor::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    dTmin = input->readNumber<RX>(FILELINE);
    ht = input->readNumber<RX>(FILELINE);
    wd = input->readNumber<RX>(FILELINE);
    cd = input->readNumber<RX>(FILELINE);
    u_T = input->readInt(FILELINE);
    u_H = input->readInt(FILELINE);
    u_W = input->readInt(FILELINE);
}

QString Afe_Dor::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9 %10").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(dTmin)).arg(RX_C(ht)).arg(RX_C(wd)).arg(RX_C(cd)).arg(u_T).arg(u_H).arg(u_W);
    return string;
}

Afe_Sup::Afe_Sup(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    nse=0;
    sched=0;
    u_H=0;
    nr=0;
    relHt=RX_I(0);
    filt=0;
    type = AirflowElement::AFE_SUP;
}

void Afe_Sup::readDetails(Reader *input)
{
    nse = input->readInt(FILELINE);
    sched = input->readInt(FILELINE);
    u_H = input->readInt(FILELINE);
    nr = input->readInt(FILELINE);
    relHt = input->readNumber<RX>(FILELINE);
    filt = input->readInt(FILELINE);
}

QString Afe_Sup::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3\n").arg(nse).arg(sched).arg(u_H);
    string += QString("%1 %2 %3").arg(nr).arg(RX_C(relHt)).arg(filt);
    return string;
}

Afe_Fan::Afe_Fan(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    rdens=RX_I(0);
    fdf=RX_I(0);
    sop=RX_I(0);
    off=RX_I(0);
    for(int i=0;i<4;i++)
        fpc[i]=RX_I(0);
    // npts=0;
    Sarea=RX_I(0);
    u_Sa=0;
    type = AirflowElement::AFE_FAN;
}

void Afe_Fan::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    rdens = input->readNumber<RX>(FILELINE);
    fdf = input->readNumber<RX>(FILELINE);
    sop = input->readNumber<RX>(FILELINE);
    off = input->readNumber<RX>(FILELINE);
    for(int i=0;i<4;i++)
        fpc[i] = input->readNumber<RX>(FILELINE);
    int npts = input->readInt(FILELINE);
    Sarea = input->readNumber<RX>(FILELINE);
    u_Sa = input->readInt(FILELINE);
    mF.resize(npts);
    u_mF.resize(npts);
    dP.resize(npts);
    u_dP.resize(npts);
    rP.resize(npts);
    u_rP.resize(npts);
    for(int i=0;i<npts;i++)
    {
        mF[i] = input->readNumber<RX>(FILELINE);
        u_mF[i] = input->readInt(FILELINE);
        dP[i] = input->readNumber<RX>(FILELINE);
        u_dP[i] = input->readInt(FILELINE);
        rP[i] = input->readNumber<RX>(FILELINE);
        u_rP[i] = input->readInt(FILELINE);
    }
}

QString Afe_Fan::write()
{
    QString string;
    // Should check if these are all the same... later
    int n = qMin(mF.size(),
                 qMin(u_mF.size(),
                      qMin(dP.size(),
                           qMin(u_dP.size(),
                                qMin(rP.size(),u_rP.size())))));
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(rdens)).arg(RX_C(fdf)).arg(RX_C(sop)).arg(RX_C(off));
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(RX_C(fpc[0])).arg(RX_C(fpc[1])).arg(RX_C(fpc[2])).arg(RX_C(fpc[3])).arg(n).arg(RX_C(Sarea)).arg(u_Sa);
    for(int i=0;i<n;i++)
        string += QString("\n%1 %2 %3 %4 %5 %6").arg(RX_C(mF[i])).arg(u_mF[i]).arg(RX_C(dP[i])).arg(u_dP[i]).arg(RX_C(rP[i])).arg(u_rP[i]);
    return string;
}

Plr_Bdq::Plr_Bdq(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    Cp=RX_I(0);
    xp=RX_I(0);
    Cn=RX_I(0);
    xn=RX_I(0);
    type = AirflowElement::PLR_BDQ;
}

void Plr_Bdq::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    Cp = input->readNumber<RX>(FILELINE);
    xp = input->readNumber<RX>(FILELINE);
    Cn = input->readNumber<RX>(FILELINE);
    xn = input->readNumber<RX>(FILELINE);
}

QString Plr_Bdq::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5").arg(RX_C(lam)).arg(RX_C(Cp)).arg(RX_C(xp)).arg(RX_C(Cn)).arg(RX_C(xn));
    return string;
}

Afe_Cmf::Afe_Cmf(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    Flow=RX_I(0);
    u_F=0;
    type = AirflowElement::AFE_CMF;
}

void Afe_Cmf::readDetails(Reader *input)
{
    Flow = input->readNumber<RX>(FILELINE);
    u_F = input->readInt(FILELINE);
}

QString Afe_Cmf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2").arg(RX_C(Flow)).arg(u_F);
    return string;
}

Plr_Stair::Plr_Stair(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    Ht=RX_I(0);
    Area=RX_I(0);
    peo=RX_I(0);
    tread=0;
    u_A=0;
    u_D=0;
    type = AirflowElement::PLR_STAIR;
}

void Plr_Stair::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    Ht = input->readNumber<RX>(FILELINE);
    Area = input->readNumber<RX>(FILELINE);
    peo = input->readNumber<RX>(FILELINE);
    tread = input->readInt(FILELINE);
    u_A = input->readInt(FILELINE);
    u_D = input->readInt(FILELINE);
}

QString Plr_Stair::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(Ht)).arg(RX_C(Area)).arg(RX_C(peo)).arg(tread).arg(u_A).arg(u_D);
    return string;
}

Plr_Qcn::Plr_Qcn(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    type = AirflowElement::PLR_QCN;
}

void Plr_Qcn::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
}

QString Plr_Qcn::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt));
    return string;
}

Qfr_Fab::Qfr_Fab(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    a=RX_I(0);
    b=RX_I(0);
    type = AirflowElement::QFR_FAB;
}

void Qfr_Fab::readDetails(Reader *input)
{
    a = input->readNumber<RX>(FILELINE);
    b = input->readNumber<RX>(FILELINE);
}

QString Qfr_Fab::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2").arg(RX_C(a)).arg(RX_C(b));
    return string;
}

Plr_Shaft::Plr_Shaft(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    Ht=RX_I(0);
    area=RX_I(0);
    perim=RX_I(0);
    rough=RX_I(0);
    u_A=0;
    u_D=0;
    u_P=0;
    u_R=0;
    type = AirflowElement::PLR_SHAFT;
}

void Plr_Shaft::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    Ht = input->readNumber<RX>(FILELINE);
    area = input->readNumber<RX>(FILELINE);
    perim = input->readNumber<RX>(FILELINE);
    rough = input->readNumber<RX>(FILELINE);
    u_A = input->readInt(FILELINE);
    u_D = input->readInt(FILELINE);
    u_P = input->readInt(FILELINE);
    u_R = input->readInt(FILELINE);
}

QString Plr_Shaft::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(Ht)).arg(RX_C(area)).arg(RX_C(perim)).arg(RX_C(rough)).arg(u_A).arg(u_D).arg(u_P).arg(u_R);
    return string;
}

Qfr_Test2::Qfr_Test2(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    a=RX_I(0);
    b=RX_I(0);
    dP1=RX_I(0);
    F1=RX_I(0);
    dP2=RX_I(0);
    F2=RX_I(0);
    u_P1=0;
    u_F1=0;
    u_P2=0;
    u_F2=0;
    type = AirflowElement::QFR_TEST2;
}

void Qfr_Test2::readDetails(Reader *input)
{
    a = input->readNumber<RX>(FILELINE);
    b = input->readNumber<RX>(FILELINE);
    dP1 = input->readNumber<RX>(FILELINE);
    F1 = input->readNumber<RX>(FILELINE);
    dP2 = input->readNumber<RX>(FILELINE);
    F2 = input->readNumber<RX>(FILELINE);
    u_P1 = input->readInt(FILELINE);
    u_F1 = input->readInt(FILELINE);
    u_P2 = input->readInt(FILELINE);
    u_F2 = input->readInt(FILELINE);
}

QString Qfr_Test2::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9 %10").arg(RX_C(a)).arg(RX_C(b)).arg(RX_C(dP1)).arg(RX_C(F1)).arg(RX_C(dP2)).arg(RX_C(F2)).arg(u_P1).arg(u_F1).arg(u_P2).arg(u_F2);
    return string;
}

Plr_Crack::Plr_Crack(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    length=RX_I(0);
    width=RX_I(0);
    u_L=0;
    u_W=0;
    type = AirflowElement::PLR_CRACK;
}

void Plr_Crack::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    length = input->readNumber<RX>(FILELINE);
    width = input->readNumber<RX>(FILELINE);
    u_L = input->readInt(FILELINE);
    u_W = input->readInt(FILELINE);
}

QString Plr_Crack::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(length)).arg(RX_C(width)).arg(u_L).arg(u_W);
    return string;
}

Afe_Cvf::Afe_Cvf(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    Flow=RX_I(0);
    u_F=0;
    type = AirflowElement::AFE_CVF;
}

void Afe_Cvf::readDetails(Reader *input)
{
    Flow = input->readNumber<RX>(FILELINE);
    u_F = input->readInt(FILELINE);
}

QString Afe_Cvf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2").arg(RX_C(Flow)).arg(u_F);
    return string;
}

Plr_Test2::Plr_Test2(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    dP1=RX_I(0);
    F1=RX_I(0);
    dP2=RX_I(0);
    F2=RX_I(0);
    u_P1=0;
    u_F1=0;
    u_P2=0;
    u_F2=0;
    type = AirflowElement::PLR_TEST2;
}

void Plr_Test2::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    dP1 = input->readNumber<RX>(FILELINE);
    F1 = input->readNumber<RX>(FILELINE);
    dP2 = input->readNumber<RX>(FILELINE);
    F2 = input->readNumber<RX>(FILELINE);
    u_P1 = input->readInt(FILELINE);
    u_F1 = input->readInt(FILELINE);
    u_P2 = input->readInt(FILELINE);
    u_F2 = input->readInt(FILELINE);
}

QString Plr_Test2::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(dP1)).arg(RX_C(F1)).arg(RX_C(dP2)).arg(RX_C(F2)).arg(u_P1).arg(u_F1).arg(u_P2).arg(u_F2);
    return string;
}

Dr_Pl2::Dr_Pl2(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    dH=RX_I(0);
    ht=RX_I(0);
    wd=RX_I(0);
    cd=RX_I(0);
    u_H=0;
    u_W=0;
    type = AirflowElement::DR_PL2;
}

void Dr_Pl2::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    dH = input->readNumber<RX>(FILELINE);
    ht = input->readNumber<RX>(FILELINE);
    wd = input->readNumber<RX>(FILELINE);
    cd = input->readNumber<RX>(FILELINE);
    u_H = input->readInt(FILELINE);
    u_W = input->readInt(FILELINE);
}

QString Dr_Pl2::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(dH)).arg(RX_C(ht)).arg(RX_C(wd)).arg(RX_C(cd)).arg(u_H).arg(u_W);
    return string;
}

Plr_Test1::Plr_Test1(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    dP=RX_I(0);
    Flow=RX_I(0);
    u_P=0;
    u_F=0;
    type = AirflowElement::PLR_TEST1;
}

void Plr_Test1::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    dP = input->readNumber<RX>(FILELINE);
    Flow = input->readNumber<RX>(FILELINE);
    u_P = input->readInt(FILELINE);
    u_F = input->readInt(FILELINE);
}

QString Plr_Test1::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(dP)).arg(RX_C(Flow)).arg(u_P).arg(u_F);
    return string;
}

Qfr_Crack::Qfr_Crack(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    a=RX_I(0);
    b=RX_I(0);
    length=RX_I(0);
    width=RX_I(0);
    depth=RX_I(0);
    nB=0;
    u_L=0;
    u_W=0;
    u_D=0;
    type = AirflowElement::QFR_CRACK;
}

void Qfr_Crack::readDetails(Reader *input)
{
    a = input->readNumber<RX>(FILELINE);
    b = input->readNumber<RX>(FILELINE);
    length = input->readNumber<RX>(FILELINE);
    width = input->readNumber<RX>(FILELINE);
    depth = input->readNumber<RX>(FILELINE);
    nB = input->readInt(FILELINE);
    u_L = input->readInt(FILELINE);
    u_W = input->readInt(FILELINE);
    u_D = input->readInt(FILELINE);
}

QString Qfr_Crack::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9").arg(RX_C(a)).arg(RX_C(b)).arg(RX_C(length)).arg(RX_C(width)).arg(RX_C(depth)).arg(nB).arg(u_L).arg(u_W).arg(u_D);
    return string;
}

Afe_Csf::Afe_Csf(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    npts=0;
    u_x=0;
    u_y=0;
    x=RX_I(0);
    y=RX_I(0);
    type = AirflowElement::AFE_CSF;
}

void Afe_Csf::readDetails(Reader *input)
{
    npts = input->readInt(FILELINE);
    u_x = input->readInt(FILELINE);
    u_y = input->readInt(FILELINE);
    x = input->readNumber<RX>(FILELINE);
    y = input->readNumber<RX>(FILELINE);
}

QString Afe_Csf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3").arg(npts).arg(u_x).arg(u_y);
    string += QString("\n%1 %2").arg(RX_C(x)).arg(RX_C(y));
    return string;
}

Plr_Fcn::Plr_Fcn(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    type = AirflowElement::PLR_FCN;
}

void Plr_Fcn::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
}

QString Plr_Fcn::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt));
    return string;
}

Plr_Orf::Plr_Orf(int nr,int icon,QString dtype,QString name,QString desc)
{
    this->nr = nr;
    this->icon = icon;
    this->dtype = dtype;
    this->name = name;
    this->desc = desc;
    lam=RX_I(0);
    turb=RX_I(0);
    expt=RX_I(0);
    area=RX_I(0);
    dia=RX_I(0);
    coef=RX_I(0);
    Re=RX_I(0);
    u_A=0;
    u_D=0;
    type = AirflowElement::PLR_ORF;
}

void Plr_Orf::readDetails(Reader *input)
{
    lam = input->readNumber<RX>(FILELINE);
    turb = input->readNumber<RX>(FILELINE);
    expt = input->readNumber<RX>(FILELINE);
    area = input->readNumber<RX>(FILELINE);
    dia = input->readNumber<RX>(FILELINE);
    coef = input->readNumber<RX>(FILELINE);
    Re = input->readNumber<RX>(FILELINE);
    u_A = input->readInt(FILELINE);
    u_D = input->readInt(FILELINE);
}

QString Plr_Orf::write()
{
    QString string;
    string += QString("%1 %2 %3 %4\n").arg(nr).arg(icon).arg(dtype).arg(name);
    string += desc;
    string += QString("\n%1 %2 %3 %4 %5 %6 %7 %8 %9").arg(RX_C(lam)).arg(RX_C(turb)).arg(RX_C(expt)).arg(RX_C(area)).arg(RX_C(dia)).arg(RX_C(coef)).arg(RX_C(Re)).arg(u_A).arg(u_D);
    return string;
}

// Section 13: Simple Air Handling System (AHS)

Ahs::Ahs()
{
    nr=0;
    zone_r=0;
    zone_s=0;
    path_r=0;
    path_s=0;
    path_x=0;
}

void Ahs::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    zone_r = input->readInt(FILELINE);
    zone_s = input->readInt(FILELINE);
    path_r = input->readInt(FILELINE);
    path_s = input->readInt(FILELINE);
    path_x = input->readInt(FILELINE);
    name = input->readString(FILELINE);
    desc = input->readLine(FILELINE);
}

QString Ahs::write()
{
    QString string;
    string += QString("%1 %2 %3 %4 %5 %6 %7\n").arg(nr).arg(zone_r).arg(zone_s).arg(path_r).arg(path_s).arg(path_x).arg(name);
    string += desc;
    return string;
}

// Section 14: Zones

Zone::Zone()
{
    nr=0;
    flags=0;
    ps=0;
    pc=0;
    pk=0;
    pl=0;
    relHt=RX_I(0);
    Vol=RX_I(0);
    T0=RX_I(0);
    P0=RX_I(0);
    name=QString();
    color=0;
    u_Ht=0;
    u_V=0;
    u_T=0;
    u_P=0;
    cdaxis=0;
    cfd=0;
    cfdname=QString();
    X1=RX_I(0);
    Y1=RX_I(0);
    H1=RX_I(0);
    X2=RX_I(0);
    Y2=RX_I(0);
    H2=RX_I(0);
    celldx=RX_I(0);
    axialD=RX_I(0);
    u_aD=0;
    u_L=0;
}

void Zone::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    flags = input->readInt(FILELINE);
    ps = input->readInt(FILELINE);
    pc = input->readInt(FILELINE);
    pk = input->readInt(FILELINE);
    pl = input->readInt(FILELINE);
    relHt = input->readNumber<RX>(FILELINE);
    Vol = input->readNumber<RX>(FILELINE);
    T0 = input->readNumber<RX>(FILELINE);
    P0 = input->readNumber<RX>(FILELINE);
    name = input->readString(FILELINE);
    color = input->readInt(FILELINE);
    u_Ht = input->readInt(FILELINE);
    u_V = input->readInt(FILELINE);
    u_T = input->readInt(FILELINE);
    u_P = input->readInt(FILELINE);
    cdaxis = input->readInt(FILELINE);
    cfd = input->readInt(FILELINE);
    if(cfd)
        cfdname = input->readString(FILELINE);
    else if(cdaxis)
    {
        input->readString(FILELINE);  // Read "1D:"
        X1 = input->readNumber<RX>(FILELINE);
        Y1 = input->readNumber<RX>(FILELINE);
        H1 = input->readNumber<RX>(FILELINE);
        X2 = input->readNumber<RX>(FILELINE);
        Y2 = input->readNumber<RX>(FILELINE);
        H2 = input->readNumber<RX>(FILELINE);
        celldx = input->readNumber<RX>(FILELINE);
        axialD = input->readNumber<RX>(FILELINE);
        u_aD = input->readInt(FILELINE);
        u_L = input->readInt(FILELINE);
    }
}

QString Zone::write()
{
    QString string;
    string += QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17 %18").arg(nr)
            .arg(flags).arg(ps).arg(pc).arg(pk).arg(pl).arg(RX_C(relHt)).arg(RX_C(Vol)).arg(RX_C(T0)).arg(RX_C(P0)).arg(name)
            .arg(color).arg(u_Ht).arg(u_V).arg(u_T).arg(u_P).arg(cdaxis).arg(cfd);
    if(cfd)
        string += ' ' + cfdname;
    else if(cdaxis)
        string += QString(" 1D: %1 %2 %3 %4 %5 %6 %7 %8 %9 %10\n").arg(RX_C(X1))
                .arg(RX_C(Y1)).arg(RX_C(H1)).arg(RX_C(X2)).arg(RX_C(Y2)).arg(RX_C(H2)).arg(RX_C(celldx))
                .arg(RX_C(axialD)).arg(u_aD).arg(u_L);
    return string;
}

void Zone::setVariablePressure(bool b)
{
    if(b)
        flags |= VAR_P;
    else
        flags &= NVAR_P;
}

bool Zone::variablePressure() const
{
    return flags & VAR_P;
}

void Zone::setVariableContaminants(bool b)
{
    if(b)
        flags |= VAR_C;
    else
        flags &= NVAR_C;
}

bool Zone::variableContaminants() const
{
    return flags & VAR_C;
}

void Zone::setSystem(bool b)
{
    if(b)
        flags |= SYS_N;
    else
        flags &= NSYS_N;
}

bool Zone::system() const
{
    return flags & SYS_N;
}

/*
Path::Path(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,
           RX X,RX Y,RX relHt,RX mult,RX wPset,RX wPmod,RX wazm,RX Fahs,RX Xmax,RX Xmin,
           unsigned int icon,unsigned int dir,int u_Ht,int u_XY,int u_dP,int u_F,int cfd,
           QString name,int cfd_ptype,int cfd_btype,int cfd_capp)
{
    this->nr=nr;
    this->flags=flags;
    this->pzn=pzn;
    this->pzm=pzm;
    this->pe=pe;
    this->pf=pf;
    this->pw=pw;
    this->pa=pa;
    this->ps=ps;
    this->pc=pc;
    this->pld=pld;
    this->X=X;
    this->Y=Y;
    this->relHt=relHt;
    this->mult=mult;
    this->wPset=wPset;
    this->wPmod=wPmod;
    this->wazm=wazm;
    this->Fahs=Fahs;
    this->Xmax=Xmax;
    this->Xmin=Xmin;
    this->icon=icon;
    this->dir=dir;
    this->u_Ht=u_Ht;
    this->u_XY=u_XY;
    this->u_dP=u_dP;
    this->u_F=u_F;
    this->cfd=cfd;
    this->name=name;
    this->cfd_ptype=cfd_ptype;
    this->cfd_btype=cfd_btype;
    this->cfd_capp=cfd_capp;
}
*/

Path::Path()
{
    nr=0;
    flags=0;
    pzn=0;
    pzm=0;
    pe=0;
    pf=0;
    pw=0;
    pa=0;
    ps=0;
    pc=0;
    pld=0;
    X=RX_I(0);
    Y=RX_I(0);
    relHt=RX_I(0);
    mult=RX_I(1);
    wPset=RX_I(0);
    wPmod=RX_I(0);
    wazm=RX_I(0);
    Fahs=RX_I(0);
    Xmax=RX_I(0);
    Xmin=RX_I(0);
    icon=0;
    dir=0;
    u_Ht=0;
    u_XY=0;
    u_dP=0;
    u_F=0;
    cfd=0;
    name=QString();
    cfd_ptype=0;
    cfd_btype=0;
    cfd_capp=0;
    //switch(type)
}

void Path::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    flags = input->readInt(FILELINE);
    pzn = input->readInt(FILELINE);
    pzm = input->readInt(FILELINE);
    pe = input->readInt(FILELINE);
    pf = input->readInt(FILELINE);
    pw = input->readInt(FILELINE);
    pa = input->readInt(FILELINE);
    ps = input->readInt(FILELINE);
    pc = input->readInt(FILELINE);
    pld = input->readInt(FILELINE);
    X = input->readNumber<RX>(FILELINE);
    Y = input->readNumber<RX>(FILELINE);
    relHt = input->readNumber<RX>(FILELINE);
    mult = input->readNumber<RX>(FILELINE);
    wPset = input->readNumber<RX>(FILELINE);
    wPmod = input->readNumber<RX>(FILELINE);
    wazm = input->readNumber<RX>(FILELINE);
    Fahs = input->readNumber<RX>(FILELINE);
    Xmax = input->readNumber<RX>(FILELINE);
    Xmin = input->readNumber<RX>(FILELINE);
    icon = input->readInt(FILELINE);
    dir = input->readInt(FILELINE);
    u_Ht = input->readInt(FILELINE);
    u_XY = input->readInt(FILELINE);
    u_dP = input->readInt(FILELINE);
    u_F = input->readInt(FILELINE);
    cfd = input->readInt(FILELINE);
    if(cfd)
    {
        name = input->readString(FILELINE);
        cfd_ptype = input->readInt(FILELINE);
        cfd_btype = input->readInt(FILELINE);
        cfd_capp = input->readInt(FILELINE);
    }
}


QString Path::write()
{
    QString string;
    string+=QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17 %18 %19 %20 %21 %22 %23 %24 %25 %26 %27 %28").arg(nr)
            .arg(flags).arg(pzn).arg(pzm).arg(pe).arg(pf).arg(pw).arg(pa).arg(ps).arg(pc).arg(pld).arg(RX_C(X)).arg(RX_C(Y))
            .arg(RX_C(relHt)).arg(RX_C(mult)).arg(RX_C(wPset)).arg(RX_C(wPmod)).arg(RX_C(wazm)).arg(RX_C(Fahs)).arg(RX_C(Xmax))
            .arg(RX_C(Xmin)).arg(icon).arg(dir).arg(u_Ht).arg(u_XY).arg(u_dP).arg(u_F).arg(cfd);
    if(cfd)
        string += QString(" %1 %2 %3 %4").arg(name).arg(cfd_ptype).arg(cfd_btype).arg(cfd_capp);
    return string;
}

void Path::setWindPressure(bool b)
{
    if(b)
        flags |= WIND;
    else
        flags &= 0xFFFE;
}

bool Path::windPressure()
{
    return flags & WIND;
}

void Path::setSystem(bool b)
{
    if(b)
        flags |= AHS_S;
    else
        flags &= 0xFFF7;
}

bool Path::system()
{
    return flags & AHS_S;
}

void Path::setExhaust(bool b)
{
    if(b)
        flags |= AHS_X;
    else
        flags &= 0xFFBF;
}

bool Path::exhaust()
{
    return flags & AHS_X;
}


void Path::setRecirculation(bool b)
{
    if(b)
        flags |= AHS_R;
    else
        flags &= 0xFFEF;
}

bool Path::recirculation()
{
    return flags & AHS_R;
}

void Path::setOutsideAir(bool b)
{
    if(b)
        flags |= AHS_O;
    else
        flags &= 0xFFDF;
}

bool Path::outsideAir()
{
    return flags & AHS_O;
}

SourceSink::SourceSink()
{
    nr=0;
    pz=0;
    pe=0;
    ps=0;
    pc=0;
    mult=RX_I(0);
    CC0=RX_I(0);
    Xmin=RX_I(0);
    Ymin=RX_I(0);
    Hmin=RX_I(0);
    Xmax=RX_I(0);
    Ymax=RX_I(0);
    Hmax=RX_I(0);
    u_XYZ=0;
    cfd=0;
}

void SourceSink::read(Reader *input)
{
    nr = input->readInt(FILELINE);
    pz = input->readInt(FILELINE);
    pe = input->readInt(FILELINE);
    ps = input->readInt(FILELINE);
    pc = input->readInt(FILELINE);
    mult = input->readNumber<RX>(FILELINE);
    CC0 = input->readNumber<RX>(FILELINE);
    Xmin = input->readNumber<RX>(FILELINE);
    Ymin = input->readNumber<RX>(FILELINE);
    Hmin = input->readNumber<RX>(FILELINE);
    Xmax = input->readNumber<RX>(FILELINE);
    Ymax = input->readNumber<RX>(FILELINE);
    Hmax = input->readNumber<RX>(FILELINE);
    u_XYZ = input->readInt(FILELINE);
    cfd = input->readInt(FILELINE);
    if(cfd)
        cfdname = input->readString(FILELINE);
}

QString SourceSink::write()
{
    QString string = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15").arg(nr).arg(pz).arg(pe).arg(ps).arg(pc)
            .arg(RX_C(mult)).arg(RX_C(CC0)).arg(RX_C(Xmin)).arg(RX_C(Ymin)).arg(RX_C(Hmin)).arg(RX_C(Xmax)).arg(RX_C(Ymax))
            .arg(RX_C(Hmax)).arg(u_XYZ).arg(cfd);
    if(cfd)
        string += " " + cfdname;
    return string;
}

}
CONTAMNAMESPACEEND
