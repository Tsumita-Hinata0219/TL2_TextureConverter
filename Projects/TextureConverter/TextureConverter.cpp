#include "TextureConverter.h"



/// <summary>
/// �e�N�X�`����WIC����DDS�ɕϊ�����
/// </summary>
void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath)
{
	// ���������� 1 �e�N�X�`����ǂݍ���
	LoadWICTextureFromFile(filePath);

	// ���������� 2 DDS�`���ɕϊ����ď����o��


}


/// <summary>
/// �e�N�X�`���t�@�C���̓ǂݍ���
/// </summary>
void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// ���������� 1 �t�@�C���p�X�����C�h������ɕϊ�
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	// ���������� 2 �e�N�X�`����ǂݍ���
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));
}


/// <summary>
/// �}���`�o�C�g����������C�h������ɕϊ�
/// </summary>
std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ���C�h������ɕϊ������ۂ̕��������v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ���C�h����
	std::wstring wString;
	wString.resize(filePathBufferSize);

	// ���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

