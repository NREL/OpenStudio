/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "EpwData.hpp"
#include "SolarRadiation.hpp"

namespace openstudio {
namespace isomodel {

EpwData::EpwData(const openstudio::path &t_path)
  : m_data(7, std::vector<double>(8760))
{
  loadData(t_path);
}

void EpwData::parseHeader(const std::string &line)
{
  std::stringstream linestream(line);

  //cout << "Weather Location Header: "<<endl;
  for(int i = 0;i<10;i++)
  {
    std::string s;
    std::getline(linestream, s, ',');
    switch(i)
    {
      case 1:
        m_location = s;
        //cout << "\tLocation: " << s <<endl;
        break;
      case 5:
        m_stationid = s;
        //cout << "\tStation ID: " << s <<endl;
        break;
      case 6:
        m_latitude = atof(s.c_str());
        //cout << "\tLatitude: " << s <<endl;
        break;
      case 7:
        m_longitude = atof(s.c_str());
        //cout << "\tLongitude: " << s <<endl;
        break;
      case 8:
        m_timezone = atoi(s.c_str());
        //cout << "\tTimezone: " << s <<endl;
        break;
      default:
        break;
    }
  }
}

void EpwData::parseData(const std::string &line, size_t row)
{
  std::stringstream linestream(line);
  size_t col = 0;

  for(size_t i = 0;i<22;i++)
  {
    std::string s;
    std::getline(linestream, s, ',');
    switch(i)
    {
      case 6:
      case 7:
      case 8:
      case 13:
      case 14:
      case 15:
      case 21:
        assert(col < m_data.size());
        assert(row < m_data[col].size());
        m_data[col++][row] = ::atof(s.c_str());
        break;
      default:
        break;
    }
  }
}

void EpwData::toISOData(Matrix &_msolar, Matrix &_mhdbt, Matrix &_mhEgh, Vector &_mEgh, Vector &_mdbt, Vector &_mwind) const
{
  struct MakeMatrix {
    static Matrix makeMatrix(const std::vector<std::vector<double> > &t_matrix)
    {
      size_t height = t_matrix.size();
      size_t width = t_matrix.at(0).size();

      Matrix ret(t_matrix.size(), t_matrix.at(0).size());

      for (size_t i = 0; i < height; ++i)
      {
        for (size_t j = 0; j < width; ++j)
        {
          ret(i, j) = t_matrix.at(i).at(j);
        }
      }

      return ret;
    }
  };

  TimeFrame frames;
  SolarRadiation pos(frames, *this);
  pos.Calculate();
  _msolar = MakeMatrix::makeMatrix(pos.monthlySolarRadiation());
  _mhdbt = MakeMatrix::makeMatrix(pos.hourlyDryBulbTemp());
  _mhEgh = MakeMatrix::makeMatrix(pos.hourlyGlobalHorizontalRadiation());
  _mEgh = openstudio::createVector(pos.monthlyGlobalHorizontalRadiation());
  _mdbt = openstudio::createVector(pos.monthlyDryBulbTemp());
  _mwind = openstudio::createVector(pos.monthlyWindspeed());
}

std::string EpwData::toISOData() const {
  TimeFrame frames;
  SolarRadiation pos(frames, *this);
  pos.Calculate();
  std::stringstream sstream;
  sstream << "mdbt" << std::endl;
  for(int i = 0;i<12;i++)
  {
    sstream << i << "," << pos.monthlyDryBulbTemp()[i] <<std::endl;
  }
  sstream << "mwind" << std::endl;
  for(int i = 0;i<12;i++)
  {
    sstream << i << "," << pos.monthlyWindspeed()[i] <<std::endl;
  }
  sstream << "mEgh" << std::endl;
  for(int i = 0;i<12;i++)
  {
    sstream << i << "," << pos.monthlyGlobalHorizontalRadiation()[i] <<std::endl;
  }
  sstream << "hdbt" << std::endl;
  for(int i = 0;i<12;i++)
  {
    sstream << i;
    for(int h = 0;h<24;h++)
    {
      sstream << "," << pos.hourlyDryBulbTemp()[i][h];
    } 
    sstream <<std::endl;
  }
  sstream << "hEgh" << std::endl;
  for(int i = 0;i<12;i++)
  {
    sstream << i;
    for(int h = 0;h<24;h++)
    {
      sstream << "," << pos.hourlyGlobalHorizontalRadiation()[i][h];
    } 
    sstream <<std::endl;
  }
  sstream << "solar" << std::endl;
  for(int i = 0;i<12;i++)
  {
    sstream << i;
    for(int s = 0;s<SolarRadiation::NUM_SURFACES;s++)
    {
      sstream << "," << pos.monthlySolarRadiation()[i][s];
    } 
    sstream <<std::endl;
  }
  return sstream.str();
}

void EpwData::loadData(const openstudio::path &fn)
{
  // Array was fully initialized in constructor
  std::ifstream myfile(openstudio::toString(fn).c_str());
  if (myfile.is_open())
  {
    size_t i = 0;
    size_t row = 0;
    while ( myfile.good() && row < 8760)
    {
      i++;
      std::string line;
      getline(myfile,line);
      if(i==1)
      {
        parseHeader(line);
      }
      else if(i > 8)
      {
        parseData(line,row++);
      }      
    }
    myfile.close();
  } else {
    throw std::runtime_error("Unable to open weather file: " + openstudio::toString(fn));
  }
}
}
}
