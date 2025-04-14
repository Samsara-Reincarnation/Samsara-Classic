int msgColors[26] = 
{
    "\ca", "\cb", "\cc", "\cd", "\ce", "\cf", "\cg", "\ch", "\ci", "\cj", "\ck", "\cl", "\cm",
    "\cn", "\co", "\cp", "\cq", "\cr", "\cs", "\ct", "\cu", "\cv", "\cw", "\cx", "\cy", "\cz",
};

script "SamsaraClientClass" (int slot) clientside
{
    int toClass = SamsaraClientClass-1;
    int displaymode;
    if (GetCVar("samsara_runninginzdoom") == 1)
    {
        displaymode = GetCVar("samsara_zd_pickupmode");
    } else {
        displaymode = GetUserCVar(ConsolePlayerNumber(), "samsara_cl_pickupmode");
    }
    int oldslot = slot;
    int success = 0;
    int punchdrunk =  IsPunchdrunk & 1;
    int pdUniques  = (IsPunchdrunk & 2) || punchdrunk;
    int pdSaws     = (IsPunchdrunk & 4) || punchdrunk;

    if (slot == SLOT_CHAINSAW && pdSaws) { slot = SLOT_PUNCHDRUNKSAW; }
    if (slot == SLOT_BFG9000 && punchdrunk && pdUniques) { slot = SLOT_UNIQUE; }

    while (punchdrunk)
    {
        if (slot == SLOT_PUNCHDRUNKSAW && pdSaws) { break; }
        if (slot == SLOT_UNIQUE && pdUniques) { break; }

        SetActorState(0, "Invisible");
        terminate;
    }
    
    int hasSlot = SamsaraClientWeps[slot];

    if (displaymode != 0)
    {
        if ((SamsaraItemFlash >= (Timer() - 35)) && (Timer() >= 70))
        {
            Spawn("SamsaraChangeFlash", GetActorX(0), GetActorY(0), GetActorZ(0));
        }
        
        if (toClass == -1)
        {
            SetActorState(0, "NoGuy");
            terminate;
        }
    }
    
    if (slot == SLOT_UNIQUE)
    {
        switch (displaymode)
        {
          case 0:
            SetActorState(0, "NoGuy");
            break;
            
          case 1:
            if (pdUniques) { success = SetActorState(0, PickupStates[toClass][7]); }
            if (!pdUniques || !success) { SetActorState(0, PickupStates[toClass][3]); }
            break;
            
          case 2:
            if (pdUniques) { success = SetActorState(0, PickupStates[toClass][4]); }
            if (!pdUniques || !success) { SetActorState(0, PickupStates[toClass][0]); }
            break;
        }
    }
    else switch (displaymode)
    {
      case 0:
        SetActorState(0, "NoGuy");
        break;
		
      case 1:
        if ((SamsaraClientWepFlashes[slot] >= (Timer() - 35)) && (Timer() >= 35))
        {
            Spawn("SamsaraChangeFlash2", GetActorX(0), GetActorY(0), GetActorZ(0));
        }
        
        switch (slot)
        {
          case SLOT_CHAINSAW:
          case SLOT_PUNCHDRUNKSAW:
            if (pdSaws)
            {
                if (hasSlot) { success = SetActorState(0, PickupStates[toClass][5]); }
                else         { success = SetActorState(0, PickupStates[toClass][6]); }
            }
            break;
        }

        if (!success)
        {
            if (hasSlot) { SetActorState(0, PickupStates[toClass][1]); }
            else         { SetActorState(0, PickupStates[toClass][2]); }
        }
        break;
        
      case 2:
        switch (slot)
        {
          case SLOT_CHAINSAW:
          case SLOT_PUNCHDRUNKSAW:
            if (pdSaws) { success = SetActorState(0, PickupStates[toClass][4]); }
            else { success = SetActorState(0, PickupStates[toClass][0]); }
            break;
        }
          
        if (!success) { SetActorState(0, PickupStates[toClass][0]); }
        break;
    }
}

/*
*      WARNING
*  This script is over 20 variables. Check here first for segfaults.
*  Also, keep an eye on this script for potential desync issues.
*  (current count: 25)
*/

