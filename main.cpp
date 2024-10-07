#include <cstdio>
#include <cstdlib>
#include <iostream>

const char* PAUSE = "pause";


int main(int argc, char* argv[]) {

	// argcの数だけ繰り返す
	for (int i = 0; i < argc; i++) {
		// 文字列argvのi番目を表示
		std::cout << argv[i] << std::endl;
	}


	std::cout << std::endl; // 改行
	system(PAUSE); // 閉じる前にポーズしておく
	return 0;
}