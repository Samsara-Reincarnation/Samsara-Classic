#define ACCURACYCOUNT   10
#define SIGILCOUNT      5

int SigilSplinters[SIGILCOUNT] = 
{
    "SigilSplinter1",
    "SigilSplinter2",
    "SigilSplinter3",
    "SigilSplinter4",
    "SigilSplinter5",
};

int SpectralTogglers[SIGILCOUNT] = 
{
    "SpectralToggler1",
    "SpectralToggler2",
    "SpectralToggler3",
    "SpectralToggler4",
    "SpectralToggler5",
};

int BeaconMonsters[CLASSCOUNT] = 
{
    "FriendlyDoomImp",
    "FriendlyFlemoid",
    "FriendlyNitrogolem",
    "FriendlyNazi",
    "FriendlyAfrit",
    "FriendlyAlienTrooper",
    "FriendlyPfhorTrooper",
    "FriendlyGrunt",
};

int AccuracyUniques[CLASSCOUNT][ACCURACYCOUNT] = 
{
    {"DoomZerkCounter",  "", "", "", "", "", "", "", "", ""},
    {"ChexSoulsphere",   "", "", "", "", "", "", "", "", ""},
    {"ArtiEgg2",         "", "", "", "", "", "", "", "", ""},
    {"WolfExtraLife",    "", "", "", "", "", "", "", "", ""},
    {"PortMysticAmbit",  "", "", "", "", "", "", "", "", ""},
    {"DukeguyStrifeJet", "", "", "", "", "", "", "", "", ""},
    {"RandAlienWeapon",  "", "", "", "", "", "", "", "", ""},
    {"Laser Cannon",     "RangerMaxCells", "", "", "", "", "", "", "", ""},
};

int DoomAccuracyZerks[ACCURACYCOUNT] = 
{
    "DoomguyStrifeZerk1",
    "DoomguyStrifeZerk2",
    "DoomguyStrifeZerk3",
    "DoomguyStrifeZerk4",
    "DoomguyStrifeZerk5",
    "DoomguyStrifeZerk6",
    "DoomguyStrifeZerk7",
    "DoomguyStrifeZerk8",
    "DoomguyStrifeZerk9",
    "DoomguyStrifeZerk0",
};

