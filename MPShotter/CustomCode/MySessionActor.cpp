#include "MySessionActor.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

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
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("[Subsystem]Online Subsystem Not Found"))
			);
		}
	}
	OnCreateSessionCompleteDelegate.BindUObject(this, &AMySessionActor::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate.BindUObject(this, &AMySessionActor::OnFindSessionComplete);
	OnJoinSessionCompleteDelegate.BindUObject(this, &AMySessionActor::OnJoinSessionComplete);
}

void AMySessionActor::BeginPlay()
{
	Super::BeginPlay();
	//按键绑定
	InputComponent->BindAction("CreateSession", IE_Pressed, this, &AMySessionActor::CreateMySession);
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
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void AMySessionActor::JoinMySession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void AMySessionActor::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("[Subsystem]Create Sesson: %s"), *SessionName.ToString())
			);
		}
		//切换关卡
		UWorld *World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/Maps/LobbyMap?listen");
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

void AMySessionActor::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	//循环遍历找到的结果
	for(auto res : SessionSearch->SearchResults)
	{
		FString id = res.GetSessionIdStr();
		FString name = res.Session.OwningUserName;
		FString MatchType;
		res.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString::Printf(TEXT("[Subsystem]ID: %s, Name: %s"), *id, *name)
			);
		}
		if (MatchType == "FreeForAll")
		{
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, res);
		}
		UE_LOG(LogTemp, Warning, TEXT("[Subsystem]Find Session: %s"), *res.GetSessionIdStr());
	}
}

void AMySessionActor::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}
