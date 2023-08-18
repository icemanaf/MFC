#pragma once
#include <string>
#ifndef _SENTIMENT_ANALYZER_PROMPTS_
#define _SENTIMENT_ANALYZER_PROMPTS_


static class Prompts
{
public:
	static std::string GetInitialPrompt()
	{
		auto prompt = R"(### Instruction: Given an input, categorise it's sentiment into the following values:Positive,Negative,Neutral
Only provide  responses from the above categories.

### Input: The service took a long time and when the food arrived it was cold.
### Response: Negative.

### Input: There were delays at the airport and the staff were rude.
### Response: Negative.

### Input: The food was excellent and the service was second to none.
### Response: Positive.
)";


		return prompt;
	}
};

#endif