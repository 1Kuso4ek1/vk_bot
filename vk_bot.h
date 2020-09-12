#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>

#include <curl/curl.h>
#include <json/json.h>

class vk_bot
{
public:
	vk_bot(std::string api_key);
	~vk_bot();

	void Update();
	void Send(std::string text, int chat_id);

	int GetChatId();
	int GetFromId();

	std::string GetLastMessage();
private:
	static size_t write_data(char* ptr, size_t size, size_t nmemb, std::string* data);
	Json::Value Parse();

	Json::CharReaderBuilder builder;
	Json::Value root;
	JSONCPP_STRING err;

	CURL* curl;
	CURLcode res;

	std::string content;
	std::string api_key;
	std::string message;
	std::string longpoll_key;
	std::string server;

	int chat_id;
	int ts;
	int from_id;
};

