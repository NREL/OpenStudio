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

#include "ErrorEstimation.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/QuantityConverter.hpp"
#include "../../utilities/units/UnitFactory.hpp"

namespace openstudio {
  namespace runmanager {
    FuelUses::FuelUses(double t_confidence)
      : m_units(*openstudio::createUnit("J")),
        m_confidence(t_confidence)
    {
    }

    void FuelUses::setConfidence(double t_newConfidence)
    {
      m_confidence = t_newConfidence;
    }

    double FuelUses::confidence() const
    {
      return m_confidence;
    }

    double FuelUses::average() const
    {
      double sum = 0;
      for (const auto & use : m_uses)
      {
        sum += use.second;
      }

      return sum / m_uses.size();
    }

    double FuelUses::sum() const
    {
      double s = 0;
      for (const auto & use : m_uses)
      {
        s += use.second;
      }

      return s;
    }


    double FuelUses::absoluteAverage() const
    {
      double sum = 0;
      for (const auto & use : m_uses)
      {
        sum += fabs(use.second);
      }

      return sum / m_uses.size();
    }

    FuelUses FuelUses::operator/(const double scalar) const
    {
      FuelUses ret(*this);
      ret /= scalar;
      return ret;
    }

    FuelUses &FuelUses::operator/=(const double scalar)
    {
      for (auto & use : m_uses)
      {
        use.second /= scalar;
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
      for (const auto & use : t_fuelUses.m_uses)
      {
        (*this) -= FuelUse(use.first, use.second, t_fuelUses.m_units);
      }

      return *this;
    }

    FuelUses &FuelUses::operator-=(const FuelUse &t_fuelUse)
    {
      auto itr = m_uses.find(t_fuelUse.fuelType);
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
      for (const auto & use : t_fuelUses.m_uses)
      {
        (*this) += FuelUse(use.first, use.second, t_fuelUses.m_units);
      }

      return *this;
    }

    FuelUses &FuelUses::operator+=(const FuelUse &t_fuelUse)
    {
      auto itr = m_uses.find(t_fuelUse.fuelType);
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
      auto itr = m_uses.find(t_type);

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



    ErrorEstimation::ErrorEstimation(const size_t t_numVariables)
      : m_numVariables(t_numVariables)
    {

    }

    void ErrorEstimation::setConfidence(const std::string &t_sourceName, double t_confidence)
    {
      if (t_confidence < 0 || t_confidence > 1.0)
      {
        throw std::range_error("confidence level must be between 0 and 1");
      }

      m_confidences[t_sourceName] = t_confidence;
    }


    FuelUses ErrorEstimation::add(const isomodel::UserModel &t_userModel, const isomodel::ISOResults &t_isoResults, const std::string &t_sourceName, const std::vector<double> &t_variables)
    {
      return add(getUses(t_sourceName, t_userModel, t_isoResults), t_sourceName, t_variables);
    }

    FuelUses ErrorEstimation::add(const SqlFile &t_sql, const std::string &t_sourceName, const std::vector<double> &t_variables)
    {
      return add(getUses(t_sourceName, t_sql), t_sourceName, t_variables);
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
    FuelUses ErrorEstimation::add(FuelUses t_origUses, const std::string &t_sourceName, const std::vector<double> &t_variables)
    {
      std::map<openstudio::FuelType, double> data = t_origUses.data();

      for (std::map<openstudio::FuelType, double>::const_iterator itr = data.begin();
           itr != data.end();
           ++itr)
      {
        m_data.insert(std::make_pair(std::make_pair(t_sourceName, itr->first), LinearApproximation(m_numVariables)))
          .first->second.addVals(t_variables, itr->second);
      }
      

      FuelUses error = getError(t_sourceName, t_variables);

      double avgerror = error.absoluteAverage();
      double avgorig = t_origUses.average();

      double newConfidence = avgorig>0?std::min(1.0, std::max(1.0  - (avgerror / avgorig), 0.0)):t_origUses.confidence();


      // new confidence reflect how much error is being applied. 1 means no error, 0 means 100% error (that's bad);
      // now we need to scale that to take into account the confidence level of the error we are applying.
      newConfidence *= error.confidence();

      // we're going to apply this confidence back into the list of confidences
      if (newConfidence != 0)
      {
        // don't let it sink to 0, that really hurts our averages
        LOG(Debug, "Updating source confidence " << t_sourceName << " from: " << m_confidences[t_sourceName] << " to " << newConfidence);
        LOG(Trace, "Updating source confidence " << t_sourceName << " avgorig " << avgorig << " avgerror " << avgerror << " origUses.confidence " << t_origUses.confidence());
        m_confidences[t_sourceName] = newConfidence;
      }

      t_origUses += error;

      // Now that the error has been applied, we're going to average the confidence we've calculated with the 
      // confidence from the error
      t_origUses.setConfidence((newConfidence + error.confidence()) / 2);

      return t_origUses;
    }

    std::set<std::pair<double, std::string> > ErrorEstimation::getConfidences() const 
    {
      std::set<std::pair<double, std::string> > retval;

      for (const auto & confidence : m_confidences)
      {
        retval.insert(std::make_pair(confidence.second, confidence.first));
      }

      return retval;
    }


    FuelUses ErrorEstimation::getError(const std::string &t_sourceName, const std::vector<double> &t_variables) const
    {
      if (m_confidences.empty())
      {
        throw std::runtime_error("no registered confidence levels");
      }

      std::set<std::pair<double, std::string> > confidences = getConfidences();

      auto mostConfident = confidences.rbegin();
      
      
      // The baseline is the good value, no error
      if (mostConfident != confidences.rend() && t_sourceName == mostConfident->second) return FuelUses(mostConfident->first);

      std::set<int> allFuelUses = openstudio::FuelType::getValues();


      FuelUses retval(0);

      double sumConfidence = 0;
      int numFound = 0;

      for (const auto & fuelUse : allFuelUses)
      {

        auto thisOne = 
          m_data.find(std::make_pair(t_sourceName, openstudio::FuelType(fuelUse)));

        double error = 0;
        double confidence = 0;
        bool somethingFound = false;

        for (auto confitr = confidences.rbegin();
             confitr != confidences.rend();
             ++confitr)
        {
          auto baseline = 
            m_data.find(std::make_pair(confitr->second, openstudio::FuelType(fuelUse)));

          confidence = confitr->first;

          if (baseline != m_data.end() && thisOne != m_data.end())
          {
            somethingFound = true;

            try {
              double baselineval = baseline->second.approximate(t_variables);
              double thisoneval = thisOne->second.approximate(t_variables); 
              error = baselineval - thisoneval;
              break; // we found a good one
            } catch (const std::exception &) {
              // not enough data to run approximation
              error = baseline->second.average() - thisOne->second.average();
            }
          }
        }

        if (somethingFound)
        {
          LOG(Trace, "Adding error for: " << openstudio::FuelType(fuelUse).valueName() << " of " << error);
          retval += FuelUse(openstudio::FuelType(), error, *openstudio::createUnit("J"));
          ++numFound;
          sumConfidence += confidence;
        }
      }

      if (numFound > 0)
      {
        retval.setConfidence(sumConfidence / numFound); // reset the confidence level to that of average of the inputs
      } else {
        retval.setConfidence(0);
      }

      return retval;
    }

    /// Returns an estimated FuelUsage for the given variable at the given value
    /// 
    FuelUses ErrorEstimation::approximate(const std::vector<double> &t_values) const
    {
      std::set<int> allFuelUses = openstudio::FuelType::getValues();

      FuelUses retval(0);
      std::set<std::pair<double, std::string> > confidences = getConfidences();

      double confidenceSum = 0;
      int numApproximations = 0;


      for (const auto & fuelUse : allFuelUses)
      {
        std::map<std::pair<double, double>, FuelUse> approximations;

        for (auto confitr = confidences.rbegin();
            confitr != confidences.rend();
            ++confitr)
        {
          auto baseline = 
            m_data.find(std::make_pair(confitr->second, openstudio::FuelType(fuelUse)));


          if (baseline != m_data.end())
          {
            try {
              double value = baseline->second.approximate(t_values);
              FuelUse use(openstudio::FuelType(fuelUse), value, *openstudio::createUnit("J"));
              // If this is a known value, use the confidence straight up. If it's an approximation
              // scale it by the distances.
              try {
                std::pair<double, double> distance = baseline->second.nearestFurthestNeighborDistances(t_values);
                LOG(Trace, "confidence Confitr->first " << confitr->first << " distance.first " << distance.first << " distance.second " << distance.second);
                if (distance.first == 0)
                {
                  // we have an exact match, no scaling necessary
                  approximations.insert(std::make_pair(std::make_pair(confitr->first, confitr->first), use));
                } else {
                  // it's illogical, there has to be a point that's greater than 0 from where we are
                  assert(distance.second != 0);

                  // scale the confidence based on distances to neighbors
                  approximations.insert(std::make_pair(std::make_pair(confitr->first*(1-(distance.first/(distance.second + distance.first))), 
                          confitr->first), use));
                }
              } catch (const std::exception &) {
                // we don't have enough info to know, so let's say 50% of confidence for this estimation
                LOG(Trace, "confidence Confitr->first " << confitr->first);
                approximations.insert(std::make_pair(std::make_pair(confitr->first * 0.5, confitr->first), use));
              }
            } catch (const std::exception &) {
              // approximation failed, on to next
            }
          }

        }

        LOG(Trace, "confidence " << approximations.size() << " approximations generated for " << openstudio::FuelType(fuelUse).valueName());

        if (!approximations.empty())
        {
          std::map<std::pair<double, double>, FuelUse>::const_reverse_iterator itr = approximations.rbegin();

          LOG(Trace, "confidence choosing item with confidence of " << itr->first.first << " which was generated from item with confidence of " << itr->first.second);
          retval += itr->second;
          ++numApproximations;
          confidenceSum += itr->first.first;
        }
      }

      // and set the confidence finally to the average of the confidences that made this up
      if (numApproximations == 0)
      {
        retval.setConfidence(0);
      } else {
        retval.setConfidence(confidenceSum / numApproximations);
      }

      return retval;
    }

    double ErrorEstimation::getConfidence(const std::string &t_sourceName) const
    {
      auto itr = m_confidences.find(t_sourceName);

      if (itr == m_confidences.end())
      {
        throw std::runtime_error("Unknown source name, no registered confidence level");
      }

      return itr->second;
    }

    FuelUses ErrorEstimation::getUses(const std::string &t_sourceName, const isomodel::UserModel &t_userModel, const isomodel::ISOResults &t_results) const
    {
      auto itr = m_confidences.find(t_sourceName);

      if (itr == m_confidences.end())
      {
        throw std::runtime_error("Unknown source name, no registered confidence level");
      }

      FuelUses retval(itr->second);


      std::vector<EndUseFuelType> fuelTypes = EndUses::fuelTypes();


      for (const auto & fuelType : fuelTypes)
      {
        double value = 0;
        for (const auto & monthlyResult : t_results.monthlyResults)
        {
          value += monthlyResult.getEndUseByFuelType(fuelType);
        }
        LOG(Debug, "Read fuel use of " << value << " For " << fuelType.valueName());

        try {
          // the fuel uses in this case seem to be stored in kWh/m2
          FuelUse fuse(openstudio::FuelType(fuelType.valueName()), value * 3600000 * t_userModel.floorArea(), *openstudio::createUnit("J"));
          retval += fuse;
        } catch (const std::exception &e) {
          LOG(Error, "Unmatched fuel type " << e.what());
        }
      }

      return retval;
    }

    FuelUses ErrorEstimation::getUses(const std::string &t_sourceName, const SqlFile &t_sql) const
    {
      auto itr = m_confidences.find(t_sourceName);

      if (itr == m_confidences.end())
      {
        throw std::runtime_error("Unknown source name, no registered confidence level");
      }

      std::vector<SummaryData> sd = t_sql.getSummaryData();

      FuelUses retval(itr->second);

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

