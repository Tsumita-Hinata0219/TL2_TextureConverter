#include "TextureConverter.h"
using namespace std;


/// <summary>
/// �e�N�X�`����WIC����DDS�ɕϊ�����
/// </summary>
void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath, int numOptions, char* options[])
{
	// ���������� exe�̃f�B���N�g�����擾���� ����������
	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH); // exe�t�@�C���̐�΃p�X���擾
	filesystem::path exeDirectory = filesystem::path(exePath).remove_filename();
	// �o�̓f�B���N�g������ݒ� (exe�Ɠ����f�B���N�g������"Converted"�t�H���_���쐬)
	wstring outputDirectory = exeDirectory / L"Converted";
	// �o�̓f�B���N�g�����쐬
	namespace fs = filesystem;
	if (!fs::exists(outputDirectory)) {
		fs::create_directories(outputDirectory); // �f�B���N�g�����ċA�I�ɍ쐬
	}


	// ���������� 1 ���C�h������ւ̕ϊ��ƕ���
	ConvertAndSeparateFilePath(filePath);

	// ���������� 2 �e�N�X�`����ǂݍ���
	LoadWICTextureFromFile();

	// ���������� 3 DDS�`���ɕϊ����ď����o��
	SaveDDSTextureToFile(numOptions, options, outputDirectory);
}


/// <summary>
/// �g�p���@���o�͂���(�\��)����
/// </summary>
void TextureConverter::OutputUsage()
{
	cout << "�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂�" << endl;
	cout << "�t�@�C�����w�肵�Ă�������" << endl;
	cout << "TextureConverter [�h���C�u:][�p�X][�t�@�C����]" << endl;
	cout << endl;
}


/// <summary>
/// ���C�h������ւ̕ϊ��ƃX����
/// </summary>
void TextureConverter::ConvertAndSeparateFilePath(const std::string& filePath)
{
	// ���������� �t�@�C���p�X�����C�h������ɕϊ�
	wFilePath_ = ConvertMultiByteStringToWideString(filePath);

	// ���������� �t�H���_�p�X�ƃt�@�C�����𕪗�����
	SeparateFilePath();

	// ���������� �������ꂽ�p�X�����o��
	OutputSeparatedPathInfo();
}


/// <summary>
/// �e�N�X�`���t�@�C���̓ǂݍ���
/// </summary>
void TextureConverter::LoadWICTextureFromFile()
{
	// ���������� �e�N�X�`����ǂݍ���
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath_.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));
}


/// <summary>
/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
/// </summary>
void TextureConverter::SeparateFilePath()
{
	size_t pos1 = 0;
	wstring exceptExt;

	// ��؂蕶�� '.' ���o�Ă����ԍŌ�̕���������
	pos1 = wFilePath_.rfind('.');
	// �������q�b�g������
	if (pos1 != wstring::npos) {
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt_ = wFilePath_.substr(pos1 + 1, wFilePath_.size() - pos1 - 1);
		// ��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = wFilePath_.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = wFilePath_;
	}
	// ��؂蕶�� '\\' ���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('\\');
	if (pos1 != wstring::npos) {
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// ��؂蕶�� '/' ���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	if (pos1 != wstring::npos) {
		// ��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
		directoryPath_ = L"";
		fileName_ = exceptExt;
	}
}


/// <summary>
/// �������ꂽ�p�X�����o��
/// </summary>
void TextureConverter::OutputSeparatedPathInfo()
{
	cout << "�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂�" << endl;
	cout << endl;

	// wstring ���}���`�o�C�g������ɕϊ�����֐�
	auto WideStringToMultiByte = [](const std::wstring& wideString) -> std::string {
		if (wideString.empty()) return std::string();

		// �K�v�ȃo�b�t�@�T�C�Y���v�Z
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (bufferSize <= 0) return std::string();

		// �o�b�t�@���m�ۂ��ĕϊ�
		std::string multiByteString(bufferSize, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &multiByteString[0], bufferSize, nullptr, nullptr);

		// �I�[�������폜���ĕԂ�
		multiByteString.pop_back(); // Null terminator ���폜
		return multiByteString;
		};

	// �e�ϐ��� UTF-8 �ɕϊ����� cout �ŏo��
	cout << "���͂��ꂽ�t�@�C���p�X : " << WideStringToMultiByte(wFilePath_) << endl;
	cout << "�f�B���N�g���p�X       : " << WideStringToMultiByte(directoryPath_) << endl;
	cout << "�t�@�C����             : " << WideStringToMultiByte(fileName_) << endl;
	cout << "�t�@�C���g���q         : " << WideStringToMultiByte(fileExt_) << endl;
	cout << endl;

	// �o�͏ꏊ�̕\��
	cout << "'Converted'�ɏo�͂���܂�" << endl;
	// �����̐i�s�󋵂̃T���v���o��
	cout << "[�i�s��] �ϊ���..." << endl;
}


/// <summary>
/// �}���`�o�C�g����������C�h������ɕϊ�
/// </summary>
std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ���C�h������ɕϊ������ۂ̕��������v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ���C�h����
	wstring wString;
	wString.resize(filePathBufferSize);

	// ���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}


/// <summary>
/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
/// </summary>
void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[], const std::wstring& outputDirectory)
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
		scratchImage_ = move(flippedImage);
		metaData_ = scratchImage_.GetMetadata();
	}

	// �~�b�v�}�b�v����
	// �~�b�v�}�b�v���x���w�������
	size_t mipLevel = 0;
	for (int i = 0; i < numOptions; i++) {
		if (string(options[i]) == "-ml") {
			// �~�b�v���x���w��
			mipLevel = stoi(options[i + 1]);
			break;
		}
	}
	DirectX::ScratchImage mipChain;
	hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(), 
		scratchImage_.GetImageCount(), 
		scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 
		mipLevel, 
		mipChain);
	if (SUCCEEDED(hr)) {
		// �C���[�W�ƃ��^�f�[�^���A�~�b�v�}�b�v�łŒu��������
		scratchImage_ = move(mipChain);
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
		scratchImage_ = move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}

	// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	// �o�̓t�@�C������ݒ肷��
	wstring filePath = outputDirectory + L"\\" + fileName_ + L".dds";

	// DDS�t�@�C�������o��
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}

