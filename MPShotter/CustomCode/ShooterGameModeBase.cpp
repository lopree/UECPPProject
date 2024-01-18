#include "ShooterGameModeBase.h"
#include "ShooterHUD.h"

AShooterGameModeBase::AShooterGameModeBase()
{
	HUDClass = AShooterHUD::StaticClass();
}
