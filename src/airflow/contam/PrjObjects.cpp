/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PrjObjects.hpp"

#include "../../utilities/core/Logger.hpp"

namespace openstudio {
namespace contam {

  Zone::Zone() : m_impl(std::shared_ptr<detail::ZoneImpl>(new detail::ZoneImpl())) {}

  Zone::Zone(unsigned flags, std::string Vol, std::string T0, std::string name) : m_impl(std::shared_ptr<detail::ZoneImpl>(new detail::ZoneImpl)) {
    m_impl->setFlags(flags);
    m_impl->setVol(Vol);
    m_impl->setT0(T0);
    m_impl->setName(name);
  }

  Zone::Zone(unsigned flags, double Vol, double T0, std::string name) : m_impl(std::shared_ptr<detail::ZoneImpl>(new detail::ZoneImpl)) {
    m_impl->setFlags(flags);
    m_impl->setVol(Vol);
    m_impl->setT0(T0);
    m_impl->setName(name);
  }

  Zone::Zone(int nr, unsigned int flags, int ps, int pc, int pk, int pl, std::string relHt, std::string Vol, std::string T0, std::string P0,
             std::string name, int color, int u_Ht, int u_V, int u_T, int u_P, int cdaxis, int cfd, std::string cfdname, std::string X1,
             std::string Y1, std::string H1, std::string X2, std::string Y2, std::string H2, std::string celldx, std::string axialD, int u_aD,
             int u_L)
    : m_impl(std::shared_ptr<detail::ZoneImpl>(new detail::ZoneImpl(nr, flags, ps, pc, pk, pl, relHt, Vol, T0, P0, name, color, u_Ht, u_V, u_T, u_P,
                                                                    cdaxis, cfd, cfdname, X1, Y1, H1, X2, Y2, H2, celldx, axialD, u_aD, u_L))) {}

  Zone::Zone(int nr, unsigned int flags, int ps, int pc, int pk, int pl, double relHt, double Vol, double T0, double P0, std::string name, int color,
             int u_Ht, int u_V, int u_T, int u_P, int cdaxis, int cfd, std::string cfdname, double X1, double Y1, double H1, double X2, double Y2,
             double H2, double celldx, double axialD, int u_aD, int u_L)
    : m_impl(std::shared_ptr<detail::ZoneImpl>(new detail::ZoneImpl(nr, flags, ps, pc, pk, pl, relHt, Vol, T0, P0, name, color, u_Ht, u_V, u_T, u_P,
                                                                    cdaxis, cfd, cfdname, X1, Y1, H1, X2, Y2, H2, celldx, axialD, u_aD, u_L))) {}

  Zone::Zone(const Zone& other) = default;

  Zone& Zone::operator=(const Zone& other) = default;

  bool Zone::operator==(const Zone& other) const {
    return m_impl == other.m_impl;
  }

  bool Zone::operator!=(const Zone& other) const {
    return m_impl != other.m_impl;
  }

  int Zone::nr() const {
    return m_impl->nr();
  }

  void Zone::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  unsigned int Zone::flags() const {
    return m_impl->flags();
  }

  void Zone::setFlags(const unsigned int flags) {
    m_impl->setFlags(flags);
  }

  int Zone::ps() const {
    return m_impl->ps();
  }

  void Zone::setPs(const int ps) {
    m_impl->setPs(ps);
  }

  int Zone::pc() const {
    return m_impl->pc();
  }

  void Zone::setPc(const int pc) {
    m_impl->setPc(pc);
  }

  int Zone::pk() const {
    return m_impl->pk();
  }

  void Zone::setPk(const int pk) {
    m_impl->setPk(pk);
  }

  int Zone::pl() const {
    return m_impl->pl();
  }

  void Zone::setPl(const int pl) {
    m_impl->setPl(pl);
  }

  double Zone::relHt() const {
    return m_impl->relHt();
  }

  bool Zone::setRelHt(const double relHt) {
    return m_impl->setRelHt(relHt);
  }

  bool Zone::setRelHt(const std::string& relHt) {
    return m_impl->setRelHt(relHt);
  }

  double Zone::Vol() const {
    return m_impl->Vol();
  }

  bool Zone::setVol(const double Vol) {
    return m_impl->setVol(Vol);
  }

  bool Zone::setVol(const std::string& Vol) {
    return m_impl->setVol(Vol);
  }

  double Zone::T0() const {
    return m_impl->T0();
  }

  bool Zone::setT0(const double T0) {
    return m_impl->setT0(T0);
  }

  bool Zone::setT0(const std::string& T0) {
    return m_impl->setT0(T0);
  }

  double Zone::P0() const {
    return m_impl->P0();
  }

  bool Zone::setP0(const double P0) {
    return m_impl->setP0(P0);
  }

  bool Zone::setP0(const std::string& P0) {
    return m_impl->setP0(P0);
  }

  std::string Zone::name() const {
    return m_impl->name();
  }

  void Zone::setName(const std::string& name) {
    m_impl->setName(name);
  }

  int Zone::color() const {
    return m_impl->color();
  }

  void Zone::setColor(const int color) {
    m_impl->setColor(color);
  }

  int Zone::u_Ht() const {
    return m_impl->u_Ht();
  }

  void Zone::setU_Ht(const int u_Ht) {
    m_impl->setU_Ht(u_Ht);
  }

  int Zone::u_V() const {
    return m_impl->u_V();
  }

  void Zone::setU_V(const int u_V) {
    m_impl->setU_V(u_V);
  }

  int Zone::u_T() const {
    return m_impl->u_T();
  }

  void Zone::setU_T(const int u_T) {
    m_impl->setU_T(u_T);
  }

  int Zone::u_P() const {
    return m_impl->u_P();
  }

  void Zone::setU_P(const int u_P) {
    m_impl->setU_P(u_P);
  }

  int Zone::cdaxis() const {
    return m_impl->cdaxis();
  }

  void Zone::setCdaxis(const int cdaxis) {
    m_impl->setCdaxis(cdaxis);
  }

  int Zone::cfd() const {
    return m_impl->cfd();
  }

  void Zone::setCfd(const int cfd) {
    m_impl->setCfd(cfd);
  }

  std::string Zone::cfdname() const {
    return m_impl->cfdname();
  }

  void Zone::setCfdname(const std::string& cfdname) {
    m_impl->setCfdname(cfdname);
  }

  double Zone::X1() const {
    return m_impl->X1();
  }

  bool Zone::setX1(const double X1) {
    return m_impl->setX1(X1);
  }

  bool Zone::setX1(const std::string& X1) {
    return m_impl->setX1(X1);
  }

  double Zone::Y1() const {
    return m_impl->Y1();
  }

  bool Zone::setY1(const double Y1) {
    return m_impl->setY1(Y1);
  }

  bool Zone::setY1(const std::string& Y1) {
    return m_impl->setY1(Y1);
  }

  double Zone::H1() const {
    return m_impl->H1();
  }

  bool Zone::setH1(const double H1) {
    return m_impl->setH1(H1);
  }

  bool Zone::setH1(const std::string& H1) {
    return m_impl->setH1(H1);
  }

  double Zone::X2() const {
    return m_impl->X2();
  }

  bool Zone::setX2(const double X2) {
    return m_impl->setX2(X2);
  }

  bool Zone::setX2(const std::string& X2) {
    return m_impl->setX2(X2);
  }

  double Zone::Y2() const {
    return m_impl->Y2();
  }

  bool Zone::setY2(const double Y2) {
    return m_impl->setY2(Y2);
  }

  bool Zone::setY2(const std::string& Y2) {
    return m_impl->setY2(Y2);
  }

  double Zone::H2() const {
    return m_impl->H2();
  }

  bool Zone::setH2(const double H2) {
    return m_impl->setH2(H2);
  }

  bool Zone::setH2(const std::string& H2) {
    return m_impl->setH2(H2);
  }

  double Zone::celldx() const {
    return m_impl->celldx();
  }

  bool Zone::setCelldx(const double celldx) {
    return m_impl->setCelldx(celldx);
  }

  bool Zone::setCelldx(const std::string& celldx) {
    return m_impl->setCelldx(celldx);
  }

  double Zone::axialD() const {
    return m_impl->axialD();
  }

  bool Zone::setAxialD(const double axialD) {
    return m_impl->setAxialD(axialD);
  }

  bool Zone::setAxialD(const std::string& axialD) {
    return m_impl->setAxialD(axialD);
  }

  int Zone::u_aD() const {
    return m_impl->u_aD();
  }

  void Zone::setU_aD(const int u_aD) {
    return m_impl->setU_aD(u_aD);
  }

  int Zone::u_L() const {
    return m_impl->u_L();
  }

  void Zone::setU_L(const int u_L) {
    return m_impl->setU_L(u_L);
  }

  std::string Zone::write() {
    return m_impl->write();
  }

  void Zone::read(Reader& input) {
    m_impl->read(input);
  }

  void Zone::setVariablePressure(bool b) {
    m_impl->setVariablePressure(b);
  }

  bool Zone::variablePressure() const {
    return m_impl->variablePressure();
  }

  void Zone::setVariableContaminants(bool b) {
    m_impl->setVariableContaminants(b);
  }

  bool Zone::variableContaminants() const {
    return m_impl->variableContaminants();
  }

  void Zone::setSystem(bool b) {
    m_impl->setSystem(b);
  }

  bool Zone::system() const {
    return m_impl->system();
  }

  double Zone::ic(const int i) const {
    return m_impl->ic(i);
  }

  std::vector<double> Zone::ic() const {
    return m_impl->ic();
  }

