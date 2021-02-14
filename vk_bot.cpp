#include "vk_bot.h"

vk_bot::vk_bot(std::string api_key) : api_key(api_key)
{
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_DEFAULT);
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_URL, ("https://api.vk.com/method/groups.getLongPollServer?group_id=192506983&access_token=" + api_key + "&v=5.122").c_str());
	}
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cout << "Error!" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
	root = Parse();
	longpoll_key = root["response"]["key"].asString();
	server = root["response"]["server"].asString();
	ts = stoi(root["response"]["ts"].asString());
}

vk_bot::~vk_bot()
{
	curl_easy_cleanup(curl);
}

void vk_bot::Update()
{
	message.clear();
	curl_easy_setopt(curl, CURLOPT_URL, (server + "?act=a_check&key=" + longpoll_key + "&ts=" + std::to_string(ts) + "&wait=1").c_str());
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cout << "Error!" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
	root = Parse();
	ts = stoi(root["ts"].asString());
	//std::cout << content << std::endl;
	if (!root["updates"].empty()) {
		message = root["updates"][0]["object"]["message"]["text"].asString();
		chat_id = root["updates"][0]["object"]["message"]["peer_id"].asInt();
		from_id = root["updates"][0]["object"]["message"]["from_id"].asInt();
	}
}

void vk_bot::Send(std::string text, int chat_id)
{
	curl_easy_setopt(curl, CURLOPT_URL, ("https://api.vk.com/method/messages.send?peer_id=" + std::to_string(chat_id) + "&message=" + text + "&" + "random_id=0&" + "access_token=" + api_key + "&v=5.122").c_str());
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cout << "Error!" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
	}
}

int vk_bot::GetChatId()
{
	return chat_id;
}

int vk_bot::GetFromId()
{
	return from_id;
}

std::string vk_bot::GetLastMessage()
{
	return message;
}

size_t vk_bot::write_data(char* ptr, size_t size, size_t nmemb, std::string* data)
{
	if (data)
	{
		data->clear();
		data->append(ptr, size * nmemb);
		return size * nmemb;
	}
	else
	{
		return 0;
	}
}

Json::Value vk_bot::Parse()
{
	Json::Value root;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(content.c_str(), content.c_str() + content.size(), &root, &err))
	{
		std::cout << "error" << std::endl;
	}
	return root;
}
