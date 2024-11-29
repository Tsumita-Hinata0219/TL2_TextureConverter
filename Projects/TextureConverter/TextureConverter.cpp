#include "TextureConverter.h"
#include <filesystem>
#include <Windows.h>


/// <summary>
/// テクスチャをWICからDDSに変換する
/// </summary>
void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath)
{
	// ───── exeのディレクトリを取得する ─────
	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH); // exeファイルの絶対パスを取得
	std::filesystem::path exeDirectory = std::filesystem::path(exePath).remove_filename();
	// 出力ディレクトリ名を設定 (exeと同じディレクトリ内に"Converted"フォルダを作成)
	std::wstring outputDirectory = exeDirectory / L"Converted";
	// 出力ディレクトリを作成
	namespace fs = std::filesystem;
	if (!fs::exists(outputDirectory)) {
		fs::create_directories(outputDirectory); // ディレクトリを再帰的に作成
	}


	// ───── 1 テクスチャを読み込む
	LoadWICTextureFromFile(filePath);

	// ───── 2 DDS形式に変換して書き出す
	SaveDDSTextureToFile(outputDirectory);
}


/// <summary>
/// 使用方法を出力する(表示)する
/// </summary>
void TextureConverter::OutputUsage()
{
	std::cout << "画像ファイルをWIC形式からDDS形式に変換します" << std::endl;
	std::cout << "ファイルを指定してください" << std::endl;
	std::cout << "TextureConverter [ドライブ:][パス][ファイル名]" << std::endl;
	std::cout << std::endl;
}


/// <summary>
/// テクスチャファイルの読み込み
/// </summary>
void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// ───── 1 ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	// ───── 2 テクスチャを読み込む
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));

	// ───── フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);
}


/// <summary>
/// マルチバイト文字列をワイド文字列に変換
/// </summary>
std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ワイド文字
	std::wstring wString;
	wString.resize(filePathBufferSize);

	// ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}


/// <summary>
/// フォルダパスとファイル名を分離する
/// </summary>
void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1 = 0;
	std::wstring exceptExt;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// 区切り文字がないのでファイル名のみとして扱う
		directoryPath_ = L"";
		fileName_ = exceptExt;
	}
}


/// <summary>
/// DDSテクスチャとしてファイル書き出し
/// </summary>
void TextureConverter::SaveDDSTextureToFile(const std::wstring& outputDirectory)
{
	HRESULT hr{};
	// 上下反転用の ScratchImage を用意
	DirectX::ScratchImage flippedImage;
	HRESULT flipResult = DirectX::FlipRotate(
		*scratchImage_.GetImage(0, 0, 0),
		DirectX::TEX_FR_FLIP_HORIZONTAL | 
		DirectX::TEX_FR_FLIP_VERTICAL, // X軸・Y軸両方反転
		flippedImage);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(flippedImage);
		metaData_ = scratchImage_.GetMetadata();
	}

	// ミップマップ生成
	DirectX::ScratchImage mipChain;
	hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 
		0, mipChain);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	// 圧縮形式に変換
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

	// 読み込んだテクスチャをSRGBとして扱う
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	// 出力ファイル名を設定する
	std::wstring filePath = outputDirectory + L"\\" + fileName_ + L".dds";

	// DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}

