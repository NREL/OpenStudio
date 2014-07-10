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

#ifndef RUNMANAGER_LIB_ERRORESTIMATION_HPP
#define RUNMANAGER_LIB_ERRORESTIMATION_HPP

#include "RunManagerAPI.hpp"

#include "../../utilities/units/Unit.hpp"
#include "../../utilities/data/DataEnums.hpp"
#include "../../utilities/sql/SqlFileEnums.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include "../../isomodel/SimModel.hpp"
#include "../../isomodel/UserModel.hpp"

#include "LinearApproximation.hpp"

/// These classes are currently stubbed in here as we are move towards a full error estimation model
namespace openstudio {
  namespace runmanager {

    struct RUNMANAGER_API FuelUse
    {
      FuelUse(const openstudio::FuelType t_fuelType,
          const double t_value,
          const openstudio::Unit t_units)
        : fuelType(t_fuelType), value(t_value), units(t_units)
      {
      }

      openstudio::FuelType fuelType;
      double value;
      openstudio::Unit units;
    };

    struct RUNMANAGER_API FuelUses
    {
      FuelUses(double t_confidence);

      FuelUses operator/(const double t_scalar) const;
      FuelUses &operator/=(const double t_scalar);

      FuelUses operator-(const FuelUses &t_fuelUses) const;

      FuelUses &operator-=(const FuelUses &t_fuelUses);
      FuelUses &operator-=(const FuelUse &t_fuelUse);

      FuelUses &operator+=(const FuelUses &t_fuelUses);
      FuelUses &operator+=(const FuelUse &t_fuelUse);

      double fuelUse(const openstudio::FuelType &t_type) const;
      openstudio::Unit units() const;
      std::map<openstudio::FuelType, double> data() const;

      void setConfidence(double t_newConfidence);
      double confidence() const;
      double average() const;
      double absoluteAverage() const;
      double sum() const;

      private:
        std::map<openstudio::FuelType, double> m_uses;
        openstudio::Unit m_units;

        double m_confidence;

    };

    class RUNMANAGER_API ErrorEstimation
    {
      public:
        /// Constructs an ErrorEstimation object.
        ///
        ErrorEstimation(const size_t t_numVariables);

        /// Sets the confidence level for a given data source
        void setConfidence(const std::string &t_sourceName, double t_confidence);

        /// \returns the confidence for the given source name
        double getConfidence(const std::string &t_sourceName) const;

        /// adds the data of an SqlFile simulation result into the ErrorEstimation and returns the error corrected values
        /// 
        /// \param[in] t_sql The SqlFile to pull the year end summary data from
        /// \param[in] t_sourceName The name of the simulation source this data is from, registered with addSource
        /// \param[in] t_variables The name of the variable that this data is for
        ///
        /// \returns The error corrected values, or the input values if error correction was not possible
        openstudio::runmanager::FuelUses add(const SqlFile &t_sql, const std::string &t_sourceName, const std::vector<double> &t_variables);

        /// Adds the data of an ISOResults from an ISO simulation into the ErrorEstimation and returns the error corrected values;
        /// 
        /// \param[in] t_results The SqlFile to pull the year end summary data from
        /// \param[in] t_sourceName The name of the simulation source this data is from, registered with addSource
        /// \param[in] t_variables The name of the variable that this data is for
        ///
        /// \returns The error corrected values, or the input values if error correction was not possible
        openstudio::runmanager::FuelUses add(const isomodel::UserModel &t_userModel, const isomodel::ISOResults &t_results, const std::string &t_sourceName, const std::vector<double> &t_variables);

        openstudio::runmanager::FuelUses add(FuelUses t_origUses, const std::string &t_sourceName, const std::vector<double> &t_variables);

        /// Returns an estimated FuelUsage for the given variable at the given value
        /// 
        /// \param[in] t_variable The variable to 
        openstudio::runmanager::FuelUses approximate(const std::vector<double> &t_variables) const;


      private:
        REGISTER_LOGGER("openstudio.runmanager.ErrorEstimation");

        std::string m_baselineSourceName;
        std::map<std::pair<std::string, openstudio::FuelType>, LinearApproximation> m_data;
        size_t m_numVariables;

        std::set<std::pair<double, std::string> > getConfidences() const;

        std::map<std::string, double> m_confidences;

        /// Returns the error, by fuel type for the given conditions
        FuelUses getError(const std::string &t_sourceName, const std::vector<double> &t_variables) const;

        /// Returns the FuelUses from the given SqlFile, with the confidence level of the source name applied
        FuelUses getUses(const std::string &t_sourceName, const SqlFile &t_sql) const;

        /// Returns the FuelUses from the given ISOResults, with the confidence level of the source name applied
        FuelUses getUses(const std::string &t_sourceName, const isomodel::UserModel &t_userModel, const isomodel::ISOResults &t_results) const;
    };

  }
}

#endif // RUNMANAGER_LIB_ERRORESTIMATION_HPP

