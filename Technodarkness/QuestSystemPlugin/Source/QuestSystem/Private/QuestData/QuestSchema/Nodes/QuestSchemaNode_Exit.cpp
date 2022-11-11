// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Exit.h"

#include "QuestData/Quest.h"
#include "QuestData/QuestSchema/QuestSchema.h"

UQuestSchemaNode_Exit* UQuestSchemaNode_Exit::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Exit* Node = NewObject<UQuestSchemaNode_Exit>(OwningSchema);
	return Node;
}

void UQuestSchemaNode_Exit::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;
	ExitNode();
}

#if WITH_EDITORONLY_DATA
FString UQuestSchemaNode_Exit::GetNodeDetails(int32 IndentLevel) const
{
	return FString::Printf(TEXT("With %s"), *UEnum::GetDisplayValueAsText(QuestCompletionType).ToString().ToUpper());
}
#endif

void UQuestSchemaNode_Exit::ExitNode()
{
	Super::ExitNode();

	UQuestSchema* QuestSchema = GetOwningSchema();
	QuestSchema->EndSchemaProcess(QuestCompletionType);
}
