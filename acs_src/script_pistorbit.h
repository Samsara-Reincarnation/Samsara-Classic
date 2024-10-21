// There was no alternative.

script "SamsaraSpaceFlight" (void)
{
    int i, j, k, l;
    int myTID = defaultTID(-1);
    int class = samsaraClassNum();
    if (class < 0 || class >= CLASSCOUNT) { class = 0; }

    int myx = GetActorX(0);
    int myy = GetActorY(0);
    int myz = GetActorZ(0);

    if (PlayerNumber() != -1) { SetPlayerProperty(false, false, PROP_FROZEN); }
    SetActorVelocity(0, 0, 0, 16.0, false, false);

    for (l = 0; l < 15; l++)
    {
        i = random(0, 1.0);
        j = random(0, 1.0);
        k = unusedTID(20000, 25000);

        SpawnForced("SamsaraSpaceExhaust", GetActorX(0), GetActorY(0), GetActorZ(0), k);
        SetActorVelocity(k, 6 * FixedMul(cos(i), cos(j)), 6 * FixedMul(sin(i), cos(j)), 1 * sin(j) + 2.0, false, false);
    }

    Delay(12);
    GiveInventory("SamsaraSpaceReady", 1);

    j = random(2.0,3.0);

    for (i = 0; i < 105; i++)
    {
        SetActorVelocity(0, 5 * sin(itof(i) / 18), 5 * cos(itof(i) / 18), i * j, false, false);
        GiveInventory("SamsaraSpaceTrail", 1);
        Delay(1);
    }

    SpawnForced(PeopleDieBlasts[class], GetActorX(0), GetActorY(0), GetActorZ(0));

    if (!isDead(0)) { SetActorProperty(0, APROP_Health, 1); }

    i = random(0, 1.0);
    j = random(0, 1.0);

    SetActorVelocity(0, 50 * FixedMul(cos(i), cos(j)), 50 * FixedMul(sin(i), cos(j)), 50 * sin(j), false, false);

    k = 0;

    if (GetActorCeilingZ(0) >= GetActorZ(0))
    {
        GiveInventory("SamsaraSpaceRelease", 1);
        k = 1;
    }

    DamageThing(0);
}
