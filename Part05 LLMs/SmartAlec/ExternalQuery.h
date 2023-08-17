#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;


std::string DoWikipediaQuery(std::string keywords)
{
	std::string sRet = "";

	std::string queryString = std::format("/w/api.php?format=json&action=query&generator=search&gsrnamespace=0&explaintext&gsrsearch={0}&gsrlimit=1&prop=extracts&exintro&explaintext&exsentences=10&exlimit=max", keywords);
	
	httplib::Client cli("https://en.wikipedia.org");
	
	auto res = cli.Get(queryString);
	auto s = res->status;
	auto b = res->body;

	json j = json::parse(b);

	auto x = j["query"]["pages"];

	for (const auto& item : x)
	{

		if (item["extract"] != item.empty())
		{
			sRet= item["extract"];

			break;
		}

		
	}


	return sRet;
}