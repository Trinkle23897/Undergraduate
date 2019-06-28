#include <gmpxx.h>
#include <sys/time.h>
#include <bits/stdc++.h>

class RSA {
public:
	int nbit, ebit, maxlen;
	mpz_class n, e, d;
	gmp_randstate_t grt;
	RSA(int _nbit=2048, int _ebit=17): nbit(_nbit), ebit(_ebit) {
		maxlen = nbit / 8 - 11;
		gmp_randinit_mt(grt);
		gmp_randseed_ui(grt, time(0));
		mpz_class p = get_prime(nbit / 2), q = get_prime(nbit / 2);
		n = p * q;
		mpz_class phi = (p - 1) * (q - 1), gcd;
		e = 65537;
		while (mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t()), gcd != 1)
			e = get_prime(ebit);
		mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
		printf("p: %s (10)\n", p.get_str().c_str());
		printf("q: %s (10)\n", q.get_str().c_str());
		printf("e: %s (10)\n", e.get_str().c_str());
		printf("d: %s (10)\n", d.get_str().c_str());
		printf("n: %s (10)\n", n.get_str().c_str());
	}
	bool witness(mpz_class& a, mpz_class& p) {
		int k = 0;
		mpz_class q = p - 1;
		while((q & 1) == 0)
			++k, q >>= 1;
		mpz_class v;
		mpz_powm(v.get_mpz_t(), a.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t());
		if(v == 1 || v == p - 1) // probably prime number
			return false;
		while(k-- != 0) {
			mpz_powm(v.get_mpz_t(), v.get_mpz_t(), v.get_mpz_t(), p.get_mpz_t());
			if(v == p - 1)
				return false;
		}
		return true; // composite number
	}
	bool check_prime(mpz_class& p, int bit, int rep=50) {
		if (p == 2)
			return true;
		if ((p & 1) == 0)
			return false;
		mpz_class r;
		for (int i = 0; i < rep; ++i) {
			mpz_rrandomb(r.get_mpz_t(), grt, bit);
			r = r % (p - 1) + 1;
			if (witness(r, p))
				return false;
		}
		return true;
	}
	mpz_class get_next_prime(mpz_class p, int bit) {
		while (check_prime(p, bit) == false)
			++p;
		return p;
	}
	mpz_class get_prime(int bit) {
		mpz_class p;
		mpz_rrandomb(p.get_mpz_t(), grt, bit);
		p = get_next_prime(p, bit);
		// mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());
		assert(mpz_probab_prime_p(p.get_mpz_t(), 50) > 0);
		return p;
	}
	inline mpz_class encrypt(const mpz_class&m) {
		mpz_class ret;
		mpz_powm(ret.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
		return ret;
	}
	inline mpz_class decrypt(const mpz_class&m) {
		mpz_class ret;
		mpz_powm(ret.get_mpz_t(), m.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
		return ret;
	}
	inline mpz_class str2mpz(std::string s) {
		char tmp[4];
		std::string hex;
		for (int i = 0; i < s.length(); ++i) {
			sprintf(tmp, "%x", s[i]);
			hex += tmp;
		}
		mpz_class ret;
		mpz_init_set_str(ret.get_mpz_t(), hex.c_str(), 16);
		return ret;
	}
	inline int char2int(char c) {
		if ('0' <= c && c <= '9')
			return c - '0';
		if ('a' <= c && c <= 'f')
			return c - 'a' + 10;
		if ('A' <= c && c <= 'F')
			return c - 'A' + 10;
	}
	inline std::string mpz2str(const mpz_class&m) {
		std::string s = m.get_str(16);
		char tmp[4];
		std::string t;
		for (int i = 0; i < s.length(); i += 2) {
			unsigned x = char2int(s[i]) << 4 | char2int(s[i + 1]);
			sprintf(tmp, "%c", x);
			t += tmp;
		}
		return t;
	}
	inline std::string encrypt(const std::string& s) {
		std::string* result = new std::string[s.length() / maxlen + 2];
#pragma omp parallel for schedule(dynamic, 1)
		for (int i = 0; i < s.length(); i += maxlen) {
			std::string tmp = encrypt(str2mpz(s.substr(i, maxlen))).get_str(16);
			for (int zeros = nbit / 4 - tmp.length(); zeros; --zeros)
				tmp = '0' + tmp;
			result[i / maxlen] = tmp;
		}
		std::string ret;
		for (int i = 0; i < s.length(); i += maxlen)
			ret += result[i / maxlen];
		return ret;
	}
	inline std::string decrypt(const std::string& s) {
		std::string* result = new std::string[s.length() / (nbit / 4) + 2];
#pragma omp parallel for schedule(dynamic, 1)
		for (int i = 0; i < s.length(); i += nbit / 4) {
			mpz_class tmp;
			mpz_init_set_str(tmp.get_mpz_t(), s.substr(i, nbit / 4).c_str(), 16);
			result[i / (nbit / 4)] = mpz2str(decrypt(tmp));
		}
		std::string ret;
		for (int i = 0; i < s.length(); i += nbit / 4)
			ret += result[i / (nbit / 4)];
		return ret;
	}
};

double mytime() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec * 1e-6 + tv.tv_sec;
}

int main(int argc, char const *argv[])
{
	srand(time(0));
	RSA rsa;
	std::string orig("moderncryptography");
	std::string s = rsa.encrypt(orig);
	printf("enc length: %lu\n", s.length());
	std::string t = rsa.decrypt(s);
	assert(orig == t);
	printf("pass checking!\n----------\n");
	int len = atoi(argv[1]);
	printf("gen strlen = %d random string\n", len);
	char* g = new char[len + 1];
	for (int i = 0; i < len; ++i)
		g[i] = rand() % 10 + '0';
	g[len] = 0;
	orig = g;
	double c0 = mytime();
	s = rsa.encrypt(orig);
	double c1 = mytime() - c0;
	printf("enc use %lfs, speed %.2lfM/s\n", c1, len / c1 / 1000000);
	c0 = mytime();
	t = rsa.decrypt(s);
	c1 = mytime() - c0;
	printf("dec use %lfs, speed %.2lfK/s\n", c1, len / c1 / 1000);
	assert(orig == t);
	printf("pass checking!\n");
	return 0;
}
