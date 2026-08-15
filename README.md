# pwbot2
A simple bot for Perfect World 1.6.x

This "pwbot2" is an upgrade from a previous version "pwbot", which works in a newer version of PW.
This version also drops support for 32bit version of the game, as custom servers tend to drop it.

Tested at this server, which claims to be 1.6.x, shrunken down to 1.3.6: https://pwglobal.org/

What this bot does:
- acquires character info
- targets mobs by "TAB"
- kills mobs by "F1"
- grabs loot by "2"

FAQ:
Q: Why would you need a bot for PW 1.6.x which already has a bot built in?
A: Some custom servers block usage of the built-in bot, so an external bot should be handy. Especially when it has more features (still in TODO).

Q: This bot doesn't work!
A: 1) Make sure to launch the .exe as an administrator 
2) Make sure you game window is called "Perfect World"
3) Keep mind that the bot is made for and tested only at this server: https://pwglobal.org/

TODO:
- implement all in-game interactions by code injections. This should allow doing more complex behaviour, such as killing specific mobs, complex looting, bulding navigation routes, and running the bot in background
