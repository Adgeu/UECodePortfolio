// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

// Helper TeamId macros.

// TeamIdInfo class body start.
#define TeamIdInfoClass(TeamId) \
class FTeamIdInfo_##TeamId## : public FTeamIdInfo_Base \
{ \
public: \
FTeamIdInfo_##TeamId##() : FTeamIdInfo_Base(ETeamId::##TeamId##) {}

// Friendly id list
#define FriendlyIdList(...) \
FORCEINLINE virtual TArray<ETeamId> GetFriendlyIdList() const override \
{ \
return {##__VA_ARGS__}; \
}

// Hostile id list
#define HostileIdList(...) \
FORCEINLINE virtual TArray<ETeamId> GetHostileIdList() const override \
{ \
return {##__VA_ARGS__}; \
}

// TeamIdInfo class body end.
#define TeamIdInfoClassEnd() };

// TeamIdInfo switch case.
#define FactoryTeamIdInfoLine(TeamId) \
case ETeamId::##TeamId##: \
return new FTeamIdInfo_##TeamId##(); 
