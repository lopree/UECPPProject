#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class MPSHOTTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};
