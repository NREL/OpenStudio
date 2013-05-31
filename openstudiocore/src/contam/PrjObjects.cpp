#include "PrjObjects.hpp"
#include "IconDefs.hpp"

// Zone flags from contam.h
// Zone flag bit 1; variable pressure
#define  VAR_P 0x0001
#define NVAR_P 0xFFFE
// Zone flag bit 2; variable contaminants
#define  VAR_C 0x0002
#define NVAR_C 0xFFFD
// Zone flag bit 3; variable temperature
#define  VAR_T 0x0004
#define NVAR_T 0xFFFB
// Zone flag bit 4; system zone: to avoid zone volume check
#define  SYS_N 0x0008 
//#define NSYS_N 0xFFF7
// Zone flag bit 5; un|conditioned space: to compute air change rate
#define  UNCZN 0x0010   // flags | UNCZN to indicate unconditioned zone
#define SETCZN 0xFFEF   // flags & SETCZN to indicate conditioned zone
// Zone flag bit 6; CFD zone
#define  CFDZN 0x0020   // flags | CFDZN to set CFD zone / flags & CFDZN to test for CFD zone
#define NCFDZN 0xFFDF   // flags & NCFDZN to unset CFD zone

#define FLAG_N 0x003F   /* all zone flag bits, used in PrjRead() */

static QString formatter(const char * format, QString value)
{
    return QString(format).arg(value);
}

static QString formatter(const char * format, float value)
{
    return QString(format).arg(value);
}

static QString formatter(const char * format, int value)
{
    return QString(format).arg(value);
}

static QString formatter(const char * format, unsigned int value)
{
    return QString(format).arg(value);
}

