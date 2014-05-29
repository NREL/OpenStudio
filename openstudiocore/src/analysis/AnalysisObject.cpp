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

#include "AnalysisObject.hpp"
#include "AnalysisObject_Impl.hpp"

// for deserializing top-level json files
#include "Analysis.hpp"
#include "Analysis_Impl.hpp"
#include "DataPoint.hpp"
#include "DataPoint_Impl.hpp"

#include "../utilities/core/Json.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  AnalysisObject_Impl::AnalysisObject_Impl(const std::string& name)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_displayName(name),
      m_dirty(true)
  {}

  AnalysisObject_Impl::AnalysisObject_Impl(const UUID& uuid,
                                           const UUID& versionUUID,
                                           const std::string& name,
                                           const std::string& displayName,
                                           const std::string& description)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_description(description),
      m_dirty(false)
  {}

  AnalysisObject_Impl::AnalysisObject_Impl(const AnalysisObject_Impl &other)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(other.name()),
      m_displayName(other.displayName()),
      m_description(other.description()),
      m_dirty(true)
  {}

  std::string AnalysisObject_Impl::name() const {
    return m_name;
  }

  std::string AnalysisObject_Impl::displayName() const {
    return m_displayName;
  }

  std::string AnalysisObject_Impl::description() const {
    return m_description;
  }

  openstudio::UUID AnalysisObject_Impl::uuid() const {
    return m_uuid;
  }

  openstudio::UUID AnalysisObject_Impl::versionUUID() const {
    return m_versionUUID;
  }

  bool AnalysisObject_Impl::isDirty() const {
    return m_dirty;
  }

  bool AnalysisObject_Impl::uuidEqual(const AnalysisObject& other) const {
    return (uuid() == other.uuid());
  }

  bool AnalysisObject_Impl::uuidAndVersionEqual(const AnalysisObject& other) const {
    return ((uuid() == other.uuid()) && (versionUUID() == other.versionUUID()));
  }

  void AnalysisObject_Impl::setName(const std::string& newName) {
    m_name = newName;
    onChange(AnalysisObject_Impl::Benign);
  }

  void AnalysisObject_Impl::setDisplayName(const std::string& newDisplayName) {
    m_displayName = newDisplayName;
    onChange(AnalysisObject_Impl::Benign);
  }

  void AnalysisObject_Impl::setDescription(const std::string& newDescription) {
    m_description = newDescription;
    onChange(AnalysisObject_Impl::Benign);
  }

  bool AnalysisObject_Impl::clearDirtyFlag() {
    m_dirty = false;
    emit clean();
    return true;
  }

  bool AnalysisObject_Impl::connect(const char* signal,
                                    const QObject* receiver,
                                    const char* slot,
                                    Qt::ConnectionType type) const
  {
    return QObject::connect(this, signal, receiver, slot, type);
  }

  bool AnalysisObject_Impl::disconnect(const char* signal,
                                       const QObject* receiver,
                                       const char* slot) const
  {
    return QObject::disconnect(this,signal,receiver,slot);
  }

  void AnalysisObject_Impl::updateInputPathData(const openstudio::path& originalBase,
                                                const openstudio::path& newBase)
  {}

  void AnalysisObject_Impl::onChange(ChangeType changeType) {
    m_versionUUID = createUUID();
    m_dirty = true;
    emit changed(changeType);
  }

  boost::optional<AnalysisObject> AnalysisObject_Impl::parent() const {
    return m_parent;
  }

  void AnalysisObject_Impl::setParent(const AnalysisObject& parent) const {
    m_parent = parent;
  }

  void AnalysisObject_Impl::clearParent() const {
    m_parent.reset();
  }

  QVariant AnalysisObject_Impl::toVariant() const {
    QVariantMap analysisObjectData;

    analysisObjectData["uuid"] = toQString(removeBraces(uuid()));
    analysisObjectData["version_uuid"] = toQString(removeBraces(versionUUID()));
    std::string str = name();
    if (!str.empty()) {
      analysisObjectData["name"] = toQString(str);
    }
    str = displayName();
    if (!str.empty()) {
      analysisObjectData["display_name"] = toQString(str);
    }
    str = description();
    if (!str.empty()) {
      analysisObjectData["description"] = toQString(str);
    }

    return QVariant(analysisObjectData);
  }

  QVariant AnalysisObject_Impl::toServerFormulationVariant() const {
    return QVariant();
  }

  QVariant AnalysisObject_Impl::toServerDataPointsVariant() const {
    return QVariant();
  }

  void AnalysisObject_Impl::onChildChanged(ChangeType changeType) {
    onChange(changeType);
  }

  void AnalysisObject_Impl::onParentClean() {
    clearDirtyFlag();
  }

  void AnalysisObject_Impl::connectChild(AnalysisObject& child, bool setParent) const {
    if (setParent) {
      AnalysisObject copyOfThis = getPublicObject<AnalysisObject>();
      child.setParent(copyOfThis);
    }

    bool connected = connect(SIGNAL(clean()),
                             child.getImpl<detail::AnalysisObject_Impl>().get(),
                             SLOT(onParentClean()));
    OS_ASSERT(connected);
    connected = child.connect(SIGNAL(changed(ChangeType)),
                              this,
                              SLOT(onChildChanged(ChangeType)));
    OS_ASSERT(connected);
  }

  void AnalysisObject_Impl::disconnectChild(AnalysisObject& child,bool clearParent) const {
    if (clearParent) {
      child.clearParent();
    }

    bool disconnected = disconnect(SIGNAL(clean()),
                                   child.getImpl<detail::AnalysisObject_Impl>().get(),
                                   SLOT(onParentClean()));
    OS_ASSERT(disconnected);
    disconnected = child.disconnect(SIGNAL(changed(ChangeType)),
                                    this,
                                    SLOT(onChildChanged(ChangeType)));
    OS_ASSERT(disconnected);
  }

  void AnalysisObject_Impl::setDirtyFlag() {
    m_dirty = true;
  }

} // detail

