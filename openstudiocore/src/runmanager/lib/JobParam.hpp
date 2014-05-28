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

#ifndef RUNMANAGER_LIB_JOBPARAM_HPP
#define RUNMANAGER_LIB_JOBPARAM_HPP

#include "RunManagerAPI.hpp"

#include <vector>
#include <string>
#include <algorithm>

namespace openstudio {
namespace runmanager {

  /// Hierarchical list of named parameters to be used when passing advanced options to a Job
  /// The WorkflowJob currently makes use of this.
  struct RUNMANAGER_API JobParam
  {
    private:
      /// Functor for comparing the values between two different JobParam objects
      struct JobParamValueCompare
      {
        JobParamValueCompare(const std::string &t_param)
          : m_param(t_param)
        { }

        bool operator()(const JobParam &t_param)
        {
          return t_param.value == m_param;
        }

        private:
        std::string m_param;
      };

    public:

      /// \param[in] t_params the list of params to search
      /// \param[in] t_value the value to search for
      /// \returns the JobParam matching the given value. Throws an exception if it is not found
      static JobParam paramByValue(const std::vector<JobParam> &t_params, const std::string &t_value)
      {
        auto itr = std::find_if(t_params.rbegin(), t_params.rend(), JobParamValueCompare(t_value));

        if (itr != t_params.rend())
        {
          return *itr;
        }

        throw std::runtime_error("Unknown value: " + t_value);
      }

      /// \param[in] t_params the list of params to search
      /// \param[in] t_value the value to search for
      /// \returns the JobParam matching the given value. Throws an exception if it is not found
      static bool hasByValue(const std::vector<JobParam> &t_params, const std::string &t_value)
      {
        auto itr = std::find_if(t_params.rbegin(), t_params.rend(), JobParamValueCompare(t_value));

        if (itr != t_params.rend())
        {
          return true;
        }

        return false;
      }



      /// \param[in] t_params the list of params to search
      /// \param[in] t_value the value to remove
      /// Erases the JobParam with the given value if it exists
      static void removeByValue(std::vector<JobParam> &t_params, const std::string &t_value)
      {
        bool cont = true;
        while (cont)
        {
          auto itr = std::find_if(t_params.begin(), t_params.end(), JobParamValueCompare(t_value));

          if (itr != t_params.end())
          {
            t_params.erase(itr);
          } else {
            cont = false;
          }
        }
      }

      /// \param[in] t_value The value to assign the newly constructed JobParam
      JobParam(const std::string &t_value)
        : value(t_value)
      {
      }

      bool operator==(const JobParam &t_rhs) const
      {
        return value == t_rhs.value
          && std::equal(children.begin(), children.end(), t_rhs.children.begin());
      }

      std::string value; //< Param's value
      std::vector<JobParam> children; //< The vector of child JobParam objects this param contains
  };


  /// Represents a collection of JobParam
  class RUNMANAGER_API JobParams
  {
    public:
      /// Default constructor
      JobParams()
      {
      }

      /// Create a JobParams object from a vector of JobParam
      JobParams(const std::vector<JobParam> &t_params)
        : m_params(t_params)
      {}

      bool operator==(const JobParams &t_rhs) const
      {
        return std::equal(m_params.begin(), m_params.end(), t_rhs.m_params.begin());
      }

      /// Returns the vector of params contained.
      std::vector<JobParam> params() const
      {
        return m_params;
      }

      /// Append a collection of JobParams to this JobParams object
      void append(const JobParams &p)
      {
        m_params.insert(m_params.end(), p.m_params.begin(), p.m_params.end());
      }

      /// Append a JobParam to this JobParams object
      void append(const JobParam &p)
      {
        m_params.push_back(p);
      }

      /// Shortcut for appending a new JobParam to this collection.
      ///
      /// This function generates two JobParam objects. The first is the t_key value
      /// and the second is a child of the first, containing the t_value.
      /// 
      /// \param[in] t_key Param key
      /// \param[in] t_value Param value
      void append(const std::string &t_key, const std::string &t_value)
      {
        JobParam key(t_key);
        key.children.push_back(JobParam(t_value));
        append(key);
      }

      void append(const std::string &t_key, const JobParams &t_params)
      {
        JobParam key(t_key);
        key.children = t_params.m_params;
        append(key);

      }

      /// returns the JobParam with the given value if it exists
      /// \throws std::runtime_error if the value does not exist
      JobParam get(const std::string &value) const
      {
        return JobParam::paramByValue(m_params, value);
      }

      /// removes the JobParam with the given value if it exists
      void remove(const std::string &value) 
      {
        JobParam::removeByValue(m_params, value);
      }

      bool has(const std::string &value) const
      {
        return JobParam::hasByValue(m_params, value);
      }

    private:
      std::vector<JobParam> m_params;
  };

}
}


#endif // RUNMANAGER_LIB_JOBPARAM_HPP

