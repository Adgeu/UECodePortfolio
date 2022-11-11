// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "TDTeamIdHelpers.h"
#include "GenericTeamAgentInterface.h"
#include "TDTeamId.generated.h"

////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
//						Add TeamId Instruction
// -------------------------------------------------------------------
// For adding new TeamId we need to do 3 steps:
//
// 1. Add TeamId name in the ETeamId enum.
// 
// 2. Add TeamIdInfo class using helper macros in TeamIdInfo custom definitions.
//		Example (we want to add Player id):
//
//        TeamIdInfoClass(Player)                                     - definition of the TeamIdInfo class body
//              FriendlyIdList(ETeamId::Player, ETeamId::Civilian)    - definition of the friendly id list
//              HostileIdList(ETeamId::Catcher, ETeamId::Necrosis)    - definition of the hostile id list
//        TeamIdInfoClassEnd()                                        - end of the TeamIdInfo
//
// 3. Add TeamIdInfo line in Make method in FTeamIdUtils class using FactoryTeamIdInfoLine() macro.
//

////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
// TeamId enum
// -------------------------------------------------------------------

UENUM(BlueprintType)
enum class ETeamId : uint8
{
	None = 0,
	Player,
	Civilian,
	Catcher,
	Necrosis,
	Mutant
};

////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
// TeamIdInfo base class (do not touch)
// -------------------------------------------------------------------

/**
 * Contains TeamId and attitude with other IDs.
 */
class FTeamIdInfo_Base
{
public:
	FTeamIdInfo_Base() = default;

	FTeamIdInfo_Base(ETeamId TeamId_In) : TeamId(TeamId_In)
	{
	}

	virtual ~FTeamIdInfo_Base() = default;

	/** Returns TeamId. */
	FORCEINLINE ETeamId GetTeamId() const
	{
		return TeamId;
	}

	/** Returns list of friendly IDs. */
	virtual TArray<ETeamId> GetFriendlyIdList() const = 0;

	/** Returns list of hostile IDs. */
	virtual TArray<ETeamId> GetHostileIdList() const = 0;

	/** Returns team attitude to other TeamId. */
	ETeamAttitude::Type GetTeamAttitudeTo(ETeamId OtherTeamId) const
	{
		if (GetFriendlyIdList().Contains(OtherTeamId))
		{
			return ETeamAttitude::Friendly;
		}
		if (GetHostileIdList().Contains(OtherTeamId))
		{
			return ETeamAttitude::Hostile;
		}
		return ETeamAttitude::Neutral;
	}

	bool operator ==(const FTeamIdInfo_Base& Other) const
	{
		return TeamId == Other.TeamId;
	}

	bool operator !=(const FTeamIdInfo_Base& Other) const
	{
		return TeamId != Other.TeamId;
	}

private:
	ETeamId TeamId;
};

////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
// TeamIdInfo custom definitions 
// -------------------------------------------------------------------

// Use macros:
//TeamIdInfoClass()
// 	FriendlyIdList()
//	HostileIdList()
//TeamIdInfoClassEnd()

TeamIdInfoClass(None)
	FriendlyIdList()
	HostileIdList()
TeamIdInfoClassEnd()

TeamIdInfoClass(Player)
	FriendlyIdList(ETeamId::Player, ETeamId::Civilian)
	HostileIdList(ETeamId::Catcher, ETeamId::Necrosis, ETeamId::Mutant)
TeamIdInfoClassEnd()

TeamIdInfoClass(Civilian)
	FriendlyIdList(ETeamId::Player, ETeamId::Civilian, ETeamId::Catcher)
	HostileIdList(ETeamId::Necrosis, ETeamId::Mutant)
TeamIdInfoClassEnd()

TeamIdInfoClass(Catcher)
	FriendlyIdList(ETeamId::Civilian, ETeamId::Catcher)
	HostileIdList(ETeamId::Player, ETeamId::Necrosis, ETeamId::Mutant)
TeamIdInfoClassEnd()

TeamIdInfoClass(Necrosis)
	FriendlyIdList(ETeamId::Necrosis)
	HostileIdList(ETeamId::Player, ETeamId::Civilian, ETeamId::Catcher, ETeamId::Mutant)
TeamIdInfoClassEnd()

TeamIdInfoClass(Mutant)
	FriendlyIdList(ETeamId::Mutant)
	HostileIdList(ETeamId::Player, ETeamId::Civilian, ETeamId::Catcher, ETeamId::Necrosis)
TeamIdInfoClassEnd()

////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
// TeamId utils
// -------------------------------------------------------------------

/** Contains utils methods for working with TeamId. */
class FTeamIdUtils
{
public:
	/** Returns Source team id attitude towards Target team id. */
	static ETeamAttitude::Type GetTeamAttitude(ETeamId Source, ETeamId Target)
	{
		ETeamAttitude::Type Result = ETeamAttitude::Neutral;
		const FTeamIdInfo_Base* SourceTeamIdInfo = Make(Source);
		if (SourceTeamIdInfo)
		{
			Result = SourceTeamIdInfo->GetTeamAttitudeTo(Target);
			delete SourceTeamIdInfo;
		}
		return Result;
	}

private:
	/** Make TeamIdInfo class based on TeamId. */
	static FTeamIdInfo_Base* Make(ETeamId TeamId)
	{
		switch (TeamId)
		{
		// Add team id info lines here
		// Use macro: FactoryTeamIdInfoLine()
			
		FactoryTeamIdInfoLine(None)
		FactoryTeamIdInfoLine(Player)
		FactoryTeamIdInfoLine(Civilian)
		FactoryTeamIdInfoLine(Catcher)
		FactoryTeamIdInfoLine(Necrosis)
		FactoryTeamIdInfoLine(Mutant)

		// ...
		// End of the team id info lines
		}

		return nullptr;
	}
};
