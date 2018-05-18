#include<bits/stdc++.h>
#define mp make_pair
#define pb push_back      
#define F first
#define S second

typedef int ints;

#define TASK "1"

using namespace std;


struct algo {
	vector<vector<ints> > input;
	int d;
	int n;

	algo() {
	}
	algo(int dd): d(dd){
	}
	algo(vector<vector<ints> > a): input(a) {
		n = a.size();
		if (n > 0) d = a[0].size();
	}
	void load(vector<vector<ints> > a) {
		n = a.size();
		if (n > 0) d = a[0].size();
		input = a;
	}
	virtual ints eval() {
		return 0;
	}
	virtual string name() {return "#";}
	void to_min(vector<ints> & a, vector<ints> & b) {
		if (a.size() == 0) a = b; else {
			for (int i = 0; i < d; i++) a[i] = min(a[i], b[i]);
		}
	}
	int less_or_equal(vector<ints> & a, vector<ints> & b) {
		for (int i = 0; i < d; i++) if (a[i] > b[i]) return 0;
		return 1;
	}


	ints get_volume(vector<ints> & a) {
		ints ret = 1;
		for (ints x : a) ret *= x;
		return ret;
	}


	void make_free(vector<vector<ints> > & a) {
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
	ints go(vector<vector<ints> > a) {
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
};

struct wf_take_smallest : walking_fish {

	wf_take_smallest() : walking_fish() {}
	wf_take_smallest(int d) : walking_fish(d) {}
	wf_take_smallest(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_take_smallest";}


	int make_choice(vector<vector<ints> > &a) override {
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

struct wf_smallest_and_volume : walking_fish {

	wf_smallest_and_volume() : walking_fish() {}
	wf_smallest_and_volume(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_smallest_and_volume";}


	int make_choice(vector<vector<ints> > &a) override {
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




struct wf_estimate : walking_fish {

	wf_estimate() : walking_fish() {}
	wf_estimate(vector<vector<ints> > a) : walking_fish(a) {}
	virtual string name() {return "wf_estimate";}


	double estimate(vector<vector<ints> > &a) {
		if (a.size() <= 1) return 1;
		make_free(a);
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
		options.pb(wf_take_smallest(d).make_choice(a));

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
		if (n < 24) return wf_take_smallest(d).make_choice(a);
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

void time_test2() {
	srand(0);
	vector<vector<vector<ints> > > data;
	int n = 30;
	for (int it = 0; it < 100; it++) {
		int d = 10;
		vector<vector<ints> > a;
		for (int i = 0; i < n; i++) {
			vector<int> t;
			for (int j = 0; j < d; j++) t.pb(rand() % 100);
			a.pb(t);
		}
		algo(d).make_free(a);
		data.pb(a);
	}
	vector<algo*> algos;
	algos.pb(new wf_take_smallest());
	algos.pb(new wf_estimate());
	algos.pb(new wf_smallest_volume());
	algos.pb(new wf_smallest_and_volume());
	algos.pb(new walking_fish());

	timer T;
	for (algo * A : algos) {
		T.start();

		for (auto test : data) {
			A->load(test);
			A->eval();
		}				

		T.show(A->name() + " :");
		walking_fish * g = static_cast<walking_fish *>(A);
		cerr << "total calls: " << g->go_cnt << endl;
	}
}

/*
void time_test() {
	timer T;
	srand(0);
	int n = 32;
	for (int it = 0; it < 1; it++) {
		int d = 18 + it / 3;

		vector<vector<ints> > a;
		for (int i = 0; i < n; i++) {
			vector<int> t;
			for (int j = 0; j < d; j++) t.pb(rand() % 100);
			a.pb(t);
		}
		algo(d).make_free(a);
		cerr << "---------------" << endl;
		cerr << "iteration : " << it << endl;
		cerr << "n : " << a.size() << endl;
		cerr << "d : " << d << endl;

		T.start();
		ints r2 =  walking_fish(a).eval();
		T.show("fish time: ");


		T.start();

		auto A1 = wf_take_smallest(a);
		ints r1 =  A1.eval();
		T.show("smallest time: ");
		cerr << "cnt1: " << A1.go_cnt << endl;

		T.start();
		auto A2 = wf_estimate(a);
		ints r3 =  A2.eval();
		T.show("estimate time: ");
		cerr << "cnt1: " << A2.go_cnt << endl;


		T.start();
		auto A3 = wf_smallest_volume(a);
		ints r4 =  A3.eval();
		T.show("volume time: ");
		cerr << "cnt1: " << A3.go_cnt << endl;

		T.start();
		auto A4 = walking_fish(a);
		ints r5 =  A4.eval();
		T.show("last time: ");
		cerr << "cnt1: " << A4.go_cnt << endl;





//		T.start();
//		ints r1 =  inc_excl(a).eval();
//		ints r1 =  r2;
//		T.show("inc excl time: ");


//		assert(r1 == r2);

	}
}*/


int main(){
	#ifdef home
		freopen(TASK".in","r",stdin);	
		freopen(TASK".out","w",stdout);
	#endif		
	ios::sync_with_stdio(false);
	int n, d;
	cin >> n >> d;
	vector<vector<int> > a(n);
	for (int i = 0; i < n; i++) {
		vector<int> b(d);
		for (int j = 0; j < d; j++) cin >> b[j];
		a[i] = b;
	}
	time_test2();
//	cout << algo(a).eval() << endl;
//	cout << inc_excl(a).eval() << endl;
//	cout << walking_fish(a).eval() << endl;
	return 0;
}





