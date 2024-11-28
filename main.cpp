#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

const char* PAUSE = "pause";


int main(int argc, char* argv[]) {

	// argcの数だけ繰り返す
	for (int i = 0; i < argc; i++) {
		// 文字列argvのi番目を表示
		cout << argv[i] << endl;
	}

	system(PAUSE); // 閉じる前にポーズしておく
	return 0;
}