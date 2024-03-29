#define SAMSARA_ENTER               611
#define SAMSARA_RETURN              613
#define SAMSARA_RESPAWN             618
#define SAMSARA_DEATH               619
#define SAMSARA_OPEN                623
#define SAMSARA_SPAWN               624
#define SAMSARA_DISCONNECT          617
#define SAMSARA_WOLFMOVE            673
#define SAMSARA_SCHEDULED           674
#define SAMSARA_PDWTAK              675
#define SAMSARA_GETSETTINGS         676

#define SAMSARA_CONFIRMCLASS        206
#define SAMSARA_DECORATE            215
#define SAMSARA_GIVEWEAPON          229
#define SAMSARA_GIVEUNIQUE          231
#define SAMSARA_CLIENT_WEAPONPICKUP 616
#define SAMSARA_CLIENT_UNIQUEPICKUP 633
#define SAMSARA_MARATHON            609
#define SAMSARA_QUAKE               606
#define SAMSARA_RESONATE            608
#define SAMSARA_SYNTHFIRE           607
#define SAMSARA_MEGAHEALTH          259
#define SAMSARA_QPOWERS             636

#define SAMSARA_TIPBOX              300
#define SAMSARA_TIPBOX_CLIENT       301

#define SAMSARA_PUKE                226
#define SAMSARA_PUKE_CLIENT         227
#define SAMSARA_OPEN_CLIENT         548
#define SAMSARA_ENTER_CLIENT        521
#define SAMSARA_DISCONNECT_CLIENT   522
#define SAMSARA_CLIENT_CLASS        228
#define SAMSARA_RECOIL              312
#define SAMSARA_CLIENT_DECORATE     212

#define SAMSARA_STRIFEACCURACY      313
#define SAMSARA_CLIENT_ACCURACY     314
#define SAMSARA_BEACON              315
#define SAMSARA_SPECTRES            316
#define SAMSARA_SIGIL               317

#define SAMSARA_CL_VERSION          2601

#define CLASSCOUNT          8
#define UNIQUECOUNT         2
#define SLOTCOUNT           10
#define TIPCOUNT            2

#define LMSMODES            6
#define ARMORMODES          5

#define CLASS_DOOM      0
#define CLASS_CHEX      1
#define CLASS_HERETIC   2
#define CLASS_WOLFEN    3
#define CLASS_HEXEN     4
#define CLASS_DUKE      5
#define CLASS_MARATHON  6
#define CLASS_QUAKE     7

#define SPEED_FORWARD       15
#define SPEED_SIDE          13

#define UNLOADCOUNT 62

#define P_COUNT 2
#define P_QUAD  0
#define P_REGEN 1

#define QUAD_THRESHOLD      2100   // one minute

#define PY640   380.0
#define PY320   (PY640/2)

#define POFF640 40.0
#define POFF320 20.0

#define REGEN_CENTER_X      0.953125    // 610 / 640
#define REGEN_CENTER_Y      (PY640 / 480)
#define REGEN_OFFSET        0.083333    // 40 / 480

#define TIP_SCROLLRATE      8

#define BLOODYHELL_HAPPYFUN		-1
#define BLOODYHELL_VANILLA		0
#define BLOODYHELL_NASHGORE		1
#define BLOODYHELL_BRUTAL		2
#define BLOODYHELL_NIGHTMARE	3

#define GLOBAL_DONEBITCHING     0
#define GLOBAL_SIGILBASE        1

