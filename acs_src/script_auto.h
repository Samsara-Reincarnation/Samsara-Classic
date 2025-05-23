script "SamsaraOpen" open
{
    IsServer = 1;
    int opd, pd;
    int opdu, pdu;
    int opds, pds;

    if (SamsaraGlobal[GLOBAL_DONEBITCHING] == 0)
    {
        if (GameType() == GAME_TITLE_MAP)
        {
        }
        if (isSinglePlayer())
        {
            if(GetCvar("samsara_cvarinfo") == 0)
            {
            HudMessage(s:HELPSTR;
            HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
            HudMessage(s:HELPSTR_CL;
            HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
            HudMessage(s:"\cjSee the console for \cacvar information\c-.";
            HUDMSG_FADEOUT, 92712, CR_WHITE, 1.5, 0.2, 2.0, 0.5);
            SetCVar ("samsara_cvarinfo", 1);
            //ConsoleCommand("archivecvar samsara_cvarinfo");
            }
        }
        else
        {
            Log(s:HELPSTR);
        }
        
        SamsaraGlobal[GLOBAL_DONEBITCHING] = 1;

        // Warn the player about playing Samsara in GZDoom multiplayer
        if (PlayerCount() > 1 && isZDoom())
        {
            SetFont("SMALLFONT");
            HudMessageBold(l:"SAMSARA_INCOMPATIBILITY_GZDOOMMP"; HUDMSG_TYPEON | HUDMSG_LOG, 0, CR_WHITE, 1.5, 0.8, 5.0, 0.05, 2.0);
        }
    }

    ArmorMode = middle(0, GetCVar("samsara_armormode"), ARMORMODES-1);
    CheckMapArmors();
    
    while (1)
    {
        if (!isZDoom())
        {
            if (!GetCVar("compat_clientssendfullbuttoninfo") && IsNetworkGame()) { ConsoleCommand("compat_clientssendfullbuttoninfo 1"); } // cannot use SetCVar with cvars defined in the source ports
        }

        opd = pd;
        pd = !!GetCVar("samsara_punchdrunk");

        opdu = pdu;
        pdu = !!GetCVar("samsara_punchdrunkuniques");

        opds = pds;
        pds = !!GetCVar("samsara_punchdrunksaws");

        if (pd != opd || opdu != pdu || opds != pds || Timer() % 18 == 0)
        {
            IsPunchdrunk  = pd  << 0;
            IsPunchdrunk |= pdu << 1;
            IsPunchdrunk |= pds << 2;
            ACS_NamedExecuteAlways("SamsaraClientDecorate", 0, 8, IsPunchdrunk);
        }
        
        Delay(1);
    }
}

script "SamsaraEnter" enter { ACS_NamedExecuteWithResult("SamsaraSpawn", 0,0,0); }
script "SamsaraRespawn" respawn { ACS_NamedExecuteWithResult("SamsaraSpawn", 1,0,0); }
script "SamsaraReturn" return { UnloadingNow = 0; ACS_NamedExecuteWithResult("SamsaraSpawn", 0,0,0); }

script "SamsaraSpawn" (int respawning)
{
    int pln = PlayerNumber();
    int pcount, opcount;
    int startTime = Timer();
    int endloop;
    int canbuddha;
    int wsteSide;
    int armor, oarmor, type, otype;
    int i;

    ServerEnterTimes[pln] = startTime;
    ACS_NamedExecuteWithResult("SamsaraSynthFire", startTime);

    if (!CheckInventory("IsSamsaraClass")) { terminate; }


    ACS_NamedExecuteAlways("SamsaraEnterClient", 0, startTime,0,0);
    ACS_NamedExecuteWithResult("SamsaraWolfMove", startTime,0,0);
    ACS_NamedExecuteWithResult("SamsaraQPowers",  startTime,0,0);
    
    if (isLMS()) { ApplyLMS(); }
    if (GetCvar("samsara_lmsrules") == 1)
    {
        ApplyLMS();
        GiveInventory("CanDualShotties",1);
    }
    if (isSinglePlayer()) { SamsaraWepType = samsaraClassNum()+1; }

    if (!respawning)
    {
        ClientTipboxes[pln] = 0;
        ACS_NamedExecuteAlways("SamsaraScheduled", 0, respawning,1,0);
	}

    HandleBackpackSpawn(respawning);
    for (i = SLOT_FIST; i <= SLOT_BFG9000; i++) { HandleSlotSpawn(i, respawning); }
    if ((!respawning || (respawning && !(isSinglePlayer() || isCoop()))) && !(isLMS() || GetCVar("samsara_lmsrules")))
    {
        if (GetCVar("sv_shotgunstart")) { GiveClassWeapon(samsaraClassNum(), SLOT_SHOTGUN, 3);}
    }
    HandleUniqueSpawn(respawning);
    HandleRailgunSpawn(respawning);
    HandlePunchdrunk(respawning);
    HandleInstagib(respawning);
	
    ACS_NamedExecuteAlways("SamsaraScheduled", 0, respawning,0,0);

    switch (samsaraClassNum())
    {
      case CLASS_DUKE:
        if (!respawning) { GiveInventory("DukeReady", 1); }
        break;
        
      case CLASS_MARATHON:
        if (isInvasion()) { GiveInventory("InvasionDualShottyCheck", 1); }
        break;
    }

    if (isSinglePlayer())
    {
        GiveInventory("SPModeOn", 1);
    }
    else if (isCoop())
    {
        GiveInventory("CoopModeOn", 1);
        SetActorState(0, "CoOpModeOn");
    }

    pcount = PlayerCount();

    if (CheckInventory("SamsaraLevelSwitch_HadDoE") && !respawning)
    {
        GiveInventory("UsingDoEAmmo", 1);

        if (CheckWeapon("Grenade Launcher")) { GiveInventory("UsingMultiNades", 1); }
        if (CheckWeapon("  Rocket Launcher  ")) { GiveInventory("UsingMultiRockets", 1); }
        if (CheckWeapon("Nailgun")) { GiveInventory("FiredLavaNails", 1); }
        if (CheckWeapon("Super Nailgun")) { GiveInventory("FiredSLNails", 1); }
    }

    while (!endloop && ServerEnterTimes[pln] == startTime)
    {
        if (GetArmorType("MarathonRedArmor", pln) || GetArmorType("MRedArmorNoDrop", pln)) { SetInventory("SamsaraArmorCategory", 10); }
        else if (GetArmorType("MarathonYellowArmor", pln) || GetArmorType("MYellowArmorNoDrop", pln) ||
                 GetArmorType("EnchantedShield", pln))
        { SetInventory("SamsaraArmorCategory", 9); }
        else if (GetArmorType("QuakeGreenArmor", pln) || GetArmorType("QGreenArmorNoDrop", pln)) { SetInventory("SamsaraArmorCategory", 8); }
        else if (GetArmorType("SilverShield", pln) ||
                 GetArmorType("SamsaraSilverArmor", pln) || GetArmorType("SilverArmorNoDrop", pln) ||
                 GetArmorType("SilverArmor", pln)) // just in case
        { SetInventory("SamsaraArmorCategory", 7); }
        else if (GetArmorType("YellowArmor", pln) || // just in case
                 GetArmorType("SamsaraYellowArmor", pln) || GetArmorType("YellowArmorNoDrop", pln) ||
                 GetArmorType("QuakeYellowArmor", pln) || GetArmorType("QYellowArmorNoDrop", pln) ||
                 GetArmorType("MarathonBlueArmor", pln) || GetArmorType("MBlueArmorNoDrop", pln))
        { SetInventory("SamsaraArmorCategory", 6); }
        //else if (GetArmorType("", pln)) { SetInventory("SamsaraArmorCategory", 5); } // hexen 25 armor pickups give 25 points of tier 1 armor
        else if (GetArmorType("QuakeRedArmor", pln) || GetArmorType("QRedArmorNoDrop", pln) ||
                 GetArmorType("RedArmor", pln) || // why not?
                 GetArmorType("ArmorPack3", pln) || GetArmorType("RedArmorNoDrop", pln))
        { SetInventory("SamsaraArmorCategory", 4); }
        else if (GetArmorType("BasicArmorPickup", pln) || GetArmorType("BlueArmor", pln) || GetArmorType("BlueArmorForMegasphere", pln) || GetArmorType("MetalArmor", pln) || GetArmorType("SuperChexArmor", pln) || // BasicArmorPickup is given by armor cheat
                 GetArmorType("ArmorPack2", pln) || GetArmorType("BlueArmorNoDrop", pln) ||
                 GetArmorType("HereticArmorPack2", pln) ||
                 GetArmorType("FuckArmor", pln))
        { SetInventory("SamsaraArmorCategory", 3); }
        else if (GetArmorType("BasicArmorBonus", pln) || GetArmorType("ArmorBonus", pln) || GetArmorType("MaxArmorBonus", pln) || GetArmorType("SlimeRepellent", pln) ||
                 GetArmorType("ArmorScrap", pln) || GetArmorType("InfiniteArmorBonus", pln) ||
                 GetArmorType("HereticArmorDump", pln) ||
                 GetArmorType("HexenArmor1", pln) ||
                 GetArmorType("HexenArmor5B", pln) ||
                 GetArmorType("MysticAmbitArmor", pln) ||
                 GetArmorType("GreenArmor", pln) || GetArmorType("LeatherArmor", pln) || GetArmorType("ChexArmor", pln) ||
                 GetArmorType("ArmorPack1", pln) || GetArmorType("GreenArmorNoDrop", pln) ||
                 GetArmorType("HereticArmorPack1", pln))
        { SetInventory("SamsaraArmorCategory", 2); }
        else if (GetArmorType("MarathonGreenArmor", pln) || GetArmorType("MGreenArmorNoDrop", pln)) { SetInventory("SamsaraArmorCategory", 1); }
        else { SetInventory("SamsaraArmorCategory", 0); } // other armors

        SetInventory("SamsaraHasDoubleFiringSpeed", CheckInventory("PowerDoubleFiringSpeed") || CheckInventory("RuneDoubleFiringSpeed") || CheckInventory("CustomDoubleFiringSpeed") ||
                                                    CheckInventory("PowerRage1"));

        SetInventory("SamsaraHasProsperity", CheckInventory("PowerProsperity") || CheckInventory("RuneProsperity") || CheckInventory("CustomProsperity"));

        SetInventory("SamsaraHasSpread", CheckInventory("PowerSpread") || CheckInventory("RuneSpread") || CheckInventory("CustomSpread") ||
                                         CheckInventory("PowerSpread1"));

        SetInventory("SamsaraHasInfiniteAmmo", GetCVar("sv_infiniteammo") ||
                                               CheckInventory("PowerInfiniteAmmo") || CheckInventory("RuneInfiniteAmmo") || CheckInventory("CustomInfiniteAmmo")); // the latter two don't exist, but are there just in case

        SetInventory("SamsaraHasInfiniteInventory", GetCVar("sv_infiniteinventory"));

        SetInventory("SamsaraCurrentWaterLevel", GetActorProperty(0, APROP_WaterLevel));

        if(CheckInventory("MetaCheatDetector")) { ACS_NamedExecuteWithResult("InventoryCheat",0,0,0); }

        SetInventory("WolfenMovement", GetUserCVar(pln, "samsara_cl_wolfmove") && !GetCVar("samsara_banwolfmove"));

        SetInventory("DukeBallgag", GetUserCVar(pln, "samsara_cl_ballgag"));

        SetInventory("VanillaDoom", !GetUserCVar(pln, "samsara_cl_smoothanims"));

        SetInventory("ExpandedHud", GetUserCVar(pln, "samsara_cl_weaponhud"));

        if (GetCVar("samsara_runninginzdoom") == 1) 
		{
			TakeInventory("SamsaraIsPlayingInZandronum", 1);

			if (GetCVar("sv_nobfgaim") == 1) { GiveInventory("DoomNoBFGAim", 1); }
			else { TakeInventory("DoomNoBFGAim",  0x7FFFFFFF); }
		}
		
		if (GetCVar("samsara_runninginzandro") == 1)
        {
			GiveInventory("SamsaraIsPlayingInZandronum", 1);

			if (GetCVar("sv_bfgfreeaim") == 0) { GiveInventory("DoomNoBFGAim", 1); }
			else { TakeInventory("DoomNoBFGAim",  0x7FFFFFFF); }
		}

        if (GetCVar("samsara_classiclaz") == 1) { GiveInventory("ChexClassicLaz", 1); }
        else { TakeInventory("ChexClassicLaz", 0x7FFFFFFF); }

        if (GetCVar("samsara_allcanrj") == 1) { GiveInventory("RJingModeOn", 1); }
        else { TakeInventory("RJingModeOn", 0x7FFFFFFF); }

        if (GetCVar("samsara_pistolammo") == 1) { GiveInventory("PistolModeOn", 1); }
        else { TakeInventory("PistolModeOn", 0x7FFFFFFF); }

        if (GetCVar("samsara_vanillaquake") == 1) { GiveInventory("QuakeModeOn", 1); }
        else { TakeInventory("QuakeModeOn", 0x7FFFFFFF); }

        if (GetCVar("samsara_dukesoundboard") == 1) { if (CheckInventory("DukeClass") == 1) {
		GiveInventory("Soundboard - Boss Kill",1);
		GiveInventory("Soundboard - Deathmatch Kill",1);
		GiveInventory("Soundboard - Dukematch Kill",1);
		GiveInventory("Soundboard - Boss Sight",1);
		GiveInventory("Soundboard - Enemy Kill",1);
		GiveInventory("Soundboard - Boot Kill",1);
		GiveInventory("Soundboard - Gib Kill",1);
		GiveInventory("Soundboard - Weapon Acquired",1);
		GiveInventory("Soundboard - Level Intro",1);
		GiveInventory("Soundboard - Idle Taunt",1);
		GiveInventory("Soundboard - Critical Heal",1);
		GiveInventory("Soundboard - Death Taunt",1);
		GiveInventory("Soundboard - Doug Huggem",1); }}
        else { TakeInventory("Soundboard - Boss Kill",1);
		TakeInventory("Soundboard - Deathmatch Kill",1);
		TakeInventory("Soundboard - Dukematch Kill",1);
		TakeInventory("Soundboard - Boss Sight",1);
		TakeInventory("Soundboard - Enemy Kill",1);
		TakeInventory("Soundboard - Boot Kill",1);
		TakeInventory("Soundboard - Gib Kill",1);
		TakeInventory("Soundboard - Weapon Acquired",1);
		TakeInventory("Soundboard - Level Intro",1);
		TakeInventory("Soundboard - Idle Taunt",1);
		TakeInventory("Soundboard - Critical Heal",1);
		TakeInventory("Soundboard - Death Taunt",1);
		TakeInventory("Soundboard - Doug Huggem",1); }
		
		SetInventory("SamsaraHasPermaWeaponLevel2", GetCVar("samsara_permaweaponlevel2") && (samsaraClassNum() == CLASS_HERETIC));
        
        HandleBans();
        HandleBuffCVars(respawning);
        
        TakeInventory("WeaponGetYaaaay",  1);
        TakeInventory("WeaponGetYaaaay2", 1);
        TakeInventory("Mace", 1);
        TakeInventory("MacePowered", 1);
        if (GetCvar("samsara_lmsrules") == 0) { ConvertClassWeapons(-1); }

        SetInventory("SamsaraLevelSwitch_HadDoE", CheckInventory("UsingDoEAmmo"));

        if (CheckInventory("SpectralFiring"))
        {
            TakeInventory("SpectralFiring", 1);
            GiveInventory("SpectralFiring", 1);
        }

        if (GetCVar("samsara_banjetpack") && CheckInventory("DukePortJetpack"))
        {
            GiveInventory("DukeBootserk", 1);
            TakeInventory("DukePortJetpack", 0x7FFFFFFF);
            TakeInventory("DukeJetpackFuel", 0x7FFFFFFF);
            Print(s:"The jetpack is banned on this server. Have 8x boot damage instead.");
        }

        for (i = 0; i < 10; i++)
        {
            if (CheckInventory("DoomZerkCounter") > i && !CheckInventory(DoomAccuracyZerks[i]))
            {
                GiveInventory(DoomAccuracyZerks[i], 1);
            }
        }

        for (i = 0; i < SIGILCOUNT; i++)
        {
            if (SamsaraGlobal[GLOBAL_SIGILBASE+1+i])
            {
                if (!SamsaraGlobal[GLOBAL_SIGILBASE+i+2]) { GiveInventory(SpectralTogglers[i], 1); }
                else { TakeInventory(SpectralTogglers[i], 0x7FFFFFFF); }
                GiveInventory(SigilSplinters[i], 1);
            }
            else
            {
                TakeInventory(SigilSplinters[i], 0x7FFFFFFF);
                TakeInventory(SpectralTogglers[i], 0x7FFFFFFF);
            }
        }

        // triggers buddha mode when Blazko has an extra life
        if (CheckInventory("WolfExtraLife") && !canbuddha) { SetPlayerProperty(0, 1, 16); }
        if (!CheckInventory("WolfExtraLife") && canbuddha) { SetPlayerProperty(0, 0, 16); }
        canbuddha = CheckInventory("WolfExtraLife");

        i = CheckInventory("Shell");

        if (CheckInventory("RWastemUnloaded")) { wsteSide = 1; }
        if (CheckInventory("LWastemUnloaded")) { wsteSide = 0; }

        if (i <= 2)
        {
            if (!CheckInventory("WastemEmpty"))  { GiveInventory("WastemEmpty", 1); };
            if (!CheckInventory("LWastemEmpty")) { GiveInventory("LWastemEmpty", 1); };
            if (!CheckInventory("RWastemEmpty")) { GiveInventory("RWastemEmpty", 1); };
        }
        else if (i <= 4)
        {
            if (CheckInventory("WastemEmpty"))  { TakeInventory("WastemEmpty", 0x7FFFFFFF); };

            if (!wsteSide)
            {
                if (!CheckInventory("LWastemEmpty")) { GiveInventory("LWastemEmpty", 1); };
                if (CheckInventory("RWastemEmpty")) { TakeInventory("RWastemEmpty", 0x7FFFFFFF); };
            }
            else
            {
                if (!CheckInventory("RWastemEmpty")) { GiveInventory("RWastemEmpty", 1); };
                if (CheckInventory("LWastemEmpty")) { TakeInventory("LWastemEmpty", 0x7FFFFFFF); };
            }
        }
        else
        {
            if (CheckInventory("WastemEmpty"))  { TakeInventory("WastemEmpty", 0x7FFFFFFF); };
            if (CheckInventory("LWastemEmpty") && !wsteSide) { TakeInventory("LWastemEmpty", 0x7FFFFFFF); };
            if (CheckInventory("RWastemEmpty") &&  wsteSide) { TakeInventory("RWastemEmpty", 0x7FFFFFFF); };
        }

        oarmor = armor;
        otype = type;
        armor = CheckInventory("BasicArmor");
        type = SamsaraArmorType();

        if (oarmor > armor && otype == 3)
        {
            if (samsaraClassNum() == CLASS_MARATHON)
            {
                ActivatorSound("marathon/shieldhit", 127);
                FadeRange(255, 255, 255, 0.4, 0, 0, 0, 0.0, 8.0 / 35);
            }
            else
            {
                ActivatorSound("samsara/shieldhit", 127);
                FadeRange(255, 255, 0, min(0.5, (oarmor - armor) * 0.015), 0, 0, 0, 0.0, min(35.0, 1.5 * (oarmor - armor)) / 35);
            }
        }

        /*
         * Jumping shit
         */

        switch (samsaraClassNum())
        {
          case CLASS_MARATHON:
            if (GetCVar("samsara_nocustomgravity"))
            {
                SetActorProperty(0, APROP_Gravity, 1.0);
                SetActorProperty(0, APROP_Mass, 100);
            } else {
                SetActorProperty(0, APROP_Gravity, 0.15);
                SetActorProperty(0, APROP_Mass, 220);
            }
            break;

          case CLASS_QUAKE:
            if (GetCVar("samsara_nocustomgravity"))
            {
                SetActorProperty(0, APROP_Gravity, 1.0);
                SetActorProperty(0, APROP_Mass, 100);
            } else {
                SetActorProperty(0, APROP_Gravity, 0.75);
                SetActorProperty(0, APROP_Mass, 100);
            }
            break;

          default:
            SetActorProperty(0, APROP_Gravity, 1.0);
            SetActorProperty(0, APROP_Mass, 100);
            break;
        }

        if (CheckInventory("ForceNormalGravity"))
        {
            SetActorProperty(0, APROP_Gravity, 1.0);
            SetActorProperty(0, APROP_Mass, 100);
        } else if (CheckInventory("ForceSOGravity")) {
            SetActorProperty(0, APROP_Gravity, 0.15);
            SetActorProperty(0, APROP_Mass, 220);
        } else if (CheckInventory("ForceRangerGravity")) {
            SetActorProperty(0, APROP_Gravity, 0.75);
            SetActorProperty(0, APROP_Mass, 100);
        }

        if (samsaraClassNum() == CLASS_HEXEN)  { i = JumpZFromHeight(41 + GetCVar("samsara_jumpmod"), GetActorProperty(0, APROP_Gravity)); }
        else { i = JumpZFromHeight(32 + GetCVar("samsara_jumpmod"), GetActorProperty(0, APROP_Gravity)); }

        SetActorProperty(0, APROP_JumpZ, max(i, 0));

        SetInventory("StopBreakingEverything_Bomb", 0);
        SetInventory("StopBreakingEverything_Quad", 0);

        //if (isDead(0)) { endloop = 1; }
        
        Delay(1);

        opcount = pcount;
        pcount  = PlayerCount();
    }
}

script "SamsaraTurkeyPuncher" ENTER
{
    int delayTics = GetCVar("samsara_superturboturkeyfrequency");

    if (delayTics <= 0) { delayTics = 1; }

	Delay(delayTics);

    if (GetCvar("samsara_superturboturkeypuncher3000") > 0)
    { GiveInventory("ChickenModeOn",1);
    
        if (GetCvar("samsara_superturboturkeypuncher3000") == 3)
        { str ChickenClass = "SuperFriendlyChicken";
        if (!random(0, 15))
        { ChickenClass = "AssaultFriendlyChicken";
        if (!random(0, 2))
        ChickenClass = "ExplosiveFriendlyChicken"; }}
    
        if (GetCvar("samsara_superturboturkeypuncher3000") == 2)
        { ChickenClass = "SuperDMChicken";
        if (!random(0, 15))
        { ChickenClass = "AssaultChicken";
        if (!random(0, 2))
        ChickenClass = "ExplosiveChicken"; }}
        
        if (GetCvar("samsara_superturboturkeypuncher3000") == 1)
        { ChickenClass = "SuperCoopChicken";
        if (!random(0, 15))
        { ChickenClass = "AssaultChicken";
        if (!random(0, 2))
        ChickenClass = "ExplosiveChicken"; }}
     
        do
        {
           delay(1);
           int x = GetActorX(0) + random(-128.0, 128.0);
           int y = GetActorY(0) + random(-128.0, 128.0);
           int z = GetActorCeilingZ(0) - 38.0;
           int angle = GetActorAngle(0) >> 8;
        }
        until (Spawn(ChickenClass, x, y, z, 0, angle));
        
        
        if (GetCvar("samsara_superturboturkeylimit") > 0)
        {
            if(CheckInventory("ChickenKillCount") >= (GetCvar("samsara_superturboturkeylimit")))
            { SetFont("BIGFONT");
            HudMessageBold(n:0,s:" \cjhit the kill limit!";
            HUDMSG_PLAIN, 0, CR_WHITE, 0.5, 0.2, 5.0);
            if(CheckInventory("ChickenKillCount") < (GetCvar("samsara_superturboturkeylimit")))
            {
                TakeInventory("ChickenKillCount",999); }
                Delay(35*5);
                Exit_Normal(0);
            }       
        }
        
    }
    else
    { TakeInventory("ChickenModeOn",999); }
    restart;
    //}
}

/*script 678 (int respawning)
{
    int punchdrunkloop;
    //punchdrunkloop = 0;
    
    print(s:"678 is being called");
    if (!respawning)
    {
        if (!punchdrunkloop)
        { delay(15);
          print(s:"punchdrunkloop 0 is being called");
          punchdrunkloop = 1; }
        else
        { HandlePunchdrunk(respawning);
        print(s:"handlepunchdrunk is being called");
        HandleInstagib(respawning); }
    }
    delay(1);
    restart;
}*/

script "SamsaraSynthFire" (int startTime)
{
    int pln = PlayerNumber();

    while (ServerEnterTimes[pln] == startTime)
    {
        if (CheckInventory("Health") > 0)
        {
        if (keyDown(BT_ATTACK)) { GiveInventory("SynthFireLeft", 1); }
        else { TakeInventory("SynthFireLeft", 0x7FFFFFFF); }
        
        if (keyDown(BT_ALTATTACK)) { GiveInventory("SynthFireRight", 1); }
        else { TakeInventory("SynthFireRight", 0x7FFFFFFF); }
        }

        Delay(1);
    }

    TakeInventory("SynthFireLeft", 0x7FFFFFFF);
    TakeInventory("SynthFireRight", 0x7FFFFFFF);
}

script "SamsaraConfirmClass" (int which) { SetResultValue(SamsaraWepType == which); }

script "SamsaraWolfMove" (void)
{ 
    int pln = PlayerNumber();
    int realspeed = GetActorProperty(0, APROP_Speed);
    int forward, side, angle, xthrust, ythrust;
    int forwardx, forwardy, sidex, sidey;
    int velx, vely, velz;
    int moving;
    int fired;
    int startTime = Timer();

    WolfenEnterTimes[pln] = startTime;

    while (WolfenEnterTimes[pln] == startTime)
    {
        if (UnloadingNow)
        {
            if (GetActorProperty(0, APROP_Speed) == 0)
            {
                SetActorProperty(0, APROP_Speed, realspeed);
            }
            break;
        }

        if (!CheckInventory("CanWolfMovement")) { break; }

        if (!CheckInventory("WolfenMovement"))
        {
            if (GetActorProperty(0, APROP_Speed) == 0) { SetActorProperty(0, APROP_Speed, realspeed); }

            SetInventory("WolfMoving", 0);

            Delay(1);
            continue;
        }

        if (isDead(0))
        {
            SetActorProperty(0, APROP_Speed, realspeed);
            velx = 0;
            vely = 0;
            velz = 0;
        }
        else
        {
            if (WolfenEnterTimes[pln] != startTime) { break; }
            SetActorProperty(0, APROP_Speed, 0);
            
            if(GetPlayerInput(-1,INPUT_FORWARDMOVE) != 0)
				forward = sign(GetPlayerInput(-1,INPUT_FORWARDMOVE));

            forward *= SPEED_FORWARD;
            
            if(GetPlayerInput(-1,INPUT_SIDEMOVE) != 0)
				side    = sign(GetPlayerInput(-1,INPUT_SIDEMOVE));
			
			if(KeyDown_Any(BT_STRAFE))
			{
				if(GetPlayerInput(-1,INPUT_YAW) != 0)
					side    = -sign(GetPlayerInput(-1,INPUT_YAW));
				}

            side    *= SPEED_SIDE;
            
            angle   = GetActorAngle(0);
            
            forwardx = cos(angle) * forward;
            forwardy = sin(angle) * forward;
            
            sidex = cos(angle-0.25) * side;
            sidey = sin(angle-0.25) * side;
            
            velx = forwardx + sidex;
            vely = forwardy + sidey;
            velz = sin(-GetActorPitch(0)) * forward;

			forward = 0;
			side = 0;
        }

        bool isRunning = (!GetCVar("cl_run") && keyDown(BT_SPEED)) || (GetCVar("cl_run") && !keyDown(BT_SPEED));

        if (!isRunning)
        {
            velx /= 2;
            vely /= 2;
            velz /= 2;
        }

        bool isMorphed = CheckActorClass(0, "ChickenPlayer") || CheckActorClass(0, "ChickenPlayer2") || CheckActorClass(0, "ChickenPlayer3") ||
                         CheckActorClass(0, "PigPlayer"); // hack variable

        if (!GetCVar("sv_nocrouch") && !isMorphed && GetActorViewHeight(0) <= 28.0)
        {
            velx /= 2;
            vely /= 2;
            velz /= 2;
        }
        
        SetInventory("WolfMoving", velx != 0 || vely != 0 || velz != 0);
        
        if (GetActorProperty(0, APROP_WaterLevel) >= 1 || CheckInventory("PowerFlight") || CheckInventory("PowerFlight2") || GetActorProperty(0, APROP_Gravity) == 0.0)
        {
            SetActorVelocity(0, velx, vely, velz, 0, 0);
        }
        else
        {
            SetActorVelocity(0, velx, vely, GetActorVelZ(0), 0, 0);
        }

        Delay(1);
    }

    if (GetActorProperty(0, APROP_Speed) == 0)
    {
        SetActorProperty(0, APROP_Speed, realspeed);
    }
}

script "SamsaraEnterClient" (void) clientside
{
    int execInt, oExecInt, execStr;
    int class, oClass;
    int pln = PlayerNumber();
    int i, j;
    int startTime = Timer();
    
    // Comment out this line for zdoom
    int cpln = ConsolePlayerNumber();

    ClientEnterTimes[pln] = startTime;
    ClientTipboxes[pln] = 0;
    
    execInt = 0; oExecInt = 0;
    
    //if (GetCVar("samsara_cl_exists") != SAMSARA_CL_VERSION) {}

    for (i = 0; i < RESCOUNT; i++)
    {
        ACS_NamedExecuteAlways("SamsaraResonate", 0, i, 16, 0);
    }
    
    DukeQuoteCooldown[pln] = 0; 

    //Log(s:"Client ", n:pln+1, s:"\c- (", d:pln, s:") has spawned (startTime is ", d:startTime, s:")");
    
    while (ClientEnterTimes[pln] == startTime)
    {
        oClass = class;
        class  = samsaraClassNum();
        DukeQuoteCooldown[pln] = max(0, DukeQuoteCooldown[pln]-1); 

        // Also this line
        if (cpln != pln) { Delay(1); continue; }
        
        SamsaraClientClass = class+1;
        
        if (oClass != class) { SamsaraItemFlash = Timer(); }
        
        for (i = 0; i < SLOTCOUNT; i++)
        {
            j = SamsaraClientWeps[i];
            SamsaraClientWeps[i] = HasClassWeapon(class, i, 1);
            
            if (j != SamsaraClientWeps[i]) { SamsaraClientWepFlashes[i] = Timer(); }
        }
        
        if (IsServer)
        {
            array_wolfmove[pln]     = !!GetCVar("samsara_cl_wolfmove");
            array_vanillaAnim[pln]  = !!GetCVar("samsara_cl_smoothanims");
            array_ballgag[pln]      = !!GetCVar("samsara_cl_ballgag");
            array_weaponBar[pln]    = !!GetCVar("samsara_cl_weaponhud");
            array_pickupswitch[pln] = !!GetCVar("switchonpickup");
        }
        else
        {
            oExecInt = execInt;
            execInt = SamsaraClientVars();
            
            if (execInt != oExecInt)
            {
                execStr = StrParam(s:"puke -", d:SAMSARA_PUKE, s:" ", d:execInt, s:" ", d:pln);
                //ConsoleCommand(execStr);
            }
        }

        Delay(1);
    }
    //Log(s:"Client ", n:pln+1, s:"\c- (", d:pln, s:") has left spawn (", d:startTime, s:" vs. ", d:ClientEnterTimes[pln], s:")");
}

script "SamsaraDisconnectClient" (int pln) disconnect clientside
{
    // Comment out these lines for zdoom
    int cpln = ConsolePlayerNumber();
    if (cpln != pln) { terminate; }
    
    SamsaraClientClass  = 0;
    SamsaraItemFlash    = Timer();
}

script "SamsaraPuke" (int values, int pln) net
{
    array_wolfmove[pln]     = values & 1;
    array_vanillaAnim[pln]  = values & 2;
    array_ballgag[pln]      = values & 4;
    array_weaponBar[pln]    = values & 8;
    array_pickupswitch[pln] = values & 16;
}
