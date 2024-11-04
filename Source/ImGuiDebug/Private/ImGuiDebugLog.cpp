// Copyright 2023 CoC All rights reserved


#include "ImGuiDebugLog.h"

//--------------------------------------------------------------------------------------------------------------------------
DEFINE_LOG_CATEGORY(LogImGuiNone);

TMap<FName, FImGuiDebugLogCategoryInfo> FImGuiDebugLog::LogCategories;


//--------------------------------------------------------------------------------------------------------------------------
// FImGuiDebugLogCategoryInfo
//--------------------------------------------------------------------------------------------------------------------------
FString FImGuiDebugLogCategoryInfo::GetDisplayName() const
{
    if (DisplayName.IsEmpty() == false)
    {
        return DisplayName;
    }

    if (LogCategory != nullptr)
    {
        return LogCategory->GetCategoryName().ToString();
    }

    return FString("Invalid");
}

//--------------------------------------------------------------------------------------------------------------------------
// FImGuiDebugLogCategoryManager
//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugLog::AddLogCategory(FLogCategoryBase& LogCategory, const FString& DisplayName, const FString& Description, const bool bVisible)
{
    LogCategories.Add(LogCategory.GetCategoryName(),
        FImGuiDebugLogCategoryInfo
        {
            &LogCategory,
            ELogVerbosity::NumVerbosity,
            DisplayName,
            Description,
            bVisible,
            });
}

//--------------------------------------------------------------------------------------------------------------------------
bool FImGuiDebugLog::IsVerbosityActive(const ELogVerbosity::Type Verbosity)
{
    return Verbosity >= ELogVerbosity::Verbose;
}

//--------------------------------------------------------------------------------------------------------------------------
bool FImGuiDebugLog::IsLogCategoryActive(const FLogCategoryBase& LogCategory)
{
    return IsVerbosityActive(LogCategory.GetVerbosity());
}

//--------------------------------------------------------------------------------------------------------------------------
bool FImGuiDebugLog::IsLogCategoryActive(const FName CategoryName)
{
    if (const FLogCategoryBase* LogCategory = FindLogCategory(CategoryName))
    {
        return IsVerbosityActive(LogCategory->GetVerbosity());
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugLog::SetLogCategoryActive(FLogCategoryBase& LogCategory, const bool Value)
{
    LogCategory.SetVerbosity(Value ? ELogVerbosity::Verbose : ELogVerbosity::Warning);
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugLogCategoryInfo* FImGuiDebugLog::FindLogCategoryInfo(const FName CategoryName)
{
    return LogCategories.Find(CategoryName);
}

//--------------------------------------------------------------------------------------------------------------------------
FLogCategoryBase* FImGuiDebugLog::FindLogCategory(const FName CategoryName)
{
    if (const FImGuiDebugLogCategoryInfo* LogCategoryInfo = FindLogCategoryInfo(CategoryName))
    {
        return LogCategoryInfo->LogCategory;
    }
    else
    {
        return nullptr;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugLog::DeactivateAllLogCategories(UWorld& World)
{
	const FString ToggleStr = TEXT("Log LogImGuiNone Only");
    GEngine->Exec(&World, *ToggleStr);

    if (APlayerController* PlayerController = World.GetFirstPlayerController())
    {
        PlayerController->ServerExec(ToggleStr);
    }
}


//--------------------------------------------------------------------------------------------------------------------------
FLogCategoryBase* FImGuiDebugLog::GetLogCategoryBase(const FImGuiLogCategory& LogCategory)
{
#if NO_LOGGING

    return nullptr;

#else

    if (LogCategory.Name.IsNone() || LogCategory.Name.IsValid() == false)
    {
        return nullptr;
    }

    if (LogCategory.LogCategory == nullptr)
    {
        if (const FImGuiDebugLogCategoryInfo* CategoryInfo = FImGuiDebugLog::GetLogCategories().Find(LogCategory.Name))
        {
            LogCategory.LogCategory = CategoryInfo->LogCategory;
        }
    }

    return LogCategory.LogCategory;

#endif //NO_LOGGING
}
