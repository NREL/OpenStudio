/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SimModel.hpp"

#include <cmath>
#include <array>

#if _DEBUG || (__GNUC__ && !NDEBUG)
#  define DEBUG_ISO_MODEL_SIMULATION
#endif

using namespace openstudio::isomodel;
using namespace openstudio;

namespace openstudio {
namespace isomodel {

  double ISOResults::totalEnergyUse() const {
    double result = 0;
    std::vector<EndUseFuelType> fuelTypes = openstudio::EndUses::fuelTypes();
    for (const auto& monthlyResult : monthlyResults) {
      for (const auto& fuelType : fuelTypes) {
        result += monthlyResult.getEndUseByFuelType(fuelType);
      }
    }

    return result;
  }

  void SimModel::printVector(const char* vecName, const Vector& vec) {
#ifdef DEBUG_ISO_MODEL_SIMULATION
    std::stringstream ss;

    ss << vecName << "(" << vec.size() << ") = [";
    if (vec.size() > 0) {
      ss << vec[0];
      for (unsigned int i = 1; i < vec.size(); i++) {
        ss << ", " << vec[i];
      }
    }
    ss << "]";
    LOG(Trace, ss.str());
#endif
  }

  void SimModel::printMatrix(const char* matName, const Matrix& mat) {
#ifdef DEBUG_ISO_MODEL_SIMULATION
    std::stringstream ss;

    ss << matName << "(" << mat.size1() << ", " << mat.size2() << "): " << "\t";
    for (unsigned int j = 0; j < mat.size2(); j++) {
      ss << "," << j;
    }
    for (unsigned int i = 0; i < mat.size1(); i++) {
      ss << "\t" << i;
      for (unsigned int j = 0; j < mat.size2(); j++) {
        ss << "," << mat(i, j);
      }
      ss << '\n';
    }

    LOG(Trace, ss.str());
#endif
  }
  /**
   * Initializes a vector to the specified value
   */
  void vectorInit(Vector& vec, double val) {
    for (unsigned int i = 0; i < vec.size(); i++) {
      vec[i] = val;
    }
  }

  /**
   * Initializes a vector to 0
   */
  void zero(Vector& vec) {
    vectorInit(vec, 0);
  }

  /**
   * Initializes a vector to 1
   */
  void one(Vector& vec) {
    vectorInit(vec, 1);
  }

  /// array-scalar product
  Vector mult(const double* v1, const double s1, int size) {
    Vector vp = Vector(size);
    for (int i = 0; i < size; i++) {
      vp[i] = v1[i] * s1;
    }
    return vp;
  }
  /// vector-scalar product
  Vector mult(const Vector& v1, const double s1) {
    Vector vp = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vp[i] = v1[i] * s1;
    }
    return vp;
  }
  /// vector-array product
  Vector mult(const Vector& v1, const double* v2) {
    Vector vp = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vp[i] = v1[i] * v2[i];
    }
    return vp;
  }
  /// vector-vector product
  Vector mult(const Vector& v1, const Vector& v2) {
    assert(v1.size() == v2.size());
    Vector vp = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vp[i] = v1[i] * v2[i];
    }
    return vp;
  }
  ///Vector-scalar division
  Vector div(const Vector& v1, const double s1) {
    Vector vp = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      if (s1 == 0) {
        vp[i] = std::numeric_limits<double>::max();
      } else {
        vp[i] = v1[i] / s1;
      }
    }
    return vp;
  }
  Vector div(const double s1, const Vector& v1) {
    Vector vp = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      if (v1[i] == 0) {
        vp[i] = std::numeric_limits<double>::max();
      } else {
        vp[i] = s1 / v1[i];
      }
    }
    return vp;
  }
  ///vector-vector division
  Vector div(const Vector& v1, const Vector& v2) {
    assert(v1.size() == v2.size());
    Vector vp = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      if (v2[i] == 0) {
        vp[i] = std::numeric_limits<double>::max();
      } else {
        vp[i] = v1[i] / v2[i];
      }
    }
    return vp;
  }
  Vector sum(const Vector& v1, const Vector& v2) {
    assert(v1.size() == v2.size());

    Vector vs = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vs[i] = v1[i] + v2[i];
    }
    return vs;
  }
  Vector sum(const Vector& v1, const double v2) {
    Vector vs = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vs[i] = v1[i] + v2;
    }
    return vs;
  }
  Vector dif(const Vector& v1, const Vector& v2) {
    assert(v1.size() == v2.size());
    Vector vd = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vd[i] = v1[i] - v2[i];
    }
    return vd;
  }
  Vector dif(const Vector& v1, const double v2) {
    Vector vd = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vd[i] = v1[i] - v2;
    }
    return vd;
  }
  Vector dif(const double v1, const Vector& v2) {
    Vector vd = Vector(v2.size());
    for (size_t i = 0; i < v2.size(); i++) {
      vd[i] = v1 - v2[i];
    }
    return vd;
  }
  double maximum(const Vector& v1) {
    double max = -std::numeric_limits<double>::max();
    for (size_t i = 0; i < v1.size(); i++) {
      if (v1[i] > max) {
        max = v1[i];
      }
    }
    return max;
  }
  Vector maximum(const Vector& v1, const Vector& v2) {
    assert(v1.size() == v2.size());
    Vector vx = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vx[i] = std::max(v1[i], v2[i]);
    }
    return vx;
  }
  Vector maximum(const Vector& v1, double val) {
    Vector vx = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vx[i] = std::max(v1[i], val);
    }
    return vx;
  }
  double minimum(const Vector& v1) {
    double min = std::numeric_limits<double>::max();
    for (size_t i = 0; i < v1.size(); i++) {
      if (v1[i] < min) {
        min = v1[i];
      }
    }
    return min;
  }
  Vector minimum(const Vector& v1, double val) {
    Vector vn = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      vn[i] = std::min(v1[i], val);
    }
    return vn;
  }
  Vector abs(const Vector& v1) {
    Vector va = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      va[i] = std::fabs(v1[i]);
    }
    return va;
  }
  Vector pow(const Vector& v1, const double xp) {
    Vector va = Vector(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
      va[i] = std::pow(v1[i], xp);
    }
    return va;
  }

  //End Utility Functions
  constexpr double daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  constexpr double hoursInMonth[] = {744, 672, 744, 720, 744, 720, 744, 744, 720, 744, 720, 744};
  constexpr double megasecondsInMonth[] = {2.6784, 2.4192, 2.6784, 2.592, 2.6784, 2.592, 2.6784, 2.6784, 2.592, 2.6784, 2.592, 2.6784};
  constexpr double monthFractionOfYear[] = {0.0849315068493151, 0.0767123287671233, 0.0849315068493151, 0.0821917808219178,
                                            0.0849315068493151, 0.0821917808219178, 0.0849315068493151, 0.0849315068493151,
                                            0.0821917808219178, 0.0849315068493151, 0.0821917808219178, 0.0849315068493151};
  constexpr double daysInYear = 365;
  constexpr double hoursInYear = 8760;
  constexpr double hoursInWeek = 168;
  constexpr double EECALC_NUM_MONTHS = 12;
  constexpr double EECALC_NUM_HOURS = 24;
  constexpr double EECALC_WEEKDAY_START = 7;
  constexpr double kWh2MJ = 3.6f;

  //Solver functions
  void SimModel::scheduleAndOccupancy(Vector& weekdayOccupiedMegaseconds, Vector& weekdayUnoccupiedMegaseconds, Vector& weekendOccupiedMegaseconds,
                                      Vector& weekendUnoccupiedMegaseconds, Vector& clockHourOccupied, Vector& clockHourUnoccupied,
                                      double& frac_hrs_wk_day, double& hoursUnoccupiedPerDay, double& hoursOccupiedPerDay, double& frac_hrs_wk_nt,
                                      double& frac_hrs_wke_tot) const {
    hoursOccupiedPerDay = pop->hoursEnd() - pop->hoursStart();
    if (hoursOccupiedPerDay < 0) {
      hoursOccupiedPerDay += 24;
    }
    double daysOccupiedPerWeek = pop->daysEnd() - pop->daysStart() + 1;
    if (daysOccupiedPerWeek < 0) {
      daysOccupiedPerWeek += 7;
    }

    double hoursOccupiedDuringWeek = hoursOccupiedPerDay * daysOccupiedPerWeek;
    frac_hrs_wk_day = hoursOccupiedDuringWeek / hoursInWeek;

    hoursUnoccupiedPerDay = 24 - hoursOccupiedPerDay;
    double hoursUnoccupiedDuringWeek = (daysOccupiedPerWeek - 1) * hoursUnoccupiedPerDay;
    frac_hrs_wk_nt = hoursUnoccupiedDuringWeek / hoursInWeek;

    // double occupationDensity = pop->densityOccupied();
    // double unoccupiedDensity = pop->densityUnoccupied();
    // double densityRatio = occupationDensity / unoccupiedDensity;

    double totalWeekendHours = hoursInWeek - hoursOccupiedDuringWeek - hoursUnoccupiedDuringWeek;
    frac_hrs_wke_tot = totalWeekendHours / hoursInWeek;

    double weekendHoursOccupied = (7 - daysOccupiedPerWeek) * hoursOccupiedPerDay;
    double frac_hrs_wke_day = weekendHoursOccupied / hoursInWeek;

    double weekendHoursUnoccupied = totalWeekendHours - weekendHoursOccupied;
    double frac_hrs_wke_nt = weekendHoursUnoccupied / hoursInWeek;

    for (int m = 0; m < EECALC_NUM_MONTHS; m++) {
      weekdayOccupiedMegaseconds[m] = megasecondsInMonth[m] * frac_hrs_wk_day;
      weekdayUnoccupiedMegaseconds[m] = megasecondsInMonth[m] * frac_hrs_wk_nt;
      weekendOccupiedMegaseconds[m] = megasecondsInMonth[m] * frac_hrs_wke_day;
      weekendUnoccupiedMegaseconds[m] = megasecondsInMonth[m] * frac_hrs_wke_nt;
    }
    for (int h = 0; h < EECALC_NUM_HOURS; h++) {
      if (h - EECALC_WEEKDAY_START >= 0 && h - EECALC_WEEKDAY_START < hoursOccupiedPerDay) {
        clockHourOccupied[h] = 1;
        clockHourUnoccupied[h] = 0;
      } else {
        clockHourOccupied[h] = 0;
        clockHourUnoccupied[h] = 1;
      }
    }
  }
  void SimModel::solarRadiationBreakdown(const Vector& weekdayOccupiedMegaseconds, const Vector& weekdayUnoccupiedMegaseconds,
                                         const Vector& weekendOccupiedMegaseconds, const Vector& weekendUnoccupiedMegaseconds,
                                         const Vector& clockHourOccupied, const Vector& clockHourUnoccupied, Vector& v_hrs_sun_down_mo,
                                         Vector& frac_Pgh_wk_nt, Vector& frac_Pgh_wke_day, Vector& frac_Pgh_wke_nt, Vector& v_Tdbt_nt) const {

    Matrix m_mhEgh = location->weather()->mhEgh();
    Matrix m_mhdbt = location->weather()->mhdbt();

    // TODO: unreadVariable
    // Vector v_Tdbt_Day = prod(m_mhdbt, clockHourOccupied);
    // v_Tdbt_Day /= sum(clockHourOccupied);

    v_Tdbt_nt = prod(m_mhdbt, clockHourUnoccupied);
    v_Tdbt_nt /= sum(clockHourUnoccupied);

    Vector v_Egh_day = prod(m_mhEgh, clockHourOccupied);
    v_Egh_day /= sum(clockHourOccupied);

    Vector v_Egh_nt = prod(m_mhEgh, clockHourUnoccupied);
    v_Egh_nt /= sum(clockHourUnoccupied);
    /**
v_mdbt=W.mdbt;  % copy to a new variable so vector nature is clear
M_mhdbt=W.mhdbt;  % copy to a new variable so matrix nature is clear
M_mhEgh=W.mhEgh; % copy to a new variable so matrix nature is clear

% Note, these are matrix multiplies (matrix*vector) resulting in a vector
v_Tdbt_day=(M_mhdbt*v_day_hrs_yesno)./sum(v_day_hrs_yesno); %monthly average dry bulb temp (dbt) during the occupied hours of days
v_Tdbt_nt=(M_mhdbt*v_nt_hrs_yesno)./sum(v_nt_hrs_yesno); %monthly avg dbt during the unoccupied hours of days

v_Egh_day=(M_mhEgh*v_day_hrs_yesno)./sum(v_day_hrs_yesno);  %monthly avg global horiz rad power (Egh)  during the "day" hours
v_Egh_nt=(M_mhEgh*v_nt_hrs_yesno)./sum(v_nt_hrs_yesno);  %monthly avg Egh during the "night" hours
*/

    Vector v_Wgh_wk_day = mult(v_Egh_day, weekdayOccupiedMegaseconds);
    Vector v_Wgh_wk_nt = mult(v_Egh_nt, weekdayUnoccupiedMegaseconds);
    Vector v_Wgh_wke_day = mult(v_Egh_day, weekendOccupiedMegaseconds);
    Vector v_Wgh_wke_nt = mult(v_Egh_nt, weekendUnoccupiedMegaseconds);
    Vector v_Wgh_tot = sum(sum(v_Wgh_wk_day, v_Wgh_wk_nt), sum(v_Wgh_wke_day, v_Wgh_wke_nt));
    /**
v_Wgh_wk_day=v_Egh_day.*v_Msec_wk_day; % monthly avg Egh energy (Wgh) during the week days
v_Wgh_wk_nt=v_Egh_nt.*v_Msec_wk_nt;  %monthly avg Wgh during week nights
v_Wgh_wke_day=v_Egh_day.*v_Msec_wke_day; %monthly avg Wgh during weekend days
v_Wgh_wke_nt=v_Egh_nt.*v_Msec_wke_nt; %monthly avg Wgh during weekend nights
v_Wgh_tot=v_Wgh_wk_day+v_Wgh_wk_nt+v_Wgh_wke_day+v_Wgh_wke_nt; %Egh_avg_total MJ/m2
*/
    frac_Pgh_wk_nt = div(v_Wgh_wk_nt, v_Wgh_tot);
    frac_Pgh_wke_day = div(v_Wgh_wke_day, v_Wgh_tot);
    frac_Pgh_wke_nt = div(v_Wgh_wke_nt, v_Wgh_tot);
    /**

%FRAC_PGH_DAYTIME=v_Wgh_wk_day./v_Wgh_tot; %frac_Egh_occ
frac_Pgh_wk_nt=v_Wgh_wk_nt./v_Wgh_tot; %frac_Egh_unocc_weekday_night
frac_Pgh_wke_day=v_Wgh_wke_day./v_Wgh_tot; %frac_Egh_unocc_weekend_day
frac_Pgh_wke_nt=v_Wgh_wke_nt./v_Wgh_tot; %frac_Egh_unocc_weekend_night
*/
    Vector v_frac_hrs_sun_down = Vector(12);
    Vector v_frac_hrs_sun_up = Vector(12);
    // TODO: unreadVariable
    // Vector v_sun_up_time = Vector(12);
    // Vector v_sun_down_time = Vector(12);
    for (int i = 0; i < 12; i++) {
      v_frac_hrs_sun_up[i] = 0;
      v_frac_hrs_sun_down[i] = 0;
      for (int j = 0; j < 24; j++) {
        if (m_mhEgh(i, j) != 0) {
          v_frac_hrs_sun_up[i] = j;
          break;
        }
      }
      for (int j = 23; j >= 0; j--) {
        if (m_mhEgh(i, j) != 0) {
          v_frac_hrs_sun_down[i] = j;
          break;
        }
      }
      v_frac_hrs_sun_up[i] = (v_frac_hrs_sun_down[i] - v_frac_hrs_sun_up[i] + 1) / 24.0;
      v_frac_hrs_sun_down[i] = 1.0 - v_frac_hrs_sun_up[i];
    }
    /**
%%  find what time the sun comes up and goes down and the fraction of hours sun is up and down

%%% CHANGE
% the following two lines were in the original spreadsheet and the fraction of hours up was fixed.
% Let's do this properly by looking for the hours the sun is up and down each month
% RTM 10-NOV-2012
%
% v_frac_hrs_sun_down=ones(12,1)*(24-(19-7+1))/24;
% v_hrs_sun_down_mo=b_frac_hrs_sun_down.*v_hrs_ina_mo;

v_frac_hrs_sun_down=zeros(12,1);
v_frac_hrs_sun_up=zeros(12,1);
v_sun_up_time=zeros(12,1);
v_sun_down_time=zeros(12,1);

for  I=1:12
    J=find(W.mhEgh(I,:)~=0);  % find the hours where Egh is nonzero and identify as sun being up
    v_sun_up_time(I)=J(1);  % first element is the sunup hour
    v_sun_down_time(I)=J(end); % last element is sundown hour
    v_frac_hrs_sun_up(I)=length(J)/24;  % fraction of hours in the day the sun is up
    v_frac_hrs_sun_down(I)=1-v_frac_hrs_sun_up(I); % fraction of hours in the day the sun is down
end
v_hrs_sun_down_mo=v_frac_hrs_sun_down.*v_hrs_ina_mo;
    */
    //Vector v_hrs_sun_down_mo = Vector(v_frac_hrs_sun_down.size());
    for (size_t i = 0; i < v_frac_hrs_sun_down.size(); i++) {
      v_hrs_sun_down_mo[i] = v_frac_hrs_sun_down[i] * hoursInMonth[i];
    }
  }

  void SimModel::lightingEnergyUse(const Vector& v_hrs_sun_down_mo, double& Q_illum_occ, double& Q_illum_unocc, double& Q_illum_tot_yr,
                                   Vector& v_Q_illum_tot, Vector& v_Q_illum_ext_tot) const {
    double lpd_occ = lights->powerDensityOccupied();
    double lpd_unocc = lights->powerDensityUnoccupied();
    double F_D = lights->dimmingFraction();
    double F_O = building->lightingOccupancySensor();
    double F_C = building->constantIllumination();
    /*
  %% compute lighting energy use as per prEN 15193:2006

lpd_occ=In.LPD_occ;
lpd_unocc=In.LPD_unocc;

% assign fractions for daylighting, occupancy sensors, and const illum
% sensors as per prEN 1593:2006.
% Lookup tables are on the spreadsheet

F_D=In.daylighting_sensor; % F_D = daylight sensor dimming fraction
F_O=In.lighting_occupancy_sensor; % F_O = occupancy sensor control fraction
F_C=In.lighting_constant_illumination; %F_c = constant illuminance control fraction
*/

    double n_day_start = 7;
    double n_day_end = 19;
    double n_weeks = 50;
    double t_lt_D =
      (std::min(n_day_end, pop->hoursEnd()) - std::max(pop->hoursStart(), n_day_start)) * (pop->daysEnd() + 1 - pop->daysStart() + 1) * n_weeks;
    double t_lt_N = (std::max(n_day_start - pop->hoursStart(), 0.0) + std::max(pop->hoursEnd() - n_day_end, 0.0))
                    * (pop->daysEnd() + 1 - pop->daysStart() + 1) * n_weeks;
    Q_illum_occ = structure->floorArea() * lpd_occ * F_C * F_O * (t_lt_D * F_D + t_lt_N) / 1000.0;
    /*
%%% NOTE
% the following assumes day starts at hour 7 and ends at hour 19
% and 2 weeks per year are considered completely unoccupied for lighting
% This should be converted to a monthly quanitity using the monthly
% average sunup and sundown times

n_day_start=7;
n_day_end=19;
n_weeks=50;
t_lt_D=(min(In.occ_hour_end,n_day_end)-max(In.occ_hour_start,n_day_start))*(In.occ_day_end+1-In.occ_day_start+1)*n_weeks; %lighting_operating_hours during the daytime
t_lt_N=(max(n_day_start-In.occ_hour_start,0)+max(In.occ_hour_end-n_day_end,0))*(In.occ_day_end+1-In.occ_day_start+1)*n_weeks; %lighting_operating_hours during the nighttime

% total lighting energy
Q_illum_occ=In.cond_flr_area*lpd_occ*F_C*F_O*(t_lt_D*F_D + t_lt_N)/1000;  % find the total lighting energy for occupied times in kWh

%%% CHANGE
% as an alternative to a fixed parasitic lighting energy density use an unoccupied
% number for illumuniation LPD
% RTM 13-NOV-2012
%
% this is the original code from the spreadsheet that fixed the parasitic
% lighting load at 6 kWh/m2/yr as per pren 15193-2006 B12
% Q_lt_par_den=1+5;  % parasitic lighting energy density  is emergency + control lighting kWh/m2/yr
% Q_illum_par=Q_lt_par_den*In.cond_flr_area;
% Q_illum_tot_yr=Q_illum_occ+Q_illum_par;
*/

    double t_unocc = hoursInYear - t_lt_D - t_lt_N;
    Q_illum_unocc = structure->floorArea() * lpd_unocc * t_unocc / 1000.0;
    Q_illum_tot_yr = Q_illum_occ + Q_illum_unocc;
    v_Q_illum_tot = mult(monthFractionOfYear, Q_illum_tot_yr, 12);
    v_Q_illum_ext_tot = mult(v_hrs_sun_down_mo, lights->exteriorEnergy() / 1000.0);
    /*
 t_unocc=hrs_ina_yr - t_lt_D - t_lt_N;  % find the number of unoccupied lighting hours in the year
 Q_illum_unocc = In.cond_flr_area*lpd_unocc*t_unocc/1000;  % find the total annual lighting energy for unoccupied times in kWh
 Q_illum_tot_yr=Q_illum_occ+Q_illum_unocc;  % find the total annual lighting energy in kWh

% split annual lighting energy into monthly lighting energy via the month fraction of year
v_Q_illum_tot=Q_illum_tot_yr.*v_mo_frac_of_yr; %total interior monthly lighting energy in kWh

% exterior lighting
v_Q_illum_ext_tot=v_hrs_sun_down_mo*In.E_lt_ext/1000;  %etotal exterior monthly lighting energy in kWh

*/
  }
  void SimModel::envelopCalculations(Vector& v_win_A, Vector& v_wall_emiss, Vector& v_wall_alpha_sc, Vector& v_wall_U, Vector& v_wall_A,
                                     double& H_tr) const {
    v_wall_A = structure->wallArea();
    v_win_A = structure->windowArea();
    v_wall_U = structure->wallUniform();
    Vector v_win_U = structure->windowUniform();

    Vector v_env_UA = sum(mult(v_wall_A, v_wall_U), mult(v_win_A, v_win_U));
    double H_D = sum(v_env_UA);
    /*
  %% compute envelope parameters as per ISO 13790 8.3

% convert vectored inputs to vectors for clarification
v_wall_A=In.wall_area;
v_win_A=In.win_area;
v_wall_U=In.wall_U;
v_win_U=In.win_U;

v_env_UA=v_wall_A.*v_wall_U + v_win_A.*v_win_U; %compute total envelope U*A


% compute direct transmission heat transfer coefficient to exterior in W/K as per 8.3.1
% ignore linear and point thermal bridges for now
H_D = sum(v_env_UA);
*/
    double H_g = 0;
    double H_U = 0;
    double H_A = 0;
    H_tr = H_D + H_g + H_U + H_A;
    /*
% for now, also ignore heat transfer to ground (minmal in big office buildings), unconditioned spaces
% and to adjacent buildings
H_g = 0; % steady state  heat transfer coefficient by transfer to ground - not yet implemented
H_U = 0; % heat transfer coefficient for transmission through unconditioned spaces - not yet implemented
H_A = 0; % heat transfer coefficient for transmission to adjacent buildings  - not yet implemented

H_tr=H_D+H_g+H_U+H_A; %total transmission heat transfer coefficient as per eqn 17 in 8.3.1
*/
    v_wall_emiss = structure->wallThermalEmissivity();
    v_wall_alpha_sc = structure->wallSolarAbsorbtion();
    /*
% copy the following from input structure to vectors for clarity
v_wall_emiss=In.wall_thermal_emiss; % wall thermal emissivity
v_wall_alpha_sc =In.wall_solar_alpha; %wall solar absorption coefficient
*/
  }
  void SimModel::windowSolarGain(const Vector& v_win_A, const Vector& v_wall_emiss, const Vector& v_wall_alpha_sc, const Vector& v_wall_U,
                                 const Vector& v_wall_A, Vector& v_wall_A_sol, Vector& v_win_hr, Vector& v_wall_R_sc, Vector& v_win_A_sol) const {
    /*
  %%  Window Solar Gain

%%% REVISIT THIS SECTION
%%% The solar heat gain could be improved
%%% better understand SCF and SDF and how they map to F_sh
%%% calculate effective sky temp so we can better estimate theta_er and
%%% theta_ss, and hr

% From ISO 13790 11.3.3 Effective solar collecting area of glazed elements,% eqn 44
% A_sol = F_sh,gl* g_gl*(1 ? F_f)*A_w,p
% A_sol = effective solar collecting area of window in m2
% F_sh,gl = shading reduction factor for movable shades as per 11.4.3 % (v_win_SDF *v_win_SDF_frac)
% g_gl = total solar energy transmittance of transparent element as per % 11.4.2
% F_f = Frame area fraction (ratio of projected frame area to overall glazed element area) as per 11.4.5 (v_wind_ff)
% A_w,p = ovaral projected area of glazed element in m2 (v_wind_A)

%v_win_SHGC=In.win_SHGC; % copy to a new variable with a v_ for clarification
%v_win_SCF=In.win_SCF;  % copy to a new variable with a v_ for clarification


% The frame factor, v_win_ff is found using ISO 10077-1, in absence a national standard
% can be used.  Examples range from 0.2 for heating climates to 0.3 for
% cooling.  Use 0.25 as a compromise
*/
    int vsize = 9;
    double n_win_ff = 0.25;
    Vector v_win_ff = Vector(vsize);

    constexpr std::array<double, 3> n_win_SDF_table = {0.5, 0.35, 1.0};
    Vector v_win_SDF = Vector(vsize);
    Vector v_win_SDF_frac = Vector(vsize);

    /// \todo looking at forward translator, I'm not sure what's supposed to happen here,
    ///       but I know I cannot let it get below 0 or above 2, previous the code was hitting -1
    int n_win_SDF_table_index = std::min(2, std::max(static_cast<int>(structure->windowShadingDevice()) - 1, 0));

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "WindowShadingDevice index: " << n_win_SDF_table_index);
#endif
    for (int i = 0; i < vsize; i++) {
      v_win_ff[i] = 1.0 - n_win_ff;
      v_win_SDF[i] = n_win_SDF_table[n_win_SDF_table_index];
      v_win_SDF_frac[i] = 1.0;
    }
    Vector v_win_F_shgl = mult(v_win_SDF, v_win_SDF_frac);
    /*
n_win_ff=0.25;
v_win_ff=ones(size(1,9))*n_win_ff; %window frame_factor;

% assign SDF based on pulldown:
n_win_SDF_table=[0.5,0.35,1.0];% assign SDF based on pulldown value of 1, 2 or 3
v_win_SDF=n_win_SDF_table(In.win_SDF); %window SDF

% set the SCF and SDF fractions which includes heat transfer - set at 100% for now
%v_win_SCF_frac=ones(size(In.win_SCF)); % SCF fraction to include in HX;
v_win_SDF_frac=ones(size(v_win_SDF)); % SDF fraction to include in HX; Fixed at 100% for now

v_win_F_shgl = v_win_SDF.*v_win_SDF_frac;
*/
    Vector v_g_gln = structure->windowNormalIncidenceSolarEnergyTransmittance();
    double n_win_F_W = 0.9;
    Vector v_g_gl = mult(v_g_gln, n_win_F_W);

    v_win_A_sol = mult(mult(mult(v_win_F_shgl, v_g_gl), v_win_ff), v_win_A);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_g_gln", v_g_gln);
    printVector("v_g_gl", v_g_gl);
    printVector("v_win_ff", v_win_ff);
    printVector("v_win_A", v_win_A);
    printVector("v_win_F_shgl", v_win_F_shgl);
    printVector("v_win_A_sol", v_win_A_sol);
