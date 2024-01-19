#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

class UButton;
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	//关卡蓝图中调用
	UFUNCTION(BlueprintCallable)
	void MenuSetUp();
protected:
	virtual bool Initialize() override;
private:
	UPROPERTY(meta=(BindWidget))
	UButton* HostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostBtnClicked();
	UFUNCTION()
	void JoinBtnClicked();
	//管理所有的Session
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
};
