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

#include "PreparedStatement.hpp"
#include <sqlite3.h>

namespace openstudio {
PreparedStatement::~PreparedStatement() {
  if (m_statement) {
    sqlite3_finalize(m_statement);
  }

  if (m_transaction) {
    sqlite3_exec(m_db, "COMMIT", nullptr, nullptr, nullptr);
  }
}

bool PreparedStatement::bind(int position, const std::string& t_str) {
  if (sqlite3_bind_text(m_statement, position, t_str.c_str(), t_str.size(), SQLITE_TRANSIENT) != SQLITE_OK) {
    return false;
  }
  return true;
}

bool PreparedStatement::bind(int position, int val) {
  if (sqlite3_bind_int(m_statement, position, val) != SQLITE_OK) {
    return false;
  }
  return true;
}

bool PreparedStatement::bind(int position, unsigned int val) {
  if (sqlite3_bind_int(m_statement, position, static_cast<int>(val)) != SQLITE_OK) {
    return false;
  }
  return true;
}

bool PreparedStatement::bind(int position, double val) {
  if (sqlite3_bind_double(m_statement, position, val) != SQLITE_OK) {
    return false;
  }
  return true;
}

int PreparedStatement::execute() {
  int code = sqlite3_step(m_statement);
  sqlite3_reset(m_statement);
  return code;
}

boost::optional<double> PreparedStatement::execAndReturnFirstDouble() const {
  boost::optional<double> value;
  if (m_db) {
    int code = sqlite3_step(m_statement);
    if (code == SQLITE_ROW) {
      value = sqlite3_column_double(m_statement, 0);
    }
  }
  return value;
}

boost::optional<int> PreparedStatement::execAndReturnFirstInt() const {
  boost::optional<int> value;
  if (m_db) {
    int code = sqlite3_step(m_statement);
    if (code == SQLITE_ROW) {
      value = sqlite3_column_int(m_statement, 0);
    }
  }
  return value;
}

boost::optional<std::string> PreparedStatement::execAndReturnFirstString() const {
  boost::optional<std::string> value;
  if (m_db) {
    int code = sqlite3_step(m_statement);
    if (code == SQLITE_ROW) {
      value = columnText(sqlite3_column_text(m_statement, 0));
    }
  }
  return value;
}

boost::optional<std::vector<double>> PreparedStatement::execAndReturnVectorOfDouble() const {

  boost::optional<double> value;
  boost::optional<std::vector<double>> valueVector;
  if (m_db) {
    int code = SQLITE_OK;

    while ((code != SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE))  //loop until SQLITE_DONE
    {
      if (!valueVector) {
        valueVector = std::vector<double>();
      }

      code = sqlite3_step(m_statement);
      if (code == SQLITE_ROW) {
        value = sqlite3_column_double(m_statement, 0);
        valueVector->push_back(*value);
      } else  // i didn't get a row.  something is wrong so set the exit condition.
      {       // should never get here since i test for all documented return states above
        code = SQLITE_DONE;
      }

    }  // end loop
  }

  return valueVector;
}

boost::optional<std::vector<int>> PreparedStatement::execAndReturnVectorOfInt() const {
  boost::optional<int> value;
  boost::optional<std::vector<int>> valueVector;
  if (m_db) {
    int code = SQLITE_OK;

    while ((code != SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE))  //loop until SQLITE_DONE
    {
      if (!valueVector) {
        valueVector = std::vector<int>();
      }

      code = sqlite3_step(m_statement);
      if (code == SQLITE_ROW) {
        value = sqlite3_column_int(m_statement, 0);
        valueVector->push_back(*value);
      } else  // i didn't get a row.  something is wrong so set the exit condition.
      {       // should never get here since i test for all documented return states above
        code = SQLITE_DONE;
      }

    }  // end loop
  }

  return valueVector;
}

boost::optional<std::vector<std::string>> PreparedStatement::execAndReturnVectorOfString() const {
  boost::optional<std::string> value;
  boost::optional<std::vector<std::string>> valueVector;
  if (m_db) {
    int code = SQLITE_OK;

    while ((code != SQLITE_DONE) && (code != SQLITE_BUSY) && (code != SQLITE_ERROR) && (code != SQLITE_MISUSE))  //loop until SQLITE_DONE
    {
      if (!valueVector) {
        valueVector = std::vector<std::string>();
      }

      code = sqlite3_step(m_statement);
      if (code == SQLITE_ROW) {
        value = columnText(sqlite3_column_text(m_statement, 0));
        valueVector->push_back(*value);
      } else  // i didn't get a row.  something is wrong so set the exit condition.
      {       // should never get here since i test for all documented return states above
        code = SQLITE_DONE;
      }

    }  // end loop
  }
  return valueVector;
}

PreparedStatement::PreparedStatement(PreparedStatement::InternalConstructor, const std::string& t_stmt, sqlite3* t_db, bool t_transaction)
  : m_db(t_db), m_statement(nullptr), m_transaction(t_transaction) {
  if (m_transaction) {
    sqlite3_exec(m_db, "BEGIN", nullptr, nullptr, nullptr);
  }

  int code = sqlite3_prepare_v2(m_db, t_stmt.c_str(), t_stmt.size(), &m_statement, nullptr);

  if (!m_statement) {
    int extendedErrorCode = sqlite3_extended_errcode(m_db);
    const char* err = sqlite3_errmsg(m_db);
    std::string errMsg = err;
    throw std::runtime_error("Error creating prepared statement: " + t_stmt + " with error code " + std::to_string(code) + ", extended code "
                             + std::to_string(extendedErrorCode) + ", errmsg: " + errMsg);
  }
}

int PreparedStatement::get_sqlite3_bind_parameter_count(sqlite3_stmt* statement) {
  return sqlite3_bind_parameter_count(statement);
}
}  // namespace openstudio
