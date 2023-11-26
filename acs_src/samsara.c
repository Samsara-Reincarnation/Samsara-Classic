#library "samsara"
#include "zcommon.acs"

#include "commonFuncs.h"


#include "samsaraDefs.h"
#include "samsaraWeps.h"
#include "samsaraSounds.h"
#include "samsaraMsgs.h"
#include "samsaraStrife.h"

int array_wolfmove[PLAYERMAX];
int array_vanillaAnim[PLAYERMAX];
int array_ballgag[PLAYERMAX];
int array_weaponBar[PLAYERMAX];
int array_pickupswitch[PLAYERMAX];
int DukeQuoteCooldown[PLAYERMAX];
int ServerEnterTimes[PLAYERMAX];
int WolfenEnterTimes[PLAYERMAX];
int ClientEnterTimes[PLAYERMAX];
int ClientTipboxes[PLAYERMAX];
int ResonantTimes[PLAYERMAX][RESCOUNT];
int ScheduledTimes[PLAYERMAX][3];

int SamsaraWepType, SamsaraClientClass, SamsaraItemFlash;
int SamsaraClientWeps[SLOTCOUNT] = {0};
int SamsaraClientWepFlashes[SLOTCOUNT] = {0};
int IsServer = 0;
int LMSMessaged = 0;
int UnloadingNow = 0;
int ArmorMode = -1;
int IsPunchdrunk = 0;
int MapArmors[ARMORCOUNT] = {-1};
int ClientTipboxModifier, ClientTipClassModifier;

global int 0:SamsaraGlobal[];

#include "samsaraFuncs.h"

#include "script_auto.h"
#include "script_pickup.h"
#include "script_marathon.h"
#include "script_quake.h"
#include "script_tipbox.h"
#include "script_strife.h"
#include "script_scheduled.h"
#include "script_pdwtak.h"

