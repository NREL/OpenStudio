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
#include "BCLComponent.hpp"
#include "BCLMeasure.hpp"
#include "LocalBCL.hpp"
#include "RemoteBCL.hpp"
#include "OnDemandGenerator.hpp"
#include "../core/Application.hpp"
#include "../core/Assert.hpp"
#include "../data/Attribute.hpp"
#include "../core/Path.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/System.hpp"

#include <QDir>
#include <QFile>
#include <QIcon>
#include <QInputDialog>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>

#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <set>
#include <vector>

namespace openstudio{

  LocalBCL::LocalBCL(const path& libraryPath):
    m_libraryPath(QDir().cleanPath(toQString(libraryPath))),
    m_dbName(QString("/components.sql")),
    dbVersion("1.3")
  {
    //Make sure a QApplication exists
    openstudio::Application::instance().application();

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", m_libraryPath+m_dbName);
    database.setDatabaseName(m_libraryPath+m_dbName);

    //Check for BCL directory
    if (!QDir(m_libraryPath).exists())
    {
      QDir().mkpath(m_libraryPath);
    }
    //Check for local database
    if (!QFile(m_libraryPath+m_dbName).exists())
    {
      initializeLocalDb();
    }

    bool test = database.isValid();
    OS_ASSERT(test);
    if (!database.isOpen())
    {
      test = database.open();
      OS_ASSERT(test);
    }

    //Check for out-of-date database
    updateLocalDb();

    //Retrieve oauthConsumerKeys from database
    QSqlQuery query(database);
    query.exec("SELECT data FROM Settings WHERE name='prodAuthKey'");
    if (query.next())
    {
      m_prodAuthKey = toString(query.value(0).toString());
    }
    query.exec("SELECT data FROM Settings WHERE name='devAuthKey'");
    if (query.next())
    {
      m_devAuthKey = toString(query.value(0).toString());
    }
  }

  // http://sqlite.org/faq.html#q14
  QString LocalBCL::escape(const std::string& s) const
  {
      return toQString(s).replace("'","''");
  }

  LocalBCL &LocalBCL::instance()
  {
    std::shared_ptr<LocalBCL> &ptr = instanceInternal();
    if (!ptr) {
      QSettings settings("OpenStudio", "LocalBCL");
      // DLM: might want to put this somewhere a little more hidden
      ptr = std::shared_ptr<LocalBCL>(new LocalBCL(toPath(settings.value("libraryPath",
        QDir::homePath().append("/BCL")).toString())));
    }
    return *ptr;
  }

  LocalBCL &LocalBCL::instance(const path& libraryPath)
  {
    std::shared_ptr<LocalBCL> &ptr = instanceInternal();
    if (ptr) {
      ptr = std::shared_ptr<LocalBCL>(new LocalBCL(libraryPath));
    }
    else
    {
      if (ptr->libraryPath() != toQString(libraryPath)) {
        ptr.reset();
        ptr = std::shared_ptr<LocalBCL>(new LocalBCL(libraryPath));
      }
    }
    return *ptr;
  }

  std::shared_ptr<LocalBCL> &LocalBCL::instanceInternal()
  {
    static std::shared_ptr<LocalBCL> instance;
    return instance;
  }

  void LocalBCL::close()
  {
    instanceInternal().reset();
  }

