// Copyright Peter Leontev

#pragma once
 
#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "AdditionalActorActionsCommands.h"


DECLARE_LOG_CATEGORY_EXTERN(AdditionalActorActionsEditor, All, All)
 
class FAdditionalActorActionsEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void BindCommands();

	void AddBlueprintMenuExtension(class FMenuBuilder& MenuBuilder);

private:
	TSharedPtr<FExtender> MenuExtender;
	TSharedPtr<FUICommandList> AdditionalActorActionsEditorCommands;
};