script "SamsaraDecorate" (int choice, int arg1, int arg2)
{
    int clipcount;
    int result;
    int i, j, k;
    int x, y, z;
    int armorIndex, armorToSet;
    int pln = PlayerNumber();
    
    switch (choice)
    {
      case 1:
        result = GetActorProperty(0, APROP_Dropped);
        break;
        
      case 2:
        if (CheckInventory("WolfenMovement") == 1) { SetActorState(0, "Spawn"); }
        break;
        
      case 3:
        result = !(GetCVar("sv_itemrespawn") || GetCVar("sv_weaponstay"));
        break;
        
      case 4:
        result = isInvasion() || !(isCoop() || isSinglePlayer());
        break;

      case 5:
        SetActivatorToTarget(0);
        result = CheckInventory("Cell");
        if (arg1) { TakeInventory("Cell", result); }
        break;

      case 6:
        result = GetCVar("skulltag");
        break;
      
      case 7:
        if (arg2 != 1)
        {
            GiveQuad(arg1);
        }
        else
        {
            if (isLMS())
            {
                if (GetCvar("samsara_permaquad") == 1)
                { GiveInventory("QuadDamageItem", 1); }
                break;
            }

            GiveQuad(arg1);

            if (GetCvar("samsara_permaquad") == 1)//if (isCoop() || isSinglePlayer())
            { GiveInventory("QuadDamageItem", 1); }
        }
        break;

      case 8:
		if (GetCVar("samsara_runninginzdoom")) { result = GetCVar("samsara_zd_expparticles"); }
		else { result = GetUserCVar(ConsolePlayerNumber(), "samsara_cl_expparticles"); }

		if (result == 0) { result = 100; } // default

		if (result > 0)
		{
			result = max(0, result);

			result *= max(arg1, 1);
			result /= max(arg2, 1);

			GiveInventory("QuakeExplosionCounter", result);
		}
        break;

      case 9:
        clipcount = CheckInventory("Clip");

        if (clipcount < 50)
        {
            GiveInventory("Clip", 50 - clipcount);
            TakeInventory("Clip", CheckInventory("Clip") - 50);
            result = 1;
        }
        break;

      case 10:
        TakeInventory("QuakeExplosionCounter", arg1);
        result = CheckInventory("QuakeExplosionCounter");
        break;

      case 15:
        SetActorProperty(0, APROP_Speed, percFloat(arg1, arg2));
        break;
        
      case 16:
        if (GameType () != GAME_SINGLE_PLAYER)
        {
            SetHudSize(400, 300, 0);
            Hudmessage(l:"DUKEDEADMESSAGE";
            HUDMSG_PLAIN,1,CR_LIGHTBLUE,200.4,9.1,1.75);
            delay(15);

            if (!CheckInventory("DukeBallgag"))
            {
                LocalAmbientSound("duke/mpdeath",127);
                GiveInventory("DukeTauntCooldown",5);
                ACS_NamedExecuteAlways("DukeTauntCooldown",0,0);
            }
        }
        break;

      case 17:
        if (arg1) { result = GetCVar("samsara_permault"); }
        else { result = GetCVar("sv_weaponstay"); }
        break;

      case 18:
        if (MapArmors[0] == -1) { CheckMapArmors(); }
        SetArmorMode();

        i = Timer() != 0;

        if (MapArmors[ARMOR_YELLOW] == 1) { i += 2; }

        SetActorState(0, ArmorModeStates[ArmorMode][i]);
        break;

      case 19:
        result = isLMS();
        break;

      case 20:
        SetArmorMode();

        armorIndex = -1;
        armorToSet = arg1;

        for (i = 0; i < ARMORCOUNT; i++)
        {
            if (GetArmorType(ArmorItems[ArmorMode][i][0], pln))
            {
                armorIndex = i;
                break;
            }
        }

        arg1 = middle(0, arg1, ARMORCOUNT-1);
        i = CheckInventory("Armor");
        j = ArmorItems[ArmorMode][arg1][1];

        if (j == 0) { result = 0; break; }


        /* If we're adding armor, always follow through
           Else, if the ending armor count is lower than the current armor count
           and we're not upgrading our armor, give up now */

        if (arg2 > 0)
        {
            if (arg1 <= armorIndex) { armorToSet = armorIndex; }
        }
        else if (((arg2 == 0 && i > j) || (arg2 < 0 && i > -arg2)) && (arg1 <= armorIndex))
        {
            result = 0;
            break;
        }

        if (arg2 <= 0)
        {
            TakeInventory("BasicArmor", i);
            GiveInventory(ArmorItems[ArmorMode][armorToSet][0], 1);

            k = CheckInventory("Armor");

            if (arg2 == 0) { break; }

            TakeInventory("BasicArmor", k-1);
            GiveInventory("InfiniteArmorBonus", -arg2 - 1);
        }
        else
        {
            TakeInventory("BasicArmor", i);
            GiveInventory(ArmorItems[ArmorMode][armorToSet][0], 1);

            k = CheckInventory("Armor");
            TakeInventory("BasicArmor", k-1);

            GiveInventory("InfiniteArmorBonus", (i + arg2) - 1);
        }

        result = 1;
        break;

      case 21:
        i = CheckInventory("Armor");
        if (i < arg1) { result = 0; break; }

        TakeInventory("BasicArmor", i-arg1);
        result = 1;
        break;

      case 22:
        result = GetCVar("samsara_nohealthcap");
        break;

      case 23:
        GiveInventory("TimeBombPause", 1);
        Delay(arg1);
        TakeInventory("TimeBombPause", 1);
        break;

      case 24:
        result = GetCVar("samsara_noult");
        break;

      case 25:
        if (GameType() == GAME_NET_COOPERATIVE)
		{ AmbientSound("quake/invisannouncer",127); }
		else
		{ LocalAmbientSound("quake/invisannouncer",127); }
        break;
		
	  case 26:
	    if (CheckInventory("SamsaraPowerInvisibilityNormal") == 0)
		{ GiveInventory("SamsaraPowerInvisibilityNormal",1); }
		else
		{ TakeInventory("SamsaraPowerInvisibilityNormal",1); 
		delay(1);
		GiveInventory("SamsaraPowerInvisibilityShadow",1);
		delay(1);
		GiveInventory("SamsaraPowerInvisibilityShadow",1); }
	    break;

      case 27:
        result = GetCVar("samsara_nounique");
        break;

      case 28:
        result = GetCVar("samsara_noinvuln");
        break;

      case 29:
        result = GetCVar("instagib");
        break;

      case 30:
		if (GetCVar("samsara_runninginzdoom") == 1)
		{
			result = GetCVar("samsara_zd_bloodyhell");
			//log(s:"ZDoom ",i:result);
		} else {
			result = GetCVar("samsara_cl_bloodyhell");
			//log(s:"Zandro ",i:result);
		}
        break;

      case 31:
        if (GetCVar("samsara_runninginzdoom") == 1)
		{
			result = GetCVar("samsara_zd_bloodypersistent");
		} else {
			result = GetCVar("samsara_cl_bloodypersistent");
		}
        break;

      case 32:
        result = GetCVar("samsara_nohealth");
        break;

      case 33:
        result = GetCVar("samsara_vanillaquake");
        break;
    }
    
    SetResultValue(result);
}

