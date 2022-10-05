#include "GPWaveManager.h"

#include <string>

#include "EnemySpawnLocations.h"
#include "EnemySpawnLocations.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GP_EnemyPrototype.h"
#include "GameModePrototype.h"

using namespace std;

AGPWaveManager::AGPWaveManager()
{
	
      
    
    
  

}


void AGPWaveManager::StartOfGame()
{
    GetWorldTimerManager().ClearTimer(TimerHandle);
    if (BeforeFirstwaveSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, BeforeFirstwaveSound, GetActorLocation());
    }
    startOfGameEvent();
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AGPWaveManager::setwave, 1.f, true, FirstWaveDelay);
}

void AGPWaveManager::BeginPlay()
{
   
  

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGPEnemySpawnLocation::StaticClass(),SpawnPoints);

   
    spawnOrder = FMath::RandRange(0, 5);
    
	//Save Spawn point objects
	for (int i = 0; i < SpawnPoints.Num(); i++)
        {
            
            someSpawnPoints.Add(Cast<AGPEnemySpawnLocation>(SpawnPoints[i]));
            
        }
   
	// Sort spawn points by name
	for(int j=0; j < someSpawnPoints.Num(); j++)
   {
       for (int i = 0; i < someSpawnPoints.Num(); i++)
       {
        
           FString index = FString::FromInt(j);
           SpawnPoint= someSpawnPoints[i]->Name.ToString();

           //Where SpawnPoint is == to index
			if(SpawnPoint==index)
			{
				 tmp = someSpawnPoints[j];
				 someSpawnPoints[j] = someSpawnPoints[i];
				 someSpawnPoints[i] = tmp;
			}    
       }
   }
    AGameModePrototype* Gamemode = Cast<AGameModePrototype>(UGameplayStatics::GetGameMode(GetWorld()));

    Gamemode->Wavemanager = this;
 

    spawnLocs.Add(SpawnLoc(TEXT("Garden"), false));
    spawnLocs.Add(SpawnLoc(TEXT("Library"), false));
    spawnLocs.Add(SpawnLoc(TEXT("Throne Room"), false));

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AGPWaveManager::StartOfGame, 1.f, true, StartOfGameDelay);

   
}



int AGPWaveManager::GetRandomSpawnLocation()
{
    int num = FMath::RandRange(0, 2);
    return num;
}
int AGPWaveManager::GetRandomSpawnPoint(TArray<AGPEnemySpawnLocation*> Heart)
{
    if (Heart.Num() == 0)
    {
        return 1;
    }
    int num = FMath::RandRange(1, Heart.Num());
    return num;
}
int AGPWaveManager::RandomSpawnPoint(TArray<AGPEnemySpawnLocation*> Heart)
{
    if (Heart.Num() == 0)
    {
        return 1;
    }
    int num = FMath::RandRange(0, Heart.Num() - 1);
    return num;
}

FString AGPWaveManager::GetAttackText()
{
    return AttackStateText;
}

TArray<FString> AGPWaveManager::GetActiveSpawnLocations()
{
    TArray<FString> t;
    for(int i = 0; i < spawnLocs.Num(); i++)
    {
        if(spawnLocs[i].ActiveState == true)
        {
            t.Add(spawnLocs[i].Name);
        }
    }
    return t;
}

FString AGPWaveManager::ChooseMixDialogueAndCombine()
{
    TArray<FString> MixDialogues = GetActiveSpawnLocations();
    FString s1 = MixDialogues[0];
    FString s2 = MixDialogues[1];
    
    FString f1 = "“Sir Barnabas! Thou must defend the " + s1 + " and " + s2 + " or we shall perish!”";
    FString f2 = "“You witless drudge! The " + s1 + " and " + s2 + " are crawling with enemies!”";
    FString f3 = "“The " + s1 + " & " + s2 + " will be conquered if thou doth not collect thyself!”";
    FString f4 = "“Agony and pain radiate from " + s1 + " and " + s2 + ", help me O’ Noble Knight...”";
    FString f5 = "“The " + s1 + " and " + s2 + " are being laid to waste, take back our ground!”";
    
    MixDialogues.Add(f1);
    MixDialogues.Add(f2);
    MixDialogues.Add(f3);
    MixDialogues.Add(f4);
    MixDialogues.Add(f5);

    int randomIndex = FMath::RandRange(0, 4);

    return MixDialogues[randomIndex]; 
}