  bool Zone::setIc(const int i, const double value) {
    return m_impl->setIc(i, value);
  }

  bool Zone::setIc(const int i, const std::string& value) {
    return m_impl->setIc(i, value);
  }

  bool Zone::setIc(std::vector<double>& ic) {
    return m_impl->setIc(ic);
  }

  bool Zone::setIc(std::vector<std::string>& ic) {
    return m_impl->setIc(ic);
  }

  Species::Species() : m_impl(std::shared_ptr<detail::SpeciesImpl>(new detail::SpeciesImpl)) {}

  Species::Species(std::string molwt, std::string Dm, std::string ccdef, std::string /*Cp*/, std::string name, std::string desc)
    : m_impl(std::shared_ptr<detail::SpeciesImpl>(new detail::SpeciesImpl)) {
    m_impl->setMolwt(molwt);
    m_impl->setDm(Dm);
    m_impl->setCcdef(ccdef);
    m_impl->setName(name);
    m_impl->setDesc(desc);
  }

  Species::Species(double molwt, double Dm, double ccdef, double /*Cp*/, std::string name, std::string desc)
    : m_impl(std::shared_ptr<detail::SpeciesImpl>(new detail::SpeciesImpl)) {
    m_impl->setMolwt(molwt);
    m_impl->setDm(Dm);
    m_impl->setCcdef(ccdef);
    m_impl->setName(name);
    m_impl->setDesc(desc);
  }

  Species::Species(int nr, int sflag, int ntflag, std::string molwt, std::string mdiam, std::string edens, std::string decay, std::string Dm,
                   std::string ccdef, std::string Cp, int ucc, int umd, int ued, int udm, int ucp, std::string name, std::string desc)
    : m_impl(std::shared_ptr<detail::SpeciesImpl>(
        new detail::SpeciesImpl(nr, sflag, ntflag, molwt, mdiam, edens, decay, Dm, ccdef, Cp, ucc, umd, ued, udm, ucp, name, desc))) {}

  Species::Species(int nr, int sflag, int ntflag, double molwt, double mdiam, double edens, double decay, double Dm, double ccdef, double Cp, int ucc,
                   int umd, int ued, int udm, int ucp, std::string name, std::string desc)
    : m_impl(std::shared_ptr<detail::SpeciesImpl>(
        new detail::SpeciesImpl(nr, sflag, ntflag, molwt, mdiam, edens, decay, Dm, ccdef, Cp, ucc, umd, ued, udm, ucp, name, desc))) {}

  Species::Species(const Species& other) = default;

  Species& Species::operator=(const Species& other) = default;

  bool Species::operator==(const Species& other) const {
    return m_impl == other.m_impl;
  }

  bool Species::operator!=(const Species& other) const {
    return m_impl != other.m_impl;
  }

  std::string Species::write() {
    return m_impl->write();
  }

  void Species::read(Reader& input) {
    m_impl->read(input);
  }

  int Species::nr() const {
    return m_impl->nr();
  }

  void Species::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  bool Species::sflag() const {
    return m_impl->sflag();
  }

  void Species::setSflag(const bool sflag) {
    m_impl->setSflag(sflag);
  }

  bool Species::ntflag() const {
    return m_impl->ntflag();
  }

  void Species::setNtflag(const bool ntflag) {
    m_impl->setNtflag(ntflag);
  }

  double Species::molwt() const {
    return m_impl->molwt();
  }

  bool Species::setMolwt(const double molwt) {
    return m_impl->setMolwt(molwt);
  }

  bool Species::setMolwt(const std::string& molwt) {
    return m_impl->setMolwt(molwt);
  }

  double Species::mdiam() const {
    return m_impl->mdiam();
  }

  bool Species::setMdiam(const double mdiam) {
    return m_impl->setMdiam(mdiam);
  }

  bool Species::setMdiam(const std::string& mdiam) {
    return m_impl->setMdiam(mdiam);
  }

  double Species::edens() const {
    return m_impl->edens();
  }

  bool Species::setEdens(const double edens) {
    return m_impl->setEdens(edens);
  }

  bool Species::setEdens(const std::string& edens) {
    return m_impl->setEdens(edens);
  }

  double Species::decay() const {
    return m_impl->decay();
  }

  bool Species::setDecay(const double decay) {
    return m_impl->setDecay(decay);
  }

  bool Species::setDecay(const std::string& decay) {
    return m_impl->setDecay(decay);
  }

  double Species::Dm() const {
    return m_impl->Dm();
  }

  bool Species::setDm(const double Dm) {
    return m_impl->setDm(Dm);
  }

  bool Species::setDm(const std::string& Dm) {
    return m_impl->setDm(Dm);
  }

  double Species::ccdef() const {
    return m_impl->ccdef();
  }

  bool Species::setCcdef(const double ccdef) {
    return m_impl->setCcdef(ccdef);
  }

  bool Species::setCcdef(const std::string& ccdef) {
    return m_impl->setCcdef(ccdef);
  }

  double Species::Cp() const {
    return m_impl->Cp();
  }

  bool Species::setCp(const double Cp) {
    return m_impl->setCp(Cp);
  }

  bool Species::setCp(const std::string& Cp) {
    return m_impl->setCp(Cp);
  }

  int Species::ucc() const {
    return m_impl->ucc();
  }

  void Species::setUcc(const int ucc) {
    m_impl->setUcc(ucc);
  }

  int Species::umd() const {
    return m_impl->umd();
  }

  void Species::setUmd(const int umd) {
    m_impl->setUmd(umd);
  }

  int Species::ued() const {
    return m_impl->ued();
  }

  void Species::setUed(const int ued) {
    m_impl->setUed(ued);
  }

  int Species::udm() const {
    return m_impl->udm();
  }

  void Species::setUdm(const int udm) {
    m_impl->setUdm(udm);
  }

  int Species::ucp() const {
    return m_impl->ucp();
  }

  void Species::setUcp(const int ucp) {
    m_impl->setUcp(ucp);
  }

  std::string Species::name() const {
    return m_impl->name();
  }

  void Species::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::string Species::desc() const {
    return m_impl->desc();
  }

  void Species::setDesc(const std::string& desc) {
    m_impl->setDesc(desc);
  }

  Ahs::Ahs() : m_impl(std::shared_ptr<detail::AhsImpl>(new detail::AhsImpl)) {}

  Ahs::Ahs(int nr, int zone_r, int zone_s, int path_r, int path_s, int path_x, std::string name, std::string desc)
    : m_impl(std::shared_ptr<detail::AhsImpl>(new detail::AhsImpl(nr, zone_r, zone_s, path_r, path_s, path_x, name, desc))) {}

  Ahs::Ahs(const Ahs& other) = default;

  Ahs& Ahs::operator=(const Ahs& other) = default;

  bool Ahs::operator==(const Ahs& other) const {
    return m_impl == other.m_impl;
  }

  bool Ahs::operator!=(const Ahs& other) const {
    return m_impl != other.m_impl;
  }

  std::string Ahs::write() {
    return m_impl->write();
  }

  void Ahs::read(Reader& input) {
    m_impl->read(input);
  }

  int Ahs::nr() const {
    return m_impl->nr();
  }

  void Ahs::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  int Ahs::zone_r() const {
    return m_impl->zone_r();
  }

  void Ahs::setZone_r(const int zone_r) {
    m_impl->setZone_r(zone_r);
  }

  int Ahs::zone_s() const {
    return m_impl->zone_s();
  }

  void Ahs::setZone_s(const int zone_s) {
    m_impl->setZone_s(zone_s);
  }

  int Ahs::path_r() const {
    return m_impl->path_r();
  }

  void Ahs::setPath_r(const int path_r) {
    m_impl->setPath_r(path_r);
  }

  int Ahs::path_s() const {
    return m_impl->path_s();
  }

  void Ahs::setPath_s(const int path_s) {
    m_impl->setPath_s(path_s);
  }

  int Ahs::path_x() const {
    return m_impl->path_x();
  }

  void Ahs::setPath_x(const int path_x) {
    m_impl->setPath_x(path_x);
  }

  std::string Ahs::name() const {
    return m_impl->name();
  }

  void Ahs::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::string Ahs::desc() const {
    return m_impl->desc();
  }

  void Ahs::setDesc(const std::string& desc) {
    m_impl->setDesc(desc);
  }

