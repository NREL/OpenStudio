#include "ErrorEstimation.hpp"
#include <utilities/units/Quantity.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/UnitFactory.hpp>

namespace openstudio {
  namespace runmanager {
    FuelUses::FuelUses()
      : m_units(*openstudio::createUnit("J"))
    {
    }

    FuelUses FuelUses::operator/(const double scalar) const
    {
      FuelUses ret(*this);
      ret /= scalar;
      return ret;
    }

    FuelUses &FuelUses::operator/=(const double scalar)
    {
      for (std::map<openstudio::FuelType, double>::iterator itr = m_uses.begin();
          itr != m_uses.end();
          ++itr)
      {
        itr->second /= scalar;
      }

      return *this;
    }

    FuelUses FuelUses::operator-(const FuelUses &t_fuelUses) const
    {
      FuelUses ret(*this);
      ret -= t_fuelUses;
      return ret;
    }

    FuelUses &FuelUses::operator-=(const FuelUses &t_fuelUses)
    {
      for (std::map<openstudio::FuelType, double>::const_iterator itr = t_fuelUses.m_uses.begin();
          itr != t_fuelUses.m_uses.end();
          ++itr)
      {
        (*this) -= FuelUse(itr->first, itr->second, t_fuelUses.m_units);
      }

      return *this;
    }

    FuelUses &FuelUses::operator-=(const FuelUse &t_fuelUse)
    {
      std::map<openstudio::FuelType, double>::iterator itr = m_uses.find(t_fuelUse.fuelType);
      if (t_fuelUse.units != m_units)
      {
    //    throw std::runtime_error("Unable to convert fuel use to given units");
      }

      /*
      Quantity q(t_fuelUse.value, t_fuelUse.units);
      boost::optional<openstudio::Quantity> converted = openstudio::convert(q, m_units);
      if (!converted)
      {
        throw std::runtime_error("Unable to convert fuel use to given units");
      }
*/
      if (itr == m_uses.end())
      {
        m_uses[t_fuelUse.fuelType] = t_fuelUse.value;
      } else {
        itr->second -= t_fuelUse.value;
      }

      return *this;
    }


    FuelUses &FuelUses::operator+=(const FuelUses &t_fuelUses)
    {
      for (std::map<openstudio::FuelType, double>::const_iterator itr = t_fuelUses.m_uses.begin();
          itr != t_fuelUses.m_uses.end();
          ++itr)
      {
        (*this) += FuelUse(itr->first, itr->second, t_fuelUses.m_units);
      }

      return *this;
    }

    FuelUses &FuelUses::operator+=(const FuelUse &t_fuelUse)
    {
      std::map<openstudio::FuelType, double>::iterator itr = m_uses.find(t_fuelUse.fuelType);
      if (t_fuelUse.units != m_units)
      {
//        throw std::runtime_error("Unable to convert fuel use to given units");
      }

      /*
      Quantity q(t_fuelUse.value, t_fuelUse.units);
      boost::optional<openstudio::Quantity> converted = openstudio::convert(q, m_units);
      if (!converted)
      {
        throw std::runtime_error("Unable to convert fuel use to given units");
      }
*/
      if (itr == m_uses.end())
      {
        m_uses[t_fuelUse.fuelType] = t_fuelUse.value;
      } else {
        itr->second += t_fuelUse.value;
      }

      return *this;
    }


    double FuelUses::fuelUse(const openstudio::FuelType &t_type) const
    {
      std::map<openstudio::FuelType, double>::const_iterator itr = m_uses.find(t_type);

      if (itr == m_uses.end())
      {
        return 0;
      } else {
        return itr->second;
      }
    }

    std::map<openstudio::FuelType, double> FuelUses::data() const
    {
      return m_uses;
    }

    openstudio::Unit FuelUses::units() const
    {
      return m_units;
    }



    ErrorEstimation::ErrorEstimation(const std::string &t_baselineSourceName, const size_t t_numVariables)
      : m_baselineSourceName(t_baselineSourceName), m_numVariables(t_numVariables)
    {

    }


