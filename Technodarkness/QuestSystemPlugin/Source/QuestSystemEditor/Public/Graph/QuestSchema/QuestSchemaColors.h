#pragma once

namespace QuestSchemaColors
{
	namespace NodeBody
	{
		constexpr FLinearColor Default(0.1f, 0.1f, 0.1f);
		constexpr FLinearColor Entry(0.1f, 0.04f, 0.11f);
		constexpr FLinearColor Error(1.0f, 0.0f, 0.0f);
	}

	namespace NodeBorder
	{
		constexpr FLinearColor Inactive(0.1f, 0.1f, 0.1f);
		constexpr FLinearColor Entry(0.1f, 0.1f, 0.1f);
		constexpr FLinearColor Disconnected(0.1f, 0.1f, 0.1f, 0.25f);
	}

	namespace Pin
	{
		constexpr FLinearColor Hover(1.0f, 0.7f, 0.0f);
		constexpr FLinearColor Default(0.015f, 0.015f, 0.015f);
		constexpr FLinearColor MultipleWithCheck(0.1f, 0.04f, 0.11f);
		constexpr FLinearColor MultipleWithWeight(0.0f, 0.05f, 0.15f);
		constexpr FLinearColor MultipleObjectivesOnly(0.05f, 0.05f, 0.0f);
		constexpr FLinearColor SingleNode(0.015f, 0.015f, 0.015f);
		constexpr FLinearColor ObjectiveAny(0.015f, 0.015f, 0.015f);
		constexpr FLinearColor ObjectiveSuccess(0.0f, 0.05f, 0.0f);
		constexpr FLinearColor ObjectiveFail(0.05f, 0.0f, 0.0f);
	}

	namespace Connection
	{
		constexpr FLinearColor Default(1.0f, 1.0f, 1.0f);
	}

	namespace Transition
	{
		constexpr FLinearColor Default(1.0f, 1.0f, 1.0f);
		constexpr FLinearColor Hovered(0.724f, 0.256f, 0.0f, 1.0f);
		constexpr FLinearColor Checkable(0.4f, 0.0f, 0.33f);
		constexpr FLinearColor Weightable(0.0f, 0.14f, 0.4f);
	}
}
