# UE4-Apk-Dumper
An Internal app which can be implemented into game apk and used to generate Sdk Dumps without ROOT.

# Does it need Root Access?
NO, it doesn't need Root Privileges.

# How to Build?
1. Clone the Repo 
2. Open in Android Studio and Press Trust this Project if prompted
3. Update only Offsets in Offsets.h
4. Compile the App and generate Apk
5. Done

Note - Like Ue4Dumper you dont need to specify Game Package Name, Only update offsets to Dump

# How to Use?
Download Pre Dumped SDKs - https://github.com/Real-Ascarre/UE4-Apk-Dumper/tree/master/Dumped%20SDKs/

1. Copy the Library file in the correct Game Libs folder (Shared Libraries) - Currently Supported - 32Bit & 64Bit
2. Rename the dex (dumper apk) to the next number which the game has, For example - If the game has 3 dex file named as classes.dex, classes2.dex & classes3.dex then rename our apk dex as classes4.dex and put inside the game
3. Open MainActivity file of the Game, For Example - BGMI's MainActivity where we can put our code is "SplashActivity" under "onCreate" Function.
4. Now after findind the file put this line - "invoke-static {p0}, Lashope/tech/generator/MainActivity;->CheckOverlayPermission(Landroid/content/Context;)V" under "onCreate" Function. (NOTE - Put this line right before the "onCreate" string inside the "onCreate" Function)
5. Save the Dex file and open our dex file that we copied inside the Game and Delete the "DeleteActivity" file and save the apk
6. Edit the manifest file and add permission for Manage all files specifically this line - "<uses-permission android:name="android.permission.MANAGE_EXTERNAL_STORAGE" />"
7. Now Sign the Apk (NOTE - You need to Kill SignVerification if there is any Checks)
8. Give Permissionn for All Files access, not just to Media file permission.
9. Open Game then Select as per Game and Version then turn on the Dumping Options, for Pubg turn on PubgM series option.
10. Dump will be genrated in /sdcard/DumpedGames/'Game Package Name'

# How to Get the Dump files?
If you followed the Tutorial on "# How to Use?" properly then a Directory should appear on your sdcard (same place where "Download" folder is present) named as "DumpedGames" and inside should be another folder named as the game Packagename, For Example - For Game BGMI there should be "com.pubg.imobile" folder inside "DumpedGames" folder. Now every Dump should be Present inside their Respective folder.

# Am Noob and Help me, Need Video Tutorial
Kill Apk signature Verification and follow this - https://youtu.be/cDEUkwToms4

# ChangeLogs
- v0.1: First Release with Support for No-Root

- v0.2:
- 1) Added Support for Unreal Engine 4.23+ Version
- 2) Added Seperate Option for 4.23+ Versions
- 3) Added Seperate Option for PubgM Series (Pubg Lite and New State Not Supported Yet)
- 4) Added Base Offsets on Dump to access Easily.
- 5) Fixed NamesDump for 64Bit.
- 6) Fixed Project Issue for Others

# To-Do
1. Add support to Dump Specific Offsets only.
2. Add support to dump and regenerate Libs.
3. Generate Internal Sdk Structures or Internal SDK.
4. Add Support for UE3 and many other UE4 Games.
5. Dump Calculted Offsets like ComponentToWorld and other Offsets.

# Want to Support me?
Join my Telegram Channel - https://t.me/ascarrehacks
