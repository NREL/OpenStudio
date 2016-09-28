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

#ifndef ANALYSIS_ANALYSISOBJECT_HPP
#define ANALYSIS_ANALYSISOBJECT_HPP

#include "AnalysisAPI.hpp"

#include "../utilities/core/UUID.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {
namespace analysis {

namespace detail {

  class AnalysisObject_Impl;

} // detail

/** AnalysisObject is the base class for all major classes in the openstudio::analysis namespace.
 *  AnalysisObjects are used to formulate \link Problem Problems \endlink and their instantiations
 *  as \link Analysis Analyses\endlink, and to explore solutions in the form of \link Analysis
 *  Analyses\endlink and their complete (simulated) \link DataPoint DataPoints\endlink. All
 *  AnalysisObjects are serializable to the project::ProjectDatabase, and in fact are serialized
 *  to such a database by analysis::AnalysisDriver, which is the preferred method for running
 *  \link Analysis Analyses\endlink. */
class ANALYSIS_API AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~AnalysisObject() {}

  /** Deep copy. */
  AnalysisObject clone() const;

  //@}
  /** @name Getters and Queries */
  //@{

  std::string name() const;

  std::string displayName() const;

  std::string description() const;

  openstudio::UUID uuid() const;

  openstudio::UUID versionUUID() const;

  /** Returns true if this object has been changed since its dirty flag was last cleared. New
   *  objects are dirty post-construction. De-serialized objects are not. The intention is for
   *  users of the analysis framework to clear the dirty flag after an object is saved to its
   *  ProjectDatabase. */
  bool isDirty() const;

  /** Compare impls. */
  bool operator==(const AnalysisObject& other) const;

  bool operator!=(const AnalysisObject& other) const;

  bool uuidEqual(const AnalysisObject& other) const;

  bool uuidAndVersionEqual(const AnalysisObject& other) const;

  boost::optional<AnalysisObject> parent() const;

  //@}
  /** @name Setters */
  //@{

  void setName(const std::string& newName);

  void setDisplayName(const std::string& newDisplayName);

  void setDescription(const std::string& newDescription);

  //@}
  /** @name Actions */
  //@{

  /** Call this method to reset the dirty flag of this object and all its children. In general,
   *  this method should be called after this object has been saved to the ProjectDatabase
   *  (record constructed, database saved, and transaction committed). Returns false if the flag
   *  cannot be cleared for some reason. */
  bool clearDirtyFlag();

  /** Connect signal from this AnalysisObject to slot on receiver. */
  bool connect(const char* signal,
               const QObject* receiver,
               const char* slot,
               Qt::ConnectionType type = Qt::AutoConnection) const;

  bool disconnect(const char* signal=nullptr,
                  const QObject* receiver=nullptr,
                  const char* slot=nullptr) const;

  //@}
  /** @name Type Casting */
  //@{

  /** Get the impl pointer */
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false)
   *  if object is not a T. */
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::AnalysisObject_Impl ImplType;

  friend class detail::AnalysisObject_Impl;

  explicit AnalysisObject(std::shared_ptr<detail::AnalysisObject_Impl> impl);

  // Method is const because parent data is mutable. Not ideal programming style, but preserves
  // constructor interfaces, and parent data is not directly serialized.
  void setParent(const AnalysisObject& parent) const;

  // Method is const because parent data is mutable. Not ideal programming style, but preserves
  // constructor interfaces, and parent data is not directly serialized.
  void clearParent() const;

  // Exposing method for calling onChange so parents that keep up with order of their
  // children can mark children as dirty for the ProjectDatabase.
  virtual void onChange();

  // Returns this object's data as a QVariant that can be serialized to JSON.
  QVariant toVariant() const;

  // Returns this object's data as a QVariant that can be serialized to the openstudio-server
  // problem formulation JSON.
  QVariant toServerFormulationVariant() const;

  // Returns this object's data as a QVariant that can be serialized to the openstudio-server
  // data points request JSON.
  QVariant toServerDataPointsVariant() const;

  /// @endcond
 private:

  std::shared_ptr<detail::AnalysisObject_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysis.AnalysisObject");
};

/** \relates AnalysisObject*/
typedef boost::optional<AnalysisObject> OptionalAnalysisObject;

/** \relates AnalysisObject*/
typedef std::vector<AnalysisObject> AnalysisObjectVector;

struct ANALYSIS_API AnalysisJSONLoadResult {
  /// loaded object
  boost::optional<AnalysisObject> analysisObject;
  openstudio::path projectDir;     /// metadata
  VersionString originalOSVersion; /// metadata
  /// errors -- Only !(.empty()) if (!analysisObject). Call .logMessage to get a std::string.
  std::vector<LogMessage> errors;

  AnalysisJSONLoadResult(const AnalysisObject& t_analysisObject,
                         const openstudio::path& t_projectDir,
                         const VersionString& t_originalOSVersion);

  AnalysisJSONLoadResult(const std::vector<LogMessage>& t_errors);
};

/** Factory method for loading JSON containing an AnalysisObject. Returned object will be of the
 *  correct type. \relates AnalysisObject \relates Analysis \relates DataPoint */
ANALYSIS_API AnalysisJSONLoadResult loadJSON(const openstudio::path& p);

/** Factory method for loading JSON containing an AnalysisObject. Returned object will be of the
 *  correct type. \relates AnalysisObject \relates Analysis \relates DataPoint */
ANALYSIS_API AnalysisJSONLoadResult loadJSON(std::istream& json);

/** Factory method for loading JSON containing an AnalysisObject. Returned object will be of the
 *  correct type. \relates AnalysisObject \relates Analysis \relates DataPoint */
ANALYSIS_API AnalysisJSONLoadResult loadJSON(const std::string& json);

} // analysis
} // openstudio

#endif // ANALYSIS_ANALYSISOBJECT_HPP

