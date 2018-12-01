/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "LocalBCL.hpp"
#include "RemoteBCL.hpp"
#include "../core/Application.hpp"
#include "../core/Assert.hpp"
#include "../time/DateTime.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/FilesystemHelpers.hpp"

#include <sqlite/sqlite3.h>

#include <boost/algorithm/string/replace.hpp>

#include <QIcon>
#include <QInputDialog>
#include <QSettings>

namespace openstudio{

  LocalBCL::LocalBCL(const path& libraryPath):
    m_libraryPath(libraryPath.lexically_normal()),
    m_dbName("components.sql"),
    m_dbVersion("1.3"),
    m_connectionOpen(false)
  {
    //Make sure a QApplication exists
    openstudio::Application::instance().application(false);

    //Check for BCL directory
    if (!openstudio::filesystem::is_directory(m_libraryPath) || !openstudio::filesystem::exists(m_libraryPath)) {
      openstudio::filesystem::create_directory(m_libraryPath);
    }
    // Now we know the library path exists for sure (needed for canonical call),
    // we convert to an absolute path with no symlink, or dot, or dot-dot elements
    m_libraryPath = openstudio::filesystem::canonical(m_libraryPath);

    m_sqliteFilePath = m_libraryPath / m_dbName;
    m_sqliteFilename = toString(m_sqliteFilePath.make_preferred().native());

    //Check for local database
    bool initschema = !openstudio::filesystem::exists(m_sqliteFilePath);

    // Open the database
    int success = sqlite3_open_v2(m_sqliteFilename.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE, nullptr);
    if (success != SQLITE_OK) {
      LOG_AND_THROW("Unable to open connection for LocalBCL at '" << m_sqliteFilename << "'.");
    } else {
      m_connectionOpen = true;
    }

    // If need to create it, and it didn't go well, throw
    if (initschema && !initializeLocalDb()) {
      LOG_AND_THROW("Unable to initialize Local Database");
    }

    //Check for out-of-date database
    if (!updateLocalDb()) {
      LOG_AND_THROW("Unable to update Local Database");
    }

    // Retrieve oauthConsumerKeys from database
    {
      std::string statement = "SELECT data FROM Settings WHERE name='prodAuthKey'";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG_AND_THROW("Unable to prepare m_prodAuthKey Statement");
      }
      if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {
        m_prodAuthKey = columnText(sqlite3_column_text(sqlStmtPtr, 0));
      }
      // must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);
    }