    /// adds the data of an SqlFile simulation result into the ErrorEstimation and returns the error corrected values
    /// 
    /// \param[in] t_sql The SqlFile to pull the year end summary data from
    /// \param[in] t_sourceName The name of the simulation source this data is from, registered with addSource
    /// \param[in] t_variable The name of the variable that this data is for
    /// \param[in] t_value The value of t_variable for this datapoint
    /// \param[in] t_singleVariableChange Did only one variable change for this data?
    ///
    /// \returns The error corrected values, or the input values if error correction was not possible
    FuelUses ErrorEstimation::add(const SqlFile &t_sql, const std::string &t_sourceName, const std::vector<double> &t_variables)
    {
      FuelUses origUses = getUses(t_sql);

      std::map<openstudio::FuelType, double> data = origUses.data();

      for (std::map<openstudio::FuelType, double>::const_iterator itr = data.begin();
           itr != data.end();
           ++itr)
      {
        m_data.insert(std::make_pair(std::make_pair(t_sourceName, itr->first), LinearApproximation(m_numVariables)))
          .first->second.addVals(t_variables, itr->second);
      }
      
      FuelUses error = getError(t_sourceName, t_variables);
      origUses += error;

      return origUses;
    }

    FuelUses ErrorEstimation::getError(const std::string &t_sourceName, const std::vector<double> &t_variables) const
    {
      // The baseline is the good value, no error
      if (t_sourceName == m_baselineSourceName) return FuelUses();

      std::set<int> allFuelUses = openstudio::FuelType::getValues();

      FuelUses retval;

      for (std::set<int>::const_iterator itr = allFuelUses.begin();
           itr != allFuelUses.end();
           ++itr)
      {
        std::map<std::pair<std::string, openstudio::FuelType>, LinearApproximation>::const_iterator baseline = 
          m_data.find(std::make_pair(m_baselineSourceName, openstudio::FuelType(*itr)));

        std::map<std::pair<std::string, openstudio::FuelType>, LinearApproximation>::const_iterator thisOne = 
          m_data.find(std::make_pair(t_sourceName, openstudio::FuelType(*itr)));

        double error = 0;

        if (baseline != m_data.end() && thisOne != m_data.end())
        {
          LinearApproximation diff = baseline->second - thisOne->second;


          try {
            error = diff.approximate(t_variables);
          } catch (const std::exception &) {
            // not enough data to run approximation
            error = diff.average();
          }
        }

        std::cout << "Adding error for: " << openstudio::FuelType(*itr).valueName() << " of " << error << std::endl;
        retval += FuelUse(openstudio::FuelType(*itr), error, *openstudio::createUnit("J"));
      }

      return retval;
    }

    /// Returns an estimated FuelUsage for the given variable at the given value
    /// 
    FuelUses ErrorEstimation::approximate(const std::vector<double> &t_values) const
    {
      std::set<int> allFuelUses = openstudio::FuelType::getValues();

      FuelUses retval;

      for (std::set<int>::const_iterator itr = allFuelUses.begin();
           itr != allFuelUses.end();
           ++itr)
      {
        std::map<std::pair<std::string, openstudio::FuelType>, LinearApproximation>::const_iterator baseline = 
          m_data.find(std::make_pair(m_baselineSourceName, openstudio::FuelType(*itr)));


        if (baseline != m_data.end())
        {
          double value = baseline->second.approximate(t_values);
          retval += FuelUse(openstudio::FuelType(*itr), value, *openstudio::createUnit("J"));
        }

      }

      return retval;
     }

    FuelUses ErrorEstimation::getUses(const SqlFile &t_sql)
    {
      std::vector<SummaryData> sd = t_sql.getSummaryData();

      FuelUses retval;

      for (std::vector<SummaryData>::const_iterator itr = sd.begin();
          itr != sd.end();
          ++itr)
      {
        if (itr->reportingFrequency == openstudio::ReportingFrequency::Hourly)
        {
          FuelUse fuse(itr->fuelType, itr->value, itr->units);
          retval += fuse;
        }
      }

      return retval;
    }
  }
}

