#include "TextureConverter.h"



/// <summary>
/// テクスチャをWICからDDSに変換する
/// </summary>
void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath)
{
	// ───── 1 テクスチャを読み込む
	LoadWICTextureFromFile(filePath);

	// ───── 2 DDS形式に変換して書き出す


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

