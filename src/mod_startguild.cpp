/*

# Starting Guild #

#### A module for AzerothCore by [StygianTheBest](https://github.com/StygianTheBest/AzerothCore-Content/tree/master/Modules)
------------------------------------------------------------------------------------------------------------------


### Description ###
------------------------------------------------------------------------------------------------------------------
This module automatically joins new players to a guild of your choice on first login.


### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: Player/Server
- Script: StartGuild
- Config: Yes
    - Enable Module
    - Enable Module Announce
    - Set ID for Horde and Alliance Guilds
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2017.08.01
- v2017.08.02 - Fix crash prevention when no guilds exist


### Credits ###
------------------------------------------------------------------------------------------------------------------
- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [AzerothCore Discord](https://discord.gg/gkt4y2x)
- [EMUDevs](https://youtube.com/user/EmuDevs)
- [AC-Web](http://ac-web.org/)
- [ModCraft.io](http://modcraft.io/)
- [OwnedCore](http://ownedcore.com/)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [AoWoW](https://wotlk.evowow.com/)


### License ###
------------------------------------------------------------------------------------------------------------------
- This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/


#include "ScriptMgr.h"
#include "Player.h"
#include "GuildMgr.h"
#include "Configuration/Config.h"
#include "Chat.h"

#define Welcome_Name "Notice"

class StartGuild : public PlayerScript
{

public:
	StartGuild() : PlayerScript("StartGuild") { }

	// Announce Module
	void OnLogin(Player *player)
	{
		if (sConfigMgr->GetOption<bool>("StartGuild.Enable", true))
		{
			if (sConfigMgr->GetOption<bool>("StartGuild.Announce", true))
			{
				ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StartGuild |rmodule.");
			}

            		uint8 level = sConfigMgr->GetOption<uint8>("StartGuild.Level", 0);
            		if (level > 0 && player->getLevel() == level && !player->GetGuild()) {
                		addPlayerToGuild(player);
            		}
		}
	}

	// Start Guild
	void OnFirstLogin(Player* player)
	{
		// If enabled...
		if (sConfigMgr->GetOption<bool>("StartGuild.Enable", true) && sConfigMgr->GetOption<uint8>("StartGuild.Level", 0) == 0)
		{
        	        addPlayerToGuild(player);
		}
	}

	void addPlayerToGuild(Player* player) {
		const uint32 GUILD_ID_ALLIANCE = sConfigMgr->GetOption<uint8>("StartGuild.Alliance", 0);
		const uint32 GUILD_ID_HORDE = sConfigMgr->GetOption<uint8>("StartGuild.Horde", 0);

		Guild* guild = sGuildMgr->GetGuildById(player->GetTeamId() == TEAM_ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

		// If a guild is present, assign the character to the guild; otherwise skip assignment.
		if (guild)
		{
			guild->AddMember(player->GetGUID());

			// Inform the player they have joined the guild
			std::ostringstream ss;
			ss << "Welcome to the " << player->GetGuildName() << " guild " << player->GetName() << "!";
			ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
		}
    }
};

void AddStartGuildScripts()
{
	new StartGuild();
}