int HELPSTR = 
"Welcome to the Wheel of Samsara! There are a few optional RCon commands you may want to consider.\n\
samsara_banjetpack 0/1: Toggles whether Duke is allowed to pick up his jetpack or not.\n\
samsara_banwolfmove 0/1: Toggles whether Wolfenstein movement is allowed or not.\n\
samsara_nocustomgravity 0/1: Toggles whether the heroes have custom gravity or use only Doom gravity.\n\
samsara_jumpmod: Gives the indicated multiplier to jump height. 9 for HeXen, -8 for Strife.\n\
samsara_permault 0/1: Toggles whether the Weapon VII is persistent or vanishes on pickup.\n\
samsara_lmslife [0-5]: Affects how much health/armor people have on LMS spawn/respawn.\n\
samsara_lmsult 0/1: Toggles whether players get their VII in LMS.\n\
samsara_uniquestart [0-4]: Toggles whether players get their unique on enter or spawn.\n\
samsara_chainsawstart [0-2]: Toggles whether players get their I on enter or spawn.\n\
samsara_peoplediewhentheyarekilled [0-3]: Players explode on death.\n\
samsara_punchdrunk 0/1: Toggles melee-only mode. Only works in deathmatch or coop.\n\
samsara_punchdrunkuniques 0/1: Toggles whether Punchdrunk uniques are present in normal play.\n\
samsara_punchdrunksaws 0/1: Toggles whether Punchdrunk Chainsaws are present in normal play.\n\
samsara_armormode [0-4]: Toggles the type of armor that spawns in-game.\n\
samsara_nohealthcap 0/1: Toggles whether 100/200 is the health cap for players, or infinity.\n\
samsara_noult 0/1: Toggles whether the VII comes into play or never spawns.\n\
samsara_nomonologues 0/1: Toggles whether the bosses speak on spawn or not.\n\
samsara_backpackstart 0/1: Toggles whether players spawn normally or with backpacks.\n\
samsara_classiclaz 0/1: Toggles whether the LAZ Device has Samsara behavior or original behavior.\n\
samsara_allcanrj 0/1: Every character with non-self-damaging rockets will be able to RJ with them.\n\
samsara_nounique 0/1: Toggles whether Unique Item spawns.\n\
samsara_noinvuln 0/1: Toggles whether Invulnerability spawns.\n\
samsara_ban[class] 0/1: Forbids players from playing certain classes. bandoomguy for Doomguy, banchex for Chex Warrior, etc.\n\
samsara_lmsrules 0/1: For players who want Rocket Arena-style showdowns or just to spawn with all weapons.\n\
samsara_[class]damage -10-30: Adjusts the character's damage multiplier.\n\
samsara_[class]defense -10-20: Adjusts the character's defense multiplier.\n\
samsara_permaquad: Determines whether Ranger gets a Quake II or Quake I Quad Damage on picking up the VII.";

int HELPSTR_CL = 
"Likewise, there are several clientside console variables, including:\n\
samsara_cl_printpickup 0/1: 1 = Print, 0 = Log.\n\
samsara_cl_expparticles [1-100000]: How many particles does Ranger's explosions emit? 0 is default, -1 to disable.\n\
samsara_cl_norecoil 0/1: Controls whether Ranger's weapons recoil.\n\
samsara_cl_sinerecoil 0/1: Toggles whether Ranger's recoil movement uses a sine wave.\n\
samsara_cl_weaponhud 0/1: Controls whether the weapon bar at the top of your screen appears.\n\
samsara_cl_smoothanims 0/1: Toggles the vanilla weapon animations for Doomguy.\n\
samsara_cl_wolfmove 0/1: Toggles classic Wolfenstein movement on B.J. Blazkowicz.";

int LMSArmors[LMSMODES] = 
{
    "",
    "GreenArmor",
    "BlueArmor",
    "LMSArmor", 
    "LMSArmor2",
    "LMSArmor3",
};

int ClassItems[CLASSCOUNT] =
{
    "DoomguyClass",
    "ChexClass",
    "CorvusClass",
    "WolfenClass",
    "HexenClass",
    "DukeClass",
    "MarathonClass",
    "QuakeClass",
};

