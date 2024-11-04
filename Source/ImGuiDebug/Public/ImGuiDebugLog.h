// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogVerbosity.h"
#include "ImGuiDebugLog.generated.h"

//--------------------------------------------------------------------------------------------------------------------------
DECLARE_LOG_CATEGORY_EXTERN(LogImGuiNone, Warning, All);

//--------------------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EImGuiLogVerbosity : uint8
{
	NoLogging = ELogVerbosity::NoLogging,
	Fatal = ELogVerbosity::Fatal,
	Error = ELogVerbosity::Error,
	Warning = ELogVerbosity::Warning,
	Display = ELogVerbosity::Display,
	Log = ELogVerbosity::Log,
	Verbose = ELogVerbosity::Verbose,
	VeryVerbose = ELogVerbosity::VeryVerbose
};

//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct IMGUIDEBUG_API FImGuiLogCategory
{
	GENERATED_USTRUCT_BODY()

	FImGuiLogCategory() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	FString GetName() const { return Name.ToString(); }

	mutable FLogCategoryBase* LogCategory = nullptr;
};

//--------------------------------------------------------------------------------------------------------------------------
struct IMGUIDEBUG_API FImGuiDebugLogCategoryInfo
{
    FLogCategoryBase* LogCategory = nullptr;

    ELogVerbosity::Type ServerVerbosity = ELogVerbosity::NoLogging;

    FString DisplayName;

    FString Description;

    bool bVisible = true;

    FString GetDisplayName() const;
};

//--------------------------------------------------------------------------------------------------------------------------
struct IMGUIDEBUG_API FImGuiDebugLog
{
    static void AddLogCategory(FLogCategoryBase& LogCategory, const FString& DisplayName = "", const FString& Description = "", bool bVisible = true);

    static bool IsVerbosityActive(ELogVerbosity::Type Verbosity);

    static bool IsLogCategoryActive(const FLogCategoryBase& LogCategory);

    static bool IsLogCategoryActive(FName CategoryName);

    static void SetLogCategoryActive(FLogCategoryBase& LogCategory, bool Value);

    static FLogCategoryBase* FindLogCategory(FName CategoryName);

    static FLogCategoryBase* GetLogCategoryBase(const FImGuiLogCategory& LogCategory);

    static FImGuiDebugLogCategoryInfo* FindLogCategoryInfo(FName CategoryName);

    static TMap<FName, FImGuiDebugLogCategoryInfo>& GetLogCategories() { return LogCategories; }

    static void SetServerVerbosityActive(UWorld& World, FName CategoryName, bool Value);

    static bool IsServerVerbosityActive(FName CategoryName);

    static ELogVerbosity::Type GetServerVerbosity(FName CategoryName);

    static void SetServerVerbosity(UWorld& World, FName CategoryName, ELogVerbosity::Type Verbosity);

    static void OnServerVerbosityChanged(FName CategoryName, ELogVerbosity::Type Verbosity);

    static void DeactivateAllLogCategories(UWorld& World);

private:

    static TMap<FName, FImGuiDebugLogCategoryInfo> LogCategories;

};