AnalysisObject AnalysisObject::clone() const {
  return m_impl->clone();
}

std::string AnalysisObject::name() const {
  return getImpl<detail::AnalysisObject_Impl>()->name();
}

std::string AnalysisObject::displayName() const {
  return getImpl<detail::AnalysisObject_Impl>()->displayName();
}

std::string AnalysisObject::description() const {
  return getImpl<detail::AnalysisObject_Impl>()->description();
}

openstudio::UUID AnalysisObject::uuid() const {
  return getImpl<detail::AnalysisObject_Impl>()->uuid();
}

openstudio::UUID AnalysisObject::versionUUID() const {
  return getImpl<detail::AnalysisObject_Impl>()->versionUUID();
}

bool AnalysisObject::isDirty() const {
  return getImpl<detail::AnalysisObject_Impl>()->isDirty();
}

bool AnalysisObject::operator==(const AnalysisObject& other) const {
  return m_impl == other.m_impl;
}

bool AnalysisObject::operator!=(const AnalysisObject& other) const {
  return !(operator==(other));
}

bool AnalysisObject::uuidEqual(const AnalysisObject& other) const {
  return getImpl<detail::AnalysisObject_Impl>()->uuidEqual(other);
}

bool AnalysisObject::uuidAndVersionEqual(const AnalysisObject& other) const {
  return getImpl<detail::AnalysisObject_Impl>()->uuidAndVersionEqual(other);
}

void AnalysisObject::setName(const std::string& newName) {
  getImpl<detail::AnalysisObject_Impl>()->setName(newName);
}

void AnalysisObject::setDisplayName(const std::string& newDisplayName) {
  getImpl<detail::AnalysisObject_Impl>()->setDisplayName(newDisplayName);
}

void AnalysisObject::setDescription(const std::string& newDescription) {
  getImpl<detail::AnalysisObject_Impl>()->setDescription(newDescription);
}

bool AnalysisObject::clearDirtyFlag() {
  return getImpl<detail::AnalysisObject_Impl>()->clearDirtyFlag();
}

/** Connect signal from this AnalysisObject to slot on qObject. */
bool AnalysisObject::connect(const char* signal,
                             const QObject* qObject,
                             const char* slot,
                             Qt::ConnectionType type) const
{
  return getImpl<detail::AnalysisObject_Impl>()->connect(signal,qObject,slot,type);
}

bool AnalysisObject::disconnect(const char* signal,
                                const QObject* receiver,
                                const char* slot) const
{
  return getImpl<detail::AnalysisObject_Impl>()->disconnect(signal,receiver,slot);
}

/// @cond
AnalysisObject::AnalysisObject(std::shared_ptr<detail::AnalysisObject_Impl> impl)
  : m_impl(impl)
{}

boost::optional<AnalysisObject> AnalysisObject::parent() const {
  return getImpl<detail::AnalysisObject_Impl>()->parent();
}

void AnalysisObject::setParent(const AnalysisObject& parent) const {
  getImpl<detail::AnalysisObject_Impl>()->setParent(parent);
}

void AnalysisObject::clearParent() const {
  getImpl<detail::AnalysisObject_Impl>()->clearParent();
}

void AnalysisObject::onChange() {
  // Generally used by a parent that will make their own call to onChange with
  // the proper ChangeType.
  getImpl<detail::AnalysisObject_Impl>()->onChange(detail::AnalysisObject_Impl::Benign);
}

QVariant AnalysisObject::toVariant() const {
  return getImpl<detail::AnalysisObject_Impl>()->toVariant();
}

QVariant AnalysisObject::toServerFormulationVariant() const {
  return getImpl<detail::AnalysisObject_Impl>()->toServerFormulationVariant();
}

