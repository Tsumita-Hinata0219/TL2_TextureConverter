#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <Windows.h>

#include "Projects/TextureConverter/TextureConverter.h"

using namespace std;

const char* PAUSE = "pause";

// �R�}���h���C������
enum Argment {
	KApplicationPath, // �A�v���P�[�V�����̃p�X
	kFilePath,		  // �n���ꂽ�t�@�C���̃p�X

	NumArgment,
};


int main(int argc, char* argv[]) {

	// �t�@�C�����w�肳��Ă�����R���o�[�g�����ɓ���
	if (argc >= NumArgment) {
		// COM ���C�u�����̏�����
		HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
		assert(SUCCEEDED(hr));

		// �e�N�X�`���R���o�[�^�N���X�̃C���X�^���X
		std::unique_ptr<TextureConverter> texConverter =
			std::make_unique<TextureConverter>();
		// �e�N�X�`���ϊ�
		texConverter->ConvertTextureWIC_To_DDS(argv[kFilePath]);

		// COM ���C�u�����̏I��
		CoUninitialize();
	}
	else {
		cout << "�t�@�C�����w�肵�Ă�������" << endl;
		system(PAUSE); // ����O�Ƀ|�[�Y���Ă���
	}

	//system(PAUSE); // ����O�Ƀ|�[�Y���Ă���
	return 0;
}