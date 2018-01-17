/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
