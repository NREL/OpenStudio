#include <isomodel/SolarRadiation.hpp>

namespace openstudio {
namespace isomodel {
/**
* Surface Azimuths of the "building" to calculate solar radiation for
*/
double SurfaceAzimuths[] =  {0, -45, -90, -135, 180, 135, 90, 45};


SolarRadiation::SolarRadiation(TimeFrame* frame, EpwData* wdata, double tilt)
{
	this->frame = frame;
	this->weatherData = wdata;
	this->Longitude = wdata->longitude;
	this->LocalMeridian = wdata->timezone * 15.0; //compute the local meridian from the time zone.  Negative is W of the prime meridian
	this->Latitude = wdata->latitude * PI / 180.0; //convert latitute to radians
	this->SurfaceTilt = tilt/2.0;	//surface tilt in radians (pi/2 is vertical, 0 is horizontal);
}
SolarRadiation::~SolarRadiation(void) {}

/**
* compute the monthly average solar radiation incident on the vertical surfaces for the 
* eight primary directions (N, S, E, W, NW, SW, NE, SE)
* these computations come from ASHRAE2007  Fundamentals , chapter 14
* or Duffie and Beckman "Solar engineering of thermal processes, 3rd ed",
* Wiley 2006
*/
void SolarRadiation::calculateSurfaceSolarRadiation()
{
	double rhog = 0.14;//ground reflectivity coefficient
	double GroundReflected=0,SolarAzimuthSin=0,SolarAzimuthCos=0,SolarAzimuth=0,
		Revolution,EquationOfTime,ApparentSolarTime,SolarDeclination,SolarHourAngles,SolarAltitudeAngles;

	double AngleOfIncidence,SurfaceSolarAzimuth,DirectBeam,DiffuseRadiation,DiffuseComponent;

	for(int i = 0;i< TIMESLICES;i++)
	{
		// First compute the solar azimuth for each hour of the year for our location
		Revolution = 2.0 * PI * ((double)frame->YTD[i]-1.0) / 365.0;//should be .25? //calculation revolution angle around sun in radians
		EquationOfTime = 2.2918 * (0.0075 + 0.1868 * cos(Revolution) - 3.2077 * sin(Revolution) - 1.4615 * cos(2 * Revolution) - 4.089 * sin(2 * Revolution));//equation of time??
		ApparentSolarTime = frame->Hour[i] + EquationOfTime  / 60.0 + (Longitude-LocalMeridian) / 15.0;  // Apparent Solar Time in hours

		//the following is a more accurate formula for declination as taken from - Duffie and Beckman P. 14
		SolarDeclination =  0.006918-0.399913 * cos(Revolution) + 0.070257 * sin(Revolution) - 0.006758 * cos(2.0 * Revolution) + 0.00907 * sin(2.0 * Revolution)
			- 0.002679 * cos(3.0 * Revolution) + 0.00148 * sin(3.0 * Revolution);//solar declination in radians
		SolarHourAngles = 15 * (ApparentSolarTime - 12) * PI / 180.0;//solar hour angle in radians
		SolarAltitudeAngles = asin(cos(this->Latitude) * cos(SolarDeclination) * cos(SolarHourAngles) + sin(this->Latitude) * sin(SolarDeclination));//solar altitude angle in radians

		SolarAzimuthSin = sin(SolarHourAngles) * cos(SolarDeclination) / cos(SolarAltitudeAngles);//sin of the solar azimuth
		SolarAzimuthCos = (cos(SolarHourAngles) * cos(SolarDeclination) * sin(this->Latitude) - sin(SolarDeclination) * cos(this->Latitude)) / cos(SolarAltitudeAngles);//cosine of solar azimuth
		SolarAzimuth = atan2(SolarAzimuthSin,SolarAzimuthCos);//compute solar azimuth in radians
		
		GroundReflected = (weatherData->data[EB][i] * sin(SolarAltitudeAngles)+weatherData->data[ED][i]) * rhog * (1-cos(SurfaceTilt))/2;  // ground reflected component

		//then compute the hourly radiation on each vertical surface given the solar azimuth for each hour
		for(int s = 0;s<NUM_SURFACES;s++)
		{
			SurfaceSolarAzimuth = abs(SolarAzimuth - (SurfaceAzimuths[s]*(PI/180.0)));//surface - solar azimuth in degrees, >pi/2 means surface is in shade

			AngleOfIncidence = acos(cos(SolarAltitudeAngles) * cos(SurfaceSolarAzimuth * PI / 180.0) * sin(SurfaceTilt) + sin(SolarAltitudeAngles) * cos(SurfaceTilt)); //ancle of incidence of sun's rays on surface in rad

			DirectBeam = weatherData->data[EB][i] * max(cos(AngleOfIncidence), 0.0);//Beam component of radiation

			DiffuseRadiation = max(0.45, 0.55 + 0.437 * cos(AngleOfIncidence) + 0.313 * pow(cos(AngleOfIncidence), 2.0)); //Diffuse component of radiation 
			//diffuse component for sigma> pi/2 meaning it is a wall tilted outward, for sigma<= pi/2 meaning wall vertical or tilted inward
			DiffuseComponent= (SurfaceTilt>PI/2) ? weatherData->data[ED][i] * DiffuseRadiation * sin(SurfaceTilt) : weatherData->data[ED][i] * (DiffuseRadiation * sin(SurfaceTilt)+cos(SurfaceTilt));

			Eglobe[i][s] = DirectBeam + DiffuseComponent + GroundReflected;  // add up all the components
		}
	}
}
//average the data in the bins over the count or days
void SolarRadiation::calculateMonthAvg(int midx, int cnt)
{
	int days=0;
	if(midx > -1)
	{
		//average rate by month
		MonthlyDryBulbTemp[midx] /= cnt;
		MonthlyDewPointTemp[midx] /= cnt;
		MonthlyRelativeHumidity[midx] /= cnt;
		MonthlyWindspeed[midx] /= cnt;
		MonthlyGlobalHorizontalRadiation[midx] /= cnt;
		for(int s = 0;s<NUM_SURFACES;s++)
		{
			MonthlySolarRadiation[midx][s] /= cnt;
		}
		//hours are averaged over days in the month
		days = frame->monthLength(midx+1);
		for(int h = 0;h<24;h++)
		{
			HourlyDryBulbTemp[midx][h] /= days;
			HourlyDewPointTemp[midx][h] /= days;
			HourlyGlobalHorizontalRadiation[midx][h] /= days;
		}
	}
}
//Empty bins to start accumulating data for monthly averages
void SolarRadiation::clearMonthlyAvg(int midx)
{
	for(int h = 0;h<24;h++)
	{
		HourlyDryBulbTemp[midx][h]=0;
		HourlyDewPointTemp[midx][h]=0;
		HourlyGlobalHorizontalRadiation[midx][h]=0;
	}
	for(int s = 0;s<NUM_SURFACES;s++)
	{
		MonthlySolarRadiation[midx][s] = 0;
	}
	MonthlyDryBulbTemp[midx] = 0;
	MonthlyDewPointTemp[midx] = 0;
	MonthlyRelativeHumidity[midx]  = 0;
	MonthlyWindspeed[midx]= 0;
	MonthlyGlobalHorizontalRadiation[midx] = 0;
}

void SolarRadiation::calculateAverages()
{
	int month= 0;
	int midx = -1;
	int cnt  = 0;
	int days = 0;
	int h = 0;

	for(int i = 0;i<TIMESLICES;i++, cnt++)
	{
		if(frame->Month[i] != month)
		{
			month = frame->Month[i];
			//on month change, average bin out over count
			calculateMonthAvg(midx,cnt);
			midx++;
			//reset accumulators
			clearMonthlyAvg(midx);
			cnt = 0;
		}
		//accumulate data into bins
		MonthlyDryBulbTemp[midx] += weatherData->data[DBT][i];
		MonthlyDewPointTemp[midx] += weatherData->data[DPT][i];
		MonthlyRelativeHumidity[midx]  += weatherData->data[RH][i];
		MonthlyGlobalHorizontalRadiation[midx] += weatherData->data[EGH][i];
		MonthlyWindspeed[midx]+= weatherData->data[WSPD][i];
		for(int s = 0;s<NUM_SURFACES;s++)
			MonthlySolarRadiation[midx][s] += Eglobe[i][s];
		h = frame->Hour[i]-1;
		HourlyDryBulbTemp[midx][h] += weatherData->data[DBT][i];
		HourlyDewPointTemp[midx][h] += weatherData->data[DPT][i];
		HourlyGlobalHorizontalRadiation[midx][h] += weatherData->data[EGH][i];
	}
	//final month average
	calculateMonthAvg(midx,cnt);
}

//Calculate hourly solar radiation for each surface
//and then calculate the monthly/hourly averages
void SolarRadiation::Calculate()
{
	calculateSurfaceSolarRadiation();
	calculateAverages();
}

}
}