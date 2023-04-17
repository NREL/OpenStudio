/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <boost/optional.hpp>

#include <stdexcept>
#include <string>
#include <vector>

struct sqlite3;
struct sqlite3_stmt;

namespace openstudio {

class UTILITIES_API PreparedStatement
{
 private:
  sqlite3* m_db;
  sqlite3_stmt* m_statement;
  bool m_transaction;

  struct InternalConstructor
  {
  };

  // a helper constructor to delegate to. It has the first parameter as
  // InternalConstructor so it's distinguishable from the on with the defaulted
  // t_transaction
  PreparedStatement(InternalConstructor, const std::string& t_stmt, sqlite3* t_db, bool t_transaction);

 public:
  PreparedStatement& operator=(const PreparedStatement&) = delete;
  PreparedStatement(const PreparedStatement&) = delete;

  PreparedStatement(const std::string& t_stmt, sqlite3* t_db, bool t_transaction = false)
    : PreparedStatement(InternalConstructor{}, t_stmt, t_db, t_transaction) {}

  template <typename... Args>
  PreparedStatement(const std::string& t_stmt, sqlite3* t_db, bool t_transaction, Args&&... args)
    : PreparedStatement(InternalConstructor{}, t_stmt, t_db, t_transaction) {
    if (!bindAll(args...)) {
      throw std::runtime_error("Error bindings args with statement: " + t_stmt);
    }
  }

  ~PreparedStatement();

  bool bind(int position, const std::string& t_str);

  bool bind(int position, int val);

  bool bind(int position, unsigned int val);

  bool bind(int position, double val);

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

  [[nodiscard]] static int get_sqlite3_bind_parameter_count(sqlite3_stmt* statement);

  template <typename... Args>
  bool bindAll(Args&&... args) {
    const std::size_t size = sizeof...(Args);
    int nPlaceholders = get_sqlite3_bind_parameter_count(m_statement);
    if (nPlaceholders != size) {
      throw std::runtime_error("Wrong number of placeholders [" + std::to_string(nPlaceholders) + "] versus bindArgs [" + std::to_string(size)
                               + "].");
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
    // copied in here to avoid including sqlite3 header everywhere
    constexpr auto SQLITE_DONE = 101;
    if (code != SQLITE_DONE) {
      throw std::runtime_error("Error executing SQL statement step");
    }
  }

  // Executes a **SINGLE** statement
  int execute();

  [[nodiscard]] boost::optional<double> execAndReturnFirstDouble() const;

  [[nodiscard]] boost::optional<int> execAndReturnFirstInt() const;

  static std::string columnText(const unsigned char* column) {
    return {reinterpret_cast<const char*>(column)};
  }

  [[nodiscard]] boost::optional<std::string> execAndReturnFirstString() const;

  /// execute a statement and return the results (if any) in a vector of double
  [[nodiscard]] boost::optional<std::vector<double>> execAndReturnVectorOfDouble() const;

  /// execute a statement and return the results (if any) in a vector of int
  [[nodiscard]] boost::optional<std::vector<int>> execAndReturnVectorOfInt() const;

  /// execute a statement and return the results (if any) in a vector of string
  [[nodiscard]] boost::optional<std::vector<std::string>> execAndReturnVectorOfString() const;
};

}  // namespace openstudio

#endif  // UTILITIES_SQL_PREPAREDSTATEMENT_HPP