QVariant AnalysisObject::toServerDataPointsVariant() const {
  return getImpl<detail::AnalysisObject_Impl>()->toServerDataPointsVariant();
}

/// @endcond

AnalysisJSONLoadResult::AnalysisJSONLoadResult(const AnalysisObject& t_analysisObject,
                                               const openstudio::path& t_projectDir,
                                               const VersionString& t_originalOSVersion)
  : analysisObject(t_analysisObject),
    projectDir(t_projectDir),
    originalOSVersion(t_originalOSVersion)
{}

AnalysisJSONLoadResult::AnalysisJSONLoadResult(const std::vector<LogMessage>& t_errors)
  : originalOSVersion("0.0.0"),
    errors(t_errors)
{}

AnalysisJSONLoadResult loadJSON(const openstudio::path& p) {
  OptionalAnalysisObject result;
  StringStreamLogSink logger;
  logger.setLogLevel(Error);

  try {
    QVariant variant = openstudio::loadJSON(p);
    VersionString version = extractOpenStudioVersion(variant);
    QVariantMap map = variant.toMap();
    QVariantMap objectMap;
    if (map.contains("data_point")) {
      // leave objectMap blank, because it cannot contain project_dir
      result = detail::DataPoint_Impl::factoryFromVariant(map["data_point"],version,boost::none);
    }
    else if (map.contains("analysis")) {
      objectMap = map["analysis"].toMap();
      result = detail::Analysis_Impl::fromVariant(objectMap,version);
    }
    else {
      LOG_FREE_AND_THROW("openstudio.analysis.AnalysisObject",
                         "The file at " << toString(p) << " does not contain a data_point or "
                         << "an analysis.");
    }
    OS_ASSERT(result);
    openstudio::path projectDir;
    if (version < VersionString("1.1.2")) {
      OS_ASSERT(map.contains("metadata"));
      if (map["metadata"].toMap().contains("project_dir")) {
        projectDir = toPath(map["metadata"].toMap()["project_dir"].toString());
      }
    }
    else {
      if (objectMap.contains("project_dir")) {
        projectDir = toPath(objectMap["project_dir"].toString());
      }
    }
    return AnalysisJSONLoadResult(*result,projectDir,version);
  }
  catch (std::exception& e) {
    LOG_FREE(Error,"openstudio.analysis.AnalysisObject",
             "The file at " << toString(p) << " cannot be parsed as an OpenStudio "
             << "analysis framework json file, because " << e.what());
  }
  catch (...) {
    LOG_FREE(Error,"openstudio.analysis.AnalysisObject",
             "The file at " << toString(p) << " cannot be parsed as an OpenStudio "
             << "analysis framework json file.");
  }

  return AnalysisJSONLoadResult(logger.logMessages());
}

AnalysisJSONLoadResult loadJSON(std::istream& json) {
  return loadJSON(toString(json));
}

AnalysisJSONLoadResult loadJSON(const std::string& json) {
  OptionalAnalysisObject result;
  StringStreamLogSink logger;
  logger.setLogLevel(Error);

  try {
    QVariant variant = openstudio::loadJSON(json);
    VersionString version = extractOpenStudioVersion(variant);
    QVariantMap map = variant.toMap();
    QVariantMap objectMap;
    if (map.contains("data_point")) {
      // leave objectMap blank, because it cannot contain project_dir
      result = detail::DataPoint_Impl::factoryFromVariant(map["data_point"],version,boost::none);
    }
    else if (map.contains("analysis")) {
      objectMap = map["analysis"].toMap();
      result = detail::Analysis_Impl::fromVariant(objectMap,version);
    }
    else {
      LOG_FREE_AND_THROW("openstudio.analysis.AnalysisObject",
                         "The parsed json string does not contain a data_point or an analysis.");
    }
    OS_ASSERT(result);
    openstudio::path projectDir;
    if (version < VersionString("1.1.2")) {
      OS_ASSERT(map.contains("metadata"));
      if (map["metadata"].toMap().contains("project_dir")) {
        projectDir = toPath(map["metadata"].toMap()["project_dir"].toString());
      }
    }
    else {
      if (objectMap.contains("project_dir")) {
        projectDir = toPath(objectMap["project_dir"].toString());
      }
    }
    return AnalysisJSONLoadResult(*result,projectDir,version);
  }
  catch (std::exception& e) {
    LOG_FREE(Error,"openstudio.analysis.AnalysisObject",
             "The json string cannot be parsed as an OpenStudio analysis framework "
             << "json file, because " << e.what() << ".");
  }
  catch (...) {
    LOG_FREE(Error,"openstudio.analysis.AnalysisObject",
             "The following string cannot be parsed as an OpenStudio analysis framework "
             << "json file." << std::endl << std::endl << json);
  }

  return AnalysisJSONLoadResult(logger.logMessages());
}

} // analysis
} // openstudio

