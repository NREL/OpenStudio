/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../../utilities/core/StringHelpers.hpp"

#include "PrjSubobjects.hpp"
#include "PrjSubobjectsImpl.hpp"

namespace openstudio {
namespace contam {

  WeatherData::WeatherData() : m_impl(std::shared_ptr<detail::WeatherDataImpl>(new detail::WeatherDataImpl)) {}

  WeatherData::WeatherData(std::string Tambt, std::string barpres, std::string windspd, std::string winddir, std::string relhum, int daytyp, int uTa,
                           int ubP, int uws, int uwd)
    : m_impl(
        std::shared_ptr<detail::WeatherDataImpl>(new detail::WeatherDataImpl(Tambt, barpres, windspd, winddir, relhum, daytyp, uTa, ubP, uws, uwd))) {
  }

  WeatherData::WeatherData(double Tambt, double barpres, double windspd, double winddir, double relhum, int daytyp, int uTa, int ubP, int uws,
                           int uwd)
    : m_impl(
        std::shared_ptr<detail::WeatherDataImpl>(new detail::WeatherDataImpl(Tambt, barpres, windspd, winddir, relhum, daytyp, uTa, ubP, uws, uwd))) {
  }

  WeatherData::WeatherData(const WeatherData& other) = default;

  WeatherData& WeatherData::operator=(const WeatherData& other) = default;

  bool WeatherData::operator==(const WeatherData& other) const {
    return m_impl == other.m_impl;
  }

  bool WeatherData::operator!=(const WeatherData& other) const {
    return m_impl != other.m_impl;
  }

  std::string WeatherData::write() {
    return m_impl->write();
  }

  void WeatherData::read(Reader& input) {
    m_impl->read(input);
  }

  double WeatherData::Tambt() const {
    return m_impl->Tambt();
  }

  bool WeatherData::setTambt(const double Tambt) {
    return m_impl->setTambt(Tambt);
  }

  bool WeatherData::setTambt(const std::string& Tambt) {
    return m_impl->setTambt(Tambt);
  }

  double WeatherData::barpres() const {
    return m_impl->barpres();
  }

  bool WeatherData::setBarpres(const double barpres) {
    return m_impl->setBarpres(barpres);
  }

  bool WeatherData::setBarpres(const std::string& barpres) {
    return m_impl->setBarpres(barpres);
  }

  double WeatherData::windspd() const {
    return m_impl->windspd();
  }

  bool WeatherData::setWindspd(const double windspd) {
    return m_impl->setWindspd(windspd);
  }

  bool WeatherData::setWindspd(const std::string& windspd) {
    return m_impl->setWindspd(windspd);
  }

  double WeatherData::winddir() const {
    return m_impl->winddir();
  }

  bool WeatherData::setWinddir(const double winddir) {
    return m_impl->setWinddir(winddir);
  }

  bool WeatherData::setWinddir(const std::string& winddir) {
    return m_impl->setWinddir(winddir);
  }

  double WeatherData::relhum() const {
    return m_impl->relhum();
  }

  bool WeatherData::setRelhum(const double relhum) {
    return m_impl->setRelhum(relhum);
  }

  bool WeatherData::setRelhum(const std::string& relhum) {
    return m_impl->setRelhum(relhum);
  }

  int WeatherData::daytyp() const {
    return m_impl->daytyp();
  }

  void WeatherData::setDaytyp(const int daytyp) {
    m_impl->setDaytyp(daytyp);
  }

  int WeatherData::uTa() const {
    return m_impl->uTa();
  }

  void WeatherData::setUTa(const int uTa) {
    m_impl->setUTa(uTa);
  }

  int WeatherData::ubP() const {
    return m_impl->ubP();
  }

  void WeatherData::setUbP(const int ubP) {
    m_impl->setUbP(ubP);
  }

  int WeatherData::uws() const {
    return m_impl->uws();
  }

  void WeatherData::setUws(const int uws) {
    m_impl->setUws(uws);
  }

  int WeatherData::uwd() const {
    return m_impl->uwd();
  }

  void WeatherData::setUwd(const int uwd) {
    m_impl->setUwd(uwd);
  }

  Icon::Icon() : m_impl(std::shared_ptr<detail::IconImpl>(new detail::IconImpl())) {}

  Icon::Icon(int icon, int col, int row, int nr) : m_impl(std::shared_ptr<detail::IconImpl>(new detail::IconImpl(icon, col, row, nr))) {}

  Icon::Icon(const Icon& other) = default;

  Icon& Icon::operator=(const Icon& other) = default;

  bool Icon::operator==(const Icon& other) const {
    return m_impl == other.m_impl;
  }

  bool Icon::operator!=(const Icon& other) const {
    return m_impl != other.m_impl;
  }

  std::string Icon::write() {
    return m_impl->write();
  }

  void Icon::read(Reader& input) {
    m_impl->read(input);
  }

  int Icon::icon() const {
    return m_impl->icon();
  }

  void Icon::setIcon(const int icon) {
    m_impl->setIcon(icon);
  }

  int Icon::col() const {
    return m_impl->col();
  }

