#pragma once
#include "ServerService.h"

class DefaultUserInputHandler : public ServerService::UserInputHandler
{
public:
	explicit DefaultUserInputHandler();
	virtual ~DefaultUserInputHandler();

	virtual void OnUserInput(ServerService& service, const Earlgrey::_txstring& input);
};