int PickupStates[CLASSCOUNT][8] = 
{
    {"Doomguy", "DoomFull",     "DoomEmpty",        "DoomSpecial",      "DoomPunchdrunk",      "DoomPDFull",       "DoomPDEmpty",      "DoomPDSpecial"},
    {"Chexguy", "ChexFull",     "ChexEmpty",        "ChexSpecial",      "ChexPunchdrunk",      "ChexPDFull",       "ChexPDEmpty",      "ChexPDSpecial"},
    {"Heretic", "HereticFull",  "HereticEmpty",     "HereticSpecial",   "HereticPunchdrunk",   "HereticPDFull",    "HereticPDEmpty",   "HereticPDSpecial"},
    {"Wolfguy", "WolfFull",     "WolfEmpty",        "WolfSpecial",      "WolfPunchdrunk",      "WolfPDFull",       "WolfPDEmpty",      "WolfPDSpecial"},
    {"Hexen",   "HexenFull",    "HexenEmpty",       "HexenSpecial",     "HexenPunchdrunk",     "HexenPDFull",      "HexenPDEmpty",     "HexenPDSpecial"},
    {"Dukeguy", "DukeFull",     "DukeEmpty",        "DukeSpecial",      "DukePunchdrunk",      "DukePDFull",       "DukePDEmpty",      "DukePDSpecial"},
    {"Marathon","MarathonFull", "MarathonEmpty",    "MarathonSpecial",  "MarathonPunchdrunk",  "MarathonPDFull",   "MarathonPDEmpty",  "MarathonPDSpecial"},
    {"Quake",   "QuakeFull",    "QuakeEmpty",       "QuakeSpecial",     "QuakePunchdrunk",     "QuakePDFull",      "QuakePDEmpty",     "QuakePDSpecial"},   
};

int ItoSArray[7] = {1, 3, 4, 5, 6, 7, 8};
int StoIArray[SLOTCOUNT] = {-1, 1, -1, 2, 3, 4, 5, 6, 7, -1};

int ClassFades[CLASSCOUNT][5] =
{
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {0,   255, 0,   0.4, 8},
    {255, 255, 0,   0.1, 5},
};

int SlotItems[SLOTCOUNT] = 
{
    "",
    "GotWeapon0",
    "",
    "GotWeapon2",
    "GotWeapon3",
    "GotWeapon4",
    "GotWeapon5",
    "GotWeapon6",
    "GotWeapon7",
    "",
};

int LMSItems[CLASSCOUNT] =
{
    "",
    "",
    "PortGhostly",
    "WolfenAmmoCrate",
    "",
    "",
    "",
    "",
};

int UnloadRemove[UNLOADCOUNT] =
{
    "TomeOfPowerCooldown", "DiscOfRepulsionCooldown", "RainTracker",  "DukeTauntCooldown",
    "MaceAmmo", "MacePowered", "Mace", "DukeFlying",  "DukePetJack", "DukeVision", "DukeReady",
    "DukeWaiting", "Infrared",  "SynthFireActive", "SynthFireLeft", "SynthFireRight",
    "LevelLimiter", "LeftReload", "RightReload", "UsingSMG", "UsingFists", "UsingPistols",
    "UsingShotguns", "UsingFusionPistol", "UsingAssaultRifle", "UsingSpanker", "UsingToasty",
    "UsingAlienWeapon", "UsingAlienWeapon2", "UsingWMC",  "UsingDualPistols", "UsingDualShotguns",
    "InvasionDualShottyCheck", "QuakeQuadTimer", "SamsaraQuadDamageCooldownDisplay",
    "QuakeRegenTimer", "QuakeInvisTimer", "ChickenKillCount", "GibfistSide", "DBSHOT", "DBSHOT1",
    "DB1", "DB2", "DB3", "DB4", "DB5", "DB6", "DB7", "DB8", "DB9", "DB10", "DB11", "DB12", "DB13",
    "DB14", "DB15", "DB16", "DB17", "DB18", "DB19", "QuakePentTimer", "FlechetteCooldown"
};

int PowerOutVols[5] = {96, 104, 112, 120, 127};

int Tipboxes[CLASSCOUNT][TIPCOUNT] =
{
    {"DOOMTIP1", "DOOMTIP2"},
    {"CHEXTIP1", "CHEXTIP2"},
    {"HERETIP1", "HERETIP2"},
    {"WOLFTIP1", "WOLFTIP2"},
    {"HEXNTIP1", "HEXNTIP2"},
    {"DUKETIP1", "DUKETIP2"},
    {"MARATIP1", "MARATIP2"},
    {"QUAKTIP1", "QUAKTIP2"},
};


