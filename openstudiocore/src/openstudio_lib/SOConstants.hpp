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

#ifndef OPENSTUDIO_SOCONSTANTS_HPP
#define OPENSTUDIO_SOCONSTANTS_HPP

namespace openstudio {

enum AddToModelEnum
{
  ADDTOMODEL_SPLIT_TYPE_0_THAI,			/*Split type/DX Type*/
  ADDTOMODEL_SPLIT_TYPE_1_THAI,         /*Split type/DX Type (Inverter)*/
  ADDTOMODEL_SPLIT_TYPE_2_THAI,			/*Split type/DX Type (Inverter) with ERV*/
  ADDTOMODEL_CENTRAL_SYSTEM_TYPE_0_THAI, /*Central System (Thai Central A/C Air Cooled System)*/
  ADDTOMODEL_CENTRAL_SYSTEM_TYPE_1_THAI, /*Central System (Thai Central A/C Air Cooled System)*/
  ADDTOMODEL_AIRLOOPHVAC_1_THAI,		/*Thai Air Loop*/
  ADDTOMODEL_AIRLOOPHVAC_2_THAI,		/*Thai Air Loop with ERV*/
  ADDTOMODEL_AIRLOOPHVAC_THAI,
  ADDTOMODEL_THERMALZONE,
  ADDTOMODEL_AIRLOOPHVAC,
  ADDTOMODEL_DUAL_AIRLOOPHVAC,
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

