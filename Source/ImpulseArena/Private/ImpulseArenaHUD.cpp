#include "ImpulseArenaHUD.h"
#include "ImpulseArenaGameState.h"
#include "ImpulseArenaPlayerState.h"
#include "ImpulseArenaAttributeSet.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

void AImpulseArenaHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas || !GEngine || !GetWorld()) { return; }

    const AImpulseArenaGameState* GameState = GetWorld()->GetGameState<AImpulseArenaGameState>();
    if (!GameState) { return; }

    // Scale against this player's canvas, including smaller PIE and split-screen views.
    const float Scale = FMath::Min(Canvas->ClipX / 960.0f, Canvas->ClipY / 540.0f);
    if (Scale <= 0.0f) { return; }
    const float CenterX = Canvas->ClipX * 0.5f;
    const float Top = 24.0f * Scale;
    const FLinearColor Blue(0.2f, 0.6f, 1.0f);
    const FLinearColor Red(1.0f, 0.3f, 0.3f);
    const FLinearColor Panel(0.015f, 0.02f, 0.035f, 0.85f);
    UFont* Font = GEngine->GetMediumFont();

    auto DrawCentered = [this, Font, Scale](const FString& Text, const FLinearColor& Color,
        float X, float Y, float TextScale)
    {
        float Width = 0.0f;
        float Height = 0.0f;
        const float FontScale = Scale * TextScale;
        GetTextSize(Text, Width, Height, Font, FontScale);
        DrawText(Text, Color, X - Width * 0.5f, Y, Font, FontScale);
    };

    DrawRect(Panel, CenterX - 210.0f * Scale, Top, 420.0f * Scale, 76.0f * Scale);
    DrawRect(Blue, CenterX - 210.0f * Scale, Top, 4.0f * Scale, 76.0f * Scale);
    DrawRect(Red, CenterX + 206.0f * Scale, Top, 4.0f * Scale, 76.0f * Scale);
    DrawCentered(TEXT("BLUE"), Blue, CenterX - 135.0f * Scale, Top + 24.0f * Scale, 1.0f);
    DrawCentered(TEXT("RED"), Red, CenterX + 135.0f * Scale, Top + 24.0f * Scale, 1.0f);
    DrawCentered(FString::Printf(TEXT("%d  :  %d"), GameState->GetBlueScore(), GameState->GetRedScore()),
        FLinearColor::White, CenterX, Top + 18.0f * Scale, 1.5f);

    // Read this HUD owner's replicated GAS attributes, never another player's energy.
    const APlayerController* Controller = GetOwningPlayerController();
    const AImpulseArenaPlayerState* PlayerState = Controller ? Controller->GetPlayerState<AImpulseArenaPlayerState>() : nullptr;
    const UImpulseArenaAttributeSet* Attributes = PlayerState ? PlayerState->GetAttributeSet() : nullptr;
    if (Attributes)
    {
        const float MaxEnergy = FMath::Max(0.0f, Attributes->GetMaxEnergy());
        const float Energy = FMath::Clamp(Attributes->GetEnergy(), 0.0f, MaxEnergy);
        const float Fraction = MaxEnergy > 0.0f ? Energy / MaxEnergy : 0.0f;
        const float BarTop = Canvas->ClipY - 80.0f * Scale;
        const float BarLeft = CenterX - 140.0f * Scale;
        const FLinearColor EnergyColor = Fraction <= 0.25f
            ? FLinearColor(1.0f, 0.55f, 0.15f) : FLinearColor(0.15f, 0.85f, 0.75f);
        DrawRect(Panel, BarLeft - 12.0f * Scale, BarTop - 8.0f * Scale, 304.0f * Scale, 64.0f * Scale);
        DrawCentered(FString::Printf(TEXT("ENERGY  %.0f / %.0f"), Energy, MaxEnergy),
            FLinearColor::White, CenterX, BarTop, 0.85f);
        DrawRect(FLinearColor(0.12f, 0.15f, 0.2f), BarLeft, BarTop + 30.0f * Scale, 280.0f * Scale, 12.0f * Scale);
        DrawRect(EnergyColor, BarLeft, BarTop + 30.0f * Scale, 280.0f * Scale * Fraction, 12.0f * Scale);
    }

    // The existing multicast supplies a transient goal window on each connected machine.
    // Reading it avoids announcing old scores to late joiners or replaying an expired goal.
    const double Remaining = GameState->GetSpectatorReactionEndTime() - GetWorld()->GetTimeSeconds();
    if (GameState->GetSpectatorGoalSerial() != 0 && Remaining > 0.0)
    {
        const bool bBlueScored = GameState->GetSpectatorScoringTeam() == EImpulseArenaTeam::Blue;
        const float Alpha = FMath::Clamp(static_cast<float>(Remaining) / 0.4f, 0.0f, 1.0f);
        FLinearColor BannerColor = bBlueScored ? Blue : Red;
        BannerColor.A = Alpha;
        FLinearColor BannerPanel = Panel;
        BannerPanel.A *= Alpha;
        DrawRect(BannerPanel, CenterX - 210.0f * Scale, Top + 90.0f * Scale, 420.0f * Scale, 60.0f * Scale);
        DrawCentered(bBlueScored ? TEXT("BLUE SCORES!") : TEXT("RED SCORES!"),
            BannerColor, CenterX, Top + 102.0f * Scale, 1.5f);
    }
}
