# ue4_AdditionalActorActions
This Unreal Engine 4 editor plugin allows one to save selected actors and their components as blueprint actor.

How it works:
1) Blueprint actors *with custom logic* will be added to new blueprint actor as child actor components.
2) Blueprint actors *without custom logic* will be added as usual (their components will be harvested and attached to new blueprint actor).
3) Selected actors on the scene will be replaced with new blueprint actor.

**Supported Engine Versions**: Tested on 4.17.1
