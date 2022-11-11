#pragma once

namespace XHAnimUtils
{
	template <typename T>
	const FAnimNotifyEvent* GetAnimNotifyByClass(UAnimSequenceBase* InAnimation)
	{
		if (IsValid(InAnimation))
		{
			for (const FAnimNotifyEvent& AnimNotifyEvent : InAnimation->Notifies)
			{
				if (AnimNotifyEvent.Notify->IsA<T>())
				{
					return &AnimNotifyEvent;
				}
			}
		}

		return nullptr;
	}
}
