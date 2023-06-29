/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
