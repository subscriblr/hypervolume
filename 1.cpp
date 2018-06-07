#include<bits/stdc++.h>
#define mp make_pair
#define pb push_back      
#define F first
#define S second

typedef double ints;

#define TASK "1"

using namespace std;


struct algo {
	vector<vector<ints> > input;
	int d;
	int n;

	algo() { n = d = 0; }
	algo(int dd): d(dd){
	}
	virtual void load(vector<vector<ints> > a) {
		n = a.size();
		if (n > 0) d = a[0].size();
		input = a;
	}
	algo(vector<vector<ints> > a) {
		load(a);
		n = a.size();
		if (n > 0) d = a[0].size();
	}
	virtual ints eval() {
		return 0;
	}
	virtual vector<ints> get_stat() {
		return vector<ints>();
	}

	virtual string name() {return "#";}
	void to_min(vector<ints> & a, vector<ints> & b) {
		int d = a.size();
		if (a.size() == 0) a = b; else {
			for (int i = 0; i < d; i++) a[i] = min(a[i], b[i]);
		}
	}
	int less_or_equal(vector<ints> & a, vector<ints> & b) {
		int d = a.size();
		for (int i = 0; i < d; i++) if (a[i] > b[i]) return 0;
		return 1;
	}


	ints get_volume(vector<ints> & a) {
		ints ret = 1;
		for (ints x : a) ret *= x;
		return ret;
	}


	ints compress(vector<vector<ints> > & a) {
		ints ret = 1;
		int d = a[0].size();
		bool found = false;
		for (int i = d - 1; i >= 0; i--) {
			int eq = true;
			for (int j = 1; j < (int)a.size(); j++) if (a[j][i] != a[0][i]) {
				eq = false;
				break;
			}
			if (eq) {
				found = true;
				ret *= a[0][i];
				for (int j = 0; j < (int)a.size(); j++) {
					a[j][i] = a[j].back();
					a[j].pop_back();
				}
			}
		}
		if (!found) return -1;
		return ret;
	}


	void make_free(vector<vector<ints> > & a) {
		if (a.size() == 0) return;
		for (int i = (int)a.size() - 1; i >=0 ; i--) {
			for (int j = 0; j < (int)a.size(); j++) if (i != j && less_or_equal(a[i], a[j])) {
				//remove a[i]
				a[i] = a.back();
				a.resize(a.size() - 1);
				break;
			}
		}

	}
};

struct hso : algo {

	hso() : algo() {}
	hso(vector<vector<ints> > a) : algo(a) {}
	virtual string name() {return "hso";}

	ints go(vector<vector<ints> > &a) {
		if (a.size() == 0) return 1;
		make_free(a);
		if (a.size() == 1) return get_volume(a[0]);

		int d = a[0].size();

		int id = d - 1;

		vector<ints> x;
		for (auto p : a) x.pb(p[id]);
		sort(x.begin(), x.end());

		ints ret = 0;

		ints prev = 0;
		for (int i = 0; i < (int)x.size(); i++) {
			vector<vector<ints> > b;
			for (auto p : a) if (p[id] >= x[i]) b.pb(p);

			for (int j = 0; j < (int)b.size(); j++) {
				b[j][id] = b[j].back();
				b[j].pop_back();
			}
			ret += (x[i] - prev) * go(b);
			prev = x[i];
		}
		return ret;

	}

	ints eval() {
		return go(input);
	};	

};



struct inc_excl : algo {

	inc_excl() : algo() {}
	inc_excl(vector<vector<ints> > a) : algo(a) {}
	virtual string name() {return "inc_excl";}

	ints eval() {
		ints ret = 0;
		for (int mask = 1; mask < (1 << n); mask++) {
			vector<ints> x;
			int cnt = 0;
			for (int i = 0; i < n; i++) if (mask & (1 << i)) {
				cnt++;
				to_min(x, input[i]);
			}
			ints cur = get_volume(x);
			if ((cnt & 1) == 0) cur *= -1;
			ret += cur;
		}
		return ret;
	};	
};

struct walking_fish : algo {

	long long go_cnt;

