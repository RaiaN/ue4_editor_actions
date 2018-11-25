// Copyright Peter Leontev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class FCreateBlueprintWithChildActorComponentsDialogue
{
public:
	/** 
	 * Static function to access constructing this window.
	 *
	 * @param bInHarvest		true if the components of the selected actors should be harvested for the blueprint.
	 * @param ActorOverride		If set convert the specified actor, if null use the currently selected actor
	 */
	static void OpenDialog(AActor* InActorOverride = nullptr);
private:

	/** 
	 * Will create the blueprint
	 *
	 * @param InAssetPath		Path of the asset to create
	 * @param bInHarvest		true if the components of the selected actors should be harvested for the blueprint.
	 */
	static void OnCreateBlueprint(const FString& InAssetPath);
private:
	static TWeakObjectPtr<AActor> ActorOverride;
};
