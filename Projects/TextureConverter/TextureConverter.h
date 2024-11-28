#pragma once

#include <iostream>


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
	void ConvertTextureWIC_To_DDS(const std::string& filePath);


	
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

};