#endif

    /*
v_g_gln = In.win_SHGC ; % normal incidence solar energy transmittance which is SHGC in america
n_win_F_W = 0.9;%  correction factor for non-scattering window as per ISO 13790 11.4.2
v_g_gl = v_g_gln*n_win_F_W; % solar energy transmittance of glazing as per 11.4.2

v_win_A_sol=v_win_F_shgl.*v_g_gl.*(1-v_win_ff).*v_win_A;

% form factors given in ISO 13790, 11.4.6 as 0.5 for wall, 1.0 for unshaded roof

*/

    // double n_v_env_form_factors[]={0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1};
    double n_R_sc_ext = 0.04;
    v_wall_R_sc = Vector(vsize);
    for (int i = 0; i < vsize; i++) {
      v_wall_R_sc[i] = n_R_sc_ext;
    }
    v_win_hr = mult(v_wall_emiss, 5.0);
    v_wall_A_sol = mult(mult(mult(v_wall_alpha_sc, v_wall_R_sc), v_wall_U), v_wall_A);
    /*
n_v_env_form_factors=[0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 1]; %formfactor_to_sky.  Walls are all 0.5, roof is 1.0
n_R_sc_ext=0.04;  % vertical wall external convection surface heat resistance as per ISO 6946
v_wall_R_sc=ones(size(1,9))*n_R_sc_ext; %vertical wall external convective surface heat resistances

% 11.4.6 says use hr=5 as a first approx.
v_win_hr=5*v_wall_emiss; %window external radiative heat xfer coeff.

% effective collecting area of opaque building elements, A_sol EN ISO 13790 11.3.4
% A_sol = ?_S,c × R_se × U_c × A_c
% A_sol = effective solar collecting area
% ?_S,c = dimensionless solar absorption coefficient
% R_se = external surface heat resistance determined via ISO 6946 in m2*K/W
% U_c = thermal transmittance of opaque part determined via ISO 6946 W/m2K
% A_c = projected area of opaque part in m2

v_wall_A_sol=v_wall_alpha_sc.*v_wall_R_sc.*v_wall_U.*v_wall_A;

  */
  }
  void SimModel::solarHeatGain(const Vector& v_win_A_sol, const Vector& v_wall_R_sc, const Vector& v_wall_U, const Vector& v_wall_A,
                               const Vector& v_win_hr, const Vector& v_wall_A_sol, Vector& v_E_sol) const {
    /*
  %% Solar Heat Gain
% From EN ISo 13790 11.3.2  eqn 43
%
% ?sol,k = F_sh,ob,k * A_sol,k *  I_sol,k ? F*r,k ?_r,k
%
% ?_sol,k = solar heat flow gains through building element k
% F_sh,ob,k = shading reduction factor for external obstacles calculated via 11.4.4
% A_sol,k = effective collecting area of surface calculated via 11.3.3 (glazing ) 11.3.4 (opaque)
% I_sol,k = solar irradiance, mean energy of solar irradiation per square meter calculated using Annex F
% F_r,k form factor between building and sky determined using 11.4.6
% ?_r,k extra heat flow from thermal radiation to sky determined using 11.3.5


%%% REVISIT THIS SECTION
%%% The solar heat gain could be improved
%%%  better understand SCF and SDF and how they map to F_SH
%%% calculate effective sky temp so we can better estimate theta_er and
%%% theta_ss
*/
    Vector v_win_SCF_frac(structure->windowShadingCorrectionFactor().size());
    for (unsigned int i = 0; i < v_win_SCF_frac.size(); i++) {
      v_win_SCF_frac[i] = 1;
    }
    Matrix m_I_sol(12, 9, 0);  //month x direction + 1 (roof?)
    for (size_t r = 0; r < m_I_sol.size1(); r++) {
      for (size_t c = 0; c < m_I_sol.size2() - 1; c++) {
        m_I_sol(r, c) = location->weather()->msolar()(r, c);
      }
      m_I_sol(r, m_I_sol.size2() - 1) = location->weather()->mEgh()[r];
    }
    printMatrix("m_I_sol", m_I_sol);
    /*
v_win_SCF_frac=ones(size(In.win_SCF)); % SCF fraction to include in HX;  Fixed at 100% for now

v_I_sol=[W.msolar W.mEgh];  % create a new solar irradiance vector with the horizontal included as the last column
*/
    Vector v_win_phi_sol(12);
    Vector temp(9);
    for (size_t i = 0; i < v_win_phi_sol.size(); i++) {
      for (size_t j = 0; j < temp.size(); j++) {
        temp[j] = structure->windowShadingCorrectionFactor()[j] * v_win_SCF_frac[j] * v_win_A_sol[j] * m_I_sol(i, j);
      }
      v_win_phi_sol[i] = sum(temp);
    }
    /*
% compute the total solar heat gain for the glazing area
% note that the stuff in the sum is a 1x9 row vector for each surface since
% the .* multiplies element by element not a vector multiply.
v_win_phi_sol=zeros(12,1);
for I=1:12
    temp=In.win_SCF.*v_win_SCF_frac.*v_win_A_sol.*v_I_sol(I,:);
   v_win_phi_sol(I)=sum(temp);
end

% compute opaque area thermal radiation to the sky from EN ISO 13790 11.3.5
% ?_r,k = Rse×Uc×Ac×hr×??er (46)
% ?_r,k = thermal radiation to sky in W
% R_se = external heat resistance as defined above m2K/W
% U_c = U value of element as defined above W/m2K
% A_c = area of element  defined above m2
% ??er = is the average difference between the external air temperature and the apparent sky temperature,
% determined in accordance with 11.4.6, expressed in degrees centigrade.

% 11.4.6 says take ?er=9k in sub polar zones, 13 K in tropical or 11 K in intermediate
*/
    Vector theta_er(9);
    for (size_t i = 0; i < theta_er.size(); i++) {
      theta_er[i] = 11.0;
    }
    constexpr std::array<double, 9> n_v_env_form_factors = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1};

    Vector v_wall_phi_r = mult(mult(mult(mult(v_wall_R_sc, v_wall_U), v_wall_A), v_win_hr), theta_er);
    Vector v_wall_phi_sol(12);
    for (size_t i = 0; i < v_win_phi_sol.size(); i++) {
      for (size_t j = 0; j < temp.size(); j++) {
        temp[j] = v_wall_A_sol[j] * m_I_sol(i, j) - v_wall_phi_r[j] * n_v_env_form_factors[j];
      }
      v_wall_phi_sol[i] = sum(temp);
    }

    printVector("v_wall_phi_r", v_wall_phi_r);
    printVector("v_win_phi_sol", v_win_phi_sol);
    printVector("v_wall_phi_sol", v_wall_phi_sol);
    Vector v_phi_sol = sum(v_win_phi_sol, v_wall_phi_sol);
    printVector("v_phi_sol", v_phi_sol);
    v_E_sol = mult(v_phi_sol, megasecondsInMonth);
    /*
theta_er=ones(size(1,9))*11;  % average difference between air temp and sky temp = 11K as per 11.4.6

v_wall_phi_r = v_wall_R_sc.*v_wall_U.*v_wall_A.*v_win_hr.*theta_er;

% compute the total solar heat gain for the opaque area
v_wall_phi_sol=zeros(12,1);
for I=1:12
    temp=(v_wall_A_sol.*v_I_sol(I,:)-v_wall_phi_r.*n_v_env_form_factors);
   v_wall_phi_sol(I)=sum(temp);
end

v_phi_sol=v_win_phi_sol+v_wall_phi_sol;  % total envelope solar heat gain in W
v_E_sol= v_phi_sol.* v_Msec_ina_mo(I); % total envelope heat gain in MJ
  */
  }
  void SimModel::heatGainsAndLosses(double frac_hrs_wk_day, double /*Q_illum_occ*/, double Q_illum_unocc, double Q_illum_tot_yr, double& phi_int_avg,
                                    double& phi_plug_avg, double& phi_illum_avg, double& phi_int_wke_nt, double& phi_int_wke_day,
                                    double& phi_int_wk_nt) const {
    double phi_int_occ = pop->heatGainPerPerson() / pop->densityOccupied();
    double phi_int_unocc = pop->heatGainPerPerson() / pop->densityUnoccupied();
    phi_int_avg = frac_hrs_wk_day * phi_int_occ + (1 - frac_hrs_wk_day) * phi_int_unocc;

    double phi_plug_occ = building->electricApplianceHeatGainOccupied()
                          + building->gasApplianceHeatGainOccupied();  //%get the heat again in W/m2 from appliances during occupied times
    double phi_plug_unocc = building->electricApplianceHeatGainUnoccupied()
                            + building->gasApplianceHeatGainUnoccupied();  //%get the heat again in W/m2 from appliances during unoccupied times
    phi_plug_avg = phi_plug_occ * frac_hrs_wk_day + phi_plug_unocc * (1 - frac_hrs_wk_day);  //% get the average heat gain from appliances in W/m2

    // double phi_illum_occ = Q_illum_occ/structure->floorArea()/hoursInYear/frac_hrs_wk_day*1000; //% convert occ illum energy from kWh to W/m2
    double phi_illum_unocc =
      Q_illum_unocc / structure->floorArea() / hoursInYear / (1 - frac_hrs_wk_day) * 1000;  //% convert unocc illum engergy from kWh to W/m2
    phi_illum_avg = Q_illum_tot_yr / structure->floorArea() / hoursInYear * 1000;  //% % convert avg E_illum from kWh per year to average W/m2

    /*
  %% Compute heat gains and losses

% heat gain from occupants
%%% from Table G-9 of ISO 13790

%
phi_int_occ=In.htgain_per_person/In.people_density_occ;  % get the heat gain in W/m2 from people during occupied times
phi_int_unocc=In.htgain_per_person/In.people_density_unocc;  % get the heat gain in W/m2 from people during unoccupied times
phi_int_avg=frac_hrs_wk_day*phi_int_occ +(1-frac_hrs_wk_day)*phi_int_unocc;  %get the average heat gain from people in W/m2

phi_plug_occ=In.elec_plug_dens_occ + In.gas_plug_dens_occ; %get the heat again in W/m2 from appliances during occupied times
phi_plug_unocc=In.elec_plug_dens_unocc + In.gas_plug_dens_unocc; %get the heat again in W/m2 from appliances during unoccupied times
phi_plug_avg=phi_plug_occ*frac_hrs_wk_day + phi_plug_unocc*(1-frac_hrs_wk_day); % get the average heat gain from appliances in W/m2

phi_illum_occ = Q_illum_occ/In.cond_flr_area/hrs_ina_yr/frac_hrs_wk_day*1000; % convert occ illum energy from kWh to W/m2
phi_illum_unocc = Q_illum_unocc/In.cond_flr_area/hrs_ina_yr/(1-frac_hrs_wk_day)*1000; % convert unocc illum engergy from kWh to W/m2
phi_illum_avg = Q_illum_tot_yr/In.cond_flr_area/hrs_ina_yr*1000; % % convert avg E_illum from kWh per year to average W/m2

%%% CHANGED
% modified code to read average people density during unoccupied times
% rathter than assuming it is 1/9 of occupied times.
%

% original spreadsheet computed the approximate internal heat gain for week nights, weekend days, and weekend nights
% assuming they scale as the occ. fractions.  These are used for finding temp and not for directly calculating energy
% use total so approximations are more acceptable
% phi_int_wk_nt=(phi_int_occ+phi_plug_occ+phi_illum_occ)*occ_frac_wk_nt;
% phi_int_wke_day=(phi_int_occ+phi_plug_occ+phi_illum_occ)*occ_frac_wke_day;
% phi_int_wke_nt=(phi_int_occ+phi_plug_occ+phi_illum_occ)*occ_frac_wke_nt;

% the following is a more accuate internal heat gain for week nights,
% weekend days and weekend nights as it uses the unoccupied values rather
% than just scaling occupied versions with the occupancy fraction
% RTM 13-Nov-2012
*/
    phi_int_wk_nt = (phi_int_unocc + phi_plug_unocc + phi_illum_unocc);
    phi_int_wke_day = (phi_int_unocc + phi_plug_unocc + phi_illum_unocc);
    phi_int_wke_nt = (phi_int_unocc + phi_plug_unocc + phi_illum_unocc);

    /*
phi_int_wk_nt=(phi_int_unocc+phi_plug_unocc+phi_illum_unocc);
phi_int_wke_day=(phi_int_unocc+phi_plug_unocc+phi_illum_unocc);
phi_int_wke_nt=(phi_int_unocc+phi_plug_unocc+phi_illum_unocc);
  */
  }
  void SimModel::internalHeatGain(double phi_int_avg, double phi_plug_avg, double phi_illum_avg, double& phi_I_tot) const {
    double phi_I_occ = phi_int_avg * structure->floorArea();   //%phi_I_occ  - total occupant internal heat gain per year
    double phi_I_app = phi_plug_avg * structure->floorArea();  // %phi_I_app - total appliance internal heat gain per year
    double phi_I_lt = phi_illum_avg * structure->floorArea();  //%phi_I_lg - total lighting internal heat gain per year
    phi_I_tot = phi_I_occ + phi_I_app + phi_I_lt;

    /*
  %% Internal Heat Gain in MJ

phi_I_occ = phi_int_avg*In.cond_flr_area; %phi_I_occ  - total occupant internal heat gain per year
phi_I_app = phi_plug_avg*In.cond_flr_area; %phi_I_app - total appliance internal heat gain per year
phi_I_lt = phi_illum_avg*In.cond_flr_area; %phi_I_lg - total lighting internal heat gain per year
phi_I_tot = phi_I_occ + phi_I_app + phi_I_lt;
  */
  }
  void SimModel::unoccupiedHeatGain(double phi_int_wk_nt, double phi_int_wke_day, double phi_int_wke_nt, const Vector& weekdayUnoccupiedMegaseconds,
                                    const Vector& weekendOccupiedMegaseconds, const Vector& weekendUnoccupiedMegaseconds,
                                    const Vector& frac_Pgh_wk_nt, const Vector& frac_Pgh_wke_day, const Vector& frac_Pgh_wke_nt,
                                    const Vector& v_E_sol, Vector& v_P_tot_wke_day, Vector& v_P_tot_wk_nt, Vector& v_P_tot_wke_nt) const {
    Vector v_W_int_wk_nt = mult(weekdayUnoccupiedMegaseconds, phi_int_wk_nt * structure->floorArea());
    Vector v_W_int_wke_day = mult(weekendOccupiedMegaseconds, phi_int_wke_day * structure->floorArea());
    Vector v_W_int_wke_nt = mult(weekendUnoccupiedMegaseconds, phi_int_wke_nt * structure->floorArea());
    printVector("v_W_int_wk_nt", v_W_int_wk_nt);
    printVector("v_W_int_wke_day", v_W_int_wke_day);
    printVector("v_W_int_wke_nt", v_W_int_wke_nt);
    Vector v_W_sol_wk_nt = mult(v_E_sol, frac_Pgh_wk_nt);
    Vector v_W_sol_wke_day = mult(v_E_sol, frac_Pgh_wke_day);
    Vector v_W_sol_wke_nt = mult(v_E_sol, frac_Pgh_wke_nt);
    printVector("v_W_sol_wk_nt", v_W_sol_wk_nt);
    printVector("v_W_sol_wke_day", v_W_sol_wke_day);
    printVector("v_W_sol_wke_nt", v_W_sol_wke_nt);

    v_P_tot_wk_nt = div(sum(v_W_int_wk_nt, v_W_sol_wk_nt), weekdayUnoccupiedMegaseconds);
    v_P_tot_wke_day = div(sum(v_W_int_wke_day, v_W_sol_wke_day), weekendOccupiedMegaseconds);
    v_P_tot_wke_nt = div(sum(v_W_int_wke_nt, v_W_sol_wke_nt), weekendUnoccupiedMegaseconds);
    /*
  %% Unoccupied time heat gains in MJ
v_W_int_wk_nt=In.cond_flr_area.*phi_int_wk_nt.*v_Msec_wk_nt;  %internal heat gain for "week" "night"
v_W_int_wke_day=In.cond_flr_area.*phi_int_wke_day.*v_Msec_wke_day; %internal heat gain "weekend" "day"
v_W_int_wke_nt=In.cond_flr_area.*phi_int_wke_nt.*v_Msec_wke_nt; %internal heat gain "weekend" "night"

v_W_sol_wk_nt = v_E_sol.*frac_Pgh_wk_nt;  %solar heat gain "week" "night" in MJ
v_W_sol_wke_day = v_E_sol.*frac_Pgh_wke_day; % solar heat gain "weekend" "day" in MJ
v_W_sol_wke_nt = v_E_sol.*frac_Pgh_wke_nt; % solar heat gain "weekend" "night" in MJ

v_P_tot_wk_nt = (v_W_int_wk_nt+v_W_sol_wk_nt)./v_Msec_wk_nt;  % total heat gain "week" "night" W/m2
v_P_tot_wke_day = (v_W_int_wke_day+v_W_sol_wke_day)./v_Msec_wke_day;% total heat gain "weekend" "day W/m2
v_P_tot_wke_nt = (v_W_int_wke_nt+v_W_sol_wke_nt)./v_Msec_wke_nt; % total heat gain "weekend" "night" W/m2


  */
  }
  void SimModel::interiorTemp(const Vector& v_wall_A, const Vector& /*v_P_tot_wke_day*/, const Vector& v_P_tot_wk_nt,
                              const Vector& /*v_P_tot_wke_nt*/, const Vector& v_Tdbt_nt, double H_tr, double hoursUnoccupiedPerDay,
                              double hoursOccupiedPerDay, double frac_hrs_wk_day, double frac_hrs_wk_nt, double frac_hrs_wke_tot, Vector& v_Th_avg,
                              Vector& v_Tc_avg, double& tau) const {
    //BEM Type
    double T_adj = 0;
    switch (static_cast<int>(building->buildingEnergyManagement())) {
      case 1:
        T_adj = 0.0;
        break;
      case 2:
        T_adj = 0.5;
        break;
      case 3:
        T_adj = 1.0;
        break;
    }
    double ht_tset_ctrl = heating->temperatureSetPointOccupied() - T_adj;
    double cl_tset_ctrl = cooling->temperatureSetPointOccupied() + T_adj;

    double ht_tset_unocc = heating->temperatureSetPointUnoccupied();
    double cl_tset_unocc = cooling->temperatureSetPointUnoccupied();

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "ht_tset_ctrl " << ht_tset_ctrl);
    LOG(Trace, "cl_tset_ctrl " << cl_tset_ctrl);
    LOG(Trace, "ht_tset_unocc" << ht_tset_unocc);
    LOG(Trace, "cl_tset_unocc" << cl_tset_unocc);