FString AGPWaveManager::ChooseSingleDialogue()
{
    FString LocText = GetActiveSpawnLocations()[0];

    TArray<FString> Throne
    {
        "“They target my heart within the Throne Room, Sir Barnabas!”",
        "“My Knight! The Throne Room is being overrun!”"
    };
    TArray<FString> Library
    {
        "“With haste, Sir Barnabas, to the Library!”",
        "“Vanquish those in the Library!”"
    };
    TArray<FString> Garden
    {
        "“Their blades pierce my heart, vanquish the horde in the Gardens!”",
        "“Cease thine foolishness! The assault is upon the Gardens!”"
    };

    int randomIndex = FMath::RandRange(0, 1);

    if(LocText == "Throne Room")
        return Throne[randomIndex];
    
    if(LocText == "Library")
        return Library[randomIndex];
    
    if(LocText == "Garden")
        return Garden[randomIndex];
    
    return "Error: Something went wrong";
}

void AGPWaveManager::setwave()
{
    GetWorldTimerManager().ClearTimer(TimerHandle);
   



    spawnLocs[0].ActiveState = false;
    spawnLocs[1].ActiveState = false;
    spawnLocs[2].ActiveState = false;

    DoneSpawnPoints1 = 0;
    DoneSpawnPoints2 = 0;
    DoneSpawnPoints3 = 0;

    TimeBetween = TimeBetweenWaves;
    DoneSpawnLocations = 0;
    active1 = true;
    active2 = true;
    active3 = true;


    if (WaveIndex < 4)
    {

        spawnLocs[RandomSpawnOrders[spawnOrder][WaveIndex - 1]].ActiveState = true;
    }
    else if(WaveIndex<ActiveSpawnLocations.Num())
    {
        TArray<int> arr = { 0,1,2 };
        for (int i = 0; i < ActiveSpawnLocations[WaveIndex]; i++)
        {
            int num = FMath::RandRange(0, arr.Num() - 1);
            spawnLocs[arr[num]].ActiveState = true;
            arr.RemoveAt(num);
        }
    }
    else
    {
	    spawnLocs[0].ActiveState = true;

        spawnLocs[1].ActiveState = true;
        spawnLocs[2].ActiveState = true;
    }


    if (spawnLocs[0].ActiveState == true)
    {
        
        ActiveSpawnPoints1 = Heart1.Num();
    }
    else
    {
        ActiveSpawnPoints1 = 0;
    }
    if (spawnLocs[1].ActiveState == true)
    {
      
        ActiveSpawnPoints1 = Heart2.Num();
    }
    else
    {
        ActiveSpawnPoints2 = 0;
    }
    if (spawnLocs[2].ActiveState == true)
    {
        
        ActiveSpawnPoints1 = Heart3.Num();
    }
    else
    {
        ActiveSpawnPoints3 = 0;
    }
    LocationRest = WaveSize % ActiveSpawnLocations[WaveIndex];
    rest = WaveSize % (ActiveSpawnPoints1 + ActiveSpawnPoints2 + ActiveSpawnPoints3);
    LocationAmount = (WaveSize - rest) / ActiveSpawnLocations[WaveIndex];
    SpawnPointAmount = (WaveSize - rest) / (ActiveSpawnPoints1 + ActiveSpawnPoints2 + ActiveSpawnPoints3);
   

    //SpawnPointWaveSize

    if (active1)
    {
        SpawnPointCopy1 = (Heart1);
        active1 = false;
    }
    SpawnPointIndex1 = FMath::RandRange(0, SpawnPointCopy1.Num() - 1);
    if (active2)
    {
        SpawnPointCopy2 = (Heart2);
        active2 = false;
    }
    SpawnPointIndex2 = FMath::RandRange(0, SpawnPointCopy2.Num() - 1);

    if (active3)
    {
        SpawnPointCopy3 = (Heart3);
        active3 = false;
    }
    SpawnPointIndex3 = FMath::RandRange(0, SpawnPointCopy3.Num() - 1);


    newLocation = false;

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AGPWaveManager::SpawnEnemies, SecondsBetweenSpawns, true, 1.f);



    PrintAttackText();

    if (AttackedHeartsSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, AttackedHeartsSound, GetActorLocation());
    }

    //Call SelectedHearts in Gamemode
    AGameModePrototype* Gamemode = Cast<AGameModePrototype>(UGameplayStatics::GetGameMode(GetWorld()));

    if (Gamemode)
    {
        Gamemode->SelectedHearts();
    }
}


