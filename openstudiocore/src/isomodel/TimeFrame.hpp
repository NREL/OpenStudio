/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#ifndef ISOMODEL_TIMEFRAME_HPP
#define ISOMODEL_TIMEFRAME_HPP

namespace openstudio {
namespace isomodel {
#define TIMESLICES 8760

class TimeFrame
{
protected:
	
public:
	int monthLength(int month);
	int YTD[TIMESLICES], Hour[TIMESLICES], Day[TIMESLICES], Month[TIMESLICES];
	TimeFrame(void);
	~TimeFrame(void);
};
}
}
#endif
