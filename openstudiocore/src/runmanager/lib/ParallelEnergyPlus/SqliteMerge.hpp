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

#ifndef RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEMERGE_HPP
#define RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEMERGE_HPP

#include <iostream>
#include <vector>
#include "../../../utilities/core/Path.hpp"

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

#endif // RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEMERGE_HPP
