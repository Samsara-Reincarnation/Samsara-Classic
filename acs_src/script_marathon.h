script SAMSARA_MARATHON (int class, int slot, int dropped)
{
    int giveboth    = isInvasion() || !(isCoop() || isSinglePlayer()) || GetCVar("sv_itemrespawn") || GetCVar("sv_weaponstay");
    int hasShotty   = CheckInventory("WSTE-M5 Combat Shotgun");
    int hasBoth     = CheckInventory("CanDualShotties");
    int limited     = CheckInventory("LevelLimiter");
    int limit       = GetCVar("sv_itemrespawn") || GetCVar("sv_weaponstay");
    int ammoFull    = CheckInventory("Shell") >= (GetAmmoCapacity("Shell") / (((dropped >= 2)*3)+1));
    int i;
    // The above line is because of the quadrupling of ammo capacity with dropped pickups
    // It's a really gross hack. I hate it. But it works.
    
    switch (slot)
    {
      case 1:
        GiveInventory("CanDualPistols", 1);
        GiveInventory("KKV-7 SMG Flechette", 1);
        break;
        
      case 3:
        if (limited && dropped <= 0)
        {
            SetResultValue(0);
            terminate;
        }

        if ((dropped >= 2 && hasShotty && ammoFull) ||
            (dropped == 1 && hasBoth && ammoFull) ||
            (dropped <= 0 && (hasBoth || (hasShotty && limited)) && ammoFull))
        {
            SetResultValue(0);
            terminate;
        }
        
        GiveInventory("Shell", 8 / (!!(dropped >= 2)+1));
        GiveInventory("WSTE-M5 Combat Shotgun", 1);
        
        if (giveboth || (hasShotty && dropped <= 1))
        {
            GiveInventory("CanDualShotties", 1);
        }
        
        if (limit && dropped <= 0)
        {
            GiveInventory("LevelLimiter", 1);
        }
        SetResultValue(1);
        break;
		
      case 4:
        GiveInventory("Fusion Pistol", 1);
		
		if (dropped >= 2)
		{
			GiveInventory("FusionBullet", 20);
			GiveInventory("Cell", 10);
		}
		else 
		{
			GiveInventory("FusionBullet", 20);
			GiveInventory("Cell", 40);
		}
		
        SetResultValue(1);
        break;
		
      case 5:
        GiveInventory("MA-75B Assault Rifle", 1);
		
		if (dropped >= 2)
		{
			GiveInventory("RifleBullet", 52);
			GiveInventory("Clip", 13);
		}
		else 
		{
			GiveInventory("RifleBullet", 52);
			GiveInventory("Clip", 52);
			GiveInventory("GrenadeInClip", 7);
			GiveInventory("RocketAmmo", 7);
		}
		
        SetResultValue(1);
        break;
		
      case 6:
        GiveInventory("SPNKR-XP SSM Launcher", 1);
	
		GiveInventory("SpankerAmmo", 2);
		GiveInventory("RocketAmmo", 2);
		
        SetResultValue(1);
        break;
		
      case 7:
        GiveInventory("TOZT-7 Napalm Unit", 1);
		
		if (dropped >= 2)
		{
			GiveInventory("NapalmInTank", 210);
			GiveInventory("Cell", 105);
		}
		else 
		{
			GiveInventory("NapalmInTank", 210);
			GiveInventory("Cell", 210);
		}
		
        SetResultValue(1);
        break;
		
      case 8:
        GiveInventory("ONI-71 Wave Motion Cannon", 1);
		
		if (dropped >= 2)
		{
			GiveInventory("Cell", 20);
		}
		else 
		{
			GiveInventory("Cell", 40);
			GiveInventory("RocketAmmo", 15);
		}
		
        SetResultValue(1);
        break;

      default:
        GiveInventory(ClassWeapons[class][slot][S_WEP], 1);

        if (ClassWeapons[class][slot][S_AMMO1] != "")
        {
            i = ammoCount(ClassWeapons[class][slot][S_AMMO1]);
            GiveInventory(ClassWeapons[class][slot][S_AMMO1], i/4);
        }

        if (ClassWeapons[class][slot][S_AMMO2] != "")
        {
            i = ammoCount(ClassWeapons[class][slot][S_AMMO2]);
            GiveInventory(ClassWeapons[class][slot][S_AMMO2], i/4);
        }

        SetResultValue(1);
        break;
    }
}

script "SamsaraResonate" (int which, int frequency) clientside
{
    if (which < 0 || which >= RESCOUNT) { Log(s:"\caERROR\c- (minor): Attempted to resonate with invalid index (", d:which, s:")"); terminate; }

    int pln = PlayerNumber();
    int locktime = Timer();

    int item = ResonantItems[which][0];
    int low  = ResonantItems[which][1];
    int high = ResonantItems[which][2];

    if (frequency == 0) { Log(s:"\caERROR\c-: Attempted to resonate item \"", s:item, s:"\" with frequency of 0"); terminate; }
    frequency = abs(frequency);

    int resPosition     = random(low, high);
    int resNextPos      = resPosition;
    int resAmplitude    = 0;
    int resIndex        = 0;
    int resTimer;
    int resonance;

    ResonantTimes[pln][which] = locktime;

    while (ResonantTimes[pln][which] == locktime)
    {
        if (abs(resPosition - resNextPos) <= 2) { resPosition = resNextPos; }
        else { resPosition += (resNextPos - resPosition) / 3; }

        if (resAmplitude == 0) { resTimer = 0; }

        if (resTimer == 0)
        {
            resAmplitude += random(1.0, 20.0);
            resTimer = random(1, 70);
            resNextPos = random(low, high);
        }

        resonance = FixedMul(resAmplitude, sin(itof(resIndex) / frequency)) + itof(resPosition);
        resAmplitude = max(0, resAmplitude - 0.5);

        TakeInventory(item, 0x7FFFFFFF);
        GiveInventory(item, ftoi(resonance));

        Delay(1);
        resTimer--;
        resIndex++;
        resIndex %= (frequency * 100);
    }
}
