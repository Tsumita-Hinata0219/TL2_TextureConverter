#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <Windows.h>

#include "../../Externals/DirectXTex/DirectXTex.h"


/* �e�N�X�`���R���o�[�^�N���X */
class TextureConverter {

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TextureConverter() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TextureConverter() = default;

	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�����
	/// </summary>
	/// <param name="filePath"> �t�@�C���p�X </param>
	void ConvertTextureWIC_To_DDS(const std::string& filePath, int numOptions = 0, char* options[] = nullptr);

	/// <summary>
	/// �g�p���@���o�͂���(�\��)����
	/// </summary>
	static void OutputUsage();

	
private:

	/// <summary>
	/// �e�N�X�`���t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="filePath"> �t�@�C���p�X </param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="mString"> �}���`�o�C�g������ </param>
	/// <returns> ���C�h������ </returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="filePath"></param>
	void SeparateFilePath(const std::wstring& filePath);

	/// <summary>
	/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	/// </summary>
	void SaveDDSTextureToFile(int numOptions, char* options[], const std::wstring& outputDirectory);


private:

	// �摜�̏��
	DirectX::TexMetadata metaData_{};
	// �摜�̃C���[�W�R���e�i
	DirectX::ScratchImage scratchImage_{};

	// �f�B���N�g��
	std::wstring directoryPath_{};
	// �t�@�C��
	std::wstring fileName_{};
	// �t�@�C���g���q
	std::wstring fileExt_{};

};

