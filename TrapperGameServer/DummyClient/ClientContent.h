#pragma once

USING_SHARED_PTR(ClientContent);
extern ClientContentRef GClientContent;

class ClientContent
{
public:
	void Start();
	void ChangeRunning();
	void SetMyId(string id);
	void AfterLogin();

private:
	bool _clientRun = true;
	ClientServiceRef service = nullptr;
	string myId = "";
};

