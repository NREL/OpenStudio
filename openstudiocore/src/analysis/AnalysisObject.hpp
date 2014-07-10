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

