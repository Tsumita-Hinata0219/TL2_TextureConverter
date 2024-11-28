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



};

