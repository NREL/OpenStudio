/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_SQL_PREPAREDSTATEMENT_HPP
#define UTILITIES_SQL_PREPAREDSTATEMENT_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/String.hpp"

#include <sqlite3.h>
#include <boost/optional.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace openstudio {

struct PreparedStatement
{
  sqlite3 *m_db;
  sqlite3_stmt *m_statement;
  bool m_transaction;

  PreparedStatement & operator=(const PreparedStatement&) = delete;
  PreparedStatement(const PreparedStatement&) = delete;

  PreparedStatement(const std::string &t_stmt, sqlite3 *t_db, bool t_transaction = false)
  : m_db(t_db), m_statement(nullptr), m_transaction(t_transaction)
  {
    if (m_transaction)
    {
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);
    }

    int code = sqlite3_prepare_v2(m_db, t_stmt.c_str(), t_stmt.size(), &m_statement, nullptr);

    if (!m_statement)
    {
      int extendedErrorCode = sqlite3_extended_errcode(m_db);
      const char * err = sqlite3_errmsg(m_db);
      std::string errMsg = err;
      throw std::runtime_error("Error creating prepared statement: " + t_stmt + " with error code " + std::to_string(code)
          + ", extended code " + std::to_string(extendedErrorCode) + ", errmsg: " + errMsg);
    }
  }

  template<typename... Args>
  PreparedStatement(const std::string &t_stmt, sqlite3 *t_db, bool t_transaction, Args&&... args)
  : m_db(t_db), m_statement(nullptr), m_transaction(t_transaction)
  {
    if (m_transaction)
    {
      sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);
    }

    int code = sqlite3_prepare_v2(m_db, t_stmt.c_str(), t_stmt.size(), &m_statement, nullptr);

    if (!m_statement)
    {
      int extendedErrorCode = sqlite3_extended_errcode(m_db);
      const char * err = sqlite3_errmsg(m_db);
      std::string errMsg = err;
      throw std::runtime_error("Error creating prepared statement: " + t_stmt + " with error code " + std::to_string(code)
          + ", extended code " + std::to_string(extendedErrorCode) + ", errmsg: " + errMsg);
    }

    if (!bindAll(args...)) {
      throw std::runtime_error("Error bindings args with statement: " + t_stmt);
    }
  }

  ~PreparedStatement()
  {
    if (m_statement)
    {
      sqlite3_finalize(m_statement);
    }

    if (m_transaction)
    {
      sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
    }
  }

  bool bind(int position, const std::string &t_str)
  {
    if (sqlite3_bind_text(m_statement, position, t_str.c_str(), t_str.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
      return false;
    }
    return true;
  }

  bool bind(int position, int val)
  {
    if (sqlite3_bind_int(m_statement, position, val) != SQLITE_OK) {
      return false;
    }
    return true;
  }

  bool bind(int position, unsigned int val)
  {
    if (sqlite3_bind_int(m_statement, position, val) != SQLITE_OK) {
      return false;
    }
    return true;
  }


  bool bind(int position, double val)
  {
    if (sqlite3_bind_double(m_statement, position, val) != SQLITE_OK) {
      return false;
    }
    return true;
  }

  // Makes no sense
  bool bind(int position, char val) = delete;

  /** char* to std::string. */
  bool bind(int position, const char* s) {
    return bind(position, openstudio::toString(s));
  }

  /** wstring to std::string. */
  bool bind(int position, const std::wstring& w) {
    return bind(position, openstudio::toString(w));
  }

  /** wchar_t* to std::string. */
  bool bind(int position, const wchar_t* w) {
    return bind(position, openstudio::toString(w));
  }

  // This one would happen automatically, but I would rather be explicit
  bool bind(int position, bool val) {
    return bind(position, static_cast<int>(val));
  }

  template<typename... Args>
  bool bindAll(Args&&... args) {
    const std::size_t size = sizeof...(Args);
    int nPlaceholders = sqlite3_bind_parameter_count(m_statement);
    if (nPlaceholders != size) {
      throw std::runtime_error("Wrong number of placeholders [" + std::to_string(nPlaceholders) + "] versus bindArgs [" + std::to_string(size) + "].");
      return false;
    } else {
      int i = 1;
      // Call bind(i++, args[i]) until any returns false
      bool ok = (bind(i++, args) && ...);
      return ok;
    }
  }

  // Executes a **SINGLE** statement
  void execAndThrowOnError() {
    int code = execute();
    if (code != SQLITE_DONE)
    {
      throw std::runtime_error("Error executing SQL statement step");
    }
  }

  // Executes a **SINGLE** statement
  int execute() {
    int code = sqlite3_step(m_statement);
    sqlite3_reset(m_statement);
    return code;
  }

  boost::optional<double> execAndReturnFirstDouble() const {
    boost::optional<double> value;
    if (m_db)
    {
      int code = sqlite3_step(m_statement);
      if (code == SQLITE_ROW)
      {
        value = sqlite3_column_double(m_statement, 0);
      }
    }
    return value;
  }

  boost::optional<int> execAndReturnFirstInt() const {
    boost::optional<int> value;
    if (m_db)
    {
      int code = sqlite3_step(m_statement);
      if (code == SQLITE_ROW)
      {
        value = sqlite3_column_int(m_statement, 0);
      }
    }
    return value;
  }

  std::string columnText(const unsigned char* column) const {
    return std::string(reinterpret_cast<const char*>(column));
  }

  boost::optional<std::string> execAndReturnFirstString() const {
    boost::optional<std::string> value;
    if (m_db)
    {
      int code = sqlite3_step(m_statement);
      if (code == SQLITE_ROW)
      {
        value = columnText(sqlite3_column_text(m_statement, 0));
      }
    }
    return value;
  }

  /// execute a statement and return the results (if any) in a vector of double
  boost::optional<std::vector<double> > execAndReturnVectorOfDouble() const {

    boost::optional<double> value;
    boost::optional<std::vector<double> > valueVector;
    if (m_db)
    {
      int code = SQLITE_OK;

      while ((code != SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        if (!valueVector){
          valueVector = std::vector<double>();
        }

        code = sqlite3_step(m_statement);
        if (code == SQLITE_ROW)
        {
          value = sqlite3_column_double(m_statement, 0);
          valueVector->push_back(*value);
        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }

      }// end loop
    }

    return valueVector;
  }

  /// execute a statement and return the results (if any) in a vector of int
  boost::optional<std::vector<int> > execAndReturnVectorOfInt() const {
    boost::optional<int> value;
    boost::optional<std::vector<int> > valueVector;
    if (m_db)
    {
      int code = SQLITE_OK;

      while ((code != SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        if (!valueVector){
          valueVector = std::vector<int>();
        }

        code = sqlite3_step(m_statement);
        if (code == SQLITE_ROW)
        {
          value = sqlite3_column_int(m_statement, 0);
          valueVector->push_back(*value);
        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }

      }// end loop
    }

    return valueVector;
  }

  /// execute a statement and return the results (if any) in a vector of string
  boost::optional<std::vector<std::string> > execAndReturnVectorOfString() const {
    boost::optional<std::string> value;
    boost::optional<std::vector<std::string> > valueVector;
    if (m_db)
    {
      int code = SQLITE_OK;

      while ((code != SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
      {
        if (!valueVector){
          valueVector = std::vector<std::string>();
        }

        code = sqlite3_step(m_statement);
        if (code == SQLITE_ROW)
        {
          value = columnText(sqlite3_column_text(m_statement, 0));
          valueVector->push_back(*value);
        }
        else  // i didn't get a row.  something is wrong so set the exit condition.
        {     // should never get here since i test for all documented return states above
          code = SQLITE_DONE;
        }

      }// end loop
    }
    return valueVector;
  }

};

} // namespace openstudio

#endif // UTILITIES_SQL_PREPAREDSTATEMENT_HPP
