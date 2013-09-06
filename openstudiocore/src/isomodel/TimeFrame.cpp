
#include <isomodel/TimeFrame.hpp>


namespace openstudio {
namespace isomodel {
TimeFrame::TimeFrame(void)
{
	int i = 0,j = 0, dim;
	for(int I = 1; I <= 12;I++)
	{
		dim = monthLength(I);
		for(int J = 1;J<=dim;J++)
		{
			j++;
			for(int K=1;K<=24;K++)
			{
				this->Hour[i] = K;
				this->Day[i] = J;
				this->Month[i] = I;
				this->YTD[i++] = j;
			}
		}		
	}

}


TimeFrame::~TimeFrame(void)
{


}


int TimeFrame::monthLength(int month)
{
	switch(month)
	{
		case 2:
			return 28;
		case 9:
		case 4:
		case 6:
		case 11:
			return 30;
		default:
			return 31;
	}
}
}
}