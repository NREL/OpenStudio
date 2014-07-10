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

#ifndef OPENSTUDIO_SOCONSTANTS_HPP
#define OPENSTUDIO_SOCONSTANTS_HPP

namespace openstudio {

enum AddToModelEnum
{
  ADDTOMODEL_THERMALZONE,
  ADDTOMODEL_AIRLOOPHVAC,
  ADDTOMODEL_SYSTEM_TYPE_1,
  ADDTOMODEL_SYSTEM_TYPE_2,
  ADDTOMODEL_SYSTEM_TYPE_3,
  ADDTOMODEL_SYSTEM_TYPE_4,
  ADDTOMODEL_SYSTEM_TYPE_5,
  ADDTOMODEL_SYSTEM_TYPE_6,
  ADDTOMODEL_SYSTEM_TYPE_7,
  ADDTOMODEL_SYSTEM_TYPE_8,
  ADDTOMODEL_SYSTEM_TYPE_9,
  ADDTOMODEL_SYSTEM_TYPE_10,
  ADDTOMODEL_PLANTLOOP,
};

enum NewFromTemplateEnum
{
  NEWFROMTEMPLATE_EMPTY,
  NEWFROMTEMPLATE_WIZARD
};

} //openstudio

#endif // OPENSTUDIO_SOCONSTANTS_HPP