int DMTipboxes[CLASSCOUNT][TIPCOUNT] =
{
    {"DOOMTIP1", "DOOMTIP2"},
    {"CHEXTIP1", "CHEXTIP3"},
    {"HERETIP1", "HERETIP3"},
    {"WOLFTIP1", "WOLFTIP3"},
    {"HEXNTIP1", "HEXNTIP3"},
    {"DUKETIP1", "DUKETIP3"},
    {"MARATIP1", "MARATIP3"},
    {"QUAKTIP1", "QUAKTIP3"},
};

#define RESCOUNT 2
int ResonantItems[RESCOUNT][3] =
{
    {"UnknownResonator1", 0, 100},
    {"UnknownResonator2", 0, 100},
};


#define PDWTAKCOUNT 3

int PeopleDieParticles[CLASSCOUNT] = 
{
	/*
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    */
	"DoomguysDieParticle",
    "ChexguysDieParticle",
    "CorvusDiesParticle",
    "BlazkoDiesParticle",
    "PariasDiesParticle",
    "DukeguysDieParticle",
    "VidBoisDieParticle",
    "RangersDieParticle",
};

int PeopleDiePulses[CLASSCOUNT] = 
{
	/*
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
	*/
    "ChexguysDiePulse",
    "CorvusDiesPulse",
    "BlazkoDiesPulse",
    "PariasDiesPulse",
    "DukeguysDiePulse",
    "VidBoisDiePulse",
    "RangersDiePulse",
};

int PeopleDieBlasts[CLASSCOUNT] = 
{
    "DoomguyDiesWhenHeIsKilled",
    "ChexguyDiesWhenHeIsKilled",
    "CorvusDiesWhenHeIsKilled",
    "BlazkoDiesWhenHeIsKilled",
    "PariasDiesWhenHeIsKilled",
    "DukeDiesWhenHeIsKilled",
    "ManathorDiesWhenHeIsKilled",
    "RangerDiesWhenHeIsKilled",
};

int PDWTAKItems[PDWTAKCOUNT][2] = 
{
    {"InvPeopleDie",  "InvPeopleZorch"},
    {"InvPeopleDie2", "InvPeopleZorch2"},
    {"InvPeopleDie3", "InvPeopleZorch3"},
};

#define ARMORCOUNT  6

#define ARMOR_GREEN             0
#define ARMOR_YELLOW            1
#define ARMOR_BLUE              2
#define ARMOR_BLUE_WITHYELLOW   3
#define ARMOR_RED               4
#define ARMOR_MEGASPHERE        5

int ArmorPickups[ARMORCOUNT] = 
{
    "GreenArmorPickup",
    "YellowArmorPickup",
    "BlueArmorPickup",
    "BlueArmorPickup",
    "RedArmor",
    "MegaPack",
};