#endif

    Vector v_ht_tset_ctrl(12);
    Vector v_cl_tset_ctrl(12);

    for (size_t i = 0; i < v_cl_tset_ctrl.size(); i++) {
      v_cl_tset_ctrl[i] = cl_tset_ctrl;
      v_ht_tset_ctrl[i] = ht_tset_ctrl;
    }

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_cl_tset_ctrl", v_cl_tset_ctrl);
    printVector("v_ht_tset_ctrl", v_ht_tset_ctrl);
#endif

    /*
  %%  Interior Temp  (occ, weekday unocc, weekend day, weekend night)

% det the temp differential from the interior heating./cooling setpoint
% based on the BEM type.
% an advanced BEM has the effect of reducing the effective heating temp and
% raising the effective cooling temp during times of control (i.e. during
% occupancy).
%
switch In.BEM_type
    case 1
        T_adj=0;
    case 2
        T_adj=0.5;
    case 3
        T_adj=1;
end

ht_tset_ctrl = In.ht_tset_occ - T_adj;  %
cl_tset_ctrl = In.cl_tset_occ + T_adj;

% during unoccupied times, we use a setback temp and even if we have a BEM
% it has no effect
ht_tset_unocc = In.ht_tset_unocc;
cl_tset_unocc = In.cl_tset_unocc;

v_ht_tset_ctrl = ones(12,1).*ht_tset_ctrl;  % create a column vector of the interior heating temp set point for each month
v_cl_tset_ctrl = ones(12,1).*cl_tset_ctrl;
*/

    double T_ht_ctrl_flag = 1;
    double T_cl_ctrl_flag = 1;
    double Cm_int = structure->interiorHeatCapacity() * structure->floorArea();  //% set the interior heat capacity

    double Cm_env = structure->wallHeatCapacity() * sum(v_wall_A);
    double Cm = Cm_int + Cm_env;

    /*
% flags to signify if we have heating and controls turned on or off
% and cooling and controls turned off.  We might turn off if we are
% unoccupied for an extended period of time, say a school in summer
T_ht_ctrl_flag=1;
T_cl_ctrl_flag=1;

%%% NOTE heat capacity description had an error on input sheet before.  It asked for
% envelope heat capacity but used floor area for Cm calc so this should ask
% for heat capacity of interior construction.
% We should really should separate this into envelope and interior terms
% and add them together
%  RTM 13-NOV-2012


% set the heat capacity given the  building interior heat capacity per unit
% area J/K/m2 from input spreadsheet
Cm_int=In.heat_capacity_int*In.cond_flr_area;  % set the interior heat capacity

%%% CHANGE
% add in the heat capacity of the wall to the heat capacity of interior
% based on floor area.   Assume same J/K/m2 for walls as interior for now
% do not add in window areas
Cm_env=In.heat_capacity_env.*(sum(v_wall_A));

%Cm_env=0;  % use this to match old spreadsheet Cm calc;
Cm=Cm_int+Cm_env;
*/
    double H_ve = 0.0;
    double H_tot = H_tr + H_ve;
    tau = Cm / H_tot / 3600.0;
    /*
% H_ve is overall heat transfer coefficient by ventilation as per ISO 13790 9.3
H_ve=0;  % not implemented, set to 0 for now (its small so a good approx anyhow)
H_tot=H_tr+H_ve;  %total overall heat transfer coefficient as per 12.2.1.3

tau=Cm./H_tot/3600;  % time constant of building in *hours* as per eqn 62 in 12.2.1.3 of 13790

% The following code computes the average weekend room temp using exponential rise and
% decays as we switch between day and night temp settings.  It assumes that
% the weekend is two days (we'll call them sat and sun)
%
% we do this wierd breakdown breakdown because want to separate day with
% solar loading from night without.  We can then use the average temp
% in each time frame rather than the overall monthly average.  right now
% wk_nt stuff is the same as wke_nt, but wke_day is much different because
% the solar gain increases the heat gain considerably, even on the weekend
% when occupant, lighting, and plugload gains are small
*/

    Vector v_ti(5);
    v_ti[0] = v_ti[2] = v_ti[4] = hoursUnoccupiedPerDay;
    v_ti[1] = v_ti[3] = hoursOccupiedPerDay;
    Matrix M_dT(v_P_tot_wk_nt.size(), 5, 0);
    Matrix M_Te(v_Tdbt_nt.size(), 5, 0);
    /*
%%% NOTE
% The following code is not a direct translation of the excel spreadsheet
% but makes use of both loops and matricies for clarity but computes the same results

% create a vector of lengths of the periods of times between possible temperature resets during
% the weekend
v_ti=[hrs_unocc_per_day, hrs_occ_per_day,  hrs_unocc_per_day, hrs_occ_per_day, hrs_unocc_per_day];

% generate an effective delta T matrix from ratio of total interior gains to heat
% transfer coefficient for each time period
% Note this is a matrix where the columns are the vectors v_P_tot_wk_nt/H_tot, and so on
% this is for a week night, weekend day, weekend night, weekend day,weekend night sequence
M_dT=[v_P_tot_wk_nt, v_P_tot_wke_day, v_P_tot_wke_nt, v_P_tot_wke_day, v_P_tot_wke_nt]./H_tot;
M_Te=[v_Tdbt_nt, v_Tdbt_day, v_Tdbt_nt, v_Tdbt_day, v_Tdbt_nt]; % create a matrix of the  average dry bulb temps

    */

    /*re-arrange the Else by just using the copy constructors

else % if the HVAC heating controls are turned off there is no setback so temp is constant
    v_Th_wk_day=v_ht_tset_ctrl;
    v_Th_wk_nt=v_ht_tset_ctrl;
    v_Th_wke_avg=v_ht_tset_ctrl;
end
    */

    Vector v_Th_wke_avg(v_ht_tset_ctrl);
    Vector v_Th_wk_day(v_ht_tset_ctrl);
    Vector v_Th_wk_nt(v_ht_tset_ctrl);

    /*
% compute the change in temp from setback to another heating temp in unoccupied times
if T_ht_ctrl_flag ==1  % if the HVAC heating controls are turned on.*/

    if (T_ht_ctrl_flag == 1) {  //if the HVAC heating controls are turned on.
      Matrix M_Ta(12, 4, 0);
      Vector v_Tstart(v_ht_tset_ctrl);
      for (size_t i = 0; i < M_Ta.size2(); i++) {
        for (size_t j = 0; j < M_Ta.size1(); j++) {
          v_Tstart(j) = M_Ta(j, i) = (v_Tstart(j) - M_Te(j, i) - M_dT(j, i)) * exp(-1 * v_ti(i) / tau) + M_Te(j, i) + M_dT(j, i);
        }
      }

      /*
       % find the exponential Temp decay after any changes in heating temp setpoint and put
  % in the matrix M_Ta with columns being the different time segments

    M_Ta=zeros(12,4);
    v_Tstart=v_ht_tset_ctrl;
    for I=1:4
        M_Ta(:,I)=(v_Tstart - M_Te(:,I) - M_dT(:,I)).*exp(-v_ti(I)./tau)+M_Te(:,I)+M_dT(:,I);
        v_Tstart=M_Ta(:,I);
    end
      */

      Matrix M_Taa(12, 5, 0);
#ifdef DEBUG_ISO_MODEL_SIMULATION
      printMatrix("M_Taa", M_Taa);
#endif

      for (size_t j = 0; j < M_Taa.size1(); j++) {
        M_Taa(j, 1) = v_ht_tset_ctrl[j];
      }
      for (size_t i = 1; i < M_Taa.size2(); i++) {
        for (size_t j = 0; j < M_Taa.size1(); j++) {
          M_Taa(j, i) = std::max(M_Ta(j, i - 1), ht_tset_unocc);
        }
      }
#ifdef DEBUG_ISO_MODEL_SIMULATION
      printMatrix("M_Taa", M_Taa);
#endif

      /*
       % the temp will only decay to the new lower setpoint, so find which is
    % higher the setpoint or the decay and select that as the start point for
    % the average integration to follow
   M_Taa=zeros(12,5);
   M_Taa(:,1)=v_ht_tset_ctrl;
    for I=2:5 % loop wke day to wke nt to wke day to wke nt
        M_Taa(:,I)=max(M_Ta(:,I-1),ht_tset_unocc);
        %v_Tstart=M_Ta(:,I);
    end
      */
      Matrix M_Tb(12, 5, 0);

      for (size_t i = 0; i < M_Tb.size2(); i++) {
        for (size_t j = 0; j < M_Tb.size1(); j++) {
          double v_T_avg = tau / v_ti(i) * (M_Taa(j, i) - M_Te(j, i) - M_dT(j, i)) * (1 - exp(-1 * v_ti(i) / tau)) + M_Te(j, i) + M_dT(j, i);
          M_Tb(j, i) = std::max(v_T_avg, ht_tset_unocc);
        }
      }
      for (size_t i = 0; i < v_Th_wke_avg.size(); i++) {
        double thisSum = 0;
        for (size_t j = 0; j < M_Tb.size2(); j++) {
          thisSum += M_Tb(i, j);
        }
        v_Th_wke_avg[i] = thisSum / M_Tb.size2();
      }
      for (size_t j = 0; j < M_Tb.size1(); j++) {
        v_Th_wk_nt[j] = M_Tb(j, 1);
      }
    }
    /*
   M_Tb=zeros(12,5);
    % for each time period, find the average temp given the start and
    % ending temp and assuming exponential decay of temps
    %v_t_start=M_Taa(:,1);
    for I=1:5 % loop through wk nt to wke day to wke nt to wke day to wke nt
        v_T_avg=tau./v_ti(I).*(M_Taa(:,I) - M_Te(:,I) -M_dT(:,I)).*(1-exp(-v_ti(I)/tau)) + M_Te(:,I) +M_dT(:,I);
        M_Tb(:,I) = max(v_T_avg,ht_tset_unocc);
    end

    v_Th_wke_avg=zeros(12,1);
    for I=1:12 % for each month
        v_Th_wke_avg(I)=mean(M_Tb(I,:));  % get the average for each month
    end
    v_Th_wk_day = v_ht_tset_ctrl;
    v_Th_wk_nt=M_Tb(:,1);
    */

    Vector v_Tc_wk_day(v_cl_tset_ctrl);
    Vector v_Tc_wk_nt(v_cl_tset_ctrl);
    Vector v_Tc_wke_avg(v_cl_tset_ctrl);

    /*else  % if cooling controls are turned off, temp will be constant at the control set temp with no setback
   v_Tc_wk_day=v_cl_tset_ctrl;
   v_Tc_wk_nt=v_cl_tset_ctrl;
   v_Tc_wke_avg=v_cl_tset_ctrl;
end*/

    if (T_cl_ctrl_flag == 1) {
      Matrix M_Tc(12, 4, 0);
      Vector v_Tstart(v_cl_tset_ctrl);
      for (size_t i = 0; i < M_Tc.size2(); i++) {
        for (size_t j = 0; j < M_Tc.size1(); j++) {
          v_Tstart(j) = M_Tc(j, i) = (v_Tstart(j) - M_Te(j, i) - M_dT(j, i)) * exp(-1 * v_ti(i) / tau) + M_Te(j, i) + M_dT(j, i);
        }
      }
      /*
if T_cl_ctrl_flag ==1  % if the HVAC cooling controls are on
    % find the Temp decay after any changes in cooling temp setpoint
    M_Tc=zeros(12,4);
    v_Tstart=v_cl_tset_ctrl;
    for I=1:4
        M_Tc(:,I)=(v_Tstart - M_Te(:,I) - M_dT(:,I)).*exp(-v_ti(I)/tau)+M_Te(:,I)+M_dT(:,I);
        v_Tstart=M_Tc(:,I);
    end
    */
      Matrix M_Tcc(12, 5, 0);
      for (size_t j = 0; j < M_Tcc.size1(); j++) {
        M_Tcc(j, 1) = std::min(v_ht_tset_ctrl[j], cl_tset_unocc);
      }
      for (size_t i = 1; i < M_Tcc.size2(); i++) {
        for (size_t j = 0; j < M_Tcc.size1(); j++) {
          M_Tcc(j, i) = std::max(M_Tc(j, i - 1), cl_tset_unocc);
        }
      }
      /*
    % Check to see if the decay temp is lower than the temp setpoint.  If so, the space will cool
    % to that level.  If the cooling setpoint is lower the cooling system will kick in and lower the
    % temp to the cold temp setpoint
    M_Tcc=zeros(12,5);
    M_Tcc(:,1)=min(v_cl_tset_ctrl,cl_tset_unocc);
    for I=2:5
        M_Tcc(:,I)=min(M_Tc(:,I-1),cl_tset_unocc);
    end
      */

      Matrix M_Td(12, 5, 0);

      for (size_t i = 0; i < M_Td.size2(); i++) {
        for (size_t j = 0; j < M_Td.size1(); j++) {
          double v_T_avg = tau / v_ti(i) * (M_Tcc(j, i) - M_Te(j, i) - M_dT(j, i)) * (1 - exp(-1 * v_ti(i) / tau)) + M_Te(j, i) + M_dT(j, i);
          M_Td(j, i) = std::max(v_T_avg, cl_tset_unocc);
        }
      }

      for (size_t i = 0; i < v_Th_wke_avg.size(); i++) {
        double thisSum = 0;
        for (size_t j = 0; j < M_Td.size2(); j++) {
          thisSum += M_Td(i, j);
        }
        v_Tc_wke_avg[i] = thisSum / M_Td.size2();
      }
      for (size_t j = 0; j < M_Td.size1(); j++) {
        v_Tc_wk_nt[j] = M_Td(j, 1);
      }
      /*
    % for each time period, find the average temp given the exponential
    % decay
    %v_t_start=M_Tcc(:,1);
    M_Td=zeros(12,5);
    for I=1:5
        v_T_avg=tau./v_ti(I).*(M_Tcc(:,I) - M_Te(:,I) -M_dT(:,I)).*(1-exp(-v_ti(I)/tau)) + M_Te(:,I) +M_dT(:,I);
        M_Td(:,I) = min(v_T_avg,cl_tset_unocc);
    end

    v_Tc_wke_avg=zeros(12,1);
    for I=1:12
        v_Tc_wke_avg(I)=mean(M_Td(I,:));  % get the average for each month
    end
    v_Tc_wk_day = v_cl_tset_ctrl;
    v_Tc_wk_nt=M_Td(:,1); % T_i_unocc_weekday_night
    */
    }

    Vector v_Th_wk_avg = sum(sum(mult(v_Th_wk_day, frac_hrs_wk_day), mult(v_Th_wk_nt, frac_hrs_wk_nt)), mult(v_Th_wke_avg, frac_hrs_wke_tot));
    Vector v_Tc_wk_avg = sum(sum(mult(v_Tc_wk_day, frac_hrs_wk_day), mult(v_Tc_wk_nt, frac_hrs_wk_nt)), mult(v_Tc_wke_avg, frac_hrs_wke_tot));

    //v_Th_avg(v_Th_wk_avg);
    //v_Tc_avg(v_Tc_wk_avg);
