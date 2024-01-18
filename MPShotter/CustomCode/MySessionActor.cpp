#include "MySessionActor.h"
#include "OnlineSessionSettings.h"

// Sets default values
AMySessionActor::AMySessionActor()
{
	PrimaryActorTick.bCanEverTick = true;
	//--------------------------------------Custom-----------------------------------
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Subsystem]Online Subsystem Found: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		//在屏幕中打印输出信息
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("[Subsystem]Online Subsystem Found: %s"), *OnlineSubsystem->GetSubsystemName().ToString())
			);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("[Subsystem]Online Subsystem Not Found"))
			);
	}
	OnCreateSessionCompleteDelegate.BindUObject(this, &AMySessionActor::OnCreateSessionComplete);
}

void AMySessionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMySessionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySessionActor::CreateMySession()
{
	UE_LOG(LogTemp, Warning, TEXT("[Subsystem]Press 1 to Create Session"));
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}
	//这一行代码将OnCreateSessionCompleteDelegate委托添加到CreateSessionComplete事件。这意味着当会话创建完成时，OnCreateSessionComplete函数将被调用。
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = true;
	SessionSettings->NumPublicConnections = 2;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bUsesPresence = true;
SessionSettings->bShouldAdvertise = true;
	const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}
void AMySessionActor::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("[Subsystem]Create Sesson: %s"), *SessionName.ToString())
			);
		}
	}else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString::Printf(TEXT("[Subsystem]Failed to Create Sesson!"))
			);
		}
	}
}