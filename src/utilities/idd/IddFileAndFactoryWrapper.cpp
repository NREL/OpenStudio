/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IddFileAndFactoryWrapper.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/embedded_files.hxx>

#include "../core/Assert.hpp"
#include "../core/Containers.hpp"

namespace openstudio {

// CONSTRUCTORS

IddFileAndFactoryWrapper::IddFileAndFactoryWrapper() : m_iddFileType(IddFileType(IddFileType::OpenStudio)) {}

IddFileAndFactoryWrapper::IddFileAndFactoryWrapper(const IddFile& iddFile) : m_iddFile(iddFile) {}

IddFileAndFactoryWrapper::IddFileAndFactoryWrapper(IddFileType iddFileType) : m_iddFileType(iddFileType) {
  if (iddFileType == IddFileType::UserCustom) {
    LOG(Warn, "IddFileType set to UserCustom without an actual IddFile being specified. Please "
                << "construct IddFileAndFactoryWrapper from an IddFile, or from a different IddFileType.");
  }
}

// GETTERS

std::string IddFileAndFactoryWrapper::version() const {
  std::string result;

  if (m_iddFile) {
    result = m_iddFile->version();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getVersion(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::string IddFileAndFactoryWrapper::header() const {
  std::string result;

  if (m_iddFile) {
    result = m_iddFile->header();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getHeader(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::vector<IddObject> IddFileAndFactoryWrapper::objects() const {
  IddObjectVector result;

  if (m_iddFile) {
    result = m_iddFile->objects();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getObjects(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::vector<std::string> IddFileAndFactoryWrapper::groups() const {
  StringVector result;

  if (m_iddFile) {
    result = m_iddFile->groups();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getGroups(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::vector<IddObject> IddFileAndFactoryWrapper::getObjectsInGroup(const std::string& group) const {
  IddObjectVector result;

  if (m_iddFile) {
    result = m_iddFile->getObjectsInGroup(group);
  } else if (m_iddFileType) {
    result = IddFactory::instance().getObjectsInGroup(group, *m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::vector<IddObject> IddFileAndFactoryWrapper::getObjects(const boost::regex& objectRegex) const {
  IddObjectVector result;

  if (m_iddFile) {
    result = m_iddFile->getObjects(objectRegex);
  } else if (m_iddFileType) {
    result = IddFactory::instance().getObjects(objectRegex, *m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

boost::optional<IddObject> IddFileAndFactoryWrapper::versionObject() const {
  OptionalIddObject result;

  if (m_iddFile) {
    result = m_iddFile->versionObject();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getVersionObject(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

boost::optional<IddObject> IddFileAndFactoryWrapper::getObject(const std::string& objectName) const {
  OptionalIddObject result;

  if (m_iddFile) {
    result = m_iddFile->getObject(objectName);
  } else if (m_iddFileType) {
    OptionalIddObject candidate = IddFactory::instance().getObject(objectName);
    if (candidate) {
      if (isInFile(candidate->type())) {
        result = candidate;
      }
    }
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

boost::optional<IddObject> IddFileAndFactoryWrapper::getObject(IddObjectType objectType) const {

  OptionalIddObject result;

  if (objectType == IddObjectType::UserCustom) {
    LOG(Info, "Asked to return IddObject of type IddObjectType::UserCustom. Since "
                << "UserCustom object types are generally not unique, returning false rather than "
                << "an IddObject. Please specify a different IddObjectType, or use " << "getObject(const std::string&).");
    return result;
  }

  if (m_iddFile) {
    result = m_iddFile->getObject(objectType);
  } else if (m_iddFileType) {
    if (isInFile(objectType)) {
      result = IddFactory::instance().getObject(objectType);
    }
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::vector<IddObject> IddFileAndFactoryWrapper::requiredObjects() const {

  IddObjectVector result;

  if (m_iddFile) {
    result = m_iddFile->requiredObjects();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getRequiredObjects(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

std::vector<IddObject> IddFileAndFactoryWrapper::uniqueObjects() const {

  IddObjectVector result;

  if (m_iddFile) {
    result = m_iddFile->uniqueObjects();
  } else if (m_iddFileType) {
    result = IddFactory::instance().getUniqueObjects(*m_iddFileType);
  } else {
    LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  }

  return result;
}

IddFile IddFileAndFactoryWrapper::iddFile() const {
  if (m_iddFile) {
    return *m_iddFile;
  } else if (m_iddFileType) {
    return IddFactory::instance().getIddFile(*m_iddFileType);
  }

  LOG_AND_THROW("Invalid IddFileAndFactoryWrapper has no IddFile set.");
  return {};
}

IddFileType IddFileAndFactoryWrapper::iddFileType() const {
  if (m_iddFileType) {
    return *m_iddFileType;
  }
  OS_ASSERT(m_iddFile);
  return {IddFileType::UserCustom};
}

// SETTERS

void IddFileAndFactoryWrapper::setIddFile(const IddFile& iddFile) {
  m_iddFile = iddFile;
  m_iddFileType = boost::none;
}

void IddFileAndFactoryWrapper::setIddFile(IddFileType iddFileType) {
  if (iddFileType == IddFileType::UserCustom) {
    if (m_iddFile) {
      LOG(Warn, "setIddFile(IddFileType::UserCustom) called when a custom IddFile has already been "
                  << "set. Taking no action (keeping the custom file already specified).");
      return;
    }
    // m_iddFileType is set to something other than IddFileType::UserCustom. Obey the user, but log
    // a warning.
    LOG(Warn, "IddFileType set to UserCustom without an actual IddFile being specified. Please "
                << "choose a different IddFileType, or setIddFile using an IddFile object.");
  }
  m_iddFile = boost::none;
  m_iddFileType = iddFileType;
}

// QUERIES

bool IddFileAndFactoryWrapper::isInFile(IddObjectType objectType) const {
  if (m_iddFile) {
    OptionalIddObject candidate = m_iddFile->getObject(objectType);
    return candidate.has_value();
  } else if (m_iddFileType) {
    return IddFactory::instance().isInFile(objectType, *m_iddFileType);
  }

  LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  return false;
}

bool IddFileAndFactoryWrapper::isInFile(const std::string& objectName) const {
  if (m_iddFile) {
    OptionalIddObject candidate = m_iddFile->getObject(objectName);
    return candidate.has_value();
  } else if (m_iddFileType) {
    OptionalIddObject candidate = IddFactory::instance().getObject(objectName);
    if (candidate) {
      return IddFactory::instance().isInFile(candidate->type(), *m_iddFileType);
    } else {
      return false;
    }
  }

  LOG_AND_THROW("Invalid IddFactoryWrapper has no IddFile set.");
  return false;
}

IddFile get_1_9_0_CBECC_IddFile() {
  std::stringstream ss;
  ss << ::openstudio::embedded_files::getFileAsString(":/idd/versions/1_9_0_CBECC/OpenStudio.idd");
  auto cbeccIddFile = IddFile::load(ss);
  OS_ASSERT(cbeccIddFile);
  return cbeccIddFile.get();
}

}  // namespace openstudio
