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

#ifndef AIRFLOW_CONTAM_PRJDEFINES_HPP
#define AIRFLOW_CONTAM_PRJDEFINES_HPP

#include <string>
#include <sstream>

using PRJFLOAT = std::string;

namespace openstudio::contam {

template <typename DesiredType, typename InputType>
DesiredType to(const InputType& inp) {
  std::stringstream ss;
  ss.imbue(std::locale::classic());
  ss.str(inp);
  DesiredType o;
  ss >> o;
  if (!ss.eof() || ss.fail()) {
    throw std::bad_cast();
  }
  return o;
}

template <typename DesiredType, typename InputType>
bool is_valid(const InputType& input) {
  try {
    to<DesiredType>(input);
    return true;
  } catch (const std::bad_cast&) {
    return false;
  }
}

template <typename DesiredType, typename InputType, typename DestinationType>
bool assign_if_valid(const InputType& input, DestinationType& dest) {
  if (is_valid<DesiredType>(input)) {
    dest = input;
    return true;
  } else {
    return false;
  }
}

}  // namespace openstudio::contam

#define ANY_TO_STR openstudio::toString

// CONTAM icon definitions
#define FLOW_E 1      /* flow arrow - pointing east */
#define FLOW_W 2      /* flow arrow - pointing west */
#define FLOW_N 3      /* flow arrow - pointing north */
#define FLOW_S 4      /* flow arrow - pointing south */
#define ZONE_ST 5     /* zone - standard */
#define ZONE_PH 6     /* zone - phantom */
#define ZONE_AM 7     /* zone - ambient */
#define CONT_SS_C93 8 /* contaminant source/sink (C93) */
#define WL_EW 11      /* wall - east/west */
#define WL_NS 12      /* wall - north/south */
#define WL_ES 14      /* wall - northwest corner */
#define WL_SW 15      /* wall - northeast corner */
#define WL_NW 16      /* wall - southeast corner */
#define WL_NE 17      /* wall - southwest corner */
#define WL_NES 18     /* walls - to north, east, and south */
#define WL_ESW 19     /* walls - to east, south, and west */
#define WL_NSW 20     /* walls - to south, west, and north */
#define WL_NEW 21     /* walls - to west, north, and east */
#define WL_NESW 22    /* wall quad corner */
#define OPNG 23       /* opening - 1-way airflow */
#define DOOR 24       /* opening - 2-way airflow */
#define LG_OPNG 25    /* large 1-way opening */
#define LG_DOOR 27    /* large 2-way opening */
#define FAN_E 28      /* fan - flow to the east */
#define FAN_W 29      /* fan - flow to the west */
#define FAN_N 30      /* fan - flow to the north */
#define FAN_S 31      /* fan - flow to the south */
#define NOTE 42       /* note - '*' */
#define BOX_S 128     /* box - AHS supply */
#define BOX_R 129     /* box - AHS return */
#define S_AHS 130     /* simple AHS zones/element */
#define PEXP 131      /* personal exposure */
#define PERSON 132    /* personal activity (future) */
#define CONT_SS 133   /* contaminant source/sink (C95) */
#define CONT_RX 134   /* contaminant reaction (future) */
#define WMS_EW 136    /* mass layers - east/west, floor */
#define WMS_NS 137    /* mass layers - north/south */
#define WDW_EW 138    /* window layers - east/west, floor */
#define WDW_NS 139    /* window layers - north/south */
#define GEN_MASS 140  /* generic zone mass */
#define RAD_INT 141   /* radiant interchange */
#define HEAT_SRC 142  /* heat source */
#define DCT_EW 143    /* duct (not saved) - east/west */
#define DCT_NS 144    /* duct (not saved) - north/south */
#define DCT_EWk 145   /* duct (saved) - east/west */
#define DCT_NSk 146   /* duct (saved) - north/south */
#define DCT_ES 147    /* duct - east/south */
#define DCT_SW 148    /* duct - south/west */
#define DCT_NW 149    /* duct - north/west */
#define DCT_NE 150    /* duct - north/east */
#define DCT_X 151     /* duct crossing */
#define DCT_EWg 152   /* duct - east/west - grouping icon (future) */
#define DCT_NSg 153   /* duct - north/south - grouping icon (future) */
#define DCT_NSn 154   /* duct - north/south - north positive */
#define DCT_EWe 155   /* duct - east/west - east positive */
#define DCT_NSs 156   /* duct - north/south - south positive */
#define DCT_EWw 157   /* duct - east/west - west positive */
#define JCT 158       /* junction */
#define JCT_CA 159    /* junction - connected to level above */
#define JCT_CB 160    /* junction - connected to level below */
#define JCT_CAB 161   /* junction - connected above & below */
#define IOJ 162       /* junction terminal */
#define IOJ_CA 163    /* terminal - connected to level above */
#define IOJ_CB 164    /* terminal - connected to level below */
#define CL_EW 168     /* control link (not saved) - east/west */
#define CL_NS 169     /* control link (not saved) - north/south */
#define CL_EWk 170    /* control link (saved) - east/west */
#define CL_NSk 171    /* control link (saved) - north/south */
#define CL_ES 172     /* control link - east/south */
#define CL_SW 173     /* control link - south/west */
#define CL_NW 174     /* control link - north/west */
#define CL_NE 175     /* control link - north/east */
#define CL_X 176      /* control link - cross-over */
#define CA_N 177      /* control actuator - point to north */
#define CA_E 178      /* control actuator - point to east */
#define CA_S 179      /* control actuator - point to south */
#define CA_W 180      /* control actuator - point to west */
#define CS_N 181      /* control sensor - point to north */
#define CS_E 182      /* control sensor - point to east */
#define CS_S 183      /* control sensor - point to south */
#define CS_W 184      /* control sensor - point to west */
#define CTRLN 185     /* control node */
#define CTRLP 186     /* phantom control node */
#define CTRLS 187     /* super control node */
#define CTRLO 188     /* super element output control node */
#define CTRLI 189     /* super element input control node */
#define RADIO_Y 210   /* D2 322 box with dot inside */
#define RADIO_N 211   /* D3 323 empty box */
#define BUTTON 212    /* D4 324 pushbutton */
#define CHECK_L 213   /* D5 325 left side of check box */
#define CHECK_Y 214   /* D6 326 check box - yes */
#define CHECK_R 215   /* D7 327 right side of check box */
#define CHECK_N 216   /* D8 330 check box */
#define BLOCK 219     /* DB 333 full block */
#define BLOCK_B 220   /* DC 334 bottom block */
#define BLOCK_C 222   /* DE 335 center block */
#define BLOCK_T 223   /* DF 336 top block */
#define SIMGA 228     /* E4 344 Greek */
#define MU 230        /* E6 346 Greek */
#define OMEGA 234     /* EA 352 Greek omega */
#define DELTA 235     /* EB 353 Greek DELTA */
#define PLSMN 241     /* F1 361 */
#define GTEQ 242      /* F2 362 */
#define LTEQ 243      /* F3 363 */
#define SUB_M 244     /* F4 364 subscript m */
#define SUB_N 245     /* F5 365 subscript n */
#define APPROX 247    /* F7 367 approx */
#define DEGRS 248     /* F8 370 degrees */
#define EXP_4 249     /* F9 371 exponent 4 */
#define EXP_3 250     /* FA 372 exponent 3 */
#define SQRT 251      /* FB 373 square root */
#define EXP_N 252     /* FC 374 exponent n */
#define EXP_2 253     /* FD 375 exponent 2 */
#define SUB_2 254     /* FE 376 subscript 2 */

