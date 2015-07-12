/*******************************************************************************************************************************
 ZCMD Improved (I-ZCMD)

 This include is just a re-make of the ZCMD Command Processor ( V0.3.1 31/10/2009) originnaly developed by Zeex.

 Version:0.3

 Credits:
 Yashas
 Zeex for the original include
*******************************************************************************************************************************/
#if defined _I_ZCMD_INCLUDED_V0_3_0_
	#endinput
#endif
#define _I_ZCMD_INCLUDED_V0_3_0_
#define _I_ZCMD_INCLUDED
/******************************************************************************************************************************/
#if !defined MAX_COMMANDS
	#define MAX_COMMANDS 512
#endif

#define MAX_FUNC_NAME 32

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
/******************************************************************************************************************************/
#if defined OnPlayerCommandReceived
	forward OnPlayerCommandReceived(playerid,cmdtext[]);
#endif
#if defined OnPlayerCommandPerformed
	forward OnPlayerCommandPerformed(playerid,cmdtext[], success);
#endif
/******************************************************************************************************************************/
static izcmd_pointer_OPCP = 0;

static izcmd_start_index[59] =
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
static izcmd_end_index[59] =
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
static izcmd_command_count = 0;

static izcmd_commands[MAX_COMMANDS][MAX_FUNC_NAME - 4 char];
static izcmd_command_func_address[MAX_COMMANDS];
/******************************************************************************************************************************/
static izcmd_Init()
{
	new offsetDAT, offsetPFT, offsetNFT, addr, number_of_publics;
	new prefix[4] = {'c','m','d','_'},funcname[MAX_FUNC_NAME-4];
	new tmp[32 char],chr;

	if((addr = funcidx("OnPlayerCommandPerformed")) != -1)
	{
	    #emit LCTRL       1
		#emit NEG
		#emit ADD.C       32
		#emit STOR.pri  izcmd_pointer_OPCP
		#emit LREF.alt  izcmd_pointer_OPCP

		#emit LCTRL 1
		#emit NEG
		#emit ADD
		#emit LOAD.S.alt  addr
		#emit SHL.C.alt   3
		#emit ADD
		#emit STOR.pri  izcmd_pointer_OPCP
		#emit LREF.pri  izcmd_pointer_OPCP
		#emit STOR.pri  izcmd_pointer_OPCP
   	}

   	#emit LCTRL 1
    #emit NEG
    #emit STOR.S.PRI offsetDAT

    #emit ADD.C 32
    #emit STOR.S.PRI addr
    #emit LREF.S.PRI addr
    #emit LOAD.S.ALT addr
    #emit ADD
    #emit CONST.ALT 32
    #emit SUB
    #emit STOR.S.PRI offsetPFT

    #emit LOAD.S.PRI offsetDAT
	#emit ADD.C 36
    #emit STOR.S.PRI addr
    #emit LREF.S.PRI addr
    #emit LOAD.S.ALT addr
    #emit ADD
    #emit CONST.ALT 36
    #emit SUB
    #emit STOR.S.PRI offsetNFT

    number_of_publics = ((offsetNFT - offsetPFT) >>> 3);

    for(new i = 0; i < number_of_publics; i++)
	{
	    new j;
        addr = (i << 3) + offsetPFT + 4;

        #emit LREF.S.pri addr
        #emit LOAD.S.alt offsetDAT
        #emit ADD
        #emit STOR.S.pri addr

		for(;;)
		{
          	#emit LREF.S.pri addr
            #emit STOR.S.pri chr

			#emit LOAD.S.pri addr
			#emit INC.pri
			#emit STOR.S.pri addr

            if(j < 4)
			{
				if((chr & 0xFF) != prefix[j++])
					break;
			}
			else if((funcname[j++-4] = chr & 0xFF) == EOS)
			{
   				if(izcmd_start_index[funcname[0]-64] == -1)
			        izcmd_start_index[funcname[0]-64] = izcmd_command_count;

                izcmd_end_index[funcname[0]-64] = izcmd_command_count+1;

			    #emit LCTRL       1
				#emit NEG
				#emit ADD.C       32
				#emit STOR.S.pri  addr
				#emit LREF.S.alt  addr

				#emit LCTRL 1
				#emit NEG
				#emit ADD
				#emit LOAD.S.alt  i
				#emit SHL.C.alt   3
				#emit ADD
				#emit STOR.S.pri  addr
				#emit LREF.S.pri  addr
				#emit STOR.S.pri  addr

				izcmd_command_func_address[izcmd_command_count] = addr;
				strpack(izcmd_commands[izcmd_command_count++],funcname,7);
				break;
			}
		}
	}

	for(new n = 0; n < 59; n++)
	{
	    if(izcmd_start_index[n] == -1) continue;

	   	new bool:swaped = false;
     	do
	    {
	        for(new j = izcmd_start_index[n],m = izcmd_end_index[n] - izcmd_start_index[n];j < (m-1);j++)
	        {
				for(new k = 0;k < (MAX_FUNC_NAME-4);k++)
				{
				    if(izcmd_commands[j][k] < izcmd_commands[j+1][k]) break;
	                else if(izcmd_commands[j][k] > izcmd_commands[j+1][k])
	                {
	                    swaped = true;

	                    tmp[0] = 0;
						strcat(tmp,izcmd_commands[j]);

						izcmd_commands[j][0] = 0;
						strcat(izcmd_commands[j],izcmd_commands[j+1]);

						izcmd_commands[j+1][0] = 0;
						strcat(izcmd_commands[j+1],tmp);
	                    break;
	                }
				}
			}
	    }while(swaped);
	}
}
/******************************************************************************************************************************/
#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
	   	izcmd_Init();
	    #if defined izcmd_OnFilterScriptInit
			return izcmd_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}

	#if defined _ALS_OnFilterScriptInit
	    #undef OnFilterScriptInit
	#else
	    #define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit izcmd_OnFilterScriptInit

	#if defined OnFilterScriptInit
		forward OnFilterScriptInit();
	#endif
