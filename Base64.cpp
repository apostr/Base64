#include "Base64.h"

namespace Apostr
{
	const Base64::byte Base64::base64EncodeMap[Base64::BASE64_ENCODE_MAP_SIZE]{
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
	};

	const Base64::byte Base64::base64DecodeMap[Base64::BASE64_DECODE_MAP_SIZE]{
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255, 255, 255, 63,
		52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255, 255, 255, 255, 255,
		255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
		15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  255, 255, 255, 255, 255,
		255, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
		41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	};

	std::pair<bool, std::vector<Base64::byte>>
		Base64::base64EncodeStream(
			const std::vector<byte>& _data
		)
	{
		std::pair<bool, std::vector<byte>> ret{ true, {} };

		if (_data.empty())
		{
			return ret;
		}

		decltype(_data.size()) remainingBytes = _data.size();
		auto dataIterator = _data.cbegin();
		auto dataInserterIterator = std::back_inserter(ret.second);
		for (/* Do nothing. */;
			Base64::NUMBER_OF_CHARACTERS_ENCODED_IN_ONE_ROUND <= remainingBytes;
			dataIterator += Base64::NUMBER_OF_CHARACTERS_ENCODED_IN_ONE_ROUND, remainingBytes -= Base64::NUMBER_OF_CHARACTERS_ENCODED_IN_ONE_ROUND)
		{
			*(dataInserterIterator++) = Base64::getTheFirstCharacterAfterEncoding(*(dataIterator));
			*(dataInserterIterator++) = Base64::getTheSecondCharacterAfterEncoding(*(dataIterator), *(dataIterator + 1));
			*(dataInserterIterator++) = Base64::getTheThirdCharacterAfterEncoding(*(dataIterator + 1), *(dataIterator + 2));
			*(dataInserterIterator++) = Base64::getTheFourthCharacterAfterEncoding(*(dataIterator + 2));
		}

		if (remainingBytes != Base64::NO_REMAINING_CHARACTERS)
		{
			*(dataInserterIterator++) = (Base64::getTheFirstCharacterAfterEncoding(*(dataIterator)));

			if (remainingBytes == Base64::ONE_CHARACTER_REMAINING)
			{
				*(dataInserterIterator++) = Base64::getTheSecondCharacterAfterEncoding(*(dataIterator), 0);
				*(dataInserterIterator++) = Base64::COMPLEMENT_CHARACTER;
			}
			else
			{
				*(dataInserterIterator++) = Base64::getTheSecondCharacterAfterEncoding(*(dataIterator), *(dataIterator + 1));
				*(dataInserterIterator++) = Base64::getTheThirdCharacterAfterEncoding(*(dataIterator + 1), 0);
			}

			*(dataInserterIterator++) = Base64::COMPLEMENT_CHARACTER;
		}

		return ret;
	}

	std::pair<bool, std::vector<Base64::byte>>
		Base64::base64DecodeStream(
			const std::vector<byte>& _data
		)
	{
		std::pair<bool, std::vector<byte>> ret{ true, {} };

		if (_data.empty())
		{
			return ret;
		}

		// 因为经过Base64编码后的数据一定是可以被4整除的, 所以说如果这里检测到不能被4整除, 那么就说明传入的参数是非法的.
		if ((_data.size() % Base64::NUMBER_OF_CHARACTERS_DECODED_IN_ONE_ROUND) != 0)
		{
			ret.first = false;
			return ret;
		}

		// 检测_data中是否含有非法的字符.
		if (!Base64::checkForIllegalCharacters(_data))
		{
			ret.first = false;
			return ret;
		}

		auto remainingBytes = Base64::getDecodeRemainingBytes(_data);
		auto dataIterator = _data.cbegin();
		auto dataInserterIterator = std::back_inserter(ret.second);
		for (/* Do nothing. */;
			Base64::NUMBER_OF_CHARACTERS_DECODED_IN_ONE_ROUND <= remainingBytes;
			dataIterator += Base64::NUMBER_OF_CHARACTERS_DECODED_IN_ONE_ROUND, remainingBytes -= Base64::NUMBER_OF_CHARACTERS_DECODED_IN_ONE_ROUND)
		{
			*(dataInserterIterator++) = Base64::getTheFirstCharacterAfterDecoding(*(dataIterator), *(dataIterator + 1));
			*(dataInserterIterator++) = Base64::getTheSecondCharacterAfterDecoding(*(dataIterator + 1), *(dataIterator + 2));
			*(dataInserterIterator++) = Base64::getTheThirdCharacterAfterDecoding(*(dataIterator + 2), *(dataIterator + 3));
		}

		if (remainingBytes != Base64::NO_REMAINING_CHARACTERS)
		{
			if (remainingBytes == Base64::TWO_CHARACTERS_REMAINING)
			{
				*(dataInserterIterator++) = Base64::getTheFirstCharacterAfterDecoding(*(dataIterator), *(dataIterator + 1));
			}
			else
			{
				*(dataInserterIterator++) = Base64::getTheFirstCharacterAfterDecoding(*(dataIterator), *(dataIterator + 1));
				*(dataInserterIterator++) = Base64::getTheSecondCharacterAfterDecoding(*(dataIterator + 1), *(dataIterator + 2));
			}
		}

		return ret;
	}

	bool
		Base64::NotIn(
			const byte _ch
		)
	{
		bool ret = true;

		if (_ch == Base64::COMPLEMENT_CHARACTER)
		{
			ret = false;
		}
		else
		{
			for (const auto element : Base64::base64EncodeMap)
			{
				if (element == _ch)
				{
					ret = false;
					break;
				}
			}
		}

		return ret;
	}

	bool
		Base64::checkForIllegalCharacters(
			const std::vector<byte>& _data
		)
	{
		// 检测补长字符('=')是否有出现在不合适的地方, 一般来说补长字符只能出现在编码过后的二进制流的最后两个字节, 出现在这之外的其它地方都是不合理的.
		for (auto iterator = _data.cbegin(); iterator < _data.cend(); iterator++)
		{
			if (*(iterator) == Base64::COMPLEMENT_CHARACTER)
			{
				auto pos = (_data.size() - (iterator - _data.cbegin()));
				if ((pos != 1) && (pos != 2))
				{
					return false;
				}
			}
		}

		// 检测二进制流中有没有除了编码表之外的字符.
		if (std::find_if(_data.cbegin(), _data.cend(), Base64::NotIn) != _data.cend())
		{
			return false;
		}

		return true;
	}

	std::vector<Base64::byte>::size_type
		Base64::getDecodeRemainingBytes(
			const std::vector<byte>& _data
		)
	{
		decltype(_data.size()) remainingBytes = _data.size();

		// 由于补长字符都在输出结果的后两位, 所以这里检查给定的字符串的末尾两位是否有补长字符('=').
		if (_data.at(_data.size() - 2) == Base64::COMPLEMENT_CHARACTER)
		{
			remainingBytes -= 2;
		}
		else if (_data.at(_data.size() - 1) == Base64::COMPLEMENT_CHARACTER)
		{
			remainingBytes -= 1;
		}

		return remainingBytes;
	}
}