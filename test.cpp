#include "Base64.h"
#include <Windows.h>

int
main(
	void
)
{
	::SetConsoleOutputCP(65001);

	using namespace Apostr;

	std::vector<std::string> testString{
		"雨下整夜 我的爱溢出就像雨水",
		"把永远爱你写进诗的结尾",
		"Thank you",
		"ありがとうございます",
		"Спасибо",
		"a",
		"bc",
		"def",
		"我",
		"你好",
		"你好啊",
		"Здравствуйте",
		"Bonjour",
		"こんにちは",
		"いいえ",
		"故事的小黄花",
		"从出生那年就飘着",
		"童年的荡秋千",
		"随记忆一直晃到现在",
		"刮风这天 我试过握着你手"
	};

	for (auto str : testString)
	{
		std::cout << "原句: " << str << std::endl;

		auto encodeRet = Base64::base64EncodeString(str);
		if (encodeRet.first)
		{
			std::cout << "编码过后: " << encodeRet.second << std::endl;
		}

		auto decodeRet = Base64::base64DecodeString(encodeRet.second);
		if (encodeRet.first)
		{
			std::cout << "解码过后" << decodeRet.second << std::endl;
		}

		if (str == decodeRet.second)
		{
			std::cout << "Passed!" << std::endl;
		}

		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}