#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

const char* PAUSE = "pause";


int main(int argc, char* argv[]) {

	// argc�̐������J��Ԃ�
	for (int i = 0; i < argc; i++) {
		// ������argv��i�Ԗڂ�\��
		cout << argv[i] << endl;
	}

	system(PAUSE); // ����O�Ƀ|�[�Y���Ă���
	return 0;
}