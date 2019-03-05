# WoWDesktop
An old example of how to run World of Warcraft in its own unprivileged desktop.

Back in 2005, it was revealed that World of Warcraft was downloading obfuscated
binaries that spied on other processes, ostenstibly to look for cheating
software. (http://news.bbc.co.uk/2/hi/technology/4385050.stm) This code, called
"The Warden," would both collect window titles as well as contents of other
processes' memory.

As it turns out, Windows has a bunch of isolation mechanisms. In particular, 
Windows has the concept of "Windows Stations," which have one or more
"Desktops," which can be assigned different permissions and run programs by
different users. In fact, the default Window Station (WinSta0, the
"Interactive Window Station") typically has three desktops -- one for the
current user, one for the screen saver, and one for WinLogin.

This program creates a new Desktop and grants the unprivileged WoW account
access. It then runs WoW as the unprivileged WoW account and switches to that
desktop, waits for WoW to exit, and then switched back to the normal user's
desktop.

Running under a separate desktop means that no other windows can be enumerated
or accessed. Running as an unprivileged user means that other processes cannot
be enumerated or have their memory read. It's as if the only thing running on
the system was WoW itself.

This repo is for historical interest only. It is not guaranteed to be secure.
There is no guarantee that not being able to see anything but yourself wouldn't
be considered suspicious. Use at your own risk, and definitely not for cheating.

World of Warcraft and Warcraft are trademarks or registered trademarks of
Blizzard Entertainment, Inc., in the U.S. and/or other countries, which is 
not associated with this project in any way.