	walking_fish() : algo() {go_cnt = 0;}
	walking_fish(int d) : algo(d) {go_cnt = 0;}
	walking_fish(vector<vector<ints> > a) : algo(a) {go_cnt = 0;}
	virtual string name() {return "walking_fish";}

	vector<vector<double> > C;


	ints eval() {

		int n = input.size();
		C.resize(n + 1);
		for (int i = 0; i <= n; i++) C[i].resize(n + 1);
		for (int i = 0; i <= n; i++) for (int j = 0; j <= i; j++) if (j == 0 || i == j) C[i][j] = 1; else C[i][j] = C[i - 1][j] + C[i - 1][j - 1];

		return go(input);
	};	

	
	virtual int make_choice(vector<vector<ints> > &a) {
		return (int)a.size() - 1;
	}

	ints make_a_step(vector<vector<ints> > a) {
		go_cnt++;
		if (a.size() == 0) return 0;
		if (a.size() == 1) return get_volume(a[0]);

		make_free(a);


		int id = make_choice(a);

		vector<ints> & p = a[id];

		vector<vector<ints> > b = a;
		b[id] = b.back();
		b.resize(b.size() - 1);
		ints result = go(b);
		for (int i = 0; i < (int)b.size(); i++) to_min(b[i], p);

		result += get_volume(p) - go(b);

		return result;
	}

	virtual ints go(vector<vector<ints> > a) {
		return make_a_step(a);
	}
};


struct wf_smallest_sum : walking_fish {

	wf_smallest_sum() : walking_fish() {}
	wf_smallest_sum(int d) : walking_fish(d) {}
	wf_smallest_sum(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_smallest_sum";}


	int make_choice(vector<vector<ints> > &a) override {
		ints best = 0;
		int id = -1;
		for (int i = 0; i < (int)a.size(); i++) {
			ints s = accumulate(a[i].begin(), a[i].end(), 0);
			if (id == -1 || s < best) {
				best = s;
				id = i;
			}
		}
		return id;
	}
};


struct wf_take_smallest : walking_fish {

	wf_take_smallest() : walking_fish() {}
	wf_take_smallest(int d) : walking_fish(d) {}
	wf_take_smallest(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_take_smallest";}


	int make_choice(vector<vector<ints> > &a) override {
		int d = a[0].size();
		for (int i = 0; i < d; i++) {
			int id1 = 0;
			int id2 = 0;
			for (int j = 1; j < (int)a.size(); j++) {
				if (a[j][i] < a[id1][i]) id1 = j;
				if (a[j][i] > a[id2][i]) id2 = j;
			}
			if (id1 != id2) return id1;
		}
		return (int)a.size() - 1;
	}
};

struct wf_smallest_volume : walking_fish {

	wf_smallest_volume() : walking_fish() {}
	wf_smallest_volume(int d) : walking_fish(d) {}
	wf_smallest_volume(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_smallest_volume";}


	int make_choice(vector<vector<ints> > &a) override {
		int id = 0;
		ints cur = get_volume(a[0]);
		for (int i = 1; i < (int)a.size(); i++) {
			ints t = get_volume(a[i]);
			if (t < cur) {
				cur = t;
				id = i;
			}
		}
		return id;
	}
};

struct hso_into_wf : algo {

	wf_take_smallest solver;

	hso_into_wf() : algo() {solver = wf_take_smallest();}
	hso_into_wf(vector<vector<ints> > a) : algo(a) {solver = wf_take_smallest();}
	virtual string name() {return "hso_into_wf";}

	ints go(vector<vector<ints> > &a) {
		if (a.size() == 0) return 1;
		make_free(a);
		if (a.size() == 1) return get_volume(a[0]);

		int d = a[0].size();

		int id = rand() % d;

		vector<ints> x;
		for (auto p : a) x.pb(p[id]);
		sort(x.begin(), x.end());

		ints ret = 0;

		ints prev = 0;
		for (int i = 0; i < (int)x.size(); i++) {
			vector<vector<ints> > b;
			for (auto p : a) if (p[id] >= x[i]) b.pb(p);

			for (int j = 0; j < (int)b.size(); j++) {
				b[j][id] = b[j].back();
				b[j].pop_back();
			}
			solver.load(b);
			ret += (x[i] - prev) * solver.eval();
			prev = x[i];
		}
		return ret;

	}

