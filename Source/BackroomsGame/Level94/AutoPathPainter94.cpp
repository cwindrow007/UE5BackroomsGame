#include "AutoPathPainter94.h"

#include "AutoPathPainter94.h"
#include "Landscape.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAutoPathPainter94::AAutoPathPainter94()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAutoPathPainter94::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AAutoPathPainter94::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAutoPathPainter94::PaintRandomPaths(ALandscape* Landscape, int32 numPaths)

{
    if (!Landscape || !DirtMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("No landscape or path material available for path painting"));
        return;
    }

    FRandomStream RandomStream(FDateTime::Now().GetTicks());

    for (int32 i = 0; i < numPaths  ; ++i)
    {
        FVector Start = GetRandomPointOnLandscape(Landscape, RandomStream);
        FVector End = GetRandomPointOnLandscape(Landscape, RandomStream);
        PaintPathBewteenTwoPoints(Start, End, Landscape);
    }
}

void AAutoPathPainter94::PaintPathBewteenTwoPoints(const FVector& Start, const FVector& End, ALandscape* Landscape)
{
    FVector PathDirection = (End - Start).GetSafeNormal();
    float PathLength = FVector::Dist(Start, End);
    float DecalSize = 100.0f; // Adjust this value as needed

    FVector CurrentLocation = Start;
    while (FVector::Dist(CurrentLocation, End) > DecalSize)
    {
        FRotator Rotation = PathDirection.Rotation();
        FTransform DecalTransform(Rotation, CurrentLocation);

        UDecalComponent* Decal = NewObject<UDecalComponent>(this);
        Decal->SetWorldTransform(DecalTransform);
        Decal->SetDecalMaterial(DirtMaterial);
        Decal->DecalSize = FVector(DecalSize, DecalSize, DecalSize);
        Decal->RegisterComponentWithWorld(GetWorld());

        CurrentLocation += PathDirection * DecalSize;
    }
}

FVector AAutoPathPainter94::GetRandomPointOnLandscape(ALandscape* Landscape, FRandomStream& RandomStream)
{
    FVector LandscapeOrigin;
    FVector LandscapeBoundsExtent;
    Landscape->GetActorBounds(false, LandscapeOrigin, LandscapeBoundsExtent);

    FVector LandscapeMin = LandscapeOrigin - LandscapeBoundsExtent;
    FVector LandscapeMax = LandscapeOrigin + LandscapeBoundsExtent;

    FVector RandomPoint;
    RandomPoint.X = RandomStream.FRandRange(LandscapeMin.X, LandscapeMax.X);
    RandomPoint.Y = RandomStream.FRandRange(LandscapeMin.Y, LandscapeMax.Y);

    // Perform a line trace to find the height at the random point
    FVector StartLocation = FVector(RandomPoint.X, RandomPoint.Y, 10000.0f);
    FVector EndLocation = FVector(RandomPoint.X, RandomPoint.Y, -10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
    {
        RandomPoint.Z = HitResult.Location.Z;
    }
    else
    {
        RandomPoint.Z = LandscapeOrigin.Z; // Default to landscape origin height if trace fails
    }

    return RandomPoint;
}
