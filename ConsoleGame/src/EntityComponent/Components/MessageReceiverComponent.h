#pragma once

#include <Core/Messaging/MessageBroadcaster.h>

class MessageReceiverComponent
{
public:
	// TODO: Fix copy constructor. Copied mBroadcaster will have handler functions that cannot be unregistered externally.

	MessageReceiverComponent() { }
	MessageReceiverComponent(MessageReceiverComponent&& inRHS)		: mBroadcaster( std::move(inRHS.mBroadcaster) ) { }
	MessageReceiverComponent(const MessageReceiverComponent& inRHS) : mBroadcaster( inRHS.mBroadcaster ) { }
	~MessageReceiverComponent() { }

	template<typename TMessage>
	MessageRegistrationHandle	Register(const std::function<void (const TMessage&)>& inFunctor);

	template<typename TMessage>
	void						Broadcast(const TMessage& inMessage) const;

private:
	MessageBroadcaster			mBroadcaster;
};


template<typename TMessage>
MessageRegistrationHandle MessageReceiverComponent::Register(const std::function<void (const TMessage&)>& inFunctor)
{
	return mBroadcaster.Register(inFunctor);
}

template<typename TMessage>
void MessageReceiverComponent::Broadcast(const TMessage& inMessage) const
{
	mBroadcaster.Broadcast(inMessage);
}
