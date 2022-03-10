#include <stdint.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>

void work(uint16_t amount, uint16_t number);
std::string sha256(uint8_t pass[6]);
uint32_t rightrotate(uint32_t w, uint8_t s);
std::mutex cout_guard;

int main() {
	std::cout << "Введите количество потоков для перебора (1-26):";
	uint16_t amount;
	std::cin >> amount;
	if (amount == 1) {
		std::thread workerA(work, 1, 0);
	} else if (amount == 2) {
		std::thread workerA(work, 2, 0);
		std::thread workerB(work, 2, 1);
	} else if (amount == 4) {
		std::thread workerA(work, 4, 0);
		std::thread workerB(work, 4, 1);
		std::thread workerC(work, 4, 2);
		std::thread workerD(work, 4, 3);
	} else if (amount == 8) {
		std::thread workerA(work, 8, 0);
		std::thread workerB(work, 8, 1);
		std::thread workerC(work, 8, 2);
		std::thread workerD(work, 8, 3);
		std::thread workerE(work, 8, 4);
		std::thread workerF(work, 8, 5);
		std::thread workerG(work, 8, 6);
		std::thread workerH(work, 8, 7);
	} else if (amount == 16) {
		std::thread workerA(work, 16, 0);
		std::thread workerB(work, 16, 1);
		std::thread workerC(work, 16, 2);
		std::thread workerD(work, 16, 3);
		std::thread workerE(work, 16, 4);
		std::thread workerF(work, 16, 5);
		std::thread workerG(work, 16, 6);
		std::thread workerH(work, 16, 7);
		std::thread workerI(work, 16, 8);
		std::thread workerJ(work, 16, 9);
		std::thread workerK(work, 16, 10);
		std::thread workerL(work, 16, 11);
		std::thread workerM(work, 16, 12);
		std::thread workerN(work, 16, 13);
		std::thread workerO(work, 16, 14);
		std::thread workerP(work, 16, 15);
	}

	/*for (int i = 0; i < amount; i++) {
		work(amount, i);
		//std::thread worker(work, amount, i);
		//worker.detach();
	}*/
	//uint8_t pass[5] = { 97,98,99,100,101 };
	//std::cout << sha256(pass);
	return 0;
}

void work(uint16_t amount, uint16_t number) {

	uint8_t from = 26 / amount * number, to = 26 / amount * (number + 1);
	if (number < 26 % amount) {
		from = from + number;
		to = to + number + 1;
	} else {
		from = from + 26 % amount;
		to = to + 26 % amount;
	}

	uint8_t pass[6] = { 0, 0, 0, 0, 0, '\0'};
	for (int i = from; i < to; i++) {
		pass[0] = 97 + i;
		for (int j = 0; j < 26; j++) {
			pass[1] = 97 + j;
			for (int k = 0; k < 26; k++) {
				pass[2] = 97 + k;
				for (int l = 0; l < 26; l++) {
					pass[3] = 97 + l;
					for (int m = 0; m < 26; m++) {
						pass[4] = 97 + m;
						cout_guard.lock();
						std::cout << pass << '\n';
						cout_guard.unlock();
					}
				}
			}
		}
	}
}

std::string sha256(uint8_t pass[6]) {
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
	for (int i = 0; i < 16; i++) w[i] = (s[4*i] << 24) + (s[4*i + 1] << 16) + (s[4*i + 2] << 8) + s[4*i + 3];

	for (int i = 16; i < 64; i++) {
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

	std::stringstream sha256;
	sha256 << std::hex << h0 << h1 << h2 << h3 << h4 << h5 << h6 << h7;
	return sha256.str();
}

uint32_t rightrotate(uint32_t w, uint8_t s) {
	return (w >> s) + (w << (32 - s));
}