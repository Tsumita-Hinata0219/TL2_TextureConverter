#pragma once

#include <iostream>

#include "../../Externals/DirectXTex/DirectXTex.h"


/* テクスチャコンバータクラス */
class TextureConverter {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureConverter() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureConverter() = default;

	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath"> ファイルパス </param>
	void ConvertTextureWIC_To_DDS(const std::string& filePath);

	
private:

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath"> ファイルパス </param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString"> マルチバイト文字列 </param>
	/// <returns> ワイド文字列 </returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);


private:

	// 画像の情報
	DirectX::TexMetadata metaData_;
	// 画像のイメージコンテナ
	DirectX::ScratchImage scratchImage_;

	// ディレクトリ
	std::wstring directoryPath_;
	// ファイル
	std::wstring fileName_;
	// ファイル拡張子
	std::wstring fileExt_;

};

