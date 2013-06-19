For use in the OpenStudio rulesengine, this version of CLIPS has been modified
in the following ways:

1. clipscpp.h uses the RULESENGINE_API macro to export class CLIPSCPPEnv to
   the rulesengine dll.
2. Function void UserFunctions() in userfunction.c has been modified so that 
   CLIPS can call OpenStudio rulesengine functions.