	ints eval() {
		return go(input);
	};	

};


struct wf_smallest_and_volume : walking_fish {

	wf_smallest_and_volume() : walking_fish() {}
	wf_smallest_and_volume(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_smallest_and_volume";}


	int make_choice(vector<vector<ints> > &a) override {
		int d = a[0].size();
		int id = -1;
		ints cur = 0;
		for (int i = 0; i < d; i++) {
			int id1 = 0;
			int id2 = 0;
			for (int j = 1; j < (int)a.size(); j++) {
				if (a[j][i] < a[id1][i]) id1 = j;
				if (a[j][i] > a[id2][i]) id2 = j;
			}
			if (id1 != id2) {
				ints t = get_volume(a[id1]);
				if (id == -1 || t < cur) {
					id = id1;
					cur = t;
				}
			}
		}
		if (id == -1) return (int)a.size() - 1;
		return id;
	}
};



struct wf_presort : walking_fish {

	wf_presort() : walking_fish() {}
	wf_presort(int d) : walking_fish(d) {}
	wf_presort(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_presort";}

	void load(vector<vector<ints> > a) {
		if (a.size() > 0) {
			for (int i = 0; i < (int)a.size(); i++) for (int j = i + 1; j < (int)a.size(); j++) if (a[i][0] < a[j][0]) swap(a[i], a[j]);
		}
		input = a;
	}


	int make_choice(vector<vector<ints> > &a) override {
		return (int)a.size() - 1;
	}
};

struct wf_sort : walking_fish {

	wf_sort() : walking_fish() {}
	wf_sort(int d) : walking_fish(d) {}
	wf_sort(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_sort";}

/*	void load(vector<vector<ints> > a) {
		if (a.size() > 0) {
			for (int i = 0; i < (int)a.size(); i++) for (int j = i + 1; j < (int)a.size(); j++) if (a[i][0] < a[j][0]) swap(a[i], a[j]);
		}
		input = a;
	}*/

	ints go(vector<vector<ints> > a) override {
		if (a.size() == 0) return 0;
		if (a.size() == 1) return get_volume(a[0]);

		make_free(a);
		int d = a[0].size();

		for (int i = 0; i < (int)a.size(); i++) for (int j = i + 1; j < (int)a.size(); j++) if (a[i][d-1] > a[j][d-1]) swap(a[i], a[j]);


		ints vol = 0;
		for (int i = 0; i < (int)a.size(); i++) {
			vector<vector<ints> > b;
			for (int j = i + 1; j < (int)a.size(); j++) {
				vector<ints> t = a[j];
				for (int k = 0; k < d; k++) t[k] = min(t[k], a[i][k]);
				t.pop_back();
				b.pb(t);
			}
			vol += get_volume(a[i]) - a[i][d - 1] * go(b);
		}
		return vol;
		
	}

};

struct wf_estimate : walking_fish {

