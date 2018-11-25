// Copyright Peter Leontev

#pragma once

#include "AdditionalActorActionsEditor.h"
#include "CreateBlueprintWithChildActorComponents.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "CreateBlueprintWithChildActorComponents"

UBlueprint* FCreateBlueprintWithChildActorComponents::Execute(const FString& Path, TArray<AActor*> SelectedActors, bool bReplaceInWorld)
{
	if (SelectedActors.Num() == 0)
	{
		return nullptr;
	}
	
	// Create a blueprint
	FString PackageName = Path;
	FString AssetName = FPackageName::GetLongPackageAssetName(Path);
	FString BasePath = PackageName + TEXT("/") + AssetName;

	// If no AssetName was found, generate a unique asset name.
	if(AssetName.Len() == 0)
	{
		BasePath = PackageName + TEXT("/") + LOCTEXT("BlueprintName_Default", "NewBlueprint").ToString();
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		AssetToolsModule.Get().CreateUniqueAssetName(BasePath, TEXT(""), PackageName, AssetName);
	}

	UPackage* Package = CreatePackage(nullptr, *PackageName);
	Blueprint = FKismetEditorUtilities::CreateBlueprint(
		AActor::StaticClass(), Package, *AssetName, BPTYPE_Normal,
		UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(),
		FName("HarvestFromActors")
	);

	check(Blueprint->SimpleConstructionScript != NULL);
	SCS = Blueprint->SimpleConstructionScript;

	// Create a common root if necessary
	USCS_Node* RootNodeOverride = SCS->CreateNode(USceneComponent::StaticClass(), TEXT("SharedRoot"));
	SCS->AddNode(RootNodeOverride);

	// Harvest the components from each actor and clone them into the SCS
	// Replace actors with custom logic with child actor components
	for (const AActor* Actor : SelectedActors)
	{
		bool isBlueprintClass = Cast<UBlueprint>(Actor->GetClass()->ClassGeneratedBy) != nullptr;
		if (isBlueprintClass)
		{
			UChildActorComponent* childActorComponent = NewObject<UChildActorComponent>();
			childActorComponent->SetChildActorClass(Actor->GetActorClass());
			childActorComponent->SetWorldTransform(Actor->GetActorTransform());

			USCS_Node* NewSCSNode = SCS->CreateNode(UChildActorComponent::StaticClass(), Actor->GetFName());
			UEditorEngine::FCopyPropertiesForUnrelatedObjectsParams Params;
			Params.bDoDelta = false; // We need a deep copy of parameters here so the CDO values get copied as well
			UEditorEngine::CopyPropertiesForUnrelatedObjects(childActorComponent, NewSCSNode->ComponentTemplate, Params);

			NewSCSNode->ComponentTemplate->CreationMethod = EComponentCreationMethod::Native;
			Cast<USceneComponent>(NewSCSNode->ComponentTemplate)->SetMobility(EComponentMobility::Movable);

			SCS->GetRootNodes()[0]->AddChildNode(NewSCSNode);
		}
		else
		{
			TArray<UActorComponent*> Components;
			Actor->GetComponents(Components);

			// Exclude any components created by other components
			for (int32 Index = Components.Num() - 1; Index >= 0; --Index)
			{
				UActorComponent* ComponentToConsider = Components[Index];
				if (ComponentToConsider->IsEditorOnly())
				{
					Components.RemoveAt(Index, 1, false);
				}
			}

			FKismetEditorUtilities::AddComponentsToBlueprint(Blueprint, Components, true, RootNodeOverride);
		}
	}

	// Compute the average origin for all the actors, so it can be backed out when saving them in the blueprint
	float minZ = SelectedActors[0]->GetActorLocation().Z;

	FTransform NewActorTransform = FTransform::Identity;
	{
		// Find average location of all selected actors
		FVector AverageLocation = FVector::ZeroVector;
		for (const AActor* Actor : SelectedActors)
		{
			if (USceneComponent* RootComponent = Actor->GetRootComponent())
			{
				AverageLocation += Actor->GetActorLocation();
				if (Actor->GetActorLocation().Z < minZ)
				{
					minZ = Actor->GetActorLocation().Z;
				}
			}
		}
		AverageLocation /= (float)SelectedActors.Num();
		AverageLocation.Z = minZ;

		// Spawn the new BP at that location
		NewActorTransform.SetTranslation(AverageLocation);
	}

	// Reposition all of the children of the root node to recenter them around the new pivot
	for (USCS_Node* TopLevelNode : SCS->GetRootNodes())
	{
		if (USceneComponent* TestRoot = Cast<USceneComponent>(TopLevelNode->ComponentTemplate))
		{
			for (USCS_Node* ChildNode : TopLevelNode->GetChildNodes())
			{
				if (USceneComponent* ChildComponent = Cast<USceneComponent>(ChildNode->ComponentTemplate))
				{
					const FTransform OldChildToWorld(ChildComponent->RelativeRotation, ChildComponent->RelativeLocation);
					const FTransform NewRelativeTransform = OldChildToWorld.GetRelativeTransform(NewActorTransform);
					ChildComponent->RelativeLocation = NewRelativeTransform.GetLocation();
					ChildComponent->RelativeRotation = NewRelativeTransform.GetRotation().Rotator();
				}
			}
		}
	}

	// Regenerate skeleton class as components have been added since initial generation
	FKismetEditorUtilities::GenerateBlueprintSkeleton(Blueprint, true); 
		
	// Notify the asset registry
	FAssetRegistryModule::AssetCreated(Blueprint);

	// Mark the package dirty
	Package->MarkPackageDirty();

	// Delete the old actors and create a new instance in the map
	if (bReplaceInWorld)
	{
		FVector Location = NewActorTransform.GetLocation();
		FRotator Rotator = NewActorTransform.Rotator();

		FKismetEditorUtilities::CreateBlueprintInstanceFromSelection(Blueprint, SelectedActors, Location, Rotator);
	}

	// Open the editor for the new blueprint
	FAssetEditorManager::Get().OpenEditorForAsset(Blueprint);

	return Blueprint;
}

#undef LOCTEXT_NAMESPACE
