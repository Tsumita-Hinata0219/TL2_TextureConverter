#include <cstdio>
#include <cstdlib>
#include <iostream>

const char* PAUSE = "pause";


int main(int argc, char* argv[]) {

	// argc�̐������J��Ԃ�
	for (int i = 0; i < argc; i++) {
		// ������argv��i�Ԗڂ�\��
		std::cout << argv[i] << std::endl;
	}


	std::cout << std::endl; // ���s
	system(PAUSE); // ����O�Ƀ|�[�Y���Ă���
	return 0;
}