/*******************************************************************************************************************************
 ZCMD Improved (I-ZCMD)

 This include is just a re-make of the ZCMD Command Processor ( V0.3.1 31/10/2009) originnaly developed by Zeex.

 Version:0.1

 Changes:
	ZCMD to I-ZCMD
	Functions excatly the same way as ZCMD does

	OnPlayerCommandReceived & OnPlayerCommandPerformed now are hooked using ALS Hook Method 7
    Removed the OnGameModeInit/OnFilterscriptInit Hooks
    Made minor optimizations
 Credits:
 Yashas
 Zeex for the original include
*******************************************************************************************************************************/
#if defined _I_ZCMD_INCLUDED_V031_001
	#endinput
#endif	
#define _I_ZCMD_INCLUDED_V031_001
/******************************************************************************************************************************/
#define MAX_FUNC_NAME (32)

#define COMMAND:%1(%2)          \
			forward cmd_%1(%2); \
			public cmd_%1(%2)		
			
#define CMD:%1(%2) \
			COMMAND:%1(%2)

#define command(%1,%2,%3) \
			COMMAND:%1(%2, %3)      
      
#define cmd(%1,%2,%3) \
			COMMAND:%1(%2, %3)
/******************************************************************************************************************************/
#if defined OnPlayerCommandReceived
	forward OnPlayerCommandReceived(playerid,cmdtext[]);
#endif
#if defined OnPlayerCommandPerformed
	forward OnPlayerCommandPerformed(playerid,cmdtext[], success);
#endif
/******************************************************************************************************************************/
public OnPlayerCommandText(playerid, cmdtext[])
{
    static funcname[MAX_FUNC_NAME] = "cmd_";

	#if defined OnPlayerCommandReceived
	    if(!OnPlayerCommandReceived(playerid,cmdtext)) return 1;
	#endif

	new pos = 1;
	
    while(cmdtext[pos] > ' ')
		if(cmdtext[pos] == '_') funcname[pos++ + 3] = '_';
		else funcname[pos+3] = cmdtext[pos++] | 0x20;
		
	funcname[pos+3] = 0;
	
    while (cmdtext[pos] == ' ') pos++;
	if(cmdtext[pos])
	{
  		#if defined OnPlayerCommandPerformed
			return OnPlayerCommandPerformed(playerid,cmdtext,CallLocalFunction(funcname,"is",playerid,cmdtext[pos]));
		#else
			return CallLocalFunction(funcname,"is",playerid,cmdtext[pos]);
		#endif
	}
	else
	{
	   	#if defined OnPlayerCommandPerformed
			return OnPlayerCommandPerformed(playerid,cmdtext,CallLocalFunction(funcname,"is",playerid,"\1"));
		#else
			return CallLocalFunction(funcname,"is",playerid,"\1");
		#endif
	}
}

#if defined _ALS_OnPlayerCommandText
    #undef OnPlayerCommandText
#else
    #define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText izcmd_OnPlayerCommandText
forward izcmd_OnPlayerCommandText(playerid, cmdtext[]);