void AGPWaveManager::SpawnEnemies()
{
    if(WaveIndex<ActiveSpawnLocations.Num())
    {
        if (DoneSpawnLocations == ActiveSpawnLocations[WaveIndex])
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
            GetWorldTimerManager().SetTimer(TimerHandle, this, &AGPWaveManager::Pending, 1.f, true, 1.f);
        }
    }
    else if(DoneSpawnLocations == ActiveSpawnLocations[0])
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
        GetWorldTimerManager().SetTimer(TimerHandle, this, &AGPWaveManager::Pending, 1.f, true, 1.f);
    }
  
    if (rest != 0)
    {
        if (spawnLocs[0].ActiveState == true && rest != 0)
        {
            Heart1[RandomSpawnPoint(Heart1)]->SpawnEnemies();
            rest--;
            AliveEnemies++;
        }
        if (spawnLocs[1].ActiveState == true && rest != 0)
        {
            Heart2[RandomSpawnPoint(Heart2)]->SpawnEnemies();
            rest--;
            AliveEnemies++;
        }
        if (spawnLocs[2].ActiveState == true && rest != 0)
        {
            Heart3[RandomSpawnPoint(Heart3)]->SpawnEnemies();
            rest--;
            AliveEnemies++;
        }


    }
    else
    {
	    
    
    if(spawnLocs[0].ActiveState)
    {
        if(counter1 != LocationAmount)
        {

            for (int i = 0; i <3;i++ )
            {
                Heart1[i]->SpawnEnemies();
                counter1++;
                AliveEnemies++;
            }

            if (counter1 >= LocationAmount)
            {
                DoneSpawnPoints1++;
                counter1 = 0;
            	spawnLocs[0].ActiveState = false;
                newLocation = true;
                counter1 = 0;
                active1 = true;
                DoneSpawnLocations++;
                
            }


        }


    }
    if (spawnLocs[1].ActiveState)
    {
        if (counter2 != LocationAmount)
        {

            for (int i = 0; i < 3; i++)
            {
                Heart2[i]->SpawnEnemies();
                counter2++;
                AliveEnemies++;
            }

            if (counter2 >= LocationAmount)
            {
                DoneSpawnPoints2++;
                counter2 = 0;
                spawnLocs[1].ActiveState = false;
                newLocation = true;
                counter2 = 0;
                active2 = true;
                DoneSpawnLocations++;

            }


        }
    }
    if(spawnLocs[2].ActiveState)
    {
        if (counter3 != LocationAmount)
        {

            for (int i = 0; i < 3; i++)
            {
                Heart3[i]->SpawnEnemies();
                counter3++;
                AliveEnemies++;
            }

            if (counter3 >= LocationAmount)
            {
                DoneSpawnPoints3++;
                counter3 = 0;
                spawnLocs[2].ActiveState = false;
                newLocation = true;
               
                active3 = true;
                DoneSpawnLocations++;

            }


        }

    }
 
    }
}






void AGPWaveManager::Pending()
{

    if(AliveEnemies==0)
    {
       
        if (TimeBetween == 0)
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
          if(WaveIndex>=IncreaseWaveBy.Num())
          {
              WaveSize += 5;

          }
          else
          {
              WaveSize += IncreaseWaveBy[WaveIndex];
          }
            LocationIndex++;
            WaveIndex++;
            EndOfWave();
           Cast<AGameModePrototype>(UGameplayStatics::GetGameMode(GetWorld()))->ClearHearts();
            GetWorldTimerManager().SetTimer(TimerHandle, this, &AGPWaveManager::setwave, 1.f, true, TimeBetweenWaves);

        }
        else
        {
            TimeBetween--;
	        
        }

     
   
    }
 

}



void AGPWaveManager::EnemyKilled()
{
    AliveEnemies--;
}

