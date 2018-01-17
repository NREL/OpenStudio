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

#ifndef RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEOBJECT_HPP
#define RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEOBJECT_HPP

#include <iomanip>
#include <vector>

#include "../../../utilities/core/Path.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>

#include "sqlite3.h"


class Results {

  public:

    Results();
    ~Results();

    void clear();

    std::vector<std::string> header;
    std::vector< std::vector<std::string> > data;

};


class SqliteObject {

  public:
    SqliteObject(const openstudio::path &file);
    ~SqliteObject();

    bool execute(const std::string& cmd);
    bool deleteDay(const boost::gregorian::date &);
    bool removeDesignDay();  //BLB

    void print();
    void getData(std::vector<std::string> &data);
    double sum(const std::string& name);
    void summary();
    void extract();
    void extract_by_month();
    void extract_by_variable();
    void extract_total();
    boost::gregorian::date getStartDay();


  private:
    sqlite3 *m_db;
    Results *m_results;

    openstudio::path m_file;
    int findHeaderIndex(const std::string& name);


};

#endif // RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEOBJECT_HPP
