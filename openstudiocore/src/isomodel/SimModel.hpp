/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef ISOMODEL_SIMMODEL_HPP
#define ISOMODEL_SIMMODEL_HPP

#include "ISOModelAPI.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/data/Vector.hpp"
#include "../utilities/data/Matrix.hpp"
#include "../utilities/data/EndUses.hpp"
#include "Population.hpp"
#include "Location.hpp"
#include "Lighting.hpp"
#include "Building.hpp"
#include "Cooling.hpp"
#include "Heating.hpp"
#include "Structure.hpp"
#include "Ventilation.hpp"


namespace openstudio {

  class EndUses;

namespace isomodel { 

  ISOMODEL_API Vector mult(const double* v1, const double s1, int size);
  ISOMODEL_API Vector mult(const Vector& v1, const double s1);
  ISOMODEL_API Vector mult(const Vector& v1, const double* v2);
  ISOMODEL_API Vector mult(const Vector& v1, const Vector& v2);
  ISOMODEL_API Vector div(const Vector& v1,const double s1);
  ISOMODEL_API Vector div(const double s1, const Vector& v1);
  ISOMODEL_API Vector div(const Vector& v1, const Vector& v2);
  ISOMODEL_API Vector sum(const Vector& v1, const Vector& v2);
  ISOMODEL_API Vector sum(const Vector& v1, const double v2);
  ISOMODEL_API Vector dif(const Vector& v1, const Vector& v2);
  ISOMODEL_API Vector dif(const Vector& v1, const double v2);
  ISOMODEL_API Vector dif(const double v1, const Vector& v2);

  ISOMODEL_API Vector maximum(const Vector& v1, const Vector& v2);
  ISOMODEL_API Vector maximum(const Vector& v1, double val);
  ISOMODEL_API double maximum(const Vector& v1);

  ISOMODEL_API Vector minimum(const Vector& v1, double val);
  ISOMODEL_API double minimum(const Vector& v1);

  ISOMODEL_API Vector abs(const Vector& v1);
  ISOMODEL_API Vector pow(const Vector& v1, const double xp);

  struct ISOMODEL_API ISOResults{    
    std::vector<EndUses> monthlyResults;
    double totalEnergyUse() const;
  };

  class ISOMODEL_API SimModel {
  public:
    void setPop(std::shared_ptr<Population> value){pop=value;}
    void setLocation(std::shared_ptr<Location> value){location=value;}
    void setLights(std::shared_ptr<Lighting> value){lights=value;}
    void setBuilding(std::shared_ptr<Building> value){building=value;}
    void setStructure(std::shared_ptr<Structure> value){structure=value;}
    void setHeating(std::shared_ptr<Heating> value){heating=value;}
    void setCooling(std::shared_ptr<Cooling> value){cooling=value;}
    void setVentilation(std::shared_ptr<Ventilation> value){ventilation=value;}

    /*
     *  Runs the ISO Model cacluations for the given set of input parameters.
     *  returns ISOResults which is a vector of EndUses, one EndUses per month of the year
     */
    ISOResults simulate() const;
    REGISTER_LOGGER("openstudio.isomodel.SimModel");

  private:      
    std::shared_ptr<Population> pop;
    std::shared_ptr<Location> location;
    std::shared_ptr<Lighting> lights;
    std::shared_ptr<Building> building;
    std::shared_ptr<Structure> structure;
    std::shared_ptr<Heating> heating;
    std::shared_ptr<Cooling> cooling;
    std::shared_ptr<Ventilation> ventilation;  