#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "ht_tset_ctrl " << ht_tset_ctrl);
    LOG(Trace, "cl_tset_ctrl " << cl_tset_ctrl);

    printVector("v_Th_wk_day", v_Th_wk_day);
    printVector("v_Th_wk_nt", v_Th_wk_nt);
    printVector("v_Th_wke_nt", v_Th_wke_avg);

    printVector("v_Th_wk_avg", v_Th_wk_avg);
    printVector("v_Tc_wk_avg", v_Th_wk_avg);
#endif

    for (size_t i = 0; i < v_Tc_wk_avg.size(); i++) {
      v_Th_avg[i] = std::min(v_Th_wk_avg[i], ht_tset_ctrl);
      v_Tc_avg[i] = std::min(v_Tc_wk_avg[i], cl_tset_ctrl);
    }
    /*


%find the average temp for the whole week from the fractions of each period
v_Th_wk_avg = v_Th_wk_day*frac_hrs_wk_day+v_Th_wk_nt*frac_hrs_wk_nt+v_Th_wke_avg*frac_hrs_wke_tot; % T_i_average
v_Tc_wk_avg = v_Tc_wk_day*frac_hrs_wk_day+v_Tc_wk_nt*frac_hrs_wk_nt+v_Tc_wke_avg*frac_hrs_wke_tot; % T_i_average

% the final avg for monthly energy computations is the lesser of the avg
% computed above and the heating set control
v_Th_avg = min(v_Th_wk_avg,ht_tset_ctrl) ;% T_i_heat_average_cal
v_Tc_avg = max(v_Tc_wk_avg,cl_tset_ctrl); % T_i_cool_average_cal

  */
  }
  void SimModel::ventilationCalc(const Vector& v_Th_avg, const Vector& v_Tc_avg, double frac_hrs_wk_day, Vector& v_Hve_ht, Vector& v_Hve_cl) const {
    double vent_zone_height = std::max(0.1, structure->buildingHeight());
    double qv_supp = ventilation->supplyRate() / structure->floorArea() / 3.6;
    double qv_ext = -(qv_supp - ventilation->supplyDifference() / structure->floorArea() / 3.6);
    double qv_comb = 0;
    double qv_diff = qv_supp + qv_ext + qv_comb;
    double vent_ht_recov = ventilation->heatRecoveryEfficiency();
    double vent_outdoor_frac = 1 - ventilation->exhaustAirRecirculated();
    double tot_env_A = sum(structure->wallArea()) + sum(structure->windowArea());
    /*

  %%  Ventilation
% required energy for mechanical ventilation based on  source EN ISO 13789
% C.3, C.5 and EN 15242:2007 6.7 and EN ISO 13790 Sec 9.2

vent_zone_height=max(0.1, In.stories*In.ftof);  % Ventilztion Zone Height (m) with a minimum of 0.1 m

qv_supp=In.vent_supply_rate./In.cond_flr_area./3.6;   %vent_supply_rate m3/h/m2 (input is in in L/s)
qv_ext=-(qv_supp-In.vent_supply_diff./In.cond_flr_area./3.6);   %vent exhaust rate m3/h/m2, negative indicates out of building

qv_comb = 0 ; % combustion appliance ventilation rate  - not implemented yet but will be impt for restaurants
qv_diff=qv_supp + qv_ext + qv_comb;  % difference between air intake and air exhaust including combustion exhaust

vent_ht_recov=In.vent_heat_recovery; %vent_heat_recovery_eff
vent_outdoor_frac=1-In.vent_recirc_fraction; % fctrl_vent_recirculation

% infilatration source EN 15242:2007 Sec 6.7 direct method
tot_env_A=sum(In.wall_area)+sum(In.win_area);
*/
    double n_p_exp = 0.65;
    /// \todo Note: v_Q75pa, aka infiltrationRate(), is never being calculated or set, so it is, at least in some cases, a
    ///             random value (now that the matrices are properly initialized to 0). So when it goes to 0, the rest
    ///             of the model goes to infinity for the HVAC calculations.
    ///             I'm setting it to non-zero here
    double v_Q75pa = structure->infiltrationRate();
    if (v_Q75pa == 0) {
      v_Q75pa = 0.00000000001;  // this might be a vestige of the rmeove of the "epsilon" code in the translator
    }

    double floorArea = structure->floorArea();
    double v_Q4pa = v_Q75pa * tot_env_A / floorArea * (std::pow((4.0 / 75.0), n_p_exp));

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "v_Q4pa " << v_Q4pa << " v_Q75pa " << v_Q75pa << " tot_env_A " << tot_env_A << " floorArea: " << floorArea << " n_p_exp " << n_p_exp);
#endif

    double n_zone_frac = 0.7;
    double h_stack = n_zone_frac * vent_zone_height;
    double n_stack_exp = 0.667;  //% reset the pressure exponent to 0.667 for this part of the calc
    double n_stack_coeff = 0.0146;
    Vector dbtDiff = dif(location->weather()->mdbt(), v_Th_avg);
    printVector("dbtDiff", dbtDiff);
    Vector dbtDiffAbs = abs(dbtDiff);
    printVector("dbtDiffAbs", dbtDiffAbs);
    Vector dbtHStack = mult(dbtDiffAbs, h_stack);
    printVector("dbtHstack", dbtHStack);
    Vector dbtPowered = pow(dbtHStack, n_stack_exp);
    printVector("dbtPowered", dbtPowered);
    Vector dbtMultQ4 = mult(dbtPowered, n_stack_coeff * v_Q4pa);
    printVector("dbtMultQ4", dbtMultQ4);

    Vector v_qv_stack_ht = maximum(dbtMultQ4, 0.001);  //%qv_stack_heating m3/h/m2
    dbtDiff = dif(location->weather()->mdbt(), v_Tc_avg);
    printVector("dbtDiff", dbtDiff);
    dbtDiffAbs = abs(dbtDiff);
    printVector("dbtDiffAbs", dbtDiffAbs);
    dbtHStack = mult(dbtDiffAbs, h_stack);
    printVector("dbtHstack", dbtHStack);
    dbtPowered = pow(dbtHStack, n_stack_exp);
    printVector("dbtPowered", dbtPowered);
    dbtMultQ4 = mult(dbtPowered, n_stack_coeff * v_Q4pa);
    printVector("dbtMultQ4", dbtMultQ4);
    Vector v_qv_stack_cl = maximum(dbtMultQ4, 0.001);  //%qv_stack_cooling
    printVector("v_qv_stack_ht", v_qv_stack_ht);
    printVector("v_qv_stack_cl", v_qv_stack_cl);

    /*
% infiltration data from
% Tamura, (1976), Studies on exterior wall air tightness and air infiltration of tall buildings, ASHRAE Transactions, 82(1), 122-134.
% Orm (1998), AIVC TN44: Numerical data for air infiltration and natural ventilation calculations, Air Infiltration and Ventilation Centre.
% Emmerich, (2005), Investigation of the Impact of Commercial Building Envelope Airtightness on HVAC Energy Use.
% create a different table for different building types
%n_highrise_inf_table=[4 6 10 15 20];  % infiltration table for high rise buildings as per Tamura, Orm and Emmerich
n_p_exp=0.65; % assumed flow exponent for infiltration pressure conversion
v_Q75pa=In.infilt_rate; % infiltration rate in m3/h/m2 @ 75 Pa based on wall area
v_Q4pa = v_Q75pa.*tot_env_A./In.cond_flr_area*( (4/75).^n_p_exp);  % convert infiltration to Q@4Pa in m3/h /m2 based on floor area

n_zone_frac = 0.7; %fraction that h_stack/zone height.  assume 0.7 as per en 15242

h_stack=n_zone_frac.*vent_zone_height;  % get the effective stack height for infiltration calcs
% calculate the infiltration from stack effect pressure difference from EN 15242: sec 6.7.1
n_stack_exp=0.667;  % reset the pressure exponent to 0.667 for this part of the calc
n_stack_coeff=0.0146;
v_qv_stack_ht=max(n_stack_coeff.*v_Q4pa*(h_stack.*abs(W.mdbt-v_Th_avg)).^n_stack_exp,0.001); %qv_stack_heating m3/h/m2
v_qv_stack_cl=max(n_stack_coeff.*v_Q4pa*(h_stack.*abs(W.mdbt-v_Tc_avg)).^n_stack_exp,0.001); %qv_stack_cooling
*/
    double n_wind_exp = 0.667;
    double n_wind_coeff = 0.0769;
    double n_dCp = 0.75;  // % conventional value for cp difference between windward and leeward sides for low rise buildings as per 15242

    Vector v_qv_wind_ht =
      mult(mult(pow(mult(mult(location->weather()->mwind(), location->weather()->mwind()), n_dCp * location->terrain()), n_wind_exp), v_Q4pa),
           n_wind_coeff);  // % qv_wind_heating
    Vector v_qv_wind_cl =
      mult(mult(pow(mult(mult(location->weather()->mwind(), location->weather()->mwind()), n_dCp * location->terrain()), n_wind_exp), v_Q4pa),
           n_wind_coeff);  // % qv_wind_cooling

    printVector("v_qv_wind_ht", v_qv_wind_ht);
    printVector("v_qv_wind_cl", v_qv_wind_cl);

    double n_sw_coeff = 0.14;
    Vector v_qv_ht_max = maximum(v_qv_stack_ht, v_qv_wind_ht);
    Vector v_qv_cl_max = maximum(v_qv_stack_cl, v_qv_wind_cl);
    printVector("v_qv_ht_max", v_qv_ht_max);
    printVector("v_qv_cl_max", v_qv_cl_max);

    Vector v_qv_sw_ht = sum(v_qv_ht_max, div(mult(mult(v_qv_stack_ht, v_qv_wind_ht), n_sw_coeff), v_Q4pa));  // %qv_sw_heat m3/h/m2
    Vector v_qv_sw_cl = sum(v_qv_cl_max, div(mult(mult(v_qv_stack_cl, v_qv_wind_cl), n_sw_coeff), v_Q4pa));  // %qv_sw_cool m3/h/m2

    printVector("v_qv_sw_ht", v_qv_sw_ht);
    printVector("v_qv_sw_cl", v_qv_sw_cl);

    Vector v_qv_inf_ht = sum(v_qv_sw_ht, std::max(0.0, -qv_diff));  // %q_inf_heat m3/h/m2
    Vector v_qv_inf_cl = sum(v_qv_sw_cl, std::max(0.0, -qv_diff));  // %q_inf_cool m3/h/m2
    printVector("v_qv_inf_ht", v_qv_inf_ht);
    printVector("v_qv_inf_cl", v_qv_inf_cl);

    /*
% calculate infiltration from wind
% note:  terrain_class [1 = open, 0.9 = country, 0.8 =urban]
n_wind_exp=0.667;
n_wind_coeff=0.0769;
n_dCp = 0.75; % conventional value for cp difference between windward and leeward sides for low rise buildings as per 15242

v_qv_wind_ht=n_wind_coeff.*v_Q4pa.*(n_dCp.*In.terrain_class.*W.mwind.^2).^n_wind_exp; % qv_wind_heating
v_qv_wind_cl=n_wind_coeff.*v_Q4pa.*(n_dCp.*In.terrain_class.*W.mwind.^2).^n_wind_exp; % qv_wind_cooling

n_sw_coeff=0.14;
v_qv_sw_ht = max(v_qv_stack_ht,v_qv_wind_ht)+n_sw_coeff.*v_qv_stack_ht.*v_qv_wind_ht./v_Q4pa; %qv_sw_heat m3/h/m2
v_qv_sw_cl = max(v_qv_stack_cl,v_qv_wind_cl)+n_sw_coeff.*v_qv_stack_cl.*v_qv_wind_cl./v_Q4pa; %qv_sw_cool m3/h/m2

v_qv_inf_ht = max(0,-qv_diff)+v_qv_sw_ht; %q_inf_heat m3/h/m2
v_qv_inf_cl = max(0,-qv_diff)+v_qv_sw_cl; %q_inf_cool m3/h/m2


% source EN ISO 13789 C.5  There they use Vdot instead of Q
% Vdot = Vdot_f (1??_v) +Vdot_x
% Vdot_f is the design airflow rate due to mechanical ventilation;
% Vdot_x is the additional airflow rate with fans on, due to wind effects;
% ?_v is the global heat recovery efficiency, taking account of the differences between supply and extract
% airflow rates. Heat in air leaving the building through leakage cannot be recovered.

 % set vent_rate_flag=0 if ventilation rate is constant, 1 if we assume vent off in unoccopied times or
 % 2 if we assume ventilation rate is dropped proportionally to population
 %
 % set to 1 to mimic the behavior of the original spreadsheet
 */
    int vent_rate_flag = 1;
    double vent_op_frac;
    switch (vent_rate_flag) {
      case 0:
        vent_op_frac = 1;
        break;
      case 1:
        vent_op_frac = frac_hrs_wk_day;
        break;
      default:
        vent_op_frac = frac_hrs_wk_day + (1 - frac_hrs_wk_day) * pop->densityOccupied() / pop->densityUnoccupied();
        break;
    }
    /*
vent_rate_flag=1;

% set the operation fraction for the ventilation rate
if vent_rate_flag==0
    vent_op_frac=1;
elseif vent_rate_flag==1
    vent_op_frac=frac_hrs_wk_day;
else
    vent_op_frac=frac_hrs_wk_day+(1-frac_hrs_wk_day)*occ_dens/unocc_dens;
end
*/
    double initVal = ventilation->type() == 3 ? 0 : (vent_op_frac * qv_supp * vent_outdoor_frac * (1 - vent_ht_recov));
    Vector v_qv_mve_ht(12);
    Vector v_qv_mve_cl(12);
    for (size_t i = 0; i < v_qv_mve_ht.size(); i++) {
      v_qv_mve_cl[i] = v_qv_mve_ht[i] = initVal;
    }
    Vector v_qve_ht = sum(v_qv_inf_ht, v_qv_mve_ht);
    Vector v_qve_cl = sum(v_qv_inf_cl, v_qv_mve_cl);
    printVector("v_qve_ht", v_qve_ht);
    printVector("v_qve_cl", v_qve_cl);

    double n_rhoc_air = 1200;

    v_Hve_ht = div(mult(v_qve_ht, n_rhoc_air), 3600.0);
    v_Hve_cl = div(mult(v_qve_cl, n_rhoc_air), 3600.0);
    /*
if In.vent_type==3
    v_qv_mve_ht=zeros(12,1); %qv_me_heating for calc
else
    v_qv_mve_ht=ones(12,1)*vent_op_frac*qv_supp*vent_outdoor_frac*(1-vent_ht_recov);
end

%qv_f_cl=ones(12,1)*qv_supp*vent_outdoor_frac; %qv_me_cooling
if In.vent_type==3
    v_qv_mve_cl=zeros(12,1); %qv_me_cooling for calc
else
    v_qv_mve_cl=ones(12,1)*vent_op_frac*qv_supp*vent_outdoor_frac*(1-vent_ht_recov);
end

v_qve_ht = v_qv_inf_ht+v_qv_mve_ht; %total air flow in m3/s when heating
v_qve_cl = v_qv_inf_cl+v_qv_mve_cl; %total air flow in m3/s when cooling

n_rhoc_air = 1200;  % heat capacity of air per unit volume in J/(m3 K)
v_Hve_ht = n_rhoc_air*v_qve_ht/3600;  % Hve (W/K/m2)  heating
v_Hve_cl = n_rhoc_air*v_qve_cl/3600; % Hve (W/K/m2)  cooling


  */
  }
  void SimModel::heatingAndCooling(const Vector& v_E_sol, const Vector& v_Th_avg, const Vector& v_Hve_ht, const Vector& v_Tc_avg,
                                   const Vector& v_Hve_cl, double tau, double H_tr, double phi_I_tot, double frac_hrs_wk_day, Vector& v_Qfan_tot,
                                   Vector& v_Qneed_ht, Vector& v_Qneed_cl, double& Qneed_ht_yr, double& Qneed_cl_yr) const {
    Vector temp = mult(megasecondsInMonth, phi_I_tot, 12);
    Vector v_tot_mo_ht_gain = sum(temp, v_E_sol);

    double a_H0 = 1;
    double tau_H0 = 15;
    double a_H = a_H0 + tau / tau_H0;

    Vector v_QT_ht = mult(mult(dif(v_Th_avg, location->weather()->mdbt()), megasecondsInMonth), H_tr);
    Vector v_QV_ht = mult(mult(mult(v_Hve_ht, structure->floorArea()), dif(v_Th_avg, location->weather()->mdbt())), megasecondsInMonth);
    Vector v_Qtot_ht = sum(v_QT_ht, v_QV_ht);
    /*
  %% Heating and Cooling Needs

%total monthly heat gains (MJ)

v_tot_mo_ht_gain = phi_I_tot*v_Msec_ina_mo + v_E_sol;  % total_heat_gain = total internal + total solar in MJ/m2.

% compute the heating need including thermal mass effects
% NOTE: the building heat thermal time constant, tau, was calculated in the section
% on interior temperature
a_H0=1; % a_H_0 = reference dimensionless parameter
tau_H0=15; % tau_H_0 = reference time constant
a_H = a_H0+tau/tau_H0;  %a_H_building heating dimensionless constant

v_QT_ht = H_tr.*(v_Th_avg-v_mdbt).*v_Msec_ina_mo;  %QT = transmission loss (MJ)
v_QV_ht = v_Hve_ht*In.cond_flr_area.*(v_Th_avg-v_mdbt).*v_Msec_ina_mo; % QV in MJ
v_Qtot_ht = v_QT_ht+v_QV_ht ; %QL_total total heat loss in MJ
*/
    Vector v_gamma_H_ht = div(v_tot_mo_ht_gain, sum(v_Qtot_ht, std::numeric_limits<double>::min()));
    Vector v_eta_g_H(12);
    for (size_t i = 0; i < v_eta_g_H.size(); i++) {
      v_eta_g_H[i] = v_gamma_H_ht(i) > 0 ? (1 - std::pow(v_gamma_H_ht[i], a_H)) / (1 - std::pow(v_gamma_H_ht[i], (a_H + 1)))
                                         : 1 / (v_gamma_H_ht(i) + std::numeric_limits<double>::min());
    }
    v_Qneed_ht = dif(v_Qtot_ht, mult(v_eta_g_H, v_tot_mo_ht_gain));
    Qneed_ht_yr = sum(v_Qneed_ht);

    /*
% compute the ratio of heat gain to heating loss, gamma_H
v_gamma_H_ht = v_tot_mo_ht_gain./(v_Qtot_ht+eps);  %gamma_H = QG/QL  - eps added to avoid divide by zero problem

% for each month, check the heat gain ratio and set the heating gain
% utilization factor, eta_g_H accordingly
v_eta_g_H=zeros(12,1);
for I=1:12
    if v_gamma_H_ht(I)>0
        v_eta_g_H(I) = (1-v_gamma_H_ht(I).^a_H)./(1-v_gamma_H_ht(I).^(a_H+1));  % eta_g_H heating gain utilization factor
    else
        v_eta_g_H(I) = 1./(v_gamma_H_ht(I)+eps);
    end
end
v_Qneed_ht = v_Qtot_ht - v_eta_g_H.*v_tot_mo_ht_gain; %QNH = QL,H - eta_G_H.*Q_G_H

Qneed_ht_yr = sum(v_Qneed_ht);
   */

    Vector v_QT_cl = mult(mult(dif(v_Tc_avg, location->weather()->mdbt()), H_tr), megasecondsInMonth);  // % QT for cooling in MJ
    Vector v_QV_cl =
      mult(mult(mult(v_Hve_cl, structure->floorArea()), dif(v_Tc_avg, location->weather()->mdbt())), megasecondsInMonth);  // % QT for coolin in MJ
    Vector v_Qtot_cl = sum(v_QT_cl, v_QV_cl);  // % QL = QT + QV for cooling = total cooling heat loss in MJ

    Vector v_gamma_H_cl = div(v_Qtot_cl, sum(v_tot_mo_ht_gain, std::numeric_limits<double>::min()));  //  %gamma_C = heat loss ratio Qloss/Qgain

    //% compute the cooling gain utilization factor eta_g_cl
    Vector v_eta_g_CL(12);
    for (size_t i = 0; i < v_eta_g_CL.size(); i++) {
#ifdef DEBUG_ISO_MODEL_SIMULATION
      double numer = (1.0 - std::pow(v_gamma_H_cl[i], a_H));
      //double denom = (1.0-std::pow(v_gamma_H_cl[i],(a_H+1.0)));
      LOG(Trace, numer << " = 1.0 - " << v_gamma_H_cl[i] << "^" << a_H);
      LOG(Trace, numer << " = 1.0 - " << v_gamma_H_cl[i] << "^" << (a_H + 1.0));
#endif

      v_eta_g_CL[i] = v_gamma_H_cl(i) > 0.0 ? (1.0 - std::pow(v_gamma_H_cl[i], a_H)) / (1.0 - std::pow(v_gamma_H_cl[i], (a_H + 1.0))) : 1.0;
    }

    v_Qneed_cl = dif(v_tot_mo_ht_gain, mult(v_eta_g_CL, v_Qtot_cl));  // % QNC = Q_G_C - eta*Q_L_C = total cooling need
    Qneed_cl_yr = sum(v_Qneed_cl);
    /*
% n_a_C0 = 1; %a_C_0 building cooling reference constant
% n_tau_C0 = 15;%tau_C_0 building cooling reference time constant
% C366 = n_a_C0+tau/n_tau_C0; % a_C = building cooling constant

v_QT_cl = H_tr*(v_Tc_avg - v_mdbt).*v_Msec_ina_mo; % QT for cooling in MJ
v_QV_cl = v_Hve_cl*In.cond_flr_area.*(v_Tc_avg - v_mdbt).*v_Msec_ina_mo; % QT for coolin in MJ
v_Qtot_cl = v_QT_cl+v_QV_cl; % QL = QT + QV for cooling = total cooling heat loss in MJ

v_gamma_H_cl = v_Qtot_cl./(v_tot_mo_ht_gain+eps);  %gamma_C = heat loss ratio Qloss/Qgain

% compute the cooling gain utilization factor eta_g_cl
v_eta_g_CL=zeros(12,1);
for I=1:12 % for each month
    if v_gamma_H_cl(I)>0
        v_eta_g_CL(I) = (1-v_gamma_H_cl(I).^a_H)./(1-v_gamma_H_cl(I).^(a_H+1));  % eta_g_H cooling gain utilization factor
    else
        v_eta_g_CL(I) = 1;
    end
end

v_Qneed_cl = v_tot_mo_ht_gain - v_eta_g_CL.*v_Qtot_cl; % QNC = Q_G_C - eta*Q_L_C = total cooling need
Qneed_cl_yr=sum(v_Qneed_cl);
*/
    double n_dT_supp_ht = 7.0;                                                //% set heating temp diff between supply air and room air
    double n_dT_supp_cl = 7.0;                                                //%set cooling temp diff between supply air and room air
    double T_sup_ht = heating->temperatureSetPointOccupied() + n_dT_supp_ht;  //%hot air supply temp  - assume supply air is 7C hotter than room
    double T_sup_cl = cooling->temperatureSetPointOccupied() - n_dT_supp_cl;  //%cool air supply temp - assume 7C lower than room
    double n_rhoC_a = 1.22521 * 0.001012;
    /*
%% Fan Energy

n_dT_supp_ht=7;  % set heating temp diff between supply air and room air
n_dT_supp_cl=7; %set cooling temp diff between supply air and room air
T_sup_ht = In.ht_tset_occ+n_dT_supp_ht;  %hot air supply temp  - assume supply air is 7C hotter than room
T_sup_cl = In.cl_tset_occ-n_dT_supp_cl;  %cool air supply temp - assume 7C lower than room

n_rhoC_a = 1.22521.*0.001012; % rho*Cp for air (MJ/m3/K)
*/

    Vector v_Vair_ht = div(v_Qneed_ht, sum(mult(dif(T_sup_ht, v_Th_avg), n_rhoC_a), std::numeric_limits<double>::min()));
    Vector v_Vair_cl = div(v_Qneed_cl, sum(mult(dif(v_Tc_avg, T_sup_cl), n_rhoC_a), std::numeric_limits<double>::min()));
    ventilation->fanPower();
    ventilation->fanControlFactor();
    structure->floorArea();
    printVector("v_Vair_ht", v_Vair_ht);
    printVector("v_Vair_cl", v_Vair_cl);

    Vector v_Vair_tot = maximum(sum(v_Vair_ht, v_Vair_cl),
                                div(mult(megasecondsInMonth, ventilation->supplyRate() * frac_hrs_wk_day, 12), 1000));  //% compute air flow in m3
    printVector("v_Vair_tot", v_Vair_tot);
    Vector fanPower = mult(v_Vair_tot, ventilation->fanPower() * ventilation->fanControlFactor());
    printVector("fanPower", fanPower);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "ventilation->fanPower() = " << ventilation->fanPower());
    LOG(Trace, "ventilation->fanControlFactor() = " << ventilation->fanControlFactor());
    LOG(Trace, "structure->floorArea() = " << structure->floorArea());
