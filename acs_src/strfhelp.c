// Banking off the fact that this never changes...

#library "strfhelp"
#include "zcommon.acs"

#include "samsaraDefs.h"

#define VDOORSPEED  16
#define VDOORWAIT   150

str QuestItems[32] =
{
    "QuestItemThatDoesNotExist",
    "QuestItem1", "QuestItem2", "QuestItem3",
    "QuestItem4", "QuestItem5", "QuestItem6", "QuestItem7",
    "QuestItem8", "QuestItem9", "QuestItem10", "QuestItem11",
    "QuestItem12", "QuestItem13", "QuestItem14", "QuestItem15",
    "QuestItem16", "QuestItem17", "QuestItem18", "QuestItem19",
    "QuestItem20", "QuestItem21", "QuestItem22", "QuestItem23",
    "QuestItem24", "QuestItem25", "QuestItem26", "QuestItem27",
    "QuestItem28", "QuestItem29", "QuestItem30", "QuestItem31"
};

str MusicNames[34] =
{
    "",
    "d_action",
    "d_tavern",
    "d_danger",
    "d_fast",
    "d_intro",
    "d_darker",
    "d_strike",
    "d_slide",
    "d_tribal",
    "d_march",
    "d_danger",
    "d_mood",
    "d_castle",
    "d_darker",
    "d_action",
    "d_fight",
    "d_spense",
    "d_slide",
    "d_strike",
    "d_dark",
    "d_tech",
    "d_slide",
    "d_drone",
    "d_panthr",
    "d_sad",
    "d_instry",
    "d_tech",
    "d_action",
    "d_instry",
    "d_drone",
    "d_fight",
    "d_happy",
    "d_end"
};

int Splinters[5] = 
{
    "SigilSplinter1",
    "SigilSplinter2",
    "SigilSplinter3",
    "SigilSplinter4",
    "SigilSplinter5",
};

function int SplinterCount(void)
{
    int i, ret;
    for (i = 0; i < 5; i++)
    {
        if (CheckInventory(Splinters[i])) { ret += 1; }
    }
    return ret;
}

