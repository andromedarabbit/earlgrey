#include "stdafx.h"
#include "UserInputHandlers.h"

DefaultUserInputHandler::DefaultUserInputHandler()
{

}

DefaultUserInputHandler::~DefaultUserInputHandler()
{

}

void DefaultUserInputHandler::OnUserInput(ServerService& service, const Earlgrey::_txstring& input) 
{
	if(input == _T("stop"))
		service.OnStop();
}