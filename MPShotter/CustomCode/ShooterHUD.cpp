#include "ShooterHUD.h"

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();
	//显示鼠标
	GetOwningPlayerController()->bShowMouseCursor = true;
}