#else
	public OnGameModeInit()
	{
	    izcmd_Init();
	    #if defined izcmd_OnGameModeInit
			return izcmd_OnGameModeInit();
		#else
			return 1;
		#endif
	}

	#if defined _ALS_OnGameModeInit
	    #undef OnGameModeInit
	#else
	    #define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit izcmd_OnGameModeInit

	#if defined izcmd_OnGameModeInit
	forward izcmd_OnGameModeInit();
	#endif
#endif
/******************************************************************************************************************************/
public OnPlayerCommandText(playerid, cmdtext[])
{
    #if defined OnPlayerCommandReceived
	    if(!OnPlayerCommandReceived(playerid,cmdtext)) return 1;
	#endif

	static packed_cmd_name[MAX_FUNC_NAME - 4 char],nullstr[] = "",min_char_checks,start,end,pos,tmp1,tmp2;

	if((start = izcmd_start_index[(tmp1 = cmdtext[1] - 64)]) == -1)
	{
	    #if !defined OnPlayerCommandPerformed
		switch(false)
		{
		    case true:
		    {
        #endif
             		#emit PUSH.C 0
			  		#emit PUSH.S cmdtext
					#emit PUSH.S playerid
					#emit PUSH.C 12

					#emit LCTRL      6
					#emit ADD.C      28
					#emit PUSH.pri

					#emit LOAD.pri izcmd_pointer_OPCP
					#emit SCTRL      6

					#emit RETN

    	#if !defined OnPlayerCommandPerformed
			    }
			}
		#endif

			#emit ZERO.pri
			#emit RETN
	}

	new k = 0, i = (start + (end = izcmd_end_index[tmp1]))>>1;

    if( (pos = (strfind(cmdtext[1]," ",false,1) + 1)) )	cmdtext[pos] = 0;
    
	min_char_checks = strpack(packed_cmd_name,cmdtext[1])>>2;

    loop:
	if((tmp1 = izcmd_commands[i][0]) < (tmp2 = packed_cmd_name[0]))
	{
	    i = (end + (start = i+1))>>1;
	}
	else if(tmp1 > tmp2)
	{
	    i = ((end = i-1) + start)>>1;
	}
	else
	{
		while(k++ < min_char_checks)
		{
			if((tmp1 = izcmd_commands[i][k]) < (tmp2 = packed_cmd_name[k]))
			{
			    i = (end + (start = i+1))>>1;
			    k = 0;
			    if(start > end) goto cmd_invalid;
			    goto loop;
			}
			else if(tmp1 > tmp2)
			{
			    i = ((end = i-1) + start)>>1;
			    k = 0;
			    if(start > end) goto cmd_invalid;
			    goto loop;
			}
		}

	    switch(pos)
	    {
	        case 0:
			{
				#emit PUSH.C nullstr
			}
			default:
			{
				#emit LOAD.S.alt cmdtext
		        #emit LOAD.pri pos
		        #emit ADD.C 1
		        #emit IDXADDR

			    #emit PUSH.pri
			}
	    }
	    //COMPILER BUG FIX
	    {
       		#emit PUSH.S playerid
			#emit PUSH.C 8
			#emit LCTRL      6
			#emit ADD.C      40
			#emit PUSH.pri

			#emit CONST.alt izcmd_command_func_address
			#emit LOAD.S.pri i
			#emit LIDX

			#emit SCTRL      6

            #emit PUSH.pri

			cmdtext[pos] = ' ';
			
			#if !defined OnPlayerCommandPerformed
		    switch(false)
			{
			    case true:
			    {
            #endif
			  		#emit PUSH.S cmdtext
					#emit PUSH.S playerid
					#emit PUSH.C 12

					#emit LCTRL      6
					#emit ADD.C      28
					#emit PUSH.pri

					#emit LOAD.pri izcmd_pointer_OPCP
					#emit SCTRL      6

					#emit STACK 8
					#emit RETN

    		#if !defined OnPlayerCommandPerformed
			    }
			}
			#endif

            #emit POP.pri
            #emit STACK 8
			#emit RETN
		}
	  	return 1;
	}
	if(start > end)
    {
        cmd_invalid:
 		#if !defined OnPlayerCommandPerformed
	    switch(false)
		{
		    case true:
		    {
        #endif
                cmdtext[pos] = ' ';

                #emit PUSH.C 0
		  		#emit PUSH.S cmdtext
				#emit PUSH.S playerid
				#emit PUSH.C 12

				#emit LCTRL      6
				#emit ADD.C      28
				#emit PUSH.pri

				#emit LOAD.pri izcmd_pointer_OPCP
				#emit SCTRL      6

				#emit STACK 4
				#emit RETN

    	#if !defined OnPlayerCommandPerformed
		    }
		}
		#endif

		#emit STACK 4
		#emit RETN
	}
    //k = 0;
	goto loop;

	//COMPILER BUG FIX
	return 1;
}
/******************************************************************************************************************************/