#endif

    v_Qfan_tot = div(div(fanPower, structure->floorArea()), 3600);  //% compute fan energy in kWh/m2

    /*
v_Vair_ht = v_Qneed_ht./(n_rhoC_a.*(T_sup_ht -v_Th_avg)+eps);  %compute volume of air moved for heating
v_Vair_cl = v_Qneed_cl./(n_rhoC_a.*(v_Tc_avg - T_sup_cl)+eps); % compute volume of air moved for cooling

v_Vair_tot=max((v_Vair_ht+v_Vair_cl),In.vent_supply_rate.*frac_hrs_wk_day.*v_Msec_ina_mo./1000); % compute air flow in m3
v_Qfan_tot = v_Vair_tot.*In.fan_specific_power.*In.fan_flow_ctrl_factor./In.cond_flr_area./3600;  % compute fan energy in kWh/m2
*/
  }
  void SimModel::hvac(const Vector& v_Qneed_ht, const Vector& v_Qneed_cl, double Qneed_ht_yr, double Qneed_cl_yr, Vector& v_Qelec_ht,
                      Vector& v_Qgas_ht, Vector& v_Qcl_elec_tot, Vector& v_Qcl_gas_tot) const {
    double DH_YesNo = 0;
    double n_eta_DH_network = 0.9;
    double n_eta_DH_sys = 0.87;
    double n_frac_DH_free = 0.000;

    double DC_YesNo = 0;
    double n_eta_DC_network = 0.9;
    double n_eta_DC_COP = 5.5;
    double n_eta_DC_frac_abs = 0;
    double n_eta_DC_COP_abs = 1;
    double n_frac_DC_free = 0;
    /*
  %% District H/C info

DH_YesNo =0;  % building connected to DH (0=no, 1=yes.  Assume DH is powered by natural gas)
n_eta_DH_network = 0.9; % efficiency of DH network.  Typical value 0l75-0l9 EN 15316-4-5
n_eta_DH_sys = 0.87; % efficiency of DH heating system
n_frac_DH_free = 0.000; % fraction of free heat source to DH (0 to 1)

DC_YesNo = 0;  % building connected to DC (0=no, 1=yes)
n_eta_DC_network = 0.9;  % efficiency of DC network.
n_eta_DC_COP = 5.5;  % COP of DC elec Chillers
n_eta_DC_frac_abs = 0;  % fraction of DC chillers that are absorption
n_eta_DC_COP_abs = 1;  % COP of DC absorption chillers
n_frac_DC_free = 0;  % fraction of free heat source to absorption DC chillers (0 to 1)
*/
    double IEER = cooling->cop() * cooling->partialLoadValue();
    double f_waste = heating->hotcoldWasteFactor();
    double a_ht_loss = heating->hvacLossFactor();
    double a_cl_loss = cooling->hvacLossFactor();

    /*  %% HVAC System
%
% From EN 15243-2007 Annex E.
% HVAC system info table from EN 15243:2007 Table E1.  columns are

% SEER = COP *mPLV  or maybe more properly , IEER = COP * IPLV
IEER = In.COP*In.PLV ; % compute IEER the effective average COP for the cooling system

% copy over the HVAC loss/waste factors into local variables with names
% that match the equations better
f_waste=In.hotcold_waste_factor;
a_ht_loss=In.heat_loss_factor;
a_cl_loss=In.cool_loss_factor;

*/
    double f_dem_ht = std::max(Qneed_ht_yr / (Qneed_cl_yr + Qneed_ht_yr), 0.1);
    double f_dem_cl = std::max((1.0 - f_dem_ht), 0.1);
    double eta_dist_ht = 1.0 / (1.0 + a_ht_loss + f_waste / f_dem_ht);  //% overall distribution efficiency for heating
    double eta_dist_cl = 1.0 / (1.0 + a_cl_loss + f_waste / f_dem_cl);  //%overall distrubtion efficiency for cooling

    Vector v_Qloss_ht_dist = div(mult(v_Qneed_ht, (1 - eta_dist_ht)), eta_dist_ht);
    Vector v_Qloss_cl_dist = div(mult(v_Qneed_cl, (1 - eta_dist_cl)), eta_dist_cl);
    printVector("v_Qloss_ht_dist", v_Qloss_ht_dist);
    printVector("v_Qloss_cl_dist", v_Qloss_cl_dist);
    /*
f_dem_ht=max(Qneed_ht_yr/(Qneed_cl_yr+Qneed_ht_yr),0.1); %fraction of yearly heating demand with regard to total heating + cooling demand
f_dem_cl=max((1-f_dem_ht),0.1); %fraction of yearly cooling demand
eta_dist_ht  =1.0/(1.0+a_ht_loss+f_waste/f_dem_ht); % overall distribution efficiency for heating
eta_dist_cl = 1.0/(1.0+a_cl_loss+f_waste/f_dem_cl); %overall distrubtion efficiency for cooling

v_Qloss_ht_dist=v_Qneed_ht*(1-eta_dist_ht)/eta_dist_ht;  %losses from HVAC heat distribution
v_Qloss_cl_dist = v_Qneed_cl*(1-eta_dist_cl)/eta_dist_cl;  %losses from HVAC cooling distribution
*/
    Vector v_Qht_sys(12);
    Vector v_Qht_DH(12);
    Vector v_Qcl_sys(12);
    Vector v_Qcool_DC(12);
    zero(v_Qht_sys);
    zero(v_Qht_DH);
    zero(v_Qcl_sys);
    zero(v_Qcool_DC);
    // TODO: always true right now
    // cppcheck-suppress knownConditionTrueFalse
    if (DH_YesNo == 1) {
      v_Qht_DH = sum(v_Qneed_ht, v_Qloss_ht_dist);
    } else {
      v_Qht_sys = div(sum(v_Qloss_ht_dist, v_Qneed_ht), heating->efficiency() + std::numeric_limits<double>::min());
    }

    // TODO: always true right now
    // cppcheck-suppress knownConditionTrueFalse
    if (DC_YesNo == 1) {
      v_Qcool_DC = sum(v_Qneed_cl, v_Qloss_cl_dist);
    } else {
      v_Qcl_sys = div(sum(v_Qloss_cl_dist, v_Qneed_cl), IEER + std::numeric_limits<double>::min());
    }
    printVector("v_Qht_sys", v_Qht_sys);
    printVector("v_Qht_DH", v_Qht_DH);
    printVector("v_Qcl_sys", v_Qcl_sys);
    printVector("v_Qcool_DC", v_Qcool_DC);
    /*
if DH_YesNo==1
    v_Qht_sys = zeros(12,1);  % if we have district heating our heating energy needs from our system are zero
    v_Qht_DH = v_Qneed_ht+v_Qloss_ht_dist;  %Q_heat_nd for DH
else
    v_Qht_sys =(v_Qloss_ht_dist+v_Qneed_ht)/(In.heat_sys_eff+eps);  % total heating energy need from our system including losses
    v_Qht_DH = zeros(12,1);
end

if DC_YesNo==1
    v_Qcl_sys = zeros(12,1);  % if we have district cooling our cooling energy needs from our system are zero
    v_Qcool_DC = v_Qloss_cl_dist+v_Qneed_cl;  % if we have DC the cooling needs are the dist losses + the cooling needs themselves
else
    v_Qcl_sys =(v_Qloss_cl_dist+v_Qneed_cl)/(IEER+eps);  % if no DC compute our total system cooling energy needs including losses
    v_Qcool_DC=zeros(12,1); % if no DC, DC cooling needs are zero
end


*/
    Vector v_Qcl_DC_elec = div(mult(v_Qcool_DC, 1 - n_eta_DC_frac_abs), n_eta_DC_COP * n_eta_DC_network);
    Vector v_Qcl_DC_abs = div(mult(v_Qcool_DC, 1 - n_frac_DC_free), n_eta_DC_COP_abs);
    printVector("v_Qcl_DC_elec", v_Qcl_DC_elec);
    printVector("v_Qcl_DC_abs", v_Qcl_DC_abs);

    // cppcheck-suppress invalidFunctionArg
    Vector v_Qht_DH_total = div(mult(v_Qht_DH, 1 - n_frac_DH_free), n_eta_DH_sys * n_eta_DH_network);
    v_Qcl_elec_tot = sum(v_Qcl_sys, v_Qcl_DC_elec);
    v_Qcl_gas_tot = v_Qcl_DC_abs;
    printVector("v_Qht_DH_total", v_Qht_DH_total);
    printVector("v_Qcl_elec_tot", v_Qcl_elec_tot);
    printVector("v_Qcl_gas_tot", v_Qcl_gas_tot);

    //Vector v_Qelec_ht,v_Qgas_ht;

    if (heating->energyType() == 1) {
      v_Qelec_ht = v_Qht_sys;
      v_Qgas_ht = v_Qht_DH_total;
    } else {
      v_Qelec_ht = Vector(12);
      zero(v_Qelec_ht);
      v_Qgas_ht = sum(v_Qht_sys, v_Qht_DH_total);
    }
    printVector("v_Qelec_ht", v_Qelec_ht);
    printVector("v_Qgas_ht", v_Qgas_ht);

    /*
v_Qcl_DC_elec = v_Qcool_DC * (1-n_eta_DC_frac_abs) / (n_eta_DC_COP*n_eta_DC_network);  % Energy used for cooling by district electric chillers
v_Qcl_DC_abs =  v_Qcool_DC * (1-n_frac_DC_free) / n_eta_DC_COP_abs; %Energy used for cooling by district absorption chillers

v_Qht_DH_total = v_Qht_DH * (1 - n_frac_DH_free) / (n_eta_DH_sys * n_eta_DH_network);
v_Qcl_elec_tot = v_Qcl_sys + v_Qcl_DC_elec; %total electric cooling energy (MJ)
v_Qcl_gas_tot = v_Qcl_DC_abs; % total gas cooliing energy

 if In.heat_energy_type==1  %check if fuel type is electric
     v_Qelec_ht=v_Qht_sys;  % total electric heating energy (MJ)
     v_Qgas_ht=v_Qht_DH_total; % total gas heating energy is DH if fuel type is electric
 else
     v_Qelec_ht = zeros(12,1);  % if we get here, fuel was gas to total electric heating energy is 0
     v_Qgas_ht=v_Qht_sys+v_Qht_DH_total;  % total gas heating energy is building + any DH
 end

  */
  }
  void SimModel::pump(const Vector& v_Qneed_ht, const Vector& v_Qneed_cl, double Qneed_ht_yr, double Qneed_cl_yr, Vector& v_Q_pump_tot) const {

    /*
       %% Pump Energy

       %%% NOTE original GIT spreadsheet had this hardwired
       %Q_pumps_yr = 8;  %  set pump energy density 8 MJ/m2/yr
       %
       %
       %  new GIT model following EPA NR 2007 6.9.7.1 and 6.9.7.2
       % European Performance Assessment - Non Residential

*/
    double n_E_pumps = 0.25;
    Vector v_Q_pumps = mult(megasecondsInMonth, n_E_pumps, 12);
    double Q_pumps_yr = sum(v_Q_pumps);

    Vector v_frac_ht_mode = div(v_Qneed_ht, sum(v_Qneed_ht, v_Qneed_cl));
    double frac_ht_total = sum(v_frac_ht_mode);
    double Q_pumps_ht = Q_pumps_yr * heating->pumpControlReduction() * structure->floorArea();
    Vector v_Q_pumps_ht = div(mult(v_frac_ht_mode, Q_pumps_ht), frac_ht_total);
    /*
       n_E_pumps = 0.25;  % specific power of systems pumps + control systems in W/m2
       v_Q_pumps=n_E_pumps*v_Msec_ina_mo;  % energy per month for pumps + control if running continuously in MJ/m2/mo

       Q_pumps_yr=sum(v_Q_pumps);% total annual energy for pumps+ctrl if running continuously MJ/m2/yr


       v_frac_ht_mode = v_Qneed_ht./(v_Qneed_ht+v_Qneed_cl);  %fraction of time system is in in heating mode each month
       frac_ht_total=sum(v_frac_ht_mode);  % total heating pump energy fraction
       Q_pumps_ht=Q_pumps_yr*In.pump_heat_ctrl_factor*In.cond_flr_area; % total heating pump energy;
       v_Q_pumps_ht = Q_pumps_ht*v_frac_ht_mode/frac_ht_total;  % break down total according to fractional operation and monthly fraction

       %v_frac_pump_ht = v_Qneed_ht./(v_Qneed_ht+v_Qneed_cl);  %heating pump operation fraction for each month.
       %v_Q_pump_mo=Q_pumps_yr*In.pump_heat_ctrl_factor*In.cond_flr_area.*v_frac_ht_mode;

*/
    Vector v_frac_cl_mode = div(v_Qneed_cl, sum(v_Qneed_ht, v_Qneed_cl));
    double frac_cl_total = sum(v_frac_cl_mode);
    double Q_pumps_cl = Q_pumps_yr * cooling->pumpControlReduction() * structure->floorArea();
    Vector v_Q_pumps_cl = div(mult(v_frac_cl_mode, Q_pumps_cl), frac_cl_total);

    /*
       v_frac_cl_mode = v_Qneed_cl./(v_Qneed_ht+v_Qneed_cl);% fraction of time system is in cooling mode
       frac_cl_total=sum(v_frac_cl_mode); % total cooling pump energy fraction
       Q_pumps_cl=Q_pumps_yr*In.pump_cool_ctrl_factor*In.cond_flr_area; % total cooling pump energy fraction
       v_Q_pumps_cl = Q_pumps_cl*v_frac_cl_mode/frac_cl_total; % break down total into monthly fractional parts

       %v_frac_pump_cl = v_Qneed_cl./(v_Qneed_ht+v_Qneed_cl);% cooling pump operation factor

*/
    Vector v_frac_tot = div(sum(v_Qneed_ht, v_Qneed_cl), Qneed_ht_yr + Qneed_cl_yr);
    if (Q_pumps_ht == 0 || Q_pumps_cl == 0) {
      v_Q_pump_tot = sum(v_Q_pumps_ht, v_Q_pumps_cl);
    } else {
      const double Q_pumps_tot = Q_pumps_ht + Q_pumps_cl;
      const double frac_total = sum(v_frac_tot);
      v_Q_pump_tot = div(mult(v_frac_tot, Q_pumps_tot), frac_total);
    }
    /*
       v_frac_tot = (v_Qneed_ht+v_Qneed_cl)/(Qneed_ht_yr+Qneed_cl_yr); % total pump operational factor
       frac_total = sum(v_frac_tot);
       Q_pumps_tot = Q_pumps_ht + Q_pumps_cl;

       if (Q_pumps_ht==0 || Q_pumps_cl==0)
       v_Q_pump_tot = v_Q_pumps_ht+v_Q_pumps_cl;
       else
       v_Q_pump_tot = Q_pumps_tot*v_frac_tot/frac_total;
       end

       %Q_pump_tot_yr=sum(v_Q_pump_tot);

*/
  }

  void SimModel::energyGeneration() const {
    /*
       %% Energy Generation

       %%% NOT INCLUDED YET
       */
  }

  void SimModel::heatedWater(Vector& v_Q_dhw_elec, Vector& v_Q_dhw_gas) const {
    double n_dhw_tset = 60;     // % water temperature set point (C)
    double n_dhw_tsupply = 20;  //% water initial temp (C)
    double n_CP_h20 = 4.18;     //% specific heat of water in MJ/m3/K
    Vector v_Q_dhw_solar(12);
    zero(v_Q_dhw_solar);  //Q from solar energy hot water collectors - not included yet
    double Q_dhw_yr = heating->hotWaterDemand() * (n_dhw_tset - n_dhw_tsupply) * n_CP_h20;

    /*%% DHW and Solar Water Heating
 %
 % Qdhw= ((Qdem;DWH/?sys;DHW) - Qses;DHW)/?gen;DHW
 % Source: NEN 2916 12.2

n_dhw_tset = 60; % water temperature set point (C)
n_dhw_tsupply = 20; % water initial temp (C)
n_CP_h20=4.18;  % specific heat of water in MJ/m3/K

%solar hot water heating contribution
%D738 =0; % solar collector surface area
v_Q_dhw_solar =zeros(12,1);  % Q from solar energy hot water collectors - not included yet


Q_dhw_yr = In.DHW_demand*(n_dhw_tset-n_dhw_tsupply).*n_CP_h20; % total annual energy required for heating DHW MJ/yr

% n_dhw_dist_eff_table=[1 0.8 0.6]; % all taps < 3m from gen = 1, taps> 3m = 0.8, circulation or unknown =0.6
% %eta_dhw_dist = n_dhw_dist_eff_table(In.DHW_dist_sys_type); % set the distribution efficiency from table
%
% eta_dhw_dist = In.DHW_dist_eff; % DHW distribtuion efficiency
% eta_dhw_sys = In.DHW_sys_eff; % DHW system efficiency


*/
    Vector v_MonthlyDemand = mult(daysInMonth, Q_dhw_yr, 12);
    Vector v_frac_MonthlyDemand_yr = div(v_MonthlyDemand, daysInYear);
    Vector v_Qe_demand = div(v_frac_MonthlyDemand_yr, heating->hotWaterDistributionEfficiency());
    Vector v_Q_dhw_demand = div(v_Qe_demand, kWh2MJ);
    Vector v_Q_dhw_need = maximum(div(dif(v_Q_dhw_demand, v_Q_dhw_solar), heating->hotWaterSystemEfficiency()), 0);
    Vector Z(v_Q_dhw_need.size());
    printVector("v_MonthlyDemand", v_MonthlyDemand);
    printVector("v_frac_MonthlyDemand_yr", v_frac_MonthlyDemand_yr);
    printVector("v_Qe_demand", v_Qe_demand);
    printVector("v_Q_dhw_demand", v_Q_dhw_demand);
    printVector("v_Q_dhw_need", v_Q_dhw_need);
    zero(Z);
    printVector("Z", Z);

    if (heating->hotWaterEnergyType() == 1) {
      v_Q_dhw_elec = v_Q_dhw_need;
      v_Q_dhw_gas = Z;
    } else {
      v_Q_dhw_gas = v_Q_dhw_need;
      v_Q_dhw_elec = Z;
    }
    printVector("v_Q_dhw_gas", v_Q_dhw_gas);
    printVector("v_Q_dhw_elec", v_Q_dhw_elec);

    /*
v_Q_dhw_demand = Q_dhw_yr.*v_days_ina_mo./days_ina_year./In.DHW_dist_eff/kWh2MJ; % monthly DHW energy demand including distribution inefficiency
v_Q_dhw_need = max((v_Q_dhw_demand-v_Q_dhw_solar)./In.DHW_sys_eff,0); % total monthly supply need is (demand - solar)/system efficiency

Z=zeros(size(v_Q_dhw_need));
if(In.DHW_energy_type)==1
    v_Q_dhw_elec = v_Q_dhw_need;
    v_Q_dhw_gas = Z;
else
    v_Q_dhw_gas = v_Q_dhw_need;
    v_Q_dhw_elec = Z;
end
  */
  }

  ISOResults SimModel::simulate() const {
    Vector weekdayOccupiedMegaseconds(12);
    Vector weekdayUnoccupiedMegaseconds(12);
    Vector weekendOccupiedMegaseconds(12);
    Vector weekendUnoccupiedMegaseconds(12);
    Vector clockHourOccupied(24);
    Vector clockHourUnoccupied(24);
    double frac_hrs_wk_day = 0;
    double hoursUnoccupiedPerDay = 0;
    double hoursOccupiedPerDay = 0;
    double frac_hrs_wk_nt = 0;
    double frac_hrs_wke_tot = 0;

    //Solor Radiation Breakdown Results
    Vector v_hrs_sun_down_mo(12);
    Vector v_Tdbt_nt;
    Vector frac_Pgh_wk_nt;
    Vector frac_Pgh_wke_day;
    Vector frac_Pgh_wke_nt;
    //Envelop Calculations Results
    Vector v_win_A;
    Vector v_wall_emiss;
    Vector v_wall_alpha_sc;
    Vector v_wall_U;
    Vector v_wall_A;

    Vector v_wall_A_sol;
    Vector v_win_hr;
    Vector v_wall_R_sc;
    Vector v_win_A_sol;

    double Q_illum_occ;
    double Q_illum_unocc;
    double Q_illum_tot_yr;

    double phi_int_avg;
    double phi_plug_avg;
    double phi_illum_avg;

    double phi_int_wk_nt;
    double phi_int_wke_day;
    double phi_int_wke_nt;
    Vector v_E_sol;

    double H_tr;
    Vector v_P_tot_wke_day;
    Vector v_P_tot_wk_nt;
    Vector v_P_tot_wke_nt;

    Vector v_Th_avg(12);
    Vector v_Tc_avg(12);

    double phi_I_tot;
    double tau;
    Vector v_Hve_ht;
    Vector v_Hve_cl;

    double Qneed_ht_yr;
    double Qneed_cl_yr;
    Vector v_Qneed_ht;
    Vector v_Qneed_cl;

    Vector v_Qelec_ht;
    Vector v_Qcl_elec_tot;
    Vector v_Q_illum_tot;
    Vector v_Q_illum_ext_tot;
    Vector v_Qfan_tot;
    Vector v_Q_pump_tot;
    Vector v_Q_dhw_elec;
    Vector v_Qgas_ht;
    Vector v_Qcl_gas_tot;
    Vector v_Q_dhw_gas;

    frac_hrs_wk_day = hoursUnoccupiedPerDay = hoursOccupiedPerDay = frac_hrs_wk_nt = frac_hrs_wke_tot = 1;

    //openstudio::isomodel::loadDefaults(simModel);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "scheduleAndOccupancy: ");
#endif

    scheduleAndOccupancy(weekdayOccupiedMegaseconds, weekdayUnoccupiedMegaseconds, weekendOccupiedMegaseconds, weekendUnoccupiedMegaseconds,
                         clockHourOccupied, clockHourUnoccupied, frac_hrs_wk_day, hoursUnoccupiedPerDay, hoursOccupiedPerDay, frac_hrs_wk_nt,
                         frac_hrs_wke_tot);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "frac_hrs_wk_day: " << frac_hrs_wk_day);
    LOG(Trace, "hoursUnoccupiedPerDay: " << hoursUnoccupiedPerDay);
    LOG(Trace, "hoursOccupiedPerDay: " << hoursOccupiedPerDay);
    LOG(Trace, "frac_hrs_wk_nt: " << frac_hrs_wk_nt);
    LOG(Trace, "frac_hrs_wke_tot: " << frac_hrs_wke_tot);

    printVector("weekdayOccupiedMegaseconds", weekdayOccupiedMegaseconds);
    printVector("weekdayUnoccupiedMegaseconds", weekdayUnoccupiedMegaseconds);
    printVector("weekendOccupiedMegaseconds", weekendOccupiedMegaseconds);
    printVector("weekendUnoccupiedMegaseconds", weekendUnoccupiedMegaseconds);
    printVector("clockHourOccupied", clockHourOccupied);
    printVector("clockHourUnoccupied", clockHourUnoccupied);

    LOG(Trace, "solarRadiationBreakdown: ");
