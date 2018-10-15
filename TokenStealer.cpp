#include <windows.h>
#include <stdio.h>


int main (int argc, char *argv[]) {

    if (argc != 2) {
        //Print usage
        printf ("Usage: %s <pid>\n", argv[0]);
        return 2;
    }
    
    //Check if program has it's own console
    DWORD dwPid;
    HWND hwnd = GetConsoleWindow ();
    GetWindowThreadProcessId (hwnd, &dwPid);

    //The program needs to have it's own console. Otherwise, a weird bug occurs,
    //where there are multiple users in same console (if it is cmd, for example),
    //and they swap each time a command is executed.
    if (hwnd != NULL && dwPid != GetCurrentProcessId ()) {
        //If program does not have his own console, restart it with new console.
        ShellExecuteA (NULL, "open", argv[0], argv[1], NULL, SW_SHOW);
        return 3;
    }


    //Check if pid is PID is not too big/small or if it's not even string


    if (strlen (argv[1]) > 10) {
        printf ("PID is not valid!\n");
        getchar ();
        return 1;
    }

    INT64 toi = atoll (argv[1]), maxuint = 4294967295LL;
    DWORD pid, usrsess;

    if (toi > maxuint || toi < 0) {
        printf ("PID is either too big, or smaller than 0!\n");
        getchar ();
        return 1;
    }

    pid = (DWORD) toi;

    //Open process and its token, then duplicate the token
     
    HANDLE hProcess = OpenProcess (PROCESS_QUERY_INFORMATION, FALSE, pid);
    HANDLE hDummy, hToken;

    if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE) {
        printf ("Failed to get process! Error %i\n", GetLastError ());
        getchar ();
        return 1;
    }

    if (!OpenProcessToken (hProcess, TOKEN_DUPLICATE, &hDummy)) {
        printf ("Failed to get process token! Error %i\n", GetLastError ());        
        CloseHandle (hProcess);
        getchar ();
        return 1;
    }

    CloseHandle (hProcess);

    if (!DuplicateTokenEx (hDummy, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hToken)) {
        printf ("Failed to duplicate token! Error %i\n", GetLastError ());
        CloseHandle (hDummy);
        getchar ();
        return 1;
    }

    CloseHandle (hDummy);

    //Use the duplicated token to start a new process

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory (&si, sizeof(si));
    ZeroMemory (&si, sizeof(si));

    si.wShowWindow = SW_SHOW;
    si.cb = sizeof (STARTUPINFOA);

    if (!CreateProcessAsUserA (hToken, "C:\\Windows\\System32\\cmd.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf ("Failed to create process! Error %i\n", GetLastError ());
        CloseHandle (hToken);
        getchar ();
        return 1;
    }

    CloseHandle (hToken);

    printf ("Successfully started a new process!\n");
    getchar ();

    return 0;

}
