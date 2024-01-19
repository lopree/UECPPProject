#include "ShooterGameModeBase.h"
#include "ShooterHUD.h"

AShooterGameModeBase::AShooterGameModeBase()
{
	//默认的HUD
	HUDClass = AShooterHUD::StaticClass();
	//默认的Pawn
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