namespace openstudio {
namespace contam {

  enum PathFlags
  {
    WIND = 0x0001,   // Path is subject to wind pressure
    WPC_P = 0x0002,  // Path uses WPC file pressure
    WPC_C = 0x0004,  // Path uses WPC file contaminants
    WPC_F = 0x0006,  // Path uses WPC pressure or contaminants
    AHS_S = 0x0008,  // Path is a system (supply or return) flow path
    AHS_R = 0x0010,  // Path is a recirculation flow path
    AHS_O = 0x0020,  // Path is an outside air flow path
    AHS_X = 0x0040,  // Path is an exhaust flow path
    AHS_P = 0x0078,  // Path is any AHS path
    LIM_P = 0x0080,  // Path is pressure limited
    LIM_F = 0x0100,  // Path is flow limited
    FAN_F = 0x0200
  };  // Path is a constant flow fan element

  enum ZoneFlags
  {
    // Zone flag bit 1; variable pressure
    VAR_P = 0x0001,
    NVAR_P = 0xFFFE,
    // Zone flag bit 2; variable contaminants
    VAR_C = 0x0002,
    NVAR_C = 0xFFFD,
    // Zone flag bit 3; variable temperature
    VAR_T = 0x0004,
    NVAR_T = 0xFFFB,
    // Zone flag bit 4; system zone: to avoid zone volume check
    SYS_N = 0x0008,
    NSYS_N = 0xFFF7,
    // Zone flag bit 5; un|conditioned space: to compute air change rate
    UNCZN = 0x0010,   // flags | UNCZN to indicate unconditioned zone
    SETCZN = 0xFFEF,  // flags & SETCZN to indicate conditioned zone
    // Zone flag bit 6; CFD zone
    CFDZN = 0x0020,   // flags | CFDZN to set CFD zone / flags & CFDZN to test for CFD zone
    NCFDZN = 0xFFDF,  // flags & NCFDZN to unset CFD zone
    FLAG_N = 0x003F
  };  // all zone flag bits, used in PrjRead()

}  // namespace contam
}  // namespace openstudio

#endif  // AIRFLOW_CONTAM_PRJDEFINES_HPP
