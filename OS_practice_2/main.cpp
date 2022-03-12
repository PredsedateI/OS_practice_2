#include <stdint.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <Windows.h>

class hash_int32 {
public:
	uint32_t h0;
	uint32_t h1;
	uint32_t h2;
	uint32_t h3;
	uint32_t h4;
	uint32_t h5;
	uint32_t h6;
	uint32_t h7;
	hash_int32(std::string hash) {
		h0 = stoul(hash.substr(0, 8), 0, 16);
		h1 = stoul(hash.substr(8, 8), 0, 16);
		h2 = stoul(hash.substr(16, 8), 0, 16);
		h3 = stoul(hash.substr(24, 8), 0, 16);
		h4 = stoul(hash.substr(32, 8), 0, 16);
		h5 = stoul(hash.substr(40, 8), 0, 16);
		h6 = stoul(hash.substr(48, 8), 0, 16);
		h7 = stoul(hash.substr(56, 8), 0, 16);
	}
	hash_int32(uint32_t n0, uint32_t n1, uint32_t n2, uint32_t n3, uint32_t n4, uint32_t n5, uint32_t n6, uint32_t n7) {
		h0 = n0;
		h1 = n1;
		h2 = n2;
		h3 = n3;
		h4 = n4;
		h5 = n5;
		h6 = n6;
		h7 = n7;
	}
};
bool operator == (hash_int32 a, hash_int32 b) {
	return { a.h0 == b.h0 and a.h1 == b.h1 and a.h2 == b.h2 and a.h3 == b.h3 and a.h4 == b.h4 and a.h5 == b.h5 and a.h6 == b.h6 and a.h7 == b.h7 };
}

void work(uint16_t number, uint16_t amount, hash_int32 hash);
hash_int32 sha256(uint8_t pass[6]);
uint32_t rightrotate(uint32_t w, uint8_t s);

std::mutex cout_guard;
uint8_t pass[11881376][6];

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	for (uint32_t i = 0; i < 11881376; i++) {
		pass[i][0] = i / 26 / 26 / 26 / 26 % 26 + 97;
		pass[i][1] = i / 26 / 26 / 26 % 26 + 97;
		pass[i][2] = i / 26 / 26 % 26 + 97;
		pass[i][3] = i / 26 % 26 + 97;
		pass[i][4] = i % 26 + 97;
		pass[i][5] = 0;
	}

	std::string hash;
	std::cout << "Введите хеш: ";
	std::cin >> hash;

	hash_int32 hash_int(hash);
	
	uint16_t amount;
	std::cout << "Введите количество потоков для перебора: ";
	std::cin >> amount;

	std::thread* worker = new std::thread[amount];
	for (uint32_t i = 0; i < amount; i++) {
		worker[i] = std::thread(work, i, amount, hash_int);
	}
	for (uint32_t i = 0; i < amount; i++) {
		worker[i].join();
	}

	return 0;
}

void work(uint16_t number, uint16_t amount, hash_int32 hash) {

	uint32_t from = 11881376 / amount * number, to = 11881376 / amount * (number + 1);
	if (number < 11881376 % amount) {
		from = from + number;
		to = to + number + 1;
	}
	else {
		from = from + 11881376 % amount;
		to = to + 11881376 % amount;
	}

	for (uint32_t i = from; i < to; i++) {
		if (hash == sha256(pass[i])) {
			cout_guard.lock();
			std::cout << pass[i] << '\n';
			cout_guard.unlock();
		}
	}

}

hash_int32 sha256(uint8_t pass[6]) {
	uint8_t s[64] = {
		pass[0], pass[1], pass[2], pass[3], pass[4], 128, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 40,
	};

	uint32_t h0 = 0x6a09e667;
	uint32_t h1 = 0xbb67ae85;
	uint32_t h2 = 0x3c6ef372;
	uint32_t h3 = 0xa54ff53a;
	uint32_t h4 = 0x510e527f;
	uint32_t h5 = 0x9b05688c;
	uint32_t h6 = 0x1f83d9ab;
	uint32_t h7 = 0x5be0cd19;

	uint32_t k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	uint32_t w[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};
	for (uint32_t i = 0; i < 16; i++) w[i] = (s[4 * i] << 24) + (s[4 * i + 1] << 16) + (s[4 * i + 2] << 8) + s[4 * i + 3];

	for (uint32_t i = 16; i < 64; i++) {
		uint32_t s0 = rightrotate(w[i - 15], 7) ^ rightrotate(w[i - 15], 18) ^ (w[i - 15] >> 3);
		uint32_t s1 = rightrotate(w[i - 2], 17) ^ rightrotate(w[i - 2], 19) ^ (w[i - 2] >> 10);
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
	}

	uint32_t a = h0;
	uint32_t b = h1;
	uint32_t c = h2;
	uint32_t d = h3;
	uint32_t e = h4;
	uint32_t f = h5;
	uint32_t g = h6;
	uint32_t h = h7;

	for (int i = 0; i < 64; i++) {
		uint32_t s1 = rightrotate(e, 6) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t temp1 = h + s1 + ch + k[i] + w[i];
		uint32_t s0 = rightrotate(a, 2) ^ rightrotate(a, 13) ^ rightrotate(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t temp2 = s0 + maj;
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}

	h0 += a;
	h1 += b;
	h2 += c;
	h3 += d;
	h4 += e;
	h5 += f;
	h6 += g;
	h7 += h;
	
	return hash_int32( h0, h1, h2, h3, h4, h5, h6, h7 );
}

uint32_t rightrotate(uint32_t w, uint8_t s) {
	return (w >> s) + (w << (32 - s));
}