script "SamsaraGiveWeapon" (int slot, int dropped, int silent)
{
    // clients were running this before, that's bad
    if (!IsServer) { terminate; }
    if (slot < 0 || slot >= SLOTCOUNT) { terminate; }

    int weaponStay = !!GetCVar("sv_weaponstay");
    int punchdrunk = IsPunchdrunk & 1;
    int pdSaws     = (IsPunchdrunk & 4) || punchdrunk;

   // hijack chainsaw for punchdrunk saw
    if (pdSaws && slot == SLOT_CHAINSAW) { slot = SLOT_PUNCHDRUNKSAW; }

    if (punchdrunk)
    {
        // do unique giving instead
        if (slot == SLOT_BFG9000)
        {
            SetResultValue(ACS_NamedExecuteWithResult("SamsaraGiveUnique", 0));
            terminate;
        }

        // no weapon for you go away
        if (slot != SLOT_PUNCHDRUNKSAW)
        {
            SetResultValue(weaponStay * WEPFLAGS_WEAPONSTAY);
            terminate;
        }
    }

    int weaponGet  = 0;
    int pclass = samsaraClassNum();
    int hasWep = HasClassWeapon(pclass, slot, 1);

    if (slot == SLOT_BFG9000) { weaponStay = !!GetCVar("samsara_permault"); }

    int a1cnt  = 0, a2cnt = 0;
    int a1max  = 0, a2max = 0;
    int a1max2 = 0, a2max2 = 0;
    int a1Full = 0, a2Full = 0;
    int a1diff = 0, a2diff = 0;

    int weapon  = ClassWeapons[pclass][slot][S_WEP],        wepbool = !!StrLen(weapon); 
    int ammo1   = ClassWeapons[pclass][slot][S_AMMO1],      a1bool  = !!StrLen(ammo1);
    int ammo2   = ClassWeapons[pclass][slot][S_AMMO2],      a2bool  = !!StrLen(ammo2);
    int check   = ClassWeapons[pclass][slot][S_CHECKITEM],  chkbool = !!StrLen(check);

    // If we have the check-fail item, kill the pickup if this wasn't a drop
    // (it's for the wastems and LevelLimiter)
    // (the wastems are still fucking terrible)
    if (!wepbool || (CheckInventory(ClassWeapons[pclass][slot][S_CHECKFAILITEM]) && dropped <= 0))
    {
        SetResultValue(weaponStay * WEPFLAGS_WEAPONSTAY);
        terminate;
    }

    if (a1bool)
    {
        a1cnt   = CheckInventory(ammo1);
        a1max   = GetAmmoCapacity(ammo1);
        a1max2  = a1max * 4;
        // so that when shaving off half the given ammo, we don't run into the ammo cap
        // and note an erroneous amount of ammo given
    }

    if (a2bool)
    {
        a2cnt   = CheckInventory(ammo2);
        a2max   = GetAmmoCapacity(ammo2);
        a2max2  = a2max * 4; // ditto
    }

    if (a1bool) { a1Full = (CheckInventory(ammo1) >= a1max); }
    if (a2bool) { a2Full = (CheckInventory(ammo2) >= a2max); }

    if (dropped >= 2)
    {
        if (a1bool) { SetAmmoCapacity(ammo1, a1max2); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max2); }
    }

    // check for both the weapon AND the check item
    // if we miss either, or we're checking for ForceCheck,
    // we're giving the weapon
    if (!hasWep || !StrCmp(check, "ForceCheck") || (chkbool && !CheckInventory(check)))
    {
        weaponGet = 1;
    }
    else if (!weaponStay || dropped >= 1)
    {
        // still attempt a pickup if we can get ammo from it
        if ((a1bool && !a1Full) || (a2Bool && !a2Full)) { weaponGet = 1; }
    }

    if (weaponGet)
    {
        int success = _giveclassweapon(pclass, slot, 3, dropped, 0);

        if (!silent && success)
        {
            // this is for mugshot grins and weapon switching
            if (!hasWep)
            {
                Spawn("WeaponGetYaaaay", GetActorX(0), GetActorY(0), GetActorZ(0));
                Spawn("WeaponGetYaaaay2", GetActorX(0), GetActorY(0), GetActorZ(0));
				
				if(isSinglePlayer())
				{
					SetMugShotState("Grin");
				}
				
				// work around for auto-switch seemingly being fucking broked in zdoom
				If (GetCVar("samsara_runninginzdoom") == 1)
				{
					If (GetCVar("neverswitchonpickup") == 0)
					{
						SetWeapon(weapon);
					}
				}
            }

            ACS_NamedExecuteAlways("SamsaraClientWeaponPickup", 0, slot, dropped);

            if (dropped <= 1) { ACS_NamedExecuteWithResult("SamsaraWeaponTaunt"); } // so it gives the taunt cooldown properly
		}
	}

    if (dropped >= 2)
    {
        // shave off half the ammo given (NOTE: this method is inaccurate for doom deathmatch ammo giving on first weapon pickup; result should be amount / 2 * 5 / 2)
        // (tossed pickups shouldn't really come up in deathmatch, though)
        int a1drop = (CheckInventory(ammo1) - a1cnt) / 2;
        int a2drop = (CheckInventory(ammo2) - a2cnt) / 2;

        SetInventory(ammo1, a1cnt + a1drop);
        SetInventory(ammo2, a2cnt + a2drop);

        // go back to normal ammo capacity
        if (a1bool) { SetAmmoCapacity(ammo1, a1max); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max); }
    }

    // make sure we're not over max ammo capacity
    if (CheckInventory(ammo1) > GetAmmoCapacity(ammo1))
    {
        if (a1cnt > GetAmmoCapacity(ammo1)) { SetInventory(ammo1, a1cnt); }
        else { SetInventory(ammo1, GetAmmoCapacity(ammo1)); }
    }

    if (CheckInventory(ammo2) > GetAmmoCapacity(ammo2))
    {
        if (a2cnt > GetAmmoCapacity(ammo2)) { SetInventory(ammo2, a2cnt); }
        else { SetInventory(ammo2, GetAmmoCapacity(ammo2)); }
    }

    // tell the pickup if weaponstay was on, and if the pickup succeeded
    SetResultValue((weaponStay * WEPFLAGS_WEAPONSTAY) + (success * WEPFLAGS_GOTWEAPON));
}

