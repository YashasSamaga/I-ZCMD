/*******************************************************************************************************************************
 ZCMD Improved (I-ZCMD)

 This include is just a re-make of the ZCMD Command Processor ( V0.3.1 31/10/2009) originnaly developed by Zeex.

 Version:0.2.2.2

 Credits:
 Yashas
 Zeex for the original include
*******************************************************************************************************************************/
#if defined _I_ZCMD_INCLUDED_V0_2_2_2_
	#endinput
#endif
#define _I_ZCMD_INCLUDED
#define _I_ZCMD_INCLUDED_V0_2_2_2_
/******************************************************************************************************************************/
#define MAX_CLIENT_MSG_LENGTH (144)

#define COMMAND:%1(%2)          \
			forward cmd_%1(%2); \
			public cmd_%1(%2)

#define CMD:%1(%2) \
			COMMAND:%1(%2)

#define command(%1,%2,%3) \
			COMMAND:%1(%2, %3)

#define cmd(%1,%2,%3) \
			COMMAND:%1(%2, %3)

#define CMD_SUCCESS 1
#define CMD_FAILURE 0

#if !defined isnull
        #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif
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
	#if defined OnPlayerCommandReceived
	    if(!OnPlayerCommandReceived(playerid,cmdtext)) return 1;
	#endif
	static funcname[MAX_CLIENT_MSG_LENGTH+4] = "cmd_";

	#if defined IZCMD_ENABLE_CASE_SENSITIVITY
		static pos;

		if((pos = strfind(cmdtext," ",false,1)) == -1)
		{
		    	pos = strlen(cmdtext);
	            	strmid(funcname[4],cmdtext,1,pos,MAX_FUNC_NAME-4);

		    	#if defined OnPlayerCommandPerformed
		    		return OnPlayerCommandPerformed(playerid,cmdtext,CallLocalFunction(funcname,"is",playerid,"\1"));
			#else
				return CallLocalFunction(funcname,"is",playerid,"\1");
			#endif
		}
		else
		{
		    	strmid(funcname[4],cmdtext,1,pos,MAX_FUNC_NAME-4);
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
	#else
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
	#endif
}
#if defined _ALS_OnPlayerCommandText
    #undef OnPlayerCommandText
#else
    #define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText izcmd_OnPlayerCommandText
forward izcmd_OnPlayerCommandText(playerid, cmdtext[]);
