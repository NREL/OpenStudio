/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef AIRFLOW_CONTAM_CONTAMENUMS_HPP
#define AIRFLOW_CONTAM_CONTAMENUMS_HPP

#include "../../utilities/core/Enum.hpp"

namespace openstudio {
namespace contam {

  // clang-format off

/** \class AirflowElementType
 *  \brief Enumeration is used to identify the various airflow element types.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM( MeasureType,
  ((ModelMeasure)(Model Measure))
  ((EnergyPlusMeasure)(EnergyPlus Measure))
  ((UtilityMeasure)(Utility Measure))
  ((ReportingMeasure)(Reporting Measure))
);
 *  \endcode */
OPENSTUDIO_ENUM(AirflowElementType,
  ((PL_ORFC)(plr_orfc))
  ((PL_LEAK1)(plr_leak1))
  ((PL_LEAK2)(plr_leak2))
  ((PL_LEAK3)(plr_leak3))
  ((PL_CONN)(plr_conn))
  ((PL_QCN)(plr_qcn))
  ((PL_FCN)(plr_fcn))
  ((PL_TEST1)(plr_test1))
  ((PL_TEST2)(plr_test2))
  ((PL_CRACK)(plr_crack))
  ((PL_STAIR)(plr_stair))
  ((PL_SHAFT)(plr_shaft))
  ((PL_BDQ)(plr_bdq))
  ((PL_BDF)(plr_bdf))
  ((QFR_QAB)(qfr_qab))
  ((QFR_QAF)(qfr_fab))
  ((QFR_CRACK)(qfr_crack))
  ((QFR_TEST2)(qfr_test2))
  ((DR_DOOR)(dor_door))
  ((DR_PL2)(dor_pl2))
  ((FN_CMF)(fan_cmf))
  ((FN_CVF)(fan_cvf))
  ((FN_FAN)(fan_fan))
  ((CS_FSP)(csf_fsp))
  ((CS_QSP)(csf_qsp))
  ((CS_PSF)(csf_psf))
  ((CS_PSQ)(csf_psq))
  ((AF_SUP)(sup_afe))
  ((UNKNOWN))
);

/** \relates ControlNodeType */
using OptionalAirflowElementType = boost::optional<AirflowElementType>;

/** \relates ControlNodeType */
using AirflowElementTypeVector = std::vector<AirflowElementType>;



/** \class ControlNodeType
 *  \brief Enumeration is used to identify the various control node types.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(ControlNodeType,
  ((CT_SNS)(sns))
  ((CT_SCH)(sch))
  ((CT_SET)(set))
  ((CT_CVF)(cvf))
  ((CT_DVF)(dvf))
  ((CT_LOG)(log))
  ((CT_PAS)(pas))
  ((CT_MOD)(mod))
  ((CT_HYS)(hys))
  ((CT_ABS)(abs))
  ((CT_BIN)(bin))
  ((CT_DLS)(dls))
  ((CT_DLX)(dlx))
  ((CT_INT)(int))
  ((CT_RAV)(rav))
  ((CT_INV)(inv))
  ((CT_AND)(and))
  ((CT_OR)(od))
  ((CT_XOR)(xor))
  ((CT_ADD)(add))
  ((CT_SUB)(sub))
  ((CT_MUL)(mul))
  ((CT_DIV)(div))
  ((CT_SUM)(sum))
  ((CT_AVG)(avg))
  ((CT_MAX)(max))
  ((CT_MIN)(min))
  ((CT_LLS)(lls))
  ((CT_ULS)(uls))
  ((CT_LBS)(lbs))
  ((CT_UBS)(ubs))
  ((CT_LLC)(llc))
  ((CT_ULC)(ulc))
  ((CT_PC1)(pc1))
  ((CT_PI1)(pi1))
  ((CT_SUP)(sup))
  ((CT_SPH)(sph))
);
*  \endcode */
OPENSTUDIO_ENUM(ControlNodeType,
  ((CT_SNS)(sns))
  ((CT_SCH)(sch))
  ((CT_SET)(set))
  ((CT_CVF)(cvf))
  ((CT_DVF)(dvf))
  ((CT_LOG)(log))
  ((CT_PAS)(pas))
  ((CT_MOD)(mod))
  ((CT_HYS)(hys))
  ((CT_ABS)(abs))
  ((CT_BIN)(bin))
  ((CT_DLS)(dls))
  ((CT_DLX)(dlx))
  ((CT_INT)(int))
  ((CT_RAV)(rav))
  ((CT_INV)(inv))
  ((CT_AND)(and))
  ((CT_OR)(od))
  ((CT_XOR)(xor))
  ((CT_ADD)(add))
  ((CT_SUB)(sub))
  ((CT_MUL)(mul))
  ((CT_DIV)(div))
  ((CT_SUM)(sum))
  ((CT_AVG)(avg))
  ((CT_MAX)(max))
  ((CT_MIN)(min))
  ((CT_LLS)(lls))
  ((CT_ULS)(uls))
  ((CT_LBS)(lbs))
  ((CT_UBS)(ubs))
  ((CT_LLC)(llc))
  ((CT_ULC)(ulc))
  ((CT_PC1)(pc1))
  ((CT_PI1)(pi1))
  ((CT_SUP)(sup))
  ((CT_SPH)(sph))
  ((UNKNOWN))
);


/** \relates ControlNodeType */
using OptionalControlNodeType = boost::optional<ControlNodeType>;

/** \relates ControlNodeType */
using ControlNodeTypeVector = std::vector<ControlNodeType>;

  // clang-format on

}  // namespace contam
}  // namespace openstudio

#endif  // AIRFLOW_CONTAM_CONTAMENUMS_HPP
