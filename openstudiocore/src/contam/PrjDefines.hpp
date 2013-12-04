/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
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
#ifndef PRJDEFINES_H
#define PRJDEFINES_H

#define STD_STRING
#define RX_IS_STRING

#ifdef RX_IS_STRING
#define RX std::string
#define RX_C(v) v
#define RX_INIT(v) std::string(#v)
#endif

#define NOFILELINE

#ifdef NOFILELINE
#define DECFILELINE
#define DECFILELINEC
#define DECCFILELINE
#define ARGFILELINE
#define ARGFILELINEC
#define ARGCFILELINE
#define FILELINE
#define CFILELINE
#define FILELINEC
#else
#define DECFILELINE const char *file, int line
#define DECFILELINEC const char *file, int line,
#define DECCFILELINE ,const char *file, int line
#define ARGFILELINE file,line
#define ARGFILELINEC file,line,
#define ARGCFILELINE ,file,line
#define FILELINE __FILE__,__LINE__
#define CFILELINE ,__FILE__,__LINE__
#define FILELINEC __FILE__,__LINE__,
#endif

// CONTAM icon definitions
#define FLOW_E      1  /* flow arrow - pointing east */
#define FLOW_W      2  /* flow arrow - pointing west */
#define FLOW_N      3  /* flow arrow - pointing north */
#define FLOW_S      4  /* flow arrow - pointing south */
#define ZONE_ST     5  /* zone - standard */
#define ZONE_PH     6  /* zone - phantom */
#define ZONE_AM     7  /* zone - ambient */
#define CONT_SS_C93 8  /* contaminant source/sink (C93) */
#define WL_EW      11  /* wall - east/west */
#define WL_NS      12  /* wall - north/south */
#define WL_ES      14  /* wall - northwest corner */
#define WL_SW      15  /* wall - northeast corner */
#define WL_NW      16  /* wall - southeast corner */
#define WL_NE      17  /* wall - southwest corner */
#define WL_NES     18  /* walls - to north, east, and south */
#define WL_ESW     19  /* walls - to east, south, and west */
#define WL_NSW     20  /* walls - to south, west, and north */
#define WL_NEW     21  /* walls - to west, north, and east */
#define WL_NESW    22  /* wall quad corner */
#define OPNG       23  /* opening - 1-way airflow */
#define DOOR       24  /* opening - 2-way airflow */
#define LG_OPNG    25  /* large 1-way opening */
#define LG_DOOR    27  /* large 2-way opening */
#define FAN_E      28  /* fan - flow to the east */
#define FAN_W      29  /* fan - flow to the west */
#define FAN_N      30  /* fan - flow to the north */
#define FAN_S      31  /* fan - flow to the south */
#define NOTE       42  /* note - '*' */
#define BOX_S     128  /* box - AHS supply */
#define BOX_R     129  /* box - AHS return */
#define S_AHS     130  /* simple AHS zones/element */
#define PEXP      131  /* personal exposure */
#define PERSON    132  /* personal activity (future) */
#define CONT_SS   133  /* contaminant source/sink (C95) */
#define CONT_RX   134  /* contaminant reaction (future) */
#define WMS_EW    136  /* mass layers - east/west, floor */
#define WMS_NS    137  /* mass layers - north/south */
#define WDW_EW    138  /* window layers - east/west, floor */
#define WDW_NS    139  /* window layers - north/south */
#define GEN_MASS  140  /* generic zone mass */
#define RAD_INT   141  /* radiant interchange */
#define HEAT_SRC  142  /* heat source */
#define DCT_EW    143  /* duct (not saved) - east/west */
#define DCT_NS    144  /* duct (not saved) - north/south */
#define DCT_EWk   145  /* duct (saved) - east/west */
#define DCT_NSk   146  /* duct (saved) - north/south */
#define DCT_ES    147  /* duct - east/south */
#define DCT_SW    148  /* duct - south/west */
#define DCT_NW    149  /* duct - north/west */
#define DCT_NE    150  /* duct - north/east */
#define DCT_X     151  /* duct crossing */
#define DCT_EWg   152  /* duct - east/west - grouping icon (future) */
#define DCT_NSg   153  /* duct - north/south - grouping icon (future) */
#define DCT_NSn   154  /* duct - north/south - north positive */
#define DCT_EWe   155  /* duct - east/west - east positive */
#define DCT_NSs   156  /* duct - north/south - south positive */
#define DCT_EWw   157  /* duct - east/west - west positive */
#define JCT       158  /* junction */
#define JCT_CA    159  /* junction - connected to level above */
#define JCT_CB    160  /* junction - connected to level below */
#define JCT_CAB   161  /* junction - connected above & below */
#define IOJ       162  /* junction terminal */
#define IOJ_CA    163  /* terminal - connected to level above */
#define IOJ_CB    164  /* terminal - connected to level below */
#define CL_EW     168  /* control link (not saved) - east/west */
#define CL_NS     169  /* control link (not saved) - north/south */
#define CL_EWk    170  /* control link (saved) - east/west */
#define CL_NSk    171  /* control link (saved) - north/south */
#define CL_ES     172  /* control link - east/south */
#define CL_SW     173  /* control link - south/west */
#define CL_NW     174  /* control link - north/west */
#define CL_NE     175  /* control link - north/east */
#define CL_X      176  /* control link - cross-over */
#define CA_N      177  /* control actuator - point to north */
#define CA_E      178  /* control actuator - point to east */
#define CA_S      179  /* control actuator - point to south */
#define CA_W      180  /* control actuator - point to west */
#define CS_N      181  /* control sensor - point to north */
#define CS_E      182  /* control sensor - point to east */
#define CS_S      183  /* control sensor - point to south */
#define CS_W      184  /* control sensor - point to west */
#define CTRLN     185  /* control node */
#define CTRLP     186  /* phantom control node */
#define CTRLS     187  /* super control node */
#define CTRLO     188  /* super element output control node */
#define CTRLI     189  /* super element input control node */
#define RADIO_Y   210  /* D2 322 box with dot inside */
#define RADIO_N   211  /* D3 323 empty box */
#define BUTTON    212  /* D4 324 pushbutton */
#define CHECK_L   213  /* D5 325 left side of check box */
#define CHECK_Y   214  /* D6 326 check box - yes */
#define CHECK_R   215  /* D7 327 right side of check box */
#define CHECK_N   216  /* D8 330 check box */
#define BLOCK     219  /* DB 333 full block */
#define BLOCK_B   220  /* DC 334 bottom block */
#define BLOCK_C   222  /* DE 335 center block */
#define BLOCK_T   223  /* DF 336 top block */
#define SIMGA     228  /* E4 344 Greek */
#define MU        230  /* E6 346 Greek */
#define OMEGA     234  /* EA 352 Greek omega */
#define DELTA     235  /* EB 353 Greek DELTA */
#define PLSMN     241  /* F1 361 */
#define GTEQ      242  /* F2 362 */
#define LTEQ      243  /* F3 363 */
#define SUB_M     244  /* F4 364 subscript m */
#define SUB_N     245  /* F5 365 subscript n */
#define APPROX    247  /* F7 367 approx */
#define DEGRS     248  /* F8 370 degrees */
#define EXP_4     249  /* F9 371 exponent 4 */
#define EXP_3     250  /* FA 372 exponent 3 */
#define SQRT      251  /* FB 373 square root */
#define EXP_N     252  /* FC 374 exponent n */
#define EXP_2     253  /* FD 375 exponent 2 */
#define SUB_2     254  /* FE 376 subscript 2 */

// The defines below were mainly for the ReverseTranslator object - so
// can be eliminated if that object doesn't ever come back.
// Directional icon defines: E=1, N=2, W=4, S=8
#define BIT_E       1
#define BIT_N       2
#define BIT_W       4
#define BIT_S       8
#define BIT_NE      3
#define BIT_EW      5
#define BIT_NW      6
#define BIT_NEW     7
#define BIT_ES      9
#define BIT_NS     10
#define BIT_NES    11
#define BIT_SW     12
#define BIT_ESW    13
#define BIT_NSW    14
#define BIT_NESW   15

namespace BitMath { // Take that, C++!
const unsigned int E = BIT_E;
const unsigned int N = BIT_N;
const unsigned int W = BIT_W;
const unsigned int S = BIT_S;
const unsigned int NE = BIT_NE;
const unsigned int EW = BIT_EW;
const unsigned int NW = BIT_NW;
const unsigned int NEW = BIT_NEW;
const unsigned int ES = BIT_ES;
const unsigned int NS = BIT_NS;
const unsigned int NES = BIT_NES;
const unsigned int SW = BIT_SW;
const unsigned int ESW = BIT_ESW;
const unsigned int NSW = BIT_NSW;
const unsigned int NESW = BIT_NESW;
}

#endif