script "SamsaraClientDecorate" (int which, int a1, int a2) clientside
{
    int i, j, k;
    int x, y, z;
	int result;
    int bloodyHellCVar;

    SetFont("SMALLFONT");
    switch (which)
    {
      case 1:
        Print(l:"NOTUNLESSINJURED");
        break;
        
      case 2:
        Print(s:"");
        break;
        
      case 3:
        SetHudSize(400, 300, 0);
        Hudmessage(l:"FOOTENGAGED";
        HUDMSG_PLAIN,1,CR_LIGHTBLUE,200.4,9.1,1.0);
        break;

	  case 4:
		// [tv50] can't use while, otherwise the cpln isn't acquired I guess?
		// [tv50] in any case, it won't work. at least, not to my knowledge

		//while (true)
		//{
			bool cvarenabled = false;

			if (GetCVar("samsara_runninginzdoom")) { cvarenabled = GetCVar("samsara_zd_noadditivepickups"); }
			else { cvarenabled = GetUserCVar(ConsolePlayerNumber(), "samsara_cl_noadditivepickups"); }

			if (cvarenabled)
			{
				SetActorProperty(0, APROP_RenderStyle, STYLE_Normal);
				SetActorProperty(0, APROP_Alpha, itof(a1)/100);
			} else {
				SetActorProperty(0, APROP_RenderStyle, STYLE_Add);
				SetActorProperty(0, APROP_Alpha, itof(a2)/100);
			}

			//Delay(35);
		//}
		break;
        
      case 5:
        Print(l:"NOFUEL");
        break;
        
      case 6:
        Print(l:"NOPOWER");
        break;
        
      case 7:
        Print(l:"ALREADYFLYING");
        break;

      case 8:
        if (IsServer) { terminate; }
        IsPunchdrunk = a1;
        break;

      case 9:
        x = GetActorX(0); y = GetActorY(0); z = GetActorZ(0);
        i = GetActorPitch(0);
        j = GetActorAngle(0);
        k = unusedTID(4000, 14000);

        z += itof(cond(keyDown(BT_CROUCH), random(10, 14), random(30, 34)));
        Spawn("GauntletSparks", x + FixedMul(cos(i), 16 * cos(j)), y + FixedMul(cos(i), 16 * sin(j)), z - (16 * sin(i)), k);
        SetActorVelocity(k, GetActorVelX(0), GetActorVelY(0), GetActorVelZ(0), 0,0);
        break;
		
    case 10:
		if (GetCVar("samsara_runninginzdoom") == 1) { bloodyHellCVar = GetCvar("samsara_zd_bloodyhell"); }
		else { bloodyHellCVar = GetUserCvar(ConsolePlayerNumber(), "samsara_cl_bloodyhell"); }

		switch (bloodyHellCVar)
		{
			case BLOODYHELL_HAPPYFUN:		SetActorState(0,"XDeathHappyfun"); break;
			case BLOODYHELL_VANILLA:		break;
			case BLOODYHELL_NASHGORE:		SetActorState(0,"XDeathNashgore"); break;
			case BLOODYHELL_BRUTAL:			SetActorState(0,"XDeathBrutal"); break;
			case BLOODYHELL_NIGHTMARE:		SetActorState(0,StrParam(s: "XDeathNightmare", d: random(1,2))); break;
		}
		break;
		
    case 11:
		if (GetCVar("samsara_runninginzdoom") == 1) { bloodyHellCVar = GetCvar("samsara_zd_bloodyhell"); }
		else { bloodyHellCVar = GetUserCvar(ConsolePlayerNumber(), "samsara_cl_bloodyhell"); }

		switch (bloodyHellCVar)
		{
			case BLOODYHELL_HAPPYFUN:		SetActorState(0,"DeathHappyfun"); break;
			case BLOODYHELL_VANILLA:		break;
			case BLOODYHELL_NASHGORE:		SetActorState(0,"DeathNashgore"); break;
			case BLOODYHELL_BRUTAL:			SetActorState(0,"DeathBrutal"); break;
			case BLOODYHELL_NIGHTMARE:		SetActorState(0,StrParam(s: "DeathNightmare", d: random(1,6))); break;
		}
		break;
		
	case 12:
		//Many players won't use this option, so it's best to check default to cut back on if statements
		if (GetCVar("samsara_runninginzdoom") == 1) { bloodyHellCVar = GetCvar("samsara_zd_bloodyhell"); }
		else { bloodyHellCVar = GetUserCvar(ConsolePlayerNumber(), "samsara_cl_bloodyhell"); }

		switch (bloodyHellCVar)
		{
			case BLOODYHELL_HAPPYFUN:		SetActorState(0,GetBloodStateModifier("SpawnHappyfun", a1)); break;
			case BLOODYHELL_VANILLA:		SetActorState(0,GetBloodStateModifier("SpawnNormal", a1)); break;
			case BLOODYHELL_NASHGORE:		SetActorState(0,GetBloodStateModifier("SpawnNashgore", a1)); break;
			case BLOODYHELL_BRUTAL:			SetActorState(0,"SpawnBrutal"); break;
			case BLOODYHELL_NIGHTMARE:		SetActorState(0,GetBloodStateModifier("SpawnNightmare", a1)); break;
		}
		break;
	
	case 13:
	    if (GetCVar("samsara_runninginzdoom") == 1)
		{
			result = GetCVar("samsara_zd_bloodypersistent");
		} else {
			result = GetUserCVar(ConsolePlayerNumber(), "samsara_cl_bloodypersistent");
		}
        break;
		
	case 15:
		if (GetCVar("samsara_runninginzdoom") == 1) { bloodyHellCVar = GetCvar("samsara_zd_bloodyhell"); }
		else { bloodyHellCVar = GetUserCvar(ConsolePlayerNumber(), "samsara_cl_bloodyhell"); }

		switch (bloodyHellCVar)
		{
			case BLOODYHELL_HAPPYFUN:		SetActorState(0,"CrashHappyfun"); break;
			case BLOODYHELL_VANILLA:		break;
			case BLOODYHELL_NASHGORE:		SetActorState(0,"CrashNashgore"); break;
			case BLOODYHELL_BRUTAL:			SetActorState(0,"CrashBrutal"); break;
			case BLOODYHELL_NIGHTMARE:		SetActorState(0,"CrashNightmare"); break;
		}
		break;
    }
    
    SetResultValue(result);
}

