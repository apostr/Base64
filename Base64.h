//
// @file: Base64.h
// @brief: Base64 encoding and decoding with Windows C++
// @birth: Created by Benny Hu on 2023-03-23
// @version: V1.1.1
//

#pragma once

#ifndef _BASE64_H_UKXJCJ_7F2V5R_5CA68B_ZD3HHN_UDJC03_RR6IWH_
#define _BASE64_H_UKXJCJ_7F2V5R_5CA68B_ZD3HHN_UDJC03_RR6IWH_

#ifndef _BUILD_BASE64_DLL
#define DLL_API __declspec(dllimport)
#else
#define DLL_API __declspec(dllexport)
#endif // !_BUILD_BASE64_DLL

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace Apostr
{
	class DLL_API Base64
	{
		// 注意: 无法创建该类的对象.
	private:

		// 本类的拷贝构造函数被定义为删除的函数.
		Base64(
			const Base64&
		) = delete;

		// 本类的析构函数被定义为删除的函数.
		~Base64(
			void
		) = delete;

	public:

		using byte = unsigned char;

		// @brief: 将给定的字符串进行Base64编码.
		// @param _data: 要进行Base64编码的字符串.
		// @return: 返回一个pair容器
		//		ret.first - 当函数成功时为true, 失败则为false.
		//		ret.second - 当函数成功时返回对应的编码后的字符串, 失败则是一个空string(有特殊情况, 见warning部分).
		// @warning:
		//		1. 该函数的返回值不应该被忽略.
		//		2. 当要使用该函数的返回值时, 应该先检测ret.first是否为true.
		//		3. 当传入的是一个空string, 那么ret.first为true, ret.second为一个空string.
		[[nodiscard]] static std::pair<bool, std::string>
			base64EncodeString(
				const std::string& _string
			)
		{
			auto ret = Base64::base64EncodeStream(std::vector<byte>(_string.cbegin(), _string.cend()));
			return { ret.first, std::string(ret.second.cbegin(), ret.second.cend()) };
		}

		// @brief: 将给定的字符串进行Base64解码.
		// @param _data: 要进行Base64解码的字符串.
		// @return: 返回一个pair容器
		//		ret.first - 当函数成功时为true, 失败则为false.
		//		ret.second - 当函数成功时返回对应的解码后的字符串, 失败则是一个空string(有特殊情况, 见warning部分).
		// @warning:
		//		1. 该函数的返回值不应该被忽略.
		//		2. 当要使用该函数的返回值时, 应该先检测ret.first是否为true.
		//		3. 当传入的是一个空string, 那么ret.first为true, ret.second为一个空string.
		[[nodiscard]] static std::pair<bool, std::string>
			base64DecodeString(
				const std::string& _string
			)
		{
			auto ret = Base64::base64DecodeStream(std::vector<byte>(_string.cbegin(), _string.cend()));
			return { ret.first, std::string(ret.second.cbegin(), ret.second.cend()) };
		}

		// @brief: 对_data进行Base64编码.
		// @param _data: 此参数将被视为一个要进行Base64编码的二进制流.
		// @return: 返回一个pair容器
		//		ret.first - 当函数成功时为true, 失败则为false.
		//		ret.second - 当函数成功时返回对应的编码后的二进制流, 失败则是一个空vector(有特殊情况, 见warning部分).
		// @warning:
		//		1. 该函数的返回值不应该被忽略.
		//		2. 当要使用该函数的返回值时, 应该先检测ret.first是否为true.
		//		3. 当传入的是一个空vector, 那么ret.first为true, ret.second为一个空vector.
		[[nodiscard]] static std::pair<bool, std::vector<byte>>
			base64EncodeStream(
				const std::vector<byte>& _data
			);

		// @brief: 对_data进行Base64解码.
		// @param _data: 此参数将被视为一个要进行Base64解码的二进制流.
		// @return: 返回一个pair容器
		//		ret.first - 当函数成功时为true, 失败则为false.
		//		ret.second - 当函数成功时返回对应的解码后的二进制流, 失败则是一个空vector(有特殊情况, 见warning部分).
		// @warning:
		//		1. 该函数的返回值不应该被忽略.
		//		2. 当要使用该函数的返回值时, 应该先检测ret.first是否为true.
		//		3. 当传入的是一个空vector, 那么ret.first为true, ret.second为一个空vector.
		[[nodiscard]] static std::pair<bool, std::vector<byte>>
			base64DecodeStream(
				const std::vector<byte>& _data
			);

	private:

		// @brief: 作为std::find_if模板函数的一元谓词, 用来检测base64EncodeMap中是否含有_value.
		// @param _value: 数值.
		// @return: 当_value与base64EncodeMap中的一个元素匹配时返回false, 否则返回true.
		static bool
			NotIn(
				const byte _ch
			);

		// @brief: 检测_data的有效性(是否含有非法的字节).
		// @param _data: 要进行检测有效性的二进制流.
		// @return: 如果该二进制流是有效的, 返回值为true, 否则为false.
		static bool
			checkForIllegalCharacters(
				const std::vector<byte>& _data
			);

		// @brief: 获取解码时实际上有意义的剩余字节数(也就是不包括补长字符('=')的总字节数).
		// @param _data: 此参数将被视为一个要进行Base64解码的二进制流.
		// @return: 返回解码时实际上有意义的剩余字节数, 没有错误返回.
		static std::vector<byte>::size_type
			getDecodeRemainingBytes(
				const std::vector<byte>& _data
			);

		// 以下均是实现Base64算法的函数, 每个函数都是隐式内联实现的.
		// -------------------------------------------------------------------------------------

		static byte
			getTheFirstCharacterAfterEncoding(
				const byte _ch
			)
		{
			return (Base64::base64EncodeMap[(_ch >> 2)]);
		}

		static byte
			getTheSecondCharacterAfterEncoding(
				const byte _ch1,
				const byte _ch2
			)
		{
			return (Base64::base64EncodeMap[(((_ch1 << 4) & 0x30) | (_ch2 >> 4))]);
		}

		static byte
			getTheThirdCharacterAfterEncoding(
				const byte _ch1,
				const byte _ch2
			)
		{
			return (Base64::base64EncodeMap[(((_ch1 & 0xF) << 2) | (_ch2 >> 6))]);
		}

		static byte
			getTheFourthCharacterAfterEncoding(
				const byte _ch
			)
		{
			return (Base64::base64EncodeMap[(_ch & 0x3F)]);
		}

		static byte
			getTheFirstCharacterAfterDecoding(
				const byte _ch1,
				const byte _ch2
			)
		{
			return ((Base64::base64DecodeMap[_ch1] << 2) | (Base64::base64DecodeMap[_ch2] >> 4));
		}

		static byte
			getTheSecondCharacterAfterDecoding(
				const byte _ch1,
				const byte _ch2
			)
		{
			return ((Base64::base64DecodeMap[_ch1] << 4) | (Base64::base64DecodeMap[_ch2] >> 2));
		}

		static byte
			getTheThirdCharacterAfterDecoding(
				const byte _ch1,
				const byte _ch2
			)
		{
			return ((Base64::base64DecodeMap[_ch1] << 6) | (Base64::base64DecodeMap[_ch2]));
		}

		// -------------------------------------------------------------------------------------

	private:

		enum {
			NO_REMAINING_CHARACTERS = 0,
			ONE_CHARACTER_REMAINING = 1,
			TWO_CHARACTERS_REMAINING = 2,
			THREE_CHARACTERS_REMAINING = 3,
			FOUR_CHARACTERS_REMAINING = 4,
		};

		enum {
			NUMBER_OF_CHARACTERS_ENCODED_IN_ONE_ROUND = 3,
			NUMBER_OF_CHARACTERS_DECODED_IN_ONE_ROUND = 4,
		};

		enum {
			BASE64_ENCODE_MAP_SIZE = 64,
			BASE64_DECODE_MAP_SIZE = 255,
		};

		// 当最后一轮编码不够3个字符的时候, Base64算法会使用此字符来对编码结果补长到4个字符.
		static constexpr byte COMPLEMENT_CHARACTER = '=';

		// Base64的编码映射表与解码映射表.
		static const byte base64EncodeMap[BASE64_ENCODE_MAP_SIZE];
		static const byte base64DecodeMap[BASE64_DECODE_MAP_SIZE];
	};
}

#endif // !_BASE64_H_UKXJCJ_7F2V5R_5CA68B_ZD3HHN_UDJC03_RR6IWH_