int ArmorItems[ARMORMODES][ARMORCOUNT][3] = 
{
    {
        {"ArmorPack1",          100, "GreenArmorNoDrop"},
        {"SamsaraYellowArmor",  150, "YellowArmorNoDrop"},
        {"ArmorPack2",          200, "BlueArmorNoDrop"},
        {"ArmorPack2",          200, "BlueArmorNoDrop"},
        {"ArmorPack3",          250, "RedArmorNoDrop"},
        {"ArmorPack2",          200, "BlueArmorNoDrop"},
    },
    {
        {"QuakeGreenArmor",     100, "QGreenArmorNoDrop"},
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
        {"SamsaraSilverArmor",  250, "SilverArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
    },
    {
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
        {"SamsaraSilverArmor",  250, "SilverArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
    },
    {
        {"MarathonGreenArmor",  100, "MGreenArmorNoDrop"},
        {"MarathonYellowArmor", 150, "MYellowArmorNoDrop"},
        {"MarathonBlueArmor",   200, "MBlueArmorNoDrop"},
        {"MarathonBlueArmor",   200, "MBlueArmorNoDrop"},
        {"MarathonRedArmor",    300, "MRedArmorNoDrop"},
        {"MarathonRedArmor",    300, "MRedArmorNoDrop"},
    },
    {
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
    }
};

int ArmorModeStates[ARMORMODES][4] = 
{
    {"Armor1", "Armor1_Drop", "Armor1_WithYellow", "Armor1_YellowDrop"},
    {"Armor2", "Armor2_Drop", "Armor2_WithYellow", "Armor2_YellowDrop"},
    {"Armor3", "Armor3_Drop", "Armor3_WithYellow", "Armor3_YellowDrop"},
    {"Armor4", "Armor4_Drop", "Armor4_WithYellow", "Armor4_YellowDrop"},
    {"Armor5", "Armor5_Drop", "Armor5_WithYellow", "Armor5_YellowDrop"},
};

int ArmorModeNames[ARMORMODES] = 
{
    "Normal",
    "Quake",
    "Quake (old)",
    "Marathon",
    "None",
};

int PunchDrunkItems[CLASSCOUNT][2] = 
{
    {"DoomguyPunchDrunk",   "DoomguyPunchUnique"},
    {"ChexguyPunchDrunk",   "ChexguyPunchUnique"},
    {"CorvusPunchDrunk",    "CorvusPunchUnique"},
    {"BlazkoPunchDrunk",    "BlazkoPunchUnique"},
    {"PariasPunchDrunk",    "PariasPunchUnique"},
    {"DukePunchDrunk",      "DukePunchUnique"},
    {"ManathorPunchDrunk",  "ManathorPunchUnique"},
    {"RangerPunchDrunk",    "RangerPunchUnique"},
};

int BanCVars[CLASSCOUNT] = 
{
    "samsara_bandoomguy",
    "samsara_banchex",
    "samsara_bancorvus",
    "samsara_banbj",
    "samsara_banparias",
    "samsara_banduke",
    "samsara_banso",
    "samsara_banranger",
};

int BanStrings[CLASSCOUNT] =
{
	"\c[m7]The admin has banned \cdDoomguy\c[m7] from play.",
	"\c[m7]The admin has banned \chChex Warrior\c[m7] from play.",
	"\c[m7]The admin has banned \cqCorvus\c[m7] from play.",
	"\c[m7]The admin has banned \cjB.J. Blazkowicz\c[m7] from play.",
	"\c[m7]The admin has banned \cgParias\c[m7] from play.",
	"\c[m7]The admin has banned \c[m1]Duke Nukem\c[m7] from play.",
	"\c[m7]The admin has banned \cnSecurity Officer\c[m7] from play.",
	"\c[m7]The admin has banned \csRanger\c[m7] from play.",
};


#define DAMAGEMODES 31
#define DAMAGEZEROINDEX 10

#define DEFENSEMODES 31
#define DEFENSEZEROINDEX 10

#define SPEEDMODES 17
#define SPEEDZEROINDEX 6

int DamageCVars[CLASSCOUNT] = 
{
    "samsara_doomguydamage",
    "samsara_chexdamage",
    "samsara_corvusdamage",
    "samsara_bjdamage",
    "samsara_pariasdamage",
    "samsara_dukedamage",
    "samsara_sodamage",
    "samsara_rangerdamage",
};

int DefenseCVars[CLASSCOUNT] = 
{
    "samsara_doomguydefense",
    "samsara_chexdefense",
    "samsara_corvusdefense",
    "samsara_bjdefense",
    "samsara_pariasdefense",
    "samsara_dukedefense",
    "samsara_sodefense",
    "samsara_rangerdefense",
};

int CVarDamageItems[DAMAGEMODES] = 
{
    "PowerDamageBuff-10",
    "PowerDamageBuff-9",
    "PowerDamageBuff-8",
    "PowerDamageBuff-7",
    "PowerDamageBuff-6",
    "PowerDamageBuff-5",
    "PowerDamageBuff-4",
    "PowerDamageBuff-3",
    "PowerDamageBuff-2",
    "PowerDamageBuff-1",
    "Trigger", // middle point
    "PowerDamageBuff1",
    "PowerDamageBuff2",
    "PowerDamageBuff3",
    "PowerDamageBuff4",
    "PowerDamageBuff5",
    "PowerDamageBuff6",
    "PowerDamageBuff7",
    "PowerDamageBuff8",
    "PowerDamageBuff9",
    "PowerDamageBuff10",
    "PowerDamageBuff11",
    "PowerDamageBuff12",
    "PowerDamageBuff13",
    "PowerDamageBuff14",
    "PowerDamageBuff15",
    "PowerDamageBuff16",
    "PowerDamageBuff17",
    "PowerDamageBuff18",
    "PowerDamageBuff19",
    "PowerDamageBuff20",
};

int CVarDefenseItems[DEFENSEMODES] =
{
    "PowerDefenseBuff-10",
    "PowerDefenseBuff-9",
    "PowerDefenseBuff-8",
    "PowerDefenseBuff-7",
    "PowerDefenseBuff-6",
    "PowerDefenseBuff-5",
    "PowerDefenseBuff-4",
    "PowerDefenseBuff-3",
    "PowerDefenseBuff-2",
    "PowerDefenseBuff-1",
    "Trigger", // middle point
    "PowerDefenseBuff1",
    "PowerDefenseBuff2",
    "PowerDefenseBuff3",
    "PowerDefenseBuff4",
    "PowerDefenseBuff5",
    "PowerDefenseBuff6",
    "PowerDefenseBuff7",
    "PowerDefenseBuff8",
    "PowerDefenseBuff9",
    "PowerDefenseBuff10",
    "PowerDefenseBuff11",
    "PowerDefenseBuff12",
    "PowerDefenseBuff13",
    "PowerDefenseBuff14",
    "PowerDefenseBuff15",
    "PowerDefenseBuff16",
    "PowerDefenseBuff17",
    "PowerDefenseBuff18",
    "PowerDefenseBuff19",
    "PowerDefenseBuff20",
};

int CVarSpeedItems[SPEEDMODES] = 
{
    "PowerSpeedBuff-6",
    "PowerSpeedBuff-5",
    "PowerSpeedBuff-4",
    "PowerSpeedBuff-3",
    "PowerSpeedBuff-2",
    "PowerSpeedBuff-1",
    "Trigger", // middle point
    "PowerSpeedBuff1",
    "PowerSpeedBuff2",
    "PowerSpeedBuff3",
    "PowerSpeedBuff4",
    "PowerSpeedBuff5",
    "PowerSpeedBuff6",
    "PowerSpeedBuff7",
    "PowerSpeedBuff8",
    "PowerSpeedBuff9",
    "PowerSpeedBuff10",
};

int SpeedModeNames[SPEEDMODES] = 
{
    "Immobile",
    "Almost Immobile",
    "Extremely Slow",
    "Slower",
    "Slow",
    "Slightly Slow",
    "Normal",
    "Slightly Fast",
    "Fast",
    "Faster",
    "Doubled",
    "Quadrupled",
    "Extremely Fast",
    "Uncontrollable",
    "Insane",
    "AAAAAAAAAAAAAAAAHHH",
    "FOR THE LOVE OF GOD PLEASE SLOW DOWN",
};


int ChangelogString =
"  FROM 0.3666 TO CLASSIC 2.1-pre\n\
==============================\n\
- Slightly tweaked and fixed some Punchdrunk stuff:\n\
  - Fixed the Punchdrunk unique pickup messages not showing up.\n\
  - Duke's nuclear kicks no longer alerts enemies.\n\
  - The Talon Ceramic Combat Blade no longer gets crushed by doors, and it now fades away when thrown while infinite ammo is active.\n\
- Updated the code for newer versions of Zandronum:\n\
  - Used a better method of detecting Zandronum, courtesy of tehvappy50.\n\
  - Fixed menus in newer versions of Zandronum.\n\
  - Fixed Rage, Spread, and Prosperity runes not working in newer versions of Zandronum.\n\
- Added some fixes and additions for non-Doom IWAD support:\n\
  - Flemoid slime is now affected by the Gib Persistence setting.\n\
  - Polished up the Chex Quest 3 enemies and gave them tags.\n\
  - Fixed Strife acolytes not dropping their clips on death.\n\
  - Gave the Quake Grunt its own set of sprites.\n\
  - Fixed the friendly imp not making sight or death sounds.\n\
  - Fixed the Strife Sigil not taking the player's health.\n\
  - Polished the Strife/Hexen allies and gave them their missing states back.\n\
  - Fixed Strife blue acolyte death stuff not triggering properly.\n\
  - Spectral weapons' fire rates should now match the normal versions.\n\
- Converted every numbered ACS script to use names instead, allowing for mapsets like TUTNT: Supportive Edition to work with Samsara.\n\
- Fixed and slightly tweaked some HUD elements.\n\
- Pistol Start mode won't trigger in the title map.\n\
- Fixed the HoloDuke not being taken away in Pistol Start.\n\
- Fixed Duke's jetpack and night vision fuel being reduced with infinite inventory enabled.\n\
- Added class-specific PDWTAK death particles.\n\
- Fixed the grammar and spelling of a few pickup messages, and updated some others.\n\
- Added strafing support for the Wolfenstein 3D movement.\n\
- The Super Large Zorcher no longer generates blood decals.\n\
- Added smooth animations for Doomguy's Chainsaw and Plasma Rifle.\n\
- Added proper kicking and reloading for Duke's Golden Desert Eagle.\n\
- Changed the Marathon infrared powerup color to BlueMap to match earlier ZDoom-compatible versions of Samsara.\n\
- Fixed Marathon weapons taking ammo when reloading with infinite ammo enabled.\n\
- Fixed the XDeath animations for the scripted marines.\n\
- Re-enabled the unused Nightmare setting for Bloody Hell.\n\
- Fixed the class-specific pickup messages showing up for everyone in the server.\n\
- Added widescreen-friendly weapon bars from an older version of Samsara Extra Heroes.\n\
- Added a setting to disable Ranger's DOE ammo.\n\
- Added vanilla fire states for the Spectral LAZ Device.\n\
- Fixed one of the quit messages having a miscolored character.\n\
- Changed how some of the clientside options are handled internally.\n\
- Changed the megasphere powerup animation to better resemble how it did on the original ZDoom forum thread.\n\
- Fixed the HoloDuke cooldown display showing up on the weapon bar.\n\
- Added a few serverside settings from Samsara Reincarnation such as the speed modifier, permanent powered weapons, and updated buff/nerf settings.\n\
- Changed how the chainsaw, shotgun, and backpack start settings are handled to match Samsara Reincarnation's logic.\n\
- Converted all of the coop hitscan weapons to use A_FireBullets instead of A_FireCustomMissile.\n\
- Added Corruption Cards and War Trophies compatibility.\n\
- Added partial support for Nash's Gore Mod: Vengeance Edition.\n\
- Removed every instance of the ACLOE0 sprite since it's no longer needed.\n\
- Polished the STTP3K mode to no longer use sprite translations, allowing it to be used outside of Doom.\n\
- Security Officer is now launched to the proper height from arch-viles with custom gravity disabled.\n\
- Fusion pistol charge sound now stops when letting go of altfire without attacking, as per Marathon.\n\
- Status bar mugshot grins work more consistently in single player.\n\
- Fixed some active powerups not resetting their timers when picking another one up.\n\
- Fixed Ranger's regeneration still ticking at 100 HP to no effect.\n\
- Fixed Ranger's nails not having a height defined.\n\
- Updated the Bloody Hell code to match Reincarnation's codebase.\n\
- Updated the server settings hotkey display.\n\
- Fixed Duke's Nashgore gibs using the wrong blood color.\n\
- Fixed B.J. occasionally leaving a standing corpse with Wolfenstein 3D movement.\n\
- Fixed Duke not playing death taunts in single player.\n\
- Fixed secret messages not being logged properly in netplay.\n\
- Made B.J.'s extra life start activating at 4 HP instead of 19 HP, as per ZDoom versions of Samsara.\n\
- Integrated B.J.'s extra lives into the HUD as a counter. They also properly carry over between levels now.\n\
- Added swimming support for the WolfMovement system.\n\
- Reimplemented the taunt button from ZDoom versions of Samsara, better than ever!\n\
- Fixed samsara_uniquestart 1 and 3 sometimes not giving uniques for some characters.\n\
- Due to bugs, glitches, and possible crashes, a warning is now provided when playing multiplayer in GZDoom.\n\
- Added widescreen sprites for Doomguy's Super Shotgun, Corvus' staff, and Parias' Mace of Contrition, Timon's Axe, and Hammer of Retribution.";