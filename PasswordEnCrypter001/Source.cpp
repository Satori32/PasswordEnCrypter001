#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdint>
#include <algorithm>
#include <tuple>

#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
//this is not URI_Encode.
//this is like a URI_encode.not same.

//Yaki is Burn.焼。no mean postfix.
std::string ItoAYaki(std::intmax_t N, const std::string& Ch) {
	std::string R;
	bool F = N >= 0 ? true : false;
	while (N != 0) {
		R += Ch[N % Ch.size()];
		N /= Ch.size();//radix divide
	}
	if (!F) R += '-';

	std::reverse(R.begin(), R.end());

	return R;


}
std::intmax_t AtoIYaki(const std::string& S, const std::string& Ch) {//this is not reference to gryph.
	std::uintmax_t R=0;
	bool F = S[0] != '-'? true : false;
	for(auto &o:S){
		if (o == '-')continue;
		auto it = std::find(Ch.begin(), Ch.end(),o);
		R *= Ch.size();//radix multiple.
		R += std::distance(Ch.begin(), it);
	}
	if (!F)R *= -1;

	return R;


}

std::string ReversibleHash_Encode(const std::string& S,const std::string& C) {
	std::string R;
	for (auto& o : S) {
		R += '%';
		R += ItoAYaki(static_cast<std::uint8_t>(o), C);
	}
	return R;
}
std::string ReversibleHash_Decode(const std::string& S,const std::string& C) {
	std::string R;
	std::string V;
	bool F = true;
	for (auto& o : S) {
		std::intmax_t N = o;
		if (o != '%') {
			F = false;
			V += o;
		}
		else {
			if (F)continue;
			R += AtoIYaki(V.data(),C);
			V.clear();
		}
	}
	if (V.size())R += AtoIYaki(V.data(), C);
	return R;
}
std::tuple<std::string,std::string> GetInputByManual() {

	std::string Mail="example@email.com";
	std::string Pass="password";


	return { Mail,Pass };

}
std::tuple<std::string,std::string> GetInput() {

	std::string Mail;
	std::string Pass;

	std::getline(std::cin, Mail);
	std::getline(std::cin, Pass);

	return { Mail,Pass };

}


//https://ja.wikipedia.org/wiki/MD5

typedef std::vector<bool> BType;
typedef std::vector<std::uint32_t> DType;
typedef std::vector<std::uint8_t> Bytes;

template<class T>
T BitToUIntN(const BType& B) {
	std::size_t BitSize = std::numeric_limits<T>::digits;
	std::size_t ByteBit = std::numeric_limits<std::uint8_t>::digits;
	std::size_t ByteSize = BitSize / ByteBit;
	T R = 0;

	for (std::size_t i = 0; i < B.size() / ByteBit; i++) {
		for (std::size_t j = 0; j < ByteBit; j++) {
			std::size_t idx = 8 * i + j;

			R |= (B[idx] ? 1ull : 0) << (((ByteBit - 1) - j) + (i * ByteBit));
		}
	}
	return R;
}