  void Icon::setCol(const int col) {
    m_impl->setCol(col);
  }

  int Icon::row() const {
    return m_impl->row();
  }

  void Icon::setRow(const int row) {
    m_impl->setRow(row);
  }

  int Icon::nr() const {
    return m_impl->nr();
  }

  void Icon::setNr(const int nr) {
    m_impl->setNr(nr);
  }

  bool Icon::isWall() {
    return m_impl->isWall();
  }

  void FanDataPoint::setDefaults() {
    m_mF = PRJFLOAT("0.0");
    m_u_mF = 0;
    m_dP = PRJFLOAT("0.0");
    m_u_dP = 0;
    m_rP = PRJFLOAT("0.0");
    m_u_rP = 0;
  }

  FanDataPoint::FanDataPoint() {
    setDefaults();
  }

  FanDataPoint::FanDataPoint(double mF, int u_mF, double dP, int u_dP, double rP, int u_rP) {
    setDefaults();
    setMF(mF);
    setU_mF(u_mF);
    setDP(dP);
    setU_dP(u_dP);
    setRP(rP);
    setU_rP(u_rP);
  }

  FanDataPoint::FanDataPoint(std::string mF, int u_mF, std::string dP, int u_dP, std::string rP, int u_rP) {
    setDefaults();
    setMF(mF);
    setU_mF(u_mF);
    setDP(dP);
    setU_dP(u_dP);
    setRP(rP);
    setU_rP(u_rP);
  }

  void FanDataPoint::read(Reader& input) {
    setMF(input.readNumber<std::string>());
    setU_mF(input.read<int>());
    setDP(input.readNumber<std::string>());
    setU_dP(input.read<int>());
    setRP(input.readNumber<std::string>());
    setU_rP(input.read<int>());
  }

  std::string FanDataPoint::write() {
    std::string string;
    string += ANY_TO_STR(m_mF) + ' ' + ANY_TO_STR(m_u_mF) + ' ' + ANY_TO_STR(m_dP) + ' ' + ANY_TO_STR(m_u_dP) + ' ' + ANY_TO_STR(m_rP) + ' '
              + ANY_TO_STR(m_u_rP) + '\n';
    return string;
  }

  double FanDataPoint::mF() const {
    return openstudio::string_conversions::to<double>(m_mF);
  }

  bool FanDataPoint::setMF(const double mF) {
    m_mF = openstudio::string_conversions::number(mF);
    return true;
  }

  bool FanDataPoint::setMF(const std::string& mF) {
    return assign_if_valid<double>(mF, m_mF);
  }

  int FanDataPoint::u_mF() const {
    return m_u_mF;
  }

  void FanDataPoint::setU_mF(const int u_mF) {
    m_u_mF = u_mF;
  }

  double FanDataPoint::dP() const {
    return openstudio::string_conversions::to<double>(m_dP);
  }

  bool FanDataPoint::setDP(const double dP) {
    m_dP = openstudio::string_conversions::number(dP);
    return true;
  }

  bool FanDataPoint::setDP(const std::string& dP) {
    return assign_if_valid<double>(dP, m_dP);
  }

  int FanDataPoint::u_dP() const {
    return m_u_dP;
  }

  void FanDataPoint::setU_dP(const int u_dP) {
    m_u_dP = u_dP;
  }

  double FanDataPoint::rP() const {
    return openstudio::string_conversions::to<double>(m_rP);
  }

  bool FanDataPoint::setRP(const double rP) {
    m_rP = openstudio::string_conversions::number(rP);
    return true;
  }

  bool FanDataPoint::setRP(const std::string& rP) {
    return assign_if_valid<double>(rP, m_rP);
  }

  int FanDataPoint::u_rP() const {
    return m_u_rP;
  }

  void FanDataPoint::setU_rP(const int u_rP) {
    m_u_rP = u_rP;
  }

  void XyDataPoint::setDefaults() {
    m_x = PRJFLOAT("0.0");
    m_y = PRJFLOAT("0.0");
  }

  XyDataPoint::XyDataPoint() {
    setDefaults();
  }

  XyDataPoint::XyDataPoint(double x, double y) {
    setDefaults();
    setX(x);
    setY(y);
  }

  XyDataPoint::XyDataPoint(std::string x, std::string y) {
    setDefaults();
    setX(x);
    setY(y);
  }

  void XyDataPoint::read(Reader& input) {
    setX(input.readNumber<std::string>());
    setY(input.readNumber<std::string>());
  }

  std::string XyDataPoint::write() {
    std::string string;
    string += ANY_TO_STR(m_x) + ' ' + ANY_TO_STR(m_y) + '\n';
    return string;
  }

  double XyDataPoint::x() const {
    return openstudio::string_conversions::to<double>(m_x);
  }

  bool XyDataPoint::setX(const double x) {
    m_x = openstudio::string_conversions::number(x);
    return true;
  }

  bool XyDataPoint::setX(const std::string& x) {
    return assign_if_valid<double>(x, m_x);
  }

  double XyDataPoint::y() const {
    return openstudio::string_conversions::to<double>(m_y);
  }

  bool XyDataPoint::setY(const double y) {
    m_y = openstudio::string_conversions::number(y);
    return true;
  }

