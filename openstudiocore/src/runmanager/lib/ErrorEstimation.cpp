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

    openstudio::Unit FuelUses::units() const
    {
      return m_units;
    }



    ErrorEstimation::ErrorEstimation(const std::string &t_baselineSourceName)
      : m_baselineSourceName(t_baselineSourceName)
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
    /// \returns The error corrected values, or the input values if error correction was possible
    FuelUses ErrorEstimation::add(const SqlFile &t_sql, const std::string &t_sourceName, const std::string &t_variable, 
        const double t_value, const bool t_singleVariableChange)
    {
      FuelUses origUses = getUses(t_sql);

      m_data[t_variable][t_value][t_sourceName] = origUses;

      FuelUses error = getError(t_sourceName, t_variable, t_value);

      origUses += error;
      return origUses;
    }

    FuelUses ErrorEstimation::getError(const std::string &t_sourceName, const std::string &/*t_variable*/, const double /*t_value*/) const
    {
      // The baseline is the good value, no error
      if (t_sourceName == m_baselineSourceName) return FuelUses();

      FuelUses error;
      int numfound = 0;

      // For now, let's just average the error across all variables
      for (std::map<std::string, std::map<double, std::map<std::string, FuelUses> > >::const_iterator itr = m_data.begin();
           itr != m_data.end();
           ++itr)
      {
        for (std::map<double, std::map<std::string, FuelUses> >::const_iterator itr2 = itr->second.begin();
             itr2 != itr->second.end();
             ++itr2)
        {
          std::map<std::string, FuelUses>::const_iterator baseline = itr2->second.find(m_baselineSourceName);
          std::map<std::string, FuelUses>::const_iterator thisone = itr2->second.find(t_sourceName);

          if (baseline != itr2->second.end() 
              && thisone != itr2->second.end())
          {
            error += baseline->second - thisone->second;
            ++numfound;
          }

        }
      }

      return error / numfound;
    }

    /// Returns an estimated FuelUsage for the given variable at the given value
    /// 
    /// \param[in] t_variable The variable to 
    FuelUses ErrorEstimation::estimate(const std::string &t_variable, const double t_value) const
    {
      assert("Not yet Implemented");
      throw std::runtime_error("ErrorEstimation::estimate is not yet implemented");
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
