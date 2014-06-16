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

#ifndef RUNMANAGER_LIB_JOBERRORS_HPP
#define RUNMANAGER_LIB_JOBERRORS_HPP

#include "RunManagerAPI.hpp"
#include "../../ruleset/OSResult.hpp"
#include <QMetaType>

namespace openstudio {
namespace runmanager {
  OPENSTUDIO_ENUM( ErrorType, 
      ((Error)(Error)(0))
      ((Warning)(Warning)(1))
      ((Info)(Information)(2))
      ((InitialCondition)(InitialCondition)(3))
      ((FinalCondition)(FinalCondition)(4))
    );

  /**
   * Holds a vector of errors and warnings descriptions generated during a job execution plus a flag
   * indicating if the job could be considered a success regardless of errors
   */
  struct RUNMANAGER_API JobErrors 
  {
    /// Default constructor
    /// Defaults to succeded = false
    JobErrors()
      : result(openstudio::ruleset::OSResultValue::Fail), numNAs(0)
    {
    }

    /// Constructor
    /// \param[in] t_result Set to true if the job was considered a success regardless of errors generated
    /// \param[in] t_warnings The vector of warning description strings to set
    /// \param[in] t_errors The vector of error description strings to set
    JobErrors(openstudio::ruleset::OSResultValue t_result, 
        const std::vector<std::pair<ErrorType, std::string> > &t_errors)
      : result(t_result), allErrors(t_errors)
    {
      if (t_result == openstudio::ruleset::OSResultValue::NA){
        numNAs = 1;
      }else{
        numNAs = 0;
      }
    }

    /// Add a JobErrors object to this one
    ///
    /// \param[in] t_rhs Right hand side of operation
    ///
    /// \returns a new JobErrors object consisting of the set of warnings and errors from the RHS
    ///          concatenated to the end of the warnings and errors from the LHS. Succeeded is set to
    ///          true in the result only if both lhs and rhs result.
    JobErrors operator+(const JobErrors &t_rhs) const
    {
      JobErrors ret(*this);
      ret.allErrors.insert(ret.allErrors.end(), t_rhs.allErrors.begin(), t_rhs.allErrors.end());
      ret.numNAs += t_rhs.numNAs;
      
      if (ret.result == openstudio::ruleset::OSResultValue::Fail
          || t_rhs.result == openstudio::ruleset::OSResultValue::Fail)
      {
        ret.result = openstudio::ruleset::OSResultValue::Fail;
      } else if (ret.result == openstudio::ruleset::OSResultValue::Success
          || t_rhs.result == openstudio::ruleset::OSResultValue::Success) {
        ret.result = openstudio::ruleset::OSResultValue::Success;
      } else {
        ret.result = openstudio::ruleset::OSResultValue::NA;
      }

      return ret;
    }

    std::vector<std::pair<int, std::string> > errorsByTypeWithCount(const ErrorType &t_et) const;

    int totalCountByType(const ErrorType &t_et) const;


    std::vector<std::string> errorsByType(const ErrorType &t_et) const
    {
      std::vector<std::string> results;

      for (const auto & error : allErrors)
      {
        if (error.first == t_et)
        {
          results.push_back(error.second);
        }
      }

      return results;
    }

    std::vector<std::string> errors() const
    {
      return errorsByType(ErrorType::Error);
    }

    std::vector<std::string> warnings() const
    {
      return errorsByType(ErrorType::Warning);
    }

    std::vector<std::string> infos() const
    {
      return errorsByType(ErrorType::Info);
    }

    std::vector<std::string> initialConditions() const
    {
      return errorsByType(ErrorType::InitialCondition);
    }

    std::vector<std::string> finalConditions() const
    {
      return errorsByType(ErrorType::FinalCondition);
    }

    void addError(ErrorType t_et, const std::string &t_value)
    {
      allErrors.push_back(std::make_pair(t_et, t_value));
    }

    bool succeeded() const
    {
      return result != openstudio::ruleset::OSResultValue::Fail;
    }

    bool operator==(const JobErrors &t_rhs) const;

    openstudio::ruleset::OSResultValue result; 
    std::vector<std::pair<ErrorType, std::string> > allErrors; //< List of warning strings collected about job
    unsigned numNAs;
  };


}
}

Q_DECLARE_METATYPE(openstudio::runmanager::JobErrors);

#endif // RUNMANAGER_LIB_JOBERRORS_HPP