	wf_estimate() : walking_fish() {}
	wf_estimate(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_estimate";}


	double estimate(vector<vector<ints> > &a) {
		if (a.size() <= 1) return 1;
		make_free(a);
		int d = a[0].size();
		int k = 0;
		for (int i = 0; i < d; i++) {
			int id1 = 0;
			int id2 = 0;
			for (int j = 1; j < (int)a.size(); j++) {
				if (a[j][i] < a[id1][i]) id1 = j;
				if (a[j][i] > a[id2][i]) id2 = j;
			}
			if (id1 != id2) k++;
		}
		int n = (int)a.size();
		double Et = 0;
		for (int i = 0; i <= k; i++) Et += C[n][i];

		return Et;
	}

	long long recursive_estimate(vector<vector<ints> > &a, int depth) {
		int n = a.size();
		if (depth == 0) return estimate(a);

		vector<int> options;
		options.pb(wf_take_smallest().make_choice(a));

		double best = -1;
		int id = -1;
		for (int i = 0; i < n; i++) {
			vector<ints> & p = a[i];
			vector<vector<ints> > b = a;
			b[i] = b.back();
			b.resize(b.size() - 1);

			double Et = estimate(b);
			for (int j = 0; j < (int)b.size(); j++) to_min(b[j], p);
			Et += estimate(b);
			if (id == -1 || Et < best) {
				best = Et;
				id = i;
			}
		}
		options.pb(id);

		sort(options.begin(), options.end());
		options.resize(unique(options.begin(), options.end()) - options.begin());

		best = -1;
		for (int i : options) {
			vector<ints> & p = a[i];
			vector<vector<ints> > b = a;
			b[i] = b.back();
			b.resize(b.size() - 1);

			double Et = recursive_estimate(b, depth - 1);
			for (int j = 0; j < (int)b.size(); j++) to_min(b[j], p);
			Et += recursive_estimate(b, depth - 1);
			if (best == -1 || Et < best) best = Et;
		}
		return best;
	}

	int make_choice(vector<vector<ints> > &a) override {
		int n = a.size();
		if (n < 24) return wf_take_smallest().make_choice(a);
//		if (n < 15) return n - 1;
		
//		int vv = wf_take_smallest(d).make_choice(a);



		double best = -1;
		int id = -1;
		for (int i = 0; i < n; i++) {
			vector<ints> & p = a[i];
			vector<vector<ints> > b = a;
			b[i] = b.back();
			b.resize(b.size() - 1);

			double Et = recursive_estimate(b, 0);
			for (int j = 0; j < (int)b.size(); j++) to_min(b[j], p);
			Et += recursive_estimate(b, 0);

			if (id == -1 || Et < best) {
				best = Et;
				id = i;
			}
		}

		assert(id >= 0);
		return id;
	}
};


struct wf_map_estimate : walking_fish {

	wf_map_estimate() : walking_fish() {}
	wf_map_estimate(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_map_estimate";}

	map<long long, ints> was;
	map<long long, ints> was2;

	int all = 0, found = 0;

	long long get_hash(vector<vector<ints> > &a) {
		int n = a.size();
		if (n <= 1) return 0;
		vector<int> q1(n, 0);
		vector<int> q2(n, 0);
		vector<int> q3(n, 0);

		for (int i = 0; i < d; i++) {
			int id1 = 0;
			int id2 = 0;

			int second = 0;
			for (int j = 1; j < n; j++) {
				if (a[j][i] < a[id1][i]) {
					second = id1;
					id1 = j;
				} else if (a[j][i] < a[second][i]) {
					second = j;
				}

				if (a[j][i] > a[id2][i]) id2 = j;
			}
			if (id1 == id2) continue;
			q1[id1]++;
			q2[second]++;
//			q3[id2]++;
		}
		for (int i = 0; i < n; i++) q1[i] = q1[i] * (n + 2) * (n + 2) + q2[i] * (n + 2) + q3[i] + 1;
		sort(q1.begin(), q1.end());
		q1.pb(n);

		long long mod = 1e12 + 7;

		long long ret = 0;
		for (int x : q1) ret = (ret * 1000007 + x) % mod;

		return ret;
	}


	double estimate(vector<vector<ints> > &a) {
		if (a.size() <= 1) return 1;
		make_free(a);
		long long h = get_hash(a);
		if (was.count(h)) {
			return was[h] / 1. / was2[h];
		} else return -1;
	}

	int make_choice(vector<vector<ints> > &a) override {
		int n = a.size();
		if (n < 5) return wf_take_smallest().make_choice(a);
//		if (n < 5) return walking_fish(d).make_choice(a);

		double best = -1;
		int id = -1;

//		int go_heur = walking_fish(d).make_choice(a);
		int go_heur = wf_take_smallest().make_choice(a);
		bool have_est = false;

		for (int i = 0; i < n; i++) {
			vector<ints> & p = a[i];
			vector<vector<ints> > b = a;
			b[i] = b.back();
			b.resize(b.size() - 1);

			double Et = estimate(b);
			if (Et < 0) continue;

			for (int j = 0; j < (int)b.size(); j++) to_min(b[j], p);

			double t2 = estimate(b);
			if (t2 < 0) continue;
			Et += t2;

			if (i == go_heur) have_est = true;

			if (id == -1 || Et < best) {
				best = Et;
				id = i;
			}
		}
		if (!have_est) return go_heur;

		if (id != -1) {
			return id;
		}
		return wf_take_smallest().make_choice(a);
//		return walking_fish(d).make_choice(a);
	}

	ints go(vector<vector<ints> > a) {
		long long h = get_hash(a);

		long long t = go_cnt;


		ints ret = make_a_step(a);
		
		t = go_cnt - t;

		/*if (was2[h] != 0 && a.size() > 15) {
			double here = was[h] / 1. / was2[h];
			double q = max(here, (double)t) / 1. / min(here, (double)t);
			cout << q << endl;
			found++;
		}
		if (a.size() > 15) all++;
		if (a.size() > 15) {
			if (go_cnt % 1000 == 0) {
				cerr << found / 1. / all << endl;	
				cerr << was.size() << endl;
				found = all = 0;
			}
		}*/


		was[h] += t;
		was2[h]++;
//		was[h] = t;
//		was2[h] = 1;

		return ret;
	}
};


struct wf_map_predictor : walking_fish {