DType MD5(const Bytes& Data) {

	BType Bits;


	auto UIntNToBit = [](const auto& N)-> BType {
		std::size_t BitSize = std::numeric_limits<decltype(N)>::digits;
		std::size_t ByteBit = std::numeric_limits<std::uint8_t>::digits;
		std::size_t ByteSize = BitSize / ByteBit;

		BType B;

		for (std::size_t i = 0; i < ByteSize; i++) {
			for (std::size_t j = 0; j < ByteBit; j++) {
				std::size_t BS = i * ByteBit;
				std::size_t BB = (ByteBit - 1 - j);

				B.push_back(N & (1ull << (BS + BB)));
			}
		}

		return B;
	};

	auto BitToUInt32 = [](const BType& B) -> std::uint32_t {
		std::uint32_t R = 0;
		for (std::size_t i = 0; i < B.size() / 8; i++) {
			for (std::size_t j = 0; j < 8; j++) {
				std::size_t idx = 8 * i + j;
				R |= (B[idx] ? 1 : 0) << ((7 - j) + ((8 * i)));
			}
		}
		return R;
	};

	auto LeftRotate = [](const std::uint32_t& X, const std::int32_t& C)->std::uint32_t {
		return (X << C) | (X >> (32 - C));
	};
	auto makeK = [](const std::uint32_t& i) -> std::uint32_t {
		return static_cast<std::uint32_t>(std::floor(((1ull << 32)) * std::abs(std::sin(i + 1.0))));
	};

	/* * /
	DType K;

	for (std::size_t i = 0; i < 64; i++) {
		K.push_back(makeK(i));
	}
	/**/
	DType K =
	{
		0xD76AA478, 0xE8C7B756, 0x242070DB,0xC1BDCEEE,
		0xF57C0FAF, 0x4787C62A, 0xA8304613,0xFD469501,
		0x698098D8, 0x8B44F7AF, 0xFFFF5BB1,0x895CD7BE,
		0x6B901122, 0xFD987193, 0xA679438E,0x49B40821,
		0xF61E2562, 0xC040B340, 0x265E5A51,0xE9B6C7AA,
		0xD62F105D, 0x02441453, 0xD8A1E681,0xE7D3FBC8,
		0x21E1CDE6, 0xC33707D6, 0xF4D50D87,0x455A14ED,
		0xA9E3E905, 0xFCEFA3F8, 0x676F02D9,0x8D2A4C8A,
		0xFFFA3942, 0x8771F681, 0x6D9D6122,0xFDE5380C,
		0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60,0xBEBFBC70,
		0x289B7EC6, 0xEAA127FA, 0xD4EF3085,0x04881D05,
		0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8,0xC4AC5665,
		0xF4292244, 0x432AFF97, 0xAB9423A7,0xFC93A039,
		0x655B59C3, 0x8F0CCC92, 0xFFEFF47D,0x85845DD1,
		0x6FA87E4F, 0xFE2CE6E0, 0xA3014314,0x4E0811A1,
		0xF7537E82, 0xBD3AF235, 0x2AD7D2BB,0xEB86D391,
	};
	/**/

	const DType s =
	{
	 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
	};

	for (std::size_t i = 0; i < Data.size(); i++) {
		auto B = UIntNToBit(Data[i]);
		Bits.insert(Bits.end(), B.begin(), B.end());

	}

	Bits.push_back(true);
	std::uint64_t IML = Data.size() * 8;
	while ((Bits.size() % 512) != 448) {
		Bits.push_back(false);
	}

	for (std::size_t i = 0; i < 2; i++) {
		for (std::int32_t j = 0; j < 4; j++) {
			for (std::size_t k = 0; k < 8; k++) {
				Bits.push_back(IML & ((1ull << ((j * 8) + (7 - k))) * i));
			}
		}
	}

	//A、B、C、Dの初期値
	std::uint32_t a0 = 0x67452301; // A
	std::uint32_t b0 = 0xEFCDAB89; // B
	std::uint32_t c0 = 0x98BADCFE; // C
	std::uint32_t d0 = 0x10325476; // D

	for (std::size_t j = 0; j < (Bits.size() / 512); j++) {
		DType M;
		for (std::size_t k = 0; k < 16; k++) {
			BType T{ Bits.begin() + (k * 32) + (j * 512) ,Bits.begin() + ((k + 1) * 32) + (j * 512) };
			M.push_back(BitToUIntN<std::uint32_t>(T));
		}

		std::uint32_t A = a0;
		std::uint32_t B = b0;
		std::uint32_t C = c0;
		std::uint32_t D = d0;

		std::uint32_t F = 0;
		std::int32_t g = 0;

		for (std::size_t i = 0; i < 64; i++) {
			if (0 <= i && i <= 15) {
				F = (B & C) | ((~B) & D);
				g = i;
			}
			else if (16 <= i && i <= 31) {
				F = (D & B) | ((~D) & C);
				g = ((5 * i) + 1) % 16;
			}
			else if (32 <= i && i <= 47) {
				F = (B ^ C) ^ D;
				g = ((3 * i) + 5) % 16;
			}
			else if (48 <= i && i <= 63) {
				F = C ^ (B | (~D));
				g = (7 * i) % 16;
			}

			F = F + A + K[i] + M[g];
			D = C;
			C = B;
			A = D;

			B = B + LeftRotate(F, s[i]);
		}
		a0 = a0 + A;
		b0 = b0 + B;
		c0 = c0 + C;
		d0 = d0 + D;
	}
	DType md5 = { a0,b0,c0,d0 };

	return md5;
}

//2019/0/14
//this MD5 function is modification to Wikipedia algorithm.
//but not match to sample anther. i am confusing...
int main() {

	std::string Mail;
	std::string Pass;

	std::tie(Mail, Pass) = GetInputByManual();
	//std::tie(Mail, Pass) = GetInput();

	std::string T = Mail;
	std::string C;
	for (std::size_t i = 0; i < T.size(); i++) {
		if (T[i] == '.')continue;
		if (T[i] == '@')continue;
		auto it = std::find(C.begin(), C.end(), T[i]);
		if (it == C.end()) { C += T[i]; }
	}

	std::string RM = ReversibleHash_Encode(Mail, C);
	std::string RP = ReversibleHash_Encode(Pass, C);

	std::cout << Mail << " -> " << RM << " -> " << ReversibleHash_Decode(RM, C) << std::endl;
	std::cout << Pass << " -> " << RP << "  -> " << ReversibleHash_Decode(RP, C) << std::endl;
	std::cout << "Code" << "  -> " << C << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	DType D;
	std::cout << "MD5??" << std::endl;
	std::cout << Mail << " -> " << RM << std::endl;
	D = MD5({ RM.begin(),RM.end() });
	for (auto& o : D) { std::cout << std::hex << o << std::dec << ' '; }
	std::cout << std::endl;


	std::cout << Pass << " -> " << RP << std::endl;
	D = MD5({ RP.begin(),RP.end() });
	for (auto& o : D) { std::cout << std::hex << o << std::dec << ' '; }
	std::cout << std::endl;

	return 0;
}