script "SamsaraGetSettings" (void) net
{
    int lmsLevel = middle(0, GetCVar("samsara_lmslife"), LMSMODES-1);
    int lmsHP, lmsArmor;
    int lmsUlt, lmsUnique;
    int ultStay, highLow;

    if (lmsLevel) { lmsHP    = 100*lmsLevel; lmsArmor = 100*lmsLevel; }
    else { lmsHP = 100; lmsArmor = 0; }

    if (GetCVar("samsara_lmsunique")) { lmsUnique = "\cdwith"; }
    else { lmsUnique = "\cgwithout"; }

    if (GetCVar("samsara_lmsult")) { lmsUlt = "\cdwith"; }
    else { lmsUlt = "\cgwithout"; }

    if (GetCVar("samsara_permault")) { ultStay = "\cdstay"; }
    else { ultStay = "\cado not stay"; }

    if (GetCVar("samsara_jumpmod") < 0) { highLow = "\calower"; }
    else { highLow = "\cfhigher"; }

    SetHudSize(640, 480, 1);

    if (isLMS() || GetCVar("samsara_lmsrules"))
    {
        HudMessage(s:"Spawning with \ca", d:lmsHP, s:" health\c- and \cd", d:lmsArmor, s:" armor", s:"\c-.";
            HUDMSG_FADEOUT, 6761, CR_WHITE, 50.1, 80.0, 3.0, 1.0);
        
        HudMessage(s:"You spawn ", s:lmsUnique, s:"\c- your unique and ", s:lmsUlt, s:"\c- your Slot VII.";
            HUDMSG_FADEOUT, 6762, CR_WHITE, 50.1, 104.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Slot VII pickups ", s:ultStay, s:"\c- on pickup.";
            HUDMSG_FADEOUT, 6761, CR_WHITE, 50.1, 80.0, 3.0, 1.0);

        HudMessage(s:"Armor mode is \cf", s:ArmorModeNames[ArmorMode], s:"\c-.";
            HUDMSG_FADEOUT, 6762, CR_WHITE, 50.1, 96.0, 3.0, 1.0);
    }

    if (GetCVar("samsara_jumpmod"))
    {
        HudMessage(s:"You jump \cn", d:abs(GetCVar("samsara_jumpmod")), s:"\c- units ", s:highLow, s:"\c- than normal.";
                HUDMSG_FADEOUT, 6763, CR_WHITE, 50.1, 112.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Jumping is \cbnormal", s:"\c-.";
                HUDMSG_FADEOUT, 6763, CR_WHITE, 50.1, 112.0, 3.0, 1.0);
    }


    if (GetCVar("samsara_banjetpack"))
    {
        HudMessage(s:"Duke's jetpack is \cgbanned", s:"\c-.";
                HUDMSG_FADEOUT, 6764, CR_WHITE, 50.1, 128.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Duke's jetpack is \cdallowed", s:"\c-.";
                HUDMSG_FADEOUT, 6764, CR_WHITE, 50.1, 128.0, 3.0, 1.0);
    }

    if (GetCVar("samsara_banwolfmove"))
    {
        HudMessage(s:"Wolfenstein movement is \cgbanned", s:"\c-.";
                HUDMSG_FADEOUT, 6765, CR_WHITE, 50.1, 144.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Wolfenstein movement is \cdallowed", s:"\c-.";
                HUDMSG_FADEOUT, 6765, CR_WHITE, 50.1, 144.0, 3.0, 1.0);
    }

    if (GetCVar("samsara_nocustomgravity"))
    {
        HudMessage(s:"Custom gravities are \cadisabled", s:"\c-.";
                HUDMSG_FADEOUT, 6766, CR_WHITE, 50.1, 160.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Custom gravities are \cdenabled", s:"\c-.";
                HUDMSG_FADEOUT, 6766, CR_WHITE, 50.1, 160.0, 3.0, 1.0);
    }

    HudMessage(s:"Party mode is ", s:cond(GetCVar("samsara_peoplediewhentheyarekilled"), "\cdon\c-.", "\caoff\c-."), s:"\c- (", d:GetCVar("samsara_peoplediewhentheyarekilled"), s:")";
            HUDMSG_FADEOUT, 6767, CR_WHITE, 50.1, 176.0, 3.0, 1.0);

    HudMessage(s:"Gentleman mode is ", s:cond(GetCVar("samsara_punchdrunk"), "\cdon\c-.", "\caoff\c-."), s:"\c- (", d:GetCVar("samsara_punchdrunk"), s:")";
            HUDMSG_FADEOUT, 6768, CR_WHITE, 50.1, 192.0, 3.0, 1.0);

    HudMessage(s:"Limitless health is ", s:cond(GetCVar("samsara_nohealthcap"), "\cdon\c-.", "\caoff\c-.");
            HUDMSG_FADEOUT, 6769, CR_WHITE, 50.1, 208.0, 3.0, 1.0);

    HudMessage(s:"Chainsaw/unique start: \ca", d:GetCVar("samsara_chainsawstart"), s:"\c- / \cn", d:GetCvar("samsara_uniquestart");
            HUDMSG_FADEOUT, 6770, CR_WHITE, 50.1, 224.0, 3.0, 1.0);
}

