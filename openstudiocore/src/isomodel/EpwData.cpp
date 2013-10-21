#include <isomodel/EpwData.hpp>

namespace openstudio {
namespace isomodel {

EpwData::EpwData()
  : m_data(7)
{
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
        this->m_location = s;
        //cout << "\tLocation: " << s <<endl;
        break;
      case 5:
        this->m_stationid = s;
        //cout << "\tStation ID: " << s <<endl;
        break;
      case 6:
        this->m_latitude = atof(s.c_str());
        //cout << "\tLatitude: " << s <<endl;
        break;
      case 7:
        this->m_longitude = atof(s.c_str());
        //cout << "\tLongitude: " << s <<endl;
        break;
      case 8:
        this->m_timezone = atoi(s.c_str());
        //cout << "\tTimezone: " << s <<endl;
        break;
      default:
        break;
    }
  }			
}

void EpwData::parseData(const std::string &line, int row)
{
  std::stringstream linestream(line);
  int col = 0;

  for(int i = 0;i<22;i++)
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
        this->m_data[col++][row] = ::atof(s.c_str());
        break;
      default:
        break;
    }
  }	
}

std::string EpwData::toISOData(){
  TimeFrame frames;
  SolarRadiation pos(&frames,this);
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

void EpwData::loadData(const std::string &fn)
{

  for(int c = 0;c<7;c++)
  {
    m_data[c].resize(8760);
  }

  std::ifstream myfile(fn.c_str());
  if (myfile.is_open())
  {
    int i = 0;
    int row = 0;
    while ( myfile.good() && row < 8760)
    {
      i++;
      std::string line;
      getline(myfile,line);
      if(i==1)
      {
        this->parseHeader(line);
      }
      else if(i > 8)
      {
        this->parseData(line,row++);
      }      
    }
    myfile.close();
  }
}
}
}
