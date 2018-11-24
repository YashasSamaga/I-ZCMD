# Improved ZCMD
**Latest Version: 0.2.3.0  (August 2016)**

ZCMD hasn't been updated for many years. A lot of advancements made in these years haven't been incorporated into ZCMD implementation. iZCMD is basically an updated version of v0.3.1 ZCMD. This include is neater and faster than ZCMD. It addresses some of the issues related to ZCMD and also allows you to toggle case-sensitivity.

## Changes:
- optimizations
- case sensitivity is togglable (compile-time)
- fixes crashes caused by large command names

## Switching from ZCMD to iZCMD
By default there is no change in *functionality* between iZCMD and ZCMD; all the changes affect the speed and efficiency of the script. The only new feature introduced in iZCMD is the ability to toggle case-sensitivity of commands (i.e. should `/pm` be treated as the same command as `/PM`). To allow case-sensitivity, define `IZCMD_ENABLE_CASE_SENSITIVITY` before including iZCMD.

**TL;DR:** replace your include header from `izcmd.inc` to `izcmd.inc`

## Tutorial

### Creating commands

You can create a command using any of the following styles. They are custom syntax defined in the include to abstract the implementation details of iZCMD. They are ultimately reduced to public functions by the preprocessor.

```
COMMAND:yourcommand(playerid, params[])
{
     return CMD_SUCCESS;
}
CMD:yourcommand(playerid, params[])
{
     return CMD_SUCCESS;
}
command(yourcommand, playerid, params[])
{
     return CMD_SUCCESS;
}
cmd(yourcommand, playerid, params[])
{
     return CMD_SUCCESS;
}
```

When a player types `/yourcommand parameters`, the command's handler will be called. The `playerid` parameter will have the id of the player who used the command and the `params[]` parameter will have the text that was typed excluding the command itself (for the given example, `params[]` will have `parameters`). You must return `CMD_SUCCESS` (or `1`) if the command was executed successfully; otherwise, return `CMD_FAILURE` (or `0`). Returning `CMD_FAILURE` would make server send the "Unknown Command" message unless handled in `OnPlayerCommandPerformed`.

Note that `params[]` will never be empty. If the player did not provide any arguments to the command, `params[0]` will be `'\1'`. Use the `isnull(string[])` macro provided with the include to check for nullity.

### OnPlayerCommandReceived

This callback is called everytime a player sends a command and before the command function is executed.

**Parameters:**
- `playerid` the player who used the command
- `cmdtext` text which the player typed (inclusive of the command delimiter `\` and the command)

**Return Values:**
- `1` - execute the command
- `0` - do not execute the command

```
public OnPlayerCommandReceived(playerid, cmdtext[])
{
	return 1;
}
```

### OnPlayerCommandPerformed
This callback is called after a command has been executed.

**Parameters:**
- `playerid` the player who used the command
- `cmdtext` text which the player typed (inclusive of the command delimiter `\` and the command)
- `success` is what the command function returned (`CMD_SUCCESS` or `CMD_FAILURE`)

**Return Values:**
- `0` or `CMD_FAILURE` - player will receive the "Unknown command" messsage
- `1` or `CMD_SUCCESS` - nothing happens

```
public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	return success;
}
```

If you are not using `OnPlayerCommandPerformed` then the the return value of the command function will be used as the return value of `OnPlayerCommandPerformed`.

### Case-Sensitivity
Case Sensitivity is disabled by default which means that `/pm` and `/PM` will be treated as the same command. Case-sensitivity can be enabled by defining `IZCMD_ENABLE_CASE_SENSITIVITY` before iZCMD is included.

## Tips & Tricks

### Execute commands manually

```cmd_yourcommand(playerid, params);```

You need to prefix "cmd_" to your command name to call the command function.

### Shortcut Commands

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

```
public OnPlayerCommandReceived(playerid,cmdtext[])
{
        if(!PlayerLoggedIn[playerid]) 
        {
               SendClientMessage(playerid, -1, "You need to log in to use commands");
               return 0;
        }
	return 1;
}
```

### iZCMD with *sscanf* is an efficient way to process commands

```
COMMAND:setskin(playerid, params[])
{
      new skinid;
      if(sscanf(params, "i", skinid)) SendClientMessage(playerid, -1, "Usage:/setskin [skinid]");
      else SetPlayerSkin(playerid, skin);
      return CMD_SUCCESS;
}
```
## Credits
- **Zeex** for the original ZCMD include: the concept and the algorithm
- Yashas for spending 60 minutes to update the include.
