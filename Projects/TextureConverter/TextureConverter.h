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



};