namespace openstudio {
namespace contam {
namespace prj {

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
    Tambt = input->readNumber<RX>();
    barpres = input->readNumber<RX>();
    windspd = input->readNumber<RX>();
    winddir = input->readNumber<RX>();
    relhum = input->readNumber<RX>();
    daytyp = input->readInt();
    uTa = input->readInt();
    ubP = input->readInt();
    uws = input->readInt();
    uwd = input->readInt();
}

QString Weather::write()
{
    QString string = formatter("%1",Tambt);
    string += formatter(" %1",barpres);
    string += formatter(" %1",windspd);
    string += formatter(" %1",winddir);
    string += formatter(" %1",relhum);
    string += formatter(" %1",daytyp);
    string += formatter(" %1",uTa);
    string += formatter(" %1",ubP);
    string += formatter(" %1",uws);
    string += formatter(" %1",uwd);
    return string;
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
    name = input->readString();
    version = input->readString();
    echo = input->readInt();
    prjdesc = input->readLine();
    skheight = input->readInt();
    skwidth = input->readInt();
    def_units = input->readInt();
    def_flows = input->readInt();
    def_T = input->readNumber<RX>();
    udefT = input->readInt();
    rel_N = input->readNumber<RX>();
    wind_H = input->readNumber<RX>();
    uwH = input->readInt();
    wind_Ao = input->readNumber<RX>();
    wind_a = input->readNumber<RX>();
    scale = input->readNumber<RX>();
    uScale = input->readInt();
    orgRow = input->readInt();
    orgCol = input->readInt();
    invYaxis = input->readInt();
    showGeom = input->readInt();
    ssWeather.read(input);
    wptWeather.read(input);
    WTHpath = input->readString();
    CTMpath = input->readString();
    CVFpath = input->readString();
    DVFpath = input->readString();
    WPCfile = input->readString();
    EWCfile = input->readString();
    WPCdesc = input->readLine();
    X0 = input->readNumber<RX>();
    Y0 = input->readNumber<RX>();
    Z0 = input->readNumber<RX>();
    angle = input->readNumber<RX>();
    u_XYZ = input->readInt();
    epsPath = input->readNumber<RX>();
    epsSpcs = input->readNumber<RX>();
    tShift = input->readString();
    dStart = input->readString();
    dEnd = input->readString();
    useWPCwp = input->readInt();
    useWPCmf = input->readInt();
    wpctrig = input->readInt();
    latd = input->readNumber<RX>();
    lgtd = input->readNumber<RX>();
    Tznr = input->readNumber<RX>();
    altd = input->readNumber<RX>();
    Tgrnd = input->readNumber<RX>();
    utg = input->readInt();
    u_a = input->readInt();
    sim_af = input->readInt();
    afcalc = input->readInt();
    afmaxi = input->readInt();
    afrcnvg = input->readNumber<RX>();
    afacnvg = input->readNumber<RX>();
    afrelax = input->readNumber<RX>();
    uac2 = input->readInt();
    Pres = input->readNumber<RX>();
    uPres = input->readInt();
    afslae = input->readInt();
    afrseq = input->readInt();
    aflmaxi = input->readInt();
    aflcnvg = input->readNumber<RX>();
    aflinit = input->readInt();
    Tadj = input->readInt();
    sim_mf = input->readInt();
    ccmaxi = input->readInt();
    ccrcnvg = input->readNumber<RX>();
    ccacnvg = input->readNumber<RX>();
    ccrelax = input->readNumber<RX>();
    uccc = input->readInt();
    mfnmthd = input->readInt();
    mfnrseq = input->readInt();
    mfnmaxi = input->readInt();
    mfnrcnvg = input->readNumber<RX>();
    mfnacnvg = input->readNumber<RX>();
    mfnrelax = input->readNumber<RX>();
    mfngamma = input->readNumber<RX>();
    uccn = input->readInt();
    mftmthd = input->readInt();
    mftrseq = input->readInt();
    mftmaxi = input->readInt();
    mftrcnvg = input->readNumber<RX>();
    mftacnvg = input->readNumber<RX>();
    mftrelax = input->readNumber<RX>();
    mftgamma = input->readNumber<RX>();
    ucct = input->readInt();
    mfvmthd = input->readInt();
    mfvrseq = input->readInt();
    mfvmaxi = input->readInt();
    mfvrcnvg = input->readNumber<RX>();
    mfvacnvg = input->readNumber<RX>();
    mfvrelax = input->readNumber<RX>();
    uccv = input->readInt();
    mf_solver = input->readInt();
    sim_1dz = input->readInt();
    sim_1dd = input->readInt();
    celldx = input->readNumber<RX>();
    sim_vjt = input->readInt();
    udx = input->readInt();
    cvode_mth = input->readInt();
    cvode_rcnvg = input->readNumber<RX>();
    cvode_acnvg = input->readNumber<RX>();
    cvode_dtmax = input->readNumber<RX>();
    tsdens = input->readInt();
    tsrelax = input->readNumber<RX>();
    tsmaxi = input->readInt();
    cnvgSS = input->readInt();
    densZP = input->readInt();
    stackD = input->readInt();
    dodMdt = input->readInt();
    date_st = input->readString();
    time_st = input->readString();
    date_0 = input->readString();
    time_0 = input->readString();
    date_1 = input->readString();
    time_1 = input->readString();
    time_step = input->readString();
    time_list = input->readString();
    time_scrn = input->readString();
    restart = input->readInt();
    rstdate = input->readString();
    rsttime = input->readString();
    list = input->readInt();
    doDlg = input->readInt();
    pfsave = input->readInt();
    zfsave = input->readInt();
    zcsave = input->readInt();
    achvol = input->readInt();
    achsave = input->readInt();
    abwsave = input->readInt();
    cbwsave = input->readInt();
    expsave = input->readInt();
    ebwsave = input->readInt();
    zaasave = input->readInt();
    zbwsave = input->readInt();
    rzfsave = input->readInt();
    rzmsave = input->readInt();
    rz1save = input->readInt();
    csmsave = input->readInt();
    srfsave = input->readInt();
    logsave = input->readInt();
    for(int i=0;i<16;i++)
        save[i] = input->readInt();
    int nrvals = input->readInt();
    //rvals = new RX[nrvals];
	rvals.resize(nrvals);
    for(int i=0;i<nrvals;i++)
        rvals[i] = input->readNumber<RX>();
    BldgFlowZ = input->readInt();
    BldgFlowD = input->readInt();
    BldgFlowC = input->readInt();
    cfd_ctype = input->readInt();
    cfd_convcpl = input->readNumber<RX>();
    cfd_var = input->readInt();
    cfd_zref = input->readInt();
    cfd_imax = input->readInt();
    cfd_dtcmo = input->readInt();
}

QString RunControl::write()
{
    QString string = formatter("%1",name);
    string += formatter(" %1",version);
    string += formatter(" %1\n",echo);
    string += formatter("%1\n",prjdesc);
    string += formatter("%1",skheight);
    string += formatter(" %1",skwidth);
    string += formatter(" %1",def_units);
    string += formatter(" %1",def_flows);
    string += formatter(" %1",def_T);
    string += formatter(" %1",udefT);
    string += formatter(" %1",rel_N);
    string += formatter(" %1",wind_H);
    string += formatter(" %1",uwH);
    string += formatter(" %1",wind_Ao);
    string += formatter(" %1\n",wind_a);
    string += formatter("%1",scale);
    string += formatter(" %1",uScale);
    string += formatter(" %1",orgRow);
    string += formatter(" %1",orgCol);
    string += formatter(" %1",invYaxis);
    string += formatter(" %1\n",showGeom);
    string += ssWeather.write() + '\n';
    string += wptWeather.write() + '\n';
    string += formatter("%1\n",WTHpath);
    string += formatter("%1\n",CTMpath);
    string += formatter("%1\n",CVFpath);
    string += formatter("%1\n",DVFpath);
    string += formatter("%1\n",WPCfile);
    string += formatter("%1\n",EWCfile);
    string += formatter("%1\n",WPCdesc);
    string += formatter("%1",X0);
    string += formatter(" %1",Y0);
    string += formatter(" %1",Z0);
    string += formatter(" %1",angle);
    string += formatter(" %1\n",u_XYZ);
    string += formatter("%1",epsPath);
    string += formatter(" %1",epsSpcs);
    string += formatter(" %1",tShift);
    string += formatter(" %1",dStart);
    string += formatter(" %1",dEnd);
    string += formatter(" %1",useWPCwp);
    string += formatter(" %1",useWPCmf);
    string += formatter(" %1\n",wpctrig);
    string += formatter("%1",latd);
    string += formatter(" %1",lgtd);
    string += formatter(" %1",Tznr);
    string += formatter(" %1",altd);
    string += formatter(" %1",Tgrnd);
    string += formatter(" %1",utg);
    string += formatter(" %1\n",u_a);
    string += formatter("%1",sim_af);
    string += formatter(" %1",afcalc);
    string += formatter(" %1",afmaxi);
    string += formatter(" %1",afrcnvg);
    string += formatter(" %1",afacnvg);
    string += formatter(" %1",afrelax);
    string += formatter(" %1",uac2);
    string += formatter(" %1",Pres);
    string += formatter(" %1\n",uPres);
    string += formatter("%1",afslae);
    string += formatter(" %1",afrseq);
    string += formatter(" %1",aflmaxi);
    string += formatter(" %1",aflcnvg);
    string += formatter(" %1",aflinit);
    string += formatter(" %1\n",Tadj);
    string += formatter("%1",sim_mf);
    string += formatter(" %1",ccmaxi);
    string += formatter(" %1",ccrcnvg);
    string += formatter(" %1",ccacnvg);
    string += formatter(" %1",ccrelax);
    string += formatter(" %1\n",uccc);
    string += formatter("%1",mfnmthd);
    string += formatter(" %1",mfnrseq);
    string += formatter(" %1",mfnmaxi);
    string += formatter(" %1",mfnrcnvg);
    string += formatter(" %1",mfnacnvg);
    string += formatter(" %1",mfnrelax);
    string += formatter(" %1",mfngamma);
    string += formatter(" %1\n",uccn);
    string += formatter("%1",mftmthd);
    string += formatter(" %1",mftrseq);
    string += formatter(" %1",mftmaxi);
    string += formatter(" %1",mftrcnvg);
    string += formatter(" %1",mftacnvg);
    string += formatter(" %1",mftrelax);
    string += formatter(" %1",mftgamma);
    string += formatter(" %1\n",ucct);
    string += formatter("%1",mfvmthd);
    string += formatter(" %1",mfvrseq);
    string += formatter(" %1",mfvmaxi);
    string += formatter(" %1",mfvrcnvg);
    string += formatter(" %1",mfvacnvg);
    string += formatter(" %1",mfvrelax);
    string += formatter(" %1\n",uccv);
    string += formatter("%1",mf_solver);
    string += formatter(" %1",sim_1dz);
    string += formatter(" %1",sim_1dd);
    string += formatter(" %1",celldx);
    string += formatter(" %1",sim_vjt);
    string += formatter(" %1\n",udx);
    string += formatter("%1",cvode_mth);
    string += formatter(" %1",cvode_rcnvg);
    string += formatter(" %1",cvode_acnvg);
    string += formatter(" %1\n",cvode_dtmax);
    string += formatter("%1",tsdens);
    string += formatter(" %1",tsrelax);
    string += formatter(" %1",tsmaxi);
    string += formatter(" %1",cnvgSS);
    string += formatter(" %1",densZP);
    string += formatter(" %1",stackD);
    string += formatter(" %1\n",dodMdt);
    string += formatter("%1",date_st);
    string += formatter(" %1",time_st);
    string += formatter(" %1",date_0);
    string += formatter(" %1",time_0);
    string += formatter(" %1",date_1);
    string += formatter(" %1",time_1);
    string += formatter(" %1",time_step);
    string += formatter(" %1",time_list);
    string += formatter(" %1\n",time_scrn);
    string += formatter("%1",restart);
    string += formatter(" %1",rstdate);
    string += formatter(" %1\n",rsttime);
    string += formatter("%1",list);
    string += formatter(" %1",doDlg);
    string += formatter(" %1",pfsave);
    string += formatter(" %1",zfsave);
    string += formatter(" %1\n",zcsave);
    string += formatter("%1",achvol);
    string += formatter(" %1",achsave);
    string += formatter(" %1",abwsave);
    string += formatter(" %1",cbwsave);
    string += formatter(" %1",expsave);
    string += formatter(" %1",ebwsave);
    string += formatter(" %1",zaasave);
    string += formatter(" %1\n",zbwsave);
    string += formatter("%1",rzfsave);
    string += formatter(" %1",rzmsave);
    string += formatter(" %1",rz1save);
    string += formatter(" %1",csmsave);
    string += formatter(" %1",srfsave);
    string += formatter(" %1\n",logsave);
    for(int i=0;i<16;i++)
        string += formatter(" %1",save[i]);
    //string += formatter("\n%1\n",nrvals);
    string += formatter("\n%1\n",rvals.size());
    for(int i=0;i<rvals.size();i++)
        string += formatter(" %1",rvals[i]);
    string += formatter("\n%1",BldgFlowZ);
    string += formatter(" %1",BldgFlowD);
    string += formatter(" %1\n",BldgFlowC);
    string += formatter("%1",cfd_ctype);
    string += formatter(" %1",cfd_convcpl);
    string += formatter(" %1",cfd_var);
    string += formatter(" %1",cfd_zref);
    string += formatter(" %1",cfd_imax);
    string += formatter(" %1",cfd_dtcmo);
    return string;
}

// Section 2: Species and Contaminants

Species::Species()
{
    nr=0;
    sflag=0;
    ntflag=0;
    molwt=RX_INIT;
    mdiam=RX_INIT;
    edens=RX_INIT;
    decay=RX_INIT;
    Dm=RX_INIT;
    ccdef=RX_INIT;
    Cp=RX_INIT;
    ucc=0;
    umd=0;
    ued=0;
    udm=0;
    ucp=0;
}

void Species::read(Reader *input)
{
    nr = input->readInt();
    sflag = input->readInt();
    ntflag = input->readInt();
    molwt = input->readNumber<RX>();
    mdiam = input->readNumber<RX>();
    edens = input->readNumber<RX>();
    decay = input->readNumber<RX>();
    Dm = input->readNumber<RX>();
    ccdef = input->readNumber<RX>();
    Cp = input->readNumber<RX>();
    ucc = input->readInt();
    umd = input->readInt();
    ued = input->readInt();
    udm = input->readInt();
    ucp = input->readInt();
    name = input->readString();
    desc = input->readLine();
}

QString Species::write()
{
    QString string = formatter(" %1",nr);
    string += formatter(" %1",sflag);
    string += formatter(" %1",ntflag);
    string += formatter(" %1",RX_C(molwt));
    string += formatter(" %1",RX_C(mdiam));
    string += formatter(" %1",RX_C(edens));
    string += formatter(" %1",RX_C(decay));
    string += formatter(" %1",RX_C(Dm));
    string += formatter(" %1",RX_C(ccdef));
    string += formatter(" %1",RX_C(Cp));
    string += formatter(" %1",ucc);
    string += formatter(" %1",umd);
    string += formatter(" %1",ued);
    string += formatter(" %1",udm);
    string += formatter(" %1",ucp);
    string += formatter(" %1\n",name);
    string += formatter("%1",desc);
    return string;
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
    this->icon = input->readInt();
    this->col = input->readInt();
    this->row = input->readInt();
    this->nr = input->readInt();
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
    refht=RX_INIT;
    delht=RX_INIT;
    u_rfht=0;
    u_dlht=0;
}

void Level::read(Reader *input)
{
    nr = input->readInt();
    refht = input->readNumber<RX>();
    delht = input->readNumber<RX>();
    int nicon = input->readInt();
    u_rfht = input->readInt();
    u_dlht = input->readInt();
    name = input->readString();
    for(int i=0;i<nicon;i++)
    {
        Icon icon;
        icon.read(input);
        icons << icon;
    }
}

QString Level::write()
{
    QString string = formatter(" %1",nr);
    string += formatter(" %1",refht);
    string += formatter(" %1",delht);
    string += formatter(" %1",icons.size());
    string += formatter(" %1",u_rfht);
    string += formatter(" %1",u_dlht);
    string += formatter(" %1\n",name);
    if(icons.size())
    {
        for(int i=0;i<icons.size()-1;i++)
            string += icons[i].write() + '\n';
        string += icons[icons.size()-1].write();
    }
    return string;
}

Ahs::Ahs()
{
    nr=0;
    zone_r=0;
    zone_s=0;
    path_r=0;
    path_s=0;
    path_x=0;
    name=QString();
    desc=QString();
}

void Ahs::read(Reader *input)
{
    nr = input->readInt();
    zone_r = input->readInt();
    zone_s = input->readInt();
    path_r = input->readInt();
    path_s = input->readInt();
    path_x = input->readInt();
    name = input->readString();
    desc = input->readLine();
}

QString Ahs::write()
{
    QString string = formatter(" %1",nr);
    string += formatter(" %1",zone_r);
    string += formatter(" %1",zone_s);
    string += formatter(" %1",path_r);
    string += formatter(" %1",path_s);
    string += formatter(" %1",path_x);
    string += formatter(" %1\n",name);
    string += formatter("%1",desc);
    return string;
}

Zone::Zone()
{
    nr=0;
    flags=0;
    ps=0;
    pc=0;
    pk=0;
    pl=0;
    relHt=RX_INIT;
    Vol=RX_INIT;
    T0=RX_INIT;
    P0=RX_INIT;
    name=QString();
    color=0;
    u_Ht=0;
    u_V=0;
    u_T=0;
    u_P=0;
    cdaxis=0;
    cfd=0;
    cfdname=QString();
    X1=RX_INIT;
    Y1=RX_INIT;
    H1=RX_INIT;
    X2=RX_INIT;
    Y2=RX_INIT;
    H2=RX_INIT;
    celldx=RX_INIT;
    axialD=RX_INIT;
    u_aD=0;
    u_L=0;
}

void Zone::read(Reader *input)
{
    nr = input->readInt();
    flags = input->readInt();
    ps = input->readInt();
    pc = input->readInt();
    pk = input->readInt();
    pl = input->readInt();
    relHt = input->readNumber<RX>();
    Vol = input->readNumber<RX>();
    T0 = input->readNumber<RX>();
    P0 = input->readNumber<RX>();
    name = input->readString();
    color = input->readInt();
    u_Ht = input->readInt();
    u_V = input->readInt();
    u_T = input->readInt();
    u_P = input->readInt();
    cdaxis = input->readInt();
    cfd = input->readInt();
    if(cfd)
        cfdname = input->readString();
    else if(cdaxis)
    {
        input->readString();  // Read "1D:"
        X1 = input->readNumber<RX>();
        Y1 = input->readNumber<RX>();
        H1 = input->readNumber<RX>();
        X2 = input->readNumber<RX>();
        Y2 = input->readNumber<RX>();
        H2 = input->readNumber<RX>();
        celldx = input->readNumber<RX>();
        axialD = input->readNumber<RX>();
        u_aD = input->readInt();
        u_L = input->readInt();
    }
}

QString Zone::write()
{
    QString string = formatter("%1",nr);
    string += formatter(" %1",flags);
    string += formatter(" %1",ps);
    string += formatter(" %1",pc);
    string += formatter(" %1",pk);
    string += formatter(" %1",pl);
    string += formatter(" %1",relHt);
    string += formatter(" %1",Vol);
    string += formatter(" %1",T0);
    string += formatter(" %1",P0);
    string += formatter(" %1",name);
    string += formatter(" %1",color);
    string += formatter(" %1",u_Ht);
    string += formatter(" %1",u_V);
    string += formatter(" %1",u_T);
    string += formatter(" %1",u_P);
    string += formatter(" %1",cdaxis);
    string += formatter(" %1",cfd);
    if(cfd)
        string += formatter(" %1",cfdname);
    else if(cdaxis)
    {
        string += formatter(" %1","1D:");
        string += formatter(" %1",X1);
        string += formatter(" %1",Y1);
        string += formatter(" %1",H1);
        string += formatter(" %1",X2);
        string += formatter(" %1",Y2);
        string += formatter(" %1",H2);
        string += formatter(" %1",celldx);
        string += formatter(" %1",axialD);
        string += formatter(" %1",u_aD);
        string += formatter(" %1",u_L);
    }
    return string;
}

void Zone::setVariablePressure(bool b)
{
    if(b)
        flags |= VAR_P;
    else
        flags &= NVAR_P;
}

bool Zone::variablePressure()
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

bool Zone::variableContaminants()
{
    return flags & VAR_C;
}

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
    X=RX_INIT;
    Y=RX_INIT;
    relHt=RX_INIT;
    mult=RX_INIT;
    wPset=RX_INIT;
    wPmod=RX_INIT;
    wazm=RX_INIT;
    Fahs=RX_INIT;
    Xmax=RX_INIT;
    Xmin=RX_INIT;
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
}

void Path::read(Reader *input)
{
    nr = input->readInt();
    flags = input->readInt();
    pzn = input->readInt();
    pzm = input->readInt();
    pe = input->readInt();
    pf = input->readInt();
    pw = input->readInt();
    pa = input->readInt();
    ps = input->readInt();
    pc = input->readInt();
    pld = input->readInt();
    X = input->readNumber<RX>();
    Y = input->readNumber<RX>();
    relHt = input->readNumber<RX>();
    mult = input->readNumber<RX>();
    wPset = input->readNumber<RX>();
    wPmod = input->readNumber<RX>();
    wazm = input->readNumber<RX>();
    Fahs = input->readNumber<RX>();
    Xmax = input->readNumber<RX>();
    Xmin = input->readNumber<RX>();
    icon = input->readInt();
    dir = input->readInt();
    u_Ht = input->readInt();
    u_XY = input->readInt();
    u_dP = input->readInt();
    u_F = input->readInt();
    cfd = input->readInt();
    if(cfd)
    {
        name = input->readString();
        cfd_ptype = input->readInt();
        cfd_btype = input->readInt();
        cfd_capp = input->readInt();
    }
}

QString Path::write()
{
    QString string = formatter("%1",nr);
    string += formatter(" %1",flags);
    string += formatter(" %1",pzn);
    string += formatter(" %1",pzm);
    string += formatter(" %1",pe);
    string += formatter(" %1",pf);
    string += formatter(" %1",pw);
    string += formatter(" %1",pa);
    string += formatter(" %1",ps);
    string += formatter(" %1",pc);
    string += formatter(" %1",pld);
    string += formatter(" %1",X);
    string += formatter(" %1",Y);
    string += formatter(" %1",relHt);
    string += formatter(" %1",mult);
    string += formatter(" %1",wPset);
    string += formatter(" %1",wPmod);
    string += formatter(" %1",wazm);
    string += formatter(" %1",Fahs);
    string += formatter(" %1",Xmax);
    string += formatter(" %1",Xmin);
    string += formatter(" %1",icon);
    string += formatter(" %1",dir);
    string += formatter(" %1",u_Ht);
    string += formatter(" %1",u_XY);
    string += formatter(" %1",u_dP);
    string += formatter(" %1",u_F);
    string += formatter(" %1",cfd);
    if(cfd)
    {
        string += formatter(" %1",name);
        string += formatter(" %1",cfd_ptype);
        string += formatter(" %1",cfd_btype);
        string += formatter(" %1",cfd_capp);
    }
    return string;
}

}
}
}

// Kill off the zone flags
#undef  VAR_P
#undef NVAR_P
#undef  VAR_C
#undef NVAR_C
#undef  VAR_T
#undef NVAR_T
#undef  SYS_N 
//#define NSYS_N 0xFFF7
#undef  UNCZN
#undef SETCZN
#undef  CFDZN
#undef NCFDZN
#undef FLAG_N