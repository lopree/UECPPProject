#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
{
	//绑定
	OnCreateSessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnDestroySessionComplete);
	OnStartSessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnStartSessionComplete);
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionSubsystem::CreatSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	const auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
	//将委托存储在FDelegateHandle中,之后可以从委托列表中将委托移除
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	//设置会话
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->Set(FName("MatchType"),MatchType,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//如果没有创建成功，则冲委托列表中删除委托
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession,*LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		//通知自定义的动态多播，没有成功创建
		MultiplayerOnCreateSessionsComplete.Broadcast(false);
	}
}

void UMultiplayerSessionSubsystem::FindSession(int32 MaxSearchResults)
{
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
}

void UMultiplayerSessionSubsystem::DestroySession()
{
}

void UMultiplayerSessionSubsystem::StartSession()
{
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//链接成功，先清理委托
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	//通知自定义的动态多播，创建成功
	MultiplayerOnCreateSessionsComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
