   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.20  01/31/02          */
   /*                                                     */
   /*              COMPATIBILITY HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Use this file to enable the use of some old      */
/*   CLIPS macros and functions with version 6.0 of CLIPS.   */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _CMPTBLTY_HEADER_
#define _CMPTBLTY_HEADER_

#include "clips.h"

#define DeleteDefrule(x)               Undefrule(x)
#define DeleteDeftemplate(x)           Undeftemplate(x)
#define DeleteDeffacts(x)              Undeffacts(x)
#define DeleteDefclass(c)              Undefclass(c)
#define DeleteDefinstances(d)          Undefinstances(d)
#define DeleteDefmessageHandler(c,h)   UndefmessageHandler(c,h)
#define DeleteDeffunction(d)           Undeffunction(d)
#define DeleteDefgeneric(g)            Undefgeneric(g)
#define DeleteDefmethod(g,m)           Undefmethod(g,m)

#define ListFacts()                    Facts(WDISPLAY,GetCurrentModule(),-1L,-1L,-1L)
#define ListInstances(c,i)             Instances(WDISPLAY,GetCurrentModule(),GetDefclassName(c),i)

#define ResetCLIPS()                   Reset()
#define LoadConstructs(x)              Load(x)
#define SaveConstructs(x)              Save(x)
#define ClearCLIPS()                   Clear()
#define RunCLIPS(x)                    Run(x)
#define OpenDribble(x)                 DribbleOn(x)
#define CloseDribble()                 DribbleOff()
#define AddFact(x)                     Assert(x)
#define RetractFact(x)                 Retract(x)
#define ListAgenda()                   Agenda(WDISPLAY,GetCurrentModule())
#define ListMatches(x)                 Matches(x)
#define AddBreakpoint(x)               SetBreak(x)
#define RemoveBreakpoint(x)            RemoveBreak(x)
#define ListBreakpoints()              ShowBreaks(WDISPLAY,GetCurrentModule())
#define RefreshDefrule(x)              Refresh(x)
#define MemoryUsed()                   MemUsed()
#define MemoryRequests()               MemRequests()
#define ReleaseMemory()                ReleaseMem()

#define CLIPSTestSlot(i,s,0)           SlotExistP(GetInstanceClass(i),s,TRUE)
#define CLIPSTestSlot(i,s,1)           TRUE
#define CLIPSTestSlot(i,s,2)           SlotWritableP(GetInstanceClass(i),s)
#define CLIPSTestSlot(i,s,3)           SlotInitableP(GetInstanceClass(i),s)
#define CLIPSGetSlot(i,s,r)            DirectGetSlot(i,s,r)
#define CLIPSPutSlot(i,s,v)            DirectPutSlot(i,s,r)
#define PreviewMessage(c,m)            PreviewSend(WDISPLAY,c,m)
#define CLIPSSendMessage(d,m,a,r)      Send(d,m,a,r)
#define CLIPSUnmakeInstance(i)         UnmakeInstance(i)
#define CLIPSDeleteInstance(i)         DeleteInstance(i)
#define CLIPSMakeInstance(s)           MakeInstance(s)
#define BrowseClass(c)                 BrowseClasses(c)
#define IsClassAbstract(c)             ClassAbstractP(c)
#define GetClassSuperclasses(c,r,i)    ClassSuperclasses(c,r,i)
#define GetClassSubclasses(c,r,i)      ClassSubclasses(c,r,i)
#define GetClassSlots(c,r,i)           ClassSlots(c,r,i)
#define GetClassMessageHandlers(c,r,i) GetDefmessageHandlerList(c,r,i)
#define GetSlotFacets(c,s,r)           SlotFacets(c,s,r)
#define GetSlotSources(c,s,r)          SlotSources(c,s,r)

#endif