#endif

    solarRadiationBreakdown(weekdayOccupiedMegaseconds, weekdayUnoccupiedMegaseconds, weekendOccupiedMegaseconds, weekendUnoccupiedMegaseconds,
                            clockHourOccupied, clockHourUnoccupied, v_hrs_sun_down_mo, frac_Pgh_wk_nt, frac_Pgh_wke_day, frac_Pgh_wke_nt, v_Tdbt_nt);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_hrs_sun_down_mo", v_hrs_sun_down_mo);
    printVector("frac_Pgh_wk_nt", frac_Pgh_wk_nt);
    printVector("frac_Pgh_wke_day", frac_Pgh_wke_day);
    printVector("frac_Pgh_wke_nt", frac_Pgh_wke_nt);
    printVector("v_Tdbt_nt", v_Tdbt_nt);

    LOG(Trace, "lightingEnergyUse: ");
#endif

    lightingEnergyUse(v_hrs_sun_down_mo, Q_illum_occ, Q_illum_unocc, Q_illum_tot_yr, v_Q_illum_tot, v_Q_illum_ext_tot);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "Q_illum_occ: " << Q_illum_occ);
    LOG(Trace, "Q_illum_unocc: " << Q_illum_unocc);
    LOG(Trace, "Q_illum_unocc: " << Q_illum_unocc);
    printVector("v_Q_illum_tot", v_Q_illum_tot);
    printVector("v_Q_illum_ext_tot", v_Q_illum_ext_tot);

    LOG(Trace, "envelopCalculations: "); /*
                                                                      v_wall_A = structure->wallArea();
                                                                      v_win_A = structure->windowArea();
                                                                      v_wall_U = structure->wallUniform();
                                                                      Vector v_win_U = structure->windowUniform();*/
    printVector("structure->wallArea()", structure->wallArea());
    printVector("structure->windowArea()", structure->windowArea());
    printVector("structure->wallUniform()", structure->wallUniform());
    printVector("structure->windowUniform()", structure->windowUniform());