    void scheduleAndOccupancy(Vector& weekdayOccupiedMegaseconds, 
            Vector& weekdayUnoccupiedMegaseconds,
            Vector& weekendOccupiedMegaseconds,
            Vector& weekendUnoccupiedMegaseconds,
            Vector& clockHourOccupied,
            Vector& clockHourUnoccupied,
            double& frac_hrs_wk_day,
            double& hoursUnoccupiedPerDay, 
            double& hoursOccupiedPerDay,
            double& frac_hrs_wk_nt, 
            double& frac_hrs_wke_tot) const;
    void solarRadiationBreakdown(const Vector& weekdayOccupiedMegaseconds, 
            const Vector& weekdayUnoccupiedMegaseconds,
            const Vector& weekendOccupiedMegaseconds,
            const Vector& weekendUnoccupiedMegaseconds,
            const Vector& clockHourOccupied,
            const Vector& clockHourUnoccupied,
            Vector& v_hrs_sun_down_mo,
            Vector& frac_Pgh_wk_nt,
            Vector& frac_Pgh_wke_day,
            Vector& frac_Pgh_wke_nt,
            Vector& v_Tdbt_nt) const;
    void lightingEnergyUse(const Vector& v_hrs_sun_down_mo, 
            double& Q_illum_occ, 
            double& Q_illum_unocc,
            double& Q_illum_tot_yr,
            Vector& v_Q_illum_tot,
            Vector& v_Q_illum_ext_tot) const;
    void envelopCalculations(Vector& v_win_A,
            Vector& v_wall_emiss,
            Vector& v_wall_alpha_sc,
            Vector& v_wall_U,
            Vector& v_wall_A,
            double& H_tr) const;
    void windowSolarGain(const Vector& v_win_A, 
            const Vector& v_wall_emiss, 
            const Vector& v_wall_alpha_sc,
            const Vector& v_wall_U,
            const Vector& v_wall_A,
            Vector& v_wall_A_sol,
            Vector& v_win_hr,
            Vector& v_wall_R_sc,
            Vector& v_win_A_sol) const;
    void solarHeatGain(const Vector& v_win_A_sol,
            const Vector& v_wall_R_sc,
            const Vector& v_wall_U,
            const Vector& v_wall_A,
            const Vector& v_win_hr,
            const Vector& v_wall_A_sol,
            Vector& v_E_sol) const;
    void heatGainsAndLosses(double frac_hrs_wk_day, 
            double Q_illum_occ, 
            double Q_illum_unocc,
            double Q_illum_tot_yr,
            double& phi_int_avg,
            double& phi_plug_avg, 
            double& phi_illum_avg,
            double& phi_int_wke_nt,
            double& phi_int_wke_day,
            double& phi_int_wk_nt) const;
    void internalHeatGain(double phi_int_avg, 
            double phi_plug_avg, 
            double phi_illum_avg, 
            double& phi_I_tot) const;
    void unoccupiedHeatGain(double phi_int_wk_nt, 
            double phi_int_wke_day, 
            double phi_int_wke_nt,
            const Vector& weekdayUnoccupiedMegaseconds,
            const Vector& weekendOccupiedMegaseconds,
            const Vector& weekendUnoccupiedMegaseconds,
            const Vector& frac_Pgh_wk_nt,
            const Vector& frac_Pgh_wke_day,
            const Vector& frac_Pgh_wke_nt,
            const Vector& v_E_sol,
            Vector& v_P_tot_wke_day,
            Vector& v_P_tot_wk_nt,
            Vector& v_P_tot_wke_nt) const;
    void interiorTemp(const Vector& v_wall_A, 
            const Vector& v_P_tot_wke_day,
            const Vector& v_P_tot_wk_nt,
            const Vector& v_P_tot_wke_nt,
            const Vector& v_Tdbt_nt,
            double H_tr,
            double hoursUnoccupiedPerDay,
            double hoursOccupiedPerDay,
            double frac_hrs_wk_day,
            double frac_hrs_wk_nt,
            double frac_hrs_wke_tot,
            Vector& v_Th_avg,
            Vector& v_Tc_avg,
            double& tau) const;
    void ventilationCalc(const Vector& v_Th_avg,
            const Vector& v_Tc_avg,
            double frac_hrs_wk_day,
            Vector& v_Hve_ht,
            Vector& v_Hve_cl) const;
    void heatingAndCooling(const Vector& v_E_sol,
            const Vector& v_Th_avg,
            const Vector& v_Hve_ht,
            const Vector& v_Tc_avg,
            const Vector& v_Hve_cl,
            double tau,
            double H_tr,
            double phi_I_tot,
            double frac_hrs_wk_day,
            Vector& v_Qfan_tot,
            Vector& v_Qneed_ht,
            Vector& v_Qneed_cl,
            double& Qneed_ht_yr,
            double& Qneed_cl_yr) const;
    void hvac(const Vector& v_Qneed_ht,
            const Vector& v_Qneed_cl,
            double Qneed_ht_yr, 
            double Qneed_cl_yr,
            Vector& v_Qelec_ht,
            Vector& v_Qgas_ht,
            Vector& v_Qcl_elec_tot,
            Vector& v_Qcl_gas_tot) const;
    void pump(const Vector& v_Qneed_ht,
            const Vector& v_Qneed_cl,
            double Qneed_ht_yr, 
            double Qneed_cl_yr,
            Vector& v_Q_pump_tot) const;
    void energyGeneration() const;
    void heatedWater(Vector& v_Q_dhw_elec, Vector& v_Q_dhw_gas) const;

    ISOResults outputGeneration(const Vector& v_Qelec_ht,
            const Vector& v_Qcl_elec_tot,
            const Vector& v_Q_illum_tot,
            const Vector& v_Q_illum_ext_tot,
            const Vector& v_Qfan_tot,
            const Vector& v_Q_pump_tot,
            const Vector& v_Q_dhw_elec,
            const Vector& v_Qgas_ht,
            const Vector& v_Qcl_gas_tot,
            const Vector& v_Q_dhw_gas,
            double frac_hrs_wk_day) const;

    static void printVector(const char* vecName, const Vector &vec);
    static void printMatrix(const char* matName, const Matrix &mat);
  };
} // isomodel
} // openstudio

#endif // ISOMODEL_SIMMODEL_HPP
