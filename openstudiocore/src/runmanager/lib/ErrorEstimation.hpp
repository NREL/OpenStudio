#ifndef OPENSTUDIO_RUNMANAGER_ERRORESTIMATION_HPP
#define OPENSTUDIO_RUNMANAGER_ERRORESTIMATION_HPP

#include "RunManagerAPI.hpp"

#include <utilities/units/Unit.hpp>
#include <utilities/data/DataEnums.hpp>
#include <utilities/sql/SqlFileEnums.hpp>
#include <utilities/sql/SqlFile.hpp>

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
      FuelUses();

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

      private:
        std::map<openstudio::FuelType, double> m_uses;
        openstudio::Unit m_units;
         
    };

    class RUNMANAGER_API ErrorEstimation
    {
      public:
        /// Constructs an ErrorEstimation object.
        ///
        /// \param[in] t_baselineSourceName The name of the data sources which is considered to be trustworthy, with no error
        ErrorEstimation(const std::string &t_baselineSourceName, const size_t t_numVariables);


        /// adds the data of an SqlFile simulation result into the ErrorEstimation and returns the error corrected values
        /// 
        /// \param[in] t_sql The SqlFile to pull the year end summary data from
        /// \param[in] t_sourceName The name of the simulation source this data is from, registered with addSource
        /// \param[in] t_variable The name of the variable that this data is for
        /// \param[in] t_value The value of t_variable for this datapoint
        /// \param[in] t_singleVariableChange Did only one variable change for this data?
        ///
        /// \returns The error corrected values, or the input values if error correction was possible
        FuelUses add(const SqlFile &t_sql, const std::string &t_sourceName, const std::vector<double> &t_variables);

        /// Returns an estimated FuelUsage for the given variable at the given value
        /// 
        /// \param[in] t_variable The variable to 
        FuelUses approximate(const std::vector<double> &t_variables) const;

        /// Returns the error, by fuel type for the given conditions
        FuelUses getError(const std::string &t_sourceName, const std::vector<double> &t_variables) const;

      private:
        std::string m_baselineSourceName;
        std::map<std::pair<std::string, openstudio::FuelType>, LinearApproximation> m_data;
        size_t m_numVariables;

        FuelUses getUses(const SqlFile &t_sql);
    };

  }
}

#endif

