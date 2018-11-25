// Copyright Peter Leontev

#include "AdditionalActorActionsEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"


IMPLEMENT_GAME_MODULE(FAdditionalActorActionsEditorModule, AdditionalActorActionsEditor);

DEFINE_LOG_CATEGORY(AdditionalActorActionsEditor)
 
#define LOCTEXT_NAMESPACE "AdditionalActorActionsEditor"
 
void FAdditionalActorActionsEditorModule::StartupModule()
{
	UE_LOG(AdditionalActorActionsEditor, Log, TEXT("Log Started"));

	FAdditionalActorActionsCommands::Register();
	BindCommands();

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension(
			"EditMain", EExtensionHook::After, AdditionalActorActionsEditorCommands,
			FMenuExtensionDelegate::CreateRaw(this, &FAdditionalActorActionsEditorModule::AddBlueprintMenuExtension)
		);
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
}
 
void FAdditionalActorActionsEditorModule::ShutdownModule()
{
	UE_LOG(AdditionalActorActionsEditor, Warning, TEXT("Log Ended"));
	FAdditionalActorActionsCommands::Unregister();
}

void FAdditionalActorActionsEditorModule::AddBlueprintMenuExtension(class FMenuBuilder& MenuBuilder)
{
	UE_LOG(AdditionalActorActionsEditor, Warning, TEXT("Menu has been build!"));
	MenuBuilder.BeginSection("AdditionalActorActions", NSLOCTEXT("AdditionalActorActions", "AdditionalActorActions", "AdditionalActorActions"));
	{
		MenuBuilder.AddMenuEntry(FAdditionalActorActionsCommands::Get().ConvertSelectedActorsToBlueprintWithChildActorComponents);
	}
	MenuBuilder.EndSection();
}

void FAdditionalActorActionsEditorModule::BindCommands()
{
	check(!AdditionalActorActionsEditorCommands.IsValid());

	AdditionalActorActionsEditorCommands = MakeShareable(new FUICommandList);
	const FAdditionalActorActionsCommands& Commands = FAdditionalActorActionsCommands::Get();

	AdditionalActorActionsEditorCommands->MapAction(
		Commands.ConvertSelectedActorsToBlueprintWithChildActorComponents,
		FExecuteAction::CreateStatic(&FAdditionalActorActionsCommandsCallbacks::ConvertSelectedActorsToBlueprintWithChildActorComponents),
		FCanExecuteAction::CreateStatic(&FAdditionalActorActionsCommandsCallbacks::CanConvertSelectedActorsToBlueprintWithChildActorComponents)
	);
}
 
#undef LOCTEXT_NAMESPACE