/*
 *
 * This is still to be converted.
 *
 */

int keys[3][26] = {{0},
    {"RedCard", "YellowCard", "BlueCard", "RedSkull", "YellowSkull", "BlueSkull", "KeyBlue", "KeyGreen", "KeyYellow", "ChexRedCard", "ChexYellowCard", "ChexBlueCard", "RedFlemKey", "YellowFlemKey", "BlueFlemKey", "KeyAxe", "KeyCastle", "KeyCave", "KeyDungeon", "KeyEmerald", "KeyFire", "KeyHorn", "KeyRusted", "KeySilver", "KeySteel", "KeySwamp"},
{"\cgRed Keycard", "\ckYellow Keycard", "\chBlue Keycard", "\cgRed Skull", "\ckYellow Skull", "\chBlue Skull", "\chBlue Prism Key", "\cqGreen Prism Key", "\ckYellow Prism Key", "\cgRed Card", "\ckYellow Card", "\chBlue Card", "\cgRed Flem Key", "\ckYellow Flem Key", "\chBlue Flem Key", "\cuAxe Key", "\cfCastle Key", "\csCave Key", "\cuDungeon Key", "\cdEmerald Key", "\cgFire Key", "\ceHorn Key", "\cbRusted Key", "\cuSilver Key", "\cmSteel Key", "\cpSwamp Key"}};
//0, 1, 2: Doom R/Y/B Keycard. - \cg, \ck, \ch
//3, 4, 5: Doom R/Y/B Skull. - \cg, \ck, \ch
//6, 7, 8: Heretic B/G/Y Prism Key. - \ch, \cq, \ck
//9, 10, 11: Chex Quest R/Y/B Keycard. - \cg, \ck, \ch
//12, 13, 14: Chex Quest R/Y/B Flem Key. - \cg, \ck, \ch
//15: Axe Key - \cu (Grey)
//16: Castle Key - \cf (Gold)
//17: Cave Key - \cs (Brown)
//18: Dungeon Key - \cu (Grey)
//19: Emerald Key - \cd (Light Green)
//20: Fire Key - \cg (Red)
//21: Horn Key - \ce (Beige)
//22: Rusted Key - \cb (Very light off-white)
//23: Silver Key - \cu (Grey)
//24: Steel Key - \cm (Black)
//25: Swamp Key - \cp (Drab green-brown)
//SCRIPTS
//200:
//201: Check if SP/DM/co-op for the Tome of Power/Morph Ovum.
//202: Cooldown for Tome of Power.
//203: Take away all the shit on level ending!
//204: Boss monologues.
//205: Cooldown for Duke's taunts.
//207: Flechette cooldown.
//208: Buddha mode for B.J.'s Extra Life.
//209: Activate Send Full Button Info and activate sv_banjetpack/sv_lmslife/sv_lmsult.
//212: Displaying text.
//214: Duke Jetpack/Visor fuel draining.
//901-902: I'm pretty sure Synert is a wizard, too.
//224: Doomguy's vanilla animations. By Ijon Tichy, transcribed by Llewellyn.
//225: Weapon bar. By Ijon Tichy, transcribed by Llewellyn.
////////////////////
// SHARED KEYS
// (by Synert)
/////////////////