	wf_map_predictor() : walking_fish() {}
	wf_map_predictor(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_map_predictor";}

	map<long long, ints> was;
	map<long long, ints> was2;

	int all = 0, found = 0;

	long long get_hash(vector<vector<ints> > &a) {
		int n = a.size();
		if (n <= 1) return 0;
		vector<int> q1(n, 0);
		vector<int> q2(n, 0);
		vector<int> q3(n, 0);

		for (int i = 0; i < d; i++) {
			int id1 = 0;
			int id2 = 0;

			int second = 0;
			for (int j = 1; j < n; j++) {
				if (a[j][i] < a[id1][i]) {
					second = id1;
					id1 = j;
				} else if (a[j][i] < a[second][i]) {
					second = j;
				}

				if (a[j][i] > a[id2][i]) id2 = j;
			}
			if (id1 == id2) continue;
			q1[id1]++;
			q2[second]++;
//			q3[id2]++;
		}
		for (int i = 0; i < n; i++) q1[i] = q1[i] * (n + 2) * (n + 2) + q2[i] * (n + 2) + q3[i] + 1;
		sort(q1.begin(), q1.end());
		q1.pb(n);

		long long mod = 1e12 + 7;

		long long ret = 0;
		for (int x : q1) ret = (ret * 1000007 + x) % mod;

		return ret;
	}

	int make_choice(vector<vector<ints> > &a) override {
		return wf_take_smallest().make_choice(a);
	}

	ints go(vector<vector<ints> > a) {
		long long h = get_hash(a);
		if (was2[h] != 0) {
			return was[h] / 1. / was2[h];
		}

		long long t = go_cnt;


		ints ret = make_a_step(a);
		
		t = go_cnt - t;

		was[h] += t;
		was2[h]++;
		return ret;
	}
};


struct wf_map_sort : walking_fish {