#endif

    envelopCalculations(v_win_A, v_wall_emiss, v_wall_alpha_sc, v_wall_U, v_wall_A, H_tr);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "H_tr: " << H_tr);
    printVector("v_win_A", v_win_A);
    printVector("v_wall_emiss", v_wall_emiss);
    printVector("v_wall_alpha_sc", v_wall_alpha_sc);
    printVector("v_wall_U", v_wall_U);
    printVector("v_wall_A", v_wall_A);

    LOG(Trace, "windowSolarGain: ");
#endif

    windowSolarGain(v_win_A, v_wall_emiss, v_wall_alpha_sc, v_wall_U, v_wall_A, v_wall_A_sol, v_win_hr, v_wall_R_sc, v_win_A_sol);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_wall_A_sol", v_wall_A_sol);
    printVector("v_win_hr", v_win_hr);
    printVector("v_wall_R_sc", v_wall_R_sc);
    printVector("v_win_A_sol", v_win_A_sol);

    LOG(Trace, "solarHeatGain: ");
#endif

    solarHeatGain(v_win_A_sol, v_wall_R_sc, v_wall_U, v_wall_A, v_win_hr, v_wall_A_sol, v_E_sol);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_E_sol", v_E_sol);

    LOG(Trace, "heatGainsAndLosses: ");