// Give keys as needed, for people joining the game.

script "SamsaraKeyShare" ENTER
{
    if (!(IsSinglePlayer() || IsCoop())) { terminate; }

    while (1)
    {
        for (int a = 0; a < 26; a++)
        {
            if (keys[0][a] == 1)
            {
                GiveInventory(keys[1][a], 1);
            }
        }
        delay(10);
    }
}

script "SamsaraKeyBroadcast" (int a) { // Picked up a key, broadcast that shit to the whole world!
    if(keys[0][a] == 0 && isCoop() && !isSinglePlayer()) {
        Log(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*."); // Let the server admins know.
        HudMessageBold(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*.";HUDMSG_FADEOUT, 900, CR_GOLD, 0.5, 0.1, 3.0, 0.5);
    }
    keys[0][a] = 1;
}

///////////////
// ITEM STUFF
//////////////

script "DukeResourceDrain" (int dukeshit)
{
    switch(dukeshit)
    {
      case 1:
        if (CheckInventory("DukeFlying") == 1)
        {
            if (CheckInventory("DukeJetpackFuel") > 0)
            {
                delay(15);
				if (CheckInventory("SamsaraHasInfiniteInventory") >= 1)
				{
					restart;
				}
				else
                TakeInventory("DukeJetpackFuel",1);
                restart;
            }
            else
            {
                TakeInventory("DukeFlying",1);
                TakeInventory("DukePetJack",1);
            }
        }
        break;
        
      case 2:
        if (CheckInventory("DukeVision") == 1)
        {
            if (CheckInventory("DukeVisionFuel") > 0)
            {
                delay(10);
				if (CheckInventory("SamsaraHasInfiniteInventory") >= 1)
				{
					restart;
				}
				else
                TakeInventory("DukeVisionFuel",1);
                restart;
            }
            else
            {
                TakeInventory("DukeVision",1);
                TakeInventory("PowerLightAmp",1);
            }
        }
        break;
    }
}

script "TomeOfPowerCooldown" (void)
{
    while (CheckInventory("TomeOfPowerCooldown") > 0)
    {
        Delay(35);

        if (CheckInventory("TomeOfPowerCooldown") > 240)
        {
            TakeInventory("TomeOfPowerCooldown",1);
        }
        else
        {
            TakeInventory("TomeOfPowerCooldown",1);
            TakeInventory("RainTracker",1);
        }

        if (CheckInventory("TomeOfPowerCooldown") == 160)  // To end the spectral effect when the tome ends
        {
            TakeInventory("SpectralFiring", 0x7FFFFFFF);
            ActivatorSound("weapons/sigil", 127);
        }
    }
}

script "DiscOfRepulsionCooldown" (void)
{
    if (CheckInventory("DiscOfRepulsionCooldown") > 0)
    {
        delay(35);
        TakeInventory("DiscOfRepulsionCooldown",1);
        restart;
    }
}

script "SamsaraUnloading" unloading
{
    int i;
    UnloadingNow = 1;

    for (i = 0; i < UNLOADCOUNT; i++) { TakeInventory(UnloadRemove[i], 0x7FFFFFFF); }
}

/////////////////
// BOSS STUFF
//////////////

script "SamsaraBossMonologue" (int bossmonologueshit)
{
    switch(bossmonologueshit)
    {
      case 1: // KORAX
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX01"; HUDMSG_FADEOUT,15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX02"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX03"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX04"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX05"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX06"; HUDMSG_FADEOUT,15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX07"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX08"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX09"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX10"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(100);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_KORAX11"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
		}
        break;
        
      case 2: // LORD SNOTFOLUS
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX01"; HUDMSG_FADEOUT,15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX02"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX03"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX04"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX05"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX06"; HUDMSG_FADEOUT,15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX07"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX08"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX09"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX10"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(100);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_CHEX11"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
		}
        break;
        
      case 3: // D'SPARIL
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL01"; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL02"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL03"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL04"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL05"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL06"; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL07"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL08"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL09"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL10"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(140);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_DSPARIL11"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
		}
        break;
        
      case 4: // THE ENTITY
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY01"; HUDMSG_FADEOUT,15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY02"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY03"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY04"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY05"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY06"; HUDMSG_FADEOUT,15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY07"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY08"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY09"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY10"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(140);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ENTITY11"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
		}
        break;
		
      case 5: // THE ICON OF SIN
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON01"; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON02"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON03"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON04"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON05"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON06"; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON07"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON08"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON09"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON10"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(140);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(l:"BOSS_MONOLOGUE_ICON11"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
		}
        break;
    }
}