	wf_map_sort() : walking_fish() {}
	wf_map_sort(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_map_sort";}

	map<long long, ints> was;
	map<long long, ints> was2;

	int all = 0, found = 0;
	long long worked = 0;
	vector<int> p;

	long long get_hash(vector<vector<ints> > &a) {
		int n = a.size();
		if (n <= 1) return 0;
		vector<int> q1(n, 0);
		vector<int> q2(n, 0);
		vector<int> q3(n, 0);

		int d = a[0].size();

		for (int i = 0; i < d; i++) {
			int id1 = 0;
			int id2 = 0;

			int second = 0;
			for (int j = 1; j < n; j++) {
				if (a[j][i] < a[id1][i]) {
					second = id1;
					id1 = j;
				} else if (a[j][i] < a[second][i]) {
					second = j;
				}

				if (a[j][i] > a[id2][i]) id2 = j;
			}
			if (id1 == id2) continue;
			q1[id1]++;
			q2[second]++;
//			q3[id2]++;
		}
		for (int i = 0; i < n; i++) q1[i] = q1[i] * (n + 2) * (n + 2) + q2[i] * (n + 2) + q3[i] + 1;
		sort(q1.begin(), q1.end());
		q1.pb(n);

		long long mod = 1e12 + 7;

		long long ret = 0;
		for (int x : q1) ret = (ret * 1000007 + x) % mod;

		return ret;
	}


	void load(vector<vector<ints> > a) {
		input = a;
		int n = a.size();
		int d = a[0].size();
		p = vector<int>(d);
		for (int i = 0; i < d; i++) p[i] = i;
		for (int i = 0; i < d; i++) swap(p[i], p[rand() % (i + 1)]);

		for (int i = 0; i < n; i++) for (int j = 0; j < d; j++) input[i][j] = a[i][p[j]];

	}

	ints go(vector<vector<ints> > a) override {
		if (a.size() == 0) return 1;
		if (a.size() == 1) return 1;

		make_free(a);
		int d = a[0].size();

		long long h = get_hash(a);
		if (was2[h] > 0) return was[h] / was2[h];

		for (int i = 0; i < (int)a.size(); i++) for (int j = i + 1; j < (int)a.size(); j++) if (a[i][d-1] > a[j][d-1]) swap(a[i], a[j]);


		ints rtime = 0;
		for (int i = 0; i < (int)a.size(); i++) {
			vector<vector<ints> > b;
			for (int j = i + 1; j < (int)a.size(); j++) {
				vector<ints> t = a[j];
				for (int k = 0; k < d; k++) t[k] = min(t[k], a[i][k]);
				t.pop_back();
				b.pb(t);
			}
			rtime += go(b);
		}
		was[h] += rtime;
		was2[h]++;

		return rtime;
		
	}

	vector<ints> get_stat() {
		return vector<ints>(1, worked);
	}
	ints eval() {
		ints t = go(input);
		worked += t;
		return t;
	}

};

struct timer {
	int was;
	void start() {
		was = clock();
	}
	void show() {
		cerr << clock() - was << endl;
	}
	void show(string s) {
		cerr << s << " " << clock() - was << endl;
	}
};

void run(vector<vector<vector<ints> > > &data, vector<algo*> algos) {
	timer T;
	for (algo * A : algos) {
		T.start();

		for (auto test : data) {
			T.start();
			A->load(test);
			cout << (A->eval()) << endl;
		}
		cout << "-----" << endl;				

		//T.show(A->name() + " :");
		//walking_fish * g = static_cast<walking_fish *>(A);
		//cerr << "total calls: " << g->go_cnt << endl;
	}
}

void output(vector<vector<vector<ints> > > &data, string file_name) {
	ofstream file;
	file.open (file_name);
	for (int i = 0; i < (int)data.size(); i++) {
		int n = data[i].size();
		for (int j = 0; j < n; j++) {
			int d = data[i][j].size();
			for (int k = 0; k < d; k++) file << data[i][j][k] << " ";
			file << endl;
		}
		if (i + 1 < data.size()) file << "#" << endl;
	}
	file.close();
}

void permute(vector<vector<vector<ints> > > &data) {
	int d = data[0][0].size();
	int p[d];
	for (int i = 0; i < d; i++) p[i] = i;
	for (int i = 0; i < d; i++) swap(p[i], p[rand() % (i + 1)]);
	for (int i = 0; i < (int)data.size(); i++) for (int k = 0; k < (int)data[i].size(); k++) {
		int d = data[i][k].size();
		vector<ints>temp(d);
		for (int j = 0; j < d; j++) temp[j] = data[i][k][p[j]];
		data[i][k] = temp;
	}
}


void permute(vector<vector<vector<ints> > > &data, vector<int>&p) {
	for (int i = 0; i < (int)data.size(); i++) for (int k = 0; k < (int)data[i].size(); k++) {
		int d = data[i][k].size();
		vector<ints>temp(d);
		for (int j = 0; j < d; j++) temp[j] = data[i][k][p[j]];
		data[i][k] = temp;
	}
}

void modify(vector<vector<vector<ints> > > &data, string name = "") {
	vector<algo*> algos;
	for (int i = 0; i < 25; i++) algos.pb(new wf_map_sort());
	vector<vector<vector<ints> > > data2run;
	for (int i = 0; i < 10; i++) data2run.pb(data[i]);

	output(data, "before" + name + ".txt");
	run(data2run, algos);

	algo * best = NULL;
	for (auto A : algos) {
		cerr << (A->get_stat()[0]) << endl;
		if (best == NULL || (A->get_stat()[0]) < (best->get_stat()[0])) best = A;
	}
	permute(data, (static_cast<wf_map_sort*>(best))->p); 
	output(data, "after" + name + ".txt");

}

void time_old() {
	srand(0);
	vector<vector<vector<ints> > > data;
	int n = 210;
	int d = 10;
	vector<int> g;
	for (int i = 0; i < d; i++) g.pb((int)pow(2, rand() % 8 + 1));
	for (int it = 0; it < 100; it++) {
		int d = 10;
		vector<vector<ints> > a;
		for (int i = 0; i < n; i++) {
			vector<ints> t;
			for (int j = 0; j < d; j++) t.pb(rand() % g[j]);
			a.pb(t);
		}
		algo(d).make_free(a);
		data.pb(a);
	}
	modify(data);
}


void depend() {
	srand(0);
	vector<vector<vector<ints> > > data;
	int n = 180;
	for (int it = 0; it < 100; it++) {
		int d = 14;
		vector<int> g(d, 100);
		vector<vector<ints> > a;
		for (int i = 0; i < n; i++) {
			vector<ints> t;
			for (int j = 0; j < d; j++) t.pb(rand() % g[j]);
			a.pb(t);
		}
		for (int i = 0; i < n; i++) {
			for (int j = d / 2; j < d; j++) {
				double q = 0.99 + (rand() % 2000) / 100000.;
				a[i][j] = (int)(a[i][j - d / 2] * q);
			}
		}


		algo(d).make_free(a);
		data.pb(a);
	}
	permute(data);
	modify(data, "_dep");
}



void time_test2() {
	srand(0);
	vector<vector<vector<ints> > > data;
	int n = 80;
	for (int it = 0; it < 10; it++) {
		int d = 10;
		vector<vector<ints> > a;
		for (int i = 0; i < n; i++) {
			vector<ints> t;
			for (int j = 0; j < d; j++) t.pb(rand() % 100);
			a.pb(t);
		}
		algo(d).make_free(a);

		for (auto x : a) {
			for (auto y : x) cout << y << " ";
			cout << endl;
		}

		data.pb(a);
	}
//	for (int i = 0; i < 1; i++) data.pb(data.back());
	vector<algo*> algos;
//	algos.pb(new wf_map_sort());
	algos.pb(new wf_take_smallest());
	algos.pb(new wf_presort());
	algos.pb(new wf_sort());
	algos.pb(new wf_estimate());
//	algos.pb(new wf_map_estimate());
//	algos.pb(new wf_map_predictor());
	algos.pb(new wf_smallest_volume());
//	algos.pb(new wf_smallest_and_volume());
	algos.pb(new walking_fish());
	algos.pb(new hso_into_wf());


	timer T;
	for (algo * A : algos) {
		T.start();

		int last = 0;

		for (auto test : data) {
//			T.start();
			A->load(test);
			A->eval();
/*			cerr << (A->eval()) << endl;

			T.show(A->name() + " :");
			walking_fish * g = static_cast<walking_fish *>(A);
			cerr << "total calls: " << (g->go_cnt) - last << endl;
			last = (g->go_cnt);*/
		}				

		T.show(A->name() + " :");
		walking_fish * g = static_cast<walking_fish *>(A);
		cerr << "total calls: " << g->go_cnt << endl;
	}
}


int main(){
	#ifdef home
		freopen(TASK".in","r",stdin);	
		freopen(TASK".out","w",stdout);
	#endif		
	ios::sync_with_stdio(false);
	time_old();
//	depend();
//	time_test2();
//	cout << algo(a).eval() << endl;
//	cout << inc_excl(a).eval() << endl;
//	cout << walking_fish(a).eval() << endl;
	return 0;
}





