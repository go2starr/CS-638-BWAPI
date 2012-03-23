Code Download and Build Instructions
We have tagged a version of our code that can be downloaded here:
https://github.com/go2starr/CS-638-BWAPI/tags

Required Software

    StarCraft Brood War 1.16.1
    Microsoft Visual C++ 2008, the express edition of Microsoft Visual C++ is freely available
    Native Microsoft Windows XP/Vista (Windows 7 seems usable as well)
        Broodwar uses DLL injection and therefore is unlikely to work in Wine or a virtual windows environment
        Windows 7 support has not been tested
        Mac is not supported
    Seting up Broodwar
        Install StarCraft, the Brood War expansion, and upgrade to version 1.16.1. Logging on to battlet.net will automatically cause StarCraft to upgrade to the most recent patch
        Install Visual C++
        Extract the Chaos (http://www.google.com/url?q=http%3A%2F%2Feis.ucsc.edu%2Fsites%2Fdefault%2Ffiles%2Fchaoslauncher.zip&sa=D&sntz=1&usg=AFQjCNF0RmMC-vHZ2RzRP60-lD2tXk-yNg)Launcher to: C:\program files\starcraft\chaoslauncher
        Update your Chaos Launcher by connecting your StarCraft to Battle.net
        Extract the newest CS-638-BWAPI (http://www.google.com/url?q=https%3A%2F%2Fgithub.com%2Fgo2starr%2FCS-638-BWAPI%2Fzipball%2Fmaster&sa=D&sntz=1&usg=AFQjCNGg7ycnmimRwvO58pj4CsSmfTt2iQ) release to: C:\program files\starcraft\CS-638-BWAPI
        Copy files from C:\program files\starcraft\CS-638-BWAPI \chaoslauncher to C:\program files\starcraft\chaoslauncher
        Copy files from C:\program files\starcraft\CS-638-BWAPI\Starcraft to C:\program files\starcraft
        Copy files from C:\program files\starcraft\CS-638-BWAPI\WINDOWS to C:\WINDOWS
            This step might not be necessary on all platforms. If you are unsure about throwing DLL files into your windows directory, you can try skipping this step.
        Compiling the Bot
            Open the example AI project C:\program files\starcraft\CS-638-BWAPI\BasicAIModule\BasicAIModule.sln
            Compile the project as a RELEASE.
                Change the dropdown box in the top center of the window from ¡°Debug¡± to ¡°Release¡±.
                Click Build ¡ú Build Solution.
                The example bot is now compiled into a dll
                The bot dll is located at C:\program files\starcraft\CS-638-BWAPI\BasicAIModulee\release\BasicAIModule.dll
            Copy the dll from C:\program files\starcraft\CS-638-BWAPI\BasicAIModule\Release\BasicAIModule.dll to C:\program files\starcraft\bwapi-data\AI\BasicAIModule.dll
        Running the Bot
            Run the chaos launcher: C:\program files\starcraft\chaoslauncher\Chaoslauncher.exe
            set the path to starcraft, under the settings tab, set the path to: C:\program files\starcraft
            select the bwapi checkbox, uncheck all others
                select the w-mode checkbox if you want to run starcraft in windowed mode\
            For the first time running Chaoslauncher you need to edit the config file so that it knows the path to the dll.  Highlight the BWAPI Injector (1.16.1) and then click the config button.  This brings up a text file config that you can edit then save and close.  Change the ai variable to this: ai     = bwapi-data\AI\BasicAIModule.dll
            click start to launch starcraft
            Then click on Multiplayer->StarCraft: Brood War expansion.  Then in Select Connection, select either Local PC or Local Area Network (UDP).  Then create a new Registry ID, the name doesn¡¯t matter. Then create a game, select the Astral Balance map, set the first name that you created (ex basicaimodule) to the Terran race.  This is done by holding down the left mouse button and then moving to the Terran, then release.  Then do the same thing for ¡°Open¡±, but select ¡°Computer¡±, and then ¡°Random¡± is fine for it¡¯s race or this can also be selected. Then click ¡°Ok¡± to start the match.  Now you are playing from the view point of our AI Bot. The match will run until the other player is eliminated or our bot chooses to surrender
            The agent should print out the following message:
                BWAPI: Loaded the AI Module: bwapi-data\AI\BasicAIModule.dll
            The bot will print a summary of the game state to the console every few seconds
            If the bot does not start up, check the log files at C:\program files\starcraft\bwapi-data\log or check the FAQ