int AccuracyMessages[CLASSCOUNT][MSGCOUNT] = 
{
    {
        "You grow more berserk.",
        "Someone spiked the punch.",
        "Plot twist: you were Baratus all along.",
        "You're not actually getting angrier. You're just punching more accurately.",
        "Yes... Channel your anger...",
        "Well, if you insist on using your fists...",
        "I'm sure those reinforced steel robots are trembling over your fleshy hands.",
        "Let's settle this dispute like gentlemen! Or kill each other. Both work.",
        "It was a ruse! You were secretly in a boxing match all along!",
        "Don't get too cocky, Mikey.",
    },
    {
        "Supercharge Breakfast!",
        "OM NOM NOM NOM",
        "Because medikits are for wimps.",
        "Crunchy.",
        "Chex marks the spot!",
        "Is this cannibalism or what?",
        "It's milk you can chew!",
        "Free breakfast with every training!",
        "How generous of him to provide you breakfast, too.",
        "So, uh, where did you get the milk from? The cows are dead.",
    },
    {
        "Morph Ovum",
        "Parthoris Fried Chicken.",
        "Because nothing's better than turning a templar harmless.",
        "I'm calling fowl on this.",
        "Don't eat it.",
        "I'm sure there's hundreds of egg puns you can use.",
        "Omelettes, break some eggs, etc.",
        "",
        "",
        "",
    },
    {
        "You got an extra life!",
        "Maybe now you can go to college.",
        "Works in Zandronum!",
        "One day, the villains will learn how to respawn, too.",
        "Now you can hug a giant metal robot with no repercussions.",
        "Good for the soul, too.",
        "Go ahead, taunt those inquisitors.",
        "",
        "",
        "",
    },
    {
        "Mystic Ambit Incant",
        "Heal plz.",
        "Warning: Not for use in deathmatch.",
        "A crowd on demand.",
        "Well, you're a cleric. Gotta have a heal.",
        "Plot twist: it's a badly-inscribed poison spell.",
        "This would probably be more useful if Strife could do coop.",
        "The rebels already have a doctor...",
        "",
        "",
    },
    {
        "Jetpack",
        "I believe I can touch the sky.",
        "Guaranteed to make friends with mappers.",
        "And this is why Virus will never be in.",
        "Because you have that on your sprite for a reason.",
        "Watch your ass, the jet burns.",
        "The gas mileage is \caterrible\c-.",
        "And fuck you, too, fall damage.",
        "",
        "",
    },
    {
        "You got th@nS0-.'\casystem error \cd0xfded",
        "The difference between a room of aliens and a carpet of them.",
        "What fun to watch you work.",
        "~text interface term\ciinal malfunction error \cd~2992dud",
        "your former self destroyed the dreaming way is eased",
        "down to the crushing center and spared the dance of forever",
        "This will allow you to kill more aliens. Does that make you happy?",
        "shedding all the weight of imagined moments and fear",
        "because fear is not the blood",
        "I know who you are.",
    },
    {
        "You got the laser cannon!",
        "If this were in vanilla Quake, it would be slot 6. \ckbut alas",
        "Are any of the weapon designs in Quake even practical?",
        "Do not aim at face. Not that it'll help.",
        "Does it come in blueberry?",
        "What is this, Star Wars?",
        "Bouncy red death!",
        "Pewpewlasers!",
        "Ia ia fhtagn, you flying squash.",
        "Ia ia fhtagn, you floating flower pot.",
    },
};

script "SamsaraStrifeMinesTransmitterUniqueGiver" ENTER
{
    if (CheckInventory("SamsaraMinesTransmitterUniqueGiven") || GetLevelInfo(LEVELINFO_LEVELNUM) != 14) { terminate; }

    // keep a constant check on our stats to ensure they function as intended
    int orighp = GetActorProperty(0, APROP_Health);
    int origsta = GetActorProperty(0, APROP_Stamina);
    int origacc = GetActorProperty(0, APROP_Accuracy);

    while (true)
    {
        if (CheckInventory("QuestItem29"))
        {
            if (!CheckInventory("SamsaraMinesTransmitterUniqueGiven"))
            {
                if (!isDead(0)) { SetActorProperty(0, APROP_Health, orighp); }
                SetActorProperty(0, APROP_Stamina, origsta);
                SetActorProperty(0, APROP_Accuracy, origacc);
                GiveInventory("UpgradeStamina", 10);
                GiveInventory("StrifeAccuracyReplacement", 1);
                GiveInventory("SamsaraMinesTransmitterUniqueGiven", 1);
            }

            terminate;
        } else {
            orighp = GetActorProperty(0, APROP_Health);
            origsta = GetActorProperty(0, APROP_Stamina);
            origacc = GetActorProperty(0, APROP_Accuracy);
        }

        Delay(1);
    }
}

script "SamsaraStrifeAcolyteBlueDeathChecker" (void) // Haaaaack.
{
    if (!isZDoom() && !ThingCountName("AcolyteBlue2", 0))
    {
        for (int i = 0; i < PLAYERMAX; i++)
        {
            if (!PlayerInGame(i)) { continue; }

            SetPlayerActivator(i);

            if (!isDead(0) && !CheckInventory("QuestItem7"))
            {
                bool hadcommunicator = CheckInventory("Communicator"); // bypass item requirement if you're cheating

                GiveInventory("QuestItem7", 1);

                if (!hadcommunicator) { GiveInventory("Communicator", 1); }
                SendToCommunicator(14, 0, 14, 0);
                if (!hadcommunicator) { TakeInventory("Communicator", 0x7FFFFFFF); }
            }
        }
    }
}