// Script 0 is used to implement several of Strife's unique line types.
// It's also used to implement the sky change after the Programmer dies.
script << 0 >> (int type, int tag)
{
    int i;

    switch (type)
    {
        // WALK TRIGGERS
        
      case 230:
        i = GetLineRowOffset() & 31;
        if (CheckInventory (QuestItems[i]))
        {
            Door_Open (tag, VDOORSPEED);
            clearlinespecial ();
        }
        break;
        
      case 227:
        i = GetLineRowOffset() & 31;
        if (CheckInventory (QuestItems[i]))
        {
            Door_Close (tag, VDOORSPEED);
            clearlinespecial ();
        }
        break;
        
      case 228:
        if (CheckInventory ("QuestItem24"))
        {
            if (CheckInventory ("QuestItem28"))
            {
                LocalAmbientSound ("svox/voc130", 127);
            }
            else
            {
                LocalAmbientSound ("svox/voc128", 127);
            }
            clearlinespecial ();
        }
        break;
        
      case 196:
        if (SplinterCount() > 1)
        {
            Floor_LowerToLowest (tag, 8);
            clearlinespecial ();
        }
        break;
        
      case 197:
        if (SplinterCount() > 1)
        {
            Door_Close (tag, VDOORSPEED*4);
            clearlinespecial ();
        }
        break;
        
      case 212:
        if (CheckInventory ("FlameThrower"))
        {
            Floor_LowerToLowest (tag, 8);
            clearlinespecial ();
        }
        break;
        
      case 193:
        i = GetLineRowOffset() & 31;
        if (CheckInventory (QuestItems[i]))
        {
            Floor_LowerToLowest (tag, 8);
            clearlinespecial ();
        }
        break;
        
      case 52:
        tag /= 100;
        if (tag == 0)
        {
            Exit_Normal (0);
        }
        else
        {
            Teleport_NewMap (tag, 0, FALSE);
        }
        break;
        
      case 187:
        i = GetLineRowOffset() & 31;
        if (CheckInventory (QuestItems[i]))
        {
            ClearForceField (tag);
            clearlinespecial ();
        }
        break;
        
      case 188:
        if (CheckInventory ("QuestItem16"))
        {
            Door_Open (tag, VDOORSPEED);
            clearlinespecial ();
        }
        break;
        
      case 200:
        if (SplinterCount())
        {
            Door_Open (tag, VDOORSPEED);
            clearlinespecial ();
        }
        break;
        
      case 215:
        i = (tag % 100) & 31;
        if (CheckInventory (QuestItems[i]))
        {
            SendToCommunicator (tag/100, 0, 1, 0);
            clearlinespecial ();
        }
        break;
        
      case 204:
      case 203:
        if (tag >= 0 && tag <= 33)
        {
            SetMusic (MusicNames[tag]);
        }
        break;
        
        
        // WALK RETRIGGERS
        
      case 216:
        i = GetLineRowOffset() & 31;
        if (CheckInventory (QuestItems[i]))
        {
            Door_Raise (tag, VDOORSPEED, VDOORWAIT);
        }
        break;
        
      case 186:
        if (lineside() != LINE_FRONT) { break; }

      case 145:
        if (gametype() == GAME_NET_DEATHMATCH)
        {
            Floor_RaiseByValue (tag, 128, 64);
        }
        else
        {
            Teleport_NewMap (tag/100, tag%100, TRUE);
        }
        break;
        
      case 175:
        if (GetActorFloorZ(0) + 16.0 > GetActorZ(0))
        {
            NoiseAlert (0, 0);
        }
        break;
        
      case 198:
        if (!CheckInventory ("OfficersUniform"))
        {
            NoiseAlert (0, 0);
        }
        break;
        
      case 208:
        if (CheckInventory ("FlameThrower"))
        {
            NoiseAlert (0, 0);
        }
        break;
        
      case 206:
        if (CheckInventory ("OfferingChalice"))
        {
            NoiseAlert (0, 0);
        }
        break;
        
      case 184:
        if (Plat_UpNearestWaitDownStay (tag, 16, 35))
        {
            // FIXME
        }
        break;
        
      case 213:
        if (!CheckInventory ("OfferingChalice"))
        {
            print (s:"You need the chalice!");
            activatorsound ("*usefail", 127);
            SetResultValue (0);
        }
        else
        {
            SetResultValue (Door_Raise (0, VDOORSPEED, VDOORWAIT, tag));
        }
        break;
        
      case 232:
        if (!CheckInventory ("QuestItem18"))
        {
            print (s:"You need the Oracle Pass!");
            activatorsound ("*usefail", 127);
            SetResultValue (0);
        }
        else
        {
            SetResultValue (Door_Raise (0, VDOORSPEED, VDOORWAIT, tag));
        }
        break;
        
      case 180:
      case 181:
        SetResultValue (Floor_RaiseByValueTxTy (tag, 8, 512));
        break;
        
      case 194:
        if (Door_Open (tag, VDOORSPEED))
        {
            print (s:"You've freed the prisoners!");
            GiveInventory ("QuestItem13", 1);
        }
        else
        {
            SetResultValue (0);
        }
        break;
        
      case 199:
        if (Ceiling_LowerAndCrush (tag, 8, 10))
        {
            print (s:"You've destroyed the Converter!");
            GiveInventory("QuestItem25", 1);
            GiveInventory("UpgradeStamina", 10);
            ACS_NamedExecuteAlways("SamsaraStrifeAccuracy", 0, -1,0,0);
        }
        else
        {
            SetResultValue (0);
        }
        break;
        
      case 209:
        if (CheckInventory ("OfferingChalice"))
        {
            SetResultValue (Generic_Stairs (tag, 16, 16, 0, 0));
        }
        else
        {
            print (s:"You need the chalice!");
            activatorsound ("*usefail", 127);
            SetResultValue (0);
        }
        break;
        
      case 219:
      case 220:
        SetResultValue (Floor_LowerToHighest (tag, 8, 128));
        break;
        
      case 226:
        if (Floor_LowerToHighest (tag, 8, 128))
        {
            GiveInventory("UpgradeStamina", 10);
            ACS_NamedExecuteAlways("SamsaraStrifeAccuracy", 0, -1,0,0);
            print(s:"Congratulations! You have completed the training area!");
        }
        else
        {
            SetResultValue (0);
        }
        break;
        
      case 154:
        SetResultValue (Plat_DownWaitUpStayLip (tag, 32, 105, 0));
        break;
        
      case 177:
        SetResultValue (Plat_DownWaitUpStayLip (tag, 32, 105, 0));
        break;
        
      case 214:	// This only needs to be ACS for the long delay
        SetResultValue (Plat_DownWaitUpStayLip (tag, 8, 1050, 0, 1));
        break;
        
      case 235:
        if (SplinterCount() < 5)
        {
            SetResultValue (0);
            break;
        }
        // Intentional fall-through
      case 174:
      case 40:
      case 189:
      case 233:
        i = Door_Open (tag, VDOORSPEED/2);
        i = i | Floor_LowerToLowest (tag, VDOORSPEED/2);
        SetResultValue (i);
        if (type == 233 && i)
        {
            SendToCommunicator (70, 0, 0, 0);
        }
        break;
        
      case 183:
        i = Door_Open (tag, VDOORSPEED/2);
        i = i | Floor_LowerToHighest (tag, VDOORSPEED/2, 128);
        SetResultValue (i);
        break;
        
      case 229:
        SetResultValue (0);
        if (SplinterCount() >= 5)
        {
            SetResultValue(Door_Animated(tag, 4, 105));
        }
        break;
        
      case 234:
        if (CheckInventory ("QuestItem3"))
        {
            SetResultValue (Door_Raise (tag, VDOORSPEED, VDOORWAIT));
        }
        else
        {
            SetResultValue (Door_LockedRaise (0, 0, 0, 102));
        }
        break;
        
      case 256:
        // Not a line type, but used by the Programmer death script.
        ChangeSky ("SKYMNT01", "SKYMNT01");
        break;
    }
}

// When the Programmer dies, he executes script 250.
script 250 (void)
{
	// The next time you visit maps 1-9, their skies need to change.
	for (int i = 1; i < 10; i++)
	{
	    ACS_Execute (0, i, 256);
	}
}
