#include <cstdio>
#include <cstring>

#define N 128
#define B 16
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

struct BN {
	// A Hex-bigint implementation
	int len, a[N];
	BN() { len = 1; memset(a, 0, sizeof a); }
	BN(const char*c) { memset(a, 0, sizeof a); set(c); }
	BN(unsigned long long x) { memset(a, 0, sizeof a); set(x); }
	BN operator = (unsigned long long x) {
		memset(a, 0, sizeof a);
		set(x);
		return *this;
	}
	void set(const char*c) {
		len = min(strlen(c), N);
		if (len == 0) {
			len = 1;
			a[0] = 0;
			return;
		}
		for (int i = len - 1, j = 0; j < len; --i, ++j) {
			if ('a' <= c[i] && c[i] <= 'f')
				a[j] = c[i] - 'a' + 10;
			else if ('A' <= c[i] && c[i] <= 'F')
				a[j] = c[i] - 'A' + 10;
			else
				a[j] = c[i] - '0';
		}
		getlen();
	}
	void set(unsigned long long x) {
		len = 0;
		while (x > 0 && len < N) {
			a[len++] = x % B;
			x /= B;
		}
		getlen();
	}
	char* getstr() const {
		char* c = new char[len + 1];
		for (int i = len - 1, j = 0; j < len; --i, ++j)
			if (a[i] >= 10)
				c[j] = a[i] - 10 + 'a';
			else
				c[j] = a[i] + '0';
		c[len] = 0;
		return c;
	}
	inline bool operator == (const BN&b) const {
		if (len != b.len) return false;
		for (int i = 0; i < len; ++i)
			if (a[i] != b.a[i])
				return false;
		return true;
	}
	inline bool operator != (const BN&b) const {
		if (len != b.len) return true;
		for (int i = 0; i < len; ++i)
			if (a[i] != b.a[i])
				return true;
		return false;
	}
	inline bool operator < (const BN&b) const {
		if (len != b.len) return len < b.len;
		for (int i = len - 1; i >= 0; --i)
			if (a[i] != b.a[i])
				return a[i] < b.a[i];
		return false;
	}
	inline bool operator <= (const BN&b) const {
		if (len != b.len) return len < b.len;
		for (int i = len - 1; i >= 0; --i)
			if (a[i] != b.a[i])
				return a[i] < b.a[i];
		return true;
	}
	inline bool operator > (const BN&b) const {
		if (len != b.len) return len > b.len;
		for (int i = len - 1; i >= 0; --i)
			if (a[i] != b.a[i])
				return a[i] > b.a[i];
		return false;
	}
	inline bool operator >= (const BN&b) const {
		if (len != b.len) return len > b.len;
		for (int i = len - 1; i >= 0; --i)
			if (a[i] != b.a[i])
				return a[i] > b.a[i];
		return true;
	}
	inline int getlen() {
		for (len = N; len > 1 && a[len - 1] == 0; --len);
		return len;
	}
	BN operator + (const BN&b) const { // check
		BN c;
		for (int i = 0; i < len || i < b.len; ++i) {
			c.a[i] += a[i] + b.a[i];
			if (c.a[i] >= B)
				c.a[i] -= B, ++c.a[i + 1];
		}
		c.getlen();
		return c;
	}
	BN operator - (const BN&b) const { // check
		BN c;
		for (int i = 0; i < N; ++i) {
			c.a[i] += a[i] - b.a[i];
			if (c.a[i] < 0) {
				c.a[i] += B;
				if (i + 1 < N)
					--c.a[i + 1];
			}
		}
		c.getlen();
		return c;
	}
	BN& operator ++ () { // check
		if (++a[0] >= B) {
			for (int i = 0; i < N && a[i] >= B; ++i) {
				a[i] -= B;
				if (i + 1 < N)
					++a[i + 1];
			}
		}
		getlen();
		return *this;
	}
	BN& operator -- () { // check
		if (--a[0] < 0) {
			for (int i = 0; i < N && a[i] < 0; ++i) {
				a[i] += B;
				if (i + 1 < N)
					--a[i + 1];
			}
		}
		getlen();
		return *this;
	}
	BN operator * (const BN&b) const { // check
		BN c;
		for (int i = 0; i < len; ++i)
			for (int j = 0; j < b.len && i + j < N; ++j)
				c.a[i + j] += a[i] * b.a[j];
		c.len = min(N, len + b.len - 1);
		for (int i = 0; i < c.len; ++i)
			if (c.a[i] >= B) {
				if (i + 1 < N)
					c.a[i + 1] += c.a[i] / B;
				c.a[i] %= B;
			}
		c.getlen();
		return c;
	}
	BN operator << (int o) const { // check
		BN c;
		for (int i = 0; i < len; ++i)
			for (int j = 0; j < 4; ++j) {
				int target = (i << 2 | j) + o;
				c.a[target >> 2] ^= ((a[i] >> j) & 1) << (target & 3);
			}
		c.getlen();
		return c;
	}
	BN operator >> (int o) const { // check
		BN c;
		for (int i = 0; i < len; ++i)
			for (int j = 0; j < 4; ++j) {
				int target = (i << 2 | j) - o;
				if (target >= 0)
					c.a[target >> 2] ^= ((a[i] >> j) & 1) << (target & 3);
			}
		c.getlen();
		return c;
	}
	inline void shl4() {
		for (int i = min(N - 1, len); i > 0; --i)
			a[i] = a[i - 1];
		a[0] = 0;
		getlen();
	}
	inline void shr4() {
		for (int i = 0; i < len - 1; ++i)
			a[i] = a[i + 1];
		a[len - 1] = 0;
		getlen();
	}
	BN operator / (const BN&b) const {
		BN div, mod;
		divi(*this, b, div, mod);
		return div;
	}
	BN operator % (const BN&b) const {
		BN div, mod;
		divi(*this, b, div, mod);
		return mod;
	}
	friend void divi(const BN&a, const BN&b, BN&div, BN&mod) { // check
		mod = 0;
		if (b.len <= 1 && b.a[0] <= 1) {
			div = a;
			return;
		}
		// div = a // b
		// mod = a % b
		BN mb[B];
		for (int i = 1; i < B; ++i) {
			mb[i] = mb[i - 1] + b;
			if (mb[i] < mb[i - 1]) { // overflow
				int ans = 0;
				for (; ans < i - 1; ++ans)
					if (a < mb[ans + 1])
						break;
				div = ans;
				mod = a - mb[ans];
				return;
			}
		}
		mb[0] = 0; // wtf O2 ???
		for (int i = a.len - 1; i >= 0; --i) {
			mod.shl4();
			mod.a[0] = a.a[i];
			int index = 0;
			// find index
			for (; index < 15; ++index)
				if (mod < mb[index + 1])
					break;
			if (index > 0)
				mod = mod - mb[index];
			div.shl4();
			div.a[0] = index;
		}
		div.getlen();
		mod.getlen();
	}
	friend BN fast_mult(BN t, const BN&k, const BN&p) { // t * k % p
		BN f(0ull);
		for (int i = 0; i < k.len; ++i)
			for (int j = 0; j < 4; ++j) {
				if ((k.a[i] >> j) & 1) {
					f = f + t;
					if (f >= p)
						f = f - p;
				}
				t = t + t;
				if (t >= p)
					t = t - p;
			}
		return f;
	} 
	friend BN fast_power(BN t, const BN&k, const BN&p) { // t ** k % p
		BN f(1ull);
		t = t % p;
		// for (t %= p; k; k >>= 1, t = t * t % p)
		// 	if (k & 1)
		// 		f = f * t % p;
		for (int i = 0; i < k.len; ++i)
			for (int j = 0; j < 4; ++j) {
				if ((k.a[i] >> j) & 1)
					f = fast_mult(f, t, p);
				t = fast_mult(t, t, p);
			}
		return f;
	}
};

void test_BN() {
	char sa[10], sb[10], sc[10], sd[10];
	for (int i = 0; i < 4096; ++i)
	{
		BN a(i), b(i >> 4);
		a.shr4();
		assert(strcmp(a.getstr(), b.getstr()) == 0);
	}
	for (int i = 0; i < 4096; ++i)
		for (int j = 1; j < 4096; ++j) {
			sprintf(sa, "%x", i);
			sprintf(sb, "%x", j);
			sprintf(sc, "%x", (i / j) & 0xfff);
			sprintf(sd, "%x", (i % j) & 0xfff);
			// printf("--%d %d %x %x\n", i, j, (i / j) & 0xfff, (i % j) & 0xfff);
			BN a(sa), b(sb), c, d;
			// c = a / b;
			divi(a, b, c, d);
			// printf("%s %s\n", c.getstr(), d.getstr());
			// assert(strcmp(c.getstr(), sc) == 0);
			// assert(strcmp(d.getstr(), sd) == 0);
		}
}