  LocalBCL::~LocalBCL()
  {
    // we cannot cleanup if the driver has already bee
    if (QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
      {
        QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName, false);
        if (database.isValid() && database.isOpen())
        {
          database.close();
        }
      }
      QSqlDatabase::removeDatabase(m_libraryPath+m_dbName);
    }
  }

  bool LocalBCL::initializeLocalDb()
  {
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    if (database.open())
    {
      QSqlQuery query(database);
      bool success = query.exec("CREATE TABLE Settings (name VARCHAR, data VARCHAR)");
      success = success && query.exec("CREATE TABLE Components (uid VARCHAR, version_id VARCHAR, "
        "name VARCHAR, description VARCHAR, date_added DATETIME, date_modified DATETIME)");
      success = success && query.exec("CREATE TABLE Files (uid VARCHAR, version_id VARCHAR, filename VARCHAR, "
        "filetype VARCHAR, usage_type VARCHAR, checksum VARCHAR)");
      success = success && query.exec("CREATE TABLE Attributes (uid VARCHAR, version_id VARCHAR, name VARCHAR, "
        "value VARCHAR, units VARCHAR, type VARCHAR)");
      success = success && query.exec("CREATE TABLE Measures (uid VARCHAR, version_id VARCHAR, name VARCHAR, "
        "description VARCHAR, modeler_description VARCHAR, date_added DATETIME, date_modified DATETIME)");
      query.prepare("INSERT INTO Settings VALUES (:name, :data)");
      query.bindValue(":name", "dbVersion");
      query.bindValue(":data", dbVersion);
      success = success && query.exec();
      query.bindValue(":name", "prodAuthKey");
      query.bindValue(":data", "");
      success = success && query.exec();
      query.bindValue(":name", "devAuthKey");
      query.bindValue(":data", "");
      success = success && query.exec();
      return success;
    }
    return false;
  }

  bool LocalBCL::updateLocalDb()
  {
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);

    // If latest version, do nothing
    bool success = query.exec("SELECT data FROM Settings WHERE name='dbVersion'");
    if (success && query.next())
    {
      QString localDbVersion = query.value(0).toString();
      if (localDbVersion == dbVersion)
      {
        return true;
      }
    }

    // 1.0, 1.1 -> 1.2
    success = query.exec("SELECT oauthConsumerKey, dbVersion FROM Settings");
    if (success && query.next())
    {
      QString oauthConsumerKey = query.value(0).toString();
      QString localDbVersion = query.value(1).toString();
      if (localDbVersion == "1.1" || localDbVersion == "1.0")
      {
        success = success && query.exec("ALTER TABLE Components ADD date_added DATETIME");
        success = success && query.exec("ALTER TABLE Components ADD date_modified DATETIME");

        success = success && query.exec("DROP TABLE Settings");
        success = success && query.exec("CREATE TABLE Settings (name VARCHAR, data VARCHAR)");
        query.prepare("INSERT INTO Settings VALUES (:name, :data)");
        query.bindValue(":name", "dbVersion");
        query.bindValue(":data", dbVersion);
        success = success && query.exec();

        query.bindValue(":name", "prodAuthKey");
        query.bindValue(":data", oauthConsumerKey);
        success = success && query.exec();

        query.bindValue(":name", "devAuthKey");
        query.bindValue(":data", "");
        success = success && query.exec();
        return success;
      }
    }

    // 1.2 -> 1.3
    success = query.exec("SELECT data FROM Settings WHERE name='dbVersion'");
    if (success && query.next())
    {
      QString localDbVersion = query.value(0).toString();
      if (localDbVersion == "1.2")
      {
        success = success && query.exec("ALTER TABLE Attributes ADD version_id VARCHAR");
        success = success && query.exec("ALTER TABLE Files ADD version_id VARCHAR");
        success = success && query.exec("SELECT uid, version_id FROM Components WHERE uid IN (SELECT DISTINCT uid FROM Components)");
        QSqlQuery queryLoop(database);
        while (query.next()) {
          queryLoop.prepare("UPDATE Attributes SET version_ID = :versionId WHERE uid = :uid");
          queryLoop.bindValue(":versionId", query.value(1));
          queryLoop.bindValue(":uid", query.value(0));
          success = success && queryLoop.exec();
          queryLoop.prepare("UPDATE Files SET version_ID = :versionId WHERE uid = :uid");
          queryLoop.bindValue(":versionId", query.value(1));
          queryLoop.bindValue(":uid", query.value(0));
          success = success && queryLoop.exec();
        }

        success = success && query.exec("UPDATE Attributes SET type = 'string' WHERE type = 'varchar'");

        query.exec("SELECT uid, version_id, directory FROM Components");
        while (query.next()) {
          path src = toPath(m_libraryPath + "/" + query.value(0).toString() + "/" + query.value(1).toString() + query.value(2).toString().mid(query.value(2).toString().lastIndexOf("/")));
          path dest = src.parent_path();
          QFile::remove(toQString(dest / toPath("DISCLAIMER.txt")));
          QFile::remove(toQString(dest / toPath("README.txt")));
          QFile::remove(toQString(dest / toPath("output.xml")));
          copyDirectory(src, dest);
          removeDirectory(src);
        }

        success = success && query.exec("ALTER TABLE Components RENAME TO ComponentsTmp");
        success = success && query.exec("CREATE TABLE Components(uid VARCHAR, version_id VARCHAR, name VARCHAR, description VARCHAR, date_added DATETIME, date_modified DATETIME)");
        success = success && query.exec("INSERT INTO Components SELECT uid, version_id, name, type, date_added, date_modified FROM ComponentsTmp");
        success = success && query.exec("DROP TABLE ComponentsTmp");

        success = success && query.exec("ALTER TABLE Files ADD usage_type VARCHAR");
        success = success && query.exec("ALTER TABLE Files ADD checksum VARCHAR");

        success = success && query.exec("CREATE TABLE Measures (uid VARCHAR, version_id VARCHAR, name VARCHAR, description VARCHAR, modeler_description VARCHAR, date_added DATETIME, date_modified DATETIME)");

        query.prepare("UPDATE Settings SET data = :dbVersion WHERE name = 'dbVersion'");
        query.bindValue(":dbVersion", dbVersion);
        success = success && query.exec();
        return success;
      }
    }

    return false;
  }

  /// Inherited members

  boost::optional<BCLComponent> LocalBCL::getComponent(const std::string& uid, const std::string& versionId) const
  {
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    if (versionId.empty())
    {
      query.exec(QString("SELECT version_id FROM Components WHERE uid='%1'").arg(escape(uid)));
      if (query.next())
      {
        return boost::optional<BCLComponent>(toString(m_libraryPath) + "/" + uid + "/" + toString(query.value(0).toString()));
      }
      return boost::none;
    }
    query.exec(QString("SELECT version_id FROM Components WHERE uid='%1' AND version_id='%2'").arg(escape(uid), escape(versionId)));
    if (query.next())
    {
      return boost::optional<BCLComponent>(toString(m_libraryPath) + "/" + uid + "/" + versionId);
    }
    return boost::none;
  }

  boost::optional<BCLMeasure> LocalBCL::getMeasure(const std::string& uid, const std::string& versionId) const
  {
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    if (versionId.empty())
    {
      query.exec(QString("SELECT version_id FROM Measures WHERE uid='%1'").arg(escape(uid)));
      if (query.next())
      {
        try{
          return boost::optional<BCLMeasure>(toPath(m_libraryPath) / toPath(uid) / toPath(toString(query.value(0).toString())));
        }catch(const std::exception&){
        }
      }
      return boost::none;
    }
    query.exec(QString("SELECT version_id FROM Measures WHERE uid='%1' AND version_id='%2'").arg(escape(uid), escape(versionId)));
    if (query.next())
    {
      try{
        return boost::optional<BCLMeasure>(toPath(m_libraryPath) / toPath(uid) / toPath(versionId));
      }catch(const std::exception&){
      }
    }
    return boost::none;
  }

  boost::optional<BCLComponent> LocalBCL::getOnDemandComponent(const OnDemandGenerator& generator) const
  {
    if (!generator.checkArgumentValues()){
      return boost::none;
    }

    std::vector<std::pair<std::string, std::string> > searchTerms;

    searchTerms.push_back(std::make_pair<std::string, std::string>("OnDemandGenerator UID", generator.uid()));
    searchTerms.push_back(std::make_pair<std::string, std::string>("OnDemandGenerator VID", generator.versionId()));

    for (const OnDemandGeneratorArgument& argument : generator.activeArguments()) {

      // can't handle other types for now
      if (argument.dataType() != OnDemandGeneratorArgumentType::String){
        return boost::none;
      }

      std::string value = "";
      boost::optional<std::string> valueAsString = argument.valueAsString();
      if (valueAsString){
        value = *valueAsString;
      }
      searchTerms.push_back(std::make_pair(argument.name(), value));
    }

    std::vector<BCLComponent> components = componentAttributeSearch(searchTerms);

    if (!components.empty()){
      return components[0];
    }

    return boost::none;
  }

  std::vector<BCLComponent> LocalBCL::components() const
  {
    std::vector<BCLComponent> allComponents;
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    query.exec("SELECT uid, version_id FROM Components");
    while (query.next())
    {
      // DLM: this does not look like it is handling error of missing file correctly
      boost::optional<BCLComponent> current(toString(toPath(m_libraryPath) / toPath(query.value(0).toString()) / toPath(query.value(1).toString())));
      if (current)
      {
        allComponents.push_back(*current);
      }
    }
    return allComponents;
  }
  
  std::vector<BCLMeasure> LocalBCL::measures() const
  {
    std::vector<BCLMeasure> allMeasures;
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    query.exec("SELECT uid, version_id FROM Measures");
    while (query.next())
    {
      boost::optional<BCLMeasure> current = BCLMeasure::load(toPath(m_libraryPath) / toPath(query.value(0).toString()) / toPath(query.value(1).toString()));
      if (current)
      {
        allMeasures.push_back(*current);
      }
    }
    return allMeasures;
  }

  std::vector<std::string> LocalBCL::measureUids() const
  {
    std::vector<std::string> uids;
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    query.exec("SELECT DISTINCT uid FROM Measures");
    while (query.next())
    {
      uids.push_back(toString(query.value(0).toString()));
    }
    return uids;
  }

  std::vector<BCLComponent> LocalBCL::searchComponents(const std::string& searchTerm,
    const std::string& componentType) const 
  {
    std::vector<BCLComponent> results;
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    query.exec(toQString("SELECT uid, version_id FROM Components where name LIKE \"%"+searchTerm+"%\" OR description LIKE \"%"+searchTerm+"%\""));
    while (query.next())
    {
      // DLM: this does not look like it is handling error of missing file correctly
      boost::optional<BCLComponent> current(toString(toPath(m_libraryPath) / toPath(query.value(0).toString()) / toPath(query.value(1).toString())));
      if (current)
      {
        results.push_back(*current);
      }
    }
    return results;
  }

  std::vector<BCLComponent> LocalBCL::searchComponents(const std::string& searchTerm,
    const unsigned componentTypeTID) const 
  {
    return searchComponents(searchTerm, "");
  }

  std::vector<BCLMeasure> LocalBCL::searchMeasures(const std::string& searchTerm,
    const std::string& componentType) const 
  {
    std::vector<BCLMeasure> results;
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    query.exec(toQString("SELECT uid, version_id FROM Measures where name LIKE \"%"+searchTerm+"%\""
      "OR description LIKE \"%"+searchTerm+"%\" OR modeler_description LIKE \"%"+searchTerm+"%\""));
    while (query.next())
    {
      boost::optional<BCLMeasure> current = BCLMeasure::load(toPath(m_libraryPath) / toPath(query.value(0).toString()) / toPath(query.value(1).toString()));
      if (current)
      {
        results.push_back(*current);
      }
    }
    return results;
  }

  std::vector<BCLMeasure> LocalBCL::searchMeasures(const std::string& searchTerm,
    const unsigned componentTypeTID) const 
  {
    return searchMeasures(searchTerm, "");
  }

  /// Class members

  bool LocalBCL::addComponent(BCLComponent& component)
  {
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    //Check for uid
    if (!component.uid().empty() && !component.versionId().empty())
    {
      if (!query.exec(QString("DELETE FROM Components WHERE uid='%1' AND version_id='%2'").arg(
        escape(component.uid()), escape(component.versionId()))))
        return false;

      if (!query.exec(QString("INSERT INTO Components (uid, version_id, name, description, "
        "date_added, date_modified) VALUES('%1', '%2', '%3', '%4', %5, %6)").arg(
        escape(component.uid()), escape(component.versionId()), escape(component.name()),
        escape(component.description()), "datetime('now','localtime')", "datetime('now','localtime')")))
        return false;

      //Insert files
      if (!query.exec(QString("DELETE FROM Files WHERE uid='%1' AND version_id='%2'").arg(
          escape(component.uid()), escape(component.versionId()))))
          return false;
      if (!component.files().empty())
      {
        for (int i=0; i < (int) component.files().size(); i++)
        {
          if (!query.exec(QString("INSERT INTO Files (uid, version_id, filename, filetype) "
            "VALUES('%1', '%2', '%3', '%4')").arg(escape(component.uid()), escape(component.versionId()),
            escape(component.files()[i]), escape(component.filetypes()[i]))))
            return false;
        }
      }
      //Insert attributes
      if (!query.exec(QString("DELETE FROM Attributes WHERE uid='%1' AND version_id='%2'").arg(
          escape(component.uid()), escape(component.versionId()))))
          return false;
      if (!component.attributes().empty())
      {
        for (const Attribute& attribute : component.attributes())
        {
          std::string dataValue, dataType;
          if (attribute.valueType().value() == AttributeValueType::Boolean) {
            dataValue = boost::lexical_cast<std::string>(attribute.valueAsBoolean());
            dataType = "boolean";
          } else if (attribute.valueType().value() == AttributeValueType::Integer) {
            dataValue = boost::lexical_cast<std::string>(attribute.valueAsInteger());
            dataType = "int";
          } else if (attribute.valueType().value() == AttributeValueType::Double) {
            dataValue = formatString(attribute.valueAsDouble());
            dataType = "float";
          } else {
            dataValue = attribute.valueAsString();
            dataType = "string";
          }

          if (!query.exec(QString("INSERT INTO Attributes (uid, version_id, name, value, units, type) "
            "VALUES('%1', '%2', '%3', '%4', '%5', '%6')").arg(escape(component.uid()), escape(component.versionId()),
            escape(attribute.name()), escape(dataValue), escape(attribute.units() ? attribute.units().get() : ""),
            escape(dataType))))
            return false;
        }
      }
      return true;
    }

    return false;
  }

  bool LocalBCL::removeComponent(BCLComponent& component)
  {
    // if uid is empty or not found in database return false
    if (component.uid().empty() || component.versionId().empty()){
      return false;
    }

    // proceed deleting component
    openstudio::path pathToRemove = toPath(m_libraryPath) / toPath(component.uid()) / toPath(component.versionId());
    QDir dir(toQString(pathToRemove.parent_path()));
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    // Only one versionId, delete the uid directory
    if (dir.entryInfoList().size() == 1)
    {
      pathToRemove = pathToRemove.parent_path();
    }
    removeDirectory(pathToRemove);

    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    bool test = query.exec(QString("DELETE FROM Components WHERE uid='%1' AND version_id='%2'").arg(escape(component.uid()),
      escape(component.versionId())));
    OS_ASSERT(test);

    test = query.exec(QString("DELETE FROM Files WHERE uid='%1' AND version_id='%2'").arg(escape(component.uid()),
      escape(component.versionId())));
    OS_ASSERT(test);

    test = query.exec(QString("DELETE FROM Attributes WHERE uid='%1' AND version_id='%2'").arg(escape(component.uid()),
      escape(component.versionId())));
    OS_ASSERT(test);

    return true;
  }

  bool LocalBCL::addMeasure(BCLMeasure& measure)
  {
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    //Check for uid
    if (!measure.uid().empty() && !measure.versionId().empty())
    {
      if (!query.exec(QString("DELETE FROM Measures WHERE uid='%1' AND version_id='%2'").arg(
        escape(measure.uid()), escape(measure.versionId()))))
        return false;

      if (!query.exec(QString("INSERT INTO Measures (uid, version_id, name, description, modeler_description, "
        "date_added, date_modified) VALUES('%1', '%2', '%3', '%4', '%5', %6, %7)").arg(
        escape(measure.uid()), escape(measure.versionId()), escape(measure.name()), escape(measure.description()),
        escape(measure.modelerDescription()), "datetime('now','localtime')", "datetime('now','localtime')")))
        return false;

      //Insert files
      if (!query.exec(QString("DELETE FROM Files WHERE uid='%1' AND version_id='%2'").arg(
          escape(measure.uid()), escape(measure.versionId()))))
          return false;
      if (!measure.files().empty())
      {
        for (const BCLFileReference& file : measure.files())
        {
          if (!query.exec(QString("INSERT INTO Files (uid, version_id, filename, filetype, usage_type, checksum) "
            "VALUES('%1', '%2', '%3', '%4', '%5', '%6')").arg(escape(measure.uid()), escape(measure.versionId()),
            escape(file.fileName()), escape(file.fileType()), escape(file.usageType()), escape(file.checksum()))))
            return false;
        }
      }

      //Insert attributes
      if (!query.exec(QString("DELETE FROM Attributes WHERE uid='%1' AND version_id='%2'").arg(
          escape(measure.uid()), escape(measure.versionId()))))
          return false;
      if (!measure.attributes().empty())
      {
        for (const Attribute& attribute : measure.attributes())
        {
          std::string dataValue, dataType;
          if (attribute.valueType().value() == AttributeValueType::Boolean) {
            dataValue = boost::lexical_cast<std::string>(attribute.valueAsBoolean());
            dataType = "boolean";
          } else if (attribute.valueType().value() == AttributeValueType::Integer) {
            dataValue = boost::lexical_cast<std::string>(attribute.valueAsInteger());
            dataType = "int";
          } else if (attribute.valueType().value() == AttributeValueType::Double) {
            dataValue = formatString(attribute.valueAsDouble());
            dataType = "float";
          } else {
            dataValue = attribute.valueAsString();
            dataType = "string";
          }

          if (!query.exec(QString("INSERT INTO Attributes (uid, version_id, name, value, units, type) "
            "VALUES('%1', '%2', '%3', '%4', '%5', '%6')").arg(escape(measure.uid()), escape(measure.versionId()),
            escape(attribute.name()), escape(dataValue), escape(attribute.units() ? attribute.units().get() : ""),
            escape(dataType))))
            return false;
        }
      }
      return true;
    }
    return false;
  }

  bool LocalBCL::removeMeasure(BCLMeasure& measure)
  {
    // if uid is empty or not found in database return false
    if (measure.uid().empty() || measure.versionId().empty()){
      return false;
    }

    // proceed deleting measure
    openstudio::path pathToRemove = toPath(m_libraryPath) / toPath(measure.uid()) / toPath(measure.versionId());
    QDir dir(toQString(pathToRemove.parent_path()));
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    // Only one versionId, delete the uid directory
    if (dir.entryInfoList().size() == 1)
    {
      pathToRemove = pathToRemove.parent_path();
    }
    removeDirectory(pathToRemove);

    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    bool test = query.exec(QString("DELETE FROM Measures WHERE uid='%1' AND version_id='%2'").arg(escape(measure.uid()),
      escape(measure.versionId())));
    OS_ASSERT(test);

    test = query.exec(QString("DELETE FROM Files WHERE uid='%1' AND version_id='%2'").arg(escape(measure.uid()),
      escape(measure.versionId())));
    OS_ASSERT(test);

    test = query.exec(QString("DELETE FROM Attributes WHERE uid='%1' AND version_id='%2'").arg(escape(measure.uid()),
      escape(measure.versionId())));
    OS_ASSERT(test);

    return true;
  }

  std::vector<BCLComponent> LocalBCL::componentAttributeSearch(const std::vector<std::pair<std::string, std::string> >& searchTerms) const
  {
    auto uids = attributeSearch(searchTerms, "component");
    if (uids.empty()){
       return std::vector<BCLComponent>();
    }

    std::vector<BCLComponent> result;
    for (const auto & uid : uids)
    {
      boost::optional<BCLComponent> component = getComponent(uid.first, uid.second);
      if (component){
        result.push_back(*component);
      }
    }

    return result;
  }

  std::vector<BCLMeasure> LocalBCL::measureAttributeSearch(const std::vector<std::pair<std::string, std::string> >& searchTerms) const
  {
    auto uids = this->attributeSearch(searchTerms, "measure");
    if (uids.empty()){
       return std::vector<BCLMeasure>();
    }

    std::vector<BCLMeasure> result;
    for (const auto uid : uids)
    {
      boost::optional<BCLMeasure> measure = getMeasure(uid.first, uid.second);
      if (measure){
        result.push_back(*measure);
      }
    }

    return result;
  }


  std::set<std::pair<std::string, std::string> > LocalBCL::attributeSearch(
      const std::vector<std::pair<std::string, std::string> >& searchTerms,
      const std::string componentType) const
  {
    typedef std::vector<std::pair<std::string, std::string> > UidsVecType;
    typedef std::set<std::pair<std::string, std::string> > UidsType;

    UidsType uids;
    QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
    QSqlQuery query(database);
    std::string tableName = componentType == "component" ? "Components" : componentType == "measure" ? "Measures" : "";
    query.exec(toQString("SELECT DISTINCT uid, version_id FROM " + tableName));
    while (query.next()) {
      uids.insert(make_pair(toString(query.value(0).toString()), toString(query.value(1).toString())));
    }
    
    for (const auto & searchTerm : searchTerms){

      UidsType theseUids;
      QString queryString = QString("SELECT uid, version_id FROM Attributes WHERE name='" + escape(searchTerm.first) + "' COLLATE NOCASE AND value='" + escape(searchTerm.second) + "' COLLATE NOCASE");
      query.exec(queryString);
      while (query.next()) {
        theseUids.insert(make_pair(toString(query.value(0).toString()), toString(query.value(1).toString())));
      }

      UidsVecType newUids(std::max(uids.size(), theseUids.size()));
      auto insertEnd = std::set_intersection(uids.begin(), uids.end(), theseUids.begin(), theseUids.end(), newUids.begin());
      
      uids.clear();
      uids.insert(newUids.begin(), insertEnd);

      if (uids.empty()){
        return UidsType();
      }
    }

    return uids;
  }

  std::string LocalBCL::formatString(double d, uint prec)
  {
    std::stringstream ss;
    ss << std::setprecision(prec) << std::showpoint << d;
    std::string s = ss.str();

    // truncate zeros from the end
    int i = s.size() - 1;
    while (i > 0 && s[i] == '0')
    {
      --i;
    }
    if (i > 0)
    {
      s.erase(i + 1);
      if (*s.rbegin() == '.')
      {
        s.push_back('0');
      }
    }
    return s;
  }


  bool LocalBCL::prodAuthKeyUserPrompt(QWidget* parent)
  {
    // make sure application is initialized
    Application::instance().application(true);

    QInputDialog inputDlg(parent);
    inputDlg.setInputMode(QInputDialog::TextInput);
    inputDlg.setLabelText("BCL Auth Key:                                            ");
    inputDlg.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    inputDlg.setWindowTitle("Enter Your BCL Auth Key");
    if (Application::instance().isDefaultInstance())
    {
      QIcon icon = QIcon(":/images/os_16.png");
      icon.addPixmap(QPixmap(":/images/os_32.png"));
      icon.addPixmap(QPixmap(":/images/os_48.png"));
      icon.addPixmap(QPixmap(":/images/os_64.png"));
      icon.addPixmap(QPixmap(":/images/os_128.png"));
      icon.addPixmap(QPixmap(":/images/os_256.png"));
      inputDlg.setWindowIcon(icon);
    }
    bool result = inputDlg.exec();
    QString text = inputDlg.textValue();

    if (result && !text.isEmpty()){
      std::string authKey = toString(text);
      result = setProdAuthKey(authKey);
    }

    return result;
  }

  bool LocalBCL::devAuthKeyUserPrompt(QWidget* parent)
  {
    // make sure application is initialized
    openstudio::Application::instance().application(true);

    QInputDialog inputDlg(parent);
    inputDlg.setInputMode(QInputDialog::TextInput);
    inputDlg.setLabelText("BCL Dev Auth Key:                                            ");
    inputDlg.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    inputDlg.setWindowTitle("Enter Your BCL Dev Auth Key");
    if (Application::instance().isDefaultInstance())
    {
      QIcon icon = QIcon(":/images/os_16.png");
      icon.addPixmap(QPixmap(":/images/os_32.png"));
      icon.addPixmap(QPixmap(":/images/os_48.png"));
      icon.addPixmap(QPixmap(":/images/os_64.png"));
      icon.addPixmap(QPixmap(":/images/os_128.png"));
      icon.addPixmap(QPixmap(":/images/os_256.png"));
      inputDlg.setWindowIcon(icon);
    }
    bool result = inputDlg.exec();
    QString text = inputDlg.textValue();

    if (result && !text.isEmpty()){
      std::string devAuthKey = toString(text);
      result = setDevAuthKey(devAuthKey);
    }

    return result;
  }

  std::string LocalBCL::prodAuthKey() const
  {
    return m_prodAuthKey;
  }

  bool LocalBCL::setProdAuthKey(const std::string& authKey)
  {
    RemoteBCL remoteBCL;
    if (remoteBCL.setProdAuthKey(authKey))
    {
      m_prodAuthKey = authKey;
      //Overwrite prodAuthKey in database
      QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
      QSqlQuery query(database);
      return query.exec(QString("UPDATE Settings SET data='%1' WHERE name='prodAuthKey'").arg(escape(authKey)));
    }
    return false;
  }

  std::string LocalBCL::devAuthKey() const
  {
    return m_devAuthKey;
  }

  bool LocalBCL::setDevAuthKey(const std::string& authKey)
  {
    RemoteBCL remoteBCL;
    if (remoteBCL.setDevAuthKey(authKey))
    {
      m_devAuthKey = authKey;
      //Overwrite devAuthKey in database
      QSqlDatabase database = QSqlDatabase::database(m_libraryPath+m_dbName);
      QSqlQuery query(database);
      return query.exec(QString("UPDATE Settings SET data='%1' WHERE name='devAuthKey'").arg(escape(authKey)));
    }
    return false;
  }

  QString LocalBCL::libraryPath() const
  {
    return m_libraryPath;
  }

  bool LocalBCL::setLibraryPath(const std::string& libraryPath)
  {
    //cleanup old straggling one if it exists
    QSqlDatabase::removeDatabase(m_libraryPath+m_dbName);

    QString path = QDir().cleanPath(toQString(libraryPath));
    if (!QDir(path).exists())
    {
      bool success = QDir().mkpath(path);
      if (!success) return false;
    }
    m_libraryPath = path;

    if (!QFile(path+m_dbName).exists())
    {
      QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", path+m_dbName);
      database.setDatabaseName(path+m_dbName);

      bool success = initializeLocalDb();
      if (!success) return false;
    }

    QSettings settings("OpenStudio", "LocalBCL");
    settings.setValue("libraryPath", path);

    return true;
  }


} // openstudio