#endif

    heatGainsAndLosses(frac_hrs_wk_day, Q_illum_occ, Q_illum_unocc, Q_illum_tot_yr, phi_int_avg, phi_plug_avg, phi_illum_avg, phi_int_wke_nt,
                       phi_int_wke_day, phi_int_wk_nt);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "phi_int_avg: " << phi_int_avg);
    LOG(Trace, "phi_plug_avg: " << phi_plug_avg);
    LOG(Trace, "phi_illum_avg: " << phi_illum_avg);
    LOG(Trace, "phi_int_wke_nt: " << phi_int_wke_nt);
    LOG(Trace, "phi_int_wke_day: " << phi_int_wke_day);
    LOG(Trace, "phi_int_wk_nt: " << phi_int_wk_nt);

    LOG(Trace, "internalHeatGain: ");
#endif

    internalHeatGain(phi_int_avg, phi_plug_avg, phi_illum_avg, phi_I_tot);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "phi_I_tot: " << phi_I_tot);

    LOG(Trace, "unoccupiedHeatGain: ");
#endif

    unoccupiedHeatGain(phi_int_wk_nt, phi_int_wke_day, phi_int_wke_nt, weekdayUnoccupiedMegaseconds, weekendOccupiedMegaseconds,
                       weekendUnoccupiedMegaseconds, frac_Pgh_wk_nt, frac_Pgh_wke_day, frac_Pgh_wke_nt, v_E_sol, v_P_tot_wke_day, v_P_tot_wk_nt,
                       v_P_tot_wke_nt);
#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_P_tot_wke_day", v_P_tot_wke_day);
    printVector("v_P_tot_wk_nt", v_P_tot_wk_nt);
    printVector("v_P_tot_wke_nt", v_P_tot_wke_nt);

    LOG(Trace, "interiorTemp: ");
#endif

    interiorTemp(v_wall_A, v_P_tot_wke_day, v_P_tot_wk_nt, v_P_tot_wke_nt, v_Tdbt_nt, H_tr, hoursUnoccupiedPerDay, hoursOccupiedPerDay,
                 frac_hrs_wk_day, frac_hrs_wk_nt, frac_hrs_wke_tot, v_Th_avg, v_Tc_avg, tau);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "tau: " << tau);
    printVector("v_Th_avg", v_Th_avg);
    printVector("v_Tc_avg", v_Tc_avg);

    LOG(Trace, "ventilationCalc: ");
#endif

    ventilationCalc(v_Th_avg, v_Tc_avg, frac_hrs_wk_day, v_Hve_ht, v_Hve_cl);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_Hve_ht", v_Hve_ht);
    printVector("v_Hve_cl", v_Hve_cl);

    LOG(Trace, "heatingAndCooling: ");
#endif

    heatingAndCooling(v_E_sol, v_Th_avg, v_Hve_ht, v_Tc_avg, v_Hve_cl, tau, H_tr, phi_I_tot, frac_hrs_wk_day, v_Qfan_tot, v_Qneed_ht, v_Qneed_cl,
                      Qneed_ht_yr, Qneed_cl_yr);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "Qneed_ht_yr: " << Qneed_ht_yr);
    LOG(Trace, "Qneed_cl_yr: " << Qneed_cl_yr);
    printVector("v_Qfan_tot", v_Qfan_tot);

    LOG(Trace, "hvac: ");
#endif

    hvac(v_Qneed_ht, v_Qneed_cl, Qneed_ht_yr, Qneed_cl_yr, v_Qelec_ht, v_Qgas_ht, v_Qcl_elec_tot, v_Qcl_gas_tot);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_Qelec_ht", v_Qelec_ht);
    printVector("v_Qgas_ht", v_Qgas_ht);
    printVector("v_Qcl_elec_tot", v_Qcl_elec_tot);
    printVector("v_Qcl_gas_tot", v_Qcl_gas_tot);

    LOG(Trace, "pump: ");
#endif

    pump(v_Qneed_ht, v_Qneed_cl, Qneed_ht_yr, Qneed_cl_yr, v_Q_pump_tot);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_Q_pump_tot", v_Q_pump_tot);

    LOG(Trace, "energyGeneration: ");
#endif

    energyGeneration();

#ifdef DEBUG_ISO_MODEL_SIMULATION
    LOG(Trace, "heatedWater: ");
#endif

    heatedWater(v_Q_dhw_elec, v_Q_dhw_gas);

#ifdef DEBUG_ISO_MODEL_SIMULATION
    printVector("v_Q_dhw_elec", v_Q_dhw_elec);
    printVector("v_Q_dhw_gas", v_Q_dhw_gas);
#endif

    return outputGeneration(v_Qelec_ht, v_Qcl_elec_tot, v_Q_illum_tot, v_Q_illum_ext_tot, v_Qfan_tot, v_Q_pump_tot, v_Q_dhw_elec, v_Qgas_ht,
                            v_Qcl_gas_tot, v_Q_dhw_gas, frac_hrs_wk_day);
  }

  ISOResults SimModel::outputGeneration(const Vector& v_Qelec_ht, const Vector& v_Qcl_elec_tot, const Vector& v_Q_illum_tot,
                                        const Vector& v_Q_illum_ext_tot, const Vector& v_Qfan_tot, const Vector& v_Q_pump_tot,
                                        const Vector& v_Q_dhw_elec, const Vector& v_Qgas_ht, const Vector& v_Qcl_gas_tot, const Vector& v_Q_dhw_gas,
                                        double frac_hrs_wk_day) const {
    EndUses results[12];
    ISOResults allResults;

    double E_plug_elec =
      building->electricApplianceHeatGainOccupied() * frac_hrs_wk_day + building->electricApplianceHeatGainUnoccupied() * (1.0 - frac_hrs_wk_day);
    double E_plug_gas =
      building->gasApplianceHeatGainOccupied() * frac_hrs_wk_day + building->gasApplianceHeatGainUnoccupied() * (1.0 - frac_hrs_wk_day);

    Vector v_Q_plug_elec = div(mult(hoursInMonth, E_plug_elec, 12), 1000.0);
    Vector v_Q_plug_gas = div(mult(hoursInMonth, E_plug_gas, 12), 1000.0);
    printVector("v_Q_plug_elec", v_Q_plug_elec);
    printVector("v_Q_plug_gas", v_Q_plug_gas);

    /*
  %% Plugload

E_plug_elec =( In.elec_plug_dens_occ*frac_hrs_wk_day + In.elec_plug_dens_unocc*(1-frac_hrs_wk_day)); % average electric plugloads in W/m2
E_plug_gas = (In.gas_plug_dens_occ*frac_hrs_wk_day + In.gas_plug_dens_unocc*(1-frac_hrs_wk_day)); % averaged gas plugloads in W/m2

v_Q_plug_elec = E_plug_elec*v_hrs_ina_mo/1000; % Electric plugload kWh/m2
v_Q_plug_gas = E_plug_gas*v_hrs_ina_mo/1000; % gas plugload kWh/m2

*/
    Vector Eelec_ht = div(div(v_Qelec_ht, structure->floorArea()), kWh2MJ);      //% Total monthly electric usage for heating
    Vector Eelec_cl = div(div(v_Qcl_elec_tot, structure->floorArea()), kWh2MJ);  //% Total monthly electric usage for cooling
    Vector Eelec_int_lt = div(v_Q_illum_tot, structure->floorArea());            //% Total monthly electric usage density for interior lighting
    Vector Eelec_ext_lt = div(v_Q_illum_ext_tot, structure->floorArea());        //% Total monthly electric usage for exterior lights
    Vector Eelec_fan = v_Qfan_tot;                                               //%Total monthly elec usage for fans
    Vector Eelec_pump = div(div(v_Q_pump_tot, structure->floorArea()), kWh2MJ);  //% Total monthly elec usage for pumps
    Vector Eelec_plug = v_Q_plug_elec;                                           //% Total monthly elec usage for elec plugloads
    Vector Eelec_dhw = div(v_Q_dhw_elec, structure->floorArea());
    /*
%% Generating output table

Eelec_ht = v_Qelec_ht./In.cond_flr_area./kWh2MJ; % Total monthly electric usage for heating
Eelec_cl = v_Qcl_elec_tot./In.cond_flr_area./kWh2MJ; % Total monthly electric usage for cooling
Eelec_int_lt = v_Q_illum_tot./In.cond_flr_area; % Total monthly electric usage density for interior lighting
Eelec_ext_lt = v_Q_illum_ext_tot./In.cond_flr_area; % Total monthly electric usage for exterior lights
Eelec_fan = v_Qfan_tot; %Total monthly elec usage for fans
Eelec_pump = v_Q_pump_tot./In.cond_flr_area./kWh2MJ; % Total monthly elec usage for pumps
Eelec_plug = v_Q_plug_elec; % Total monthly elec usage for elec plugloads
Eelec_dhw  = v_Q_dhw_elec/In.cond_flr_area;

*/

    Vector Egas_ht = div(div(v_Qgas_ht, structure->floorArea()), kWh2MJ);      //% total monthly gas usage for heating
    Vector Egas_cl = div(div(v_Qcl_gas_tot, structure->floorArea()), kWh2MJ);  //% total monthly gas usage for cooling
    Vector Egas_plug = v_Q_plug_gas;                                           //% total monthly gas plugloads
    Vector Egas_dhw = div(v_Q_dhw_gas, structure->floorArea());                //% total monthly dhw gas plugloads

    for (int i = 0; i < 12; i++) {
      results[i].addEndUse(Eelec_ht[i], EndUseFuelType::Electricity, EndUseCategoryType::Heating);
      results[i].addEndUse(Eelec_cl[i], EndUseFuelType::Electricity, EndUseCategoryType::Cooling);
      results[i].addEndUse(Eelec_int_lt[i], EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights);
      results[i].addEndUse(Eelec_ext_lt[i], EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights);
      results[i].addEndUse(Eelec_fan[i], EndUseFuelType::Electricity, EndUseCategoryType::Fans);
      results[i].addEndUse(Eelec_pump[i], EndUseFuelType::Electricity, EndUseCategoryType::Pumps);
      results[i].addEndUse(Eelec_plug[i], EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment);
      results[i].addEndUse(Eelec_dhw[i], EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems);

      results[i].addEndUse(Egas_ht[i], EndUseFuelType::Gas, EndUseCategoryType::Heating);
      results[i].addEndUse(Egas_cl[i], EndUseFuelType::Gas, EndUseCategoryType::Cooling);
      results[i].addEndUse(Egas_plug[i], EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment);
      results[i].addEndUse(Egas_dhw[i], EndUseFuelType::Gas, EndUseCategoryType::WaterSystems);
      allResults.monthlyResults.push_back(results[i]);
    }

    return allResults;

    // Note JM 2019-01-03: commented out any code after the existing, above return statement

    /*
%Etotal_elec=Eelec_ht + Eelec_cl+Eelec_int_lt+Eelec_ext_lt+Eelec_fan+Eelec_pump+Eelec_plug;

Egas_ht = v_Qgas_ht./In.cond_flr_area./kWh2MJ; % total monthly gas usage for heating
Egas_cl = v_Qcl_gas_tot./In.cond_flr_area./kWh2MJ; % total monthly gas usage for cooling
Egas_plug = v_Q_plug_gas; % total monthly gas plugloads
Egas_dhw = v_Q_dhw_gas./In.cond_flr_area; % total monthly dhw gas plugloads

Z=zeros(size(Eelec_ht));

*/
    //Vector Etot_ht = sum(Eelec_ht, Egas_ht);
    //Vector Etot_cl = sum(Eelec_cl, Egas_cl);
    //Vector Etot_int_lt = Eelec_int_lt; //% Total monthly electric usage density for interior lighting
    //Vector Etot_ext_lt = Eelec_ext_lt; //% Total monthly electric usage for exterior lights
    //Vector Etot_fan = Eelec_fan; //%Total monthly elec usage for fans
    //Vector Etot_pump = Eelec_pump; //% Total monthly elec usage for pumps
    //Vector Etot_plug = sum(v_Q_plug_elec, v_Q_plug_gas); //% Total monthly elec usage for elec plugloads
    //Vector Etot_dhw  = sum(v_Q_dhw_elec, v_Q_plug_elec);

    /*
Ebldg.elec=[Eelec_ht,Eelec_cl,Eelec_int_lt,Eelec_ext_lt,Eelec_fan,Eelec_pump,Eelec_plug,Eelec_dhw];
Ebldg.gas=[Egas_ht,Egas_cl,Z,Z,Z,Z,Egas_plug,Egas_dhw];
Ebldg.total=Ebldg.elec+Ebldg.gas;
Ebldg.cols={'Heat','Cool','Int Lt','Ext Lt','Fans','Pump','Plug','DHW'};
Ebldg.rows=['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];


*/
    //double yrSum = 0;
    //Vector monthly(Etot_ht.size());
    //for(size_t i = 0;i<Etot_ht.size();i++)
    //{
    //monthly[i] = Etot_ht[i] + Etot_cl[i] + Etot_int_lt[i] + Etot_ext_lt[i] +
    //Etot_fan[i] + Etot_pump[i] + Etot_plug[i] + Etot_dhw[i];
    //yrSum += monthly[i];
    //}

    /*
% Find the totals for each month by summing across the columns
Ebldg.mon=sum(Ebldg.total,2);  % sum normally works down columns but by putting in the ,2 we sum across rows instead
Ebldg.yr=sum(Ebldg.mon);
  */
  }
}  // namespace isomodel
}  // namespace openstudio
