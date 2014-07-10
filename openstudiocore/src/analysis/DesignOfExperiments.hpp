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

#ifndef ANALYSIS_DESIGNOFEXPERIMENTS_HPP
#define ANALYSIS_DESIGNOFEXPERIMENTS_HPP

#include "AnalysisAPI.hpp"
#include "OpenStudioAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class DesignOfExperimentsOptions;

namespace detail {

  class DesignOfExperiments_Impl;

} // detail

/** DesignOfExperiments is an OpenStudioAlgorithm. Currently, DesignOfExperiments may be used to 
 *  perform full mesh parametric analyses on \link Problem Problems \endlink for which 
 *  Problem::allVariablesAreDiscrete. DesignOfExperiments::createNextIteration adds all \link 
 *  DataPoint DataPoints \endlink at once, in one batch. */
class ANALYSIS_API DesignOfExperiments : public OpenStudioAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DesignOfExperiments(const DesignOfExperimentsOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  DesignOfExperiments(const UUID& uuid, 
                      const UUID& versionUUID,
                      const std::string& displayName,
                      const std::string& description,
                      bool complete,
                      bool failed,
                      int iter,
                      const DesignOfExperimentsOptions& options);

  virtual ~DesignOfExperiments() {}

  //@}

  static std::string standardName();
  
  /** @name Getters and Queries */
  //@{

  DesignOfExperimentsOptions designOfExperimentsOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::DesignOfExperiments_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::DesignOfExperiments_Impl;

  explicit DesignOfExperiments(std::shared_ptr<detail::DesignOfExperiments_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DesignOfExperiments");
};

/** \relates DesignOfExperiments*/
typedef boost::optional<DesignOfExperiments> OptionalDesignOfExperiments;

/** \relates DesignOfExperiments*/
typedef std::vector<DesignOfExperiments> DesignOfExperimentsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DESIGNOFEXPERIMENTS_HPP