  bool XyDataPoint::setY(const std::string& y) {
    return assign_if_valid<double>(y, m_y);
  }

  void AirflowSubelementData::setDefaults() {
    m_nr = 0;
    m_relHt = PRJFLOAT("0.0");
    m_filt = 0;
  }

  AirflowSubelementData::AirflowSubelementData() {
    setDefaults();
  }

  AirflowSubelementData::AirflowSubelementData(int nr, double relHt, int filt) {
    setDefaults();
    setNr(nr);
    setRelHt(relHt);
    setFilt(filt);
  }

  AirflowSubelementData::AirflowSubelementData(int nr, std::string relHt, int filt) {
    setDefaults();
    setNr(nr);
    setRelHt(relHt);
    setFilt(filt);
  }

  void AirflowSubelementData::read(Reader& input) {
    setNr(input.read<int>());
    setRelHt(input.readNumber<std::string>());
    setFilt(input.read<int>());
  }

  std::string AirflowSubelementData::write() {
    std::string string;
    string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_relHt) + ' ' + ANY_TO_STR(m_filt) + '\n';
    return string;
  }

  int AirflowSubelementData::nr() const {
    return m_nr;
  }

  void AirflowSubelementData::setNr(const int nr) {
    m_nr = nr;
  }

  double AirflowSubelementData::relHt() const {
    return openstudio::string_conversions::to<double>(m_relHt);
  }

  bool AirflowSubelementData::setRelHt(const double relHt) {
    m_relHt = openstudio::string_conversions::number(relHt);
    return true;
  }

  bool AirflowSubelementData::setRelHt(const std::string& relHt) {
    return assign_if_valid<double>(relHt, m_relHt);
  }

  int AirflowSubelementData::filt() const {
    return m_filt;
  }

  void AirflowSubelementData::setFilt(const int filt) {
    m_filt = filt;
  }

  void PressureCoefficientPoint::setDefaults() {
    m_azm = PRJFLOAT("0.0");
    m_coef = PRJFLOAT("0.0");
  }

  PressureCoefficientPoint::PressureCoefficientPoint() {
    setDefaults();
  }

  PressureCoefficientPoint::PressureCoefficientPoint(double azm, double coef) {
    setDefaults();
    setAzm(azm);
    setCoef(coef);
  }

  PressureCoefficientPoint::PressureCoefficientPoint(std::string azm, std::string coef) {
    setDefaults();
    setAzm(azm);
    setCoef(coef);
  }

  void PressureCoefficientPoint::read(Reader& input) {
    setAzm(input.readNumber<std::string>());
    setCoef(input.readNumber<std::string>());
  }

  std::string PressureCoefficientPoint::write() {
    std::string string;
    string += ANY_TO_STR(m_azm) + ' ' + ANY_TO_STR(m_coef) + '\n';
    return string;
  }

  double PressureCoefficientPoint::azm() const {
    return openstudio::string_conversions::to<double>(m_azm);
  }

  bool PressureCoefficientPoint::setAzm(const double azm) {
    m_azm = openstudio::string_conversions::number(azm);
    return true;
  }

  bool PressureCoefficientPoint::setAzm(const std::string& azm) {
    return assign_if_valid<double>(azm, m_azm);
  }

  double PressureCoefficientPoint::coef() const {
    return openstudio::string_conversions::to<double>(m_coef);
  }

  bool PressureCoefficientPoint::setCoef(const double coef) {
    m_coef = openstudio::string_conversions::number(coef);
    return true;
  }

  bool PressureCoefficientPoint::setCoef(const std::string& coef) {
    return assign_if_valid<double>(coef, m_coef);
  }

  void SchedulePoint::setDefaults() {
    m_time = std::string("00:00:00");
    m_ctrl = PRJFLOAT("0.0");
  }

  SchedulePoint::SchedulePoint() {
    setDefaults();
  }

  SchedulePoint::SchedulePoint(std::string time, double ctrl) {
    setDefaults();
    setTime(time);
    setCtrl(ctrl);
  }

  SchedulePoint::SchedulePoint(std::string time, std::string ctrl) {
    setDefaults();
    setTime(time);
    setCtrl(ctrl);
  }

  void SchedulePoint::read(Reader& input) {
    setTime(input.readString());
    setCtrl(input.readNumber<std::string>());
  }

  std::string SchedulePoint::write() {
    std::string string;
    string += m_time + ' ' + ANY_TO_STR(m_ctrl) + '\n';
    return string;
  }

  std::string SchedulePoint::time() const {
    return m_time;
  }

  void SchedulePoint::setTime(const std::string& time) {
    m_time = time;
  }

  double SchedulePoint::ctrl() const {
    return openstudio::string_conversions::to<double>(m_ctrl);
  }

  bool SchedulePoint::setCtrl(const double ctrl) {
    m_ctrl = openstudio::string_conversions::number(ctrl);
    return true;
  }

  bool SchedulePoint::setCtrl(const std::string& ctrl) {
    return assign_if_valid<double>(ctrl, m_ctrl);
  }

}  // namespace contam
}  // namespace openstudio
