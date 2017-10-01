#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"
#include "CreateBlueprintWithChildActorComponentsDialogue.h"

#define LOCTEXT_NAMESPACE "LevelEditorActions"

class FAdditionalActorActionsCommands : public TCommands<FAdditionalActorActionsCommands>
{

public:
	FAdditionalActorActionsCommands() :
		TCommands<FAdditionalActorActionsCommands>(
			"AdditionalActorActionsEditor",
			NSLOCTEXT("Contexts", "AdditionalActorActionsEditor", "Additional Actor Actions Editor"),
			NAME_None,
			FEditorStyle::GetStyleSetName()
		) {}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(
			ConvertSelectedActorsToBlueprintWithChildActorComponents,
			"Convert selected actors to blueprint actor with child actor components",
			"Replace all of the selected actors with a new Blueprint Class based on Actor that besides usual components contains child actor components for actors with custom logic",
			EUserInterfaceActionType::Button,
			FInputChord()
		);
	}

public:
	TSharedPtr<FUICommandInfo> ConvertSelectedActorsToBlueprintWithChildActorComponents;
};


class FAdditionalActorActionsCommandsCallbacks
{
public:
	static void ConvertSelectedActorsToBlueprintWithChildActorComponents()
	{
		FCreateBlueprintWithChildActorComponentsDialogue::OpenDialog();
	}

	static bool CanConvertSelectedActorsToBlueprintWithChildActorComponents()
	{
		return GEditor->GetSelectedActorCount() > 0;
	}
};

#undef LOCTEXT_NAMESPACE