    {
      std::string statement = "SELECT data FROM Settings WHERE name='devAuthKey'";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG_AND_THROW("Unable to prepare m_prodAuthKey Statement");
      }
      if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {
        m_devAuthKey = columnText(sqlite3_column_text(sqlStmtPtr, 0));
      }
      // must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);
    }

  }

  openstudio::filesystem::path LocalBCL::dbPath() const
  {
    return m_libraryPath / m_dbName;
  }

  std::string LocalBCL::columnText(const unsigned char* column) const {
    return std::string(reinterpret_cast<const char*>(column));
  }

  // http://sqlite.org/faq.html#q14
  std::string LocalBCL::escape(const std::string& s) const
  {
    return boost::replace_all_copy(s, "'", "''");
  }

  LocalBCL &LocalBCL::instance()
  {
    std::shared_ptr<LocalBCL> &ptr = instanceInternal();
    if (!ptr) {
      QSettings settings("OpenStudio", "LocalBCL");
      // DLM: might want to put this somewhere a little more hidden
      ptr = std::shared_ptr<LocalBCL>(new LocalBCL(
          toPath(settings.value("libraryPath", toQString(openstudio::filesystem::home_path() / toPath("BCL"))).toString())));
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
      openstudio::path canLibraryPath(libraryPath);
      // If libraryPath is an existing dir, take the canonical path, so that it matches what we stored in the Ctor
      if (openstudio::filesystem::is_directory(canLibraryPath) && openstudio::filesystem::exists(canLibraryPath)) {
        canLibraryPath = openstudio::filesystem::canonical(canLibraryPath);
      }
      if (ptr->libraryPath() != canLibraryPath) {
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
    // Close the connection to the database if needed
    if (m_connectionOpen) {
      sqlite3_close(m_db);
      m_connectionOpen = false;
    }

  }

  bool LocalBCL::initializeLocalDb()
  {
    std::string create_statements(
        "CREATE TABLE Settings (name VARCHAR, data VARCHAR);"
        "CREATE TABLE Components (uid VARCHAR, version_id VARCHAR, name VARCHAR, description VARCHAR, date_added DATETIME, date_modified DATETIME);"
        "CREATE TABLE Files (uid VARCHAR, version_id VARCHAR, filename VARCHAR, filetype VARCHAR, usage_type VARCHAR, checksum VARCHAR);"
        "CREATE TABLE Attributes (uid VARCHAR, version_id VARCHAR, name VARCHAR, value VARCHAR, units VARCHAR, type VARCHAR);"
        "CREATE TABLE Measures (uid VARCHAR, version_id VARCHAR, name VARCHAR, description VARCHAR, modeler_description VARCHAR, date_added DATETIME, date_modified DATETIME);");

    char * err = nullptr;
    if (sqlite3_exec(m_db, create_statements.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
      std::string errstr;

      if (err) {
        errstr = err;
        sqlite3_free(err);
      }

      LOG(Error, "Erorr in initializeLocalDb when creating tables: " << errstr);
      return false;
    }

    sqlite3_stmt* sqlStmtPtr = nullptr;
    // 1=name, 2=value
    std::string insert_statement = "INSERT INTO Settings VALUES (?, ?)";
    if (sqlite3_prepare_v2(m_db, insert_statement.c_str(), insert_statement.size(), &sqlStmtPtr, nullptr) != SQLITE_OK) {
      LOG(Error, "Error preparing insert statement");
      return false;
    }

    std::vector<std::pair<std::string, std::string>> vals = {
      {"dbVersion", m_dbVersion},
      {"prodAuthKey", ""},
      {"devAuthKey", ""},
    };

    bool errorsFound = false;
    for (const auto& p: vals) {
      if (sqlite3_bind_text(sqlStmtPtr, 1, p.first.c_str(), p.first.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
        LOG(Error, "Error binding to the 1st parameter (initializeLocalDb): " << p.first);
        errorsFound = true;
      }
      else if (sqlite3_bind_text(sqlStmtPtr, 2, p.second.c_str(), p.second.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
        LOG(Error, "Error binding to the 2nd parameter (initializeLocalDb): " << p.second);
        errorsFound = true;
      }
      else if (sqlite3_step(sqlStmtPtr) != SQLITE_DONE) {
        LOG(Error, "Error executing prepared statement");
        errorsFound = true;
      }

      if (errorsFound) {
        sqlite3_reset(sqlStmtPtr);
        return false;
      }

      // Reset the statement to allow binding variables on the next iteration
     sqlite3_reset(sqlStmtPtr);

    }

    sqlite3_finalize(sqlStmtPtr);

    return true;
  }

  bool LocalBCL::updateLocalDb() {

    std::string localDbVersion;

    {
      // Get current version from the sqlite file in question
      std::string statement = "SELECT data FROM Settings WHERE name='dbVersion'";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare dbVersion Statement");
        return false;
      }
      // If this doesn't return, it may be normal (old version), so don't return / log an error
      if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {
        localDbVersion = columnText(sqlite3_column_text(sqlStmtPtr, 0));
        // If already latest version, do nothing
        if (localDbVersion == m_dbVersion) {
          return true;
        }
      }
      sqlite3_finalize(sqlStmtPtr);
    }

    // 1.0, 1.1 -> 1.2
    {
      // Get current version from the sqlite file in question
      std::string statement = "SELECT oauthConsumerKey, dbVersion FROM Settings";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare dbVersion 1.0/1.1 Statement");
        return false;
      }
      // If this doesn't return, it may be normal (not the right version), so don't return / log an error
      // Otherwise, we expect one row, with the two values we care about about
      if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {

        std::string oauthConsumerKey = columnText(sqlite3_column_text(sqlStmtPtr, 0));
        std::string localDbVersion = columnText(sqlite3_column_text(sqlStmtPtr, 1));
        sqlite3_finalize(sqlStmtPtr);


        // If already latest version, do nothing
        if (localDbVersion == "1.0" || localDbVersion == "1.1") {

          // Start a transaction, so we can handle failures without messing up the database
          sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);


          std::string update_statements(
              "ALTER TABLE Components ADD date_added DATETIME;"
              "ALTER TABLE Components ADD date_modified DATETIME;"
              "DROP TABLE Settings;"
              "CREATE TABLE Settings (name VARCHAR, data VARCHAR);");


          char * err = nullptr;
          if (sqlite3_exec(m_db, update_statements.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            std::string errstr;

            if (err) {
              errstr = err;
              sqlite3_free(err);
            }

            LOG(Error, "Error in UpdateLocalDb when altering tables from 1.0/1.1 to " << m_dbVersion << ": " << errstr);
            // Rollback changes
            sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);

            return false;
          }


          // Insertion block
          {
            sqlite3_stmt* sqlStmtPtr = nullptr;
            // 1=name, 2=value
            std::string insert_statement = "INSERT INTO Settings VALUES (?, ?)";
            if (sqlite3_prepare_v2(m_db, insert_statement.c_str(), insert_statement.size(), &sqlStmtPtr, nullptr) != SQLITE_OK) {
              LOG(Error, "Error preparing insert statement");
              return false;
            }

            std::vector<std::pair<std::string, std::string>> vals = {
              {"dbVersion", m_dbVersion},
              // old oauthConsumerKey becomes prodAuthKey
              {"prodAuthKey", oauthConsumerKey},
              {"devAuthKey", ""},
            };

            bool errorsFound = false;
            for (const auto& p: vals) {
              if (sqlite3_bind_text(sqlStmtPtr, 1, p.first.c_str(), p.first.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
                LOG(Error, "Error binding to the 1st parameter: " << p.first);
                errorsFound = true;
              }
              else if (sqlite3_bind_text(sqlStmtPtr, 2, p.second.c_str(), p.second.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
                LOG(Error, "Error binding to the 2nd parameter: " << p.second);
                errorsFound = true;
              }
              else if (sqlite3_step(sqlStmtPtr) != SQLITE_DONE) {
                LOG(Error, "Error executing prepared statement");
                errorsFound = true;
              }

              if (errorsFound) {
                sqlite3_reset(sqlStmtPtr);
                // Rollback changes
                sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
                return false;
              }

              // Reset the statement to allow binding variables on the next iteration
              sqlite3_reset(sqlStmtPtr);

            }

            sqlite3_finalize(sqlStmtPtr);
          }

          // Commit changes now that everything went well
          sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
          return true;
        }
      }
    } // End version 1.0/1.1


    // 1.2 -> 1.3
    if (localDbVersion == "1.2") {

      // Start a transaction, so we can handle failures without messing up the database
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);


      // First update
      {
        std::string update_statements(
            "ALTER TABLE Attributes ADD version_id VARCHAR;"
            "ALTER TABLE Files ADD version_id VARCHAR;"
            "UPDATE Attributes SET type = 'string' WHERE type = 'varchar';"
            "ALTER TABLE Files ADD usage_type VARCHAR;"
            "ALTER TABLE Files ADD checksum VARCHAR;"
            "CREATE TABLE Measures (uid VARCHAR, version_id VARCHAR, name VARCHAR, description VARCHAR, modeler_description VARCHAR, date_added DATETIME, date_modified DATETIME);"
            "UPDATE Settings SET data = " + m_dbVersion + " WHERE name = 'dbVersion';"
            );


        char * err = nullptr;
        if (sqlite3_exec(m_db, update_statements.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
          std::string errstr;

          if (err) {
            errstr = err;
            sqlite3_free(err);
          }

          LOG(Error, "Error in UpdateLocalDb when altering tables from 1.2 to " << m_dbVersion << ": " << errstr);
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);

          return false;
        }
      }

      // Now, Loop on the components table to update version_ID
      {

        // Update statement
        std::string update_attribute_record = "UPDATE Attributes SET version_ID=? WHERE uid=?";
        sqlite3_stmt * updateAtrStmtPtr;
        if (sqlite3_prepare_v2(m_db, update_attribute_record.c_str(), update_attribute_record.size(), &updateAtrStmtPtr, nullptr) != SQLITE_OK) {
          LOG(Error, "Error preparing update_attribute_record statement");
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          sqlite3_reset(updateAtrStmtPtr);
          return false;
        }

        std::string update_file_record = "UPDATE Files SET version_ID=? WHERE uid=?";
        sqlite3_stmt * updateFileStmtPtr;
        if (sqlite3_prepare_v2(m_db, update_file_record.c_str(), update_file_record.size(), &updateFileStmtPtr, nullptr) != SQLITE_OK) {
          LOG(Error, "Error preparing update_file_record statement");
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          sqlite3_reset(updateAtrStmtPtr);
          sqlite3_reset(updateFileStmtPtr);
          return false;
        }

        // Get current version from the sqlite file in question
        std::string statement = "SELECT uid, version_id FROM Components WHERE uid IN (SELECT DISTINCT uid FROM Components)";
        sqlite3_stmt* sqlStmtPtr;
        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

        bool errorsFound = false;

        // Loop until done (or failed)
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            // Get values from SELECT
            std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
            std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));


            // Update record
            if (sqlite3_bind_text(updateAtrStmtPtr, 1, uid.c_str(), uid.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
              LOG(Error, "updateAtrStmtPtr: Error binding to the 1st parameter uid=" << uid);
              errorsFound = true;
            }
            else if (sqlite3_bind_text(updateAtrStmtPtr, 2, version_id.c_str(), version_id.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
              LOG(Error, "updateAtrStmtPtr: Error binding to the 2nd parameter version_id=" << version_id);
              errorsFound = true;
            } else if (sqlite3_step(updateAtrStmtPtr) != SQLITE_DONE) {
              LOG(Error, "Error executing prepared updateAtrStmtPtr");
              errorsFound = true;
            } else {
              // Reset the statement to allow binding variables on the next iteration
              sqlite3_reset(updateAtrStmtPtr);
            }

            // Update record
            if (sqlite3_bind_text(updateFileStmtPtr, 1, uid.c_str(), uid.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
              LOG(Error, "updateFileStmtPtr: Error binding to the 1st parameter uid=" << uid);
              errorsFound = true;
            }
            else if (sqlite3_bind_text(updateFileStmtPtr, 2, version_id.c_str(), version_id.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
              LOG(Error, "updateFileStmtPtr: Error binding to the 2nd parameter version_id=" << version_id);
              errorsFound = true;
            } else if (sqlite3_step(updateFileStmtPtr) != SQLITE_DONE) {
              LOG(Error, "Error executing prepared updateFileStmtPtr");
              errorsFound = true;
            } else {
              // Reset the statement to allow binding variables on the next iteration
              sqlite3_reset(updateFileStmtPtr);
            }

          }
          else  // i didn't get a row.  something is wrong so set the exit condition.
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;
          }
        } // End loop on each uid, version_id


        // Finalize all statements whether it suceeded or not
        sqlite3_finalize(sqlStmtPtr);
        sqlite3_finalize(updateAtrStmtPtr);
        sqlite3_finalize(updateFileStmtPtr);


        if (errorsFound || code != SQLITE_DONE) {
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          return false;
        }
      } // End of update of the version_id

      // Change paths
      {
        // Get current version from the sqlite file in question
        std::string statement = "SELECT uid, version_id, directory FROM Components";
        sqlite3_stmt* sqlStmtPtr;
        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

        // Loop until done (or failed)
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            // Get values from SELECT
            std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
            std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));
            std::string directory = columnText(sqlite3_column_text(sqlStmtPtr, 2));

            // TODO: replace with remove_trailing_separator once update_depends is merged in
            // openstudio::path p_directory(directory);
            // openstudio::path src = m_libraryPath / uid / version_id / p_directory.remove_trailing_separator().filename();

            // remove_trailing_separator works well in 1.68, but isn't there in 1.55
            const size_t len = directory.size();
            if (len > 1 && ( directory[len-1] == '\\' || directory[len-1] == '/') ) {
              directory[len-1] = 0;
            }

            openstudio::path p_directory(directory);
            openstudio::path src = m_libraryPath / uid / version_id / p_directory.filename();
            openstudio::path dest = src.parent_path();

            openstudio::filesystem::remove(dest / toPath("DISCLAIMER.txt"));
            openstudio::filesystem::remove(dest / toPath("README.txt"));
            openstudio::filesystem::remove(dest / toPath("output.xml"));
            copyDirectory(src, dest);
            removeDirectory(src);

          }
          else  // i didn't get a row.  something is wrong so set the exit condition.
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;
          }
        } // End loop on each uid, version_id, directory

        // Finalize statement whether it suceeded or not
        sqlite3_finalize(sqlStmtPtr);

        if (code != SQLITE_DONE) {
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          return false;
        }

      } // End update paths


      // Final update
      {

        std::string update_statements(
            "ALTER TABLE Components RENAME TO ComponentsTmp;"
            "CREATE TABLE Components(uid VARCHAR, version_id VARCHAR, name VARCHAR, description VARCHAR, date_added DATETIME, date_modified DATETIME);"
            "INSERT INTO Components SELECT uid, version_id, name, type, date_added, date_modified FROM ComponentsTmp;"
            "DROP TABLE ComponentsTmp;"
        );


        char * err = nullptr;
        if (sqlite3_exec(m_db, update_statements.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
          std::string errstr;

          if (err) {
            errstr = err;
            sqlite3_free(err);
          }

          LOG(Error, "Error in UpdateLocalDb when altering final tables from 1.2 to " << m_dbVersion << ": " << errstr);
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);

          return false;
        }

      }


      // Commit changes now that everything went well
      sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
      return true;

    } // End 1.2 -> 1.3

    return false;
  }

  /// Inherited members

  boost::optional<BCLComponent> LocalBCL::getComponent(const std::string& uid, const std::string& versionId) const {
    boost::optional<BCLComponent> result;

    if (m_db) {

      if (versionId.empty()) {

        std::string statement = "SELECT version_id FROM Components WHERE uid='" + escape(uid) +"'";
        sqlite3_stmt* sqlStmtPtr;
        if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
          LOG(Error, "Unable to prepare version_id Statement");
        }

        if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          result = boost::optional<BCLComponent>(m_libraryPath / uid / version_id);
        }
        // Finalize statement to prevent memory leak
        sqlite3_finalize(sqlStmtPtr);
      } else {
        std::string statement = "SELECT version_id FROM Components WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
        sqlite3_stmt* sqlStmtPtr;
        if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
          LOG(Error, "Unable to prepare version_id Statement");
        }

        if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          result = boost::optional<BCLComponent>(m_libraryPath / uid / version_id);
        }
        // Finalize statement to prevent memory leak
        sqlite3_finalize(sqlStmtPtr);
      }

    }

    return result;

  }

  boost::optional<BCLMeasure> LocalBCL::getMeasure(const std::string& uid, const std::string& versionId) const
  {
    boost::optional<BCLMeasure> result;

     if (m_db) {

      if (versionId.empty()) {

        std::string statement = "SELECT version_id FROM Measures WHERE uid='" + escape(uid) +"'";
        sqlite3_stmt* sqlStmtPtr;
        if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
          LOG(Error, "Unable to prepare version_id Statement");
        }

        // We seek the most recent modified one in case we find several, so store that
        boost::optional<DateTime> mostRecentModified;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

        // Loop until done (or failed)
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            // Get values from SELECT
            std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 0));

            try{
              BCLMeasure measure(m_libraryPath / uid / version_id);

              boost::optional<DateTime> versionModified = measure.versionModified();
              if (mostRecentModified){
                if (versionModified && versionModified.get() > mostRecentModified.get()){
                  mostRecentModified = versionModified;
                  result = measure;
                }
              } else {
                mostRecentModified = versionModified;
                result = measure;
              }
            }catch(const std::exception&){
            }

          }
          else  // i didn't get a row.  something is wrong so set the exit condition.
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;
          }
        } // End loop on each match

        // Finalize statement whether it suceeded or not
        sqlite3_finalize(sqlStmtPtr);

      } else {

        std::string statement = "SELECT version_id FROM Measures WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
        sqlite3_stmt* sqlStmtPtr;
        if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
          LOG(Error, "Unable to prepare version_id Statement");
        }

        if (sqlite3_step(sqlStmtPtr) == SQLITE_ROW) {
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          try{
            result = boost::optional<BCLMeasure>(m_libraryPath / uid / version_id);
          } catch(const std::exception&) {
          }
        }
        // Finalize statement to prevent memory leak
        sqlite3_finalize(sqlStmtPtr);
      }

    }

    return result;
  }

  std::vector<BCLComponent> LocalBCL::components() const
  {
    std::vector<BCLComponent> allComponents;
    if (m_db) {
      std::string statement = "SELECT uid, version_id FROM Components";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare components Statement");
      }

      int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

      // Loop until done (or failed)
      while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          // Get values from SELECT
          std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));

          boost::optional<BCLComponent> current(m_libraryPath / uid / version_id);
          if (current) {
            allComponents.push_back(current.get());
          }

        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }
      } // End loop on each match

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
    }
    return allComponents;
  }

  std::vector<BCLMeasure> LocalBCL::measures() const
  {
    std::vector<BCLMeasure> allMeasures;

    if (m_db) {
      std::string statement = "SELECT uid, version_id FROM Measures";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare measures Statement");
      }

      int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

      // Loop until done (or failed)
      while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          // Get values from SELECT
          std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));

          boost::optional<BCLMeasure> current = BCLMeasure::load(m_libraryPath / uid / version_id);
          if (current) {
            allMeasures.push_back(current.get());
          }

        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }
      } // End loop on each match

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
    }

    return allMeasures;
  }

  std::vector<std::string> LocalBCL::measureUids() const
  {
    std::vector<std::string> uids;

    if (m_db) {
      std::string statement = "SELECT DISTINCT uid FROM Measures";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare measureUids Statement");
      }

      int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

      // Loop until done (or failed)
      while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
            // Get values from SELECT
            uids.push_back(columnText(sqlite3_column_text(sqlStmtPtr, 0)));
        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }
      } // End loop on each match

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
    }

    return uids;
  }

  std::vector<BCLComponent> LocalBCL::searchComponents(const std::string& searchTerm,
    const std::string& componentType) const
  {
    std::vector<BCLComponent> results;

    if (m_db) {
      std::string statement("SELECT uid, version_id FROM Components where name"
                            "  LIKE \"%"+searchTerm+"%\" OR description LIKE \"%"+searchTerm+"%\"");
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare searchComponents Statement");
      }

      int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

      // Loop until done (or failed)
      while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          // Get values from SELECT
          std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));

          boost::optional<BCLComponent> current(m_libraryPath / uid / version_id);
          if (current) {
            results.push_back(current.get());
          }
        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }
      } // End loop on each match

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
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

    if (m_db) {
      std::string statement("SELECT uid, version_id FROM Measures where name LIKE \"%"+searchTerm+"%\""
                            "  OR description LIKE \"%"+searchTerm+"%\" OR modeler_description LIKE \"%"+searchTerm+"%\"");
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Unable to prepare searchMeasures Statement");
      }

      int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

      // Loop until done (or failed)
      while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          // Get values from SELECT
          std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));

          boost::optional<BCLMeasure> current = BCLMeasure::load(m_libraryPath / uid / version_id);
          if (current) {
            results.push_back(current.get());
          }
        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }
      } // End loop on each match

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
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
    //Check for uid
    if (m_db && !component.uid().empty() && !component.versionId().empty() ) {

      // Start a transaction, so we can handle failures without messing up the database
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);

      std::string uid = component.uid();
      std::string versionId = component.versionId();


      std::string statement = "DELETE FROM Components WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      std::stringstream ss;
      ss << "INSERT INTO Components (uid, version_id, name, description, date_added, date_modified) "
        << "VALUES('" << escape(uid) << "', '" << escape(versionId) << "', '" << escape(component.name())
        << "', '" << escape(component.description())<< "', datetime('now','localtime'), datetime('now','localtime'));";

      statement = ss.str();

      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      // Insert files
      statement = "DELETE FROM Files WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      if (!component.files().empty()) {
        for (int i=0; i < (int) component.files().size(); i++)
        {

          std::stringstream ss;
          ss << "INSERT INTO Files (uid, version_id, filename, filetype) "
            << "VALUES('" << escape(uid) << "', '" << escape(versionId) << "', '" << escape(component.files()[i])
            << "', '" << escape(component.filetypes()[i])<< "');";

          statement = ss.str();

          if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
            // Rollback changes
            sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
            return false;
          }
        }
      } // End insert all component's files


      // Insert Attributes
      statement = "DELETE FROM Attributes WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      for (const Attribute& attribute : component.attributes()) {
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

        std::stringstream ss;
        ss << "INSERT INTO Attributes (uid, version_id, name, value, units, type) "
          << "VALUES('" << escape(uid) << "', '" << escape(versionId) << "', '" << escape(attribute.name())
          << "', '" << escape(dataValue) << "', '" << escape(attribute.units() ? attribute.units().get() : "")
          << "', '" << escape(dataType)  << "');";

        statement = ss.str();

        if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          return false;
        }
      } // End insert each attribute

      // Commit changes now that everything went well
      sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
      return true;

    } // End check uid

    return false;
  }

  bool LocalBCL::removeComponent(BCLComponent& component)
  {
    // if uid is empty or not found in database return false
    if (!m_db || component.uid().empty() || component.versionId().empty()){
      return false;
    }

    // Start by deleting stuff from the SQL file
    // We don't expect any of these to fail really, if it isn't present, then it still returns OK
    // But let's be prudent and still start a transaction

    // Start a transaction, so we can handle failures without messing up the database
    sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);

    std::string uid = component.uid();
    std::string versionId = component.versionId();

    std::string statement(
        "DELETE FROM Components WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"';"
        "DELETE FROM Files WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"';"
        "DELETE FROM Attributes WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"';"
        );
    if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
      LOG(Error, "Couldn't delete component from SQL file");
      // Rollback changes
      sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
      return false;
    }

    // Commit changes now that everything went well
    sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);

    // proceed deleting component on disk
    openstudio::path pathToRemove = m_libraryPath / uid / versionId;

    // We look at the parent path, and if we find only one folder = (one versionId), we delete the parent instead
    // so we don't end up with an empty folder
    unsigned int n_dirs = 0;
    for (auto dir_itr = openstudio::filesystem::directory_iterator(pathToRemove.parent_path());
        dir_itr != openstudio::filesystem::directory_iterator();
        ++dir_itr)
    {
      const auto p = dir_itr->path();
      if (openstudio::filesystem::is_directory(p) && openstudio::filesystem::exists(p)) {
        ++n_dirs;
      }
    }

    // Lambda version, doesn't really help clarity here
    /*
     *int n_dirs = std::count_if(openstudio::filesystem::directory_iterator(pathToRemove.parent_path()),
     *                           openstudio::filesystem::directory_iterator(),
     *                           [](const openstudio::filesystem::directory_entry& d) {
     *                              const auto p = d.path();
     *                              return openstudio::filesystem::is_directory(p) && openstudio::filesystem::exists(p);
     *                            }
     *                          );
     */


    if (n_dirs == 1) {
      pathToRemove = pathToRemove.parent_path();
    }
    removeDirectory(pathToRemove);


    return true;
  }

  bool LocalBCL::addMeasure(BCLMeasure& measure)
  {

    // Check for uid
    if (m_db && !measure.uid().empty() && !measure.versionId().empty() ) {

      // Start a transaction, so we can handle failures without messing up the database
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);

      std::string uid = measure.uid();
      std::string versionId = measure.versionId();


      std::string statement = "DELETE FROM Measures WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      std::stringstream ss;
      ss << "INSERT INTO Measures (uid, version_id, name, description, modeler_description, date_added, date_modified) "
        << "VALUES('" << escape(uid) << "', '" << escape(versionId) << "', '" << escape(measure.name())
        << "', '" << escape(measure.description()) << "', '" << escape(measure.description()) << "'"
        << ", datetime('now','localtime'), datetime('now','localtime'));";

      statement = ss.str();

      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      // Insert files
      statement = "DELETE FROM Files WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      for(const BCLFileReference& file: measure.files()) {

        std::stringstream ss;
        ss << "INSERT INTO Files (uid, version_id, filename, filetype, usage_type, checksum) "
          << "VALUES('" << escape(uid) << "', '" << escape(versionId) << "', '" << escape(file.fileName())
          << "', '" << escape(file.fileType()) << "', '" << escape(file.usageType())
          << "', '" << escape(file.checksum()) << "');";

        statement = ss.str();

        if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          return false;
        }
      } // End insert all measure's files


      // Insert Attributes
      statement = "DELETE FROM Attributes WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }

      for (const Attribute& attribute : measure.attributes()) {
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

        std::stringstream ss;
        ss << "INSERT INTO Attributes (uid, version_id, name, value, units, type) "
          << "VALUES('" << escape(uid) << "', '" << escape(versionId) << "', '" << escape(attribute.name())
          << "', '" << escape(dataValue) << "', '" << escape(attribute.units() ? attribute.units().get() : "")
          << "', '" << escape(dataType)  << "');";

        statement = ss.str();

        if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
          // Rollback changes
          sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
          return false;
        }
      } // End insert each attribute

      // Commit changes now that everything went well
      sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
      return true;

    } // End check uid

    return false;

  }

  bool LocalBCL::removeMeasure(BCLMeasure& measure)
  {
    // if uid is empty
    if (!m_db || measure.uid().empty() || measure.versionId().empty()){
      return false;
    }

    // TODO: JM 2018-11-14 Check if Measure is actually part of the SQL database first?

    // Start by deleting stuff from the SQL file
    // We don't expect any of these to fail really, if it isn't present, then it still returns OK
    // But let's be prudent and still start a transaction

    // Start a transaction, so we can handle failures without messing up the database
    sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);

    std::string uid = measure.uid();
    std::string versionId = measure.versionId();

    std::string statement(
        "DELETE FROM Measures WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"';"
        "DELETE FROM Files WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"';"
        "DELETE FROM Attributes WHERE uid='" + escape(uid) +"' AND version_id='" + escape(versionId) +"';"
        );
    if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
      LOG(Error, "Couldn't delete component from SQL file");
      // Rollback changes
      sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
      return false;
    }

    // Commit changes now that everything went well
    sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);

    // proceed deleting component on disk
    openstudio::path pathToRemove = m_libraryPath / uid / versionId;

    // We look at the parent path, and if we find only one folder = (one versionId), we delete the parent instead
    // so we don't end up with an empty folder
    unsigned int n_dirs = 0;
    for (auto dir_itr = openstudio::filesystem::directory_iterator(pathToRemove.parent_path());
        dir_itr != openstudio::filesystem::directory_iterator();
        ++dir_itr)
    {
      const auto p = dir_itr->path();
      if (openstudio::filesystem::is_directory(p) && openstudio::filesystem::exists(p)) {
        ++n_dirs;
      }
    }

    // Lambda version, doesn't really help clarity here
    /*
     *int n_dirs = std::count_if(openstudio::filesystem::directory_iterator(pathToRemove.parent_path()),
     *                           openstudio::filesystem::directory_iterator(),
     *                           [](const openstudio::filesystem::directory_entry& d) {
     *                              const auto p = d.path();
     *                              return openstudio::filesystem::is_directory(p) && openstudio::filesystem::exists(p);
     *                            }
     *                          );
     */

    if (n_dirs == 1) {
      pathToRemove = pathToRemove.parent_path();
    }
    removeDirectory(pathToRemove);

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
      const std::string& componentType) const
  {
    typedef std::vector<std::pair<std::string, std::string> > UidsVecType;
    typedef std::set<std::pair<std::string, std::string> > UidsType;

    UidsType uids;

    if (!m_db) {
      return uids;
    }

    // Make pairs of (uid, version_id)
    {
      std::string tableName = (componentType == "component") ? "Components" : ((componentType == "measure") ? "Measures" : "");

      std::string statement = "SELECT DISTINCT uid, version_id FROM " + tableName;
      sqlite3_stmt* sqlStmtPtr;
      int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr);

      // Loop until done (or failed)
      while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          // Get values from SELECT
          std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
          std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));

          uids.insert(make_pair(uid, version_id));

        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }
      } // End loop on each match

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
    }

    // Loop on search terms
    {

      // Prepare a statement that we'll bind
      // (Note: do not do name='?', it won't think it's a bind parameter)
      std::string statement = "SELECT uid, version_id FROM Attributes WHERE name=? COLLATE NOCASE AND value=? COLLATE NOCASE";
      sqlite3_stmt* sqlStmtPtr;
      if (sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, nullptr) != SQLITE_OK) {
        LOG(Error, "Cannot prepare statement (in searchTerms)");
        sqlite3_finalize(sqlStmtPtr);
        return UidsType();
      }

      for (const auto & searchTerm : searchTerms){

        UidsType theseUids;

        // Reset the statement to allow binding variables on the next iteration
        sqlite3_reset(sqlStmtPtr);

        // Bind the values now
        std::string name = escape(searchTerm.first);
        std::string value = escape(searchTerm.second);

        if (sqlite3_bind_text(sqlStmtPtr, 1, name.c_str(), name.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
          LOG(Error, "Error binding to the 1st parameter (in searchTerms), name: " << name);
          sqlite3_finalize(sqlStmtPtr);
          return UidsType();
        }
        else if (sqlite3_bind_text(sqlStmtPtr, 2, value.c_str(), value.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
          LOG(Error, "Error binding to the 2nd parameter (in searchTerms), value: " << value);
          sqlite3_finalize(sqlStmtPtr);
          return UidsType();
        }

        int code = SQLITE_OK;

        // Loop until done (or failed)
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            // Get values from SELECT
            std::string uid = columnText(sqlite3_column_text(sqlStmtPtr, 0));
            std::string version_id = columnText(sqlite3_column_text(sqlStmtPtr, 1));

            theseUids.insert(make_pair(uid, version_id));

          }
          else  // i didn't get a row.  something is wrong so set the exit condition.
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;
          }
        } // End loop on each match

        UidsVecType newUids(std::max(uids.size(), theseUids.size()));
        auto insertEnd = std::set_intersection(uids.begin(), uids.end(), theseUids.begin(), theseUids.end(), newUids.begin());

        uids.clear();
        uids.insert(newUids.begin(), insertEnd);

        if (uids.empty()){
          sqlite3_finalize(sqlStmtPtr);
          return UidsType();
        }


      } // End loop on searchTerms

      // Finalize statement to prevent memory leak
      sqlite3_finalize(sqlStmtPtr);
    } // End Loop on search terms

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
    Application::instance().application(false);

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
    openstudio::Application::instance().application(false);

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
    if (m_db && remoteBCL.setProdAuthKey(authKey)) {
      // Store key
      m_prodAuthKey = authKey;

      //Overwrite prodAuthKey in database

      // Start a transaction, so we can handle failures without messing up the database
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);

      std::string statement = "UPDATE Settings SET data='" + escape(authKey) +"' WHERE name='prodAuthKey'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }
      sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
      return true;
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
    if (m_db && remoteBCL.setDevAuthKey(authKey)) {
      // Store key
      m_devAuthKey = authKey;

      //Overwrite devAuthKey in database

      // Start a transaction, so we can handle failures without messing up the database
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);

      std::string statement = "UPDATE Settings SET data='" + escape(authKey) +"' WHERE name='devAuthKey'";
      if (sqlite3_exec(m_db, statement.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Rollback changes
        sqlite3_exec(m_db, "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
      }
      sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
      return true;
    }
    return false;
  }

  openstudio::path LocalBCL::libraryPath() const
  {
    return m_libraryPath;
  }

  bool LocalBCL::setLibraryPath(const path& libraryPath)
  {

    // TODO: JM 2018-11-14 Can't we just call this? then write the settings
    // &LocalBCL::instance(libraryPath):

    // cleanup old staggling one if it exists
    close();

    const auto path = libraryPath.lexically_normal();
    
    openstudio::filesystem::create_directories(path);
    if (!openstudio::filesystem::is_directory(path))
    {
      const bool success = openstudio::filesystem::create_directories(path);
      if (!success) return false;
    }
    m_libraryPath = path;

    m_sqliteFilePath = m_libraryPath / m_dbName;
    m_sqliteFilename = toString(m_sqliteFilePath.make_preferred().native());

    //Check for local database
    bool initschema = !openstudio::filesystem::exists(m_sqliteFilePath);

    // Open the database
    int success = sqlite3_open_v2(m_sqliteFilename.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE, nullptr);
    if (success != SQLITE_OK) {
      LOG(Error, "Unable to open connection for LocalBCL at '" << m_sqliteFilename << "'.");
      return false;
    } else {
      m_connectionOpen = true;
    }

    // If need to create it, and it didn't go well, throw
    if (initschema && !initializeLocalDb()) {
      LOG(Error, "Unable to initialize Local Database");
    }

    // TODO: JM 2018-11-14 In old implementation using Qt, there's no update call!

    QSettings settings("OpenStudio", "LocalBCL");
    settings.setValue("libraryPath", toQString(m_libraryPath));

    return true;
  }


} // openstudio
