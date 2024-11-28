#pragma once

#include <iostream>


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

};

