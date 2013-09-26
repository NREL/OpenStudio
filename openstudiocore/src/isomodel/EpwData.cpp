#include <isomodel/EpwData.hpp>

namespace openstudio {
namespace isomodel {

EpwData::EpwData(void)
{
	this->data = new double * [7];
  for(int c = 0;c<7;c++)
	{
		data[c] = NULL;
	}
}


EpwData::~EpwData(void)
{
	delete[] data;
}


void EpwData::parseHeader(string line)
{
	stringstream linestream (line);
	string s;
  //cout << "Weather Location Header: "<<endl;
	for(int i = 0;i<10;i++)
	{
		getline(linestream, s, ',');
		switch(i)
		{	
		case 1:
			this->location = s;
			//cout << "\tLocation: " << s <<endl;
			break;
		case 5:
			this->stationid = s;
			//cout << "\tStation ID: " << s <<endl;
			break;
		case 6:
			this->latitude = atof(s.c_str());
			//cout << "\tLatitude: " << s <<endl;
			break;
		case 7:
			this->longitude = atof(s.c_str());
			//cout << "\tLongitude: " << s <<endl;
			break;
		case 8:
			this->timezone = (int)atof(s.c_str());
			//cout << "\tTimezone: " << s <<endl;
			break;
		default:
			break;
		}
	}			
}
void EpwData::parseData(string line, int row)
{
	stringstream linestream (line);
	string s;
	int col = 0;
	for(int i = 0;i<22;i++)
	{
		getline(linestream, s, ',');
		switch(i)
		{	
			case 6:
			case 7:
			case 8:
			case 13:
			case 14:
			case 15:
			case 21:
				this->data[col++][row] = (double)::atof(s.c_str());
				break;
			default:
				break;
		}
	}	
}
std::string EpwData::toISOData(){
  std::string results;
  TimeFrame frames;
	SolarRadiation* pos = new SolarRadiation(&frames,this);
	pos->Calculate();
  stringstream sstream;
  sstream << "mdbt" << endl;
	for(int i = 0;i<12;i++)
	{
		sstream << i << "," << pos->MonthlyDryBulbTemp[i] <<endl;
	}
	sstream << "mwind" << endl;
	for(int i = 0;i<12;i++)
	{
		sstream << i << "," << pos->MonthlyWindspeed[i] <<endl;
	}
	sstream << "mEgh" << endl;
	for(int i = 0;i<12;i++)
	{
		sstream << i << "," << pos->MonthlyGlobalHorizontalRadiation[i] <<endl;
	}
	sstream << "hdbt" << endl;
	for(int i = 0;i<12;i++)
	{
    sstream << i;
		for(int h = 0;h<24;h++)
		{
			sstream << "," << pos->HourlyDryBulbTemp[i][h];
		} 
		sstream <<endl;
	}
	sstream << "hEgh" << endl;
	for(int i = 0;i<12;i++)
	{
		sstream << i;
		for(int h = 0;h<24;h++)
		{
			sstream << "," << pos->HourlyGlobalHorizontalRadiation[i][h];
		} 
		sstream <<endl;
	}
	sstream << "solar" << endl;
	for(int i = 0;i<12;i++)
	{
		sstream << i;
		for(int s = 0;s<NUM_SURFACES;s++)
		{
			sstream << "," << pos->MonthlySolarRadiation[i][s];
		} 
		sstream <<endl;
	}
  return sstream.str();
}
void EpwData::loadData(std::string fn)
{
	string line;
	ifstream myfile (fn);
	int i = 0;
	int row = 0;
	
	for(int c = 0;c<7;c++)
	{
		data[c] = new double[8760];
	}
	if (myfile.is_open())
	{
		while ( myfile.good())
		{
			i++;
			getline (myfile,line);
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