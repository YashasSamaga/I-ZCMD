# Improved ZCMD
**Latest Version: 0.2.3.0  (August 2016)**

ZCMD hasn't been updated in the last 6 years. A lot of advancements have been made in these years but the new knowledge hasn't been incorporated into ZCMD implementation. iZCMD is basically an updated version of v0.3.1 ZCMD include. This include is neater and faster than ZCMD. Improved ZCMD also addresses few issues/bugs with ZCMD and also allows you to toggle case-sensitivity.

## Changes:
- OnPlayerCommandReceived & OnPlayerCommandPerformed are now called directly instead of using CallLocalFunction
- removes OnGameModeInit/OnFilterScript hooks
- optimizations
- case sensitivity is togglable (compile-time)
- addresses crashes which are caused by large command names


## How to install?
For those who are already using ZCMD, you just need to replace ZCMD include with iZCMD include. There is no change in functionality (by default); all the changes affect the speed and efficiency of the script. The only new feature in iZCMD is that now case-sensitivity can be turned on/off by defining `IZCMD_ENABLE_CASE_SENSITIVITY` before including iZCMD. To maximize compatibility, iZCMD is not case-sensitive by default (so is ZCMD). 

For those who are not using ZCMD, you need to download iZCMD include and paste it in your include folder. All tutorials targetted at ZCMD are applicable to iZCMD since there is no change in functionality and syntax in iZCMD from ZCMD.

## How to use?

To create a command, all you need to do is create a public function using any of the given formats.

```
COMMAND:yourcommand(playerid,params[])
{
     return CMD_SUCCESS;
}
CMD:yourcommand(playerid,params[])
{
     return CMD_SUCCESS;
}
command(yourcommand,playerid,params[])
{
     return CMD_SUCCESS;
}
cmd(yourcommand,playerid,params[])
{
     return CMD_SUCCESS;
}
```

When a player types `/yourcommand parameters`, the function will be called. The `playerid` parameter will have the id of the player who used the command and the `params[]` parameter will have the text which the player typed after typing the command (for the given example, `params[]` will have `parameters`).

The `params` parameter will never be empty. If the player did not provide any parameters, `params[0` will be `'\1'`.

You must return `CMD_SUCCESS` (or 1) if the command was executed successfully; otherwise, return `CMD_FAILURE`. Returning `CMD_FAILURE` would make server send the "Unknown Command" message if the failure is not handled in `OnPlayerCommandPerformed`.
You can also use the ZCMD style of returning, i.e: 1 for success and any other value for failure.

You cannot use [OnPlayerCommandText](https://wiki.sa-mp.com/wiki/OnPlayerCommandText) with this include. It won't be called. There are two new callbacks instead.

### OnPlayerCommandReceived
This callback is called before the actual command function is called.

_Parameters:_
- `playerid` is the ID of the player who used the command
- `cmdtext` is text which the player typed


_Return Values:_
- 1 - command function will be called
- 0 - the command function won't be called.

```
public OnPlayerCommandReceived(playerid,cmdtext[])
{
	return 1;
}
```

### OnPlayerCommandPerformed
This callback is called after the command function is executed.

_Parameters:_
- payerid is the ID of the player who used the command
- cmdtext is text which the player typed
success is what the command function returned (CMD_SUCCESS or CMD_FAILURE)

_Return Values:_
- 0 or `CMD_FAILURE` - Player will see the Error Message, i.e "Unknown command"
- 1 or `CMD_SUCCESS` - The error message won't be sent

```
public OnPlayerCommandPerformed(playerid,cmdtext[], success)
{
	return success;
}
```

If you are not using `OnPlayerCommandPerformed` then what you return in your command function will decide if the error message will be sent or not.

Returning 0 or `CMD_FAILURE` in the command function means the error message will be sent.
Returning 1 or `CMD_SUCCESS` in the command function means the error message won't be sent.

### Case-Sensitivity
Case Sensitivity is disabled by default which means that `/pm` and `/PM` will be treated as the same command. Case-sensitivity can be enabled by defining `IZCMD_ENABLE_CASE_SENSITIVITY` before iZCMD is included.

## Tips & Tricks

### Calling command functions manually
You can call a command function using the following code.

```cmd_yourcommand(playerid,params);```

You need to prefix "cmd_" to your command to call the command function.

### Shortcut Commands
You can make shortcut commands using the idea given below.

```
COMMAND:arrest(playerid,params[])
{
     //your arrest code
     return CMD_SUCCESS; 
}
COMMAND:ar(playerid,params[])
{
     return cmd_arrest(playerid,params);
}
```

### Disable Commands if player is not logged in
You can disable commands for a player who hasn't logged in using the following idea.

```
public OnPlayerCommandReceived(playerid,cmdtext[])
{
        if(!PlayerLoggedIn[playerid]) 
        {
               SendClientMessage(playerid,-1,"You need to log in to use commands");
               return 0;
        }
	return 1;
}
```

### iZCMD with *sscanf* is an efficient way to process commands

```
COMMAND:setskin(playerid,params[])
{
      new skinid;
      if(sscanf(params,"i",skinid)) SendClientMessage(playerid,-1,"Usage:/setskin [skinid]");
      else SetPlayerSkin(playerid, skin);
      return CMD_SUCCESS;
}
```

## Examples
```
COMMAND:getvid(playerid,params[])
{
		new id,string[144],vid;
		if(sscanf(params,"u",id))
		{
			if(IsPlayerInAnyVehicle(playerid))
			{
			    vid = GetPlayerVehicleID(playerid);
			    format(string,sizeof(string),"The vehicle ID of your vehicle is %d.Use/getvid [Name/ID] to get vehicle ID of other player's vehicle.",vid);
				return SendClientMessage(playerid,-1,string);
		 	}
			return SendClientMessage(playerid,-1,"Your not in a vehicle nor you have specified any player from which to get the vehicle id.Use /getvid [Name/ID] to get the vehicle ID of their vehicle.");
	 	}
		if(IsPlayerConnected(id))
		{
		    if(IsPlayerInAnyVehicle(id))
		    {
                        new pName[MAX_PLAYER_NAME];
                        GetPlayerName(playerid,pName,MAX_PLAYER_NAME);
		        vid = GetPlayerVehicleID(id);
		        format(string,sizeof(string),"The vehicle ID of the vehicle which %s(%d) is using is %d.",pName,id,vid);
				SendClientMessage(playerid,-1,string);
			}
		    else
		    {
		        SendClientMessage(playerid,-1,"The given player is not using any vehicle.");
		    }
		}
		else { return SendClientMessage(playerid,-1,"Usage:/getvid:Invalid Player ID"); }
	}
	return CMD_SUCCESS;
}
```

## Credits
- **Zeex** for the original ZCMD include: the concept/algorithm
- Yashas for spending 60 minutes to update the include.