  AirflowPath::AirflowPath() : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl)) {}

  AirflowPath::AirflowPath(int flags, int pzn, int pe, int pw, int pld, std::string relHt, std::string mult, std::string wPset, std::string wPmod,
                           std::string wazm, unsigned int icon)
    : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl)) {
    m_impl->setFlags(flags);
    m_impl->setPzn(pzn);
    m_impl->setPzm(-1);
    m_impl->setPe(pe);
    m_impl->setPw(pw);
    m_impl->setPld(pld);
    m_impl->setRelHt(relHt);
    m_impl->setMult(mult);
    m_impl->setWPset(wPset);
    m_impl->setWPmod(wPmod);
    m_impl->setWazm(wazm);
    m_impl->setIcon(icon);
  }

  AirflowPath::AirflowPath(int flags, int pzn, int pe, int pw, int pld, double relHt, double mult, double wPset, double wPmod, double wazm,
                           unsigned int icon)
    : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl)) {
    m_impl->setFlags(flags);
    m_impl->setPzn(pzn);
    m_impl->setPzm(-1);
    m_impl->setPe(pe);
    m_impl->setPw(pw);
    m_impl->setPld(pld);
    m_impl->setRelHt(relHt);
    m_impl->setMult(mult);
    m_impl->setWPset(wPset);
    m_impl->setWPmod(wPmod);
    m_impl->setWazm(wazm);
    m_impl->setIcon(icon);
  }

  AirflowPath::AirflowPath(int flags, int pzn, int pzm, int pe, int pld, std::string relHt, std::string mult, unsigned int icon)
    : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl)) {
    m_impl->setFlags(flags);
    m_impl->setPzn(pzn);
    m_impl->setPzm(pzm);
    m_impl->setPe(pe);
    m_impl->setPld(pld);
    m_impl->setRelHt(relHt);
    m_impl->setMult(mult);
    m_impl->setIcon(icon);
  }

  AirflowPath::AirflowPath(int flags, int pzn, int pzm, int pe, int pld, double relHt, double mult, unsigned int icon)
    : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl)) {
    m_impl->setFlags(flags);
    m_impl->setPzn(pzn);
    m_impl->setPzm(pzm);
    m_impl->setPe(pe);
    m_impl->setPld(pld);
    m_impl->setRelHt(relHt);
    m_impl->setMult(mult);
    m_impl->setIcon(icon);
  }

  AirflowPath::AirflowPath(int nr, int flags, int pzn, int pzm, int pe, int pf, int pw, int pa, int ps, int pc, int pld, std::string X, std::string Y,
                           std::string relHt, std::string mult, std::string wPset, std::string wPmod, std::string wazm, std::string Fahs,
                           std::string Xmax, std::string Xmin, unsigned int icon, unsigned int dir, int u_Ht, int u_XY, int u_dP, int u_F, int cfd,
                           std::string cfd_name, int cfd_ptype, int cfd_btype, int cfd_capp)
    : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl(nr, flags, pzn, pzm, pe, pf, pw, pa, ps, pc, pld, X, Y, relHt, mult,
                                                                                  wPset, wPmod, wazm, Fahs, Xmax, Xmin, icon, dir, u_Ht, u_XY, u_dP,
                                                                                  u_F, cfd, cfd_name, cfd_ptype, cfd_btype, cfd_capp))) {}

  AirflowPath::AirflowPath(int nr, int flags, int pzn, int pzm, int pe, int pf, int pw, int pa, int ps, int pc, int pld, double X, double Y,
                           double relHt, double mult, double wPset, double wPmod, double wazm, double Fahs, double Xmax, double Xmin,
                           unsigned int icon, unsigned int dir, int u_Ht, int u_XY, int u_dP, int u_F, int cfd, std::string cfd_name, int cfd_ptype,
                           int cfd_btype, int cfd_capp)
    : m_impl(std::shared_ptr<detail::AirflowPathImpl>(new detail::AirflowPathImpl(nr, flags, pzn, pzm, pe, pf, pw, pa, ps, pc, pld, X, Y, relHt, mult,
                                                                                  wPset, wPmod, wazm, Fahs, Xmax, Xmin, icon, dir, u_Ht, u_XY, u_dP,
                                                                                  u_F, cfd, cfd_name, cfd_ptype, cfd_btype, cfd_capp))) {}

  AirflowPath::AirflowPath(const AirflowPath& other) = default;

  AirflowPath& AirflowPath::operator=(const AirflowPath& other) = default;

  bool AirflowPath::operator==(const AirflowPath& other) const {
    return m_impl == other.m_impl;
  }

  bool AirflowPath::operator!=(const AirflowPath& other) const {
    return m_impl != other.m_impl;
  }

  void AirflowPath::read(Reader& input) {
    m_impl->read(input);
  }

  std::string AirflowPath::write() {
    return m_impl->write();
  }

  int AirflowPath::nr() const {
    return m_impl->nr();
  }

  void AirflowPath::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  int AirflowPath::flags() const {
    return m_impl->flags();
  }

  void AirflowPath::setFlags(const int flags) {
    m_impl->setFlags(flags);
  }

  int AirflowPath::pzn() const {
    return m_impl->pzn();
  }

  void AirflowPath::setPzn(const int pzn) {
    m_impl->setPzn(pzn);
  }

  int AirflowPath::pzm() const {
    return m_impl->pzm();
  }

  void AirflowPath::setPzm(const int pzm) {
    m_impl->setPzm(pzm);
  }

  int AirflowPath::pe() const {
    return m_impl->pe();
  }

  void AirflowPath::setPe(const int pe) {
    m_impl->setPe(pe);
  }

  int AirflowPath::pf() const {
    return m_impl->pf();
  }

  void AirflowPath::setPf(const int pf) {
    m_impl->setPf(pf);
  }

  int AirflowPath::pw() const {
    return m_impl->pw();
  }

  void AirflowPath::setPw(const int pw) {
    m_impl->setPw(pw);
  }

  int AirflowPath::pa() const {
    return m_impl->pa();
  }

  void AirflowPath::setPa(const int pa) {
    m_impl->setPa(pa);
  }

  int AirflowPath::ps() const {
    return m_impl->ps();
  }

  void AirflowPath::setPs(const int ps) {
    m_impl->setPs(ps);
  }

  int AirflowPath::pc() const {
    return m_impl->pc();
  }

  void AirflowPath::setPc(const int pc) {
    m_impl->setPc(pc);
  }

  int AirflowPath::pld() const {
    return m_impl->pld();
  }

  void AirflowPath::setPld(const int pld) {
    m_impl->setPld(pld);
  }

  double AirflowPath::X() const {
    return m_impl->X();
  }

  bool AirflowPath::setX(const double X) {
    return m_impl->setX(X);
  }

  bool AirflowPath::setX(const std::string& X) {
    return m_impl->setX(X);
  }

  double AirflowPath::Y() const {
    return m_impl->Y();
  }

  bool AirflowPath::setY(const double Y) {
    return m_impl->setY(Y);
  }

  bool AirflowPath::setY(const std::string& Y) {
    return m_impl->setY(Y);
  }

  double AirflowPath::relHt() const {
    return m_impl->relHt();
  }

  bool AirflowPath::setRelHt(const std::string& relHt) {
    return m_impl->setRelHt(relHt);
  }

  bool AirflowPath::setRelHt(const double relHt) {
    return m_impl->setRelHt(relHt);
  }

  double AirflowPath::mult() const {
    return m_impl->mult();
  }

  bool AirflowPath::setMult(const double mult) {
    return m_impl->setMult(mult);
  }

  bool AirflowPath::setMult(const std::string& mult) {
    return m_impl->setMult(mult);
  }

  double AirflowPath::wPset() const {
    return m_impl->wPset();
  }

  bool AirflowPath::setWPset(const double wPset) {
    return m_impl->setWPset(wPset);
  }

  bool AirflowPath::setWPset(const std::string& wPset) {
    return m_impl->setWPset(wPset);
  }

  double AirflowPath::wPmod() const {
    return m_impl->wPmod();
  }

  bool AirflowPath::setWPmod(const double wPmod) {
    return m_impl->setWPmod(wPmod);
  }

  bool AirflowPath::setWPmod(const std::string& wPmod) {
    return m_impl->setWPmod(wPmod);
  }

  double AirflowPath::wazm() const {
    return m_impl->wazm();
  }

  bool AirflowPath::setWazm(const double wazm) {
    return m_impl->setWazm(wazm);
  }

  bool AirflowPath::setWazm(const std::string& wazm) {
    return m_impl->setWazm(wazm);
  }

  double AirflowPath::Fahs() const {
    return m_impl->Fahs();
  }

  bool AirflowPath::setFahs(const double Fahs) {
    return m_impl->setFahs(Fahs);
  }

  bool AirflowPath::setFahs(const std::string& Fahs) {
    return m_impl->setFahs(Fahs);
  }

  double AirflowPath::Xmax() const {
    return m_impl->Xmax();
  }

  bool AirflowPath::setXmax(const double Xmax) {
    return m_impl->setXmax(Xmax);
  }

  bool AirflowPath::setXmax(const std::string& Xmax) {
    return m_impl->setXmax(Xmax);
  }

  double AirflowPath::Xmin() const {
    return m_impl->Xmin();
  }

  bool AirflowPath::setXmin(const double Xmin) {
    return m_impl->setXmin(Xmin);
  }

  bool AirflowPath::setXmin(const std::string& Xmin) {
    return m_impl->setXmin(Xmin);
  }

  unsigned int AirflowPath::icon() const {
    return m_impl->icon();
  }

  void AirflowPath::setIcon(const unsigned int icon) {
    m_impl->setIcon(icon);
  }

  unsigned int AirflowPath::dir() const {
    return m_impl->dir();
  }

  void AirflowPath::setDir(const unsigned int dir) {
    m_impl->setDir(dir);
  }

  int AirflowPath::u_Ht() const {
    return m_impl->u_Ht();
  }

  void AirflowPath::setU_Ht(const int u_Ht) {
    m_impl->setU_Ht(u_Ht);
  }

  int AirflowPath::u_XY() const {
    return m_impl->u_XY();
  }

  void AirflowPath::setU_XY(const int u_XY) {
    m_impl->setU_XY(u_XY);
  }

  int AirflowPath::u_dP() const {
    return m_impl->u_dP();
  }

  void AirflowPath::setU_dP(const int u_dP) {
    m_impl->setU_dP(u_dP);
  }

  int AirflowPath::u_F() const {
    return m_impl->u_F();
  }

  void AirflowPath::setU_F(const int u_F) {
    m_impl->setU_F(u_F);
  }

  int AirflowPath::cfd() const {
    return m_impl->cfd();
  }

  void AirflowPath::setCfd(const int cfd) {
    m_impl->setCfd(cfd);
  }

  std::string AirflowPath::cfd_name() const {
    return m_impl->cfd_name();
  }

  void AirflowPath::setCfd_name(const std::string& cfd_name) {
    m_impl->setCfd_name(cfd_name);
  }

  int AirflowPath::cfd_ptype() const {
    return m_impl->cfd_ptype();
  }

  void AirflowPath::setCfd_ptype(const int cfd_ptype) {
    m_impl->setCfd_ptype(cfd_ptype);
  }

  int AirflowPath::cfd_btype() const {
    return m_impl->cfd_btype();
  }

  void AirflowPath::setCfd_btype(const int cfd_btype) {
    m_impl->setCfd_btype(cfd_btype);
  }

  int AirflowPath::cfd_capp() const {
    return m_impl->cfd_capp();
  }

  void AirflowPath::setCfd_capp(const int cfd_capp) {
    m_impl->setCfd_capp(cfd_capp);
  }

  void AirflowPath::setWindPressure(bool b) {
    m_impl->setWindPressure(b);
  }

  bool AirflowPath::windPressure() {
    return m_impl->windPressure();
  }

  void AirflowPath::setSystem(bool b) {
    m_impl->setSystem(b);
  }

  bool AirflowPath::system() {
    return m_impl->system();
  }

  void AirflowPath::setExhaust(bool b) {
    m_impl->setExhaust(b);
  }

  bool AirflowPath::exhaust() {
    return m_impl->exhaust();
  }

  void AirflowPath::setRecirculation(bool b) {
    m_impl->setRecirculation(b);
  }

  bool AirflowPath::recirculation() {
    return m_impl->recirculation();
  }

  void AirflowPath::setOutsideAir(bool b) {
    m_impl->setOutsideAir(b);
  }

  bool AirflowPath::outsideAir() {
    return m_impl->outsideAir();
  }

  RunControl::RunControl() : m_impl(std::shared_ptr<detail::RunControlImpl>(new detail::RunControlImpl())) {}
  /*
RunControl::RunControl(int sim_af,int afcalc,int afmaxi,double afrcnvg,double afacnvg,double afrelax,
int uac2,double Pres,int uPres,int afslae,int afrseq,int aflmaxi,double aflcnvg,
int aflinit,int Tadj,int sim_mf,int ccmaxi,double ccrcnvg,double ccacnvg,
double ccrelax,int uccc,int mfnmthd,int mfnrseq,int mfnmaxi,double mfnrcnvg,
double mfnacnvg,double mfnrelax,double mfngamma,int uccn,int mftmthd,int mftrseq,
int mftmaxi,double mftrcnvg,double mftacnvg,double mftrelax,double mftgamma,int ucct,
int mfvmthd,int mfvrseq,int mfvmaxi,double mfvrcnvg,double mfvacnvg,double mfvrelax,
int uccv,int mf_solver,int sim_1dz,int sim_1dd,double celldx,int sim_vjt,int udx,
int cvode_mth,double cvode_rcnvg,double cvode_acnvg,double cvode_dtmax,int tsdens,
double tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,int dodMdt,
std::string date_st,std::string time_st,std::string date_0,std::string time_0,std::string date_1,
std::string time_1,std::string time_step,std::string time_list,std::string time_scrn,int restart,
std::string rstdate,std::string rsttime,int list,int doDlg,int pfsave,int zfsave,
int zcsave,int achvol,int achsave,int abwsave,int cbwsave,int expsave,
int ebwsave,int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,
int csmsave,int srfsave,int logsave,std::vector<int> save,std::vector<double> rvals,
int BldgFlowZ,int BldgFlowD,int BldgFlowC,int cfd_ctype,double cfd_convcpl,
int cfd_var,int cfd_zref,int cfd_imax,int cfd_dtcmo)
{
d = new detail::RunControlImpl(sim_af,afcalc,afmaxi,afrcnvg,afacnvg,afrelax,uac2,Pres,uPres,afslae,afrseq,
aflmaxi,aflcnvg,aflinit,Tadj,sim_mf,ccmaxi,ccrcnvg,ccacnvg,ccrelax,uccc,mfnmthd,
mfnrseq,mfnmaxi,mfnrcnvg,mfnacnvg,mfnrelax,mfngamma,uccn,mftmthd,mftrseq,mftmaxi,
mftrcnvg,mftacnvg,mftrelax,mftgamma,ucct,mfvmthd,mfvrseq,mfvmaxi,mfvrcnvg,
mfvacnvg,mfvrelax,uccv,mf_solver,sim_1dz,sim_1dd,celldx,sim_vjt,udx,cvode_mth,
cvode_rcnvg,cvode_acnvg,cvode_dtmax,tsdens,tsrelax,tsmaxi,cnvgSS,densZP,stackD,
dodMdt,date_st,time_st,date_0,time_0,date_1,time_1,time_step,time_list,time_scrn,
restart,rstdate,rsttime,list,doDlg,pfsave,zfsave,zcsave,achvol,achsave,abwsave,
cbwsave,expsave,ebwsave,zaasave,zbwsave,rzfsave,rzmsave,rz1save,csmsave,srfsave,
logsave,save,rvals,BldgFlowZ,BldgFlowD,BldgFlowC,cfd_ctype,cfd_convcpl,cfd_var,
cfd_zref,cfd_imax,cfd_dtcmo);
}

RunControl::RunControl(int sim_af,int afcalc,int afmaxi,std::string afrcnvg,std::string afacnvg,std::string afrelax,
int uac2,std::string Pres,int uPres,int afslae,int afrseq,int aflmaxi,std::string aflcnvg,
int aflinit,int Tadj,int sim_mf,int ccmaxi,std::string ccrcnvg,std::string ccacnvg,
std::string ccrelax,int uccc,int mfnmthd,int mfnrseq,int mfnmaxi,std::string mfnrcnvg,
std::string mfnacnvg,std::string mfnrelax,std::string mfngamma,int uccn,int mftmthd,int mftrseq,
int mftmaxi,std::string mftrcnvg,std::string mftacnvg,std::string mftrelax,std::string mftgamma,
int ucct,int mfvmthd,int mfvrseq,int mfvmaxi,std::string mfvrcnvg,std::string mfvacnvg,
std::string mfvrelax,int uccv,int mf_solver,int sim_1dz,int sim_1dd,std::string celldx,
int sim_vjt,int udx,int cvode_mth,std::string cvode_rcnvg,std::string cvode_acnvg,
std::string cvode_dtmax,int tsdens,std::string tsrelax,int tsmaxi,int cnvgSS,int densZP,
int stackD,int dodMdt,std::string date_st,std::string time_st,std::string date_0,std::string time_0,
std::string date_1,std::string time_1,std::string time_step,std::string time_list,std::string time_scrn,
int restart,std::string rstdate,std::string rsttime,int list,int doDlg,int pfsave,int zfsave,
int zcsave,int achvol,int achsave,int abwsave,int cbwsave,int expsave,int ebwsave,
int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,int csmsave,int srfsave,
int logsave,std::vector<int> save,std::vector<std::string> rvals,int BldgFlowZ,int BldgFlowD,
int BldgFlowC,int cfd_ctype,std::string cfd_convcpl,int cfd_var,int cfd_zref,
int cfd_imax,int cfd_dtcmo)
{
d = new detail::RunControlImpl(sim_af,afcalc,afmaxi,afrcnvg,afacnvg,afrelax,uac2,Pres,uPres,afslae,afrseq,
aflmaxi,aflcnvg,aflinit,Tadj,sim_mf,ccmaxi,ccrcnvg,ccacnvg,ccrelax,uccc,mfnmthd,
mfnrseq,mfnmaxi,mfnrcnvg,mfnacnvg,mfnrelax,mfngamma,uccn,mftmthd,mftrseq,mftmaxi,
mftrcnvg,mftacnvg,mftrelax,mftgamma,ucct,mfvmthd,mfvrseq,mfvmaxi,mfvrcnvg,
mfvacnvg,mfvrelax,uccv,mf_solver,sim_1dz,sim_1dd,celldx,sim_vjt,udx,cvode_mth,
cvode_rcnvg,cvode_acnvg,cvode_dtmax,tsdens,tsrelax,tsmaxi,cnvgSS,densZP,stackD,
dodMdt,date_st,time_st,date_0,time_0,date_1,time_1,time_step,time_list,time_scrn,
restart,rstdate,rsttime,list,doDlg,pfsave,zfsave,zcsave,achvol,achsave,abwsave,
cbwsave,expsave,ebwsave,zaasave,zbwsave,rzfsave,rzmsave,rz1save,csmsave,srfsave,
logsave,save,rvals,BldgFlowZ,BldgFlowD,BldgFlowC,cfd_ctype,cfd_convcpl,cfd_var,
cfd_zref,cfd_imax,cfd_dtcmo);
}*/

  RunControl::RunControl(const RunControl& other) = default;

  RunControl& RunControl::operator=(const RunControl& other) = default;

  bool RunControl::operator==(const RunControl& other) const {
    return m_impl == other.m_impl;
  }

  bool RunControl::operator!=(const RunControl& other) const {
    return m_impl != other.m_impl;
  }

  std::string RunControl::write() {
    return m_impl->write();
  }

  void RunControl::read(Reader& input) {
    m_impl->read(input);
  }

  int RunControl::sim_af() const {
    return m_impl->sim_af();
  }

  void RunControl::setSim_af(const int sim_af) {
    m_impl->setSim_af(sim_af);
  }

  int RunControl::afcalc() const {
    return m_impl->afcalc();
  }

  void RunControl::setAfcalc(const int afcalc) {
    m_impl->setAfcalc(afcalc);
  }

  int RunControl::afmaxi() const {
    return m_impl->afmaxi();
  }

  void RunControl::setAfmaxi(const int afmaxi) {
    m_impl->setAfmaxi(afmaxi);
  }

  double RunControl::afrcnvg() const {
    return m_impl->afrcnvg();
  }

  bool RunControl::setAfrcnvg(const double afrcnvg) {
    return m_impl->setAfrcnvg(afrcnvg);
  }

  bool RunControl::setAfrcnvg(const std::string& afrcnvg) {
    return m_impl->setAfrcnvg(afrcnvg);
  }

  double RunControl::afacnvg() const {
    return m_impl->afacnvg();
  }

  bool RunControl::setAfacnvg(const double afacnvg) {
    return m_impl->setAfacnvg(afacnvg);
  }

  bool RunControl::setAfacnvg(const std::string& afacnvg) {
    return m_impl->setAfacnvg(afacnvg);
  }

  double RunControl::afrelax() const {
    return m_impl->afrelax();
  }

  bool RunControl::setAfrelax(const double afrelax) {
    return m_impl->setAfrelax(afrelax);
  }

  bool RunControl::setAfrelax(const std::string& afrelax) {
    return m_impl->setAfrelax(afrelax);
  }

  int RunControl::uac2() const {
    return m_impl->uac2();
  }

  void RunControl::setUac2(const int uac2) {
    m_impl->setUac2(uac2);
  }

  double RunControl::Pres() const {
    return m_impl->Pres();
  }

  bool RunControl::setPres(const double Pres) {
    return m_impl->setPres(Pres);
  }

  bool RunControl::setPres(const std::string& Pres) {
    return m_impl->setPres(Pres);
  }

  int RunControl::uPres() const {
    return m_impl->uPres();
  }

  void RunControl::setUPres(const int uPres) {
    m_impl->setUPres(uPres);
  }

  int RunControl::afslae() const {
    return m_impl->afslae();
  }

  void RunControl::setAfslae(const int afslae) {
    m_impl->setAfslae(afslae);
  }

  int RunControl::afrseq() const {
    return m_impl->afrseq();
  }

  void RunControl::setAfrseq(const int afrseq) {
    m_impl->setAfrseq(afrseq);
  }

  int RunControl::aflmaxi() const {
    return m_impl->aflmaxi();
  }

  void RunControl::setAflmaxi(const int aflmaxi) {
    m_impl->setAflmaxi(aflmaxi);
  }

  double RunControl::aflcnvg() const {
    return m_impl->aflcnvg();
  }

  bool RunControl::setAflcnvg(const double aflcnvg) {
    return m_impl->setAflcnvg(aflcnvg);
  }

  bool RunControl::setAflcnvg(const std::string& aflcnvg) {
    return m_impl->setAflcnvg(aflcnvg);
  }

  int RunControl::aflinit() const {
    return m_impl->aflinit();
  }

  void RunControl::setAflinit(const int aflinit) {
    m_impl->setAflinit(aflinit);
  }

  int RunControl::Tadj() const {
    return m_impl->Tadj();
  }

  void RunControl::setTadj(const int Tadj) {
    m_impl->setTadj(Tadj);
  }

  int RunControl::sim_mf() const {
    return m_impl->sim_mf();
  }

  void RunControl::setSim_mf(const int sim_mf) {
    m_impl->setSim_mf(sim_mf);
  }

  int RunControl::ccmaxi() const {
    return m_impl->ccmaxi();
  }

  void RunControl::setCcmaxi(const int ccmaxi) {
    m_impl->setCcmaxi(ccmaxi);
  }

  double RunControl::ccrcnvg() const {
    return m_impl->ccrcnvg();
  }

  bool RunControl::setCcrcnvg(const double ccrcnvg) {
    return m_impl->setCcrcnvg(ccrcnvg);
  }

  bool RunControl::setCcrcnvg(const std::string& ccrcnvg) {
    return m_impl->setCcrcnvg(ccrcnvg);
  }

  double RunControl::ccacnvg() const {
    return m_impl->ccacnvg();
  }

  bool RunControl::setCcacnvg(const double ccacnvg) {
    return m_impl->setCcacnvg(ccacnvg);
  }

  bool RunControl::setCcacnvg(const std::string& ccacnvg) {
    return m_impl->setCcacnvg(ccacnvg);
  }

  double RunControl::ccrelax() const {
    return m_impl->ccrelax();
  }

  bool RunControl::setCcrelax(const double ccrelax) {
    return m_impl->setCcrelax(ccrelax);
  }

  bool RunControl::setCcrelax(const std::string& ccrelax) {
    return m_impl->setCcrelax(ccrelax);
  }

  int RunControl::uccc() const {
    return m_impl->uccc();
  }

  void RunControl::setUccc(const int uccc) {
    m_impl->setUccc(uccc);
  }

  int RunControl::mfnmthd() const {
    return m_impl->mfnmthd();
  }

  void RunControl::setMfnmthd(const int mfnmthd) {
    m_impl->setMfnmthd(mfnmthd);
  }

  int RunControl::mfnrseq() const {
    return m_impl->mfnrseq();
  }

  void RunControl::setMfnrseq(const int mfnrseq) {
    m_impl->setMfnrseq(mfnrseq);
  }

  int RunControl::mfnmaxi() const {
    return m_impl->mfnmaxi();
  }

  void RunControl::setMfnmaxi(const int mfnmaxi) {
    m_impl->setMfnmaxi(mfnmaxi);
  }

  double RunControl::mfnrcnvg() const {
    return m_impl->mfnrcnvg();
  }

  bool RunControl::setMfnrcnvg(const double mfnrcnvg) {
    return m_impl->setMfnrcnvg(mfnrcnvg);
  }

  bool RunControl::setMfnrcnvg(const std::string& mfnrcnvg) {
    return m_impl->setMfnrcnvg(mfnrcnvg);
  }

  double RunControl::mfnacnvg() const {
    return m_impl->mfnacnvg();
  }

  bool RunControl::setMfnacnvg(const double mfnacnvg) {
    return m_impl->setMfnacnvg(mfnacnvg);
  }

  bool RunControl::setMfnacnvg(const std::string& mfnacnvg) {
    return m_impl->setMfnacnvg(mfnacnvg);
  }

  double RunControl::mfnrelax() const {
    return m_impl->mfnrelax();
  }

  bool RunControl::setMfnrelax(const double mfnrelax) {
    return m_impl->setMfnrelax(mfnrelax);
  }

  bool RunControl::setMfnrelax(const std::string& mfnrelax) {
    return m_impl->setMfnrelax(mfnrelax);
  }

  double RunControl::mfngamma() const {
    return m_impl->mfngamma();
  }

  bool RunControl::setMfngamma(const double mfngamma) {
    return m_impl->setMfngamma(mfngamma);
  }

  bool RunControl::setMfngamma(const std::string& mfngamma) {
    return m_impl->setMfngamma(mfngamma);
  }

  int RunControl::uccn() const {
    return m_impl->uccn();
  }

  void RunControl::setUccn(const int uccn) {
    m_impl->setUccn(uccn);
  }

  int RunControl::mftmthd() const {
    return m_impl->mftmthd();
  }

  void RunControl::setMftmthd(const int mftmthd) {
    m_impl->setMftmthd(mftmthd);
  }

  int RunControl::mftrseq() const {
    return m_impl->mftrseq();
  }

  void RunControl::setMftrseq(const int mftrseq) {
    m_impl->setMftrseq(mftrseq);
  }

  int RunControl::mftmaxi() const {
    return m_impl->mftmaxi();
  }

  void RunControl::setMftmaxi(const int mftmaxi) {
    m_impl->setMftmaxi(mftmaxi);
  }

  double RunControl::mftrcnvg() const {
    return m_impl->mftrcnvg();
  }

  bool RunControl::setMftrcnvg(const double mftrcnvg) {
    return m_impl->setMftrcnvg(mftrcnvg);
  }

  bool RunControl::setMftrcnvg(const std::string& mftrcnvg) {
    return m_impl->setMftrcnvg(mftrcnvg);
  }

  double RunControl::mftacnvg() const {
    return m_impl->mftacnvg();
  }

  bool RunControl::setMftacnvg(const double mftacnvg) {
    return m_impl->setMftacnvg(mftacnvg);
  }

  bool RunControl::setMftacnvg(const std::string& mftacnvg) {
    return m_impl->setMftacnvg(mftacnvg);
  }

  double RunControl::mftrelax() const {
    return m_impl->mftrelax();
  }

  bool RunControl::setMftrelax(const double mftrelax) {
    return m_impl->setMftrelax(mftrelax);
  }

  bool RunControl::setMftrelax(const std::string& mftrelax) {
    return m_impl->setMftrelax(mftrelax);
  }

  double RunControl::mftgamma() const {
    return m_impl->mftgamma();
  }

  bool RunControl::setMftgamma(const double mftgamma) {
    return m_impl->setMftgamma(mftgamma);
  }

  bool RunControl::setMftgamma(const std::string& mftgamma) {
    return m_impl->setMftgamma(mftgamma);
  }

  int RunControl::ucct() const {
    return m_impl->ucct();
  }

  void RunControl::setUcct(const int ucct) {
    m_impl->setUcct(ucct);
  }

  int RunControl::mfvmthd() const {
    return m_impl->mfvmthd();
  }

  void RunControl::setMfvmthd(const int mfvmthd) {
    m_impl->setMfvmthd(mfvmthd);
  }

  int RunControl::mfvrseq() const {
    return m_impl->mfvrseq();
  }

  void RunControl::setMfvrseq(const int mfvrseq) {
    m_impl->setMfvrseq(mfvrseq);
  }

  int RunControl::mfvmaxi() const {
    return m_impl->mfvmaxi();
  }

  void RunControl::setMfvmaxi(const int mfvmaxi) {
    m_impl->setMfvmaxi(mfvmaxi);
  }

  double RunControl::mfvrcnvg() const {
    return m_impl->mfvrcnvg();
  }

  bool RunControl::setMfvrcnvg(const double mfvrcnvg) {
    return m_impl->setMfvrcnvg(mfvrcnvg);
  }

  bool RunControl::setMfvrcnvg(const std::string& mfvrcnvg) {
    return m_impl->setMfvrcnvg(mfvrcnvg);
  }

  double RunControl::mfvacnvg() const {
    return m_impl->mfvacnvg();
  }

  bool RunControl::setMfvacnvg(const double mfvacnvg) {
    return m_impl->setMfvacnvg(mfvacnvg);
  }

  bool RunControl::setMfvacnvg(const std::string& mfvacnvg) {
    return m_impl->setMfvacnvg(mfvacnvg);
  }

  double RunControl::mfvrelax() const {
    return m_impl->mfvrelax();
  }

  bool RunControl::setMfvrelax(const double mfvrelax) {
    return m_impl->setMfvrelax(mfvrelax);
  }

  bool RunControl::setMfvrelax(const std::string& mfvrelax) {
    return m_impl->setMfvrelax(mfvrelax);
  }

  int RunControl::uccv() const {
    return m_impl->uccv();
  }

  void RunControl::setUccv(const int uccv) {
    m_impl->setUccv(uccv);
  }

  int RunControl::mf_solver() const {
    return m_impl->mf_solver();
  }

  void RunControl::setMf_solver(const int mf_solver) {
    m_impl->setMf_solver(mf_solver);
  }

  int RunControl::sim_1dz() const {
    return m_impl->sim_1dz();
  }

  void RunControl::setSim_1dz(const int sim_1dz) {
    m_impl->setSim_1dz(sim_1dz);
  }

  int RunControl::sim_1dd() const {
    return m_impl->sim_1dd();
  }

  void RunControl::setSim_1dd(const int sim_1dd) {
    m_impl->setSim_1dd(sim_1dd);
  }

  double RunControl::celldx() const {
    return m_impl->celldx();
  }

  bool RunControl::setCelldx(const double celldx) {
    return m_impl->setCelldx(celldx);
  }

  bool RunControl::setCelldx(const std::string& celldx) {
    return m_impl->setCelldx(celldx);
  }

  int RunControl::sim_vjt() const {
    return m_impl->sim_vjt();
  }

  void RunControl::setSim_vjt(const int sim_vjt) {
    m_impl->setSim_vjt(sim_vjt);
  }

  int RunControl::udx() const {
    return m_impl->udx();
  }

  void RunControl::setUdx(const int udx) {
    m_impl->setUdx(udx);
  }

  int RunControl::cvode_mth() const {
    return m_impl->cvode_mth();
  }

  void RunControl::setCvode_mth(const int cvode_mth) {
    m_impl->setCvode_mth(cvode_mth);
  }

  double RunControl::cvode_rcnvg() const {
    return m_impl->cvode_rcnvg();
  }

  bool RunControl::setCvode_rcnvg(const double cvode_rcnvg) {
    return m_impl->setCvode_rcnvg(cvode_rcnvg);
  }

  bool RunControl::setCvode_rcnvg(const std::string& cvode_rcnvg) {
    return m_impl->setCvode_rcnvg(cvode_rcnvg);
  }

  double RunControl::cvode_acnvg() const {
    return m_impl->cvode_acnvg();
  }

  bool RunControl::setCvode_acnvg(const double cvode_acnvg) {
    return m_impl->setCvode_acnvg(cvode_acnvg);
  }

  bool RunControl::setCvode_acnvg(const std::string& cvode_acnvg) {
    return m_impl->setCvode_acnvg(cvode_acnvg);
  }

  double RunControl::cvode_dtmax() const {
    return m_impl->cvode_dtmax();
  }

  bool RunControl::setCvode_dtmax(const double cvode_dtmax) {
    return m_impl->setCvode_dtmax(cvode_dtmax);
  }

  bool RunControl::setCvode_dtmax(const std::string& cvode_dtmax) {
    return m_impl->setCvode_dtmax(cvode_dtmax);
  }

  int RunControl::tsdens() const {
    return m_impl->tsdens();
  }

  void RunControl::setTsdens(const int tsdens) {
    m_impl->setTsdens(tsdens);
  }

  double RunControl::tsrelax() const {
    return m_impl->tsrelax();
  }

  bool RunControl::setTsrelax(const double tsrelax) {
    return m_impl->setTsrelax(tsrelax);
  }

  bool RunControl::setTsrelax(const std::string& tsrelax) {
    return m_impl->setTsrelax(tsrelax);
  }

  int RunControl::tsmaxi() const {
    return m_impl->tsmaxi();
  }

  void RunControl::setTsmaxi(const int tsmaxi) {
    m_impl->setTsmaxi(tsmaxi);
  }

  int RunControl::cnvgSS() const {
    return m_impl->cnvgSS();
  }

  void RunControl::setCnvgSS(const int cnvgSS) {
    m_impl->setCnvgSS(cnvgSS);
  }

  int RunControl::densZP() const {
    return m_impl->densZP();
  }

  void RunControl::setDensZP(const int densZP) {
    m_impl->setDensZP(densZP);
  }

  int RunControl::stackD() const {
    return m_impl->stackD();
  }

  void RunControl::setStackD(const int stackD) {
    m_impl->setStackD(stackD);
  }

  int RunControl::dodMdt() const {
    return m_impl->dodMdt();
  }

  void RunControl::setDodMdt(const int dodMdt) {
    m_impl->setDodMdt(dodMdt);
  }

  std::string RunControl::date_st() const {
    return m_impl->date_st();
  }

  void RunControl::setDate_st(const std::string& date_st) {
    m_impl->setDate_st(date_st);
  }

  std::string RunControl::time_st() const {
    return m_impl->time_st();
  }

  void RunControl::setTime_st(const std::string& time_st) {
    m_impl->setTime_st(time_st);
  }

  std::string RunControl::date_0() const {
    return m_impl->date_0();
  }

  void RunControl::setDate_0(const std::string& date_0) {
    m_impl->setDate_0(date_0);
  }

  std::string RunControl::time_0() const {
    return m_impl->time_0();
  }

  void RunControl::setTime_0(const std::string& time_0) {
    m_impl->setTime_0(time_0);
  }

  std::string RunControl::date_1() const {
    return m_impl->date_1();
  }

  void RunControl::setDate_1(const std::string& date_1) {
    m_impl->setDate_1(date_1);
  }

  std::string RunControl::time_1() const {
    return m_impl->time_1();
  }

  void RunControl::setTime_1(const std::string& time_1) {
    m_impl->setTime_1(time_1);
  }

  std::string RunControl::time_step() const {
    return m_impl->time_step();
  }

  void RunControl::setTime_step(const std::string& time_step) {
    m_impl->setTime_step(time_step);
  }

  std::string RunControl::time_list() const {
    return m_impl->time_list();
  }

  void RunControl::setTime_list(const std::string& time_list) {
    m_impl->setTime_list(time_list);
  }

  std::string RunControl::time_scrn() const {
    return m_impl->time_scrn();
  }

  void RunControl::setTime_scrn(const std::string& time_scrn) {
    m_impl->setTime_scrn(time_scrn);
  }

  int RunControl::restart() const {
    return m_impl->restart();
  }

  void RunControl::setRestart(const int restart) {
    m_impl->setRestart(restart);
  }

  std::string RunControl::rstdate() const {
    return m_impl->rstdate();
  }

  void RunControl::setRstdate(const std::string& rstdate) {
    m_impl->setRstdate(rstdate);
  }

  std::string RunControl::rsttime() const {
    return m_impl->rsttime();
  }

  void RunControl::setRsttime(const std::string& rsttime) {
    m_impl->setRsttime(rsttime);
  }

  int RunControl::list() const {
    return m_impl->list();
  }

  void RunControl::setList(const int list) {
    m_impl->setList(list);
  }

  int RunControl::doDlg() const {
    return m_impl->doDlg();
  }

  void RunControl::setDoDlg(const int doDlg) {
    m_impl->setDoDlg(doDlg);
  }

  int RunControl::pfsave() const {
    return m_impl->pfsave();
  }

  void RunControl::setPfsave(const int pfsave) {
    m_impl->setPfsave(pfsave);
  }

  int RunControl::zfsave() const {
    return m_impl->zfsave();
  }

  void RunControl::setZfsave(const int zfsave) {
    m_impl->setZfsave(zfsave);
  }

  int RunControl::zcsave() const {
    return m_impl->zcsave();
  }

  void RunControl::setZcsave(const int zcsave) {
    m_impl->setZcsave(zcsave);
  }

  int RunControl::achvol() const {
    return m_impl->achvol();
  }

  void RunControl::setAchvol(const int achvol) {
    m_impl->setAchvol(achvol);
  }

  int RunControl::achsave() const {
    return m_impl->achsave();
  }

  void RunControl::setAchsave(const int achsave) {
    m_impl->setAchsave(achsave);
  }

  int RunControl::abwsave() const {
    return m_impl->abwsave();
  }

  void RunControl::setAbwsave(const int abwsave) {
    m_impl->setAbwsave(abwsave);
  }

  int RunControl::cbwsave() const {
    return m_impl->cbwsave();
  }

  void RunControl::setCbwsave(const int cbwsave) {
    m_impl->setCbwsave(cbwsave);
  }

  int RunControl::expsave() const {
    return m_impl->expsave();
  }

  void RunControl::setExpsave(const int expsave) {
    m_impl->setExpsave(expsave);
  }

  int RunControl::ebwsave() const {
    return m_impl->ebwsave();
  }

  void RunControl::setEbwsave(const int ebwsave) {
    m_impl->setEbwsave(ebwsave);
  }

  int RunControl::zaasave() const {
    return m_impl->zaasave();
  }

  void RunControl::setZaasave(const int zaasave) {
    m_impl->setZaasave(zaasave);
  }

  int RunControl::zbwsave() const {
    return m_impl->zbwsave();
  }

  void RunControl::setZbwsave(const int zbwsave) {
    m_impl->setZbwsave(zbwsave);
  }

  int RunControl::rzfsave() const {
    return m_impl->rzfsave();
  }

  void RunControl::setRzfsave(const int rzfsave) {
    m_impl->setRzfsave(rzfsave);
  }

  int RunControl::rzmsave() const {
    return m_impl->rzmsave();
  }

  void RunControl::setRzmsave(const int rzmsave) {
    m_impl->setRzmsave(rzmsave);
  }

  int RunControl::rz1save() const {
    return m_impl->rz1save();
  }

  void RunControl::setRz1save(const int rz1save) {
    m_impl->setRz1save(rz1save);
  }

  int RunControl::csmsave() const {
    return m_impl->csmsave();
  }

  void RunControl::setCsmsave(const int csmsave) {
    m_impl->setCsmsave(csmsave);
  }

  int RunControl::srfsave() const {
    return m_impl->srfsave();
  }

  void RunControl::setSrfsave(const int srfsave) {
    m_impl->setSrfsave(srfsave);
  }

  int RunControl::logsave() const {
    return m_impl->logsave();
  }

  void RunControl::setLogsave(const int logsave) {
    m_impl->setLogsave(logsave);
  }

  std::vector<int> RunControl::save() const {
    return m_impl->save();
  }

  void RunControl::setSave(const std::vector<int>& save) {
    m_impl->setSave(save);
  }

  std::vector<double> RunControl::rvals() const {
    return m_impl->rvals();
  }

  bool RunControl::setRvals(const std::vector<double>& rvals) {
    return m_impl->setRvals(rvals);
  }

  bool RunControl::setRvals(const std::vector<std::string>& rvals) {
    return m_impl->setRvals(rvals);
  }

  int RunControl::BldgFlowZ() const {
    return m_impl->BldgFlowZ();
  }

  void RunControl::setBldgFlowZ(const int BldgFlowZ) {
    m_impl->setBldgFlowZ(BldgFlowZ);
  }

  int RunControl::BldgFlowD() const {
    return m_impl->BldgFlowD();
  }

  void RunControl::setBldgFlowD(const int BldgFlowD) {
    m_impl->setBldgFlowD(BldgFlowD);
  }

  int RunControl::BldgFlowC() const {
    return m_impl->BldgFlowC();
  }

  void RunControl::setBldgFlowC(const int BldgFlowC) {
    m_impl->setBldgFlowC(BldgFlowC);
  }

  int RunControl::cfd_ctype() const {
    return m_impl->cfd_ctype();
  }

  void RunControl::setCfd_ctype(const int cfd_ctype) {
    m_impl->setCfd_ctype(cfd_ctype);
  }

  double RunControl::cfd_convcpl() const {
    return m_impl->cfd_convcpl();
  }

  bool RunControl::setCfd_convcpl(const double cfd_convcpl) {
    return m_impl->setCfd_convcpl(cfd_convcpl);
  }

  bool RunControl::setCfd_convcpl(const std::string& cfd_convcpl) {
    return m_impl->setCfd_convcpl(cfd_convcpl);
  }

  int RunControl::cfd_var() const {
    return m_impl->cfd_var();
  }

  void RunControl::setCfd_var(const int cfd_var) {
    m_impl->setCfd_var(cfd_var);
  }

  int RunControl::cfd_zref() const {
    return m_impl->cfd_zref();
  }

  void RunControl::setCfd_zref(const int cfd_zref) {
    m_impl->setCfd_zref(cfd_zref);
  }

  int RunControl::cfd_imax() const {
    return m_impl->cfd_imax();
  }

  void RunControl::setCfd_imax(const int cfd_imax) {
    m_impl->setCfd_imax(cfd_imax);
  }

  int RunControl::cfd_dtcmo() const {
    return m_impl->cfd_dtcmo();
  }

  void RunControl::setCfd_dtcmo(const int cfd_dtcmo) {
    m_impl->setCfd_dtcmo(cfd_dtcmo);
  }

  Level::Level() : m_impl(std::shared_ptr<detail::LevelImpl>(new detail::LevelImpl)) {}

  Level::Level(double delht, std::string name) : m_impl(std::shared_ptr<detail::LevelImpl>(new detail::LevelImpl)) {
    m_impl->setDelht(delht);
    m_impl->setName(name);
  }

  Level::Level(std::string delht, std::string name) : m_impl(std::shared_ptr<detail::LevelImpl>(new detail::LevelImpl)) {
    m_impl->setDelht(delht);
    m_impl->setName(name);
  }

  Level::Level(int nr, double refht, double delht, int u_rfht, int u_dlht, std::string name, std::vector<Icon> icons)
    : m_impl(std::shared_ptr<detail::LevelImpl>(new detail::LevelImpl(nr, refht, delht, u_rfht, u_dlht, name, icons))) {}

  Level::Level(int nr, std::string refht, std::string delht, int u_rfht, int u_dlht, std::string name, std::vector<Icon> icons)
    : m_impl(std::shared_ptr<detail::LevelImpl>(new detail::LevelImpl(nr, refht, delht, u_rfht, u_dlht, name, icons))) {}

  Level::Level(const Level& other) = default;

  Level& Level::operator=(const Level& other) = default;

  bool Level::operator==(const Level& other) const {
    return m_impl == other.m_impl;
  }

  bool Level::operator!=(const Level& other) const {
    return m_impl != other.m_impl;
  }

  std::string Level::write() {
    return m_impl->write();
  }

  void Level::read(Reader& input) {
    m_impl->read(input);
  }

  int Level::nr() const {
    return m_impl->nr();
  }

  void Level::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  double Level::refht() const {
    return m_impl->refht();
  }

  bool Level::setRefht(const double refht) {
    return m_impl->setRefht(refht);
  }

  bool Level::setRefht(const std::string& refht) {
    return m_impl->setRefht(refht);
  }

  double Level::delht() const {
    return m_impl->delht();
  }

  bool Level::setDelht(const double delht) {
    return m_impl->setDelht(delht);
  }

  bool Level::setDelht(const std::string& delht) {
    return m_impl->setDelht(delht);
  }

  int Level::u_rfht() const {
    return m_impl->u_rfht();
  }

  void Level::setU_rfht(const int u_rfht) {
    m_impl->setU_rfht(u_rfht);
  }

  int Level::u_dlht() const {
    return m_impl->u_dlht();
  }

  void Level::setU_dlht(const int u_dlht) {
    m_impl->setU_dlht(u_dlht);
  }

  std::string Level::name() const {
    return m_impl->name();
  }

  void Level::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::vector<Icon> Level::icons() const {
    return m_impl->icons();
  }

  void Level::setIcons(const std::vector<Icon>& icons) {
    m_impl->setIcons(icons);
  }

  DaySchedule::DaySchedule() : m_impl(std::shared_ptr<detail::DayScheduleImpl>(new detail::DayScheduleImpl())) {}

  DaySchedule::DaySchedule(int nr, int shape, int utyp, int ucnv, std::string name, std::string desc, std::vector<SchedulePoint> points)
    : m_impl(std::shared_ptr<detail::DayScheduleImpl>(new detail::DayScheduleImpl(nr, shape, utyp, ucnv, name, desc, points))) {}

  DaySchedule::DaySchedule(const DaySchedule& other) = default;

  DaySchedule& DaySchedule::operator=(const DaySchedule& other) = default;

  bool DaySchedule::operator==(const DaySchedule& other) const {
    return m_impl == other.m_impl;
  }

  bool DaySchedule::operator!=(const DaySchedule& other) const {
    return m_impl != other.m_impl;
  }

  std::string DaySchedule::write() {
    return m_impl->write();
  }

  void DaySchedule::read(Reader& input) {
    m_impl->read(input);
  }

  int DaySchedule::nr() const {
    return m_impl->nr();
  }

  void DaySchedule::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  int DaySchedule::shape() const {
    return m_impl->shape();
  }

  void DaySchedule::setShape(const int shape) {
    m_impl->setShape(shape);
  }

  int DaySchedule::utyp() const {
    return m_impl->utyp();
  }

  void DaySchedule::setUtyp(const int utyp) {
    m_impl->setUtyp(utyp);
  }

  int DaySchedule::ucnv() const {
    return m_impl->ucnv();
  }

  void DaySchedule::setUcnv(const int ucnv) {
    m_impl->setUcnv(ucnv);
  }

  std::string DaySchedule::name() const {
    return m_impl->name();
  }

  void DaySchedule::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::string DaySchedule::desc() const {
    return m_impl->desc();
  }

  void DaySchedule::setDesc(const std::string& desc) {
    m_impl->setDesc(desc);
  }

  std::vector<SchedulePoint> DaySchedule::points() const {
    return m_impl->points();
  }

  void DaySchedule::setPoints(const std::vector<SchedulePoint>& points) {
    m_impl->setPoints(points);
  }

  WeekSchedule::WeekSchedule() : m_impl(std::shared_ptr<detail::WeekScheduleImpl>(new detail::WeekScheduleImpl())) {}

  WeekSchedule::WeekSchedule(int nr, int utyp, int ucnv, std::string name, std::string desc, std::vector<int> j)
    : m_impl(std::shared_ptr<detail::WeekScheduleImpl>(new detail::WeekScheduleImpl(nr, utyp, ucnv, name, desc, j))) {}

  WeekSchedule::WeekSchedule(const WeekSchedule& other) = default;

  WeekSchedule& WeekSchedule::operator=(const WeekSchedule& other) = default;

  bool WeekSchedule::operator==(const WeekSchedule& other) const {
    return m_impl == other.m_impl;
  }

  bool WeekSchedule::operator!=(const WeekSchedule& other) const {
    return m_impl != other.m_impl;
  }

  std::string WeekSchedule::write() {
    return m_impl->write();
  }

  void WeekSchedule::read(Reader& input) {
    m_impl->read(input);
  }

  int WeekSchedule::nr() const {
    return m_impl->nr();
  }

  void WeekSchedule::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  int WeekSchedule::utyp() const {
    return m_impl->utyp();
  }

  void WeekSchedule::setUtyp(const int utyp) {
    m_impl->setUtyp(utyp);
  }

  int WeekSchedule::ucnv() const {
    return m_impl->ucnv();
  }

  void WeekSchedule::setUcnv(const int ucnv) {
    m_impl->setUcnv(ucnv);
  }

  std::string WeekSchedule::name() const {
    return m_impl->name();
  }

  void WeekSchedule::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::string WeekSchedule::desc() const {
    return m_impl->desc();
  }

  void WeekSchedule::setDesc(const std::string& desc) {
    m_impl->setDesc(desc);
  }

  std::vector<int> WeekSchedule::j() const {
    return m_impl->j();
  }

  void WeekSchedule::setJ(const std::vector<int>& j) {
    m_impl->setJ(j);
  }

  WindPressureProfile::WindPressureProfile() : m_impl(std::shared_ptr<detail::WindPressureProfileImpl>(new detail::WindPressureProfileImpl())) {}

  WindPressureProfile::WindPressureProfile(int nr, int type, std::string name, std::string desc, std::vector<PressureCoefficientPoint> coeffs)
    : m_impl(std::shared_ptr<detail::WindPressureProfileImpl>(new detail::WindPressureProfileImpl(nr, type, name, desc, coeffs))) {}

  WindPressureProfile::WindPressureProfile(const WindPressureProfile& other) = default;

  WindPressureProfile& WindPressureProfile::operator=(const WindPressureProfile& other) = default;

  bool WindPressureProfile::operator==(const WindPressureProfile& other) const {
    return m_impl == other.m_impl;
  }

  bool WindPressureProfile::operator!=(const WindPressureProfile& other) const {
    return m_impl != other.m_impl;
  }

  std::string WindPressureProfile::write() {
    return m_impl->write();
  }

  void WindPressureProfile::read(Reader& input) {
    m_impl->read(input);
  }

  int WindPressureProfile::nr() const {
    return m_impl->nr();
  }

  void WindPressureProfile::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  int WindPressureProfile::type() const {
    return m_impl->type();
  }

  void WindPressureProfile::setType(const int type) {
    m_impl->setType(type);
  }

  std::string WindPressureProfile::name() const {
    return m_impl->name();
  }

  void WindPressureProfile::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::string WindPressureProfile::desc() const {
    return m_impl->desc();
  }

  void WindPressureProfile::setDesc(const std::string& desc) {
    m_impl->setDesc(desc);
  }

  std::vector<PressureCoefficientPoint> WindPressureProfile::coeffs() const {
    return m_impl->coeffs();
  }

  void WindPressureProfile::setCoeffs(const std::vector<PressureCoefficientPoint>& coeffs) {
    m_impl->setCoeffs(coeffs);
  }

  ControlNodeType ControlNode::convertTag(const std::string& tag) {
    try {
      return {tag};
    } catch (...) {
      return ControlNodeType::UNKNOWN;
    }
  }

  ControlNode* ControlNode::readElement(Reader& input) {
    ControlNode* out = nullptr;
    int nr = input.read<int>();
    std::string dataType = input.readString();
    int seqnr = input.read<int>();
    auto flags = input.read<unsigned int>();
    int inreq = input.read<int>();
    int n1 = input.read<int>();
    int n2 = input.read<int>();
    std::string name = input.readString();
    std::string desc = input.readLine();

    const ControlNodeType kind = convertTag(dataType);
    if (kind == ControlNodeType::UNKNOWN) {
      LOG_FREE_AND_THROW("openstudio.contam.Reader", "Unknown control node type '" << dataType << "' at line " << input.lineNumber());
    } else if (kind == ControlNodeType::CT_CVF) {
      auto* obj = new CvfDat(nr, seqnr, flags, inreq, n1, n2, name, desc);
      obj->readDetails(input);
      out = static_cast<ControlNode*>(obj);
    } else if (kind == ControlNodeType::CT_DVF) {
      auto* obj = new DvfDat(nr, seqnr, flags, inreq, n1, n2, name, desc);
      obj->readDetails(input);
      out = static_cast<ControlNode*>(obj);
    } else {
      LOG_FREE_AND_THROW("openstudio.contam.Reader", "Control node type '" << dataType << "' is not supported.");
    }

    return out;
  }

  CdvDat::CdvDat() : m_impl(std::shared_ptr<detail::CdvDatImpl>(new detail::CdvDatImpl())) {}

  CdvDat::CdvDat(int nr, int seqnr, unsigned int flags, int inreq, int n1, int n2, std::string name, std::string desc)
    : m_impl(std::shared_ptr<detail::CdvDatImpl>(new detail::CdvDatImpl(nr, seqnr, flags, inreq, n1, n2, name, desc))) {}

  CdvDat::CdvDat(int nr, int seqnr, unsigned int flags, int inreq, int n1, int n2, std::string name, std::string desc, std::string valuename)
    : m_impl(std::shared_ptr<detail::CdvDatImpl>(new detail::CdvDatImpl(nr, seqnr, flags, inreq, n1, n2, name, desc, valuename))) {}

  CdvDat::CdvDat(const CdvDat& other) : m_impl(other.m_impl) {}

  CdvDat& CdvDat::operator=(const CdvDat& other) {
    m_impl = other.m_impl;
    return *this;
  }

  bool CdvDat::operator==(const CdvDat& other) const {
    return m_impl == other.m_impl;
  }

  bool CdvDat::operator!=(const CdvDat& other) const {
    return m_impl != other.m_impl;
  }

  std::string CdvDat::write() {
    return m_impl->write(dataType());
  }

  void CdvDat::read(Reader& input) {
    m_impl->read(input);
  }

  void CdvDat::readDetails(Reader& input) {
    m_impl->readDetails(input);
  }

  int CdvDat::nr() const {
    return m_impl->nr();
  }

  void CdvDat::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  int CdvDat::seqnr() const {
    return m_impl->seqnr();
  }

  void CdvDat::setSeqnr(const int seqnr) {
    m_impl->setSeqnr(seqnr);
  }

  unsigned int CdvDat::flags() const {
    return m_impl->flags();
  }

  void CdvDat::setFlags(const unsigned int flags) {
    m_impl->setFlags(flags);
  }

  int CdvDat::inreq() const {
    return m_impl->inreq();
  }

  void CdvDat::setInreq(const int inreq) {
    m_impl->setInreq(inreq);
  }

  int CdvDat::n1() const {
    return m_impl->n1();
  }

  void CdvDat::setN1(const int n1) {
    m_impl->setN1(n1);
  }

  int CdvDat::n2() const {
    return m_impl->n2();
  }

  void CdvDat::setN2(const int n2) {
    m_impl->setN2(n2);
  }

  std::string CdvDat::name() const {
    return m_impl->name();
  }

  void CdvDat::setName(const std::string& name) {
    m_impl->setName(name);
  }

  std::string CdvDat::desc() const {
    return m_impl->desc();
  }

  void CdvDat::setDesc(const std::string& desc) {
    m_impl->setDesc(desc);
  }

  std::string CdvDat::valuename() const {
    return m_impl->valuename();
  }

  void CdvDat::setValuename(const std::string& valuename) {
    m_impl->setValuename(valuename);
  }

  CvfDat::CvfDat() : CdvDat() {}

  CvfDat::CvfDat(int nr, int seqnr, unsigned int flags, int inreq, int n1, int n2, std::string name, std::string desc)
    : CdvDat(nr, seqnr, flags, inreq, n1, n2, name, desc) {}

  CvfDat::CvfDat(int nr, int seqnr, unsigned int flags, int inreq, int n1, int n2, std::string name, std::string desc, std::string valuename)
    : CdvDat(nr, seqnr, flags, inreq, n1, n2, name, desc, valuename) {}

  DvfDat::DvfDat() : CdvDat() {}

  DvfDat::DvfDat(int nr, int seqnr, unsigned int flags, int inreq, int n1, int n2, std::string name, std::string desc)
    : CdvDat(nr, seqnr, flags, inreq, n1, n2, name, desc) {}

  DvfDat::DvfDat(int nr, int seqnr, unsigned int flags, int inreq, int n1, int n2, std::string name, std::string desc, std::string valuename)
    : CdvDat(nr, seqnr, flags, inreq, n1, n2, name, desc, valuename) {}

}  // namespace contam
}  // namespace openstudio
