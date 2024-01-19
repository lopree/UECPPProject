#include "Menu.h"
#include "MultiplayerSessionSubsystem.h"
#include "Components/Button.h"

void UMenu::MenuSetUp(int32 NumPublicConnections, FString TypeOfMatch)
{
	CustomNumPublicConnections = NumPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	//bIsFocusable = true;
	SetFocus();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionsComplete.AddDynamic(this,&UMenu::OnCreateSession);
	}
	
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this,&UMenu::HostBtnClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this,&UMenu::JoinBtnClicked);
	}
	return true;
}

void UMenu::NativeDestruct()
{
	Super::NativeDestruct();
	MenuTearDown();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow,TEXT("Create Session Success"));
		}
		//进入下一个Map
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel("/Game/Maps/LobbyMap?listen");
		}
	}else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,TEXT("Create Session Failed"));
		}
	}
}

void UMenu::HostBtnClicked()
{
	if (MultiplayerSessionSubsystem)
	{
		//创建会话
		MultiplayerSessionSubsystem->CreatSession(CustomNumPublicConnections,MatchType);
	}
}

void UMenu::JoinBtnClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,TEXT("JoinBtnClicked"));
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	if(UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	
}
