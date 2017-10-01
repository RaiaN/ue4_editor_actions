#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Blueprint.h"
#include "Engine/SimpleConstructionScript.h"

// This class cracks open the selected actors, harvests their components, and creates a new blueprint containing copies of them
class FCreateBlueprintWithChildActorComponents
{
public:
	FCreateBlueprintWithChildActorComponents() : Blueprint(nullptr), SCS(nullptr)
	{
	}

	UBlueprint* Execute(const FString& Path, TArray<AActor*> SelectedActors, bool bReplaceInWorld);

protected:
	UBlueprint* Blueprint;
	USimpleConstructionScript* SCS;
};