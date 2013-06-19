#ifndef RUNMANAGER_SQLITEMERGE_HPP_
#define RUNMANAGER_SQLITEMERGE_HPP_

#include <iostream>
#include <vector>
#include <utilities/core/Path.hpp>

#include "sqlite3.h"


class SqliteMerge {

  public:
    SqliteMerge();
    ~SqliteMerge();

    void mergeFiles();
    void loadFile(const openstudio::path &);

  private:
    void renameFinalDatabase(const openstudio::path &);
    std::vector<openstudio::path> m_files;

    openstudio::path m_working;
    std::string m_final;

    int m_primaryKeyBase;

    // sql helper functions
    static sqlite3 * openDatabase(const openstudio::path &);
    static void closeDatabase(sqlite3 *);
    static void mergeDatabases(sqlite3 *, const openstudio::path &source);
    static void attachDatabases(sqlite3 *, const openstudio::path &source);
    static void detachDatabases(sqlite3 *);
    static bool executeCommand(sqlite3 *, const std::string &);

    static bool commit(sqlite3 *);
    static bool begin(sqlite3 *);

    // Here are the table updates...
    static void dropTabularData(sqlite3 *);
    static void tableTime(sqlite3 *);
    static void tableReportMeterData(sqlite3 *);
    static void tableReportMeterExtendedData(sqlite3 *);
    static void tableReportVariableData(sqlite3 *); 
    static void tableReportVariableExtendedData(sqlite3 *);

    static void summary(sqlite3 *);
    static void printNumberRows(sqlite3 *, const std::string &);
    static void returnNumberRows(sqlite3 *, const std::string &);  

    static void printMeterData(sqlite3 *);

    static void createView(sqlite3 *); 
    static void createABUPS(sqlite3 *); 


};

#endif
