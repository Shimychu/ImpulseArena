#include "Mass/ImpulseArenaSpectatorTrait.h"
#include "Mass/ImpulseArenaMassFragments.h"
#include "Mass/EntityFragments.h"
#include "MassEntityTemplateRegistry.h"

void UImpulseArenaSpectatorTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
    BuildContext.AddFragment<FTransformFragment>();
    BuildContext.AddFragment<FImpulseArenaSpectatorFragment>();
}