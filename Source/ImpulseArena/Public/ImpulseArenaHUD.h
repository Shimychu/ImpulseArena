#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ImpulseArenaHUD.generated.h"

UCLASS()
class IMPULSEARENA_API AImpulseArenaHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