script "SamsaraGiveUnique" (int alt)
{
    if (!IsServer) { terminate; }
    
    int uniqueGet = 0;
    int pclass = samsaraClassNum();
    int pd = GetCVar("samsara_punchdrunk") || GetCVar("samsara_punchdrunkuniques");

    while (!uniqueGet && alt >= 0)
    {
        uniqueGet = GiveUnique(pclass, alt);
        alt--;
    }
    
    if (uniqueGet && IsServer)
    {
        ACS_NamedExecuteAlways("SamsaraClientUniquePickup", 0, pd);
    }
    
    SetResultValue(uniqueGet);
}

int QuoteStorage[MSGCOUNT];

script "SamsaraClientWeaponPickup" (int slot, int dropped) CLIENTSIDE
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    int pickupsound = ClassPickupSounds[pclass][slot];

    if (dropped >= 2) { pickupsound = ClassDropSounds[pclass][slot]; }

    if (cpln == pln && GetCVar("msg") == 0)
    {
        if (GetCVar("samsara_cl_moremessages"))
        {
            for (i = 0; i < MSGCOUNT; i++)
            {
                j = ClassPickupMessages[pclass][slot][i];

                if (!StrLen(j)) { continue; }
                QuoteStorage[quoteCount++] = j;
            }

            if (!quoteCount) { logMsg = "Oh bugger there's no messages for this weapon."; }
            else { logMsg = QuoteStorage[random(0, quoteCount-1)]; }
        }
        else
        {
            logMsg = ClassPickupMessages[pclass][slot][0];

            if (!StrLen(logMsg)) { logMsg = "Oh bugger there's no message for this weapon."; }
        }

        if (GetCVar("samsara_cl_printpickup")) { Print(s:logMsg); }
        else { Log(s:msgColors[GetCVar("msg0color")], s:logMsg); }
    }

    PlaySound(0, pickupsound, CHAN_ITEM|CHAN_MAYBE_LOCAL);

    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
}

script "SamsaraClientUniquePickup" (int punchdrunk) CLIENTSIDE
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    int pickupsound;

    if (punchdrunk) { pickupsound = PunchdrunkUniqueSounds[pclass]; }
    else { pickupsound = ClassUniqueSounds[pclass]; }

    if (cpln == pln && GetCVar("msg") == 0)
    {
        if (GetCVar("samsara_cl_moremessages"))
        {
            for (i = 0; i < MSGCOUNT; i++)
            {
                if (punchdrunk) { j = PunchDrunkUniqueMessages[pclass][i]; }
                else { j = ClassUniqueMessages[pclass][i]; }

                if (!StrLen(j)) { continue; }
                QuoteStorage[quoteCount++] = j;
            }

            if (!quoteCount) { logMsg = "Oh bugger there's no messages for this unique."; }
            else { logMsg = QuoteStorage[random(0, quoteCount-1)]; }
        }
        else
        {
            if (punchdrunk) { logMsg = PunchDrunkUniqueMessages[pclass][0]; }
            else { logMsg = ClassUniqueMessages[pclass][0]; }

            if (!StrLen(logMsg)) { logMsg = "Oh bugger there's no message for this unique."; }
        }

        if (GetCVar("samsara_cl_printpickup")) { Print(s:logMsg); }
        else { Log(s:msgColors[GetCVar("msg0color")], s:logMsg); }
    }

    PlaySound(0, pickupsound, CHAN_ITEM|CHAN_MAYBE_LOCAL);

    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
}

script "SamsaraWeaponTaunt" (void)
{
    switch (samsaraClassNum())
    {
      case CLASS_DUKE:
        Delay(8);
        GiveInventory("DukeWeaponTaunt", 1);
        break;

      default:
        terminate;
    }
}
