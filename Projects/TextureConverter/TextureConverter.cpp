#include "TextureConverter.h"



/// <summary>
/// �e�N�X�`����WIC����DDS�ɕϊ�����
/// </summary>
void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath)
{
	// ���������� 1 �e�N�X�`����ǂݍ���
	LoadWICTextureFromFile(filePath);

	// ���������� 2 DDS�`���ɕϊ����ď����o��
	SaveDDSTextureToFile();
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

	// ���������� �t�H���_�p�X�ƃt�@�C�����𕪗�����
	SeparateFilePath(wFilePath);
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


/// <summary>
/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
/// </summary>
void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1 = 0;
	std::wstring exceptExt;

	// ��؂蕶�� '.' ���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');
	// �������q�b�g������
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// ��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}
	// ��؂蕶�� '\\' ���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// ��؂蕶�� '/' ���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// ��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
		directoryPath_ = L"";
		fileName_ = exceptExt;
	}
}


/// <summary>
/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
/// </summary>
void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT hr{};
	// �㉺���]�p�� ScratchImage ��p��
	DirectX::ScratchImage flippedImage;
	HRESULT flipResult = DirectX::FlipRotate(
		*scratchImage_.GetImage(0, 0, 0),
		DirectX::TEX_FR_FLIP_HORIZONTAL | 
		DirectX::TEX_FR_FLIP_VERTICAL, // X���EY���������]
		flippedImage);
	if (SUCCEEDED(hr)) {
		// �C���[�W�ƃ��^�f�[�^���A�~�b�v�}�b�v�łŒu��������
		scratchImage_ = std::move(flippedImage);
		metaData_ = scratchImage_.GetMetadata();
	}

	// �~�b�v�}�b�v����
	DirectX::ScratchImage mipChain;
	hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 
		0, mipChain);
	if (SUCCEEDED(hr)) {
		// �C���[�W�ƃ��^�f�[�^���A�~�b�v�}�b�v�łŒu��������
		scratchImage_ = std::move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	// ���k�`���ɕϊ�
	DirectX::ScratchImage converted;
	hr = DirectX::Compress(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), 
		metaData_, 
		DXGI_FORMAT_BC7_UNORM_SRGB, 
		DirectX::TEX_COMPRESS_BC7_QUICK | 
		DirectX::TEX_COMPRESS_SRGB_OUT | 
		DirectX::TEX_COMPRESS_PARALLEL, 
		1.0f, converted);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}

	// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	// �o�̓t�@�C������ݒ肷��
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDS�t�@�C�������o��
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}

