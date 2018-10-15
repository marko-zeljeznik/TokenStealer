# TokenStealer
This program will steal another program's token. 

Usage: TokenStealer.exe <pid>

pid = id of the process that the program will steal token from.


This program can also steal token of another logged-in user's process.
However, "NT AUTHORITY\SYSTEM" priviliges may be required.

If the program is successful, a new window should pop up. This window can execute standard cmd commands.
Keep in mind, if you attempt to start another program with command like "start taskmgr.exe", the program will 
start on the user, whoose token was stolen (at least in Windows 10).

