/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
    /// Defaults to succeeded = false
    JobErrors()
      : result(openstudio::ruleset::OSResultValue::Fail), numNAs(0)
    {
    }

    /// Constructor
    /// \param[in] t_result Set to true if the job was considered a success regardless of errors generated
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