script "DukeTauntCooldown" (void)
{
    while (CheckInventory("DukeTauntCooldown") > 0)
    {
        Delay(35);
        TakeInventory("DukeTauntCooldown", 1);
    }
}

script "QuakeGrenade" (int x, int y, int z)
{
    if (GetActorZ(0) - GetActorFloorZ(0) > 4.0)
    {
        SetResultValue(1);
    }
    else
    {
        SetResultValue(sqrt(x*x + y*y + z*z) );
    }
}

script "QuakeParticle" (int divI, int divF, int divF1)
{
    int div = percFloat2(divI, divF, divF1);

    int x = GetActorVelX(0);
    int y = GetActorVelY(0);
    int z = GetActorVelZ(0);

    SetActorVelocity(0, FixedMul(x, div), FixedMul(y, div), FixedMul(z, div), 0, 1);
}

script "Samsara678" (int which)
{
    int x, y, z;

    x = GetActorX(0); y = GetActorY(0); z = GetActorZ(0);
    SetActivatorToTarget(0);

    ACS_NamedExecuteAlways("Samsara679", 0, x, y, z);
}

script "Samsara679" (int tx, int ty, int tz) clientside
{
    int t, i, k = 0, l;
    int x, y, z;
    int vx, vy, vz, mag, magI;

    x  = GetActorX(0); y =  GetActorY(0);  z = GetActorZ(0) + 24.0;

    vx = tx-x; vy = ty-y; vz = tz-z; mag = magnitudeThree_f(vx, vy, vz);
    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);
    magI = ftoi(mag);

    for (i = 8; i < magI; i += 8)
    {
        Spawn("SpnkrBeam", x+(vx*i), y+(vy*i), z+(vz*i));
        
        l += (i - k);
        Delay(l / 512);
        l %= 512;
        k = i;
    }
}