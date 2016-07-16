/*
  ZCMD Improved (I-ZCMD)
  
  This include is just a re-make of the ZCMD Command Processor (v 0.3.1 31/10/2009) 
  originnaly developed by Zeex.
  
  Github Link: https://github.com/YashasSamaga/I-ZCMD
  
  Credits:
  Untonyst, Yashas, Zeex,
*/

#if defined _izcmd_included || defined CMD
	#endinput
#endif 
#define _izcmd_included

#define CMD_SUCCESS 1
#define CMD_FAILURE 0

#define COMMAND:%0(%1) \
		forward cmd_%0(%1); \
		public cmd_%0(%1)

#define CMD:%0(%1) \
		COMMAND:%0(%1)

#define command:%0(%1) \
		COMMAND:%0(%1)
		
#define cmd:%0(%1) \
		COMMAND:%0(%1)

#define ZALT:%0:%1; \
	 	forward cmd_%1(playerid, params[]); \
		public cmd_%1(playerid, params[]) \
			return cmd_%0(playerid, params);

#if !defined isnull
	#define isnull(%0)\
		(((%0[0]) == 0) || ((%0[0]) == '\1') && ((%0[1]) == 0))
#endif

#if defined OnPlayerCommandReceived
forward OnPlayerCommandReceived(playerid, cmdtext[]);
#endif

#if defined OnPlayerCommandPerformed
forward OnPlayerCommandPerformed(playerid, cmdtext[], success);
#endif

public OnPlayerCommandText(playerid, cmdtext[]) 
{
#if defined OnPlayerCommandReceived
	if (OnPlayerCommandReceived(playerid, cmdtext) == 0)
	{
		return 1;
	}
#endif

	static function[32] = "cmd_";

	new i;
	while (cmdtext[++i] > ' ')
	{
	    function[i + 3] = tolower(cmdtext[i]);
	}
	
	while (cmdtext[i++] == ' ')
	{
		// magic
	}

#if defined OnPlayerCommandPerformed
	if (cmdtext[--i] == EOF) 
	{
		return OnPlayerCommandPerformed(playerid, cmdtext, 
			CallLocalFunction(function, !"ds", playerid, !"\1")
		);
	}
	return OnPlayerCommandPerformed(playerid, cmdtext, 
		CallLocalFunction(function, !"ds", playerid, cmdtext[i])
	);
#else
	if (cmdtext[--i] == EOF) 
	{
		return CallLocalFunction(function, !"ds", playerid, !"\1");
	}
	return CallLocalFunction(function, !"ds", playerid, cmdtext[i]);
#endif
}
#if defined _ALS_OnPlayerCommandText
	#undef OnPlayerCommandText
#else
	#define _ALS_OnPlayerCommandText
#endif
#if defined izcmd_OnPlayerCommandText
forward izcmd_OnPlayerCommandText(playerid, cmdtext[]);
#endif
#define	OnPlayerCommandText izcmd_OnPlayerCommandText
